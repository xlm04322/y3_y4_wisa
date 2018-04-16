
#ifndef __KEY_H
#define __KEY_H

#include "ALL_Includes.h"
#include "speak.h"
#include "PGA2311.h"

//#define Default_Volume   162              //-16dB
#define Default_Volume   207              //-16dB
//#define Max_Volume       192              //0dB
#define Max_Volume       255              //0dB
//#define Min_Volume       62               //-60dB
#define Min_Volume       0               //-60dB

#define IR_ADD_CODE1	0x16
#define IR_ADD_CODE2	0x01
#define KEY_VOLUME_ADD	0x0a
#define KEY_VOLUME_SUB	0x0b
#define KEY_MODE_AUX	0x01
#define KEY_MODE_BAL	0x02
#define KEY_MODE_BT		0x03
#define KEY_MODE_OPT	0x04
#define KEY_MODE_COA	0x05
#define KEY_MUTE			0x0e


#define PA5  	((GPIO_ReadInputData(GPIOA)>>5)&0x01)
#define PB3  	((GPIO_ReadInputData(GPIOB)>>3)&0x01)
#define PB4  	((GPIO_ReadInputData(GPIOB)>>4)&0x01)
#define PBA  	((GPIO_ReadInputData(GPIOB)>>10)&0x01)
#define PBB  	((GPIO_ReadInputData(GPIOB)>>11)&0x01)

void IO_Init(void);
void IO_EXTI_Init(void);
void EXTI2_3_IRQHandler(void);
void EXTI4_15_IRQHandler(void);

#endif
