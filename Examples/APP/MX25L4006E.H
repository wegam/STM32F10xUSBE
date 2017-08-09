#ifndef __MX25L4006E_H
#define __MX25L4006E_H

#include "stm32f10x_type.h"
//#include "STM32_SPI.H"
#include "STM32_PWM.H"

#include "STM32_SPI.H"






void MX25L4006E_Configuration(void);
void MX25L4006E_Server(void);
void SPI_StructConf(SPI_InitTypeDef* SPI_InitStruct);

void MX25L4006E_WriteEnable(void);				//MX25L4006E写使能命令
void MX25L4006E_WaitForWriteEnd(void);		//等待命令执行结束

u32 MX25L4006E_ReadID(void);

#endif

