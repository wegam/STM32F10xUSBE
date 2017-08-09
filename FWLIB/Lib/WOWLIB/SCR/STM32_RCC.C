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


#include "STM32_RCC.H"
//#include "STM32F10x_BitBand.H"


//RCC_ClocksTypeDef
/*******************************************************************************
*函数名		:	function
*功能描述	:	获取时间
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
u32 Get_Clocks(RCC_ClocksTypeDef* RCC_ClocksStatus)
{
//	RCC_ClocksTypeDef RCC_ClocksStatus;
	u32	apbclock = 0x00;
	RCC_GetClocksFreq(RCC_ClocksStatus);
	apbclock = RCC_ClocksStatus->SYSCLK_Frequency;
//	apbclock = RCC_ClocksStatus.PCLK2_Frequency;
	apbclock = RCC_ClocksStatus->PCLK1_Frequency;
	return apbclock;
	
}


