#ifdef PWM_TEST
#include "PWM_TEST.H"

#include "string.h"
#include "math.h"


#include "STM32_EXTI.H"
#include "STM32_USART.H"

#include "STM32_SYS.H"
#include "STM32_SYSTICK.H"
#include "STM32_GPIO.H"
#include "STM32F10x_BitBand.H"

#define RxBufferSize	16

u16 PWM_Ratio=600;		//占空比
u16 TIMECUNT=0;		//占空比
u8 ADDflg=0;
u8 Ecodata=0;			//0--无变化，1--加，2--减，3--按键
u8 Aflg=0;	//A触点
u8 Bflg=0;	//B触点
u8 Buttonflg=0;	//按键
u32 KeyCount=0;

u32 Key1Flag=0;
u32 Key2Count=0;
u32 Key2Flag=0;
u32 ECount=0;
u32 ECountA=0;
u32 ECountB=0;
u32 TimeOut=0;
u8 PWonFlg=0;
u8 RevBuffer[RxBufferSize]={0};
u8 RxdBuffer[RxBufferSize]={0};
u8 RxNum=0;
void PWM_ECODE(void);			//编码器
void PWM_ECODE2(void);			//编码器
void PWM_DELAY(u32 delay);
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PWM_TEST_Configuration(void)
{
	SYS_Configuration();				//系统配置
	GPIO_DeInitAll();													//将所有的GPIO关闭----V20170605
	SysTick_Configuration(100);	//系统嘀嗒时钟配置72MHz,单位为uS
	
//	GPIO_Configuration0();
//	TIM_Configuration(TIM1,7200,3000);	//定时时间设定
//	PWM_Configuration(TIM2,7200,10000,51);
//	PWM_OUT(TIM1,PWM_OUTChannel1,20000,50);		//PWM设定
//	PWM_OUT(TIM2,PWM_OUTChannel1,5,100);	//PWM设定
//	PWM_OUT(TIM2,PWM_OUTChannel1,1,900);		//sys_led
//	PWM_OUT(TIM1,PWM_OUTChannel1,1200,500);		//PWM设定
//	PWM_OUT2(TIM1,PWM_OUTChannel1,2400,600);		//PWM设定
	PWM_OUT2(TIM3,PWM_OUTChannel3,2400,600);		//PWM设定
	PWM_OUT2(TIM4,PWM_OUTChannel2,2400,600);		//PWM设定
//	TIM1->CCR1 = 40000;
//	GPIO_Configuration_APP50	(GPIOB,	GPIO_Pin_14);			//将GPIO相应管脚配置为APP(复用推挽)输出模式，最大速度50MHz----V20170605
	
//	GPIO_Configuration_IPU(GPIOA,	GPIO_Pin_4);			//将GPIO相应管脚配置为上拉输入模式----V20170605--BUTTON
//	GPIO_Configuration_IPU(GPIOA,	GPIO_Pin_5);			//将GPIO相应管脚配置为上拉输入模式----V20170605--A
//	GPIO_Configuration_IPU(GPIOA,	GPIO_Pin_6);			//将GPIO相应管脚配置为上拉输入模式----V20170605--B
	
	GPIO_Configuration_IPU(GPIOB,	GPIO_Pin_12);			//将GPIO相应管脚配置为上拉输入模式----V20170605--BUTTON
	GPIO_Configuration_IPU(GPIOB,	GPIO_Pin_13);			//将GPIO相应管脚配置为上拉输入模式----V20170605--A
	GPIO_Configuration_IPU(GPIOB,	GPIO_Pin_14);			//将GPIO相应管脚配置为上拉输入模式----V20170605--B
	
//	EXTI_Configuration(GPIOB,GPIO_Pin_12,EXTI_Mode_Interrupt);
//	EXTI_Configuration2(GPIOB,GPIO_Pin_13,EXTI_Mode_Interrupt);
	
	USART_DMA_ConfigurationNR	(USART1,115200,(u32*)RxdBuffer,RxBufferSize);	//USART_DMA配置--查询方式，不开中断
	
	USART_DMAPrintf(USART1,"参数设置方法1（十六进制:FE A9 XX XX）\r\n");					//自定义printf串口DMA发送程序,后边的省略号就是可变参数
	
	for(ECount=0;ECount<0xFFFF;ECount++)
	{
	}
	USART_DMAPrintf(USART1,"参数设置方法2（SET:xx (xx取值为0~60000,50的比值为1/1200））\r\n");					//自定义printf串口DMA发送程序,后边的省略号就是可变参数
	for(ECount=0;ECount<0xFFFF;ECount++)
	{
	}
	USART_DMAPrintf(USART1,"D4J1-D4Q3为PWM输出\r\n");					//自定义printf串口DMA发送程序,后边的省略号就是可变参数
	for(ECount=0;ECount<0xFFFF;ECount++)
	{
	}
	USART_DMAPrintf(USART1,"D4J1为编码器输入，编码器调节步长为1/1200\r\n");					//自定义printf串口DMA发送程序,后边的省略号就是可变参数
	for(ECount=0;ECount<0xFFFF;ECount++)
	{
	}
	
//	TIM3->CCR3 =10;
//	TIM4->CCR2 =10;
	
	PWonFlg=1;
	
//	GPIO_Configuration_IPD(GPIOA,	GPIO_Pin_4);			//将GPIO相应管脚配置为上拉输入模式----V20170605--BUTTON
//	GPIO_Configuration_IPD(GPIOA,	GPIO_Pin_5);			//将GPIO相应管脚配置为上拉输入模式----V20170605
//	GPIO_Configuration_IPD(GPIOA,	GPIO_Pin_6);			//将GPIO相应管脚配置为上拉输入模式----V20170605
	
//	PWM_OUT(TIM2,PWM_OUTChannel1,5,500);		//sys_led
//	PWM_OUT(TIM3,PWM_OUTChannel1,20000,30);		//PWM设定
//	PWM_OUT(TIM4,PWM_OUTChannel1,20000,40);		//PWM设定
//	
//	PWM_OUT(TIM1,PWM_OUTChannel2,20000,50);		//PWM设定
//	PWM_OUT(TIM2,PWM_OUTChannel2,20000,500);	//PWM设定

}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PWM_TEST_Server(void)
{
	
//	SetPWM_Ratio(PWM_Ratio);		//设置占空比
	
	TIMECUNT++;
	if(TIMECUNT>=10)
	{
		TIMECUNT=0;
	}
	RxNum=USART_ReadBufferIDLE	(USART1,(u32*)RevBuffer,(u32*)RxdBuffer);	//串口空闲模式读串口接收缓冲区，如果有数据，将数据拷贝到RevBuffer,并返回接收到的数据个数，然后重新将接收缓冲区地址指向RxdBuffer
	if(RxNum)
	{
		if(RevBuffer[0]==0xFE&&RevBuffer[1]==0xA9)
		{
			PWM_Ratio=(RevBuffer[2]<<8)|(RevBuffer[3]<<0);
			
			if(PWM_Ratio>1200)
				PWM_Ratio=1200;
			USART_DMAPrintf(USART1,"当前输出占空比:%4d/1200\r\n",PWM_Ratio);					//自定义printf串口DMA发送程序,后边的省略号就是可变参数
			TIM1->CCR1 =PWM_Ratio*50;
			TIM3->CCR3 =PWM_Ratio*50;
			TIM4->CCR2 =PWM_Ratio*50;
		}
		else if(RevBuffer[0]==0x53&&RevBuffer[1]==0x45&&RevBuffer[2]==0x54&&RevBuffer[3]==0x3A)
		{
			u8 i=0;
			double Npow=0;			
			PWM_Ratio=0;
			if(RxNum>=9&&RevBuffer[4]>=0x36)
			{
				PWM_Ratio=60000;
			}
			else
			{
				RxNum=RxNum-4;
				while(RxNum>i)
				{
					i++;
					Npow=(double)pow((float)10,(float)(RxNum-i));
					
					PWM_Ratio+=(u16)(RevBuffer[i+3]-0x30)*Npow;				
				}
			}
			TIM1->CCR1 =PWM_Ratio;
			TIM3->CCR3 =PWM_Ratio;
			TIM4->CCR2 =PWM_Ratio;
			Npow=(double)PWM_Ratio/50.0;
			PWM_Ratio	=(u16)Npow;
			USART_DMAPrintf(USART1,"当前输出占空比:%4.4f/1200\r\n",Npow);					//自定义printf串口DMA发送程序,后边的省略号就是可变参数
		}
		memset(RevBuffer, 0x00,16);
	}
//	if(PWonFlg)
	PWM_ECODE2();			//编码器
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void PWM_ECODE2(void)
{
	if(PB12in==0)			//EXTI_GetFlagStatus(u32 EXTI_Line);//PA6in==0
	{
		if(Key1Flag==0)
		{
			KeyCount++;
			if(KeyCount>=200)
			{
				Key1Flag=1;
				if(PWM_Ratio<=1100)		//按键标识
				{
					PWM_Ratio+=100;
				}
				else
				{
					PWM_Ratio=0;
				}	
				USART_DMAPrintf(USART1,"当前输出占空比:%4d/1200\r\n",PWM_Ratio);					//自定义printf串口DMA发送程序,后边的省略号就是可变参数				
				TIM3->CCR3 =PWM_Ratio*50;
				TIM4->CCR2 =PWM_Ratio*50;
				
			}
		}
	}
	else
	{
		KeyCount=0;
		Key1Flag=0;
	}
	
	if(PB13in==0)			//EXTI_GetFlagStatus(u32 EXTI_Line);//PA6in==0
	{
		ECountA++;
		if(PB14in==0)
		{
			ECountB++;
		}
		else
		{
			ECountB=0;
		}
	}	
	else
	{
		if(ECountA>=200)
		{
			if(ECountA>ECountB+20)
			{
				if(PWM_Ratio<1200)
					PWM_Ratio+=1;
			}
			else
			{
				if(PWM_Ratio>=1)
					PWM_Ratio-=1;
			}	
			USART_DMAPrintf(USART1,"当前输出占空比:%4d/1200\r\n",PWM_Ratio);					//自定义printf串口DMA发送程序,后边的省略号就是可变参数			
			TIM3->CCR3 =PWM_Ratio*50;
			TIM4->CCR2 =PWM_Ratio*50;
			
		}
		ECountA=0;
		ECountB=0;
	}
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void PWM_ECODE(void)
{
	if(EXTI_GetITStatus(EXTI_Line12)==SET)			//EXTI_GetFlagStatus(u32 EXTI_Line);//PA6in==0
	{
		KeyCount=0;
		while(KeyCount++<0xFFFF);
		if(PWM_Ratio<=1100)		//按键标识
		{
			PWM_Ratio+=100;
		}
		else
		{
			PWM_Ratio=0;
		}
		USART_DMAPrintf(USART1,"当前输出占空比:%4d/1200\r\n",PWM_Ratio);					//自定义printf串口DMA发送程序,后边的省略号就是可变参数
//		TIM1->CCR1 =PWM_Ratio*50;
		TIM3->CCR3 =PWM_Ratio*50;
		TIM4->CCR2 =PWM_Ratio*50;
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
	
	if(EXTI_GetITStatus(EXTI_Line13)==SET)			//EXTI_GetFlagStatus(u32 EXTI_Line);//PA6in==0
	{
		TimeOut=0;
		Bflg=0;
		ECount=0;
		ECountA=0;
		ECountB=0;
		
		while(((PB13in==0||PB14in==0)||Bflg==0)&&(TimeOut<=0xFFF))
		{
			TimeOut++;
			if((PB13in==0&&PB14in==0)&&ECountA==0)
			{
				ECountA++;
				ECountB++;
			}
			else if(ECountA!=0)
			{
				if(PB13in==0)
				{
					ECountA++;
					ECount=0;
				}
				else
				{
					ECount++;				
				}
				if(PB14in==0)
				{
					ECountB++;
					ECount=0;
				}
				else
				{
					ECount++;				
				}
				if(ECount>=200)
				{
					Bflg=1;
				}
			}
		}
		
		if(ECountA>ECountB)
		{
			if(PWM_Ratio<1200)
				PWM_Ratio+=1;
		}
		else
		{
			if(PWM_Ratio>=1)
				PWM_Ratio-=1;
		}		
		EXTI_ClearITPendingBit(EXTI_Line13);
		
		USART_DMAPrintf(USART1,"当前输出占空比:%4d/1200\r\n",PWM_Ratio);					//自定义printf串口DMA发送程序,后边的省略号就是可变参数
//		TIM1->CCR1 =PWM_Ratio*50;
		TIM3->CCR3 =PWM_Ratio*50;
		TIM4->CCR2 =PWM_Ratio*50;
//		EXTI_ClearITPendingBit(EXTI_Line13);
	}
}

/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void PWM_DELAY(u32 delay)
{
	while(delay--);
}


#endif
