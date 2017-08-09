#ifndef __STM32F10x_DMA_Conf_H
#define __STM32F10x_DMA_Conf_H

#include "stm32f10x_dma.h"
#include "stm32f10x_nvic.h"
#include "stm32f10x_rcc.h"
#include "STM32_WOW.H"



void DMA_Configuration(DMA_Channel_TypeDef* DMAy_Channelx,u32 *MemoryAddr,u32 BufferSize);	//USART1_DMA配置
void USART_DMA_Configuration(USART_TypeDef* USARTx,u32 *MemoryAddr,u32 BufferSize);	//USART_DMA配置
void DMA_Server(void);		//DMA中断服务函数

#endif


