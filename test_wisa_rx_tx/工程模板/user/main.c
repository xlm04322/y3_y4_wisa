/*******************************************************************************
*                 
*                 		       普中科技
--------------------------------------------------------------------------------
* 实 验 名		 : printf重定向实验
* 实验说明       : 通过串口打印数据，学习printf重定向 与C语言printf功能一样
* 连接方式       : 
* 注    意		 : 	所用函数在头文件.c文件内
*******************************************************************************/


#include "public.h"
#include "printf.h"
#include "systick.h"
#include "led.h"

#define I2C_SLAVE_ADDRESS7 0x30
uint8_t  I2C2_Buffer_Tx[1200];
uint16_t I2C2_Buffer_Rx[1200];
EventStatus i2c_event= NOEVENT;

I2C_InitTypeDef   I2C_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;


/**
  * @brief  Configures NVIC and Vector Table base location.
  * @param  None
  * @retval : None
  */
void NVIC_Configuration(void)
{
#ifdef  VECT_TAB_RAM
    /* Set the Vector Table base location at 0x20000000 */
    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
    /* Set the Vector Table base location at 0x08000000 */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* Configure and enable I2Cx event interrupt -------------------------------*/
    NVIC_InitStructure.NVIC_IRQChannel = I2C2_EV_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    /* Configure and enable I2C1 error interrupt -------------------------------*/
    NVIC_InitStructure.NVIC_IRQChannel = I2C2_ER_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_Init(&NVIC_InitStructure);

}


void y3y4_i2c_init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;

    /* Configure I2C1 pins: SCL and SDA ----------------------------------------*/
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10| GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


	I2C_DeInit(I2C2);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

	    /* I2C2 configuration ------------------------------------------------------*/
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = I2C_SLAVE_ADDRESS7;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 400000;
    I2C_Init(I2C2, &I2C_InitStructure);

}


/****************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/
int main()
{	
	u32 d1=123;
	float d2=12.34;
	u8 i2c_data = 0;
	int i=0;
	//SystemInit();
	
	printf_init();	//printf串口初始化
	LED_Init();
	NVIC_Configuration();
	y3y4_i2c_init();
	printf("深圳普中科技有限公司\r\n");
	printf("www.prechin.com\r\n");
	printf("to d1 a int number %d\r\n",d1); //整型数
	printf("to d2 a float number %f\r\n",d2);//浮点数
	printf("change hexfile is %X\r\n",d1);//16进制
	printf("change 0file is %o\r\n",d1); //八进制
//		USART_SendData(USART1,'1');	
	delay_ms(1000);
	led_state(LED_OFF);
	delay_ms(1000);	
	for(i=0;i<250;i++)
	{
		I2C2_Buffer_Tx[i] = i+1;
	}
	while(1)
	{
	        /* Enable I2C2 event and buffer interrupts */
	        I2C_ITConfig(I2C2, I2C_IT_EVT | I2C_IT_BUF, ENABLE);
	        /* Enable I2C2 Error interrupts */
	        I2C_ITConfig(I2C2, I2C_IT_ERR, ENABLE);

	        if (i2c_event == EVENT_OPCOD_NOTYET_READ)
	        {
			delay_ms(1000);
			led_state(LED_OFF);
			delay_ms(1000);		
			led_state(LED_ON);
	            /* Read opcode */
				for(i=0;i<17;i++)
				{
			            i2c_data = I2C2_Buffer_Rx[i];
					printf("i2c_data[%d]: 0x%02x   ",i,i2c_data);
				}
				printf("\r\n");
	            i2c_event = EVENT_OPCOD_READ; /* The opcode has been already read */
	        }

	}
}



