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


#ifndef __STM32_DMA_H
#define __STM32_DMA_H


#include "stm32f10x_dma.h"
#include "stm32f10x_nvic.h"
#include "stm32f10x_rcc.h"
//#include "STM32_WOW.H"


/* EXTI Trigger enumeration --------------------------------------------------*/
typedef enum
{
	//---DMA1_CH1
  ADC1_RQ = 0x01,
  TIM2_CH3,  
  TIM4_CH1,
	//---DMA1_CH2
	USART3_TX	=	0X04,
	TIM1_CH1,
	TIM2_UP,
	TIM3_CH3,
	SPI1_RX,
	//---DMA1_CH3
	USART3_RX = 0X09,
	TIM1_CH2,
	TIM3_CH4,
	TIM3_UP,
	SPI1_TX,
	//---DMA1_CH4
	USART1_TX	=	0X0E,
	TIM1_CH4,
	TIM1_TRIG,
	TIM1_COM,
	TIM4_CH2,
	SPI2_RX,
	I2S2_RX,
	I2C2_TX,
	//---DMA1_CH5
	USART1_RX =	0X16,
	TIM1_UP,
	SPI2_TX,
	I2S2_TX,
	I2C2_RX,
	TIM2_CH1,
	TIM4_CH3,
	//---DMA1_CH6
	USART2_RX	=0X1D,
	TIM1_CH3,
	TIM3_CH1,
	TIM3_TRIG,
	I2C1_TX,
	//---DMA1_CH7
	USART2_TX	=	0X22,
	TIM2_CH2,
	TIM2_CH4,
	TIM4_UP,
	I2C1_RX,
	//---DMA2_CH1
	TIM5_CH4	=	0X27,
	TIM5_TRIG,
	TIM8_CH3,
	TIM8_UP,
	SPI3_RX,
	I2S3_RX,
	//---DMA2_CH2
	TIM8_CH4	=	0X2D,
	TIM8_TRIG,
	TIM8_COM,
	TIM5_CH3,
	TIM5_UP,
	SPI3_TX,
	I2S3_TX,
	//---DMA2_CH3
	TIM8_CH1	=	0X34,
	UART4_RX,
	TIM6_UP,
	DAC_CH1,
	//---DMA2_CH4
	TIM5_CH2	=	0X38,
	SDIO_RQ,
	TIM7_UP,
	DAC_CH2,
	//---DMA2_CH5
	ADC3_RQ	=	0X3C,
	TIM8_CH2,
	TIM5_CH1,
	UART4_TX
	
}Peripheral_Request_TypeDef;

#define IS_Peripheral_Request(Request) 	(((Request) ==ADC1_RQ) 		||	\
																				((Request) == TIM2_CH3) 	||	\
																				((Request) == TIM4_CH1)		||	\
																				((Request) == USART3_TX)	||	\
																				((Request) == TIM1_CH1)		||	\
																				((Request) == TIM2_UP)		||	\
																				((Request) == TIM3_CH3)		||	\
																				((Request) == SPI1_RX)		||	\
																				((Request) == USART1_TX)	||	\
																				((Request) == TIM1_TRIG)	||	\
																				((Request) == TIM1_COM)		||	\
																				((Request) == TIM4_CH2)		||	\
																				((Request) == SPI2_RX)		||	\
																				((Request) == I2S2_RX)		||	\
																				((Request) == I2C2_TX)		||	\
																				((Request) == USART1_RX)	||	\
																				((Request) == TIM1_UP)		||	\
																				((Request) == SPI2_TX)		||	\
																				((Request) == I2S2_TX)		||	\
																				((Request) == I2C2_RX)		||	\
																				((Request) == USART2_RX)	||	\
																				((Request) == TIM1_CH3)		||	\
																				((Request) == TIM3_CH1)		||	\
																				((Request) == TIM3_TRIG)	||	\
																				((Request) == I2C1_TX)		||	\
																				((Request) == USART2_TX)	||	\
																				((Request) == TIM2_CH2)		||	\
																				((Request) == TIM2_CH4)		||	\
																				((Request) == TIM4_UP)		||	\
																				((Request) == I2C1_RX)		||	\
																				((Request) == TIM5_CH4)		||	\
																				((Request) == TIM5_TRIG)	||	\
																				((Request) == TIM8_CH3)		||	\
																				((Request) == TIM8_UP)		||	\
																				((Request) == SPI3_RX)		||	\
																				((Request) == I2S3_RX)		||	\
																				((Request) == TIM8_CH4)		||	\
																				((Request) == TIM8_TRIG)	||	\
																				((Request) == TIM8_COM)		||	\
																				((Request) == TIM5_CH3)		||	\
																				((Request) == TIM5_UP)		||	\
																				((Request) == SPI3_TX)		||	\
																				((Request) == I2S3_TX)		||	\
																				((Request) == TIM8_CH1)		||	\
																				((Request) == UART4_RX)		||	\
																				((Request) == TIM6_UP)		||	\
																				((Request) == DAC_CH1)		||	\
																				((Request) == TIM5_CH2)		||	\
																				((Request) == SDIO_RQ)		||	\
																				((Request) == TIM7_UP)		||	\
																				((Request) == DAC_CH2)		||	\
																				((Request) == ADC3_RQ)		||	\
																				((Request) == TIM8_CH2)		||	\
																				((Request) == TIM5_CH1)		||	\
																				((Request) == UART4_TX)	)





void DMA_Configuration(Peripheral_Request_TypeDef Request,u32 *MemoryAddr,u32 BufferSize);	//USART1_DMA配置
void DMA_Server(void);		//DMA中断服务函数




#endif


