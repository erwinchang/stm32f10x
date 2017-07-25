#define GPIOD_CRH   (*((volatile unsigned int*)(0x40011404))) 
#define GPIOD_BSRR  (*((volatile unsigned int*)(0x40011410))) 
#define RCC_APB2ENR (*((volatile unsigned int*)(0x40021018))) 

void ResetISR(void); 
void NMIException(void); 
void HardFaultException(void); 
void delay(void); 

typedef void (*pfnISR)(void);    // Pointer to exception handle function 

__attribute__ ((section(".isr_vector")))
pfnISR VectorTable[] =  
{ 
  (pfnISR)(0x20010000),  // The initial stack pointer is the top of SRAM 
  ResetISR,              // The reset handler 
  NMIException, 
  HardFaultException 
}; 

void delay(void) 
{ 
  unsigned int i; 
  for(i = 0; i < 0x3ffff; ++i) {
    asm("nop"); 
  }
} 

char leds[6][3] = {
  {0, 0, 0},
  {0, 0, 1},
  {0, 1, 1},
  {1, 1, 1},
  {1, 1, 0},
  {1, 0, 0},
};

int main(void) 
{
  int i;

  RCC_APB2ENR |= (1<<5); 
  GPIOD_CRH = 0x44444111; 
  
  while(1){
    for (i = 0; i < 6; i++) {
      if (leds[i][0] == 0) {
        GPIOD_BSRR = (1<<8);
      }
      else {
        GPIOD_BSRR = (1<<24);
      }
      
      if (leds[i][1] == 0) {
        GPIOD_BSRR = (1<<9);
      }
      else {
        GPIOD_BSRR = (1<<25);
      }

      if (leds[i][2] == 0) {
        GPIOD_BSRR = (1<<10);
      }
      else {
        GPIOD_BSRR = (1<<26);
      }

      delay();
    }
  }
}

//***************************************************************************** 
// 
// The following are constructs created by the linker, indicating where the 
// the "data" and "bss" segments reside in memory.  The initializers for the 
// for the "data" segment resides immediately following the "text" segment. 
// 
//***************************************************************************** 
extern unsigned long _etext; 
extern unsigned long _data; 
extern unsigned long _edata; 
extern unsigned long _bss; 
extern unsigned long _ebss; 

void ResetISR(void) 
{ 
  unsigned long *src, *dst; 
  
  // 
  // Copy the data segment initializers from flash to SRAM. 
  // 
  src = &_etext;
  dst = &_data;
  while (dst < &_edata) {
    *dst++ = *src++;
  }
  
  // 
  // Zero fill the bss segment. 
  // 
  for(dst = &_bss; dst < &_ebss; dst++) { 
    *dst = 0; 
  } 
  
  // 
  // Call the application's entry point. 
  // 
  main(); 
} 

void NMIException(void) 
{ 
  return; 
} 

void HardFaultException(void) 
{ 
  return; 
}

