#ifndef	_STM32F10x_ADS1230_H
#define	_STM32F10x_ADS1230_H

//#include "IIC.H"		//I2C–≠“È
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_nvic.h"
#include "stm32f10x_exti.h"

//#include "stm32f10x_dma.h"
//#include "stm32f10x_gpio.h"
//#include "stm32f10x_rcc.h"
//#include "STM32F10x_USER.H"

void ADSREAD_SET(u8 EN);
void ADS1230_Configuration(void);
void ADS1230_INIT(void);
void ADS1230_START(void);
void ADS1230_STOP(void);
u32 ADS1230_READ(void);

void ADS1230(void);
void ADS1230_LED_DISPLAY(void);
#endif



