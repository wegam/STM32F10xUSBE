/******************************** User_library *********************************
* �ļ��� 	: STM32_SDCard.H
* ����   	: wegam@sina.com
* �汾   	: V
* ����   	: 2016/01/01
* ˵��   	: 
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
*������		:	function
*��������	:	��ȡʱ��
*����			: 
*���			:	��
*����ֵ		:	��
*����			:
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

