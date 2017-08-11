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

#ifdef PC006V20			//分检机控制板

#include "PC006V20.H"

#include "STM32_GPIO.H"
#include "CD4511.H"
#include "CD4511BM_TEST.H"
#include "STM32_PWM.H"
#include "STM32_SYS.H"
#include "STM32_SYSTICK.H"
#include "STM32F10x_BitBand.H"


#define	CD4511BM_DEBUG_TEST			//调试时数码管计数显示，正常使用时应该注释掉


u16 Time=0;
u16 data=0;
u8 Flag=0;


CD4511_Pindef CD4511_Pin;



/*******************************************************************************
*函数名		:	function
*功能描述	:	函数功能说明
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void PC006V20_Configuration(void)
{
	SYS_Configuration();											//系统配置 STM32_SYS.H	
	PWM_OUT(TIM2,PWM_OUTChannel1,1,500);
	SysTick_Configuration(1000);							//系统嘀嗒时钟配置72MHz,单位为uS
	
	//位
	GPIO_Configuration_OPP50	(GPIOC,		GPIO_Pin_4);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	GPIO_Configuration_OPP50	(GPIOC,		GPIO_Pin_5);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	
	GPIO_Configuration_OPP50	(GPIOB,		GPIO_Pin_14);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	GPIO_Configuration_OPP50	(GPIOB,		GPIO_Pin_15);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605


}

/*******************************************************************************
*函数名		:	function
*功能描述	:	函数功能说明
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void PC006V20_Server(void)
{
	++Time;	
//	if(Time<=50)
//	{
//		PB14=0;
//		PB15=1;		
//	}
//	else	if(Time<=100)
//	{
//		PB14=1;
//		PB15=1;				
//	}
//	else	if(Time<=150)
//	{
//		PB14=1;
//		PB15=0;				
//	}
//	else if(Time>5000)	//反转
//	{
//		Time=0;
//	}
//	else
//	{
//		PB14=1;
//		PB15=1;
//	}
	if(Time>2000)	//反转
	{
		Time=0;
		if(Flag==0)
		{
			Flag=1;
		}
		else
		{
			Flag=0;
		}
	}
	else if(Time<50)
	{
		if(Flag==0)
		{
			PB14=0;
			PB15=1;
		}
		else
		{
			PB14=1;
			PB15=0;
		}		
	}
	else
	{
		PB14=1;
		PB15=1;	
	}
	
	
//	if(Time<=100)	//停止
//	{
//		PC4=0;
//		PC5=0;
//	}
//	else if((Time>100)&&(Time<=10000))	//正转
//	{
//		PC4=1;
//		PC5=0;
//	}
//	else if((Time>10000)&&(Time<=10100))	//停止
//	{
//		PC4=0;
//		PC5=0;
//	}
//	else if((Time>10100)&&(Time<=20000))	//反转
//	{
//		PC4=0;
//		PC5=1;
//	}
//	else if(Time>20000)	//反转
//	{
//		PC4=0;
//		PC5=0;
//		Time=0;
//	}
}

#endif
