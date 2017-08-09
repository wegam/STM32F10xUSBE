/******************************** User_library *********************************
* �ļ��� 	: STM32_SDCard.H
* ����   	: wegam@sina.com
* �汾   	: V
* ����   	: 2016/01/01
* ˵��   	: 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


#include "STM32_DMA.H"
#include "STM32_WOW.H"
//#include "STM32F10x_BitBand.H"

/*##############################################################################
################################################################################
# ģ����		:	STM32_DMA	
# ��������	:	DMA����ʹ��
-------------------------------------------------------------------------------	
********DMA����:********
ֱ�Ӵ洢����ȡ(DMA)�����ṩ������ʹ洢��֮����ߴ洢���ʹ洢��֮��ĸ������ݴ��䡣
����CPU��Ԥ�����ݿ���ͨ��DMA���ٵ��ƶ�����ͽ�ʡ��CPU����Դ�������������� ����DMA
��������12��ͨ��(DMA1��7��ͨ����DMA2��5��ͨ��)��ÿ��ͨ��ר����������������һ����
�������Դ洢�����ʵ����󡣻���һ���ٲ�����Э������DMA���������Ȩ��
-------------------------------------------------------------------------------
********DMA��Ҫ����:********
�� 12�������Ŀ����õ�ͨ��(����)��DMA1��7��ͨ����DMA2��5��ͨ�� 
�� ÿ��ͨ����ֱ������ר�õ�Ӳ��DMA����ÿ��ͨ����ͬ��֧��������������Щ����ͨ�����������á� 
�� ��ͬһ��DMAģ���ϣ��������������Ȩ����ͨ�������������(�����ļ����ܸߡ��ߡ��еȺ͵�)������Ȩ�������ʱ��Ӳ������(����0����������1����������) �� 
�� ��������Դ��Ŀ���������Ĵ������(�ֽڡ����֡�ȫ��)��ģ�����Ͳ���Ĺ��̡�Դ��Ŀ���ַ���밴���ݴ�����ȶ��롣 
�� ֧��ѭ���Ļ��������� 
�� ÿ��ͨ������3���¼���־(DMA�봫�䡢DMA������ɺ�DMA�������)����3���¼���־�߼����Ϊһ���������ж����� 
�� �洢���ʹ洢����Ĵ��� 
�� ����ʹ洢�����洢��������֮��Ĵ��� 
�� ���桢SRAM�������SRAM��APB1��APB2��AHB���������Ϊ���ʵ�Դ��Ŀ�ꡣ 
�� �ɱ�̵����ݴ�����Ŀ�����Ϊ65535

*******************************************DMA1����ӳ��DMA1_Channelx****************************************************
-------------------------------------------------------------------------------------------------------------------------
����		|			CH1				|			CH2				|			CH3				|			CH4				|			CH5				|			CH6				|			CH7				|
-------------------------------------------------------------------------------------------------------------------------
ADC1		|ADC1						|								|								|								|								|								|								|
-------------------------------------------------------------------------------------------------------------------------
SPI/I2S	|								|SPI1_RX				|	SPI1_TX				|SPI/I2S2_RX		|SPI/I2S2_TX		|								|								|
-------------------------------------------------------------------------------------------------------------------------
USART		|								|USART3_TX			|	USART3_RX			|USART1_TX			|USART1_RX			|USART2_RX			|USART2_TX			|
-------------------------------------------------------------------------------------------------------------------------
I2C			|								|								|								|	I2C2_TX				|I2C2_RX				|I2C1_TX				|I2C1_RX				|
-------------------------------------------------------------------------------------------------------------------------
TIM1		|								|TIM1_CH1				|	TIM1_CH2			|TIM1_TX4				|TIM1_UP				|TIM1_CH3				|								|
				|								|								|								|TIM1_TRIG			|								|								|								|
				|								|								|								|TIM1_COM				|								|								|								|
-------------------------------------------------------------------------------------------------------------------------
TIM2		|IM2_CH3				|TIM2_UP				|								|								|TIM2_CH1				|								|TIM2_CH2				|
				|								|								|								|								|								|								|TIM2_CH4				|
-------------------------------------------------------------------------------------------------------------------------
TIM3		|								|TIM3_CH3				|TIM3_CH4				|								|								|TIM3_CH1				|								|
				|								|								|TIM3_UP				|								|								|TIM3_TRIG			|								|
-------------------------------------------------------------------------------------------------------------------------
TIM4		|TIM4_CH1				|								|								|TIM4_CH2				|TIM4_CH3				|								|TIM4_UP				|
-------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------
*************************************************************************************************************************

*******************************************DMA2����ӳ��DMA2_Channelx****************************************************
-------------------------------------------------------------------------------------------------------------------------
����		|			CH1				|			CH2				|			CH3				|			CH4				|			CH5				|
-------------------------------------------------------------------------------------------------------------------------
ADC3		|								|								|								|								|ADC3						|
-------------------------------------------------------------------------------------------------------------------------
SPI/I2S3|SPI/I2S3_RX		|SPI/I2S3_TX		|								|								|								|
-------------------------------------------------------------------------------------------------------------------------
UART4		|								|								|UART4_RX				|								|UART4_TX				|
-------------------------------------------------------------------------------------------------------------------------
SDIO		|								|								|								|SDIO						|								|
-------------------------------------------------------------------------------------------------------------------------
TIM5		|TIM5_CH4				|TIM5_CH3				|								|TIM5_CH2				|TIM5_CH1				|
				|TIM5_TRIG			|TIM5_UP				|								|								|								|
-------------------------------------------------------------------------------------------------------------------------
TIM6		|								|								|TIM6_UP				|								|								|
DAC1		|								|								|DAC1						|								|								|
-------------------------------------------------------------------------------------------------------------------------
TIM7		|								|								|								|TIM7_UP				|								|
DAC2		|								|								|								|DAC2						|								|
-------------------------------------------------------------------------------------------------------------------------
TIM8		|TIM8_CH3				|TIM8_CH4				|TIM8_CH1				|								|TIM8_CH2				|	
				|TIM8_UP				|TIM8_TRIG			|								|								|								|
				|								|TIM8_COM				|								|								|								|		
-------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------
*************************************************************************************************************************

################################################################################
###############################################################################*/

