/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : PC001V21.c
* Author             : WOW
* Version            : V2.0.1
* Date               : 06/26/2017
* Description        : PC001V21层控制板.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#ifdef PL006V10			//包装机数码管显示板

#include "PL006V10.H"
#include "74HC595.H"

#include "STM32F10x_BitBand.H"
#include "STM32_GPIO.H"
#include "STM32_SYS.H"
#include "STM32_SYSTICK.H"
#include "STM32_WDG.H"
#include "STM32_USART.H"
#include "STM32_PWM.H"

#include "string.h"				//串和内存操作函数头文件
#include "stm32f10x_dma.h"


HC595_Pindef HC595;

u16 DelayTime=0;

u8 num=0;

/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PL006V10_Configuration(void)
{
	SYS_Configuration();					//系统配置---打开系统时钟 STM32_SYS.H
	
	GPIO_DeInitAll();							//将所有的GPIO关闭----V20170605
	
	PL006V10_PinSet();
	
	SysTick_Configuration(1000);	//系统嘀嗒时钟配置72MHz,单位为uS
	
	IWDG_Configuration(1000);			//独立看门狗配置---参数单位ms	
	
	PWM_OUT(TIM2,PWM_OUTChannel1,1,900);						//PWM设定-20161127版本
}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PL006V10_Server(void)
{
	DelayTime++;
	
	IWDG_Feed();								//独立看门狗喂狗
	
	if(DelayTime>=500)
	{
		if(DelayTime==500)
		{
			HC595_RCK_HIGH(&HC595);
		}
		else if(DelayTime==501)
		{
			if(num==0)
			{
				HC595_WriteNumberN(&HC595,6);
				HC595_WriteNumberN(&HC595,5);
				HC595_WriteNumberN(&HC595,4);
				HC595_WriteNumberN(&HC595,3);
				HC595_WriteNumberN(&HC595,2);
				HC595_WriteNumberN(&HC595,1);
				HC595_WriteNumberN(&HC595,0);
			}
			else
			{
				HC595_WriteNumberN(&HC595,9);
				HC595_WriteNumberN(&HC595,8);
				HC595_WriteNumberN(&HC595,7);
				HC595_WriteNumberN(&HC595,6);
				HC595_WriteNumberN(&HC595,5);
				HC595_WriteNumberN(&HC595,4);
				HC595_WriteNumberN(&HC595,3);
			}
		}
		else if(DelayTime==502)
		{
			HC595_RCK_LOW(&HC595);
			DelayTime=0;
			if(num==1)
				num=0;
			else
				num=1;
		}
	}
	
}

/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PL006V10_PinSet(void)
{	
	//SDA
	HC595.HC595_SDA_PORT=GPIOB;
	HC595.HC595_SDA_Pin=GPIO_Pin_15;
	//SCK
	HC595.HC595_SCK_PORT=GPIOB;
	HC595.HC595_SCK_Pin=GPIO_Pin_13;
	//RCK
	HC595.HC595_RCK_PORT=GPIOB;
	HC595.HC595_RCK_Pin=GPIO_Pin_12;
	
	HC595_PinConf(&HC595);			//配置GPIO
}
#endif
