#ifdef PWM_TEST
#include "PWM_TEST.H"

#include "STM32_SYS.H"
#include "STM32_SYSTICK.H"
#include "STM32_GPIO.H"
#include "STM32F10x_BitBand.H"

u16 PWM_Ratio=600;		//占空比
u16 TIMECUNT=0;		//占空比
u8 ADDflg=0;
u8 Ecodata=0;			//0--无变化，1--加，2--减，3--按键
u8 Aflg=0;	//A触点
u8 Bflg=0;	//B触点
u8 Buttonflg=0;	//按键
u16 KeyCount=0;
u16 ECount=0;

void PWM_ECODE(void);			//编码器
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
	PWM_OUT(TIM2,PWM_OUTChannel1,1,900);		//sys_led
//	PWM_OUT(TIM1,PWM_OUTChannel1,1200,500);		//PWM设定
	PWM_OUT2(TIM1,PWM_OUTChannel1,2400,600);		//PWM设定
//	TIM1->CCR1 = 40000;
//	GPIO_Configuration_APP50	(GPIOB,	GPIO_Pin_14);			//将GPIO相应管脚配置为APP(复用推挽)输出模式，最大速度50MHz----V20170605
	
	GPIO_Configuration_IPU(GPIOA,	GPIO_Pin_4);			//将GPIO相应管脚配置为上拉输入模式----V20170605--BUTTON
	GPIO_Configuration_IPU(GPIOA,	GPIO_Pin_5);			//将GPIO相应管脚配置为上拉输入模式----V20170605--A
	GPIO_Configuration_IPU(GPIOA,	GPIO_Pin_6);			//将GPIO相应管脚配置为上拉输入模式----V20170605--B
	
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
//		if(ADDflg==0)
//		{
//			PWM_Ratio++;
//			if(PWM_Ratio>=1200)
//			{
//				ADDflg=1;
//				PWM_Ratio=0;
//			}
//		}
//		else
//		{
//			PWM_Ratio--;
//			if(PWM_Ratio<=0)
//			{
//				ADDflg=0;
//				PWM_Ratio--;
//				PWM_Ratio=0;
//			}			
//		}
//		TIM1->CCR1 = PWM_Ratio*50;
	}
	PWM_ECODE();			//编码器
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void PWM_ECODE(void)
{
	Aflg=0;	//A触点
	Bflg=0;	//B触点
	if(PA4in==0)
	{
		KeyCount++;
//		if(Buttonflg==0)		//按键标识
//		{
//			Buttonflg=1;	//按键
//			PWM_Ratio=0;
//			TIM1->CCR1 =0;
//		}
//		else
//		{
//			Buttonflg=0;	//按键
//			PWM_Ratio=60000;
//			TIM1->CCR1 =50000;
//		}
	}
	else if(KeyCount>=100)
	{		
		if(Buttonflg==0)		//按键标识
		{
			Buttonflg=1;	//按键
			PWM_Ratio=100;
		}
		else
		{
			Buttonflg=0;	//按键
			PWM_Ratio=1100;			
		}
		KeyCount=0;
		TIM1->CCR1 =PWM_Ratio*50;
	}
	else
	{
		KeyCount=0;
	}
	
	if(PA6in==0)
	{
		ECount++;
	}
	else if(ECount>20)
	{
		if(PA5in==0)
		{
			if(PWM_Ratio<1200)
				PWM_Ratio+=10;
		}
		else
		{
			if(PWM_Ratio>=10)
				PWM_Ratio-=10;
		}
		ECount=0;
		TIM1->CCR1 =PWM_Ratio*50;
	}
	else
	{
		ECount=0;
	}

}




#endif
