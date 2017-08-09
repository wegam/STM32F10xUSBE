/******************************** User_library *********************************
* 文件名 	: STM32_SDCard.H
* 作者   	: wegam@sina.com
* 版本   	: V
* 日期   	: 2016/01/01
* 说明   	: 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


#include "STM32_DAC.H"
//#include "STM32F10x_BitBand.H"

/*******************************************************************************
【概述】
STM32的DAC是一个12位，电压输出的DAC。可被配置为12位或者8位，也能和DMA联合使用。
DAC具有两个独立转换通道。在双DAC模式下，DA抓换可被配置成独立模式或者同步工作模式。
两路DAC参考电压以及ADC都是VREF
【主要特性】
1、两路
2、12bit时数据可被配置成左对齐或右对齐
3、具有同步更新能力
4、噪声产生
5、三角波产生
6、两个通道独立转换或同步转换
7、每个通道都有DMA能力
8、DMA出错检测
9、可外部触发
【输出电压计算公式】
Vout = VREF×DOR/4095 = 参考电压×DAC数据寄存器值/4095

*******************************************************************************/

//#define DAC_DHR8R1_Address		0x40007410
//#define DAC_DHR8R1_Address		DAC_BASE + DHR12R1_Offset + DAC_Align_8b_R		//CH1_12位右对齐数据格式寄存器地址

#define DAC_DHR8R1_Address		DAC_BASE + 0x00000008 + DAC_Align_8b_R		//CH1_12位右对齐数据格式寄存器地址
#define DAC_DHR8R2_Address		DAC_BASE + 0x00000014 + DAC_Align_8b_R		//CH2_12位右对齐数据格式寄存器地址


#define DAC_DHR12R1_Address		DAC_BASE + 0x00000008 + DAC_Align_12b_R		//CH1_12位右对齐数据格式寄存器地址
#define DAC_DHR12R2_Address		DAC_BASE + 0x00000014 + DAC_Align_12b_R		//CH2_12位右对齐数据格式寄存器地址

#define DAC_DHR12L1_Address		DAC_BASE + 0x00000008 + DAC_Align_12b_L		//CH1_12位右对齐数据格式寄存器地址
#define DAC_DHR12L2_Address		DAC_BASE + 0x00000014 + DAC_Align_12b_L		//CH2_12位右对齐数据格式寄存器地址

uc8 Escalator8bit[6] = {0x0, 0x33, 0x66, 0x99, 0xCC, 0xFF};
uc16 Escalator16bit[9] = {0x0, 0x0200, 0x0400, 0x0600, 0x0800, 0x0A00,0x0C00,0X0E00,0X0FFF};

/*******************************************************************************
*函数名		:DAC_Configuration
*功能描述	:DAC配置
*输入			:
*输出			:无
*返回值		:无
*说明			:利用DAC输出指定的电压，软件触发DAC转换。
					 STM32 DAC管脚为DAC1-PA4,DAC2-PA5 12Bit 
*******************************************************************************/
void DAC_Configuration(void)
{
	DAC_GPIO_Configuration();
	DAC_MODE_Configuration();
}

