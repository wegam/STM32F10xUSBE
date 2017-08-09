#ifdef ADC_Temperature
#include "ADC_Temperature.H"


#define ADC_TEST_BUFFERSIZE 128


u16 ADCBuffer=0;
RCC_ClocksTypeDef RCC_ClocksStatus;
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void ADC_Temperature_Configuration(void)
{
	TIM_Configuration(TIM3,7200,10000);																															//定时时间设定
	PWM_Configuration(TIM2,7200,10000,99);
	USART_DMA_Configuration(USART1,115200,1,1,(u32*)ADCBuffer,(u32*)ADCBuffer,ADC_TEST_BUFFERSIZE);	//USART_DMA配置
	ADC_TempSensorConfiguration((u32*)&ADCBuffer);																									//STM32内部温度传感器配置
}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void ADC_Temperature_Server(void)
{
	float Temperature=0.0;
	Get_Clocks(&RCC_ClocksStatus);
	if(!USART_TX_DMAFlagClear(USART1))
	{
		Temperature=Get_ADC_Temperature(ADCBuffer);														//获取内部温度传感器温度
		USART_DMAPrintf(USART1,"当前STM32内部温度为：%6.2f℃,外部高速时钟频率为：%dHz, %d, %d,  %d, %d, %d\n",Temperature,HSE_Value,RCC_ClocksStatus.SYSCLK_Frequency,RCC_ClocksStatus.HCLK_Frequency,RCC_ClocksStatus.PCLK1_Frequency,RCC_ClocksStatus.PCLK2_Frequency,RCC_ClocksStatus.ADCCLK_Frequency);
	}
}

#endif
