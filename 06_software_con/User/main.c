#include "ALL_Includes.h"//包含所需的头文件
#include "HIFI_0x98_USB.h"
#include "EQ_V0.1.h"
#include "i2c1.h"
#include "USART1.h"
#include "USART2.h"
#include "KEY.h"
#include "delay.h"
#include "PGA2311.h"
#include "F_24c02.h"


void Set_PCM1863(unsigned char in);

extern int vol_count; //0 - 255
extern int vol_count_left; //0 - 255
extern int vol_count_right; //0 - 255
int uart_vol_count;
int EQ_count = 0, Normal_count = 0;
int Buffer[255];

int g_mode = 0;

#define KeyPin 5
int g_ir_key = 0;
int g_mute_flag = 0;	// 0 = unmute  1=mute;

static unsigned char volume_set[][2]=
{
{0x30+14,0x30+14},
{0x30+12,0x30+12},
{0x30+10,0x30+10},
{0x30+8,0x30+8},
{0x30+6,0x30+6},
{0x30+4,0x30+4},
{0x30+2,0x30+2},
{0x30+0,0x30+0},
{0x30-0,0x30+0},
{0x30-0,0x30+0},
{0x30-0,0x30+0},
{0x30-0,0x30+0},
{0x30-0,0x30+0}
};
static int gpio_get_value(unsigned int gpio)
{
	unsigned int tmp = 0;

	
	//tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIODATA)) & (1<<gpio);
	tmp = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5);

	if(tmp)
		return 1;
	return 0; 
}


static unsigned int  serial_data_read_byte(void)
{
	
	int j;
	unsigned int CodeTemp=0;
	
	for(j=1;j<=8;j++)      //每个字节8个bit的判断
	{	
		
		//printf(" j=%d ",j);
		while(((gpio_get_value(KeyPin)) & 1) ==0 );       //等待上升沿
		//udelay(900);
		//delay_us(900);
		delay_us2(900);
		if(((gpio_get_value(KeyPin)) & 1) ==1 )
		{	
			//udelay(900);
			//delay_us(900);
			delay_us2(900);
			CodeTemp=CodeTemp|0x80;
			if(j<8) CodeTemp=CodeTemp>>1;	
			//printf("1");
		}
		else
		if(j<8)
		{
			CodeTemp=CodeTemp>>1;//如果是"0",则向右移一位，自动补"0"
			//printf("0");
		}
 	}
	//printf("\r\nj=%d\r\n",j);
	//printf("\r\n");
  	
	return CodeTemp;
	
}

int k1_hs0038_read(void)
{
	unsigned int values[4];
	int i=0;

	while((gpio_get_value(KeyPin) & 1)==0);	// 9ms
	
	while((gpio_get_value(KeyPin) & 1)==1)	// 4.5ms
	{
		//udelay(900);
		//delay_us(900);
		delay_us2(900);
		if(i>4)
			return -1;
		i++;
	}

	
	values[0] = serial_data_read_byte();	// add code
	values[1] = serial_data_read_byte();	// add inverse code
	values[2] = serial_data_read_byte();	// command code
	values[3] = serial_data_read_byte();	// command inverse code


	
	for(i=0;i<4;i++)
	{
		printf("values[%d]=0x%x\r\n", i,values[i]);		
	}

	if(values[0]==IR_ADD_CODE1 && values[1]==IR_ADD_CODE2)
	{
		if((values[2] |values[3])&0xff == 0xff)
			g_ir_key = values[2];
		printf("g_ir_key %d\r\n",g_ir_key);
	}
	

	return 0;
}


int  ir_key_switch_mode(int key)
{
	int tmp_mode = -1;

	switch(key)
	{
		case KEY_MODE_AUX:
			tmp_mode = 0;
		break;

		case KEY_MODE_BAL:
			tmp_mode = 1;
		break;

		case KEY_MODE_BT:
			tmp_mode = 2;
		break;

		case KEY_MODE_OPT:
			tmp_mode = 3;
		break;

		case KEY_MODE_COA:
			tmp_mode = 4;
		break;

		default:
		break;
	}

	return tmp_mode;
}

