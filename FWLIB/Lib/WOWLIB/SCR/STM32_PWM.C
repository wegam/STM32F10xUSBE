/******************************** User_library *********************************
* 文件名 	: STM32_SDCard.H
* 作者   	: wegam@sina.com
* 版本   	: V
* 日期   	: 2016/01/01
* 说明   	: 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/



#include "STM32_PWM.H"


/*##############################################################################
################################################################################
# 模块名		:	STM32_PWM	
# 功能描述	:	PWM配置使用
-------------------------------------------------------------------------------	
***********************************************************************************************************
---------------------------------------------------------------------------------------	
外设	|	CH1		|	CH2		|	CH3		|	CH4		|								|								|								|
---------------------------------------------------------------------------------------	
TIM1	|	PA8		|	PA9		|	PA10	|	PA11	|								|								|								|
---------------------------------------------------------------------------------------	
TIM2	|	PA0		|	PA1		|	PA2		|	PA3		|								|								|								|
---------------------------------------------------------------------------------------	
TIM3	|	PA6		|	PA7		|	PB0		|	PB1		|								|								|								|
---------------------------------------------------------------------------------------	
TIM4	|	PB6		|	PB7		|	PB8		|	PB9		|								|								|								|
---------------------------------------------------------------------------------------	
TIM5	|				|				|				|				|								|								|								|
---------------------------------------------------------------------------------------	
TIM6	|				|				|				|				|								|								|								|
---------------------------------------------------------------------------------------	
TIM7	|				|				|				|				|								|								|								|
---------------------------------------------------------------------------------------	
TIM8	|				|				|				|				|								|								|								|
---------------------------------------------------------------------------------------	


################################################################################
###############################################################################*/
void PWM_ClockCmd(TIM_TypeDef* TIMx,u16 Prescaler,u16 Period,u8 PWM_RATIO);		//开启相关GPIO时钟	
void PWM_GPIO_Configuration(void);			//PWM使用的GPIO配置
void PWM_SET(TIM_TypeDef* TIMx,u8 PWM_RATIO);	//PWM设定
/*******************************************************************************
* 函数名		:	PWM_OUT	
* 功能描述	:		 
* 输入		:	PWM_Frequency 频率，单位Hz	
* 输出		:
* 返回 		:
*******************************************************************************/
void PWM_OUT			//PWM输出配置
(
	TIM_TypeDef* TIMx,											//所使用的定时器
	PWM_OUTChannelTypeDef PWM_OUTChanneln,	//PWM输出通道号
	double PWM_Frequency,										//输出频率，最小频率0.02Hz		
	u16 PWM_Ratio														//输出占空比，分辨率1/1000
)
{
	//*1,结构体定义
	//*2,变量定义
	//*3,管脚确认
	//*4,打开相应的时钟
	//*5,管脚配置（初始化）
	//*6,定时器配置（初始化）
	//*7,PWM输出配置（初始化）
	//*8,占空比配置	
	
	//*1,结构体定义***********************************************************************
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;	//定时器结构体定义
	TIM_OCInitTypeDef TIMOCInitStructure;
	RCC_ClocksTypeDef RCC_ClocksStatus;							//时钟状态---时钟值	
	//*2,变量定义*************************************************************************	
	GPIO_TypeDef* GPIOx				=	0;			//x=A/B/C/D/E/F/G
	u16 GPIO_Pin_n						=	0xFF;		//n=0~15;
	u32 RCC_APB2Periph_GPIOx	=	0x00;		//x=A/B/C/D/E/F/G	
	u32	TIMx_Frequency=0;								//	定时器时钟
	u16 TIMx_Prescaler				=	0	;			//	定时器时钟分频值		取值范围：0x0000~0xFFFF
  u16 TIMx_Period						=	0	;			//	定时器自动重装载值	取值范围：0x0000~0xFFFF
//	u32	Microsecond						=	0	;		//	微秒
//	u32	Nanosecond						=	0	;		//	纳秒
	double	Tim_num1					=	0	;			//	临时变量1
//	u8	Tim_flg								=	0	;			//	临时变量2
	//*3,管脚配置--根据输入参数选择相应的GPIO************************************************
	switch (*(u32*)&TIMx)
	{
		case TIM1_BASE:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
			switch(PWM_OUTChanneln)
			{
				case PWM_OUTChannel1:	GPIOx	=	GPIOA;	GPIO_Pin_n	=	GPIO_Pin_8;		RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOA;	
															break;
				case PWM_OUTChannel2:	GPIOx	=	GPIOA;	GPIO_Pin_n	=	GPIO_Pin_9;		RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOA;
															break;
				case PWM_OUTChannel3:	GPIOx	=	GPIOA;	GPIO_Pin_n	=	GPIO_Pin_10;	RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOA;
															break;
				case PWM_OUTChannel4:	GPIOx	=	GPIOA;	GPIO_Pin_n	=	GPIO_Pin_11;	RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOA;
															break;
			}
			break;
		
		case TIM2_BASE:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
			switch(PWM_OUTChanneln)
			{
				case PWM_OUTChannel1:	GPIOx	=	GPIOA;	GPIO_Pin_n	=	GPIO_Pin_0;		RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOA;
															break;
				case PWM_OUTChannel2:	GPIOx	=	GPIOA;	GPIO_Pin_n	=	GPIO_Pin_1;		RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOA;
															break;
				case PWM_OUTChannel3:	GPIOx	=	GPIOA;	GPIO_Pin_n	=	GPIO_Pin_2;		RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOA;
															break;
				case PWM_OUTChannel4:	GPIOx	=	GPIOA;	GPIO_Pin_n	=	GPIO_Pin_3;		RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOA;
															break;
			}
			break;
		
		case TIM3_BASE:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
			switch(PWM_OUTChanneln)
			{
				case PWM_OUTChannel1:	GPIOx	=	GPIOA;	GPIO_Pin_n	=	GPIO_Pin_6;		RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOA;
															break;
				case PWM_OUTChannel2:	GPIOx	=	GPIOA;	GPIO_Pin_n	=	GPIO_Pin_7;		RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOA;
															break;
				case PWM_OUTChannel3:	GPIOx	=	GPIOB;	GPIO_Pin_n	=	GPIO_Pin_0;		RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOB;
															break;
				case PWM_OUTChannel4:	GPIOx	=	GPIOB;	GPIO_Pin_n	=	GPIO_Pin_1;		RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOB;
															break;
			}
			break;
		
		case TIM4_BASE:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
			switch(PWM_OUTChanneln)
			{
				case PWM_OUTChannel1:	GPIOx	=	GPIOB;	GPIO_Pin_n	=	GPIO_Pin_6;		RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOB;
															break;
				case PWM_OUTChannel2:	GPIOx	=	GPIOB;	GPIO_Pin_n	=	GPIO_Pin_7;		RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOB;
															break;
				case PWM_OUTChannel3:	GPIOx	=	GPIOB;	GPIO_Pin_n	=	GPIO_Pin_8;		RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOB;
															break;
				case PWM_OUTChannel4:	GPIOx	=	GPIOB;	GPIO_Pin_n	=	GPIO_Pin_9;		RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOB;
															break;
			}
			break;
		
		case TIM5_BASE:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
			break;
		
		case TIM6_BASE:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
			break;
		
		case TIM7_BASE:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
			break;
		
		case TIM8_BASE:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
			break;
		
		default:
			break;		
	}
	
	
	//*4,打开相应的时钟*********************************************************************
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//使能复用时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx, ENABLE);	//使能引脚时钟	
	//*5,管脚配置（初始化）*****************************************************************
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_n;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP ;//复用推挽输出
	GPIO_Init(GPIOx,&GPIO_InitStructure);	
	//*6,获取TIMx时钟**********************************************************************
	//1）-----分频值及自动重装载值计算（PWM_Frequency 频率，单位Hz）
	//--------1MHz 1us=1000ns,1KHz 10us=10000ns
	RCC_GetClocksFreq(&RCC_ClocksStatus);	//获取时钟参数
	TIMx_Frequency = RCC_ClocksStatus.SYSCLK_Frequency;;
	if (((*(u32*)&TIMx)&APB2PERIPH_BASE) == APB2PERIPH_BASE)
  {
    TIMx_Frequency = RCC_ClocksStatus.PCLK2_Frequency;	//APB2
  }
  else
  {
    TIMx_Frequency = RCC_ClocksStatus.PCLK1_Frequency;	//APB1
  }
	//*7,计算定时器参数*********************************************************************
	//Fsys==Fpwm*Count==Fpwm*(Prescaler*Period)	
	//	TIMx_Prescaler				=	72-1		;		// 	定时器时钟分频值
	//	TIMx_Period						=	1000-1	;		// 	定时器自动重装载值
	//	Tim_num1							=	0				;		//	临时变量1
	if(1/PWM_Frequency>10)
	{
		Tim_num1=(double)(TIMx_Frequency)/(PWM_Frequency*100);							//根据定时器输出频率计算计数值
		TIMx_Prescaler=60000;
		TIMx_Period=(u16)(((double)Tim_num1/(double)TIMx_Prescaler)*200);
	}
	else if(1/PWM_Frequency>1.0)
	{
		Tim_num1=(double)(TIMx_Frequency)/(PWM_Frequency*10);							//根据定时器输出频率计算计数值
		TIMx_Prescaler=60000;
		TIMx_Period=(u16)(((double)Tim_num1/(double)TIMx_Prescaler)*20);
	}
	else if(PWM_Frequency>=1.0)
	{
		Tim_num1=(double)(TIMx_Frequency)/(PWM_Frequency);							//根据定时器输出频率计算计数值
		Tim_num1=(double)(Tim_num1*2.0);
		if(Tim_num1>=10000000)
		{
			TIMx_Prescaler=2000;
			TIMx_Period=Tim_num1/TIMx_Prescaler;
		}
		else if(Tim_num1>=1000000)
		{
			TIMx_Prescaler=200;
			TIMx_Period=Tim_num1/TIMx_Prescaler;
		}
		else if(Tim_num1>=100000)
		{
			TIMx_Prescaler=20;
			TIMx_Period=Tim_num1/TIMx_Prescaler;
		}
		else if(Tim_num1>=65530)
		{
			TIMx_Prescaler=2;
			TIMx_Period=Tim_num1/TIMx_Prescaler;
		}
		else
		{
			TIMx_Prescaler=1;
			TIMx_Period=Tim_num1/TIMx_Prescaler;
		}
	}	
	//8)**********定时器初始化
	TIM_TimeBaseStructure.TIM_Prescaler = TIMx_Prescaler-1; 				//设定分频值
	TIM_TimeBaseStructure.TIM_Period 		= TIMx_Period-1;        		//设定自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  				//不分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  		//向上计数
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);		//初始化	
	//9）定时器配置（初始化*******************************************************************
	if((TIMx_Period*(u32)PWM_Ratio)>1000)
		PWM_Ratio=(u16)((u32)(TIMx_Period*PWM_Ratio)/1000);
	else if(PWM_Ratio!=0)
		PWM_Ratio=1;
		
	TIMOCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 								//选择定时器模式:TIM脉冲宽度调制模式1
	TIMOCInitStructure.TIM_Pulse =(u16)PWM_Ratio;										//占空比=(CCRx/ARR)*1000%
	TIMOCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;				//输出极性:TIM输出比较极性高
	TIMOCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		//比较输出使能
	//10）定时器配置（初始化*******************************************************************
	switch(PWM_OUTChanneln)
	{
		case PWM_OUTChannel1:	TIM_OC1Init(TIMx, &TIMOCInitStructure);												//TIM2的CH1输出使能
		break;
		case PWM_OUTChannel2:	TIM_OC2Init(TIMx, &TIMOCInitStructure);												//TIM2的CH1输出使能
		break;
		case PWM_OUTChannel3:	TIM_OC3Init(TIMx, &TIMOCInitStructure);												//TIM2的CH1输出使能
		break;
		case PWM_OUTChannel4:	TIM_OC4Init(TIMx, &TIMOCInitStructure);												//TIM2的CH1输出使能
		break;
		default:
		break;
	}	
	TIM_CtrlPWMOutputs(TIMx,ENABLE);																//设置TIM2的PWM输出使能
	//11）定时器配置（初始化*******************************************************************
	switch(PWM_OUTChanneln)			//使能TIM在CCR上的预装载寄存器
	{
		case PWM_OUTChannel1:	TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
				break;
		case PWM_OUTChannel2:	TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);
				break;
		case PWM_OUTChannel3:	TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);
				break;
		case PWM_OUTChannel4:	TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);
				break;
	}	
	
	TIM_ARRPreloadConfig(TIMx, ENABLE);			//ARPE使能 
	
	TIM_Cmd(TIMx, ENABLE); 									//使能TIM
}

