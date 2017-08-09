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


#include "STM32_GPIO.H"

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_type.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_map.h"

#include "STM32F10x_BitBand.H"
//#include "stm32f10x_gpio.h"
//#include "STM32F10x_BitBand.H"


/*******************************************************************************
* 函数名		:	GPIO_ClockCmd	
* 功能描述	:	开启相关GPIO端口时钟	 
* 输入		:	GPIOx：GPIOA~GPIOG
						GPIO_Pin_x:GPIO_Pin_0~GPIO_Pin_15;GPIO_Pin_All---部分IO需要重定向接口
* 输出		:
* 返回 		:
*******************************************************************************/
void GPIO_ClockConf(GPIO_TypeDef* GPIOx,						//GPIO端口			
										u16 GPIO_Pin_x									//GPIO引脚
										)		//开启相关GPIO时钟	 
{
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx)); 
	switch (*(u32*)&GPIOx)
	{
		//********************GPIOA时钟使能********************	
		case GPIOA_BASE:
//			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			if((GPIO_Pin_x==GPIO_Pin_13)||(GPIO_Pin_x==GPIO_Pin_14)||(GPIO_Pin_x==GPIO_Pin_15)||(GPIO_Pin_x==GPIO_Pin_All))
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
				//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);			//关闭SW功能
				GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		//关闭JTAG,SW功能开启
			}
			else
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			break;
		//********************GPIOB时钟使能********************
		case GPIOB_BASE:
			if((GPIO_Pin_x&&GPIO_Pin_3==GPIO_Pin_3)||(GPIO_Pin_x&&GPIO_Pin_4==GPIO_Pin_4)||(GPIO_Pin_x==GPIO_Pin_All))
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
				GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);				//关闭JTAG
			}
			else
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
			break;
		//********************GPIOC时钟使能********************
		case GPIOC_BASE:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
			if((GPIO_Pin_x==GPIO_Pin_14)||(GPIO_Pin_x==GPIO_Pin_15))
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_AFIO, ENABLE);
			break;
		//********************GPIOD时钟使能********************
		case GPIOD_BASE:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
			break;
		//********************GPIOE时钟使能********************
		case GPIOE_BASE:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
			break;
		//********************GPIOF时钟使能********************
		case GPIOF_BASE:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
			break;
		//********************GPIOG时钟使能********************
		case GPIOG_BASE:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
			break;
		
		default: break;
		
	}

}