void set_mute(int mute_flag)
{
	if(mute_flag)
	{
		I2C1_Write_Byte(0x98, 0x3d, 254);	//pcm5242
		I2C1_Write_Byte(0x98, 0x3e, 254);	//pcm5242
		I2C1_Write_Byte(0x9A, 0x3d, 254);	//pcm5242
		I2C1_Write_Byte(0x9A, 0x3e, 254);	//pcm5242
	}
	else
	{
		I2C1_Write_Byte(0x98, 0x3d, 255 - vol_count);	//pcm5242
		I2C1_Write_Byte(0x98, 0x3e, 255 - vol_count);	//pcm5242
		I2C1_Write_Byte(0x9A, 0x3d, 255 - vol_count);	//pcm5242
		I2C1_Write_Byte(0x9A, 0x3e, 255 - vol_count);	//pcm5242
	}
}

void set_volume(int volume)
{
	I2C1_Write_Byte(0x98, 0x3d, 255 - volume);	//pcm5242
	I2C1_Write_Byte(0x98, 0x3e, 255 - volume);	//pcm5242
	I2C1_Write_Byte(0x9A, 0x3d, 255 - volume);	//pcm5242
	I2C1_Write_Byte(0x9A, 0x3e, 255 - volume);	//pcm5242
}

