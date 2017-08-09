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

#ifdef PD003VG			//G,H版本智能药盒辅控板

#include "PD003VG.H"

#include "DRV8801.H"
#include "A3987.H"

#include "STM32F10x_BitBand.H"
#include "STM32_GPIO.H"
#include "STM32_SYS.H"
#include "STM32_SYSTICK.H"
#include "STM32_WDG.H"
#include "STM32_PWM.H"
#include "STM32_USART.H"

#include "string.h"				//串和内存操作函数头文件
#include "stm32f10x_dma.h"

#define RS485_Bus_RXEN	PB1=0			//rs485总线接收使能
#define RS485_Bus_TXEN	PB1=1			//rs485总线发送使能

#define BufferSize	10
u8 txBuffer_PD[BufferSize]={0};				//USART2-TXBF
u8 rxBuffer_PD[BufferSize]={0};				//USART2-RXBF

u16	DelayTime=0;
u16	StepTime=0;

u16 Lock_Toggle_CNT=0;


u8 testFlg=0;
u8 txflg=0;
//DRV8801_Pindef	DRV8801_Pin;
A3987_Pindef	A3987_Pin1;
A3987_Pindef	A3987_Pin2;
void Lock_Toggle(void);			//双向电子锁控制
void PD003VG_USART_Conf(void);
void PD003VG_485_Sens(void);
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PD003VG_Configuration(void)
{
	SYS_Configuration();					//系统配置---打开系统时钟 STM32_SYS.H
	
	GPIO_DeInitAll();							//将所有的GPIO关闭----V20170605
	
	PD003VG_PinSet();
	
	SysTick_Configuration(500);	//系统嘀嗒时钟配置72MHz,单位为uS
	
	IWDG_Configuration(1000);			//独立看门狗配置---参数单位ms	
	
	PWM_OUT(TIM2,PWM_OUTChannel1,1,900);						//PWM设定-20161127版本
	
//	PD003VG_USART_Conf();
	
	RS485_Bus_TXEN;		//rs485总线接收使能
//	USART_DMASend(USART3,(u32*)txBuffer_PD,BufferSize);			//自定义printf串口DMA发送程序

}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PD003VG_Server(void)
{	
	IWDG_Feed();								//独立看门狗喂狗
	DelayTime++;
	StepTime++;
//	PD003VG_485_Sens();
//	A3987_StepDriver(&A3987_Pin1);
	A3987_StepDriver(&A3987_Pin2);
//	Lock_Toggle();			//双向电子锁控制
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
void PD003VG_PinSet(void)
{	
	
	A3987_Pin1.A3987_PUL_PORT		=	GPIOA;
	A3987_Pin1.A3987_PUL_Pin		=	GPIO_Pin_8;
	
	A3987_Pin1.A3987_DIR_PORT		=	GPIOC;
	A3987_Pin1.A3987_DIR_Pin		=	GPIO_Pin_9;
	
	A3987_Pin1.A3987_EN_PORT		=	GPIOC;
	A3987_Pin1.A3987_EN_Pin			=	GPIO_Pin_8;
	
	A3987_Pin1.A3987_MS1_PORT		=	GPIOC;
	A3987_Pin1.A3987_MS1_Pin		=	GPIO_Pin_7;
	
	A3987_Pin1.A3987_MS2_PORT		=	GPIOC;
	A3987_Pin1.A3987_MS2_Pin		=	GPIO_Pin_6;
	
	A3987_PinConf(&A3987_Pin1);
	
	
	
	A3987_Pin2.A3987_PUL_PORT		=	GPIOB;
	A3987_Pin2.A3987_PUL_Pin		=	GPIO_Pin_6;
	
	A3987_Pin2.A3987_DIR_PORT		=	GPIOB;
	A3987_Pin2.A3987_DIR_Pin		=	GPIO_Pin_5;
	
	A3987_Pin2.A3987_EN_PORT		=	GPIOB;
	A3987_Pin2.A3987_EN_Pin			=	GPIO_Pin_7;
	
	A3987_Pin2.A3987_MS1_PORT		=	GPIOB;
	A3987_Pin2.A3987_MS1_Pin		=	GPIO_Pin_8;
	
	A3987_Pin2.A3987_MS2_PORT		=	GPIOB;
	A3987_Pin2.A3987_MS2_Pin		=	GPIO_Pin_9;
	
	A3987_PinConf(&A3987_Pin2);
	


	//传感器输入
	GPIO_Configuration_INF(GPIOB,	GPIO_Pin_12);			//将GPIO相应管脚配置为浮空输入模式----V20170605
	GPIO_Configuration_INF(GPIOB,	GPIO_Pin_13);			//将GPIO相应管脚配置为浮空输入模式----V20170605---中间传感器
	GPIO_Configuration_INF(GPIOB,	GPIO_Pin_14);			//将GPIO相应管脚配置为浮空输入模式----V20170605---最外边传感器

	//双向电子锁1
	GPIO_Configuration_OPP50	(GPIOB,	GPIO_Pin_12);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	GPIO_Configuration_OPP50	(GPIOB,	GPIO_Pin_13);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	
	//双向电子锁1
	GPIO_Configuration_OPP50	(GPIOB,	GPIO_Pin_14);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	GPIO_Configuration_OPP50	(GPIOB,	GPIO_Pin_15);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	
	//蜂鸣器
	GPIO_Configuration_OPP50	(GPIOC,	GPIO_Pin_1);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	
	//运行灯
	GPIO_Configuration_OPP50	(GPIOC,	GPIO_Pin_0);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	
	
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void PD003VG_USART_Conf(void)
{
	//*********************总线485（与单元板连接--USART3)***********************************
	USART_DMA_Configuration(USART3,9600,1,1,(u32*)rxBuffer_PD,(u32*)txBuffer_PD,BufferSize);	//USART_DMA配置
	DMA_ITConfig(DMA1_Channel2,DMA_IT_TC, DISABLE);
	USART_ITConfig(USART3,USART_IT_IDLE, DISABLE);				//使用空闲中断，DMA自动接收，检测到总线空闲表示发送端已经发送完成，数据保存在DMA缓冲器中

}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void PD003VG_485_Bus(void)
{

}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void PD003VG_485_Sens(void)
{
	u16 num=0;
	num=USART_RX_FlagClear(USART3);												//清除串口接收中断标志--返回DMA剩余缓冲区大小
	//*********************DMA接收处理***********************************
	if(num!=0xFFFF)
	{
		RS485_Bus_TXEN;		//rs485总线接收使能
		num=BufferSize-num;																	//得到真正接收数据个数
		memcpy(txBuffer_PD, rxBuffer_PD,num);
		USART_DMASend(USART3,(u32*)txBuffer_PD,BufferSize);			//自定义printf串口DMA发送程序
		USART_DMA_RxEN(USART3,(u32*)rxBuffer_PD,BufferSize);	//重新设定接收缓冲区地址及大小并使能DMA接收
	}
	//*********************DMA发送处理***********************************
	else
	{
		u8 flg_tx=0;
		flg_tx=USART_TX_FlagClear(USART3);									//清除串口DMA发送全局中断标志
//		if(flg_tx)
//		{
				txflg=1;
				RS485_Bus_RXEN;		//rs485总线接收使能
				memset(txBuffer_PD, 0x00, BufferSize);
				
				RS485_Bus_TXEN;		//rs485总线接收使能
				USART_DMASend(USART3,(u32*)txBuffer_PD,BufferSize);			//自定义printf串口DMA发送程序
			
//		}
	}
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
	if(Lock_Toggle_CNT>=5000)
	{
		Lock_Toggle_CNT=0;
		PB12=0;
		PB13=0;
		PB14=0;
		PB15=0;
	}
	else if(Lock_Toggle_CNT==100)
	{
		PB12=0;
		PB13=1;
		PB14=0;
		PB15=1;
		GPIO_Toggle	(GPIOC,	GPIO_Pin_1);	//蜂鸣器
		GPIO_Toggle	(GPIOC,	GPIO_Pin_0);	//运行灯
	}
	else if(Lock_Toggle_CNT==200)
	{
		PB12=1;
		PB13=0;
		PB14=1;
		PB15=0;		
	}
	else if(Lock_Toggle_CNT>=300)
	{
		PB12=0;
		PB13=0;
		PB14=0;
		PB15=0;
		
		PC1=0;		//蜂鸣器
		PC0=1;		//运行灯---关闭
	}

}
#endif
