#ifdef ADC_Temperature
#include "ADC_Temperature.H"


#define ADC_TEST_BUFFERSIZE 128


u16 ADCBuffer=0;
RCC_ClocksTypeDef RCC_ClocksStatus;
/*******************************************************************************
* ������		:	
* ��������	:	 
* ����		:	
* ���		:
* ���� 		:
*******************************************************************************/
void ADC_Temperature_Configuration(void)
{
	TIM_Configuration(TIM3,7200,10000);																															//��ʱʱ���趨
	PWM_Configuration(TIM2,7200,10000,99);
	USART_DMA_Configuration(USART1,115200,1,1,(u32*)ADCBuffer,(u32*)ADCBuffer,ADC_TEST_BUFFERSIZE);	//USART_DMA����
	ADC_TempSensorConfiguration((u32*)&ADCBuffer);																									//STM32�ڲ��¶ȴ���������
}
/*******************************************************************************
* ������		:	
* ��������	:	 
* ����		:	
* ���		:
* ���� 		:
*******************************************************************************/
void ADC_Temperature_Server(void)
{
	float Temperature=0.0;
	Get_Clocks(&RCC_ClocksStatus);
	if(!USART_TX_DMAFlagClear(USART1))
	{
		Temperature=Get_ADC_Temperature(ADCBuffer);														//��ȡ�ڲ��¶ȴ������¶�
		USART_DMAPrintf(USART1,"��ǰSTM32�ڲ��¶�Ϊ��%6.2f��,�ⲿ����ʱ��Ƶ��Ϊ��%dHz, %d, %d,  %d, %d, %d\n",Temperature,HSE_Value,RCC_ClocksStatus.SYSCLK_Frequency,RCC_ClocksStatus.HCLK_Frequency,RCC_ClocksStatus.PCLK1_Frequency,RCC_ClocksStatus.PCLK2_Frequency,RCC_ClocksStatus.ADCCLK_Frequency);
	}
}

#endif