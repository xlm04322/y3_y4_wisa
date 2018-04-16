#include "i2c1.h"
  
uint32_t I2C_Timeout;
uint8_t drive_Addr = 0x98;
 
 uint8_t drive_Addr_r = 0x9A;
void I2C_Configuration(void)
{ 
  GPIO_InitTypeDef  GPIO_InitStructure;
    I2C_InitTypeDef  I2C_InitStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
   
  /* Configure the I2C clock source. The clock is derived from the SYSCLK */
  RCC_I2CCLKConfig(RCC_I2C1CLK_SYSCLK);
   
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
   
  /* Connect PB8 to I2C_SCL */
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource8, GPIO_AF_1);
  /* Configure PB9 pins: SDA */
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource9, GPIO_AF_1);
     
  /* Configure PB8 pins: SCL */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
   
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
  I2C_InitStructure.I2C_DigitalFilter = 0x00;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Disable;
  I2C_InitStructure.I2C_Timing = I2C_TIMING;
  
  /* Apply I2C1 configuration after enabling it */
  I2C_Init(I2C1, &I2C_InitStructure);
    
  /* I2C1 Peripheral Enable */
  I2C_Cmd(I2C1, ENABLE);
  
}
 
/**
  * @brief  从I2C1总线上的某一器件的某一起始地址中读取一定字节的数据到数组中
  * @param  driver_Addr：I2C器件地址
  * @param  start_Addr：起始字节地址
  * @param  number_Bytes：要读取的字节数量（小于一页）
  * @param  read_Buffer：存放读取数据的数组指针
  * @retval 是否读取成功
  */
I2C_Status I2C1_Read_Byte(uint8_t driver_Addr, uint8_t start_Addr, uint8_t *read_Buffer)
{
   
  I2C_Timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET)
  {
    if((I2C_Timeout--) == 0)
    {
      return I2C_FAIL;
    }
  }
   
  I2C_TransferHandling(I2C1, driver_Addr, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
   
  I2C_Timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET)
  {
    if((I2C_Timeout--) == 0)
    {
      return I2C_FAIL;
    }
  }
   
  I2C_SendData(I2C1, start_Addr);
   
  I2C_Timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TC) == RESET)
  {
    if((I2C_Timeout--) == 0)
    {
      return I2C_FAIL;
    }
  }
   
  I2C_TransferHandling(I2C1, driver_Addr, 1,  I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
   
 // for(read_Num = 0; read_Num < number_Bytes; read_Num++)
 // {
    I2C_Timeout = I2C_TIMEOUT;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET)
    {
      if((I2C_Timeout--) == 0)
      {
        return I2C_FAIL;
      }
    }
     
    *read_Buffer = I2C_ReceiveData(I2C1);
 // }
   
  I2C_Timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET)
  {
    if((I2C_Timeout--) == 0)
    {
      return I2C_FAIL;
    }
  }
 
  return I2C_OK;
}
 
/**
  * @brief  从I2C1的总线上的某一器件的某一起始地址中读取一定字节的数据到数组中
  * @param  driver_Addr：I2C器件地址
  * @param  start_Addr：起始字节地址
  * @param  number_Bytes：要读取的字节数量（小于一页）
  * @param  write_Buffer：存放读取数据的数组指针
  * @retval 是否读取成功
  */
