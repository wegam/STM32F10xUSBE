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


#include "STM32_DMA.H"
#include "STM32_WOW.H"
//#include "STM32F10x_BitBand.H"

/*##############################################################################
################################################################################
# 模块名		:	STM32_DMA	
# 功能描述	:	DMA配置使用
-------------------------------------------------------------------------------	
********DMA介绍:********
直接存储器存取(DMA)用来提供在外设和存储器之间或者存储器和存储器之间的高速数据传输。
无须CPU干预，数据可以通过DMA快速地移动，这就节省了CPU的资源来做其他操作。 两个DMA
控制器有12个通道(DMA1有7个通道，DMA2有5个通道)，每个通道专门用来管理来自于一个或
多个外设对存储器访问的请求。还有一个仲裁器来协调各个DMA请求的优先权。
-------------------------------------------------------------------------------
********DMA主要特征:********
● 12个独立的可配置的通道(请求)：DMA1有7个通道，DMA2有5个通道 
● 每个通道都直接连接专用的硬件DMA请求，每个通道都同样支持软件触发。这些功能通过软件来配置。 
● 在同一个DMA模块上，多个请求间的优先权可以通过软件编程设置(共有四级：很高、高、中等和低)，优先权设置相等时由硬件决定(请求0优先于请求1，依此类推) 。 
● 独立数据源和目标数据区的传输宽度(字节、半字、全字)，模拟打包和拆包的过程。源和目标地址必须按数据传输宽度对齐。 
● 支持循环的缓冲器管理 
● 每个通道都有3个事件标志(DMA半传输、DMA传输完成和DMA传输出错)，这3个事件标志逻辑或成为一个单独的中断请求。 
● 存储器和存储器间的传输 
● 外设和存储器、存储器和外设之间的传输 
● 闪存、SRAM、外设的SRAM、APB1、APB2和AHB外设均可作为访问的源和目标。 
● 可编程的数据传输数目：最大为65535

*******************************************DMA1请求映像：DMA1_Channelx****************************************************
-------------------------------------------------------------------------------------------------------------------------
外设		|			CH1				|			CH2				|			CH3				|			CH4				|			CH5				|			CH6				|			CH7				|
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

*******************************************DMA2请求映像：DMA2_Channelx****************************************************
-------------------------------------------------------------------------------------------------------------------------
外设		|			CH1				|			CH2				|			CH3				|			CH4				|			CH5				|
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

//DMA结构体
//typedef struct
//{
//  u32 DMA_PeripheralBaseAddr;		//-------DMA外设基地址
//  u32 DMA_MemoryBaseAddr;				//-------DMA内存基地址
//  u32 DMA_DIR;									//-------DMA数据传输方向=============DMA_DIR_PeripheralDST（外设作为DMA的目的端），DMA_DIR_PeripheralSRC（外设作为数据传输的来源）
//  u32 DMA_BufferSize;						//-------DMA通道的DMA缓存的大小======单位为数据单位。根据传输方向，数据单位等于结构中参数DMA_PeripheralDataSize或者参数DMA_MemoryDataSize的值。
//  u32 DMA_PeripheralInc;				//-------DMA外设地址寄存器递增与否====DMA_PeripheralInc_Enable（外设地址寄存器递增），DMA_PeripheralInc_Disable（外设地址寄存器不变）
//  u32 DMA_MemoryInc;						//-------DMA内存地址寄存器递增与否====DMA_MemoryInc_Enable（内存地址寄存器递增），DMA_MemoryInc_Disable（内存地址寄存器不变）
//  u32 DMA_PeripheralDataSize;		//-------DMA外设数据宽度=============DMA_PeripheralDataSize_Byte（数据宽度为8位），DMA_PeripheralDataSize_HalfWord（数据宽度为16位），DMA_PeripheralDataSize_Word（数据宽度为32位）
//  u32 DMA_MemoryDataSize;				//-------DMA内存数据宽度=============DMA_MemoryDataSize_Byte（数据宽度为8位），DMA_MemoryDataSize_HalfWord（数据宽度为16位），DMA_MemoryDataSize_Word（数据宽度为32位）
//  u32 DMA_Mode;									//-------DMA的工作模式===============DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular（不停地传送）
																	//-------注意：当指定DMA通道数据传输配置为内存到内存时，不能使用循环缓存模式
//  u32 DMA_Priority;							//-------DMA通道的转输优先级=========DMA_Priority_VeryHigh（非常高）DMA_Priority_High（高)，DMA_Priority_Medium（中），DMA_Priority_Low（低）																	
//  u32 DMA_M2M;									//-------DMA通道的内存到内存传输=====DMA_M2M_Enable(设置为内存到内存传输)，DMA_M2M_Disable（非内存到内存传输）
//}DMA_InitTypeDef;

//u32 *DMA_Buffer;
//u8	DMA_BufferSize=0;
/*******************************************************************************
*函数名		:	DMA_Configuration
*功能描述	:	串口配置
*输入			:	MemoryAddr--内存地址,
						BufferSize--缓存大小
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void DMA_Configuration(Peripheral_Request_TypeDef	Request,u32 *MemoryAddr,u32 BufferSize)	//DMA配置
{
	DMA_InitTypeDef DMA_Initstructure;
	NVIC_InitTypeDef NVIC_Initstructure;

	DMA_Channel_TypeDef* DMAy_Channelx=0;											//-------DMA通道

	FunctionalState	DMA_IT_Interruupt=ENABLE;									//-------DMA传输完成中断使能
	u8	DMAy_Channelx_IRQChannel=DMA1_Channel1_IRQChannel;		//-------DMA中断源
	u32	DMAy_FLAG_GLx=0;																			//-------DMA全局中断标志
	
	
	u32 DMA_PeripheralBaseAddr=0;															//-------DMA外设基地址
  u32 DMA_MemoryBaseAddr=(u32)MemoryAddr;										//-------DMA内存基地址
  u32 DMA_DIR=DMA_DIR_PeripheralSRC;												//-------DMA数据传输方向=============DMA_DIR_PeripheralDST（外设作为DMA的目的端），DMA_DIR_PeripheralSRC（外设作为数据传输的来源）
  u32 DMA_BufferSize=BufferSize;														//-------DMA通道的DMA缓存的大小======单位为数据单位。根据传输方向，数据单位等于结构中参数DMA_PeripheralDataSize或者参数DMA_MemoryDataSize的值。
  u32 DMA_PeripheralInc=DMA_PeripheralInc_Disable;					//-------DMA外设地址寄存器递增与否====DMA_PeripheralInc_Enable（外设地址寄存器递增），DMA_PeripheralInc_Disable（外设地址寄存器不变）
  u32 DMA_MemoryInc=DMA_MemoryInc_Disable;									//-------DMA内存地址寄存器递增与否====DMA_MemoryInc_Enable（内存地址寄存器递增），DMA_MemoryInc_Disable（内存地址寄存器不变）
  u32 DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;		//-------DMA外设数据宽度=============DMA_PeripheralDataSize_Byte（数据宽度为8位），DMA_PeripheralDataSize_HalfWord（数据宽度为16位），DMA_PeripheralDataSize_Word（数据宽度为32位）
  u32 DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;						//-------DMA内存数据宽度=============DMA_MemoryDataSize_Byte（数据宽度为8位），DMA_MemoryDataSize_HalfWord（数据宽度为16位），DMA_MemoryDataSize_Word（数据宽度为32位）
  u32 DMA_Mode=DMA_Mode_Normal;															//-------DMA的工作模式===============DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular（不停地传送）
																														//-------注意：当指定DMA通道数据传输配置为内存到内存时，不能使用循环缓存模式
  u32 DMA_Priority=DMA_Priority_VeryHigh;										//-------DMA通道的转输优先级=========DMA_Priority_VeryHigh（非常高）DMA_Priority_High（高)，DMA_Priority_Medium（中），DMA_Priority_Low（低）																	
  u32 DMA_M2M=DMA_M2M_Disable;															//-------DMA通道的内存到内存传输=====DMA_M2M_Enable(设置为内存到内存传输)，DMA_M2M_Disable（非内存到内存传输）
	
	
//	assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
	assert_param(IS_Peripheral_Request(Request));
//	assert_param(IS_FUNCTIONAL_STATE2(DISABLE));
	//1)**********DMA通道选择
	if(Request==ADC1_RQ)					//---DMA1_CH1
	{
		DMAy_Channelx=DMA1_Channel1;
		DMA_PeripheralBaseAddr=(u32)&ADC1->DR;										//-------DMA外设基地址
		DMA_DIR=DMA_DIR_PeripheralSRC;														//-------DMA数据传输方向=============DMA_DIR_PeripheralDST（外设作为DMA的目的端），DMA_DIR_PeripheralSRC（外设作为数据传输的来源）
		DMA_PeripheralInc=DMA_PeripheralInc_Disable;							//-------DMA外设地址寄存器递增与否====DMA_PeripheralInc_Enable（外设地址寄存器递增），DMA_PeripheralInc_Disable（外设地址寄存器不变）
		DMA_MemoryInc=DMA_MemoryInc_Disable;											//-------DMA内存地址寄存器递增与否====DMA_MemoryInc_Enable（内存地址寄存器递增），DMA_MemoryInc_Disable（内存地址寄存器不变）
		DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;		//-------DMA外设数据宽度=============DMA_PeripheralDataSize_Byte（数据宽度为8位），DMA_PeripheralDataSize_HalfWord（数据宽度为16位），DMA_PeripheralDataSize_Word（数据宽度为32位）
		DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;						//-------DMA内存数据宽度=============DMA_MemoryDataSize_Byte（数据宽度为8位），DMA_MemoryDataSize_HalfWord（数据宽度为16位），DMA_MemoryDataSize_Word（数据宽度为32位）
		DMA_Mode=DMA_Mode_Circular;																//-------DMA的工作模式===============DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular（不停地传送）
		DMA_Priority=DMA_Priority_VeryHigh;												//-------DMA通道的转输优先级=========DMA_Priority_VeryHigh（非常高）DMA_Priority_High（高)，DMA_Priority_Medium（中），DMA_Priority_Low（低）
		DMA_M2M=DMA_M2M_Disable;																	//-------DMA通道的内存到内存传输=====DMA_M2M_Enable(设置为内存到内存传输)，DMA_M2M_Disable（非内存到内存传输）
		
		DMAy_FLAG_GLx=DMA1_FLAG_GL1;															//-------DMA全局中断标志
		
		DMA_IT_Interruupt=DISABLE;																//-------DMA传输完成中断使能
	}
	else if(Request==TIM2_CH3)
	{
		DMAy_Channelx=DMA1_Channel1;
		DMA_PeripheralBaseAddr=(u32)&TIM2->CCR3;										//-------DMA外设基地址
		DMA_DIR=DMA_DIR_PeripheralSRC;															//-------DMA数据传输方向=============DMA_DIR_PeripheralDST（外设作为DMA的目的端），DMA_DIR_PeripheralSRC（外设作为数据传输的来源）
		DMA_PeripheralInc=DMA_PeripheralInc_Disable;								//-------DMA外设地址寄存器递增与否====DMA_PeripheralInc_Enable（外设地址寄存器递增），DMA_PeripheralInc_Disable（外设地址寄存器不变）
		DMA_MemoryInc=DMA_MemoryInc_Disable;												//-------DMA内存地址寄存器递增与否====DMA_MemoryInc_Enable（内存地址寄存器递增），DMA_MemoryInc_Disable（内存地址寄存器不变）
		DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;			//-------DMA外设数据宽度=============DMA_PeripheralDataSize_Byte（数据宽度为8位），DMA_PeripheralDataSize_HalfWord（数据宽度为16位），DMA_PeripheralDataSize_Word（数据宽度为32位）
		DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;							//-------DMA内存数据宽度=============DMA_MemoryDataSize_Byte（数据宽度为8位），DMA_MemoryDataSize_HalfWord（数据宽度为16位），DMA_MemoryDataSize_Word（数据宽度为32位）
		DMA_Mode=DMA_Mode_Normal;																		//-------DMA的工作模式===============DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular（不停地传送）
		DMA_Priority=DMA_Priority_VeryHigh;													//-------DMA通道的转输优先级=========DMA_Priority_VeryHigh（非常高）DMA_Priority_High（高)，DMA_Priority_Medium（中），DMA_Priority_Low（低）
		DMA_M2M=DMA_M2M_Disable;																		//-------DMA通道的内存到内存传输=====DMA_M2M_Enable(设置为内存到内存传输)，DMA_M2M_Disable（非内存到内存传输）
		
		DMAy_FLAG_GLx=DMA1_FLAG_GL1;																//-------DMA全局中断标志
		
		DMA_IT_Interruupt=DISABLE;																	//-------DMA传输完成中断使能
	}
	else if(Request==TIM4_CH1)
	{
		DMAy_Channelx=DMA1_Channel1;
		DMA_PeripheralBaseAddr=(u32)&TIM4->CCR1;										//-------DMA外设基地址
		DMA_DIR=DMA_DIR_PeripheralSRC;															//-------DMA数据传输方向=============DMA_DIR_PeripheralDST（外设作为DMA的目的端），DMA_DIR_PeripheralSRC（外设作为数据传输的来源）
		DMA_PeripheralInc=DMA_PeripheralInc_Disable;								//-------DMA外设地址寄存器递增与否====DMA_PeripheralInc_Enable（外设地址寄存器递增），DMA_PeripheralInc_Disable（外设地址寄存器不变）
		DMA_MemoryInc=DMA_MemoryInc_Disable;												//-------DMA内存地址寄存器递增与否====DMA_MemoryInc_Enable（内存地址寄存器递增），DMA_MemoryInc_Disable（内存地址寄存器不变）
		DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;					//-------DMA外设数据宽度=============DMA_PeripheralDataSize_Byte（数据宽度为8位），DMA_PeripheralDataSize_HalfWord（数据宽度为16位），DMA_PeripheralDataSize_Word（数据宽度为32位）
		DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;									//-------DMA内存数据宽度=============DMA_MemoryDataSize_Byte（数据宽度为8位），DMA_MemoryDataSize_HalfWord（数据宽度为16位），DMA_MemoryDataSize_Word（数据宽度为32位）
		DMA_Mode=DMA_Mode_Normal;																		//-------DMA的工作模式===============DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular（不停地传送）
		DMA_Priority=DMA_Priority_VeryHigh;													//-------DMA通道的转输优先级=========DMA_Priority_VeryHigh（非常高）DMA_Priority_High（高)，DMA_Priority_Medium（中），DMA_Priority_Low（低）
		DMA_M2M=DMA_M2M_Disable;																		//-------DMA通道的内存到内存传输=====DMA_M2M_Enable(设置为内存到内存传输)，DMA_M2M_Disable（非内存到内存传输）
		
		DMAy_FLAG_GLx=DMA1_FLAG_GL1;																//-------DMA全局中断标志
		
		DMA_IT_Interruupt=DISABLE;																	//-------DMA传输完成中断使能
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
		DMA_PeripheralBaseAddr=(u32)&USART1->DR;									//-------DMA外设基地址
		DMA_DIR=DMA_DIR_PeripheralDST;														//-------DMA数据传输方向=============DMA_DIR_PeripheralDST（外设作为DMA的目的端），DMA_DIR_PeripheralSRC（外设作为数据传输的来源）
		DMA_PeripheralInc=DMA_PeripheralInc_Disable;							//-------DMA外设地址寄存器递增与否====DMA_PeripheralInc_Enable（外设地址寄存器递增），DMA_PeripheralInc_Disable（外设地址寄存器不变）
		DMA_MemoryInc=DMA_MemoryInc_Enable;												//-------DMA内存地址寄存器递增与否====DMA_MemoryInc_Enable（内存地址寄存器递增），DMA_MemoryInc_Disable（内存地址寄存器不变）
		DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;		//-------DMA外设数据宽度=============DMA_PeripheralDataSize_Byte（数据宽度为8位），DMA_PeripheralDataSize_HalfWord（数据宽度为16位），DMA_PeripheralDataSize_Word（数据宽度为32位）
		DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;						//-------DMA内存数据宽度=============DMA_MemoryDataSize_Byte（数据宽度为8位），DMA_MemoryDataSize_HalfWord（数据宽度为16位），DMA_MemoryDataSize_Word（数据宽度为32位）
		DMA_Mode=DMA_Mode_Normal;																	//-------DMA的工作模式===============DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular（不停地传送）
		DMA_Priority=DMA_Priority_VeryHigh;												//-------DMA通道的转输优先级=========DMA_Priority_VeryHigh（非常高）DMA_Priority_High（高)，DMA_Priority_Medium（中），DMA_Priority_Low（低）
		DMA_M2M=DMA_M2M_Disable;																	//-------DMA通道的内存到内存传输=====DMA_M2M_Enable(设置为内存到内存传输)，DMA_M2M_Disable（非内存到内存传输）
		
		DMAy_FLAG_GLx=DMA1_FLAG_GL4;															//-------DMA全局中断标志
		
		DMA_IT_Interruupt=ENABLE;																	//-------DMA传输完成中断使能
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
		DMA_PeripheralBaseAddr=(u32)&USART1->DR;									//-------DMA外设基地址
		DMA_DIR=DMA_DIR_PeripheralSRC;														//-------DMA数据传输方向=============DMA_DIR_PeripheralDST（外设作为DMA的目的端），DMA_DIR_PeripheralSRC（外设作为数据传输的来源）
		DMA_PeripheralInc=DMA_PeripheralInc_Disable;							//-------DMA外设地址寄存器递增与否====DMA_PeripheralInc_Enable（外设地址寄存器递增），DMA_PeripheralInc_Disable（外设地址寄存器不变）
		DMA_MemoryInc=DMA_MemoryInc_Enable;												//-------DMA内存地址寄存器递增与否====DMA_MemoryInc_Enable（内存地址寄存器递增），DMA_MemoryInc_Disable（内存地址寄存器不变）
		DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;		//-------DMA外设数据宽度=============DMA_PeripheralDataSize_Byte（数据宽度为8位），DMA_PeripheralDataSize_HalfWord（数据宽度为16位），DMA_PeripheralDataSize_Word（数据宽度为32位）
		DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;						//-------DMA内存数据宽度=============DMA_MemoryDataSize_Byte（数据宽度为8位），DMA_MemoryDataSize_HalfWord（数据宽度为16位），DMA_MemoryDataSize_Word（数据宽度为32位）
		DMA_Mode=DMA_Mode_Normal;																	//-------DMA的工作模式===============DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular（不停地传送）
		DMA_Priority=DMA_Priority_VeryHigh;												//-------DMA通道的转输优先级=========DMA_Priority_VeryHigh（非常高）DMA_Priority_High（高)，DMA_Priority_Medium（中），DMA_Priority_Low（低）
		DMA_M2M=DMA_M2M_Disable;																	//-------DMA通道的内存到内存传输=====DMA_M2M_Enable(设置为内存到内存传输)，DMA_M2M_Disable（非内存到内存传输）
		
		DMAy_FLAG_GLx=DMA1_FLAG_GL5;															//-------DMA全局中断标志
		
		DMA_IT_Interruupt=DISABLE;																//-------DMA传输完成中断使能
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

	
	//2)**********开启DMA时钟	
	if(DMAy_Channelx>=DMA1_Channel1||DMAy_Channelx<=DMA1_Channel7)
	{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	}
	else if(DMAy_Channelx>=DMA2_Channel1||DMAy_Channelx<=DMA2_Channel5)
	{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);
	}

	//1)**********打开相应GPIO时钟，配置GPIO	
	//1)**********打开相应GPIO时钟，配置GPIO	
	//1)**********打开相应GPIO时钟，配置GPIO	
	
		
	
	/* Enable the DMA1 Interrupt */
	NVIC_Initstructure.NVIC_IRQChannel = DMAy_Channelx_IRQChannel;       			//通道设置为串口1中断
	NVIC_Initstructure.NVIC_IRQChannelSubPriority = 0;     										//中断响应优先级0
	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;        										//打开中断
	NVIC_Init(&NVIC_Initstructure);

	/*DMA发送配置*/
	DMA_Initstructure.DMA_PeripheralBaseAddr =  (u32)DMA_PeripheralBaseAddr;	//DMA外设源地址
	DMA_Initstructure.DMA_MemoryBaseAddr     = (u32)DMA_MemoryBaseAddr;				//DMA数据内存地址
	DMA_Initstructure.DMA_DIR = (u32)DMA_DIR;																	//DMA_DIR_PeripheralDST（外设作为DMA的目的端），DMA_DIR_PeripheralSRC（外设作为数据传输的来源）
	DMA_Initstructure.DMA_BufferSize = (u32)DMA_BufferSize; 									//指定DMA通道的DMA缓存的大小
