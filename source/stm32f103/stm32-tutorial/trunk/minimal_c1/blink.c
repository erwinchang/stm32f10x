#define GPIOD_CRH   (*((volatile unsigned long*)(0x40011400 + 0x4)))
#define GPIOD_BSRR  (*((volatile unsigned long*)(0x40011400 + 0x10)))
#define RCC_APB2ENR (*((volatile unsigned long*)(0x40021000 + 0x18)))
        
__asm__(".word 0x20001000");
__asm__(".word main");
main()
{
  unsigned int c = 0;

  RCC_APB2ENR = (1 << 5);  //IOPDEN = 1
  GPIOD_CRH = 0x44444414;
  
  while(1) {
    GPIOD_BSRR = (1 << 25);  // ON
    for(c = 0; c < 100000; c++);
    GPIOD_BSRR = (1 << 9);   // OFF
    for(c = 0; c < 100000; c++);
  }  
}
