
#include "KEY.h"



void IO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

//PF7---->EQ_KEY	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
          
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

	
//PA5---->VCC_DET	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
          
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 |GPIO_Pin_15;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

	
//PA6---->POWER_ON  PA15----->LED 0
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
          
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_8 |GPIO_Pin_12 |GPIO_Pin_11 |
								GPIO_Pin_10 |GPIO_Pin_9 |GPIO_Pin_5 |GPIO_Pin_7 |
								GPIO_Pin_0 | GPIO_Pin_2 |GPIO_Pin_1 | GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//PB12--->MUTE_PGA2311,
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
          
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_0;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ;			// mode key
    GPIO_Init(GPIOB, &GPIO_InitStructure);

			  /*
		      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;			// mode key
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    */

  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	 
		
}

//void GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource)  

void IO_EXTI_Init(void)
{
		//sGPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitTypeDef GPIO_InitStructure;
		EXTI_InitTypeDef EXTI_InitStruct;
		NVIC_InitTypeDef NVIC_InitStruct;

		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE); 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); 	
 /* Configure PB.03, PB.04, as input floating */  
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_5;  
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_2;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOB, &GPIO_InitStructure);  
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, GPIO_PinSource10); 
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, GPIO_PinSource11);
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, GPIO_PinSource5);
		EXTI_ClearITPendingBit(EXTI_Line10 | EXTI_Line11 |EXTI_Line5);
	
// Configure EXTI Line3 to generate an interrupt on falling edge | rising edge  
		EXTI_InitStruct.EXTI_Line = EXTI_Line10; 
		EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;  
		EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;//EXTI_Trigger_Rising_Falling;
		EXTI_InitStruct.EXTI_LineCmd = ENABLE;  
		EXTI_Init(&EXTI_InitStruct);  
 
// Configure EXTI Line4 to generate an interrupt on falling edge  
		EXTI_InitStruct.EXTI_Line = EXTI_Line11;  
		EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;  
		EXTI_InitStruct.EXTI_Trigger =  EXTI_Trigger_Falling;//EXTI_Trigger_Rising_Falling;  
		EXTI_InitStruct.EXTI_LineCmd = ENABLE;  
		EXTI_Init(&EXTI_InitStruct);
	
	// Configure EXTI Line4 to generate an interrupt on falling edge  
			EXTI_InitStruct.EXTI_Line = EXTI_Line5;  
			EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;  
			EXTI_InitStruct.EXTI_Trigger =	EXTI_Trigger_Falling;//EXTI_Trigger_Rising_Falling;  
			EXTI_InitStruct.EXTI_LineCmd = ENABLE;	
			EXTI_Init(&EXTI_InitStruct);
			
/* enabling interrupt */ 		 
    NVIC_InitStruct.NVIC_IRQChannel = EXTI2_3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0x00;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
		
		NVIC_InitStruct.NVIC_IRQChannel = EXTI4_15_IRQn;  
		NVIC_InitStruct.NVIC_IRQChannelPriority = 0x01;    
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  
		NVIC_Init(&NVIC_InitStruct); 		
}  

//=============================================================================
//文件名称：EXTI4_15_IRQHandler  EXTI2_3_IRQHandler 
//功能概要：按键中断函数
//参数说明：无
//函数返回：无
//=============================================================================

int vol_count;  
int vol_count_left;  
int vol_count_right;  
uint8_t last_a, last_b;
extern uint8_t drive_Addr ;
extern I2C_Status I2C1_Write_Byte(uint8_t driver_Addr, uint8_t start_Addr, uint8_t write_data);
extern int k1_hs0038_read(void);
void EXTI2_3_IRQHandler(void)
{
		uint8_t a_level = 0, b_level = 0, exti_a_level = 0;
		a_level = PB3;
	
		if ( EXTI_GetITStatus(EXTI_Line3)!= RESET ) 
		{	
				exti_a_level = PB3;
		//printf("PB3 = %d\r\n", (GPIO_ReadInputData(GPIOB)>>3)&0x01);
				if(exti_a_level == a_level)
				{
						b_level = PB4;
						if (a_level==1 && b_level==0) 
						{
								if (last_a==0&&last_b==0) 
								{
									//backward
										last_a=last_a;
										last_b=last_b; 
									//printf("PB3_1\r\n");
								}
						}
					else if (a_level == 0 && b_level==0) 
					{
							if (last_a==1&&last_b==0) 
							{
								//forward
									last_a=last_a;
									last_b=last_b;
				//send event
									vol_count += 2;
									if(vol_count >= Max_Volume)//0DB
											vol_count = Max_Volume;
									PGA2311_Volume(vol_count);
									USART_SendData(USART2, vol_count);
									while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);//USART_FLAG_TC发送完成标志
									printf("++++:%d\r\n", vol_count);
							}
					}
					else if (a_level == 0 && b_level==1) 
					{
							//backward
							last_a = a_level;
							last_b = b_level;
						//printf("PB3_2\r\n");
					}
					else if (a_level == 0 && b_level==1)
					{
							last_a=last_a;
							last_b=last_b;
						//printf("PB3_3\r\n");
					}
				}
		}
		
		EXTI_ClearITPendingBit(EXTI_Line3);	
}	

		int db = 0;
