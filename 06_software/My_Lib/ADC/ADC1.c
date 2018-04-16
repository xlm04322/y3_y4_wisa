//=============================================================================
//�ļ����ƣ�ADC1.c
//���ܸ�Ҫ��ADC1�����ļ�
//����ʱ�䣺2013-11-20
//���Է�ʽ��J-Link OB ARM SWD  STLink  V2  SWD
//=============================================================================

#include "ADC1.h"

 __IO uint32_t TempSensVoltmv=0;
 __IO uint32_t VrefIntVoltmv=0;
 __IO uint32_t PC3Voltmv=0; 
 __IO uint32_t VbatVoltmv=0; 
 
 uint16_t RegularConvData_Tab[4];



/*-------------------------------------------------------------------
	----------------ADC��·����----------------------------------------
	-------------------------------------------------------------------*/
/*������� ADC_ConvertedValue  ��PA3���ó�AD1��ͨ��10 */
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

//����ADC1�Ĺ���ģʽΪDMAģʽ
/*static void ADC1_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	DMA_DeInit(DMA1_Channel1);   //����DMA�йصļĴ������ʼֵ
//����DMA�������ַ�������ת������ļĴ�����ADC1_DR_Address
	DMA_InitStructure.DMA_PeripheralBaseAddr =ADC1_DR_Address;
//�����ڴ����ַ������DMA��AD��ȡ�������ŵ�ADC_ConvertedValue
	DMA_InitStructure.DMA_MemoryBaseAddr =(u32)&ADC_ConvertedValue;
//����AD������Ϊ���ݴ������Դ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
//ָ��DMA���ټ����ڴ�ռ䣬ֻȡһ��ͨ��
	DMA_InitStructure.DMA_BufferSize = 1;
//�趨�Ĵ�����ַ�̶���������DMA��ֻ�ӹ̶��ĵط�ȡֵ
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//�趨�ڴ��ַ�̶���ֻ��ֵ�ᵽ�̶����ڴ���
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//�趨�������ݿ�ȣ���ȡ�����Ĵ�С
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
//�趨�ڴ�Ŀ��
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
//�趨DMA������ѭ������ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
//�趨DMAѡ����ͨ��������ȼ�
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
//ѡ��DMA1_channel1
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	
//����ADC�����ڶ���ģʽ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
//�涨ADת�������ڵ���ģʽ����һ��ͨ������
	ADC_InitStructure.ADC_ScanConvMode = DISABLE
//�趨ADת��������ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
//��ʹ���ⲿ����ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
//�ɼ����������Ҷ��� ��ʽ���
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
//�趨Ҫת����ADͨ����Ŀ
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
//����ADCʱ�ӣ�ΪPCLK2��8��Ƶ����9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
//����ADC1��ͨ��10Ϊ239.5����������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5);
// Enable ADC1 DMA
	ADC_DMACmd(ADC1, ENABLE);
// Enable ADC1
	ADC_Cmd(ADC1, ENABLE);
//��λУ׼�Ĵ���
	ADC_ResetCalibration(ADC1);
//�ȴ�У׼�Ĵ�����λ���
	while(ADC_GetResetCalibrationStatus(ADC1));
//ADCУ׼
	ADC_StartCalibration(ADC1);
//�ȴ�У׼���
	while(ADC_GetCalibrationStatus(ADC1));
//ʹ���������ADCת��
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
	
	ADC_DeInit(ADC1);//ADC�ָ�Ĭ������		

  DMA_DeInit(DMA1_Channel1);	/* DMA1 Channel1 Config */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;//�����ַ
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RegularConvData_Tab;//�ڴ��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//������Ϊ���ݴ������Դ
  DMA_InitStructure.DMA_BufferSize = 4;//
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�Ĵ�������
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�ڴ��ַ�Ĵ�������
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//���ݿ��Ϊ16λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//���ݿ��Ϊ16λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMA_Priority�趨DMAͨ��x��������ȼ�
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
		  
  
	
	DMA_Cmd(DMA1_Channel1, ENABLE);/* DMA1 Channel1 enable */			
	ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular); /* Enable ADC_DMA */	
  ADC_DMACmd(ADC1, ENABLE);  
	
		

	ADC_StructInit(&ADC_InitStructure);//��ʼ��ADC�ṹ
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λ����
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //�涨ģʽװ������������ģʽ
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; 
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//���ݶ���Ϊ�Ҷ���
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Backward; //ADC��ɨ�跽��
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

