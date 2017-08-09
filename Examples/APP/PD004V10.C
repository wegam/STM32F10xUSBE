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

#ifdef PD004V10				//片剂药盒控制板

#include "PD004V10.H"
#include "A3987.H"

#include "STM32F10x_BitBand.H"
#include "STM32_GPIO.H"
#include "STM32_SYS.H"
#include "STM32_SYSTICK.H"
#include "STM32_WDG.H"
#include "STM32_PWM.H"

#include "string.h"				//串和内存操作函数头文件
#include "stm32f10x_dma.h"

u16	DelayTime=0;
u8 testFlg=0;

A3987_Pindef	A3987_Pin;

/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PD004V10_Configuration(void)
{
	SYS_Configuration();					//系统配置---打开系统时钟 STM32_SYS.H
	
	GPIO_DeInitAll();							//将所有的GPIO关闭----V20170605
	
	PD004V10_PinSet();
	
	SysTick_Configuration(1000);	//系统嘀嗒时钟配置72MHz,单位为uS
	
	IWDG_Configuration(1000);			//独立看门狗配置---参数单位ms	
	
	PWM_OUT(TIM2,PWM_OUTChannel1,10,500);						//PWM设定-20161127版本
	
	PWM_OUT(TIM1,PWM_OUTChannel1,2000,500);						//PWM设定-20161127版本
	
//	DRV8801_CW(&DRV8801_Pin);					//顺时针转

}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PD004V10_Server(void)
{	
	IWDG_Feed();								//独立看门狗喂狗
	DelayTime++;
	if(DelayTime>=1000)
		DelayTime=0;
	if(DelayTime==0)
	{
		GPIO_Toggle	(GPIOC,	GPIO_Pin_11);		//将GPIO相应管脚输出翻转----V20170605
		GPIO_Toggle	(GPIOC,	GPIO_Pin_12);		//将GPIO相应管脚输出翻转----V20170605
	}

}

/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PD004V10_PinSet(void)
{	
	A3987_Pin.A3987_PUL_PORT	=	GPIOA;
	A3987_Pin.A3987_PUL_Pin		=	GPIO_Pin_8;
	
	A3987_Pin.A3987_DIR_PORT	=	GPIOA;
	A3987_Pin.A3987_DIR_Pin		=	GPIO_Pin_8;
	
	A3987_Pin.A3987_EN_PORT	=	GPIOA;
	A3987_Pin.A3987_EN_Pin		=	GPIO_Pin_8;
	
	A3987_Pin.A3987_MS1_PORT	=	GPIOA;
	A3987_Pin.A3987_MS1_Pin		=	GPIO_Pin_8;
	
	A3987_Pin.A3987_MS2_PORT	=	GPIOA;
	A3987_Pin.A3987_MS2_Pin		=	GPIO_Pin_8;
	
	A3987_PinConf(&A3987_Pin);
	
	
	GPIO_Configuration_OOD50	(GPIOC,	GPIO_Pin_11);			//将GPIO相应管脚配置为OD(开漏)输出模式，最大速度50MHz----V20170605
	GPIO_Configuration_OOD50	(GPIOC,	GPIO_Pin_12);			//将GPIO相应管脚配置为OD(开漏)输出模式，最大速度50MHz----V20170605
	PC11=1;
	PC12=0;
	
	
}
#endif
