#define RCC_APB2ENR  (*((volatile unsigned int*)(0x40021018))) 

#define GPIOA_CRH    (*((volatile unsigned int*)(0x40010804)))

#define USART1_BASE  0x40013800
#define USART1_SR   (*((volatile unsigned int*)(USART1_BASE + 0x00)))
#define USART1_DR   (*((volatile unsigned int*)(USART1_BASE + 0x04)))
#define USART1_BRR  (*((volatile unsigned int*)(USART1_BASE + 0x08)))
#define USART1_CR1  (*((volatile unsigned int*)(USART1_BASE + 0x0c)))
#define USART1_CR2  (*((volatile unsigned int*)(USART1_BASE + 0x10)))
#define USART1_CR3  (*((volatile unsigned int*)(USART1_BASE + 0x14)))

void uart1_start(void)
{
  RCC_APB2ENR |= ((1<<14) | (1<<2));

  GPIOA_CRH = 0x444444b4;

  USART1_CR1 |= (1<<13);
  USART1_CR1 &= ~(1<<12);
  USART1_CR2 &= 0xcfff;
  USART1_BRR = 0x341;  //bps = 9600;
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


void delay(void) 
{ 
  unsigned int i; 
  for(i = 0; i < 0x3ffff; ++i) {
    asm("nop"); 
  }
} 

char hello[] = "hello world!\n\r";

int main(void) 
{
  int i;
  
  uart1_start();
  delay();

  while (1) {
    for (i = 0; i < sizeof(hello); i++) {
      uart1_send(hello[i]);
    }

    delay();
  }
}
