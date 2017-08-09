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

#ifdef PM004V10			//小推车主控板

#include "PM004V10.H"

#include "STM32_USART.H"

#include "STM32F10x_BitBand.H"
#include "STM32_GPIO.H"
#include "STM32_SYS.H"
#include "STM32_SYSTICK.H"
#include "STM32_WDG.H"
#include "STM32_PWM.H"

#include "string.h"				//串和内存操作函数头文件
#include "stm32f10x_dma.h"

#define RxBufferSize	256

u8 RevBuffer[RxBufferSize]={0};
u8 RxdBuffer[RxBufferSize]={0};

u8 TevBuffer[RxBufferSize]={0};
u8 TxdBuffer[RxBufferSize]={0};

u8 strings[]="串口空闲模式读串口接收缓冲区，如果有数据，将数据拷贝到RevBuffer,并返回接收到的数据个数，然后重新将接收缓冲区地址指向RxdBuffer\n";

u16 RxNum=0;
u32	RuningTime=0;
u16 Tcnt=0;

/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PM004V10_Configuration(void)
{
	SYS_Configuration();							//系统配置---打开系统时钟 STM32_SYS.H
	
	GPIO_DeInitAll();									//将所有的GPIO关闭----V20170605
	
	USART_DMA_ConfigurationNR	(USART2,115200,(u32*)RxdBuffer,RxBufferSize);	//USART_DMA配置--查询方式，不开中断
	
	SysTick_Configuration(100);				//系统嘀嗒时钟配置72MHz,单位为uS
	
	IWDG_Configuration(1000);					//独立看门狗配置---参数单位ms	
	
	PWM_OUT(TIM2,PWM_OUTChannel1,1,900);						//PWM设定-20161127版本--SYS-LED

}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PM004V10_Server(void)
{	
	IWDG_Feed();								//独立看门狗喂狗
	RuningTime++;
	if(RuningTime>=200)				//1秒
	{
		RuningTime=0;
		Tcnt++;
		if(Tcnt>=1000)
			Tcnt=0;
		USART_DMAPrintf(USART2,"串口空闲模式读串口接收缓冲区，如果有数据，将数据拷贝到RevBuffer,并返回接收到的数据个数，然后重新将接收缓冲区地址指向RxdBuffer---%4d\n",Tcnt);					//自定义printf串口DMA发送程序,后边的省略号就是可变参数
//		USART_DMASend(USART2,(u32*)strings,strlen(strings));											//串口DMA发送程序
	}
	RxNum=USART_ReadBufferIDLE(USART2,(u32*)RevBuffer,(u32*)RxdBuffer);	//串口空闲模式读串口接收缓冲区，如果有数据，将数据拷贝到RevBuffer,并返回接收到的数据个数，然后重新将接收缓冲区地址指向RxdBuffer
	if(RxNum)
	{
		USART_DMASend(USART2,(u32*)RevBuffer,RxNum);											//串口DMA发送程序
	}
}











#endif
