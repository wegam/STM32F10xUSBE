/******************************** User_library *********************************
* 文件名 	: STM32_SDCard.H
* 作者   	: wegam@sina.com
* 版本   	: V
* 日期   	: 2016/01/01
* 说明   	: 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


#ifndef __STM32_TIM_H
#define __STM32_TIM_H

#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_nvic.h"
#include "stm32f10x_gpio.h"
//#include "STM32_WOW.H"


void TIMx_RCC_ENABLE(TIM_TypeDef* TIMx);	//定时器配置
void TIM_TIME_SET(TIM_TypeDef* TIMx,u16 Prescaler,u16 Period);	//定时时间设定
void TIM_Configuration(TIM_TypeDef* TIMx,u16 Prescaler,u16 Period);	//定时器配置
void TIM_Interrupt(TIM_TypeDef* TIMx,u16 Prescaler,u16 Period);
void TIM_Server(void);






#endif