/*******************************************************************************
* 函数名		:	PWM_OUT	
* 功能描述	:		 
* 输入		:	PWM_Frequency 频率，单位Hz	
* 输出		:
* 返回 		:
*******************************************************************************/
void PWM_OUT2			//PWM输出配置
(
	TIM_TypeDef* TIMx,											//所使用的定时器
	PWM_OUTChannelTypeDef PWM_OUTChanneln,	//PWM输出通道号
	double PWM_Frequency,										//输出频率，最小频率0.02Hz		
	u16 PWM_Ratio														//输出占空比，分辨率1/1000
)
{
	//*1,结构体定义
	//*2,变量定义
	//*3,管脚确认
	//*4,打开相应的时钟
	//*5,管脚配置（初始化）
	//*6,定时器配置（初始化）
	//*7,PWM输出配置（初始化）
	//*8,占空比配置	
	
	//*1,结构体定义***********************************************************************
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;	//定时器结构体定义
	TIM_OCInitTypeDef TIMOCInitStructure;
	RCC_ClocksTypeDef RCC_ClocksStatus;							//时钟状态---时钟值	
	//*2,变量定义*************************************************************************	
	GPIO_TypeDef* GPIOx				=	0;			//x=A/B/C/D/E/F/G
	u16 GPIO_Pin_n						=	0xFF;		//n=0~15;
	u32 RCC_APB2Periph_GPIOx	=	0x00;		//x=A/B/C/D/E/F/G	
	u32	TIMx_Frequency=0;								//	定时器时钟
	u16 TIMx_Prescaler				=	0	;			//	定时器时钟分频值		取值范围：0x0000~0xFFFF
  u16 TIMx_Period						=	0	;			//	定时器自动重装载值	取值范围：0x0000~0xFFFF
//	u32	Microsecond						=	0	;		//	微秒
//	u32	Nanosecond						=	0	;		//	纳秒
	double	Tim_num1					=	0	;			//	临时变量1
//	u8	Tim_flg								=	0	;			//	临时变量2
	//*3,管脚配置--根据输入参数选择相应的GPIO************************************************
	switch (*(u32*)&TIMx)
	{
		case TIM1_BASE:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
			switch(PWM_OUTChanneln)
			{
				case PWM_OUTChannel1:	GPIOx	=	GPIOA;	GPIO_Pin_n	=	GPIO_Pin_8;		RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOA;	
															break;
				case PWM_OUTChannel2:	GPIOx	=	GPIOA;	GPIO_Pin_n	=	GPIO_Pin_9;		RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOA;
															break;
				case PWM_OUTChannel3:	GPIOx	=	GPIOA;	GPIO_Pin_n	=	GPIO_Pin_10;	RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOA;
															break;
				case PWM_OUTChannel4:	GPIOx	=	GPIOA;	GPIO_Pin_n	=	GPIO_Pin_11;	RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOA;
															break;
			}
			break;
		
		case TIM2_BASE:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
			switch(PWM_OUTChanneln)
			{
				case PWM_OUTChannel1:	GPIOx	=	GPIOA;	GPIO_Pin_n	=	GPIO_Pin_0;		RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOA;
															break;
				case PWM_OUTChannel2:	GPIOx	=	GPIOA;	GPIO_Pin_n	=	GPIO_Pin_1;		RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOA;
															break;
				case PWM_OUTChannel3:	GPIOx	=	GPIOA;	GPIO_Pin_n	=	GPIO_Pin_2;		RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOA;
															break;
				case PWM_OUTChannel4:	GPIOx	=	GPIOA;	GPIO_Pin_n	=	GPIO_Pin_3;		RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOA;
															break;
			}
			break;
		
		case TIM3_BASE:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
			switch(PWM_OUTChanneln)
			{
				case PWM_OUTChannel1:	GPIOx	=	GPIOA;	GPIO_Pin_n	=	GPIO_Pin_6;		RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOA;
															break;
				case PWM_OUTChannel2:	GPIOx	=	GPIOA;	GPIO_Pin_n	=	GPIO_Pin_7;		RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOA;
															break;
				case PWM_OUTChannel3:	GPIOx	=	GPIOB;	GPIO_Pin_n	=	GPIO_Pin_0;		RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOB;
															break;
				case PWM_OUTChannel4:	GPIOx	=	GPIOB;	GPIO_Pin_n	=	GPIO_Pin_1;		RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOB;
															break;
			}
			break;
		
		case TIM4_BASE:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
			switch(PWM_OUTChanneln)
			{
				case PWM_OUTChannel1:	GPIOx	=	GPIOB;	GPIO_Pin_n	=	GPIO_Pin_6;		RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOB;
															break;
				case PWM_OUTChannel2:	GPIOx	=	GPIOB;	GPIO_Pin_n	=	GPIO_Pin_7;		RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOB;
															break;
				case PWM_OUTChannel3:	GPIOx	=	GPIOB;	GPIO_Pin_n	=	GPIO_Pin_8;		RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOB;
															break;
				case PWM_OUTChannel4:	GPIOx	=	GPIOB;	GPIO_Pin_n	=	GPIO_Pin_9;		RCC_APB2Periph_GPIOx	=	RCC_APB2Periph_GPIOB;
															break;
			}
			break;
		
		case TIM5_BASE:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
			break;
		
		case TIM6_BASE:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
			break;
		
		case TIM7_BASE:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
			break;
		
		case TIM8_BASE:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
			break;
		
		default:
			break;		
	}
	
	
	//*4,打开相应的时钟*********************************************************************
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//使能复用时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx, ENABLE);	//使能引脚时钟	
	//*5,管脚配置（初始化）*****************************************************************
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_n;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP ;//复用推挽输出
	GPIO_Init(GPIOx,&GPIO_InitStructure);	
	//*6,获取TIMx时钟**********************************************************************
	//1）-----分频值及自动重装载值计算（PWM_Frequency 频率，单位Hz）
	//--------1MHz 1us=1000ns,1KHz 10us=10000ns
	RCC_GetClocksFreq(&RCC_ClocksStatus);	//获取时钟参数
	TIMx_Frequency = RCC_ClocksStatus.SYSCLK_Frequency;;
	if (((*(u32*)&TIMx)&APB2PERIPH_BASE) == APB2PERIPH_BASE)
  {
    TIMx_Frequency = RCC_ClocksStatus.PCLK2_Frequency;	//APB2
  }
  else
  {
    TIMx_Frequency = RCC_ClocksStatus.PCLK1_Frequency;	//APB1
  }
	//*7,计算定时器参数*********************************************************************
	//Fsys==Fpwm*Count==Fpwm*(Prescaler*Period)	
	//	TIMx_Prescaler				=	72-1		;		// 	定时器时钟分频值
	//	TIMx_Period						=	1000-1	;		// 	定时器自动重装载值
	//	Tim_num1							=	0				;		//	临时变量1
	if(1/PWM_Frequency>10)
	{
		Tim_num1=(double)(TIMx_Frequency)/(PWM_Frequency*100);							//根据定时器输出频率计算计数值
		TIMx_Prescaler=60000;
		TIMx_Period=(u16)(((double)Tim_num1/(double)TIMx_Prescaler)*200);
	}
	else if(1/PWM_Frequency>1.0)
	{
		Tim_num1=(double)(TIMx_Frequency)/(PWM_Frequency*10);							//根据定时器输出频率计算计数值
		TIMx_Prescaler=60000;
		TIMx_Period=(u16)(((double)Tim_num1/(double)TIMx_Prescaler)*20);
	}
	else if(PWM_Frequency>=1.0)
	{
		Tim_num1=(double)(TIMx_Frequency)/(PWM_Frequency);							//根据定时器输出频率计算计数值
		Tim_num1=(double)(Tim_num1*2.0);
		if(Tim_num1>=10000000)
		{
			TIMx_Prescaler=2000;
			TIMx_Period=Tim_num1/TIMx_Prescaler;
		}
		else if(Tim_num1>=1000000)
		{
			TIMx_Prescaler=200;
			TIMx_Period=Tim_num1/TIMx_Prescaler;
		}
		else if(Tim_num1>=100000)
		{
			TIMx_Prescaler=20;
			TIMx_Period=Tim_num1/TIMx_Prescaler;
		}
		else if(Tim_num1>=65530)
		{
			TIMx_Prescaler=2;
			TIMx_Period=Tim_num1/TIMx_Prescaler;
		}
		else
		{
			TIMx_Prescaler=1;
			TIMx_Period=Tim_num1/TIMx_Prescaler;
		}
	}	
	//8)**********定时器初始化
	TIM_TimeBaseStructure.TIM_Prescaler = TIMx_Prescaler-1; 				//设定分频值
	TIM_TimeBaseStructure.TIM_Period 		= TIMx_Period-1;        		//设定自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  				//不分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  		//向上计数
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);		//初始化	
	//9）定时器配置（初始化*******************************************************************
	if((TIMx_Period*(u32)PWM_Ratio)>1200)
		PWM_Ratio=(u16)((u32)(TIMx_Period*PWM_Ratio)/1200);
	else if(PWM_Ratio!=0)
		PWM_Ratio=1;
		
	TIMOCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 								//选择定时器模式:TIM脉冲宽度调制模式1
	TIMOCInitStructure.TIM_Pulse =(u16)PWM_Ratio;										//占空比=(CCRx/ARR)*1000%
	TIMOCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;				//输出极性:TIM输出比较极性高
	TIMOCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		//比较输出使能
	//10）定时器配置（初始化*******************************************************************
	switch(PWM_OUTChanneln)
	{
		case PWM_OUTChannel1:	TIM_OC1Init(TIMx, &TIMOCInitStructure);												//TIM2的CH1输出使能
		break;
		case PWM_OUTChannel2:	TIM_OC2Init(TIMx, &TIMOCInitStructure);												//TIM2的CH1输出使能
		break;
		case PWM_OUTChannel3:	TIM_OC3Init(TIMx, &TIMOCInitStructure);												//TIM2的CH1输出使能
		break;
		case PWM_OUTChannel4:	TIM_OC4Init(TIMx, &TIMOCInitStructure);												//TIM2的CH1输出使能
		break;
		default:
		break;
	}	
	TIM_CtrlPWMOutputs(TIMx,ENABLE);																//设置TIM2的PWM输出使能
	//11）定时器配置（初始化*******************************************************************
	switch(PWM_OUTChanneln)			//使能TIM在CCR上的预装载寄存器
	{
		case PWM_OUTChannel1:	TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
				break;
		case PWM_OUTChannel2:	TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);
				break;
		case PWM_OUTChannel3:	TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);
				break;
		case PWM_OUTChannel4:	TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);
				break;
	}	
	
	TIM_ARRPreloadConfig(TIMx, ENABLE);			//ARPE使能 
	
	TIM_Cmd(TIMx, ENABLE); 									//使能TIM
}