//DMA�ṹ��
//typedef struct
//{
//  u32 DMA_PeripheralBaseAddr;		//-------DMA�������ַ
//  u32 DMA_MemoryBaseAddr;				//-------DMA�ڴ����ַ
//  u32 DMA_DIR;									//-------DMA���ݴ��䷽��=============DMA_DIR_PeripheralDST��������ΪDMA��Ŀ�Ķˣ���DMA_DIR_PeripheralSRC��������Ϊ���ݴ������Դ��
//  u32 DMA_BufferSize;						//-------DMAͨ����DMA����Ĵ�С======��λΪ���ݵ�λ�����ݴ��䷽�����ݵ�λ���ڽṹ�в���DMA_PeripheralDataSize���߲���DMA_MemoryDataSize��ֵ��
//  u32 DMA_PeripheralInc;				//-------DMA�����ַ�Ĵ����������====DMA_PeripheralInc_Enable�������ַ�Ĵ�����������DMA_PeripheralInc_Disable�������ַ�Ĵ������䣩
//  u32 DMA_MemoryInc;						//-------DMA�ڴ��ַ�Ĵ����������====DMA_MemoryInc_Enable���ڴ��ַ�Ĵ�����������DMA_MemoryInc_Disable���ڴ��ַ�Ĵ������䣩
//  u32 DMA_PeripheralDataSize;		//-------DMA�������ݿ���=============DMA_PeripheralDataSize_Byte�����ݿ���Ϊ8λ����DMA_PeripheralDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_PeripheralDataSize_Word�����ݿ���Ϊ32λ��
//  u32 DMA_MemoryDataSize;				//-------DMA�ڴ����ݿ���=============DMA_MemoryDataSize_Byte�����ݿ���Ϊ8λ����DMA_MemoryDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_MemoryDataSize_Word�����ݿ���Ϊ32λ��
//  u32 DMA_Mode;									//-------DMA�Ĺ���ģʽ===============DMA_Mode_Normal��ֻ����һ�Σ�, DMA_Mode_Circular����ͣ�ش��ͣ�
																	//-------ע�⣺��ָ��DMAͨ�����ݴ�������Ϊ�ڴ浽�ڴ�ʱ������ʹ��ѭ������ģʽ
//  u32 DMA_Priority;							//-------DMAͨ����ת�����ȼ�=========DMA_Priority_VeryHigh���ǳ��ߣ�DMA_Priority_High����)��DMA_Priority_Medium���У���DMA_Priority_Low���ͣ�																	
//  u32 DMA_M2M;									//-------DMAͨ�����ڴ浽�ڴ洫��=====DMA_M2M_Enable(����Ϊ�ڴ浽�ڴ洫��)��DMA_M2M_Disable�����ڴ浽�ڴ洫�䣩
//}DMA_InitTypeDef;

