#ifndef __MS0800_PL001V20_H
#define __MS0800_PL001V20_H

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


void MS0800_PL001V20_Configuration(void);
void MS0800_PL001V20_Server(void);
void PL001V20_CAN_RX(void);
void PL001V20_CAN_TX(void);
void MS0800_PL001V20_PROCESS(u16 time);
u8 Get_SW_ID(void);




#endif

