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


#ifndef __STM32_PWM_H
#define __STM32_PWM_H



#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

//#include "STM32_TIM.H"


typedef enum
{
  PWM_OUTChannel1	=	0,
	PWM_OUTChannel2,
	PWM_OUTChannel3,
	PWM_OUTChannel4,
} PWM_OUTChannelTypeDef;

void PWM_OUT(TIM_TypeDef* TIMx,PWM_OUTChannelTypeDef PWM_OUTChanneln,double PWM_Frequency ,u16 PWM_Ratio);	//PWM设定-20161127版本
void PWM_OUT2(TIM_TypeDef* TIMx,PWM_OUTChannelTypeDef PWM_OUTChanneln,double PWM_Frequency ,u16 PWM_Ratio);	//PWM设定-20161127版本
void SetPWM_Ratio(u16 PWM_Ratio);		//设置占空比
#endif