/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void SetPWM_Ratio(u16 PWM_Ratio)
{
//	TIM_OCInitTypeDef TIMOCInitStructure;
//	TIMOCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 								//选择定时器模式:TIM脉冲宽度调制模式1
//	TIMOCInitStructure.TIM_Pulse =PWM_Ratio;												//占空比=(CCRx/ARR)*1000%
//	TIMOCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;				//输出极性:TIM输出比较极性高
//	TIMOCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		//比较输出使能
//	
//	TIM_OC1Init(TIM2, &TIMOCInitStructure);													//TIM2的CH1输出使能
	
	TIM2->CCR1 = PWM_Ratio;
}























//*****************************************************以下为旧程序********************
///*******************************************************************************
//* 函数名		:	PWM_Configuration	
//* 功能描述	:	 
//* 输入		:	
//* 输出		:
//* 返回 		:
//*******************************************************************************/
//void PWM_Configuration(TIM_TypeDef* TIMx,u16 Prescaler,u16 Period,u8 PWM_RATIO)
//{
//	PWM_GPIO_Configuration();										//PWM使用的GPIO配置
////	TIM_Configuration(TIMx,Prescaler,Period);		//定时器配置
//	PWM_ClockCmd(TIMx,Prescaler,Period,PWM_RATIO);	//PWM设定	
////	TIM_ARRPreloadConfig(TIM2, ENABLE);
////	TIM_Cmd(TIM2, ENABLE); 	
//}

