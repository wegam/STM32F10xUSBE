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


#ifndef __STM32_USART_H
#define __STM32_USART_H

#include	"stdio.h"			//用于printf
#include	"string.h"			//用于printf
#include	"stdarg.h"			//用于获取不确定个数的参数
#include	"stdlib.h"			//malloc动态申请内存空间

#include "stm32f10x_dma.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_nvic.h"
#include "stm32f10x_rcc.h"
//#include "STM32_WOW.H"
//#include "STM32_GPIO.H"

//#define	USART1_ENABLE			//开启串口1
//#define	USART1_IDLE				//开启串口1接收空闲中断

//#ifdef	USART1_IDLE				//开启串口1接收空闲中断
//	#define	USART1_DMA				//开启串口1 DMA收发
//	#define	DMA1_BufferSize 1024		//DMA1缓冲大小
//#endif

//#define	USE_USART1 1
//#define	USE_USART2 2
//#define	USE_USART3 3
//#define	USE_UART4 4
//#define	USE_UART5 5

extern char *USART_BUFFER;
extern u32	USART_BUFFERSIZE;
typedef struct
{
	u32	USART_BUFFERSIZE;
}USART_BUFFERSIZE_TypeDef;
typedef enum USARTSELECT
{
	USE_USART1=1,
	USE_USART2,
	USE_USART3,
	USE_UART4,
	USE_UART5
}USARTSELECT_TypeDef;
//EXAMPLE
//USART_Configuration(USART1,19200,8,1,0);
void USART_BASIC_Configuration(USART_TypeDef* USARTx,u32 USART_BaudRate,u8 NVICPreemptionPriority,u8 NVIC_SubPriority);	//串口配置
void USART_DMA_Configuration(USART_TypeDef* USARTx,u32 USART_BaudRate,u8 NVICPreemptionPriority,u8 NVIC_SubPriority,u32 *RXDBuffer,u32 *TXDBuffer,u32 BufferSize);	//USART_DMA配置
void USART_Set(USART_TypeDef* USARTx,u32 USART_BaudRate,u8 NVICPreemptionPriority,u8 NVIC_SubPriority);				//根据串口号设置相关引脚和返回中断信号源
int USART_DMAPrintf(USART_TypeDef* USARTx,const char *format,...);		//后边的省略号就是可变参数
int USART_DMASend(USART_TypeDef* USARTx,u32 *USART_BUFFER,u16 BufferSize);		//自定义printf串口DMA发送程序
u32 USART_RX_FlagClear(USART_TypeDef* USARTx);											//清除串口接收中断标志--返回读取的数据个数
ErrorStatus USART_TX_DMAFlagClear(USART_TypeDef* USARTx);						//清除串口DMA发送全局中断标志

void USART_RX_Server(void);		//串口接收服务程序	



#endif

