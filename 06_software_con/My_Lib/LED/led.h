#ifndef __led_H
#define __led_H 

#include "ALL_Includes.h"

#define LED_GPIO_CLK   RCC_AHBPeriph_GPIOC
#define LED_PORT   	   GPIOC
#define LED_PIN        GPIO_Pin_13


void LED_Init(void);
void LED_ON(void);
void LED_OFF(void);

#endif



