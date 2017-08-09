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


#include "STM32_TOOLS.H"


/*******************************************************************************
* 函数名		:	8位异或校验
* 功能描述	: 
* 输入			: *P 指针，numb数据长度（个数）
* 输出			: 计算结果
* 返回			: 无
*******************************************************************************/
u8 BCC8(u8 *Pointer,u16 num)			//异或校验
{
	u16 i_Num=0;
	u16 temp=0;
	for(i_Num=0;i_Num<num;i_Num++)
	{
		temp=temp^*Pointer;
		Pointer++;	
	}
	return temp;
}
/*******************************************************************************
* 函数名		:	异或校验
* 功能描述	: 
* 输入			: 
* 输出			: 计算结果
* 返回			: 无
*******************************************************************************/
u8 CRC8(u8 *Pointer)			//循环冗余校验
{
	return 0;
}
/*******************************************************************************
* 函数名		:	纵向冗余校验
* 功能描述	: 
* 输入			: 无
* 输出			: 无
* 返回			: 无
*******************************************************************************/
u8 LRC8(u8 *Pointer)		//纵向冗余校验
{
	return 0;
}

