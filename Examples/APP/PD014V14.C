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
PD014_INF_TypeDef	PD014_Conf;

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
	
	PWM_OUT(TIM2,PWM_OUTChannel1,0.5,990);						//PWM设定-20161127版本--SYS-LED
	
	PD014V14_GetSwitchID();				//获取拨码开关地址
	
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
* 函数名			:	PD014V14_Process
* 功能描述		:	PD014V14_Process 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void PD014V14_Process(void)		//PD014V14所有板内处理数理函数
{

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
	
	//拨码开关
	GPIO_Configuration_IPU(GPIOB,	GPIO_Pin_7);					//将GPIO相应管脚配置为上拉输入模式----V20170605
	GPIO_Configuration_IPU(GPIOB,	GPIO_Pin_6);					//将GPIO相应管脚配置为上拉输入模式----V20170605
	GPIO_Configuration_IPU(GPIOB,	GPIO_Pin_5);					//将GPIO相应管脚配置为上拉输入模式----V20170605
	GPIO_Configuration_IPU(GPIOB,	GPIO_Pin_4);					//将GPIO相应管脚配置为上拉输入模式----V20170605
	GPIO_Configuration_IPU(GPIOB,	GPIO_Pin_3);					//将GPIO相应管脚配置为上拉输入模式----V20170605
	GPIO_Configuration_IPU(GPIOC,	GPIO_Pin_12);					//将GPIO相应管脚配置为上拉输入模式----V20170605
	GPIO_Configuration_IPU(GPIOC,	GPIO_Pin_11);					//将GPIO相应管脚配置为上拉输入模式----V20170605
	GPIO_Configuration_IPU(GPIOC,	GPIO_Pin_10);					//将GPIO相应管脚配置为上拉输入模式----V20170605
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
/*******************************************************************************
* 函数名			:	PD014V14_GetSwitchID
* 功能描述		:	获取拨码开关地址 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void PD014V14_GetSwitchID(void)				//获取拨码开关地址
{
	u8 tempID=0;
	tempID=tempID<<1;
	tempID+=PD014_Switch8;		//S8-MSB
	tempID=tempID<<1;
	tempID+=PD014_Switch7;		//
	tempID=tempID<<1;
	tempID+=PD014_Switch6;		//
	tempID=tempID<<1;
	tempID+=PD014_Switch5;		//
	tempID=tempID<<1;
	tempID+=PD014_Switch4;		//
	tempID=tempID<<1;
	tempID+=PD014_Switch3;		//
	tempID=tempID<<1;
	tempID+=PD014_Switch2;		//
	tempID=tempID<<1;
	tempID+=PD014_Switch1;		//
	tempID=tempID^0xFF;
	
	
	if(PD014_Conf.PD014_DATA.SWITCHID&0x80)	//最高位为1，则CAN开启
	{
		PD014_Conf.PD014_DATA.CANFLG=1;
	}
	if(PD014_Conf.PD014_DATA.SWITCHID&0x7F)	//最高位为1，则CAN开启
	{
		PD014_Conf.MODE=NORMEL;					//有拨码，正常模式
	}
	else
	{
		PD014_Conf.MODE=TEST;						//无拨码，测试模式
	}
	
	PD014_Conf.PD014_DATA.SWITCHID=tempID;
}
/*******************************************************************************
* 函数名			:	PD014V14_GetOnlieDevice
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void PD014V14_GetOnlieDevice(void)			//获取在线发药头
{
}
/*******************************************************************************
* 函数名			:	PD014V14_ResetData
* 功能描述		:	复位数据 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void PD014V14_ResetData(void)					//复位数据
{

}




#endif
