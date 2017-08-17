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


#ifndef __STM32_EXTI_H
#define __STM32_EXTI_H

#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_nvic.h"
#include "stm32f10x_rcc.h"
#include "math.h"

//#include "STM32_WOW.H"
//#include "stdio.h"

void EXTI_Configuration(GPIO_TypeDef* GPIOx, u16 GPIO_Pin,EXTIMode_TypeDef Mode);
void EXTI_Configuration2(GPIO_TypeDef* GPIOx, u16 GPIO_Pin,EXTIMode_TypeDef Mode);
void EXTI_ClockConf(GPIO_TypeDef* GPIOx, u16 GPIO_Pin_x);						//开启相关GPIO时钟	
void EXTI_PortSourceConf(GPIO_TypeDef* GPIOx,u8 *GPIO_PortSource);	//设置中断线
void EXTI_PinConf(GPIO_TypeDef* GPIOx, u16 GPIO_Pin_x,u8 *EXTI_IRQChannel);			//选择相关中断线路
void EXTI_InitStructure(GPIO_TypeDef* GPIOx, u16 GPIO_Pin_x,EXTIMode_TypeDef Mode,u8 EXTI_IRQChannel);		//外部中断初始化
void EXTI_InitStructure2(GPIO_TypeDef* GPIOx, u16 GPIO_Pin_x,EXTIMode_TypeDef Mode,u8 EXTI_IRQChannel);	
void EXTI_Interrupt_Configuration(u32 EXTI_Line);
void EXTI_Server(void);
#endif


