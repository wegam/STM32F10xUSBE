#ifndef __ADC_Temperature_H
#define __ADC_Temperature_H

#include "STM32_USART.H"
#include "STM32_TIM.H"
#include "STM32_DMA.H"
#include "STM32_PWM.H"
#include "STM32_GPIO.H"
#include "STM32_ADC.H"
#include "STM32_DAC.H"
#include "STM32_RCC.H"

void ADC_Temperature_Configuration(void);
void ADC_Temperature_Server(void);


#endif

