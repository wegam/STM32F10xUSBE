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


#include "STM32_SYSTICK.H"
#include "STM32_WOW.H"
//#include "STM32F10x_BitBand.H"


/*******************************************************************************
* 函数名		:	SysTick_Configuration
* 功能描述	:	系统嘀嗒时钟配置72MHz,单位为uS
* 输入		:	0<Time<= 1C71C7(1864135)
* 输出		:
* 返回 		:
*******************************************************************************/
void SysTick_Configuration(unsigned long Time)	//系统嘀嗒时钟配置72MHz,单位为uS
{
//	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);					//系统时钟 72MHZ
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);			//系统时钟/8==9MHz
//	SysTick_SetReload(9);			//1uS
	SysTick_SetReload(9*Time);				//Time--uS
	SysTick_ITConfig(ENABLE);
	SysTick_CounterCmd(SysTick_Counter_Enable);	//使能计数
	SysTick_ITConfig(ENABLE);
}
/*******************************************************************************
* 函数名		:	SysTick_Server
* 功能描述	:	嘀嗒时钟服务
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void SysTick_Server(void)				//嘀嗒时钟服务
{
	WOW_Server();	
}