I2C_Status I2C1_Write_Byte(uint8_t driver_Addr, uint8_t start_Addr, uint8_t write_data)
{ 
  I2C_Timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) != RESET)
  {
    if((I2C_Timeout--) == 0)
    {
            printf("I2C_FLAG_BUSY\n\r");
      return I2C_FAIL;
    }
  }
   
  I2C_TransferHandling(I2C1, driver_Addr, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
   
  I2C_Timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET)
  {
    if((I2C_Timeout--) == 0)
    {
            printf("I2C_FLAG_TXIS\n\r");
      return I2C_FAIL;
    }
  }
   
  I2C_SendData(I2C1, start_Addr);
   
  I2C_Timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TCR) == RESET)
  {
    if((I2C_Timeout--) == 0)
    {
            printf("I2C_FLAG_TCR\n\r");
      return I2C_FAIL;
    }
  }
 
  I2C_TransferHandling(I2C1, driver_Addr, 1, I2C_AutoEnd_Mode, I2C_No_StartStop);
   
 // for(write_Num = 0; write_Num < number_Bytes; write_Num++)
 // {
    I2C_Timeout = I2C_TIMEOUT;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXIS) == RESET)
    {
      if((I2C_Timeout--) == 0)
      {
				printf("I2C_FLAG_TXIS\n\r");
        return I2C_FAIL;
      }
    }
     
    I2C_SendData(I2C1, write_data);
//  }
 
  I2C_Timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET)
  {
    if((I2C_Timeout--) == 0)
    {
			printf("I2C_FLAG_STOP\n\r");
      return I2C_FAIL;
    }
  }
   
  return I2C_OK;
}

////////////////////////////////////////////////////////
///////////////////////////////////////////////////////

I2C_Status i2c_write(uint8_t *r,uint8_t count)
{
	I2C1_Write_Byte(drive_Addr, r[0], r[1]);
	//printf("%x  %x\r\n", r[0], r[1]);
	return I2C_OK;
}

I2C_Status i2c_write_r(uint8_t *r,uint8_t count)
{
	I2C1_Write_Byte(drive_Addr_r, r[0], r[1]);
	//printf("%x  %x\r\n", r[0], r[1]);
	return I2C_OK;
}
//*(unsigned char *)&registers_pcm1863[i],*((unsigned char *)&registers_pcm1863[i]+1)

//	   transmit_registers(registers, sizeof(registers)/sizeof(registers[0]));
   void transmit_registers(cfg_reg const *r, int n)
   {
	   int i = 0;

	   while (i < n) {
		   switch (r[i].struct_param.command) {
		   case CFG_META_SWITCH:
			   // Used in legacy applications.	Ignored here.
			   break;
		   case CFG_META_DELAY:
			   delay_ms(r[i].struct_param.param);
			   break;
		   case CFG_META_BURST:
			   i2c_write((unsigned char *)&r[i+1], r[i].struct_param.param);
			   i += r[i].struct_param.param/2 +1;
			   break;
		   default:
			   i2c_write((unsigned char *)&r[i], 2);
			   break;
		   }
		   i++;
	   }
   }

void transmit_registers_R(cfg_reg const *r, int n)
{
	int i = 0;

	while (i < n) {
		switch (r[i].struct_param.command) {
		case CFG_META_SWITCH:
			// Used in legacy applications.  Ignored here.
			break;
		case CFG_META_DELAY:
			delay_ms(r[i].struct_param.param);
			break;
		case CFG_META_BURST:
			i2c_write_r((unsigned char *)&r[i+1], r[i].struct_param.param);
			i += r[i].struct_param.param/2 +1;
			break;
		default:
			i2c_write_r((unsigned char *)&r[i], 2);
			break;
		}
		i++;
	}
}


// 0x00--0xFF  0:24db  0xFF:mute  1->0.5db  0x30->0db
int set_pcm5242_volume(unsigned char volume_left,  unsigned char volume_right)
{
	int ret = -1;
	printf("============set_pcm5242_volume, %d  volume: [%d  %d]============\n",__LINE__,volume_left,volume_right);
	I2C1_Write_Byte(drive_Addr, 0x00, 0);
	I2C1_Write_Byte(drive_Addr, 0x3d, volume_left);
	I2C1_Write_Byte(drive_Addr, 0x3e, volume_left);
	I2C1_Write_Byte(drive_Addr_r, 0x00, 0);
	I2C1_Write_Byte(drive_Addr_r, 0x3d, volume_right);
	I2C1_Write_Byte(drive_Addr_r, 0x3e, volume_right);


	
	ret = 0;
	return ret;
}



/************************ (C) COPYRIGHT LKL0305 ****************END OF FILE****/
