#ifndef __SPItoUSART_H
#define __SPItoUSART_H

#include "STM32_USART.H"
#include "STM32_DMA.H"
#include "STM32_TIM.H"
#include "STM32_WDG.H"
#include "STM32_PWM.H"
#include "STM32_SPI.H"
#include "stm32f10x_spi.h"

//u8 m_SPI_Buffer=0;

void SPItoUSART_Configuration(void);
void SPItoUSART_Server(void);
void SPI_StructConfU(SPI_InitTypeDef* SPI_InitStruct);
void LEDCON(void);
void LEDSET(u8 states);
//void SPI2_Conf_TEST(u32* SPI_RX_Buffer,u32 SPI_BUFFERSIZE);
	
#endif



