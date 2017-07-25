
#include "stm32f10x.h"

GPIO_InitTypeDef GPIO_InitStructure;

void Delay(__IO uint32_t nCount);

char blink_leds[6][3] = {
  {0, 0, 0},
  {1, 0, 0},
  {1, 1, 0},
  {1, 1, 1},
  {0, 1, 1},
  {0, 0, 1},
};

typedef enum {
  LED1 = 0,
  LED2 = 1,
  LED3 = 2,
} Led_TypeDef;

#define LEDn 3
/*
  PB5 - LED1
  PD6 - LED2
  PD3 - LED3
*/
#define LED1_PIN                         GPIO_Pin_5
#define LED1_GPIO_PORT                   GPIOB
#define LED1_GPIO_CLK                    RCC_APB2Periph_GPIOB

#define LED2_PIN                         GPIO_Pin_6
#define LED2_GPIO_PORT                   GPIOD
#define LED2_GPIO_CLK                    RCC_APB2Periph_GPIOD

#define LED3_PIN                         GPIO_Pin_3
#define LED3_GPIO_PORT                   GPIOD
#define LED3_GPIO_CLK                    RCC_APB2Periph_GPIOD

const uint16_t GPIO_PIN[LEDn] = {LED1_PIN, LED2_PIN, LED3_PIN};
GPIO_TypeDef* GPIO_PORT[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT, LED3_GPIO_PORT};
const uint32_t GPIO_CLK[LEDn] = {LED1_GPIO_CLK, LED2_GPIO_CLK, LED3_GPIO_CLK};

void LEDInit(Led_TypeDef Led)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  /* Enable the GPIO_LED Clock */
  RCC_APB2PeriphClockCmd(GPIO_CLK[Led], ENABLE);

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN[Led];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIO_PORT[Led], &GPIO_InitStructure);
}

void LEDOn(Led_TypeDef Led)
{
  GPIO_PORT[Led]->BSRR = GPIO_PIN[Led];
}

void LEDOff(Led_TypeDef Led)
{
  GPIO_PORT[Led]->BRR = GPIO_PIN[Led];
}


int main(void)
{
  int i;

  /* Configure all unused GPIO port pins in Analog Input mode (floating input
     trigger OFF), this will reduce the power consumption and increase the device
     immunity against EMI/EMC *************************************************/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC |
                         RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF |
                         RCC_APB2Periph_GPIOG, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_Init(GPIOG, &GPIO_InitStructure);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC |
                         RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF |
                         RCC_APB2Periph_GPIOG, DISABLE);  

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD, ENABLE);

  LEDInit(LED1);
  LEDInit(LED2);
  LEDInit(LED3);

  while (1)
  {
  
    for (i = 0; i < 6; i++) {
      if (blink_leds[i][0] == 0) {
        LEDOff(LED1);
      }
      else {
        LEDOn(LED1);
      }

      if (blink_leds[i][1] == 0) {
        LEDOff(LED2);
      }
      else {
        LEDOn(LED2);
      }

      if (blink_leds[i][2] == 0) {
        LEDOff(LED3);
      }
      else {
        LEDOn(LED3);
      }

      Delay(0xAFFFF);
    }
  }
}

void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
