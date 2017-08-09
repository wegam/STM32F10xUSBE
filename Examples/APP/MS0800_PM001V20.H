#ifndef __MS0800_PM001V20_H
#define __MS0800_PM001V20_H

//#include "STM32_USART.H"
//#include "STM32_TIM.H"
//#include "STM32_DMA.H"
//#include "STM32_PWM.H"
//#include "STM32_GPIO.H"
//#include "STM32_ADC.H"
//#include "STM32_DAC.H"
#include "STM32_CAN.H"
#include "STM32_PWM.H"
#include "STM32_CD4511BM.H"
#include "STM32_GPIO.H"
#include "STM32_USART.H"

#include "STM32_TOOLS.H"		//数据校验工具


void MS0800_PM001V20_Configuration(void);
void MS0800_PM001V20_Server(void);
void MS0800_PM001V20_PROCESS(u16 time);
u8 MS0800_PM001V20_TR(void);
void PM001V20_WORK_LED(u16 time);				//WORK_LED;
void PM001V20_BUZZER(u16 time);					//BUZZER
void PM001V20_delay(u16 time);
void Time_Sync(void);
#endif