//	DMA_Initstructure.DMA_BufferSize = sizeof(MemoryAddr[0]); 							//指定DMA通道的DMA缓存的大小
	DMA_Initstructure.DMA_PeripheralInc = (u32)DMA_PeripheralInc;							//DMA_PeripheralInc_Enable（外设地址寄存器递增），DMA_PeripheralInc_Disable（外设地址寄存器不变），
	DMA_Initstructure.DMA_MemoryInc =(u32)DMA_MemoryInc;											//DMA_MemoryInc_Enable（内存地址寄存器递增），DMA_MemoryInc_Disable（内存地址寄存器不变）
	DMA_Initstructure.DMA_PeripheralDataSize = (u32)DMA_PeripheralDataSize;		//外设数据宽度--DMA_PeripheralDataSize_Byte（数据宽度为8位），DMA_PeripheralDataSize_HalfWord（数据宽度为16位），DMA_PeripheralDataSize_Word（数据宽度为32位）
	DMA_Initstructure.DMA_MemoryDataSize = (u32)DMA_MemoryDataSize;						//内存数据宽度--DMA_MemoryDataSize_Byte（数据宽度为8位），DMA_MemoryDataSize_HalfWord（数据宽度为16位），DMA_MemoryDataSize_Word（数据宽度为32位）
	DMA_Initstructure.DMA_Mode = (u32)DMA_Mode;																//DMA工作模式--DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular（不停地传送）
	DMA_Initstructure.DMA_Priority = (u32)DMA_Priority; 											//DMA通道的转输优先级--DMA_Priority_VeryHigh（非常高）DMA_Priority_High（高)，DMA_Priority_Medium（中），DMA_Priority_Low（低）
	DMA_Initstructure.DMA_M2M = (u32)DMA_M2M;																	//DMA通道的内存到内存传输--DMA_M2M_Enable(设置为内存到内存传输)，DMA_M2M_Disable（非内存到内存传输）
	DMA_Init(DMAy_Channelx,&DMA_Initstructure);																//初始化DMA

	/* 清除DMA1通道4所有标志*/
	DMA_ClearFlag(DMAy_FLAG_GLx);                                 						// 清除DMA全局中断标志
	/* 关闭DMA1通道5*/
	DMA_Cmd(DMAy_Channelx,DISABLE);
	/* 使能DMA1通道4传输完成中断 */
	DMA_ITConfig(DMAy_Channelx,DMA_IT_TC, DMA_IT_Interruupt);									//DMA开启传输完成中断


