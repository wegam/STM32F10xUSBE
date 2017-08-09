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

//#include	"stdio.h"			//用于printf
//#include	"string.h"			//用于printf
//#include	"stdarg.h"			//用于获取不确定个数的参数
//#include	"stdlib.h"			//malloc动态申请内存空间

#include "stm32f10x_type.h"
#include "stm32f10x_usart.h"



typedef struct
{
	//USART:485用到的串口;串口号--USART1,USART2,USART3,UART4;//UART5不支持DMA
	USART_TypeDef* 	USARTx;			//USART1,USART2,USART3,UART4;//UART5
	//RS485_CTL
	GPIO_TypeDef* 	RS485_CTL_PORT;				//GPIOX
	unsigned short 	RS485_CTL_Pin;				//GPIO_Pin_x
}RS485_TypeDef;

//#define RS485_TX_EN			(RS485_TypeDef->RS485_CTL_PORT->BSRR		= R61509V_sPinfo->R61509V_sCS_Pin)
//#define RS485_RX_EN			(R61509V_sPinfo->R61509V_sCS_PORT->BRR 		= R61509V_sPinfo->R61509V_sCS_Pin)


//---------------------------串口
//*****************USART_DMA初始化
void 					USART_DMA_ConfigurationNR	(USART_TypeDef* USARTx,u32 USART_BaudRate,u32 *RxdBuffer,u32 BufferSize);	//USART_DMA配置--查询方式，不开中断
void 					USART_DMA_ConfigurationEV	(USART_TypeDef* USARTx,u32 USART_BaudRate,u32 *RxdBuffer,u32 BufferSize);	//USART_DMA配置--查询方式，不开中断--偶校验
void 					USART_DMA_ConfigurationOD	(USART_TypeDef* USARTx,u32 USART_BaudRate,u32 *RxdBuffer,u32 BufferSize);	//USART_DMA配置--查询方式，不开中断--奇校验
void 					USART_DMA_ConfigurationIT	(USART_TypeDef* USARTx,u32 USART_BaudRate,u8 NVICPreemptionPriority,u8 NVIC_SubPriority,u32 *RxdBuffer,u32 BufferSize);	//USART_DMA配置--中断方式
void 					USART_DMA_Configuration		(USART_TypeDef* USARTx,u32 USART_BaudRate,u8 NVICPreemptionPriority,u8 NVIC_SubPriority,u32 *RxdBuffer,u32 *TxdBuffer,u32 BufferSize);	//USART_DMA配置---旧

//*****************串口接收
u16						USART_ReadBufferIDLE			(USART_TypeDef* USARTx,u32 *RevBuffer,u32 *RxdBuffer);	//串口空闲模式读串口接收缓冲区，如果有数据，将数据拷贝到RevBuffer,并返回接收到的数据个数，然后重新将接收缓冲区地址指向RxdBuffer

//*****************串口发送
u16 					USART_DMAPrintf						(USART_TypeDef* USARTx,const char *format,...);					//自定义printf串口DMA发送程序,后边的省略号就是可变参数
u16 					USART_DMASend							(USART_TypeDef* USARTx,u32 *tx_buffer,u16 BufferSize);	//串口DMA发送程序







//---------------------------RS485
//*****************RS485初始化---使用DMA空闲接收，标志查询方式
void 					RS485_DMA_ConfigurationNR	(RS485_TypeDef *RS485_Info,u32 USART_BaudRate,u32 *RxdBuffer,u32 BufferSize);	//USART_DMA配置--查询方式，不开中断,配置完默认为接收状态



//*****************RS485接收
u16						RS485_ReadBufferIDLE			(RS485_TypeDef *RS485_Info,u32 *RevBuffer,u32 *RxdBuffer);	//串口空闲模式读串口接收缓冲区，如果有数据，将数据拷贝到RevBuffer,并返回接收到的数据个数，然后重新将接收缓冲区地址指向RxdBuffer

//*****************RS485发送
u16 					RS485_DMAPrintf						(RS485_TypeDef *RS485_Info,const char *format,...);					//自定义printf串口DMA发送程序,后边的省略号就是可变参数
u16 					RS485_DMASend							(RS485_TypeDef *RS485_Info,u32 *tx_buffer,u16 BufferSize);	//RS485-DMA发送程序




void USART_RX_Server(void);		//串口接收服务程序	



#endif

