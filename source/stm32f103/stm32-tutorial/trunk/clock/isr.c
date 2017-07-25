extern int main(void);

void ResetISR(void); 
void NMIException(void); 
void HardFaultException(void); 

typedef void (*pfnISR)(void);    // Pointer to exception handle function 

__attribute__ ((section(".isr_vector")))
pfnISR VectorTable[] =  
{ 
  (pfnISR)(0x20010000),  // The initial stack pointer is the top of SRAM 
  ResetISR,              // The reset handler 
  NMIException, 
  HardFaultException 
}; 

//***************************************************************************** 
// 
// The following are constructs created by the linker, indicating where the 
// the "data" and "bss" segments reside in memory.  The initializers for the 
// for the "data" segment resides immediately following the "text" segment. 
// 
//***************************************************************************** 
extern unsigned long _eisr_vector;
extern unsigned long _text;
extern unsigned long _etext; 
extern unsigned long _data; 
extern unsigned long _edata; 
extern unsigned long _bss; 
extern unsigned long _ebss; 

void ResetISR(void) 
{ 
  unsigned long *src, *dst; 

  // copy the text segment from flash to SRAM
  src = &_eisr_vector;
  dst = &_text;
  while (dst < &_etext) {
    *dst++ = *src++;
  }
  
  // Copy the data segment initializers from flash to SRAM. 
  dst = &_data;
  while (dst < &_edata) {
    *dst++ = *src++;
  }
  
  // Zero fill the bss segment. 
  for(dst = &_bss; dst < &_ebss; dst++) { 
    *dst = 0; 
  } 
  
  // Call the application's entry point. 
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