void mode_switch(int mode)
{
	if(g_mode == mode)
		return;
	else
		g_mode = mode;
	switch(mode)
	{
		case 0:
			GPIO_SetBits(GPIOB,GPIO_Pin_0);
			GPIO_SetBits(GPIOA,GPIO_Pin_7);	
			
			GPIO_ResetBits(GPIOA,GPIO_Pin_12);		// mode 0 pa12=0
			GPIO_SetBits(GPIOA,GPIO_Pin_11);		// other ??
			GPIO_SetBits(GPIOA,GPIO_Pin_10);		// other ??
			GPIO_ResetBits(GPIOA,GPIO_Pin_15);
			GPIO_SetBits(GPIOA,GPIO_Pin_9);		// other ??
			GPIO_SetBits(GPIOA,GPIO_Pin_8);		// other ??
			GPIO_ResetBits(GPIOA,GPIO_Pin_6);		// other ??
			GPIO_ResetBits(GPIOA,GPIO_Pin_5);		// other ??

			Set_PCM1863(0x04);
			delay_ms(1000);
			GPIO_ResetBits(GPIOB,GPIO_Pin_0); //
			/*
			FLASH_Unlock();
			FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR );
			//if(FLASH_ProgramOptionByteData(0x1FFFF804,0x11) == FLASH_COMPLETE)
			FLASH_ErasePage(0x0801FC00);
			if(FLASH_ProgramHalfWord(0x0801FFFC,0x3344) == FLASH_COMPLETE)
				printf("flash write ok\r\n");
			FLASH_Lock();
			
			tmp = FLASH_ReadHalfWord(0x0801FFFC);
			printf("0x%x\r\n",tmp);

			FLASH_Unlock();
			FLASH_OB_Unlock();
			FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR );
			//if(FLASH_ProgramOptionByteData(0x1FFFF804,0x11) == FLASH_COMPLETE)
			FLASH_OB_Erase();
			if(FLASH_ProgramOptionByteData(0x1FFFF804,0x77) == FLASH_COMPLETE)
				printf("flash write ok\r\n");
			FLASH_OB_Lock();
			FLASH_Lock();
			
			tmp = FLASH_ReadOptionByteData(0x1FFFF804);
			printf("0x%x\r\n",tmp);
			*/
		break;

		case 1:
			GPIO_SetBits(GPIOB,GPIO_Pin_0);
			GPIO_SetBits(GPIOA,GPIO_Pin_7);
			
			GPIO_SetBits(GPIOA,GPIO_Pin_12);		// other ??
			GPIO_ResetBits(GPIOA,GPIO_Pin_11);		// mode 1 ??
			GPIO_SetBits(GPIOA,GPIO_Pin_10);		// other ??
			GPIO_ResetBits(GPIOA,GPIO_Pin_15);
			GPIO_SetBits(GPIOA,GPIO_Pin_9);		// other ??
			GPIO_SetBits(GPIOA,GPIO_Pin_8);		// other ??
			GPIO_ResetBits(GPIOA,GPIO_Pin_6);		// other ??
			GPIO_ResetBits(GPIOA,GPIO_Pin_5);		// other ??

			//Set_PCM1863(0x03);	
			Set_PCM1863(0x10);	
			delay_ms(1000);
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);
			/*
			FLASH_Unlock();
			FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR );
			//if(FLASH_ProgramOptionByteData(0x1FFFF804,0x11) == FLASH_COMPLETE)
			FLASH_ErasePage(0x0801FC00);
			if(FLASH_ProgramHalfWord(0x0801FFFC,0x5566) == FLASH_COMPLETE)
				printf("flash write ok\r\n");
			FLASH_Lock();
			
			tmp = FLASH_ReadHalfWord(0x0801FFFE);
			printf("0x%x\r\n",tmp);
			
			FLASH_Unlock();
			FLASH_OB_Unlock();
			FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR );
			//if(FLASH_ProgramOptionByteData(0x1FFFF804,0x11) == FLASH_COMPLETE)
			FLASH_OB_Erase();
			if(FLASH_ProgramOptionByteData(0x1FFFF804,0x99) == FLASH_COMPLETE)
				printf("flash write ok\r\n");
			FLASH_OB_Lock();
			FLASH_Lock();
			
			tmp = FLASH_ReadOptionByteData(0x1FFFF804);
			printf("0x%x\r\n",tmp);
			*/
		break;

		case 2:
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);
			GPIO_SetBits(GPIOA,GPIO_Pin_7);
			
			GPIO_SetBits(GPIOA,GPIO_Pin_12);		// other ??
			GPIO_SetBits(GPIOA,GPIO_Pin_11);		// mode 1 ??
			GPIO_ResetBits(GPIOA,GPIO_Pin_10);		// other ??
			GPIO_SetBits(GPIOA,GPIO_Pin_15);		// mode 
			GPIO_SetBits(GPIOA,GPIO_Pin_9);		// other ??
			GPIO_SetBits(GPIOA,GPIO_Pin_8);		// other ??
			GPIO_ResetBits(GPIOA,GPIO_Pin_6);		// other ??
			GPIO_ResetBits(GPIOA,GPIO_Pin_5);		// other ??

			Set_PCM1863(0x08);	
		break;

		case 3:
			GPIO_SetBits(GPIOB,GPIO_Pin_0);
			GPIO_ResetBits(GPIOA,GPIO_Pin_7);
			
			GPIO_SetBits(GPIOA,GPIO_Pin_12);		// other ??
			GPIO_SetBits(GPIOA,GPIO_Pin_11);		// mode 1 ??
			GPIO_SetBits(GPIOA,GPIO_Pin_10);		// other ??
			GPIO_ResetBits(GPIOA,GPIO_Pin_15);
			GPIO_ResetBits(GPIOA,GPIO_Pin_9);		// other ??
			GPIO_SetBits(GPIOA,GPIO_Pin_8);		// other ??
			GPIO_ResetBits(GPIOA,GPIO_Pin_6);		// other ??
			GPIO_SetBits(GPIOA,GPIO_Pin_5);		// other ??
		break;

		case 4:
			GPIO_SetBits(GPIOB,GPIO_Pin_0);
			GPIO_ResetBits(GPIOA,GPIO_Pin_7);
			
			GPIO_SetBits(GPIOA,GPIO_Pin_12);		// other ??
			GPIO_SetBits(GPIOA,GPIO_Pin_11);		// mode 1 ??
			GPIO_SetBits(GPIOA,GPIO_Pin_10);		// other ??
			GPIO_ResetBits(GPIOA,GPIO_Pin_15);
			GPIO_SetBits(GPIOA,GPIO_Pin_9);		// other ??
			GPIO_ResetBits(GPIOA,GPIO_Pin_8);		// other ??
			GPIO_SetBits(GPIOA,GPIO_Pin_6);		// other ??
			GPIO_ResetBits(GPIOA,GPIO_Pin_5);		// other ??
		break;

		default:
		break;
	}
}



