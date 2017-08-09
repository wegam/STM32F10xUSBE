



#ifdef PM001V20				//网关板

#include "PM001V20.H"



#include "STM32_GPIO.H"
#include "STM32_SYS.H"
#include "STM32_SYSTICK.H"
#include "STM32_WDG.H"
#include "STM32_USART.H"
#include "STM32_PWM.H"

#include "string.h"				//串和内存操作函数头文件
#include "stm32f10x_dma.h"

#define BufferSize	512

u16 sysledcnt=0;			//系统运行指示灯扫描计数  0.5秒

u8 txBuffer[BufferSize]={0};
u8 rxBuffer[BufferSize]={0};

u8	txflg=0;	//发送标志
u8	rxflg=0;	//接收标志
u8	bpflg=0;	//蜂鸣器使能标志

/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PM001V20_Configuration(void)
{
	SYS_Configuration();					//系统配置---打开系统时钟 STM32_SYS.H
	
	SysTick_Configuration(1000);	//系统嘀嗒时钟配置72MHz,单位为uS
	
	IWDG_Configuration(1000);			//独立看门狗配置---参数单位ms
	
	GPIO_DeInitAll();							//将所有的GPIO关闭----V20170605
	
	GPIO_Configuration_OOD2	(GPIOC,GPIO_Pin_0);			//SysLed//将GPIO相应管脚配置为OD(开漏)输出模式，最大速度2MHz----V20170605
	
	GPIO_Configuration_OPP2	(GPIOC,GPIO_Pin_1);			//BUZZER//将GPIO相应管脚配置为OD(开漏)输出模式，最大速度2MHz----V20170605
	
	PM001V20_USART1();
	
	PWM_OUT(TIM2,PWM_OUTChannel1,1,500);	//PWM设定-20161127版本
	

}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PM001V20_Server(void)
{	
	IWDG_Feed();								//独立看门狗喂狗
	PM001V20_SysLed();					//系统运行指示灯	
	PM001V20_USART1_Server();
}

/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void PM001V20_SysLed(void)
{
	if(sysledcnt++>=1000)			//系统运行指示灯扫描计数  0.5秒
	{
		sysledcnt=0;
		GPIO_Toggle	(GPIOC,GPIO_Pin_0);		//SysLed//将GPIO相应管脚输出翻转----V20170605
		if(bpflg)
		GPIO_Toggle	(GPIOC,GPIO_Pin_1);		//BUZZER//将GPIO相应管脚输出翻转----V20170605
	}
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void PM001V20_USART1(void)
{
	USART_DMA_Configuration(USART1,115200,1,1,(u32*)rxBuffer,(u32*)txBuffer,BufferSize);	//USART_DMA配置
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC, DISABLE);
	USART_ITConfig(USART1,USART_IT_IDLE, DISABLE);					//使用空闲中断，DMA自动接收，检测到总线空闲表示发送端已经发送完成，数据保存在DMA缓冲器中
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void PM001V20_USART1_Server(void)
{
	u32 num=0;
	if(USART_GetFlagStatus(USART1,USART_FLAG_IDLE))
	{
		USART_ClearFlag(USART1,USART_FLAG_IDLE); 										//清除空闲串口标志位
		DMA_Cmd(DMA1_Channel5,DISABLE);    																//关闭接收DMA
		num = USART1->DR; 																					//读出数据以完成清除标志			
		num = BufferSize -  DMA_GetCurrDataCounter(DMA1_Channel5);	//得到真正接收数据个数
		DMA1_Channel5->CMAR=(u32)rxBuffer;													//重新设置DMA接收地址
		DMA1_Channel5->CNDTR=BufferSize;														//重新设置接收数据个数			
		DMA_Cmd(DMA1_Channel5,ENABLE);  														//开启接收DMA	
		
		if(num==1)
		{
			if(rxBuffer[0]==0xFA)
			{
				bpflg=1;	//蜂鸣器使能标志
			}
			else if(rxBuffer[0]==0xFB)
			{
				bpflg=0;	//蜂鸣器使能标志
				GPIO_WriteBit(GPIOC,GPIO_Pin_1,Bit_RESET);			//将相应GPIO管脚输出为高
			}
		}
		if(num)
		{
			txflg=1;	//发送标志
			memcpy(txBuffer, rxBuffer,num);
			USART_DMASend(USART1,(u32*)txBuffer,num);		//自定义printf串口DMA发送程序
			memset(rxBuffer, 0x00, BufferSize);
		}		
	}
	if(DMA_GetFlagStatus(DMA1_FLAG_GL4))
	{
		if(USART_GetFlagStatus(USART1,USART_FLAG_TC))
		{
			USART_ClearFlag(USART1,USART_FLAG_TC); 										//清除空闲串口标志位
			DMA_ClearFlag(DMA1_FLAG_GL4);
			DMA_Cmd(DMA1_Channel4,DISABLE);  														//开启接收DMA
		}
	}
	
	
}
#endif
