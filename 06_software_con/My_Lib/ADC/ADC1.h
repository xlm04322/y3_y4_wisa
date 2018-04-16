
#ifndef __ADC1_H
#define __ADC1_H

#include "ALL_Includes.h"

#define ADC1_DR_Address                0x40012440
//#define ADC1_DR_Address (u32)0x40012400+0x4c
extern  __IO uint32_t TempSensVoltmv;
extern  __IO uint32_t VrefIntVoltmv ;
extern  __IO uint32_t PC3Voltmv ;
extern  __IO uint32_t VbatVoltmv ;
extern  uint16_t RegularConvData_Tab[4];

void ADC1_DMA_Init(void);

#endif
