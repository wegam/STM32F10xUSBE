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

#ifdef PD014V14			//盒剂发药控制板

#include "PD014V14.H"

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

u16 Lock_Toggle_CNT=0;


void PD014V14_PinSet(void);
void Lock_Toggle(void);			//双向电子锁控制
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PD014V14_Configuration(void)
{
	SYS_Configuration();					//系统配置---打开系统时钟 STM32_SYS.H
	
	GPIO_DeInitAll();							//将所有的GPIO关闭----V20170605
	
	PD014V14_PinSet();
	
	SysTick_Configuration(500);	//系统嘀嗒时钟配置72MHz,单位为uS
	
	IWDG_Configuration(1000);			//独立看门狗配置---参数单位ms	
	
	PWM_OUT(TIM2,PWM_OUTChannel1,1,900);						//PWM设定-20161127版本--SYS-LED
	
	P_Sens=1;

}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PD014V14_Server(void)
{	
	IWDG_Feed();								//独立看门狗喂狗
//	DelayTime++;
//	StepTime++;

	Lock_Toggle();			//双向电子锁控制
//	GPIO_Toggle	(GPIOC,	GPIO_Pin_1);	//蜂鸣器
//	if(DelayTime>=500)
//	{
//		DelayTime=0;
//		
//	}
//	if(StepTime>=2000)
//	{
//		StepTime=0;
////		GPIO_Toggle	(GPIOA,	GPIO_Pin_5);		//将GPIO相应管脚输出翻转----V20170605
//	}
}

/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PD014V14_PinSet(void)
{	
	//发药电磁铁控制
	GPIO_Configuration_OPP50	(GPIOA,	GPIO_Pin_7);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	GPIO_Configuration_OPP50	(GPIOC,	GPIO_Pin_5);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	GPIO_Configuration_OPP50	(GPIOB,	GPIO_Pin_1);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	GPIO_Configuration_OPP50	(GPIOB,	GPIO_Pin_11);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	GPIO_Configuration_OPP50	(GPIOB,	GPIO_Pin_13);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	GPIO_Configuration_OPP50	(GPIOB,	GPIO_Pin_15);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	GPIO_Configuration_OPP50	(GPIOC,	GPIO_Pin_7);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	GPIO_Configuration_OPP50	(GPIOC,	GPIO_Pin_9);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	
	//发药传感器供电开关
	GPIO_Configuration_OPP50	(GPIOA,	GPIO_Pin_5);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	
	//发药传感器
	GPIO_Configuration_IPU(GPIOA,	GPIO_Pin_6);					//将GPIO相应管脚配置为上拉输入模式----V20170605
	GPIO_Configuration_IPU(GPIOC,	GPIO_Pin_4);					//将GPIO相应管脚配置为上拉输入模式----V20170605
	GPIO_Configuration_IPU(GPIOB,	GPIO_Pin_0);					//将GPIO相应管脚配置为上拉输入模式----V20170605
	GPIO_Configuration_IPU(GPIOB,	GPIO_Pin_10);					//将GPIO相应管脚配置为上拉输入模式----V20170605
	GPIO_Configuration_IPU(GPIOB,	GPIO_Pin_12);					//将GPIO相应管脚配置为上拉输入模式----V20170605
	GPIO_Configuration_IPU(GPIOB,	GPIO_Pin_14);					//将GPIO相应管脚配置为上拉输入模式----V20170605
	GPIO_Configuration_IPU(GPIOC,	GPIO_Pin_6);					//将GPIO相应管脚配置为上拉输入模式----V20170605
	GPIO_Configuration_IPU(GPIOC,	GPIO_Pin_8);					//将GPIO相应管脚配置为上拉输入模式----V20170605	
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void Lock_Toggle(void)
{
	Lock_Toggle_CNT++;
	if(Lock_Toggle_CNT>=2000)
	{
		Lock_Toggle_CNT=0;
		ct_pmos1=0;
		ct_pmos2=0;
		ct_pmos3=0;
		ct_pmos4=0;
		ct_pmos5=0;
		ct_pmos6=0;
		ct_pmos7=0;
		ct_pmos8=0;
	}
	else if(Lock_Toggle_CNT==100)
	{
		if(Sens_In1)
			ct_pmos1=1;
		if(Sens_In2)
			ct_pmos2=1;
		if(Sens_In3)
			ct_pmos3=1;
		if(Sens_In4)
			ct_pmos4=1;
		if(Sens_In5)
			ct_pmos5=1;
		if(Sens_In6)
			ct_pmos6=1;
		if(Sens_In7)
			ct_pmos7=1;
		if(Sens_In8)
			ct_pmos8=1;
		
	}
//	else if(Lock_Toggle_CNT==200)
//	{
//		ct_pmos1=0;
//		ct_pmos2=0;
//		ct_pmos3=0;
//		ct_pmos4=0;
//		ct_pmos5=0;
//		ct_pmos6=0;
//		ct_pmos7=0;
//		ct_pmos8=0;		
//	}
	else if(Lock_Toggle_CNT>=400)
	{
		ct_pmos1=0;
		ct_pmos2=0;
		ct_pmos3=0;
		ct_pmos4=0;
		ct_pmos5=0;
		ct_pmos6=0;
		ct_pmos7=0;
		ct_pmos8=0;
	}

}
#endif
