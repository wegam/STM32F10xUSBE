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

#ifdef PD006V20		//针剂药盒控制板

#include "PD006V20.H"

#include "DRV8801.H"
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
u16	StepTime=0;

u8 testFlg=0;
DRV8801_Pindef	DRV8801_Pin;
A3987_Pindef	A3987_Pin;

/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PD006V20_Configuration(void)
{
	SYS_Configuration();					//系统配置---打开系统时钟 STM32_SYS.H
	
	GPIO_DeInitAll();							//将所有的GPIO关闭----V20170605
	
	PD006V20_PinSet();
	
	SysTick_Configuration(1000);	//系统嘀嗒时钟配置72MHz,单位为uS
	
	IWDG_Configuration(1000);			//独立看门狗配置---参数单位ms	
	
	PWM_OUT(TIM2,PWM_OUTChannel1,4,500);						//PWM设定-20161127版本
	
	DRV8801_CW(&DRV8801_Pin);					//顺时针转

}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PD006V20_Server(void)
{	
	IWDG_Feed();								//独立看门狗喂狗
	DelayTime++;
	StepTime++;
	
	if(DelayTime>=500)
	{
		DelayTime=0;
		
	}
	if(StepTime>=2000)
	{
		StepTime=0;
//		GPIO_Toggle	(GPIOA,	GPIO_Pin_5);		//将GPIO相应管脚输出翻转----V20170605
	}
//	if(StepTime<1000)
//	{
//		DRV8801_CW(&DRV8801_Pin);					//顺时针转
//		DRV8801_Sleep(&DRV8801_Pin);			//睡眠---调速使用
//	}
//	else
//	{
//		DRV8801_CCW(&DRV8801_Pin);					//顺时针转
//		DRV8801_WKup(&DRV8801_Pin);				//唤醒---调速使用
//	}
	
	if(PB13in)					//中间传感器
	{
		DRV8801_CW(&DRV8801_Pin);						//顺时针转
	}
	else if(PB14in)			//外边传感器
	{
		DRV8801_CCW(&DRV8801_Pin);					//顺时针转
	}
	else
	{
		DRV8801_Brake_SN(&DRV8801_Pin);		//慢急停 N通道放电
	}
}

/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PD006V20_PinSet(void)
{	
	DRV8801_Pin.DRV8801_VPORTI_PORT=GPIOA;
	DRV8801_Pin.DRV8801_VPORTI_Pin=GPIO_Pin_1;
	
	DRV8801_Pin.DRV8801_EN_PORT=GPIOB;
	DRV8801_Pin.DRV8801_EN_Pin=GPIO_Pin_3;
	
	DRV8801_Pin.DRV8801_SLEEP_PORT=GPIOB;
	DRV8801_Pin.DRV8801_SLEEP_Pin=GPIO_Pin_4;
	
	DRV8801_Pin.DRV8801_PHASE_PORT=GPIOB;
	DRV8801_Pin.DRV8801_PHASE_Pin=GPIO_Pin_5;
	
	DRV8801_Pin.DRV8801_FAULT_PORT=GPIOB;
	DRV8801_Pin.DRV8801_FAULT_Pin=GPIO_Pin_7;
	
	DRV8801_Pin.DRV8801_MODE1_PORT=GPIOB;
	DRV8801_Pin.DRV8801_MODE1_Pin=GPIO_Pin_6;
	
	DRV8801_Pin.DRV8801_MODE2_PORT=GPIOB;
	DRV8801_Pin.DRV8801_MODE2_Pin=GPIO_Pin_8;
	
	DRV8801_PinConf(&DRV8801_Pin);
	
	
	
	A3987_Pin.A3987_PUL_PORT	=	GPIOA;
	A3987_Pin.A3987_PUL_Pin		=	GPIO_Pin_5;
	
	A3987_Pin.A3987_DIR_PORT	=	GPIOA;
	A3987_Pin.A3987_DIR_Pin		=	GPIO_Pin_6;
	
	A3987_Pin.A3987_EN_PORT		=	GPIOA;
	A3987_Pin.A3987_EN_Pin		=	GPIO_Pin_4;
	
	A3987_Pin.A3987_MS1_PORT	=	GPIOA;
	A3987_Pin.A3987_MS1_Pin		=	GPIO_Pin_7;
	
	A3987_Pin.A3987_MS2_PORT	=	GPIOA;
	A3987_Pin.A3987_MS2_Pin		=	GPIO_Pin_8;
	
	A3987_PinConf(&A3987_Pin);
	
	//传感器输入
	GPIO_Configuration_INF(GPIOB,	GPIO_Pin_12);			//将GPIO相应管脚配置为浮空输入模式----V20170605
	GPIO_Configuration_INF(GPIOB,	GPIO_Pin_13);			//将GPIO相应管脚配置为浮空输入模式----V20170605---中间传感器
	GPIO_Configuration_INF(GPIOB,	GPIO_Pin_14);			//将GPIO相应管脚配置为浮空输入模式----V20170605---最外边传感器
	
}
#endif