//u32 *DMA_Buffer;
//u8	DMA_BufferSize=0;
/*******************************************************************************
*������		:	DMA_Configuration
*��������	:	��������
*����			:	MemoryAddr--�ڴ��ַ,
						BufferSize--�����С
*���			:	��
*����ֵ		:	��
*����			:
*******************************************************************************/
void DMA_Configuration(Peripheral_Request_TypeDef	Request,u32 *MemoryAddr,u32 BufferSize)	//DMA����
{
	DMA_InitTypeDef DMA_Initstructure;
	NVIC_InitTypeDef NVIC_Initstructure;

	DMA_Channel_TypeDef* DMAy_Channelx=0;											//-------DMAͨ��

	FunctionalState	DMA_IT_Interruupt=ENABLE;									//-------DMA��������ж�ʹ��
	u8	DMAy_Channelx_IRQChannel=DMA1_Channel1_IRQChannel;		//-------DMA�ж�Դ
	u32	DMAy_FLAG_GLx=0;																			//-------DMAȫ���жϱ�־
	
	
	u32 DMA_PeripheralBaseAddr=0;															//-------DMA�������ַ
  u32 DMA_MemoryBaseAddr=(u32)MemoryAddr;										//-------DMA�ڴ����ַ
  u32 DMA_DIR=DMA_DIR_PeripheralSRC;												//-------DMA���ݴ��䷽��=============DMA_DIR_PeripheralDST��������ΪDMA��Ŀ�Ķˣ���DMA_DIR_PeripheralSRC��������Ϊ���ݴ������Դ��
  u32 DMA_BufferSize=BufferSize;														//-------DMAͨ����DMA����Ĵ�С======��λΪ���ݵ�λ�����ݴ��䷽�����ݵ�λ���ڽṹ�в���DMA_PeripheralDataSize���߲���DMA_MemoryDataSize��ֵ��
  u32 DMA_PeripheralInc=DMA_PeripheralInc_Disable;					//-------DMA�����ַ�Ĵ����������====DMA_PeripheralInc_Enable�������ַ�Ĵ�����������DMA_PeripheralInc_Disable�������ַ�Ĵ������䣩
  u32 DMA_MemoryInc=DMA_MemoryInc_Disable;									//-------DMA�ڴ��ַ�Ĵ����������====DMA_MemoryInc_Enable���ڴ��ַ�Ĵ�����������DMA_MemoryInc_Disable���ڴ��ַ�Ĵ������䣩
  u32 DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;		//-------DMA�������ݿ���=============DMA_PeripheralDataSize_Byte�����ݿ���Ϊ8λ����DMA_PeripheralDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_PeripheralDataSize_Word�����ݿ���Ϊ32λ��
  u32 DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;						//-------DMA�ڴ����ݿ���=============DMA_MemoryDataSize_Byte�����ݿ���Ϊ8λ����DMA_MemoryDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_MemoryDataSize_Word�����ݿ���Ϊ32λ��
  u32 DMA_Mode=DMA_Mode_Normal;															//-------DMA�Ĺ���ģʽ===============DMA_Mode_Normal��ֻ����һ�Σ�, DMA_Mode_Circular����ͣ�ش��ͣ�
																														//-------ע�⣺��ָ��DMAͨ�����ݴ�������Ϊ�ڴ浽�ڴ�ʱ������ʹ��ѭ������ģʽ
  u32 DMA_Priority=DMA_Priority_VeryHigh;										//-------DMAͨ����ת�����ȼ�=========DMA_Priority_VeryHigh���ǳ��ߣ�DMA_Priority_High����)��DMA_Priority_Medium���У���DMA_Priority_Low���ͣ�																	
  u32 DMA_M2M=DMA_M2M_Disable;															//-------DMAͨ�����ڴ浽�ڴ洫��=====DMA_M2M_Enable(����Ϊ�ڴ浽�ڴ洫��)��DMA_M2M_Disable�����ڴ浽�ڴ洫�䣩
	
	
//	assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
	assert_param(IS_Peripheral_Request(Request));
//	assert_param(IS_FUNCTIONAL_STATE2(DISABLE));
	//1)**********DMAͨ��ѡ��
	if(Request==ADC1_RQ)					//---DMA1_CH1
	{
		DMAy_Channelx=DMA1_Channel1;
		DMA_PeripheralBaseAddr=(u32)&ADC1->DR;										//-------DMA�������ַ
		DMA_DIR=DMA_DIR_PeripheralSRC;														//-------DMA���ݴ��䷽��=============DMA_DIR_PeripheralDST��������ΪDMA��Ŀ�Ķˣ���DMA_DIR_PeripheralSRC��������Ϊ���ݴ������Դ��
		DMA_PeripheralInc=DMA_PeripheralInc_Disable;							//-------DMA�����ַ�Ĵ����������====DMA_PeripheralInc_Enable�������ַ�Ĵ�����������DMA_PeripheralInc_Disable�������ַ�Ĵ������䣩
		DMA_MemoryInc=DMA_MemoryInc_Disable;											//-------DMA�ڴ��ַ�Ĵ����������====DMA_MemoryInc_Enable���ڴ��ַ�Ĵ�����������DMA_MemoryInc_Disable���ڴ��ַ�Ĵ������䣩
		DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;		//-------DMA�������ݿ���=============DMA_PeripheralDataSize_Byte�����ݿ���Ϊ8λ����DMA_PeripheralDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_PeripheralDataSize_Word�����ݿ���Ϊ32λ��
		DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;						//-------DMA�ڴ����ݿ���=============DMA_MemoryDataSize_Byte�����ݿ���Ϊ8λ����DMA_MemoryDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_MemoryDataSize_Word�����ݿ���Ϊ32λ��
		DMA_Mode=DMA_Mode_Circular;																//-------DMA�Ĺ���ģʽ===============DMA_Mode_Normal��ֻ����һ�Σ�, DMA_Mode_Circular����ͣ�ش��ͣ�
		DMA_Priority=DMA_Priority_VeryHigh;												//-------DMAͨ����ת�����ȼ�=========DMA_Priority_VeryHigh���ǳ��ߣ�DMA_Priority_High����)��DMA_Priority_Medium���У���DMA_Priority_Low���ͣ�
		DMA_M2M=DMA_M2M_Disable;																	//-------DMAͨ�����ڴ浽�ڴ洫��=====DMA_M2M_Enable(����Ϊ�ڴ浽�ڴ洫��)��DMA_M2M_Disable�����ڴ浽�ڴ洫�䣩
		
		DMAy_FLAG_GLx=DMA1_FLAG_GL1;															//-------DMAȫ���жϱ�־
		
		DMA_IT_Interruupt=DISABLE;																//-------DMA��������ж�ʹ��
	}
	else if(Request==TIM2_CH3)
	{
		DMAy_Channelx=DMA1_Channel1;
		DMA_PeripheralBaseAddr=(u32)&TIM2->CCR3;										//-------DMA�������ַ
		DMA_DIR=DMA_DIR_PeripheralSRC;															//-------DMA���ݴ��䷽��=============DMA_DIR_PeripheralDST��������ΪDMA��Ŀ�Ķˣ���DMA_DIR_PeripheralSRC��������Ϊ���ݴ������Դ��
		DMA_PeripheralInc=DMA_PeripheralInc_Disable;								//-------DMA�����ַ�Ĵ����������====DMA_PeripheralInc_Enable�������ַ�Ĵ�����������DMA_PeripheralInc_Disable�������ַ�Ĵ������䣩
		DMA_MemoryInc=DMA_MemoryInc_Disable;												//-------DMA�ڴ��ַ�Ĵ����������====DMA_MemoryInc_Enable���ڴ��ַ�Ĵ�����������DMA_MemoryInc_Disable���ڴ��ַ�Ĵ������䣩
		DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;			//-------DMA�������ݿ���=============DMA_PeripheralDataSize_Byte�����ݿ���Ϊ8λ����DMA_PeripheralDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_PeripheralDataSize_Word�����ݿ���Ϊ32λ��
		DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;							//-------DMA�ڴ����ݿ���=============DMA_MemoryDataSize_Byte�����ݿ���Ϊ8λ����DMA_MemoryDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_MemoryDataSize_Word�����ݿ���Ϊ32λ��
		DMA_Mode=DMA_Mode_Normal;																		//-------DMA�Ĺ���ģʽ===============DMA_Mode_Normal��ֻ����һ�Σ�, DMA_Mode_Circular����ͣ�ش��ͣ�
		DMA_Priority=DMA_Priority_VeryHigh;													//-------DMAͨ����ת�����ȼ�=========DMA_Priority_VeryHigh���ǳ��ߣ�DMA_Priority_High����)��DMA_Priority_Medium���У���DMA_Priority_Low���ͣ�
		DMA_M2M=DMA_M2M_Disable;																		//-------DMAͨ�����ڴ浽�ڴ洫��=====DMA_M2M_Enable(����Ϊ�ڴ浽�ڴ洫��)��DMA_M2M_Disable�����ڴ浽�ڴ洫�䣩
		
		DMAy_FLAG_GLx=DMA1_FLAG_GL1;																//-------DMAȫ���жϱ�־
		
		DMA_IT_Interruupt=DISABLE;																	//-------DMA��������ж�ʹ��
	}
	else if(Request==TIM4_CH1)
	{
		DMAy_Channelx=DMA1_Channel1;
		DMA_PeripheralBaseAddr=(u32)&TIM4->CCR1;										//-------DMA�������ַ
		DMA_DIR=DMA_DIR_PeripheralSRC;															//-------DMA���ݴ��䷽��=============DMA_DIR_PeripheralDST��������ΪDMA��Ŀ�Ķˣ���DMA_DIR_PeripheralSRC��������Ϊ���ݴ������Դ��
		DMA_PeripheralInc=DMA_PeripheralInc_Disable;								//-------DMA�����ַ�Ĵ����������====DMA_PeripheralInc_Enable�������ַ�Ĵ�����������DMA_PeripheralInc_Disable�������ַ�Ĵ������䣩
		DMA_MemoryInc=DMA_MemoryInc_Disable;												//-------DMA�ڴ��ַ�Ĵ����������====DMA_MemoryInc_Enable���ڴ��ַ�Ĵ�����������DMA_MemoryInc_Disable���ڴ��ַ�Ĵ������䣩
		DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;					//-------DMA�������ݿ���=============DMA_PeripheralDataSize_Byte�����ݿ���Ϊ8λ����DMA_PeripheralDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_PeripheralDataSize_Word�����ݿ���Ϊ32λ��
		DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;									//-------DMA�ڴ����ݿ���=============DMA_MemoryDataSize_Byte�����ݿ���Ϊ8λ����DMA_MemoryDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_MemoryDataSize_Word�����ݿ���Ϊ32λ��
		DMA_Mode=DMA_Mode_Normal;																		//-------DMA�Ĺ���ģʽ===============DMA_Mode_Normal��ֻ����һ�Σ�, DMA_Mode_Circular����ͣ�ش��ͣ�
		DMA_Priority=DMA_Priority_VeryHigh;													//-------DMAͨ����ת�����ȼ�=========DMA_Priority_VeryHigh���ǳ��ߣ�DMA_Priority_High����)��DMA_Priority_Medium���У���DMA_Priority_Low���ͣ�
		DMA_M2M=DMA_M2M_Disable;																		//-------DMAͨ�����ڴ浽�ڴ洫��=====DMA_M2M_Enable(����Ϊ�ڴ浽�ڴ洫��)��DMA_M2M_Disable�����ڴ浽�ڴ洫�䣩
		
		DMAy_FLAG_GLx=DMA1_FLAG_GL1;																//-------DMAȫ���жϱ�־
		
		DMA_IT_Interruupt=DISABLE;																	//-------DMA��������ж�ʹ��
	}
	else if(Request==USART3_TX)		//---DMA1_CH2
	{
		DMAy_Channelx=DMA1_Channel2;
	}
	else if(Request==TIM1_CH1)
	{
		DMAy_Channelx=DMA1_Channel2;
	}
	else if(Request==TIM2_UP)
	{
		DMAy_Channelx=DMA1_Channel2;
	}
	else if(Request==TIM3_CH3)		
	{
		DMAy_Channelx=DMA1_Channel2;
	}
	else if(Request==SPI1_RX)
	{
		DMAy_Channelx=DMA1_Channel2;
	}
	else if(Request==USART3_RX)		//---DMA1_CH3
	{
		DMAy_Channelx=DMA1_Channel3;
	}
	else if(Request==TIM1_CH2)
	{
		DMAy_Channelx=DMA1_Channel3;
	}
	else if(Request==TIM3_CH4)
	{
		DMAy_Channelx=DMA1_Channel3;
	}
	else if(Request==TIM3_UP)
	{
		DMAy_Channelx=DMA1_Channel3;
	}
	else if(Request==SPI1_TX)
	{
		DMAy_Channelx=DMA1_Channel3;
	}
	else if(Request==USART1_TX)		//---DMA1_CH4
	{
		DMAy_Channelx=DMA1_Channel4;
		DMA_PeripheralBaseAddr=(u32)&USART1->DR;									//-------DMA�������ַ
		DMA_DIR=DMA_DIR_PeripheralDST;														//-------DMA���ݴ��䷽��=============DMA_DIR_PeripheralDST��������ΪDMA��Ŀ�Ķˣ���DMA_DIR_PeripheralSRC��������Ϊ���ݴ������Դ��
		DMA_PeripheralInc=DMA_PeripheralInc_Disable;							//-------DMA�����ַ�Ĵ����������====DMA_PeripheralInc_Enable�������ַ�Ĵ�����������DMA_PeripheralInc_Disable�������ַ�Ĵ������䣩
		DMA_MemoryInc=DMA_MemoryInc_Enable;												//-------DMA�ڴ��ַ�Ĵ����������====DMA_MemoryInc_Enable���ڴ��ַ�Ĵ�����������DMA_MemoryInc_Disable���ڴ��ַ�Ĵ������䣩
		DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;		//-------DMA�������ݿ���=============DMA_PeripheralDataSize_Byte�����ݿ���Ϊ8λ����DMA_PeripheralDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_PeripheralDataSize_Word�����ݿ���Ϊ32λ��
		DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;						//-------DMA�ڴ����ݿ���=============DMA_MemoryDataSize_Byte�����ݿ���Ϊ8λ����DMA_MemoryDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_MemoryDataSize_Word�����ݿ���Ϊ32λ��
		DMA_Mode=DMA_Mode_Normal;																	//-------DMA�Ĺ���ģʽ===============DMA_Mode_Normal��ֻ����һ�Σ�, DMA_Mode_Circular����ͣ�ش��ͣ�
		DMA_Priority=DMA_Priority_VeryHigh;												//-------DMAͨ����ת�����ȼ�=========DMA_Priority_VeryHigh���ǳ��ߣ�DMA_Priority_High����)��DMA_Priority_Medium���У���DMA_Priority_Low���ͣ�
		DMA_M2M=DMA_M2M_Disable;																	//-------DMAͨ�����ڴ浽�ڴ洫��=====DMA_M2M_Enable(����Ϊ�ڴ浽�ڴ洫��)��DMA_M2M_Disable�����ڴ浽�ڴ洫�䣩
		
		DMAy_FLAG_GLx=DMA1_FLAG_GL4;															//-------DMAȫ���жϱ�־
		
		DMA_IT_Interruupt=ENABLE;																	//-------DMA��������ж�ʹ��
	}
	else if(Request==TIM1_CH4)
	{
		DMAy_Channelx=DMA1_Channel4;
	}
	else if(Request==TIM1_TRIG)
	{
		DMAy_Channelx=DMA1_Channel4;
	}
	else if(Request==TIM1_COM)
	{
		DMAy_Channelx=DMA1_Channel4;
	}
	else if(Request==TIM4_CH2)
	{
		DMAy_Channelx=DMA1_Channel4;
	}
	else if(Request==SPI2_RX)
	{
		DMAy_Channelx=DMA1_Channel4;
	}
	else if(Request==I2S2_RX)
	{
		DMAy_Channelx=DMA1_Channel4;
	}
	else if(Request==I2C2_TX)
	{
		DMAy_Channelx=DMA1_Channel4;
	}
	else if(Request==USART1_RX)		//---DMA1_CH5
	{
		DMAy_Channelx=DMA1_Channel5;
		DMA_PeripheralBaseAddr=(u32)&USART1->DR;									//-------DMA�������ַ
		DMA_DIR=DMA_DIR_PeripheralSRC;														//-------DMA���ݴ��䷽��=============DMA_DIR_PeripheralDST��������ΪDMA��Ŀ�Ķˣ���DMA_DIR_PeripheralSRC��������Ϊ���ݴ������Դ��
		DMA_PeripheralInc=DMA_PeripheralInc_Disable;							//-------DMA�����ַ�Ĵ����������====DMA_PeripheralInc_Enable�������ַ�Ĵ�����������DMA_PeripheralInc_Disable�������ַ�Ĵ������䣩
		DMA_MemoryInc=DMA_MemoryInc_Enable;												//-------DMA�ڴ��ַ�Ĵ����������====DMA_MemoryInc_Enable���ڴ��ַ�Ĵ�����������DMA_MemoryInc_Disable���ڴ��ַ�Ĵ������䣩
		DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;		//-------DMA�������ݿ���=============DMA_PeripheralDataSize_Byte�����ݿ���Ϊ8λ����DMA_PeripheralDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_PeripheralDataSize_Word�����ݿ���Ϊ32λ��
		DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;						//-------DMA�ڴ����ݿ���=============DMA_MemoryDataSize_Byte�����ݿ���Ϊ8λ����DMA_MemoryDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_MemoryDataSize_Word�����ݿ���Ϊ32λ��
		DMA_Mode=DMA_Mode_Normal;																	//-------DMA�Ĺ���ģʽ===============DMA_Mode_Normal��ֻ����һ�Σ�, DMA_Mode_Circular����ͣ�ش��ͣ�
		DMA_Priority=DMA_Priority_VeryHigh;												//-------DMAͨ����ת�����ȼ�=========DMA_Priority_VeryHigh���ǳ��ߣ�DMA_Priority_High����)��DMA_Priority_Medium���У���DMA_Priority_Low���ͣ�
		DMA_M2M=DMA_M2M_Disable;																	//-------DMAͨ�����ڴ浽�ڴ洫��=====DMA_M2M_Enable(����Ϊ�ڴ浽�ڴ洫��)��DMA_M2M_Disable�����ڴ浽�ڴ洫�䣩
		
		DMAy_FLAG_GLx=DMA1_FLAG_GL5;															//-------DMAȫ���жϱ�־
		
		DMA_IT_Interruupt=DISABLE;																//-------DMA��������ж�ʹ��
	}
	else if(Request==TIM1_UP)
	{
		DMAy_Channelx=DMA1_Channel5;
	}
	else if(Request==SPI2_TX)
	{
		DMAy_Channelx=DMA1_Channel5;
	}
	else if(Request==I2S2_TX)
	{
		DMAy_Channelx=DMA1_Channel5;
	}
	else if(Request==I2C2_RX)
	{
		DMAy_Channelx=DMA1_Channel5;
	}
	else if(Request==TIM2_CH1)
	{
		DMAy_Channelx=DMA1_Channel5;
	}
	else if(Request==TIM4_CH3)
	{
		DMAy_Channelx=DMA1_Channel5;
	}
	else if(Request==USART2_RX)		//---DMA1_CH6
	{
		DMAy_Channelx=DMA1_Channel6;
	}
	else if(Request==TIM1_CH3)
	{
		DMAy_Channelx=DMA1_Channel6;
	}
	else if(Request==TIM3_CH1)
	{
		DMAy_Channelx=DMA1_Channel6;
	}
	else if(Request==TIM3_TRIG)
	{
		DMAy_Channelx=DMA1_Channel6;
	}
	else if(Request==I2C1_TX)
	{
		DMAy_Channelx=DMA1_Channel6;
	}
	else if(Request==USART2_TX)		//---DMA1_CH7
	{
		DMAy_Channelx=DMA1_Channel7;
	}
	else if(Request==TIM2_CH2)
	{
		DMAy_Channelx=DMA1_Channel7;
	}
	else if(Request==TIM2_CH4)
	{
		DMAy_Channelx=DMA1_Channel7;
	}
	else if(Request==TIM4_UP)
	{
		DMAy_Channelx=DMA1_Channel7;
	}
	else if(Request==I2C1_RX)
	{
		DMAy_Channelx=DMA1_Channel7;
	}
	else if(Request==TIM5_CH4)		//---DMA2_CH1
	{
		DMAy_Channelx=DMA2_Channel1;
	}
	else if(Request==TIM5_TRIG)
	{
		DMAy_Channelx=DMA2_Channel1;
	}
	else if(Request==TIM8_CH3)
	{
		DMAy_Channelx=DMA2_Channel1;
	}
	else if(Request==TIM8_UP)
	{
		DMAy_Channelx=DMA2_Channel1;
	}
	else if(Request==SPI3_RX)
	{
		DMAy_Channelx=DMA2_Channel1;
	}
	else if(Request==I2S3_RX)
	{
		DMAy_Channelx=DMA2_Channel1;
	}
	else if(Request==TIM8_CH4)		//---DMA2_CH2
	{
		DMAy_Channelx=DMA2_Channel2;
	}
	else if(Request==TIM8_TRIG)
	{
		DMAy_Channelx=DMA2_Channel2;
	}
	else if(Request==TIM8_COM)
	{
		DMAy_Channelx=DMA2_Channel2;
	}
	else if(Request==TIM5_CH3)
	{
		DMAy_Channelx=DMA2_Channel2;
	}
	else if(Request==TIM5_UP)
	{
		DMAy_Channelx=DMA2_Channel2;
	}
	else if(Request==SPI3_TX)
	{
		DMAy_Channelx=DMA2_Channel2;
	}
	else if(Request==I2S3_TX)
	{
		DMAy_Channelx=DMA2_Channel2;
	}
	else if(Request==TIM8_CH1)		//---DMA2_CH3
	{
		DMAy_Channelx=DMA2_Channel3;
	}
	else if(Request==UART4_RX)
	{
		DMAy_Channelx=DMA2_Channel3;
	}
	else if(Request==TIM6_UP)
	{
		DMAy_Channelx=DMA2_Channel3;
	}
	else if(Request==DAC_CH1)
	{
		DMAy_Channelx=DMA2_Channel3;
	}
	else if(Request==TIM5_CH2)		//---DMA2_CH4
	{
		DMAy_Channelx=DMA2_Channel4;
	}
	else if(Request==SDIO_RQ)
	{
		DMAy_Channelx=DMA2_Channel4;
	}
	else if(Request==TIM7_UP)
	{
		DMAy_Channelx=DMA2_Channel4;
	}
	else if(Request==DAC_CH2)
	{
		DMAy_Channelx=DMA2_Channel4;
	}
	else if(Request==ADC3_RQ)		//---DMA2_CH5
	{
		DMAy_Channelx=DMA2_Channel5;
	}
	else if(Request==TIM8_CH2)
	{
		DMAy_Channelx=DMA2_Channel5;
	}
	else if(Request==TIM5_CH1)
	{
		DMAy_Channelx=DMA2_Channel5;
	}
	else if(Request==UART4_TX)
	{
		DMAy_Channelx=DMA2_Channel5;
	}

	
	//2)**********����DMAʱ��	
	if(DMAy_Channelx>=DMA1_Channel1||DMAy_Channelx<=DMA1_Channel7)
	{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	}
	else if(DMAy_Channelx>=DMA2_Channel1||DMAy_Channelx<=DMA2_Channel5)
	{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);
	}

	//1)**********����ӦGPIOʱ�ӣ�����GPIO	
	//1)**********����ӦGPIOʱ�ӣ�����GPIO	
	//1)**********����ӦGPIOʱ�ӣ�����GPIO	
	
		
	
	/* Enable the DMA1 Interrupt */
	NVIC_Initstructure.NVIC_IRQChannel = DMAy_Channelx_IRQChannel;       			//ͨ������Ϊ����1�ж�
	NVIC_Initstructure.NVIC_IRQChannelSubPriority = 0;     										//�ж���Ӧ���ȼ�0
	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;        										//���ж�
	NVIC_Init(&NVIC_Initstructure);

	/*DMA��������*/
	DMA_Initstructure.DMA_PeripheralBaseAddr =  (u32)DMA_PeripheralBaseAddr;	//DMA����Դ��ַ
	DMA_Initstructure.DMA_MemoryBaseAddr     = (u32)DMA_MemoryBaseAddr;				//DMA�����ڴ��ַ
	DMA_Initstructure.DMA_DIR = (u32)DMA_DIR;																	//DMA_DIR_PeripheralDST��������ΪDMA��Ŀ�Ķˣ���DMA_DIR_PeripheralSRC��������Ϊ���ݴ������Դ��
	DMA_Initstructure.DMA_BufferSize = (u32)DMA_BufferSize; 									//ָ��DMAͨ����DMA����Ĵ�С