/*******************************************************************************
* 函数名		:	GPIO_DeInitAll
* 功能描述	:	将所有的GPIO关闭----V20170605
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void GPIO_DeInitAll(void)
{
	//1）GPIO结构体定义
	GPIO_InitTypeDef	GPIO_InitStructure;
	//2）初始化GPIO结构体
	GPIO_StructInit(&GPIO_InitStructure);			//GPIO_Pin_All，GPIO_Speed_2MHz，GPIO_Mode_IN_FLOATING
	//3）初始化GPIO为默认状态
	GPIO_Init(GPIOA,&GPIO_InitStructure);			//GPIOA
	GPIO_Init(GPIOB,&GPIO_InitStructure);			//GPIOB
	GPIO_Init(GPIOC,&GPIO_InitStructure);			//GPIOC
	GPIO_Init(GPIOD,&GPIO_InitStructure);			//GPIOD
	GPIO_Init(GPIOE,&GPIO_InitStructure);			//GPIOE
	GPIO_Init(GPIOF,&GPIO_InitStructure);			//GPIOF
	GPIO_Init(GPIOG,&GPIO_InitStructure);			//GPIOG
	//4）关闭GPIO时钟
	GPIO_DeInit(GPIOA);												//GPIOA
	GPIO_DeInit(GPIOB);												//GPIOB
	GPIO_DeInit(GPIOC);												//GPIOC
	GPIO_DeInit(GPIOD);												//GPIOD
	GPIO_DeInit(GPIOE);												//GPIOE
	GPIO_DeInit(GPIOF);												//GPIOF
	GPIO_DeInit(GPIOG);												//GPIOG
	//4）关闭AFIO
	GPIO_AFIODeInit();												//AFIO关闭	
}
/*******************************************************************************
* 函数名		:	GPIO_Configuration
* 功能描述	:	GPIO配置 
* 输入		:	GPIOx：GPIOA~GPIOG
						GPIO_Pin_x:GPIO_Pin_0~GPIO_Pin_15;GPIO_Pin_All
* 输出		:
* 返回 		:
*******************************************************************************/
void GPIO_Configuration(GPIO_TypeDef* GPIOx,					//GPIO端口
												u16 GPIO_Pin_x,								//GPIO引脚
												GPIOMode_TypeDef GPIO_Mode,		//工作模式
												GPIOSpeed_TypeDef GPIO_Speed	//工作速度
												)			//GPIO配置	
{	
	switch(GPIO_Mode)
	{
		//输入模式
		case GPIO_Mode_AIN:					GPIO_Configuration_INA(GPIOx,GPIO_Pin_x);			//将GPIO相应管脚配置为模拟输入模式----V20170605
					break;
		case GPIO_Mode_IN_FLOATING:	GPIO_Configuration_INF(GPIOx,GPIO_Pin_x);			//将GPIO相应管脚配置为模拟输入模式----V20170605
					break;
		case GPIO_Mode_IPD:					GPIO_Configuration_IPD(GPIOx,GPIO_Pin_x);			//将GPIO相应管脚配置为模拟输入模式----V20170605
					break;
		case GPIO_Mode_IPU:					GPIO_Configuration_IPU(GPIOx,GPIO_Pin_x);			//将GPIO相应管脚配置为模拟输入模式----V20170605
					break;
		//输出模式
		case GPIO_Mode_Out_OD:	
					switch(GPIO_Speed)
					{
						case GPIO_Speed_10MHz:	GPIO_Configuration_OOD10	(GPIOx,GPIO_Pin_x);			//将GPIO相应管脚配置为OD(开漏)输出模式，最大速度2MHz----V20170605
									break;
						case GPIO_Speed_2MHz: 	GPIO_Configuration_OOD2		(GPIOx,GPIO_Pin_x);			//将GPIO相应管脚配置为OD(开漏)输出模式，最大速度2MHz----V20170605
									break;
						case GPIO_Speed_50MHz: 	GPIO_Configuration_OOD50	(GPIOx,GPIO_Pin_x);			//将GPIO相应管脚配置为OD(开漏)输出模式，最大速度2MHz----V20170605
									break;
						default : break;
					}
					break;
		case GPIO_Mode_Out_PP:
					switch(GPIO_Speed)
					{
						case GPIO_Speed_10MHz: 	GPIO_Configuration_OPP10	(GPIOx,GPIO_Pin_x);			//将GPIO相应管脚配置为OD(开漏)输出模式，最大速度2MHz----V20170605
									break;
						case GPIO_Speed_2MHz: 	GPIO_Configuration_OPP2		(GPIOx,GPIO_Pin_x);			//将GPIO相应管脚配置为OD(开漏)输出模式，最大速度2MHz----V20170605
									break;
						case GPIO_Speed_50MHz: 	GPIO_Configuration_OPP50	(GPIOx,GPIO_Pin_x);			//将GPIO相应管脚配置为OD(开漏)输出模式，最大速度2MHz----V20170605
									break;
						default : break;
					}
					break;
		case GPIO_Mode_AF_OD:
					switch(GPIO_Speed)
					{
						case GPIO_Speed_10MHz: 	GPIO_Configuration_AOD10	(GPIOx,GPIO_Pin_x);			//将GPIO相应管脚配置为OD(开漏)输出模式，最大速度2MHz----V20170605
									break;
						case GPIO_Speed_2MHz: 	GPIO_Configuration_AOD2		(GPIOx,GPIO_Pin_x);			//将GPIO相应管脚配置为OD(开漏)输出模式，最大速度2MHz----V20170605
									break;
						case GPIO_Speed_50MHz: 	GPIO_Configuration_AOD50	(GPIOx,GPIO_Pin_x);			//将GPIO相应管脚配置为OD(开漏)输出模式，最大速度2MHz----V20170605
									break;
						default : break;
					}
					break;
		case GPIO_Mode_AF_PP:
					switch(GPIO_Speed)
					{
						case GPIO_Speed_10MHz: 	GPIO_Configuration_APP10	(GPIOx,GPIO_Pin_x);			//将GPIO相应管脚配置为OD(开漏)输出模式，最大速度2MHz----V20170605
									break;
						case GPIO_Speed_2MHz: 	GPIO_Configuration_APP2		(GPIOx,GPIO_Pin_x);			//将GPIO相应管脚配置为OD(开漏)输出模式，最大速度2MHz----V20170605
									break;
						case GPIO_Speed_50MHz: 	GPIO_Configuration_APP50	(GPIOx,GPIO_Pin_x);			//将GPIO相应管脚配置为OD(开漏)输出模式，最大速度2MHz----V20170605
									break;
						default : break;
					}
					break;		
		default :	break;
	}
}
/*******************************************************************************
* 函数名		:	GPIO_InitStructure_INA
* 功能描述	:	将GPIO相应管脚配置为模拟输入模式----V20170605
* 输入		:	GPIOx--GPIO端口，GPIO_Pin_n--GPIO管脚号
* 输出		:
* 返回 		:
*******************************************************************************/
void GPIO_Configuration_INA(
														GPIO_TypeDef* GPIOx,		//GPIO端口,x=A/B/C/D/E/F/G
														u16 GPIO_Pin_n					//GPIO管脚号n=0~15/All
														)
{
	//1）GPIO结构体定义
	GPIO_InitTypeDef	GPIO_InitStructure;
	//2）初始化GPIO结构体
  GPIO_InitStructure.GPIO_Pin  		= GPIO_Pin_n;
  GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;		//输入模式下时钟配置无效
  GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AIN;			//模拟输入模式
	//3)根据GPIO端口及管脚号开启端口时钟及确定是否打开AFIO时钟
	GPIO_ClockConf(GPIOx,GPIO_Pin_n);
	//4）初始化GPIO
	GPIO_Init(GPIOx,&GPIO_InitStructure);			//GPIOA
}
/*******************************************************************************
* 函数名		:	GPIO_InitStructure_INF
* 功能描述	:	将GPIO相应管脚配置为浮空输入模式----V20170605
* 输入		:	GPIOx--GPIO端口，GPIO_Pin_n--GPIO管脚号
* 输出		:
* 返回 		:
*******************************************************************************/
void GPIO_Configuration_INF(
														GPIO_TypeDef* GPIOx,							//GPIO端口,x=A/B/C/D/E/F/G
														u16 GPIO_Pin_n										//GPIO管脚号n=0~15/All
														)
{
	//1）GPIO结构体定义
	GPIO_InitTypeDef	GPIO_InitStructure;
	//2）初始化GPIO结构体
	GPIO_InitStructure.GPIO_Pin  		= GPIO_Pin_n;
  GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;					//输入模式下时钟配置无效
  GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_IN_FLOATING;		//浮空输入模式
	//3)根据GPIO端口及管脚号开启端口时钟及确定是否打开AFIO时钟
	GPIO_ClockConf(GPIOx,GPIO_Pin_n);
	//3）初始化GPIO
	GPIO_Init(GPIOx,&GPIO_InitStructure);			//GPIOA	
}
/*******************************************************************************
* 函数名		:	GPIO_Configuration_IPD
* 功能描述	:	将GPIO相应管脚配置为下拉输入模式----V20170605
* 输入		:	GPIOx--GPIO端口，GPIO_Pin_n--GPIO管脚号
* 输出		:
* 返回 		:
*******************************************************************************/
void GPIO_Configuration_IPD(
														GPIO_TypeDef* GPIOx,							//GPIO端口,x=A/B/C/D/E/F/G
														u16 GPIO_Pin_n										//GPIO管脚号n=0~15/All
														)
{
	//1）GPIO结构体定义
	GPIO_InitTypeDef	GPIO_InitStructure;
	//2）初始化GPIO结构体
	GPIO_InitStructure.GPIO_Pin  		= GPIO_Pin_n;
  GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;					//输入模式下时钟配置无效
  GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_IPD;						//下拉输入模式
	//3)根据GPIO端口及管脚号开启端口时钟及确定是否打开AFIO时钟
	GPIO_ClockConf(GPIOx,GPIO_Pin_n);
	//3）初始化GPIO
	GPIO_Init(GPIOx,&GPIO_InitStructure);			//GPIOA	
}
/*******************************************************************************
* 函数名		:	GPIO_Configuration_IPU
* 功能描述	:	将GPIO相应管脚配置为上拉输入模式----V20170605
* 输入		:	GPIOx--GPIO端口，GPIO_Pin_n--GPIO管脚号
* 输出		:
* 返回 		:
*******************************************************************************/
void GPIO_Configuration_IPU(
														GPIO_TypeDef* GPIOx,							//GPIO端口,x=A/B/C/D/E/F/G
														u16 GPIO_Pin_n										//GPIO管脚号n=0~15/All
														)
{
	//1）GPIO结构体定义
	GPIO_InitTypeDef	GPIO_InitStructure;
	//2）初始化GPIO结构体
	GPIO_InitStructure.GPIO_Pin  		= GPIO_Pin_n;
  GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;					//输入模式下时钟配置无效
  GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_IPU;						//上拉输入模式
	//3)根据GPIO端口及管脚号开启端口时钟及确定是否打开AFIO时钟
	GPIO_ClockConf(GPIOx,GPIO_Pin_n);
	//3）初始化GPIO
	GPIO_Init(GPIOx,&GPIO_InitStructure);			//GPIOA	
}
/*******************************************************************************
* 函数名		:	GPIO_Configuration_OOD2
* 功能描述	:	将GPIO相应管脚配置为OD(开漏)输出模式，最大速度2MHz----V20170605
* 输入		:	GPIOx--GPIO端口，GPIO_Pin_n--GPIO管脚号
* 输出		:
* 返回 		:
*******************************************************************************/
void GPIO_Configuration_OOD2(
														GPIO_TypeDef* GPIOx,							//GPIO端口,x=A/B/C/D/E/F/G
														u16 GPIO_Pin_n										//GPIO管脚号n=0~15/All
														)
{
	//1）GPIO结构体定义
	GPIO_InitTypeDef	GPIO_InitStructure;
	//2）初始化GPIO结构体
	GPIO_InitStructure.GPIO_Pin  		= GPIO_Pin_n;
  GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;					//最大速度频率2MHz
  GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_Out_OD;					//OD(开漏)输出模式
	//3)根据GPIO端口及管脚号开启端口时钟及确定是否打开AFIO时钟
	GPIO_ClockConf(GPIOx,GPIO_Pin_n);
	//3）初始化GPIO
	GPIO_Init(GPIOx,&GPIO_InitStructure);			//GPIOA	
}
/*******************************************************************************
* 函数名		:	GPIO_Configuration_OOD2
* 功能描述	:	将GPIO相应管脚配置为OD(开漏)输出模式，最大速度10MHz----V20170605
* 输入		:	GPIOx--GPIO端口，GPIO_Pin_n--GPIO管脚号
* 输出		:
* 返回 		:
*******************************************************************************/
void GPIO_Configuration_OOD10(
														GPIO_TypeDef* GPIOx,							//GPIO端口,x=A/B/C/D/E/F/G
														u16 GPIO_Pin_n										//GPIO管脚号n=0~15/All
														)
{
	//1）GPIO结构体定义
	GPIO_InitTypeDef	GPIO_InitStructure;
	//2）初始化GPIO结构体
	GPIO_InitStructure.GPIO_Pin  		= GPIO_Pin_n;
  GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_10MHz;					//最大速度频率10MHz
  GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_Out_OD;					//OD(开漏)输出模式
	//3)根据GPIO端口及管脚号开启端口时钟及确定是否打开AFIO时钟
	GPIO_ClockConf(GPIOx,GPIO_Pin_n);
	//3）初始化GPIO
	GPIO_Init(GPIOx,&GPIO_InitStructure);			//GPIOA	
}
/*******************************************************************************
* 函数名		:	GPIO_Configuration_OOD50
* 功能描述	:	将GPIO相应管脚配置为OD(开漏)输出模式，最大速度50MHz----V20170605
* 输入		:	GPIOx--GPIO端口，GPIO_Pin_n--GPIO管脚号
* 输出		:
* 返回 		:
*******************************************************************************/
void GPIO_Configuration_OOD50(
														GPIO_TypeDef* GPIOx,							//GPIO端口,x=A/B/C/D/E/F/G
														u16 GPIO_Pin_n										//GPIO管脚号n=0~15/All
														)
{
	//1）GPIO结构体定义
	GPIO_InitTypeDef	GPIO_InitStructure;
	//2）初始化GPIO结构体
	GPIO_InitStructure.GPIO_Pin  		= GPIO_Pin_n;
  GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;					//最大速度频率50MHz
  GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_Out_OD;					//OD(开漏)输出模式
	//3)根据GPIO端口及管脚号开启端口时钟及确定是否打开AFIO时钟
	GPIO_ClockConf(GPIOx,GPIO_Pin_n);
	//3）初始化GPIO
	GPIO_Init(GPIOx,&GPIO_InitStructure);			//GPIOA	
}
/*******************************************************************************
* 函数名		:	GPIO_Configuration_OPP2
* 功能描述	:	将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
* 输入		:	GPIOx--GPIO端口，GPIO_Pin_n--GPIO管脚号
* 输出		:
* 返回 		:
*******************************************************************************/
void GPIO_Configuration_OPP2(
														GPIO_TypeDef* GPIOx,							//GPIO端口,x=A/B/C/D/E/F/G
														u16 GPIO_Pin_n										//GPIO管脚号n=0~15/All
														)
{
	//1）GPIO结构体定义
	GPIO_InitTypeDef	GPIO_InitStructure;
	//2）初始化GPIO结构体
	GPIO_InitStructure.GPIO_Pin  		= GPIO_Pin_n;
  GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;					//最大速度频率2MHz
  GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_Out_PP;					//PP(推挽)输出模式
	//3)根据GPIO端口及管脚号开启端口时钟及确定是否打开AFIO时钟
	GPIO_ClockConf(GPIOx,GPIO_Pin_n);
	//3）初始化GPIO
	GPIO_Init(GPIOx,&GPIO_InitStructure);			//GPIOA	
}
/*******************************************************************************
* 函数名		:	GPIO_Configuration_OPP10
* 功能描述	:	将GPIO相应管脚配置为PP(推挽)输出模式，最大速度10MHz----V20170605
* 输入		:	GPIOx--GPIO端口，GPIO_Pin_n--GPIO管脚号
* 输出		:
* 返回 		:
*******************************************************************************/
void GPIO_Configuration_OPP10(
														GPIO_TypeDef* GPIOx,							//GPIO端口,x=A/B/C/D/E/F/G
														u16 GPIO_Pin_n										//GPIO管脚号n=0~15/All
														)
{
	//1）GPIO结构体定义
	GPIO_InitTypeDef	GPIO_InitStructure;
	//2）初始化GPIO结构体
	GPIO_InitStructure.GPIO_Pin  		= GPIO_Pin_n;
  GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_10MHz;					//最大速度频率10MHz
  GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_Out_PP;					//PP(推挽)输出模式
	//3)根据GPIO端口及管脚号开启端口时钟及确定是否打开AFIO时钟
	GPIO_ClockConf(GPIOx,GPIO_Pin_n);
	//3）初始化GPIO
	GPIO_Init(GPIOx,&GPIO_InitStructure);			//GPIOA	
}
/*******************************************************************************
* 函数名		:	GPIO_Configuration_OPP50
* 功能描述	:	将GPIO相应管脚配置为PP(推挽)输出模式，最大速度10MHz----V20170605
* 输入		:	GPIOx--GPIO端口，GPIO_Pin_n--GPIO管脚号
* 输出		:
* 返回 		:
*******************************************************************************/
void GPIO_Configuration_OPP50(
														GPIO_TypeDef* GPIOx,							//GPIO端口,x=A/B/C/D/E/F/G
														u16 GPIO_Pin_n										//GPIO管脚号n=0~15/All
														)
{
	//1）GPIO结构体定义
	GPIO_InitTypeDef	GPIO_InitStructure;
	//2）初始化GPIO结构体
	GPIO_InitStructure.GPIO_Pin  		= GPIO_Pin_n;
  GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;					//最大速度频率50MHz
  GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_Out_PP;					//PP(推挽)输出模式
	//3)根据GPIO端口及管脚号开启端口时钟及确定是否打开AFIO时钟
	GPIO_ClockConf(GPIOx,GPIO_Pin_n);
	//3）初始化GPIO
	GPIO_Init(GPIOx,&GPIO_InitStructure);			//GPIOA	
}
/*******************************************************************************
* 函数名		:	GPIO_Configuration_AOD2
* 功能描述	:	将GPIO相应管脚配置为AOD(复用开漏)输出模式，最大速度2MHz----V20170605
* 输入		:	GPIOx--GPIO端口，GPIO_Pin_n--GPIO管脚号
* 输出		:
* 返回 		:
*******************************************************************************/
void GPIO_Configuration_AOD2(
														GPIO_TypeDef* GPIOx,							//GPIO端口,x=A/B/C/D/E/F/G
														u16 GPIO_Pin_n										//GPIO管脚号n=0~15/All
														)
{
	//1）GPIO结构体定义
	GPIO_InitTypeDef	GPIO_InitStructure;
	//2）初始化GPIO结构体
	GPIO_InitStructure.GPIO_Pin  		= GPIO_Pin_n;
  GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;					//最大速度频率2MHz
  GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF_OD;					//AOD(复用开漏)输出模式
	//3)根据GPIO端口及管脚号开启端口时钟及确定是否打开AFIO时钟
	GPIO_ClockConf(GPIOx,GPIO_Pin_n);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);				//打开复用时钟
	//3）初始化GPIO
	GPIO_Init(GPIOx,&GPIO_InitStructure);			//GPIOA	
}
/*******************************************************************************
* 函数名		:	GPIO_Configuration_AOD10
* 功能描述	:	将GPIO相应管脚配置为AOD(复用开漏)输出模式，最大速度10MHz----V20170605
* 输入		:	GPIOx--GPIO端口，GPIO_Pin_n--GPIO管脚号
* 输出		:
* 返回 		:
*******************************************************************************/
void GPIO_Configuration_AOD10(
														GPIO_TypeDef* GPIOx,							//GPIO端口,x=A/B/C/D/E/F/G
														u16 GPIO_Pin_n										//GPIO管脚号n=0~15/All
														)
{
	//1）GPIO结构体定义
	GPIO_InitTypeDef	GPIO_InitStructure;
	//2）初始化GPIO结构体
	GPIO_InitStructure.GPIO_Pin  		= GPIO_Pin_n;
  GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_10MHz;					//最大速度频率10MHz
  GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF_OD;					//AOD(复用开漏)输出模式
	//3)根据GPIO端口及管脚号开启端口时钟及确定是否打开AFIO时钟
	GPIO_ClockConf(GPIOx,GPIO_Pin_n);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);				//打开复用时钟
	//3）初始化GPIO
	GPIO_Init(GPIOx,&GPIO_InitStructure);			//GPIOA	
}
/*******************************************************************************
* 函数名		:	GPIO_Configuration_AOD50
* 功能描述	:	将GPIO相应管脚配置为AOD(复用开漏)输出模式，最大速度50MHz----V20170605
* 输入		:	GPIOx--GPIO端口，GPIO_Pin_n--GPIO管脚号
* 输出		:
* 返回 		:
*******************************************************************************/
void GPIO_Configuration_AOD50(
														GPIO_TypeDef* GPIOx,							//GPIO端口,x=A/B/C/D/E/F/G
														u16 GPIO_Pin_n										//GPIO管脚号n=0~15/All
														)
{
	//1）GPIO结构体定义
	GPIO_InitTypeDef	GPIO_InitStructure;
	//2）初始化GPIO结构体
	GPIO_InitStructure.GPIO_Pin  		= GPIO_Pin_n;
  GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;					//最大速度频率50MHz
  GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF_OD;					//AOD(复用开漏)输出模式
	//3)根据GPIO端口及管脚号开启端口时钟及确定是否打开AFIO时钟
	GPIO_ClockConf(GPIOx,GPIO_Pin_n);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);				//打开复用时钟
	//3）初始化GPIO
	GPIO_Init(GPIOx,&GPIO_InitStructure);			//GPIOA	
}
/*******************************************************************************
* 函数名		:	GPIO_Configuration_APP2
* 功能描述	:	将GPIO相应管脚配置为APP(复用推挽)输出模式，最大速度2MHz----V20170605
* 输入		:	GPIOx--GPIO端口，GPIO_Pin_n--GPIO管脚号
* 输出		:
* 返回 		:
*******************************************************************************/
void GPIO_Configuration_APP2(
														GPIO_TypeDef* GPIOx,							//GPIO端口,x=A/B/C/D/E/F/G
														u16 GPIO_Pin_n										//GPIO管脚号n=0~15/All
														)
{
	//1）GPIO结构体定义
	GPIO_InitTypeDef	GPIO_InitStructure;
	//2）初始化GPIO结构体
	GPIO_InitStructure.GPIO_Pin  		= GPIO_Pin_n;
  GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;					//最大速度频率2MHz
  GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF_PP;					//APP(复用推挽)输出模式
	//3)根据GPIO端口及管脚号开启端口时钟及确定是否打开AFIO时钟
	GPIO_ClockConf(GPIOx,GPIO_Pin_n);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);				//打开复用时钟
	//3）初始化GPIO
	GPIO_Init(GPIOx,&GPIO_InitStructure);			//GPIOA	
}
/*******************************************************************************
* 函数名		:	GPIO_Configuration_APP10
* 功能描述	:	将GPIO相应管脚配置为APP(复用推挽)输出模式，最大速度10MHz----V20170605
* 输入		:	GPIOx--GPIO端口，GPIO_Pin_n--GPIO管脚号
* 输出		:
* 返回 		:
*******************************************************************************/
void GPIO_Configuration_APP10(
														GPIO_TypeDef* GPIOx,							//GPIO端口,x=A/B/C/D/E/F/G
														u16 GPIO_Pin_n										//GPIO管脚号n=0~15/All
														)
{
	//1）GPIO结构体定义
	GPIO_InitTypeDef	GPIO_InitStructure;
	//2）初始化GPIO结构体
	GPIO_InitStructure.GPIO_Pin  		= GPIO_Pin_n;
  GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_10MHz;					//最大速度频率10MHz
  GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF_PP;					//APP(复用推挽)输出模式
	//3)根据GPIO端口及管脚号开启端口时钟及确定是否打开AFIO时钟
	GPIO_ClockConf(GPIOx,GPIO_Pin_n);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);				//打开复用时钟
	//3）初始化GPIO
	GPIO_Init(GPIOx,&GPIO_InitStructure);			//GPIOA	
}
/*******************************************************************************
* 函数名		:	GPIO_Configuration_APP50
* 功能描述	:	将GPIO相应管脚配置为APP(复用推挽)输出模式，最大速度50MHz----V20170605
* 输入		:	GPIOx--GPIO端口，GPIO_Pin_n--GPIO管脚号
* 输出		:
* 返回 		:
*******************************************************************************/
void GPIO_Configuration_APP50(
														GPIO_TypeDef* GPIOx,							//GPIO端口,x=A/B/C/D/E/F/G
														u16 GPIO_Pin_n										//GPIO管脚号n=0~15/All
														)
{
	//1）GPIO结构体定义
	GPIO_InitTypeDef	GPIO_InitStructure;
	//2）初始化GPIO结构体
	GPIO_InitStructure.GPIO_Pin  		= GPIO_Pin_n;
  GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;					//最大速度频率50MHz
  GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF_PP;					//APP(复用推挽)输出模式
	//3)根据GPIO端口及管脚号开启端口时钟及确定是否打开AFIO时钟
	GPIO_ClockConf(GPIOx,GPIO_Pin_n);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);				//打开复用时钟
	//3）初始化GPIO
	GPIO_Init(GPIOx,&GPIO_InitStructure);			//GPIOA	
}
/*******************************************************************************
* 函数名		:	GPIO_Configuration_APP50
* 功能描述	:	将GPIO相应管脚配置为APP(复用推挽)输出模式，最大速度50MHz----V20170605
* 输入		:	GPIOx--GPIO端口，GPIO_Pin_n--GPIO管脚号
* 输出		:
* 返回 		:
*******************************************************************************/
void GPIO_Toggle(
									GPIO_TypeDef* GPIOx,							//GPIO端口,x=A/B/C/D/E/F/G
									u16 GPIO_Pin_n										//GPIO管脚号n=0~15/All
									)
{
	//1）GPIO管脚输出低，返回Bit_RESET
	if(GPIO_ReadOutputDataBit(GPIOx,GPIO_Pin_n)==Bit_RESET)
	{
		GPIO_WriteBit(GPIOx,GPIO_Pin_n,Bit_SET);			//将相应GPIO管脚输出为高
	}
	//2）GPIO管脚输出高，返回Bit_SET
	else
	{
		GPIO_WriteBit(GPIOx,GPIO_Pin_n,Bit_RESET);			//将相应GPIO管脚输出为高
	}
}



















