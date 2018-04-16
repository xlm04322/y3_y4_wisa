//=============================================================================
//文件名称：ADC1.c
//功能概要：ADC1驱动文件
//更新时间：2013-11-20
//调试方式：J-Link OB ARM SWD  STLink  V2  SWD
//=============================================================================

#include "ADC1.h"

 __IO uint32_t TempSensVoltmv=0;
 __IO uint32_t VrefIntVoltmv=0;
 __IO uint32_t PC3Voltmv=0; 
 __IO uint32_t VbatVoltmv=0; 
 
 uint16_t RegularConvData_Tab[4];



/*-------------------------------------------------------------------
	----------------ADC单路采样----------------------------------------
	-------------------------------------------------------------------*/
/*定义变量 ADC_ConvertedValue  将PA3配置成AD1的通道10 */
/*__IO uint16_t ADC_ConvertedValue;

static void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;          // Enable ADC1 and GPIOC clock 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
*/

//配置ADC1的工作模式为DMA模式
/*static void ADC1_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	DMA_DeInit(DMA1_Channel1);   //将与DMA有关的寄存器设初始值
//定义DMA外设基地址，即存放转换结果的寄存器。ADC1_DR_Address
	DMA_InitStructure.DMA_PeripheralBaseAddr =ADC1_DR_Address;
//定义内存基地址，告诉DMA从AD中取来的数放到ADC_ConvertedValue
	DMA_InitStructure.DMA_MemoryBaseAddr =(u32)&ADC_ConvertedValue;
//定义AD外设作为数据传输的来源
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
//指定DMA开辟几个内存空间，只取一个通道
	DMA_InitStructure.DMA_BufferSize = 1;
//设定寄存器地址固定，即告诉DMA，只从固定的地方取值
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//设定内存地址固定，只将值搬到固定的内存中
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//设定外设数据宽度，即取的数的大小
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
//设定内存的宽度
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
//设定DMA工作在循环缓存模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
//设定DMA选定的通道软件优先级
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
//选用DMA1_channel1
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	
//设置ADC工作在独立模式
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
//规定AD转换工作在单次模式，对一个通道采样
	ADC_InitStructure.ADC_ScanConvMode = DISABLE
//设定AD转化在连续模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
//不使用外部触发转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
//采集的数据以右对齐 方式存放
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
//设定要转换的AD通道数目
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
//配置ADC时钟，为PCLK2的8分频，即9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
//配置ADC1的通道10为239.5个采样周期
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5);
// Enable ADC1 DMA
	ADC_DMACmd(ADC1, ENABLE);
// Enable ADC1
	ADC_Cmd(ADC1, ENABLE);
//复位校准寄存器
	ADC_ResetCalibration(ADC1);
//等待校准寄存器复位完成
	while(ADC_GetResetCalibrationStatus(ADC1));
//ADC校准
	ADC_StartCalibration(ADC1);
//等待校准完成
	while(ADC_GetCalibrationStatus(ADC1));
//使用软件触发ADC转换
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
*/

void ADC1_DMA_Init(void)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	DMA_InitTypeDef     DMA_InitStructure;
	ADC_InitTypeDef     ADC_InitStructure;

	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);		
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
	
	ADC_DeInit(ADC1);//ADC恢复默认设置		

  DMA_DeInit(DMA1_Channel1);	/* DMA1 Channel1 Config */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;//外设地址
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RegularConvData_Tab;//内存地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//外设作为数据传输的来源
  DMA_InitStructure.DMA_BufferSize = 4;//
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址寄存器不变
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存地址寄存器不变
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//数据宽度为16位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//数据宽度为16位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMA_Priority设定DMA通道x的软件优先级
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//DMA通道x没有设置为内存到内存传输
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
		  
  
	
	DMA_Cmd(DMA1_Channel1, ENABLE);/* DMA1 Channel1 enable */			
	ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular); /* Enable ADC_DMA */	
  ADC_DMACmd(ADC1, ENABLE);  
	
		

	ADC_StructInit(&ADC_InitStructure);//初始化ADC结构
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位精度
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //规定模式装换工作在连续模式
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; 
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据对其为右对齐
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Backward; //ADC的扫描方向
  ADC_Init(ADC1, &ADC_InitStructure); 
	 
  ADC_ChannelConfig(ADC1, ADC_Channel_1 , ADC_SampleTime_239_5Cycles); /* Convert the ADC1 Channel 11 with 239.5 Cycles as sampling time */  

  ADC_ChannelConfig(ADC1, ADC_Channel_Vrefint ,ADC_SampleTime_239_5Cycles); 
  ADC_VrefintCmd(ENABLE);
	
	ADC_ChannelConfig(ADC1, ADC_Channel_TempSensor ,ADC_SampleTime_239_5Cycles);
	ADC_TempSensorCmd(ENABLE);
	
	ADC_ChannelConfig(ADC1, ADC_Channel_Vbat ,ADC_SampleTime_239_5Cycles);
	ADC_VbatCmd(ENABLE);
	
	ADC_GetCalibrationFactor(ADC1); /* ADC Calibration */  
  ADC_Cmd(ADC1, ENABLE);  /* Enable ADCperipheral[PerIdx] */	  
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN)); /* Wait the ADCEN falg */
  ADC_StartOfConversion(ADC1); /* ADC1 regular Software Start Conv */ 
			
}

