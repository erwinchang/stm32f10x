/* 
   The hardware of this program is STRIVE V2.1
   PB5 - LED1
   PD6 - LED2
   PD3 - LED3
 */

#define RCC_BASE    0x40021000
#define RCC_CR      (*((volatile unsigned int*)(RCC_BASE + 0x0)))
#define RCC_CFGR    (*((volatile unsigned int*)(RCC_BASE + 0x04)))
#define RCC_CIR     (*((volatile unsigned int*)(RCC_BASE + 0x08)))

#define RCC_APB2ENR (*((volatile unsigned int*)(RCC_BASE + 0x18)))

#define FLASH_BASE  0x40022000
#define FLASH_ACR   (*((volatile unsigned int*)(FLASH_BASE)))

#define GPIOB_CRL   (*((volatile unsigned int*)(0x40010c00)))
#define GPIOB_BSRR  (*((volatile unsigned int*)(0x40010c10)))

#define GPIOD_CRL   (*((volatile unsigned int*)(0x40011400))) 
#define GPIOD_BSRR  (*((volatile unsigned int*)(0x40011410))) 

#define GPIOA_CRH   (*((volatile unsigned int*)(0x40010804)))

#define USART1_BASE 0x40013800
#define USART1_SR   (*((volatile unsigned int*)(USART1_BASE + 0x00)))
#define USART1_DR   (*((volatile unsigned int*)(USART1_BASE + 0x04)))
#define USART1_BRR  (*((volatile unsigned int*)(USART1_BASE + 0x08)))
#define USART1_CR1  (*((volatile unsigned int*)(USART1_BASE + 0x0c)))
#define USART1_CR2  (*((volatile unsigned int*)(USART1_BASE + 0x10)))
#define USART1_CR3  (*((volatile unsigned int*)(USART1_BASE + 0x14)))

void clock_init(void)
{
  RCC_CR |= (1<<16); //enable HSE
  while (!(RCC_CR & (1<<17))); 
 
  // If you want to read/write Flash, the FLASH_ACR should be changed
  //FLASH_ACR = 0x32; //Prefetch buffer enabled, LATENCY = 0b010

  // PLLMUL = 0b0111; PLL input clock x 9
  // PLLXTPRE = 0; HSE clock not divided
  // PLLSRC = 1; HSE oscillator clock selected as PLL input clock
  // PPRE2 = 0b000; PCLK2 = HCLK
  // PPRE1 = 0b100; PCLK1 = HCLK/2 (PCLK1 should not exceed 36MHz)
  // HPRE = 0b0000; AHB clock = SYSCLK
  // SW = 0b00; HSI selected as system clock
  RCC_CFGR = 0x001D0400;
  
  RCC_CR |= (1<<24); // enable PLL
  while (!(RCC_CR & (1<<25))); 

  // SW = 0b10; PLL selected as system clock
  RCC_CFGR = 0x001D0402;
  
  // wait until PLL is used as system clock
  while ((RCC_CFGR & 0xc) != 0x8);
}

void uart1_start(void)
{
  RCC_APB2ENR |= ((1<<14) | (1<<2));

  GPIOA_CRH = 0x444444b4;

  USART1_CR1 |= (1<<13);
  USART1_CR1 &= ~(1<<12);
  USART1_CR2 &= 0xcfff;
  USART1_BRR = 0x271;  //bps = 115200;
  USART1_CR1 |= (1<<3);
}

void uart1_send(char c)
{
  USART1_DR = c;
  while (!(USART1_SR & (1<<7)));
}

void uart1_stop(void)
{
  while (!(USART1_SR & (1<<6)));
}

void print(char *str, int n)
{
  int i;
  for (i = 0; i < n; i++) {
    uart1_send(str[i]);
  }  
}

void gpiod_init(void)
{
  RCC_APB2ENR |= (1<<3);  // Enable IOPB
  RCC_APB2ENR |= (1<<5);  // Enable IOPD
  
  GPIOB_CRL = 0x44344444; // PB5 -> output
  GPIOD_CRL = 0x43443444; // PD6, PD3 -> output
}

void blink(char leds[][3], int i)
{
  if (leds[i][0] == 0) {
    GPIOB_BSRR = (1<<5);
  }
  else {
    GPIOB_BSRR = (1<<21);
  }
  
  if (leds[i][1] == 0) {
    GPIOD_BSRR = (1<<6);
  }
  else {
    GPIOD_BSRR = (1<<22);
  }
  
  if (leds[i][2] == 0) {
    GPIOD_BSRR = (1<<3);
  }
  else {
    GPIOD_BSRR = (1<<19);
  }
}

void delay(void) 
{ 
  unsigned int i; 
  for(i = 0; i < 0x3ffff; ++i) {
    asm("nop"); 
  }
} 

char blink_leds[6][3] = {
  {0, 0, 0},
  {0, 0, 1},
  {0, 1, 1},
  {1, 1, 1},
  {1, 1, 0},
  {1, 0, 0},
};

char hello[] = "hello world!\r";

void main(void) 
{
  int i;

  clock_init();
  gpiod_init();
  uart1_start();
  
  while(1){
    for (i = 0; i < 6; i++) {
      blink(blink_leds, i);
      
      if (i % 3 == 0) {
        uart1_send('/');
      }
      else if (i % 3 == 1) {
        uart1_send('-');
      }
      else {
        uart1_send('|');
      }
    
      print(hello, sizeof(hello));

      delay();
    }
  }
}