int main(void)
{	
	int mode = 0;	// 0=aux in 1=balance in 2=bt 3=optical fiber 4=coaxial
	int tmp=0;
	int tmpvolume = 0;
	
		Delay_Init(48);
		USART1_Init();
		//USART2_Init();
		IO_Init();
		

		{
			GPIO_SetBits(GPIOA, GPIO_Pin_2);
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);
			GPIO_SetBits(GPIOA, GPIO_Pin_3);
			GPIO_ResetBits(GPIOA, GPIO_Pin_1);

			GPIO_SetBits(GPIOA, GPIO_Pin_8);
		}
		delay_ms(2000);
		
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);
			
		IO_EXTI_Init();
		I2C_Configuration();
	
		led_on();
		set_speak_mute();
		PGA2311_Volume(0);
		power_on();
		Set_PCM1863(0x04);	

		
		//transmit_registers(registers, sizeof(registers)/sizeof(registers[0])); 
		transmit_registers(registers_pcm5242, sizeof(registers_pcm5242)/sizeof(registers_pcm5242[0])); 
		transmit_registers_R(registers_pcm5242_R, sizeof(registers_pcm5242_R)/sizeof(registers_pcm5242_R[0])); 
		//transmit_registers_R(registers_pcm5242, sizeof(registers_pcm5242)/sizeof(registers_pcm5242[0])); 
		set_speak_on();
		////上电读取24c02内值，并赋给count////
		//I2C1_Write_Byte(WRITE_24C02_ADDR, 0x00, 168);
		//delay_ms(1000);
		
		//if(I2C1_Read_Byte(READ_24C02_ADDR, 0x00, (uint8_t *)Buffer))
		GPIO_ResetBits(GPIOA, GPIO_Pin_8);

		
		if (1)
		{
				/*if((Buffer[0] <= Max_Volume) & (Buffer[0] >= Min_Volume))
				{
						vol_count = Buffer[0];
						PGA2311_Volume(vol_count);
						printf("Read_count: %d\r\n", Buffer[0]);
				}
				
				//printf("count: %d, %d, %d, %d\r\n", Buffer[0], Buffer[1], Buffer[2], Buffer[3]);
				*/
				//else
				//{	
						vol_count = Default_Volume;
						//PGA2311_Volume(vol_count);//-16db
						//printf("Write_count: %d\r\n", vol_count);
				//}
		}	
		
		//PGA2311_Volume(Default_Volume);//-16db
		//printf("count: %d\r\n", vol_count);
		printf("hilbertzhang\r\n");

		
		
		tmpvolume = FLASH_ReadOptionByteData(0x1FFFF804) & 0xFF;
		printf("tmpvolume: %d  0x%x\r\n",tmpvolume,tmpvolume);
		vol_count = tmpvolume;
		vol_count = 0;
		
		//set_volume(tmpvolume);


		//vol_count = 0x30;	// 0db
		//vol_count = 0x30+6;	// 0db
		vol_count_left = 0x30;	// 0db
		vol_count_right = 0x30;	// 0db
		set_pcm5242_volume(volume_set[0][0],volume_set[0][1]);
		//set_pcm5242_volume(0x30,0x30);
		
		while(1)
		{


				#if 1
				//if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_3) == 0)
				if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3) == 0)
				{
					delay_ms(20);
					if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3) == 0)
					{
						printf("pb4 low???????\r\n");
						//mode ++;
						//if(mode > 4)
						//	mode = 0;
						
						//set_mute(1);

						//g_mode = mode;
						//mode_switch(mode);
						
						//set_volume(tmpvolume);
						if(vol_count_left > 0)
						{
							vol_count_left--;
						}
						if(vol_count_right > 0)
						{
							vol_count_right --;
						}
						if( vol_count > 0)
						{
							vol_count --;
						}
						set_pcm5242_volume(volume_set[vol_count][0],volume_set[vol_count][1]);
					}
				}
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == 0)
				{
					delay_ms(20);
					if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == 0)
					{
						printf("pb4 low???????\r\n");
						//mode ++;
						//if(mode > 4)
						//	mode = 0;
						
						//set_mute(1);

						//g_mode = mode;
						//mode_switch(mode);
						
						//set_volume(tmpvolume);
						if(vol_count_left < 0xFF)
						{
							vol_count_left++;
						}
						if(vol_count_right < 0xFF)
						{
							vol_count_right++;
						}
						if(vol_count < 12)
						{
							vol_count++;
						}
						set_pcm5242_volume(volume_set[vol_count][0],volume_set[vol_count][1]);
					}
				}

				#if 0
				if(tmpvolume != vol_count)
				{
					tmpvolume = vol_count;
					
					FLASH_Unlock();
					FLASH_OB_Unlock();
					FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR );
					//if(FLASH_ProgramOptionByteData(0x1FFFF804,0x11) == FLASH_COMPLETE)
					FLASH_OB_Erase();
					if(FLASH_ProgramOptionByteData(0x1FFFF804,tmpvolume&0xFF) == FLASH_COMPLETE)
						printf("flash write ok\r\n");
					FLASH_OB_Lock();
					FLASH_Lock();
					
					tmp = FLASH_ReadOptionByteData(0x1FFFF804) &0xFF;
					printf("tmp:%d 0x%x\r\n",tmp,tmp);
				}
				#endif

				while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3) == 0)
				{
					;
				}

				while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == 0)
				{
					;
				}
				//printf("xlm  tmp \r\n");
				
				//GPIO_ResetBits(GPIOA,GPIO_Pin_12);
				//delay_us2(900);
				//GPIO_SetBits(GPIOA,GPIO_Pin_12);

				#endif
				
				#if 0
				if(g_ir_key)
				{
					tmp = ir_key_switch_mode(g_ir_key);
					printf("g_ir_key tmp %d\r\n",tmp);
					if(tmp == -1)
					{
						if(g_ir_key == KEY_MUTE)
						{
							g_mute_flag = g_mute_flag?0:1;
							set_mute(g_mute_flag);
						}
						else if(g_ir_key == KEY_VOLUME_ADD)
						{
							vol_count += 2;
							if(vol_count >= Max_Volume)//0DB
									vol_count = Max_Volume;
							set_volume(vol_count);
						}
						else if(g_ir_key == KEY_VOLUME_SUB)
						{
							vol_count -= 2;
							if(vol_count <= Min_Volume)//-60DB
									vol_count = Min_Volume;
							set_volume(vol_count);
						}
						else
						{
						}
					}
					else
					{
						mode_switch(tmp);
					}
					g_ir_key = 0;
				}
				#endif

				delay_ms(30);
				
		}
}

