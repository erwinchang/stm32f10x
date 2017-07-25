#define GPIOD_CRH   (*((volatile unsigned int*)(0x40011404))) 
#define GPIOD_BSRR  (*((volatile unsigned int*)(0x40011410))) 
#define RCC_APB2ENR (*((volatile unsigned int*)(0x40021018))) 

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