//	/*DMA接收配置*/
//	DMA_Initstructure.DMA_PeripheralBaseAddr =  (u32)(&USART1->DR);					//DMA外设源地址
//	DMA_Initstructure.DMA_MemoryBaseAddr     = 	(u32)MemoryAddr;						//DMA数据内存地址
//	DMA_Initstructure.DMA_DIR = DMA_DIR_PeripheralSRC;											//DMA_DIR_PeripheralDST（外设作为DMA的目的端），DMA_DIR_PeripheralSRC（外设作为数据传输的来源）
//	DMA_Initstructure.DMA_BufferSize = BufferSize; 													//指定DMA通道的DMA缓存的大小
//	DMA_Initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//DMA_PeripheralInc_Enable（外设地址寄存器递增），DMA_PeripheralInc_Disable（外设地址寄存器不变），
//	DMA_Initstructure.DMA_MemoryInc =DMA_MemoryInc_Enable;									//DMA_MemoryInc_Enable（内存地址寄存器递增），DMA_MemoryInc_Disable（内存地址寄存器不变）
//	DMA_Initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//外设数据宽度--DMA_PeripheralDataSize_Byte（数据宽度为8位），DMA_PeripheralDataSize_HalfWord（数据宽度为16位），DMA_PeripheralDataSize_Word（数据宽度为32位）
//	DMA_Initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//内存数据宽度--DMA_MemoryDataSize_Byte（数据宽度为8位），DMA_MemoryDataSize_HalfWord（数据宽度为16位），DMA_MemoryDataSize_Word（数据宽度为32位）
//	DMA_Initstructure.DMA_Mode = DMA_Mode_Normal;														//DMA工作模式--DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular（不停地传送）
//	DMA_Initstructure.DMA_Priority = DMA_Priority_High; 										//DMA通道的转输优先级--DMA_Priority_VeryHigh（非常高）DMA_Priority_High（高)，DMA_Priority_Medium（中），DMA_Priority_Low（低）
//	DMA_Initstructure.DMA_M2M = DMA_M2M_Disable;														//DMA通道的内存到内存传输--DMA_M2M_Enable(设置为内存到内存传输)，DMA_M2M_Disable（非内存到内存传输）
//	DMA_Init(DMA1_Channel5,&DMA_Initstructure);															//初始化DMA
//	
//	/* 清除DMA1通道5所有标志*/
//	DMA_ClearFlag(DMA1_FLAG_GL5);                                 					// 清除DMA所有标志
//	/* 启动DMA1通道5*/
//	DMA_Cmd(DMA1_Channel5,ENABLE);
}

