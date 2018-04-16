#include "PGA2311.h"

void PGA2311_Volume(uint8_t RL_Channel_Count)
{
		uint8_t i = 0;
		
		GPIO_ResetBits(GPIOB, PGA2311_CLK);
		GPIO_ResetBits(GPIOA, PGA2311_CS);
	
		for(i = 0; i < 8; i++)
		{
			 if(RL_Channel_Count & 0x80)
			 {
					GPIO_SetBits(GPIOB, PGA2311_SDI);
			 }
			 else
			 {
					GPIO_ResetBits(GPIOB, PGA2311_SDI);
			 }
			
			 GPIO_SetBits(GPIOB, PGA2311_CLK);
			 RL_Channel_Count = RL_Channel_Count << 1;
			 GPIO_ResetBits(GPIOB, PGA2311_CLK);
		}
		
		GPIO_SetBits(GPIOA, PGA2311_CS);
		//GPIO_SetBits(GPIOA, PGA2311_SDI);
}































































