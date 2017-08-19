#ifndef __PWM_LED_H
#define __PWM_LED_H


//#include "stm32f10x_gpio.h"
//#include "stm32f10x_rcc.h"
//#include "STM32_GPIO.H"
#include "STM32_PWM.H"
//#include "STM32_GPIO.H"
//#include "STM32_USART.H"
//#include "STM32_DMA.H"

//typedef struct
//{
//	u32 PWM_CountBAC;		//PWM_Count±¸·ÝÖµ
//	
//}PWM_LED_TypeDef;


void PWM_LED_Configuration(void);
void PWM_LED_Server(void);

#endif