//void PWM_GPIO_Configuration(void)			//PWM使用的GPIO配置
//{//PA0配置
//	GPIO_InitTypeDef GPIO_InitStructure;
////	GPIO_DeInit(GPIOA);
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP ;//复用推挽输出
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);//使能GPIOA和复用时钟
//	GPIO_Init(GPIOA,&GPIO_InitStructure);
//}
///*******************************************************************************
//* 函数名		:	PWM_ClockCmd
//* 功能描述	:	PWM相关时钟使能	 
//* 输入		:	
//* 输出		:
//* 返回 		:
//*******************************************************************************/
//void PWM_ClockCmd(TIM_TypeDef* TIMx,u16 Prescaler,u16 Period,u8 PWM_RATIO)		//开启相关GPIO时钟	
//{
//	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;	//定时器结构体定义
//	TIM_OCInitTypeDef TIMOCInitStructure;
////	NVIC_InitTypeDef	NVIC_InitStructure;						//中断结构体
////	u8 TIM_IRQChannel=0;
//	//1)**********定义相关变量	
//		switch (*(u32*)&TIMx)
//	{
//		case TIM1_BASE:
//			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
////			TIM_IRQChannel=TIM1_UP_IRQChannel;
//			break;
//		
//		case TIM2_BASE:
//			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
////			TIM_IRQChannel=TIM2_IRQChannel;
//			break;
//		
//		case TIM3_BASE:
//			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
////			TIM_IRQChannel=TIM3_IRQChannel;
//			break;
//		
//		case TIM4_BASE:
//			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
////			TIM_IRQChannel=TIM4_IRQChannel;
//			break;
//		
//		case TIM5_BASE:
//			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
////			TIM_IRQChannel=TIM5_IRQChannel;
//			break;
//		
//		case TIM6_BASE:
//			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
////			TIM_IRQChannel=TIM6_IRQChannel;
//			break;
//		
//		case TIM7_BASE:
//			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
////			TIM_IRQChannel=TIM7_IRQChannel;
//			break;
//		
//		case TIM8_BASE:
//			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
////			TIM_IRQChannel=TIM8_UP_IRQChannel;
//			break;
//		
//		default:
//			break;
//		
//	}
//	
//	TIMx_RCC_ENABLE(TIMx);													//打开相应定时器时钟
//	//1)**********定义相关变量	
//	TIM_TimeBaseStructure.TIM_Prescaler = Prescaler-1; 		// 设定分频值
//	TIM_TimeBaseStructure.TIM_Period = Period-1;        	//设定自动重装载值
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //不分割
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数
//	//	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	//????,???????????????????-??????????
//	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);		//初始化
//	//1)**********定义相关变量	
//	TIM_ARRPreloadConfig(TIMx, ENABLE);
//	TIM_Cmd(TIMx, ENABLE); 
//	
//	//1)**********定义相关变量	
////	NVIC_InitStructure.NVIC_IRQChannel = TIM_IRQChannel;
////	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
////	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
////	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
////	NVIC_Init(&NVIC_InitStructure);
//	
//	/* Clear TIMx update pending flag[清除TIMx溢出中断] */
//	TIM_ClearFlag(TIMx, TIM_FLAG_Update);