// in  select one channel or mutiple channel   when one channel in=0x1 0x2 0x04 0x08  
//when mutiple channel in=0x01 | 0x02 or other values
void Set_PCM1863(unsigned char in)
{
		uint8_t i;
		//uint8_t REG[7][2] ={{0x06,0x02},{0x07,0x02},{0x11,0x03},{0x20,0x91},{0x25,0x07},{0x26,0x07},{0x27,0x3f}};//0x20 0x91
		uint8_t REG[7][2] ={{0x06,0x04},{0x07,0x04},{0x11,0x03},{0x20,0x91},{0x25,0x07},{0x26,0x07},{0x27,0x3f}};//0x20 0x91  {0x06,0x04},{0x07,0x04}  vin3 R/L
		//uint8_t REG[7][2] ={{0x06,0x04},{0x07,0x04},{0x11,0x03},{0x20,0x91},{0x25,0x07},{0x26,0x07},{0x27,0x3f}};//0x20 0x91  {0x06,0x04},{0x07,0x04}  vin3 R/L
		//uint8_t REG[7][2] ={{0x06,0x0c},{0x07,0x0c},{0x11,0x03},{0x20,0x91},{0x25,0x07},{0x26,0x07},{0x27,0x3f}};//0x20 0x91
		//pcm1863配置写入
		REG[0][1] = in;
		REG[1][1] = in;
		for(i = 0; i < 7; i++)
		{
				I2C1_Write_Byte(0x94, REG[i][0],REG[i][1]);
		}
		if(in == 0x10)
		{
			I2C1_Write_Byte(0x94, 0x08,0x10);
			I2C1_Write_Byte(0x94, 0x09,0x10);
		}
		else
		{
			I2C1_Write_Byte(0x94, 0x08,0);
			I2C1_Write_Byte(0x94, 0x09,0);
		}
}