/*******************************************************************************
*函数名		:DAC_GPIO_Configuration
*功能描述	:DAC管脚配置
*输入			:
*输出			:无
*返回值		:无
*说明			:利用DAC输出指定的电压，软件触发DAC转换。
					 STM32 DAC管脚为DAC1-PA4,DAC2-PA5 12Bit 
*******************************************************************************/
void DAC_GPIO_Configuration(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;				//GPIO结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);				//使能GPIOA时钟
	
	//1)**********配置DAC管脚	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;								//模拟输入
	GPIO_Init(GPIOA,&GPIO_InitStructure);												//配置引脚
	
}
/*******************************************************************************
*函数名		:DAC_MODE_Configuration
*功能描述	:DAC参数配置
*输入			:
*输出			:无
*返回值		:无
*说明			:利用DAC输出指定的电压，软件触发DAC转换。
					 STM32 DAC管脚为DAC1-PA4,DAC2-PA5 12Bit 
*******************************************************************************/
void DAC_MODE_Configuration(void)
{
	DAC_InitTypeDef		DAC_InitStructure; 				//DAC结构体
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	//1)**********将DAC设置为缺省值	
	DAC_DeInit();																//将DAC设置为缺省值

	//2)**********打开DAC时钟，并设置分频因子	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);					//使能DAC时钟

//////////////////定时器
	

//   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6 , ENABLE);   
//     TIM_TimeBaseStructure.TIM_Period =0x19;    
//     TIM_TimeBaseStructure.TIM_Prescaler = 0;    
//     TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
//     TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
//     TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

// TIM_ARRPreloadConfig(TIM6 , ENABLE);

//    /* TIM6 TRGO selection */
//     TIM_SelectOutputTrigger(TIM6 , TIM_TRGOSource_Update);

// TIM_ITConfig(TIM6 , TIM_IT_Update , ENABLE);
//////////////////////////////////

	//3)**********配置DAC模式	
	DAC_InitStructure.DAC_Trigger=DAC_Trigger_Software;													//DAC触发源----软件触发	
	DAC_InitStructure.DAC_WaveGeneration=DAC_WaveGeneration_Noise;								//波形发生器----不使用内部波形发生器
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;		//波形发生器----输出波形，此为不输出
	DAC_InitStructure.DAC_OutputBuffer=DAC_OutputBuffer_Enable;									//开输出缓存---开输出缓冲时，输出能力强些
	//	DAC_Init(DAC_Channel_1|DAC_Channel_2,&DAC_InitStructure);									//DAC1--PA4
	DAC_Init(DAC_Channel_1,&DAC_InitStructure);									//DAC1--PA4
	DAC_Init(DAC_Channel_2,&DAC_InitStructure);									//DAC2--PA5

	//4)**********使能DAC
	DAC_Cmd(DAC_Channel_1,ENABLE);
	DAC_Cmd(DAC_Channel_2,ENABLE);
	//5)**********向输出寄存器写数据，数据可根据上边公式得到
	//Vout = VREF×DOR/4095 = 参考电压×DAC数据寄存器值/4095
	DAC_SetChannel1Data(DAC_Align_12b_R,4095);
	DAC_SetChannel2Data(DAC_Align_12b_R,4095);
	//4)**********触发输出
	DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);
	DAC_SoftwareTriggerCmd(DAC_Channel_2,ENABLE);
}
/*******************************************************************************
*函数名		:DAC_MODE_Configuration
*功能描述	:DAC参数配置
*输入			:
*输出			:无
*返回值		:无
*说明			:利用DAC输出指定的电压，软件触发DAC转换。
					 STM32 DAC管脚为DAC1-PA4,DAC2-PA5 12Bit 
*******************************************************************************/
void DAC_DMA_Configuration(u32 *MemoryAddr)
{
//	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;	//定时器结构体定义
	DAC_InitTypeDef		DAC_InitStructure; 				//DAC结构体
	DMA_InitTypeDef DMA_InitStructure;
	
	/* Enable peripheral clocks --------------------------------------------------*/
	/*开启DMA时钟*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
  /* AFIO and GPIOA Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
  /* DAC Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
  /* TIM6 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
	DAC_GPIO_Configuration();
	
	 /* TIM6 Configuration */
	TIM_PrescalerConfig(TIM6, 71, TIM_PSCReloadMode_Update);		//分频
	TIM_SetAutoreload(TIM6, 1);																//初值
	/* TIM6 TRGO selection */
	TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

	/* DAC channel1 Configuration */
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_4095;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	DAC_Init(DAC_Channel_2, &DAC_InitStructure);

  /* DMA2 channel3 configuration */
	DMA_DeInit(DMA2_Channel4);
	DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R2_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)MemoryAddr;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	//	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA2_Channel4, &DMA_InitStructure);

	/* Enable DMA2 Channel3 */
	DMA_Cmd(DMA2_Channel4, ENABLE);

	/* Enable DAC Channel1 */
	DAC_Cmd(DAC_Channel_2, ENABLE);

	/* Enable DMA for DAC Channel1 */
	DAC_DMACmd(DAC_Channel_2, ENABLE);

	/* TIM6 enable counter */
	TIM_Cmd(TIM6, ENABLE);
}







