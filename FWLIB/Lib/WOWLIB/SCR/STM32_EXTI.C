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


#include "STM32_EXTI.H"

#include "STM32_WOW.H"
//#include "STM32F10x_BitBand.H"

//EXTI结构体
//typedef struct
//{
//  u32 EXTI_Line;
//  EXTIMode_TypeDef EXTI_Mode;
//  EXTITrigger_TypeDef EXTI_Trigger;
//  FunctionalState EXTI_LineCmd;
//}EXTI_InitTypeDef;


//u8 a=0;
/*******************************************************************************
* 函数名		:	EXTI_Configuration	
* 功能描述	:	外部中断配置 
* 输入		:	GPIOx：x为A...G
						GPIO_Pin为GPIO_Pin_x：x取值为0...15
						Mode：中断模式，取值为 EXTI_Mode_Interrupt：触发中断
																	EXTI_Mode_Event：事件中断
* 输出		:
* 返回 		:
*******************************************************************************/
void EXTI_Configuration(GPIO_TypeDef* GPIOx, u16 GPIO_Pin_x,EXTIMode_TypeDef Mode)
{
	//1)**********定义相关变量	
//	GPIO_InitTypeDef GPIO_InitStructure;				//GPIO结构体
	u8 GPIO_PortSource=0;
	u8 GPIO_PinSource=0;
	u8 EXTI_IRQChannel=0;
//	u32 EXTI_Line=(u32)GPIO_Pin_x;
	
	//2)**********检查参数
	/* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GPIO_PIN(GPIO_Pin_x));
	//A---------端口选择及打开相关GPIO时钟
	EXTI_ClockConf(GPIOx,GPIO_Pin_x);				//开启相关GPIO时钟	
	
	//---------引脚选择---求指数
	//	GPIO_PinSource=(u8)log((double)GPIO_Pin)/log((double)2);		//换底公式求对数	
	GPIO_PinSource=(u8)(log(GPIO_Pin_x)/log(2));		//换底公式求对数		
	
	//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource3);
	GPIO_EXTILineConfig(GPIO_PortSource,GPIO_PinSource);
	
	
	EXTI_PortSourceConf(GPIOx,&GPIO_PortSource);									//设置中断线
	EXTI_PinConf(GPIOx,GPIO_Pin_x,&EXTI_IRQChannel);							//选择相关中断线路
	EXTI_InitStructure(GPIOx,GPIO_Pin_x,Mode,EXTI_IRQChannel);		//外部中断初始化
	//3)**********选择GPIO管脚用作外部中断线路,打开相关GPIO时钟
	
}
/*******************************************************************************
* 函数名		:	EXTI_Configuration	
* 功能描述	:	外部中断配置 
* 输入		:	GPIOx：x为A...G
						GPIO_Pin为GPIO_Pin_x：x取值为0...15
						Mode：中断模式，取值为 EXTI_Mode_Interrupt：触发中断
																	EXTI_Mode_Event：事件中断
* 输出		:
* 返回 		:
*******************************************************************************/
void EXTI_Configuration2(GPIO_TypeDef* GPIOx, u16 GPIO_Pin_x,EXTIMode_TypeDef Mode)
{
	//1)**********定义相关变量	
//	GPIO_InitTypeDef GPIO_InitStructure;				//GPIO结构体
	u8 GPIO_PortSource=0;
	u8 GPIO_PinSource=0;
	u8 EXTI_IRQChannel=0;
//	u32 EXTI_Line=(u32)GPIO_Pin_x;
	
	//2)**********检查参数
	/* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GPIO_PIN(GPIO_Pin_x));
	//A---------端口选择及打开相关GPIO时钟
	EXTI_ClockConf(GPIOx,GPIO_Pin_x);				//开启相关GPIO时钟	
	
	//---------引脚选择---求指数
	//	GPIO_PinSource=(u8)log((double)GPIO_Pin)/log((double)2);		//换底公式求对数	
	GPIO_PinSource=(u8)(log(GPIO_Pin_x)/log(2));		//换底公式求对数		
	
	//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource3);
	GPIO_EXTILineConfig(GPIO_PortSource,GPIO_PinSource);
	
	
	EXTI_PortSourceConf(GPIOx,&GPIO_PortSource);									//设置中断线
	EXTI_PinConf(GPIOx,GPIO_Pin_x,&EXTI_IRQChannel);							//选择相关中断线路
	EXTI_InitStructure2(GPIOx,GPIO_Pin_x,Mode,EXTI_IRQChannel);		//外部中断初始化
	//3)**********选择GPIO管脚用作外部中断线路,打开相关GPIO时钟
	
}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void EXTI_ClockConf(GPIO_TypeDef* GPIOx, u16 GPIO_Pin_x)				//开启相关GPIO时钟	
{
	switch (*(u32*)&GPIOx)
  {
    case GPIOA_BASE:
//			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		if((GPIO_Pin_x==GPIO_Pin_13)||(GPIO_Pin_x==GPIO_Pin_14)||(GPIO_Pin_x==GPIO_Pin_All))
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
			GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);		//关闭JTAG
		}
		else
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
      break;

    case GPIOB_BASE:
		if((GPIO_Pin_x==GPIO_Pin_3)||(GPIO_Pin_x==GPIO_Pin_4)||(GPIO_Pin_x==GPIO_Pin_All))
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
			GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		//关闭JTAG
		}
		else
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
      break;

    case GPIOC_BASE:
			if((GPIO_Pin_x==GPIO_Pin_14)||(GPIO_Pin_x==GPIO_Pin_15))
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_AFIO, ENABLE);
			else
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
      break;

    case GPIOD_BASE:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
      break;
      
    case GPIOE_BASE:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
      break; 

    case GPIOF_BASE:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
      break;

    case GPIOG_BASE:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
      break;                       

    default:
      break;
  }

}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void EXTI_PortSourceConf(GPIO_TypeDef* GPIOx,u8 *GPIO_PortSource)	//设置中断线
{
//	u8 GPIO_PortSource=0;
	
//	GPIO_PortSourceP=&GPIO_PortSource;
	
	switch (*(u32*)&GPIOx)
  {
    case GPIOA_BASE:
      *GPIO_PortSource=GPIO_PortSourceGPIOA;
      break;

    case GPIOB_BASE:
      *GPIO_PortSource=GPIO_PortSourceGPIOB;
      break;

    case GPIOC_BASE:
      *GPIO_PortSource=GPIO_PortSourceGPIOC;
      break;

    case GPIOD_BASE:
      *GPIO_PortSource=GPIO_PortSourceGPIOD;
      break;
      
    case GPIOE_BASE:
      *GPIO_PortSource=GPIO_PortSourceGPIOE;
      break; 

    case GPIOF_BASE:
      *GPIO_PortSource=GPIO_PortSourceGPIOF;
      break;

    case GPIOG_BASE:
      *GPIO_PortSource=GPIO_PortSourceGPIOG;
      break;                       

    default:
      break;
  }

}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void EXTI_PinConf(GPIO_TypeDef* GPIOx, u16 GPIO_Pin_x,u8 *EXTI_IRQChannel)			//选择相关中断线路
{
	
	
	if(GPIO_Pin_x<0x0020)
	{
		switch(GPIO_Pin_x)
		{
			case GPIO_Pin_0:
				*EXTI_IRQChannel=EXTI0_IRQChannel;
			break;
			case GPIO_Pin_1:
				*EXTI_IRQChannel=EXTI1_IRQChannel;
			break;
			case GPIO_Pin_2:
				*EXTI_IRQChannel=EXTI2_IRQChannel;
			break;
			case GPIO_Pin_3:
				*EXTI_IRQChannel=EXTI3_IRQChannel;			
				if((*(u32*)&GPIOx)==GPIOB_BASE)
				{
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	
					GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		//关闭JTAG
				}
			break;
			case GPIO_Pin_4:
				*EXTI_IRQChannel=EXTI4_IRQChannel;
				if((*(u32*)&GPIOx)==GPIOB_BASE)
				{
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	
					GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		//关闭JTAG
				}
			break;
				
			default:
				break;
		}
	}
	else if(GPIO_Pin_x>0x0010&&GPIO_Pin_x<0x0400)
	{
		*EXTI_IRQChannel=EXTI9_5_IRQChannel;
	}
	else
	{
		*EXTI_IRQChannel=EXTI15_10_IRQChannel;
		if((GPIO_Pin_13||GPIO_Pin_14||GPIO_Pin_15)&&((*(u32*)&GPIOx)==GPIOA_BASE))
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	
			GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		//关闭JTAG
		}
	}

}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void EXTI_InitStructure(GPIO_TypeDef* GPIOx, u16 GPIO_Pin_x,EXTIMode_TypeDef Mode,u8 EXTI_IRQChannel)		//外部中断初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;				//GPIO结构体
	EXTI_InitTypeDef EXTI_Initstructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	u32 EXTI_Line=(u32)GPIO_Pin_x;
	
	//4)**********GPIO配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOx,&GPIO_InitStructure);
	
	EXTI_Initstructure.EXTI_Line=EXTI_Line;												//外部中断线路
	EXTI_Initstructure.EXTI_Mode=Mode;														//中断模式
//	EXTI_Initstructure.EXTI_Mode=EXTI_Mode_Interrupt;						//中断模式
	EXTI_Initstructure.EXTI_Trigger=EXTI_Trigger_Falling;					//触发方式-
	EXTI_Initstructure.EXTI_LineCmd=ENABLE;												//连接使能
	EXTI_Init(&EXTI_Initstructure);
	
	EXTI_GenerateSWInterrupt(EXTI_Line);													//使能中断
	
	//7)**********配置中断
	NVIC_InitStructure.NVIC_IRQChannel = EXTI_IRQChannel;					//选择中断通道-中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;			//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;						//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能
	NVIC_Init(&NVIC_InitStructure);	

	EXTI_ClearITPendingBit(EXTI_Line);

}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void EXTI_InitStructure2(GPIO_TypeDef* GPIOx, u16 GPIO_Pin_x,EXTIMode_TypeDef Mode,u8 EXTI_IRQChannel)		//外部中断初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;				//GPIO结构体
	EXTI_InitTypeDef EXTI_Initstructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	u32 EXTI_Line=(u32)GPIO_Pin_x;
	
	//4)**********GPIO配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOx,&GPIO_InitStructure);
	
	EXTI_Initstructure.EXTI_Line=EXTI_Line;												//外部中断线路
	EXTI_Initstructure.EXTI_Mode=Mode;														//中断模式
//	EXTI_Initstructure.EXTI_Mode=EXTI_Mode_Interrupt;						//中断模式
	EXTI_Initstructure.EXTI_Trigger=EXTI_Trigger_Falling;					//触发方式-
	EXTI_Initstructure.EXTI_LineCmd=ENABLE;												//连接使能
	EXTI_Init(&EXTI_Initstructure);
	
	EXTI_GenerateSWInterrupt(EXTI_Line);													//使能中断
	
	//7)**********配置中断
	NVIC_InitStructure.NVIC_IRQChannel = EXTI_IRQChannel;					//选择中断通道-中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;			//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;						//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能
	NVIC_Init(&NVIC_InitStructure);	

	EXTI_ClearITPendingBit(EXTI_Line);

}

/*******************************************************************************
* 函数名		:	
* 功能描述	:	外部中断 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void EXTI_Interrupt_Configuration(u32 EXTI_Line)
{
	EXTI_InitTypeDef EXTI_Initstructure;			
	
	EXTI_Initstructure.EXTI_Line=EXTI_Line5;								//外部中断线路
	EXTI_Initstructure.EXTI_Mode=EXTI_Mode_Interrupt;				//中断模式
	EXTI_Initstructure.EXTI_Trigger=EXTI_Trigger_Falling;		//触发方式-
	EXTI_Initstructure.EXTI_LineCmd=ENABLE;									//连接使能
	EXTI_Init(&EXTI_Initstructure);
	EXTI_GenerateSWInterrupt(EXTI_Line5);										//使能中断

}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	事件中断 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void EXTI_Event_Configuration(void)
{

}

/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void EXTI_Server(void)
{
	WOW_Server();				//服务函数
	EXTI_ClearITPendingBit(EXTI_Line0);
	EXTI_ClearITPendingBit(EXTI_Line1);
	EXTI_ClearITPendingBit(EXTI_Line2);
	EXTI_ClearITPendingBit(EXTI_Line3);
	EXTI_ClearITPendingBit(EXTI_Line4);
	EXTI_ClearITPendingBit(EXTI_Line5);
	EXTI_ClearITPendingBit(EXTI_Line6);
	EXTI_ClearITPendingBit(EXTI_Line7);
	EXTI_ClearITPendingBit(EXTI_Line8);
	EXTI_ClearITPendingBit(EXTI_Line9);
	EXTI_ClearITPendingBit(EXTI_Line10);
	EXTI_ClearITPendingBit(EXTI_Line11);
	EXTI_ClearITPendingBit(EXTI_Line12);
	EXTI_ClearITPendingBit(EXTI_Line13);
	EXTI_ClearITPendingBit(EXTI_Line14);
	EXTI_ClearITPendingBit(EXTI_Line15);
}