void EXTI4_15_IRQHandler(void)
{
		u8 a_level = 0, b_level = 0, exti_a_level = 0, exti_b_level = 0;
		a_level = PBA;
		b_level = PBB;
		//int db = 0;
		
		if ( EXTI_GetITStatus(EXTI_Line10)!= RESET ) 
		{	
				exti_a_level = PBA;
		//printf("PB3 = %d\r\n", (GPIO_ReadInputData(GPIOB)>>3)&0x01);
				if(exti_a_level == a_level)
				{
						b_level = PBB;
						if (a_level==1 && b_level==0) 
						{
								if (last_a==0&&last_b==0) 
								{
									//backward
										last_a=last_a;
										last_b=last_b; 
									//printf("PB3_1\r\n");
								}
						}
					else if (a_level == 0 && b_level==0) 
					{
							if (last_a==1&&last_b==0) 
							{
								//forward
									last_a=last_a;
									last_b=last_b;
				//send event
									vol_count += 2;
									if(vol_count >= Max_Volume)//0DB
											vol_count = Max_Volume;
									//{ 0x3d, 0x30 },
									//	{ 0x3e, 0x30 },
									// 0db 0x30   0x00 24db 0xff -103db
									db = 255 - vol_count;
									//I2C1_Write_Byte(0x9A, 0x3d, db);	//5754
									//I2C1_Write_Byte(0x9A, 0x3e, db);	//5754
									I2C1_Write_Byte(0x98, 0x3d, db);	//pcm5242
									I2C1_Write_Byte(0x98, 0x3e, db);	//pcm5242
									I2C1_Write_Byte(0x9A, 0x3d, db);	//pcm5242
									I2C1_Write_Byte(0x9A, 0x3e, db);	//pcm5242
									//PGA2311_Volume(vol_count);
									//USART_SendData(USART2, vol_count);
									//while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);//USART_FLAG_TC发送完成标志
									printf("++++:%d\r\n", vol_count);
							}
					}
					else if (a_level == 0 && b_level==1) 
					{
							//backward
							last_a = a_level;
							last_b = b_level;
						//printf("PB3_2\r\n");
					}
					else if (a_level == 0 && b_level==1)
					{
							last_a=last_a;
							last_b=last_b;
						//printf("PB3_3\r\n");
					}
				}
				EXTI_ClearITPendingBit(EXTI_Line10);
		}
		else if ( EXTI_GetITStatus(EXTI_Line11)!= RESET ) 
		{	
				exti_b_level = PBB;
				if(exti_b_level == b_level)
				{
						a_level = PBA;
					//printf("PB3 = %d\r\n", (GPIO_ReadInputData(GPIOB)>>3)&0x01);
						if (a_level==1 && b_level==0) 
						{
							////forward
								last_a = a_level;
								last_b = b_level;
							//printf("PB4_1\r\n");
						}
						else if (a_level == 0 && b_level==0)
						{
								if (last_a==0&&last_b==1)
								{
									//backward
										last_a=last_a;
										last_b=last_b;
									//send event
										vol_count -= 2;
										if(vol_count <= Min_Volume)//-60DB
												vol_count = Min_Volume;
		
										db = 255 - vol_count;
										//I2C1_Write_Byte(0x9A, 0x3d, db);	//5754
										//I2C1_Write_Byte(0x9A, 0x3e, db);	//5754
										I2C1_Write_Byte(0x98, 0x3d, db);	//pcm5242
										I2C1_Write_Byte(0x98, 0x3e, db);	//pcm5242
										I2C1_Write_Byte(0x9A, 0x3d, db);	//pcm5242
										I2C1_Write_Byte(0x9A, 0x3e, db);	//pcm5242
										//PGA2311_Volume(vol_count);
										//USART_SendData(USART2, vol_count);
										//while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);//发送是否完成
										printf("----:%d\r\n", vol_count);
								}
						}
						else if (a_level == 0 && b_level==1) 
						{
								if (last_a==0&&last_b==0) 
								{
									//forward
										last_a=last_a;
										last_b=last_b;
									//printf("PB4_2\r\n");
								}
						}
						else if (a_level == 1 && b_level==1) 
						{
							//if (last_a==1&&last_b==0) {
								//backward
								last_a=last_a;
								last_b=last_b;
							//printf("PB4_3\r\n");
								//}
						}
		
					}
				EXTI_ClearITPendingBit(EXTI_Line11);
		
	}		
	else if ( EXTI_GetITStatus(EXTI_Line5)!= RESET ) 
	{	
			//printf("EXTI_Line5 1\r\n");
			k1_hs0038_read();
			
			printf("EXTI_Line5 2 \r\n");
			EXTI_ClearITPendingBit(EXTI_Line5);
	
	}		

		
		
	
}	