//	DMA_Initstructure.DMA_BufferSize = sizeof(MemoryAddr[0]); 							//ָ��DMAͨ����DMA����Ĵ�С
	DMA_Initstructure.DMA_PeripheralInc = (u32)DMA_PeripheralInc;							//DMA_PeripheralInc_Enable�������ַ�Ĵ�����������DMA_PeripheralInc_Disable�������ַ�Ĵ������䣩��
	DMA_Initstructure.DMA_MemoryInc =(u32)DMA_MemoryInc;											//DMA_MemoryInc_Enable���ڴ��ַ�Ĵ�����������DMA_MemoryInc_Disable���ڴ��ַ�Ĵ������䣩
	DMA_Initstructure.DMA_PeripheralDataSize = (u32)DMA_PeripheralDataSize;		//�������ݿ���--DMA_PeripheralDataSize_Byte�����ݿ���Ϊ8λ����DMA_PeripheralDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_PeripheralDataSize_Word�����ݿ���Ϊ32λ��
	DMA_Initstructure.DMA_MemoryDataSize = (u32)DMA_MemoryDataSize;						//�ڴ����ݿ���--DMA_MemoryDataSize_Byte�����ݿ���Ϊ8λ����DMA_MemoryDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_MemoryDataSize_Word�����ݿ���Ϊ32λ��
	DMA_Initstructure.DMA_Mode = (u32)DMA_Mode;																//DMA����ģʽ--DMA_Mode_Normal��ֻ����һ�Σ�, DMA_Mode_Circular����ͣ�ش��ͣ�
	DMA_Initstructure.DMA_Priority = (u32)DMA_Priority; 											//DMAͨ����ת�����ȼ�--DMA_Priority_VeryHigh���ǳ��ߣ�DMA_Priority_High����)��DMA_Priority_Medium���У���DMA_Priority_Low���ͣ�
	DMA_Initstructure.DMA_M2M = (u32)DMA_M2M;																	//DMAͨ�����ڴ浽�ڴ洫��--DMA_M2M_Enable(����Ϊ�ڴ浽�ڴ洫��)��DMA_M2M_Disable�����ڴ浽�ڴ洫�䣩
	DMA_Init(DMAy_Channelx,&DMA_Initstructure);																//��ʼ��DMA

	/* ���DMA1ͨ��4���б�־*/
	DMA_ClearFlag(DMAy_FLAG_GLx);                                 						// ���DMAȫ���жϱ�־
	/* �ر�DMA1ͨ��5*/
	DMA_Cmd(DMAy_Channelx,DISABLE);
	/* ʹ��DMA1ͨ��4��������ж� */
	DMA_ITConfig(DMAy_Channelx,DMA_IT_TC, DMA_IT_Interruupt);									//DMA������������ж�


//	/*DMA��������*/
//	DMA_Initstructure.DMA_PeripheralBaseAddr =  (u32)(&USART1->DR);					//DMA����Դ��ַ
//	DMA_Initstructure.DMA_MemoryBaseAddr     = 	(u32)MemoryAddr;						//DMA�����ڴ��ַ
//	DMA_Initstructure.DMA_DIR = DMA_DIR_PeripheralSRC;											//DMA_DIR_PeripheralDST��������ΪDMA��Ŀ�Ķˣ���DMA_DIR_PeripheralSRC��������Ϊ���ݴ������Դ��
//	DMA_Initstructure.DMA_BufferSize = BufferSize; 													//ָ��DMAͨ����DMA����Ĵ�С
//	DMA_Initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//DMA_PeripheralInc_Enable�������ַ�Ĵ�����������DMA_PeripheralInc_Disable�������ַ�Ĵ������䣩��
//	DMA_Initstructure.DMA_MemoryInc =DMA_MemoryInc_Enable;									//DMA_MemoryInc_Enable���ڴ��ַ�Ĵ�����������DMA_MemoryInc_Disable���ڴ��ַ�Ĵ������䣩
//	DMA_Initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//�������ݿ���--DMA_PeripheralDataSize_Byte�����ݿ���Ϊ8λ����DMA_PeripheralDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_PeripheralDataSize_Word�����ݿ���Ϊ32λ��
//	DMA_Initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//�ڴ����ݿ���--DMA_MemoryDataSize_Byte�����ݿ���Ϊ8λ����DMA_MemoryDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_MemoryDataSize_Word�����ݿ���Ϊ32λ��
//	DMA_Initstructure.DMA_Mode = DMA_Mode_Normal;														//DMA����ģʽ--DMA_Mode_Normal��ֻ����һ�Σ�, DMA_Mode_Circular����ͣ�ش��ͣ�
//	DMA_Initstructure.DMA_Priority = DMA_Priority_High; 										//DMAͨ����ת�����ȼ�--DMA_Priority_VeryHigh���ǳ��ߣ�DMA_Priority_High����)��DMA_Priority_Medium���У���DMA_Priority_Low���ͣ�
//	DMA_Initstructure.DMA_M2M = DMA_M2M_Disable;														//DMAͨ�����ڴ浽�ڴ洫��--DMA_M2M_Enable(����Ϊ�ڴ浽�ڴ洫��)��DMA_M2M_Disable�����ڴ浽�ڴ洫�䣩
//	DMA_Init(DMA1_Channel5,&DMA_Initstructure);															//��ʼ��DMA
//	
//	/* ���DMA1ͨ��5���б�־*/
//	DMA_ClearFlag(DMA1_FLAG_GL5);                                 					// ���DMA���б�־
//	/* ����DMA1ͨ��5*/
//	DMA_Cmd(DMA1_Channel5,ENABLE);
}

