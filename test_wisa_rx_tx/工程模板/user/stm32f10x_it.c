/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "systick.h"
#include "smg.h"
#include "public.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

void EXTI2_IRQHandler()	   //外部中断2中断函数
{
	if(EXTI_GetITStatus(EXTI_Line2)==SET)
	{
   		EXTI_ClearITPendingBit(EXTI_Line0);//清除EXTI线路挂起位
		delay_ms(10);//消抖处理
		if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)==Bit_RESET)	   //k_left按键按下
		{
			if(GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_0)==Bit_RESET)
			{
				//LED 熄灭
			   GPIO_SetBits(GPIOC,GPIO_Pin_0);	
			}
			else
			{
			   //LED 发光
				GPIO_ResetBits(GPIOC,GPIO_Pin_0);
			}
		} 
		while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)==0);
	}		
}


void USART1_IRQHandler(void)	//串口1中断函数
{
	static u8 k;
	USART_ClearFlag(USART1,USART_FLAG_TC);
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=Bit_RESET)//检查指定的USART中断发生与否
	{
		k=USART_ReceiveData(USART1);
	//	k++;	
		USART_SendData(USART1,k);//通过外设USARTx发送单个数据
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==Bit_RESET);	
	}
}
void USART2_IRQHandler(void)	//485通信中断函数
{
	static u8 k;
	USART_ClearFlag(USART2,USART_FLAG_TC);
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)//检查指定的USART中断发生与否	
	{
		k=USART_ReceiveData(USART2);
		GPIO_SetBits(GPIOG,GPIO_Pin_3);
		delay_ms(1);
		USART_SendData(USART2,k);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
		delay_ms(2);		
		GPIO_ResetBits(GPIOG,GPIO_Pin_3);
	}
}
void TIM3_IRQHandler()	  //定时器3中断函数
{
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (AN_example_src), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
__IO uint16_t Tx_Idx =0, Rx_Idx=0;
extern uint8_t  I2C2_Buffer_Tx[];
extern uint16_t I2C2_Buffer_Rx[];
extern EventStatus i2c_event;


/**
  * @brief  This function handles I2C1 Event interrupt request.
  * @param  None
  * @retval : None
  */
  void I2C2_EV_IRQHandler(void)
{

    switch (I2C_GetLastEvent(I2C2))
    {
        /* Slave Transmitter ---------------------------------------------------*/
    case I2C_EVENT_SLAVE_BYTE_TRANSMITTED:
	printf("std\r\n");
    
      I2C_SendData(I2C2, I2C2_Buffer_Tx[Tx_Idx++]);
      break;


    case I2C_EVENT_SLAVE_BYTE_TRANSMITTING:             /* EV3 */   
	printf("stg\r\n");

        /* Transmit I2C2 data */
        I2C_SendData(I2C2, I2C2_Buffer_Tx[Tx_Idx++]);
        break;
    case 0x000600c0:             /* EV3 */   
	printf("stgre\r\n");

        /* Transmit I2C2 data */
	printf("re:0x%02x\r\n",I2C_ReceiveData(I2C2));
        I2C_SendData(I2C2, I2C2_Buffer_Tx[Tx_Idx++]);
        break;



    /* Slave Receiver ------------------------------------------------------*/
    case I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED:     /* EV1 */
	printf("sram\r\n");
    
      break;

    case I2C_EVENT_SLAVE_BYTE_RECEIVED:                /* EV2 */
    case I2C_EVENT_SLAVE_BYTE_RECEIVED|0x04:                /* EV2 */
	printf("sbr\r\n");
        /* Store I2C1 received data */
        I2C2_Buffer_Rx[Rx_Idx++] = I2C_ReceiveData(I2C2);

        break;

    case I2C_EVENT_SLAVE_STOP_DETECTED:                /* EV4 */
	printf("ssd\r\n");
        /* Clear I2C1 STOPF flag */
  
        I2C_Cmd(I2C2, ENABLE);
//        Rx_Idx=0;
        i2c_event = EVENT_OPCOD_NOTYET_READ;
    break;

    default:
			printf("0x%08x\r\n",I2C_GetLastEvent(I2C2));
        break;
    }
}



/**
  * @brief  This function handles I2C1 Error interrupt request.
  * @param  None
  * @retval : None
  */
void I2C2_ER_IRQHandler(void)
{
	printf("er\r\n");
    /* Check on I2C1 AF flag and clear it */
    if (I2C_GetITStatus(I2C2, I2C_IT_AF))
    {
        I2C_ClearITPendingBit(I2C2, I2C_IT_AF);
        Tx_Idx = 0;
        i2c_event = EVENT_OPCOD_NOTYET_READ;
    }


    /* Check on I2C1 AF flag and clear it */
    if (I2C_GetITStatus(I2C2, I2C_IT_BERR))
    {
        I2C_ClearITPendingBit(I2C2, I2C_IT_BERR);
    }
}



/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