//*****************************************************以下为旧程序20170605********************
/*##############################################################################
################################################################################
# 模块名		:	STM32_GPIO	
# 功能描述	:	GPIO配置使用
# 使用例程	:	GPIO_Configuration(GPIOA,GPIO_Pin_All,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
						GPIO_Configuration(GPIOB,GPIO_Pin_1,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
################################################################################
###############################################################################*/
/*******************************************************************************
* 函数名		:	GPIO_Configuration
* 功能描述	:	GPIO配置 
* 输入		:	GPIOx：GPIOA~GPIOG
						GPIO_Pin_x:GPIO_Pin_0~GPIO_Pin_15;GPIO_Pin_All
* 输出		:
* 返回 		:
*******************************************************************************/
//void GPIO_Conf(	GPIO_TypeDef* GPIOX,						//GPIO端口
//								u16 GPIO_Pin_x,									//GPIO引脚
//								GPIOMode_TypeDef GPIO_Mode,			//工作模式
//								GPIOSpeed_TypeDef GPIO_Speed		//工作速度
//								)			//GPIO配置	
//{
//	GPIO_ClockConf(GPIOX,GPIO_Pin_x);														//开启相关GPIO时钟	
//	GPIO_InitStructure(GPIOX,GPIO_Pin_x,GPIO_Mode,GPIO_Speed);	//GPIO初始化
//}
/*******************************************************************************
* 函数名		:	GPIO_Configuration
* 功能描述	:	GPIO配置 
* 输入		:	GPIOx：GPIOA~GPIOG
						GPIO_Pin_x:GPIO_Pin_0~GPIO_Pin_15;GPIO_Pin_All
* 输出		:
* 返回 		:
*******************************************************************************/
//void GPIO_Configuration(GPIO_TypeDef* GPIOx,					//GPIO端口
//												u16 GPIO_Pin_x,								//GPIO引脚
//												GPIOMode_TypeDef GPIO_Mode,		//工作模式
//												GPIOSpeed_TypeDef GPIO_Speed	//工作速度
//												)			//GPIO配置	
//{	
//	GPIO_ClockConf(GPIOx,GPIO_Pin_x);														//开启相关GPIO时钟	
//	GPIO_InitStructure(GPIOx,GPIO_Pin_x,GPIO_Mode,GPIO_Speed);	//GPIO初始化
//}
/*******************************************************************************
* 函数名		:	GPIO_InitStructure
* 功能描述	:	GPIO初始化 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
//void GPIO_InitStructure(
//														GPIO_TypeDef* GPIOx,								//GPIO端口,x=A/B/C/D/E/F/G
//														GPIO_InitTypeDef* GPIO_InitStructure		//GPIO配置结构体
//														)
//{
//	//1)根据GPIO端口及管脚号开启端口时钟及确定是否打开AFIO时钟
//	GPIO_ClockConf(GPIOx,GPIO_InitStructure->GPIO_Pin);
//	//2）初始化GPIO
//	GPIO_Init(GPIOx,GPIO_InitStructure);						//GPIO初始化
//}