/*******************************************************************************
*������		:DMA_Server
*��������	:DMA�жϷ�����
*����			: 
*���			:��
*����ֵ		:��
*����			��
*******************************************************************************/
void DMA_Server(void)
{ 
	WOW_Server();				//������
//#ifdef	RS485_TO_USART_PRJ
//	GPIO_ResetBits(GPIOB,GPIO_Pin_8);	
//#endif
//#ifdef	PD002T20_PRJ
//	GPIO_ResetBits(GPIOA,GPIO_Pin_1);	
//#endif
		/***USART3-TXD***/
	if(DMA_GetFlagStatus(DMA1_IT_GL2)== SET)
	{
			/*�����־λ*/
		DMA_ClearFlag(DMA1_FLAG_GL2); 
		/*�ر�DMA����*/ 
		DMA_Cmd(DMA1_Channel2,DISABLE); 
	}
	/***USART-TXD***/
	else if(DMA_GetFlagStatus(DMA1_IT_GL4)== SET)
	{
		/*�����־λ*/
		DMA_ClearFlag(DMA1_FLAG_GL4); 
		/*�ر�DMA����*/ 
		DMA_Cmd(DMA1_Channel4,DISABLE);
	}
	/***USART2-TXD***/
	else if(DMA_GetFlagStatus(DMA1_IT_GL7)== SET)
	{
			/*�����־λ*/
		DMA_ClearFlag(DMA1_FLAG_GL7); 
		/*�ر�DMA����*/ 
		DMA_Cmd(DMA1_Channel7,DISABLE); 
	}

	/***USART4-TXD***/
	else if(DMA_GetFlagStatus(DMA2_IT_GL5)== SET)
	{
			/*�����־λ*/
		DMA_ClearFlag(DMA2_FLAG_GL5); 
		/*�ر�DMA����*/ 
		DMA_Cmd(DMA2_Channel5,DISABLE); 
	}
	
/************************************************************************
***********************����2�շ�****************************************

if(USART_GetITStatus(USART2,USART_IT_IDLE))
  {
			USART_ClearITPendingBit(USART2,USART_IT_IDLE); 					//������д��ڱ�־λ
			num = USART2->DR; 												//������������������־
			DMA_Cmd(DMA1_Channel6,DISABLE);    										//�رս���DMA
			num = DMA1_BufferSize -  DMA_GetCurrDataCounter(DMA1_Channel6);	//�õ������������ݸ���
			DMA1_Channel7->CNDTR =num;										//�趨���������ݸ���
			DMA1_Channel6->CNDTR=DMA1_BufferSize;       					//�������ý������ݸ���				
			DMA_Cmd(DMA1_Channel6,ENABLE);  								//��������DMA			
			DMA_Cmd(DMA1_Channel7,ENABLE);									//DMA���Ϳ���
	}
	else if(DMA_GetFlagStatus(DMA1_FLAG_GL7))
	{
		DMA_ClearFlag(DMA1_FLAG_GL7);
		DMA_Cmd(DMA1_Channel7,DISABLE);									//DMA���Ϳ���
	}
*************************************************************************
************************************************************************/
	
}













