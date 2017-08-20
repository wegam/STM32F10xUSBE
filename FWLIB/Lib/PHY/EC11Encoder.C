#include "EC11Encoder.H"


#include "STM32_GPIO.H"

#include "stm32f10x_gpio.h"


/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void EC11_PinConf(EC11_ConfTypeDef *EC11_Conf)
{
	GPIO_Configuration_IPU	(EC11_Conf->EC11_Button_PORT,		EC11_Conf->EC11_Button_Pin);		//将GPIO相应管脚配置为上拉输入模式----V20170605
	GPIO_Configuration_IPU	(EC11_Conf->EC11_A_PORT,				EC11_Conf->EC11_A_Pin);					//将GPIO相应管脚配置为上拉输入模式----V20170605
	GPIO_Configuration_IPU	(EC11_Conf->EC11_B_PORT,				EC11_Conf->EC11_B_Pin);					//将GPIO相应管脚配置为上拉输入模式----V20170605
	
	//时间参数默认配置：//输入参数按1us时间计算
	if(EC11_Conf->LongPressFlag!=0)			//允许长按
	{
		if(EC11_Conf->LongPressStartTime==0)	//长按有效但是起始时间未设定，3秒后按键未松开表示长按
		{
			EC11_Conf->LongPressStartTime=3000000;	//3x1000x1000		3秒
		}
		if(EC11_Conf->LongPressEffectiveTime==0)	//长按有效但是步长时间未设定，每0.5（500ms)秒一步
		{
			EC11_Conf->LongPressEffectiveTime=500000;	//5x100x1000		0.5秒
		}
	}
	if(EC11_Conf->ButtonEffectiveTime==0)	//单按键有效时间--0.1秒
	{
		EC11_Conf->ButtonEffectiveTime=100000;			//1x100x1000
	}
	if(EC11_Conf->EncoderEffectiveTime==0)	//编码器有效时间---100mS
	{
		EC11_Conf->EncoderEffectiveTime=1000;				//1x1000
	}
	EC11_Conf->ButtonActiveFlag=0;
}
/*******************************************************************************
*函数名			:	EC11_GetStatus
*功能描述		:	获取编码器的状态值
*输入				: 
*返回值			:	无
*******************************************************************************/
EC11_StatusTypeDef EC11_GetStatus(EC11_ConfTypeDef *EC11_Conf)
{
	EC11_StatusTypeDef	EC11_Status;
	//--------------按键处理
	if(GPIO_ReadInputDataBit(EC11_Conf->EC11_Button_PORT,EC11_Conf->EC11_Button_Pin)==0)		//输入低电平---被按压
	{
//		EC11_Conf->ButtonTimeCount++;		//按键计时
		//------单次按键
		if(EC11_Conf->LongPressFlag==0)
		{
			EC11_Conf->ButtonTimeCount++;		//按键计时
			if(EC11_Conf->ButtonTimeCount>=EC11_Conf->ButtonActiveFlag)	//计时到达按键有效时间
			{
			}
		}
		//------长按
	}
	else		//按键松开
	{
		EC11_Conf->ButtonTimeCount=0;		//清除按键计时
	}
	//--------------编码器处理
	
	return(EC11_Status);
}















