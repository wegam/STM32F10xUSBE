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
	if(EC11_Conf->ButtonEffectiveTime==0)	//单按键有效时间--0.05秒
	{
		EC11_Conf->ButtonEffectiveTime=50000;			//1x100x1000
	}
	if(EC11_Conf->EncoderEffectiveTime==0)	//编码器有效时间---1mS
	{
		EC11_Conf->EncoderEffectiveTime=1000;				//1x1000us
	}
	EC11_Conf->ButtonActiveFlag=0;			//按键有效标志----针对单次按键,ButtonActiveFlag==0;按键时长未到，按键无效，ButtonActiveFlag==1，按键有效，ButtonTimeCount不再计时，防止溢出
}
/*******************************************************************************
*函数名			:	EC11_GetStatus
*功能描述		:	获取编码器的状态值
*输入				: 
*返回值			:	无
*******************************************************************************/
EC11_StatusTypeDef EC11_GetStatus(EC11_ConfTypeDef *EC11_Conf)
{
	EC11_StatusTypeDef	EC11_Status=EC11_IDLE;	//无触发
	//--------------按键处理
	if(GPIO_ReadInputDataBit(EC11_Conf->EC11_Button_PORT,EC11_Conf->EC11_Button_Pin)==0)		//输入低电平---被按压
	{
//		EC11_Conf->ButtonTimeCount++;		//按键计时
		//------单次按键
		if(EC11_Conf->LongPressFlag==0&&EC11_Conf->ButtonActiveFlag==0)
		{			
			if(EC11_Conf->ButtonTimeCount<EC11_Conf->ButtonEffectiveTime)	//计时未到达按键有效时间
			{
				EC11_Conf->ButtonTimeCount++;		//按键计时
			}
			else
			{
				EC11_Conf->ButtonActiveFlag=1;	//按键有效标志----针对单次按键,ButtonActiveFlag==0;按键时长未到，按键无效，ButtonActiveFlag==1，按键有效，ButtonTimeCount不再计时，防止溢出
				return(EC11_Button);		//返回一次有效按键
			}
		}
		//------长按
		else	if(EC11_Conf->LongPressFlag==1)
		{
			EC11_Conf->ButtonTimeCount++;		//按键计时
			if(EC11_Conf->ButtonTimeCount>=EC11_Conf->ButtonEffectiveTime&&EC11_Conf->ButtonActiveFlag==0)	//计时到达按键有效时间且未发生单次触发
			{
				EC11_Conf->ButtonActiveFlag=1;	//按键有效标志----针对单次按键,ButtonActiveFlag==0;按键时长未到，按键无效，ButtonActiveFlag==1，按键有效，ButtonTimeCount不再计时，防止溢出
				return(EC11_Button);		//返回一次有效按键
			}
			else if(EC11_Conf->ButtonTimeCount>=EC11_Conf->LongPressStartTime&&EC11_Conf->ButtonActiveFlag==1)	//计时到达按键有效时间且已发生单次触发，后面则为连续按键计数
			{
				if(EC11_Conf->ButtonTimeCount>=EC11_Conf->LongPressStartTime+EC11_Conf->LongPressEffectiveTime)		//到达一个单步计时
				{
					EC11_Conf->ButtonTimeCount-=EC11_Conf->LongPressEffectiveTime;			//恢复一个单步计时，准备下一个单步计时 0.2秒一个返回周期
					return(EC11_Button);		//返回一次有效按键
				}				
			}
		}
	}
	else		//按键松开
	{
		EC11_Conf->ButtonActiveFlag=0;	//按键有效标志----针对单次按键,ButtonActiveFlag==0;按键时长未到，按键无效，ButtonActiveFlag==1，按键有效，ButtonTimeCount不再计时，防止溢出
		EC11_Conf->ButtonTimeCount=0;		//清除按键计时
	}
	//--------------编码器处理
	if(GPIO_ReadInputDataBit(EC11_Conf->EC11_A_PORT,EC11_Conf->EC11_A_Pin)==0)		//A脚检测到低电平
	{
		EC11_Conf->EncoderTimeCountA++;		//引脚A计时时间---计时到EncoderEffectiveTime表示编码有效，过滤干扰使用
	}
	if(GPIO_ReadInputDataBit(EC11_Conf->EC11_B_PORT,EC11_Conf->EC11_B_Pin)==0)		//B脚检测到低电平
	{
		EC11_Conf->EncoderTimeCountB++;		//引脚B计时时间---计时到EncoderEffectiveTime表示编码有效，过滤干扰使用
	}
	if(		(GPIO_ReadInputDataBit(EC11_Conf->EC11_A_PORT,EC11_Conf->EC11_A_Pin)==1)	\
			&&(GPIO_ReadInputDataBit(EC11_Conf->EC11_B_PORT,EC11_Conf->EC11_B_Pin)==1)	\
			&&(EC11_Conf->EncoderTimeCountA>EC11_Conf->EncoderEffectiveTime							\
			&&EC11_Conf->EncoderTimeCountB>EC11_Conf->EncoderEffectiveTime)
		)
	{
		if(EC11_Conf->EncoderTimeCountA>EC11_Conf->EncoderTimeCountB)
		{
			EC11_Status=EC11_ClockWise;					//CW顺时针信号
		}
		else
		{
			EC11_Status=EC11_AntiClockWise;				//CCW逆时针信号
		}		
		EC11_Conf->EncoderTimeCountA=0;		//引脚A计时时间---计时到EncoderEffectiveTime表示编码有效，过滤干扰使用
		EC11_Conf->EncoderTimeCountB=0;		//引脚B计时时间---计时到EncoderEffectiveTime表示编码有效，过滤干扰使用
		return(EC11_Status);
	}
	return(EC11_Status);
}