/*******************************************************************************
*函数名		:DMA_Server
*功能描述	:DMA中断服务函数
*输入			: 
*输出			:无
*返回值		:无
*例程			：
*******************************************************************************/
void DMA_Server(void)
{ 
	WOW_Server();				//服务函数
//#ifdef	RS485_TO_USART_PRJ
//	GPIO_ResetBits(GPIOB,GPIO_Pin_8);	
//#endif
//#ifdef	PD002T20_PRJ
//	GPIO_ResetBits(GPIOA,GPIO_Pin_1);	
//#endif
		/***USART3-TXD***/
	if(DMA_GetFlagStatus(DMA1_IT_GL2)== SET)
	{
			/*清除标志位*/
		DMA_ClearFlag(DMA1_FLAG_GL2); 
		/*关闭DMA发送*/ 
		DMA_Cmd(DMA1_Channel2,DISABLE); 
	}
	/***USART-TXD***/
	else if(DMA_GetFlagStatus(DMA1_IT_GL4)== SET)
	{
		/*清除标志位*/
		DMA_ClearFlag(DMA1_FLAG_GL4); 
		/*关闭DMA发送*/ 
		DMA_Cmd(DMA1_Channel4,DISABLE);
	}
	/***USART2-TXD***/
	else if(DMA_GetFlagStatus(DMA1_IT_GL7)== SET)
	{
			/*清除标志位*/
		DMA_ClearFlag(DMA1_FLAG_GL7); 
		/*关闭DMA发送*/ 
		DMA_Cmd(DMA1_Channel7,DISABLE); 
	}

	/***USART4-TXD***/
	else if(DMA_GetFlagStatus(DMA2_IT_GL5)== SET)
	{
			/*清除标志位*/
		DMA_ClearFlag(DMA2_FLAG_GL5); 
		/*关闭DMA发送*/ 
		DMA_Cmd(DMA2_Channel5,DISABLE); 
	}
	
/************************************************************************
***********************串口2收发****************************************

if(USART_GetITStatus(USART2,USART_IT_IDLE))
  {
			USART_ClearITPendingBit(USART2,USART_IT_IDLE); 					//清除空闲串口标志位
			num = USART2->DR; 												//读出数据以完成清除标志
			DMA_Cmd(DMA1_Channel6,DISABLE);    										//关闭接收DMA
			num = DMA1_BufferSize -  DMA_GetCurrDataCounter(DMA1_Channel6);	//得到真正接收数据个数
			DMA1_Channel7->CNDTR =num;										//设定待发送数据个数
			DMA1_Channel6->CNDTR=DMA1_BufferSize;       					//重新设置接收数据个数				
			DMA_Cmd(DMA1_Channel6,ENABLE);  								//开启接收DMA			
			DMA_Cmd(DMA1_Channel7,ENABLE);									//DMA发送开启
	}
	else if(DMA_GetFlagStatus(DMA1_FLAG_GL7))
	{
		DMA_ClearFlag(DMA1_FLAG_GL7);
		DMA_Cmd(DMA1_Channel7,DISABLE);									//DMA发送开启
	}
*************************************************************************
************************************************************************/
	
}














