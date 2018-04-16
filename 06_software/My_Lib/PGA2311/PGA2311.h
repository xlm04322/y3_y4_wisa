#ifndef _PGA2311_H
#define _PGA2311_H

#include "ALL_Includes.h"
#include "KEY.h"

#define PGA2311_CS   GPIO_Pin_8
#define PGA2311_CLK  GPIO_Pin_14
#define PGA2311_SDI  GPIO_Pin_15
 
//void PGA2311_Volume_Up(void);
//void PGA2311_Volume_Down(void);
void PGA2311_Volume(uint8_t RL_Channel_Count);
#endif

