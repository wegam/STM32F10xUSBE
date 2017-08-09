#ifndef __STM32_A3987_H
#define __STM32_A3987_H

//#include "STM32_USART.H"
//#include "STM32_TIM.H"
//#include "STM32_DMA.H"
//#include "STM32_PWM.H"
//#include "STM32_GPIO.H"
//#include "STM32_ADC.H"
//#include "STM32_DAC.H"
//#include "STM32_SPI.H"

#include "stm32f10x_type.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"

//#include	"stdio.h"				//用于printf
//#include	"string.h"			//用于printf
//#include	"stdarg.h"			//用于获取不确定个数的参数
//#include	"stdlib.h"			//malloc动态申请内存空间
//	
//#include	"stddef.h"
//#include	"stdint.h"

#include "STM32F10x_BitBand.H"

typedef enum {LOW = 0, HIGH = !LOW} IOStatus;

//#include "STM32_SYSTICK.H"
#define HC_PD006V20
#if	defined(HC_PD006V20)

	//A3987_EN管脚定义
	#define A3987_EN_RCC													RCC_APB2Periph_GPIOA
	#define	A3987_EN_PORT													GPIOA
	#define	A3987_EN_PIN													GPIO_Pin_4
	#define A3987_EN_CLOCK(FunctionalState)				RCC_APB2PeriphClockCmd(A3987_EN_RCC, FunctionalState)
	#define A3987_EN_SET(FunctionalState)					{ if(ENABLE) PA4=0;else PA4=1;}
	#define	A3987_EN_ENABLE()											PA4=0
	#define	A3987_EN_DISABLE()										PA4=1
	//A3987_PUL管脚定义STEP
	#define A3987_PUL_RCC													RCC_APB2Periph_GPIOA
	#define	A3987_PUL_PORT												GPIOA
	#define	A3987_PUL_PIN													GPIO_Pin_5
	#define A3987_PUL_CLOCK(FunctionalState)			RCC_APB2PeriphClockCmd(A3987_PUL_RCC, FunctionalState)
	#define A3987_PUL_LOW()												PA5=0
	#define	A3987_PUL_HIGH()											PA5=1
	//A3987_DIR管脚定义
	#define A3987_DIR_RCC													RCC_APB2Periph_GPIOA
	#define	A3987_DIR_PORT												GPIOA
	#define	A3987_DIR_PIN													GPIO_Pin_6
	#define A3987_DIR_CLOCK(FunctionalState)			RCC_APB2PeriphClockCmd(A3987_DIR_RCC, FunctionalState)
	#define	A3987_DIR_LOW()												PA6=0
	#define	A3987_DIR_HIGH()											PA6=1
	//A3987_MS1管脚定义
	#define A3987_MS1_RCC													RCC_APB2Periph_GPIOA
	#define	A3987_MS1_PORT												GPIOA
	#define	A3987_MS1_PIN													GPIO_Pin_7
	#define A3987_MS1_CLOCK(FunctionalState)			RCC_APB2PeriphClockCmd(A3987_MS1_RCC, FunctionalState)
	#define	A3987_MS1_LOW()												PA7=0
	#define	A3987_MS1_HIGH()											PA7=1
	//A3987_MS2管脚定义
	#define A3987_MS2_RCC													RCC_APB2Periph_GPIOA
	#define	A3987_MS2_PORT												GPIOA
	#define	A3987_MS2_PIN													GPIO_Pin_8
	#define A3987_MS2_CLOCK(FunctionalState)			RCC_APB2PeriphClockCmd(A3987_MS2_RCC, FunctionalState)
	#define	A3987_MS2_LOW()												PA8=0
	#define	A3987_MS2_HIGH()											PA8=1
	
#elif	defined(CMSIS_CDC_BD)


#else
	#error "Board undefined"
#endif

void A3987_GPIO_Configuration(void);
void A3987_SETUP(void);





#endif

