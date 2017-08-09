#ifndef __STM32F10x_Usart_Conf_H
#define __STM32F10x_Usart_Conf_H

#include "stm32f10x_dma.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_nvic.h"
#include "stm32f10x_rcc.h"
#include "STM32_WOW.H"

#define	USART1_ENABLE			//��������1
#define	USART1_IDLE				//��������1���տ����ж�

#ifdef	USART1_IDLE				//��������1���տ����ж�
	#define	USART1_DMA				//��������1 DMA�շ�
	#define	DMA1_BufferSize 1024		//DMA1�����С
#endif

//#define	USE_USART1 1
//#define	USE_USART2 2
//#define	USE_USART3 3
//#define	USE_UART4 4
//#define	USE_UART5 5
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
void USART_Configuration(USART_TypeDef* USARTx,u32 USART_BaudRate,u16 USART_WordLength,u16 USART_StopBits,u16 USART_Parity);	//��������
void USART_RX_Server(void);		//���ڽ��շ������	


#endif