//	/* Enable TIM2 Update interrupt [TIMx溢出中断允许]*/
////	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE); 

//	/* TIM2 enable counter [使能TIMx计数]*/
//	TIM_Cmd(TIMx, ENABLE);	//使能TIMx计数
//	
//	
//	TIMOCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 							//PWM输出模式1
////	TIMOCInitStructure.TIM_Pulse =5000;													//占空比=(CCRx/ARR)*100%
//	TIMOCInitStructure.TIM_Pulse =(TIMx->ARR)*PWM_RATIO/100;			//占空比=(CCRx/ARR)*100%
//	TIMOCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;			//TIM输出极性高
//	TIMOCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//使能输出
//	TIM_OC1Init(TIMx, &TIMOCInitStructure);												//TIM2的CH1输出使能
//	TIM_CtrlPWMOutputs(TIMx,ENABLE);															//设置TIM2的PWM输出使能
//	
//	TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
//	TIM_ARRPreloadConfig(TIMx, ENABLE);
//	TIM_Cmd(TIMx, ENABLE); 
//}
///*******************************************************************************
//* 函数名		:	PWM_SET	
//* 功能描述	:		 
//* 输入		:	
//* 输出		:
//* 返回 		:
//*******************************************************************************/
//void PWM_SET(TIM_TypeDef* TIMx,u8 PWM_RATIO)	//PWM设定
//{  
//	TIM_OCInitTypeDef TIMOCInitStructure;
//	TIMOCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 							//PWM输出模式1
////	TIMOCInitStructure.TIM_Pulse =5000;													//占空比=(CCRx/ARR)*100%
//	TIMOCInitStructure.TIM_Pulse =(TIMx->ARR)/100*PWM_RATIO;			//占空比=(CCRx/ARR)*100%
//	TIMOCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;			//TIM输出极性高
//	TIMOCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//使能输出
//	TIM_OC1Init(TIMx, &TIMOCInitStructure);												//TIM2的CH1输出使能
//	TIM_CtrlPWMOutputs(TIMx,ENABLE);															//设置TIM2的PWM输出使能
//	
//	TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
//	TIM_ARRPreloadConfig(TIMx, ENABLE);
//	TIM_Cmd(TIMx, ENABLE); 																				//开启定时器
//}
//void PWM_Configuration0(TIM_TypeDef* TIMx,u16 Prescaler,u16 Period,u8 PWM_RATIO)
//{
//	PWM_GPIO_Configuration();					//PWM使用的GPIO配置
//	TIM_Configuration(TIMx,Prescaler,Period);	//定时器配置
//	PWM_SET(TIMx,PWM_RATIO);	//PWM设定	
////	TIM_ARRPreloadConfig(TIM2, ENABLE);
////	TIM_Cmd(TIM2, ENABLE); 	
//}










