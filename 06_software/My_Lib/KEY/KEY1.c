//=============================================================================
//文件名称：KEY.c
//功能概要：KEY驱动文件
//更新时间：2013-11-20
//调试方式：J-Link OB ARM SWD
//=============================================================================

#include "KEY.h"
#include "delay.h"
uint8_t last_a = 0, last_b = 0;
void IO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
          
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
	  last_a = GPIO_ReadInputDataBit(GPIOA, 5);
		last_b = GPIO_ReadInputDataBit(GPIOA, 6);
		printf("DEBUG:PA5 = %x, PA6 = %x\r\n", last_a, last_b);
	
}

void IO_EXTI_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStruct;
		EXTI_InitTypeDef EXTI_InitStruct;
		NVIC_InitTypeDef NVIC_InitStruct;
	
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	// //Configure PA5, PA6, as input floating
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_2;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
//		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOA, &GPIO_InitStruct); 
	
	
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource5);	
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource6); 
	
		EXTI_ClearITPendingBit(EXTI_Line5 | EXTI_Line6);	
// Configure EXTI Line3 to generate an interrupt on falling edge | rising edge  
	#if 1
		EXTI_InitStruct.EXTI_Line = EXTI_Line5; 
		EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;  
		EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;  
		EXTI_InitStruct.EXTI_LineCmd = ENABLE;  
		EXTI_Init(&EXTI_InitStruct);  
 #endif
 
// Configure EXTI Line13 to generate an interrupt on falling edge  
	
		EXTI_InitStruct.EXTI_Line = EXTI_Line6;  
		EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt; 
		EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
		//EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
		EXTI_InitStruct.EXTI_LineCmd = ENABLE;  
		EXTI_Init(&EXTI_InitStruct);
	
//enabling interrupt 

#if 0
		NVIC_InitStruct.NVIC_IRQChannel = EXTI2_3_IRQn;  
		NVIC_InitStruct.NVIC_IRQChannelPriority = 0x00;    
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  
		NVIC_Init(&NVIC_InitStruct);
#endif

		NVIC_InitStruct.NVIC_IRQChannel = EXTI4_15_IRQn;  
		NVIC_InitStruct.NVIC_IRQChannelPriority = 0x00;    
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  
		NVIC_Init(&NVIC_InitStruct); 		
}  


//=============================================================================
//文件名称：EXTI4_15_IRQHandler  EXTI2_3_IRQHandler 
//功能概要：按键中断函数
//参数说明：ROTARY_A  ROTARY_B编码器连接的GPIO
//函数返回：无
//=============================================================================
//uint8_t last_a, last_b;
uint8_t exti_signal_a = 0, exti_signal_b = 0;

void EXTI2_3_IRQHandler()
{
		
	if ( EXTI_GetITStatus(EXTI_Line3)!= RESET )
	{	
		//last_a = GPIO_ReadInputDataBit(GPIOA, 6);
		//last_b = GPIO_ReadInputDataBit(GPIOA, 8);
		//printf("DEBUG:PA6 = %x, PA8 = %x\r\n", last_a, last_b);
			delay_ms(2);
			exti_signal_a = GPIO_ReadInputDataBit(GPIOA, 3);
			exti_signal_b = GPIO_ReadInputDataBit(GPIOA, 11);
			printf("EXTI2_3:PA3 = %x, PA11 = %x\r\n", exti_signal_a, exti_signal_b);
			EXTI_ClearITPendingBit(EXTI_Line3);
	}
}

void EXTI4_15_IRQHandler()
{
	if ( EXTI_GetITStatus(EXTI_Line6)!= RESET )
	{	
			exti_signal_a = GPIO_ReadInputDataBit(GPIOA, 5);
			exti_signal_b = GPIO_ReadInputDataBit(GPIOA, 6);
			printf("EXTI4_15:PA5 = %x, PA6 = %x\r\n", exti_signal_a, exti_signal_b);
			EXTI_ClearITPendingBit(EXTI_Line6);
	
//		u8 t1 = 50;
//		u16 t2 = 1000;
//		//printf("PA11 = %x, PB13 = %x\r\n", (GPIOA->IDR&GPIO_Pin_11 >> 11)&0x01  , (GPIOB->IDR&GPIO_Pin_13 >> 13)&0x01);
//    if ((EXTI->PR) & (1 << 13))
//    {
//        EXTI->PR = 1 << 13;     //清除LINE3上的中断标志位
//        exti_signal_a = GPIO_ReadInputDataBit(GPIOA, 11);
//				exti_signal_b = GPIO_ReadInputDataBit(GPIOB, 13);
//				printf("PA11 = %x, PB13 = %x\r\n", (GPIOA->IDR&GPIO_Pin_11 >> 11)&0x01  , (GPIOB->IDR&GPIO_Pin_13 >> 13)&0x01);
//        if (!exti_signal_a)
//        {
//            delay_us(10);
//            while ((exti_signal_b == 0) && (t1--))
//            {
//                delay_ms(1);
//            }
//            delay_us(10);
//            if (exti_signal_a)
//                printf("顺时针转\r\n\r\n");
//        }
//        else
//        {
//            delay_us(10);
//            while ((exti_signal_b == 0) && (t2--))
//            {
//                delay_ms(1);
//            }
//            delay_us(10);
//            if (!exti_signal_a)
//                printf("逆时针转\r\n\r\n");
//        }
    }
}


