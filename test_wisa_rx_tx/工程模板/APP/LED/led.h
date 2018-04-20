#ifndef _led_H
#define _led_H
#include "stm32f10x.h"
#define LED GPIO_Pin_14	 	//π‹Ω≈∫Í∂®“Â
#define LED_ON	1
#define LED_OFF	0
void LED_Init(void);
void led_display(void);
void led_huayang(void);
void led_state(int state);


#endif 
