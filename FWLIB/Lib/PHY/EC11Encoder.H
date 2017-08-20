#ifndef __EC11Encoder_H
#define __EC11Encoder_H
#include "stm32f10x_gpio.h"



typedef enum	_EC11_Status
{
	IDLE=	0,						//无触发
	Button,							//按键
	ClockWise,					//CW顺时针信号
	AntiClockWise				//CCW逆时针信号
}EC11_StatusTypeDef;				//编码器输出值


typedef struct	_EC11_Pin			//输入参数按1us时间计算
{
	//需要配置参数：
	//Button
	GPIO_TypeDef* 	EC11_Button_PORT;			//GPIOX
	unsigned short 	EC11_Button_Pin;			//GPIO_Pin_x	
	//KEY-A
	GPIO_TypeDef* 	EC11_A_PORT;					//GPIOX
	unsigned short 	EC11_A_Pin;						//GPIO_Pin_x	
	//KEY-B
	GPIO_TypeDef* 	EC11_B_PORT;					//GPIOX
	unsigned short 	EC11_B_Pin;						//GPIO_Pin_x

	unsigned	char	LongPressFlag;				//长按标志，LongPressFlag=0表示每次只允许一次按压，长按只当一次，LongPressFlag=1；长按达到LongPressStartTime时间后表示长按，后面每计时达到LongPressEffectiveTime后按键次数增加一次
	unsigned 	int		LongPressStartTime;				//长按有效起始时间--LongPressStartTime计时到有效时间表示按键有效
	unsigned 	int		LongPressEffectiveTime;		//按键有效时间--ButtonTimeCount计时到有效时间表示按键有效
	
	unsigned 	int		ButtonEffectiveTime;	//按键有效时间--ButtonTimeCount计时到有效时间表示按键有效---默认干扰时间10us
	unsigned 	int		EncoderEffectiveTime;	//编码器有效时间---默认干扰时间10us
	
	//不需要配置参数
	unsigned	char	ButtonActiveFlag;			//按键有效标志----针对单次按键,ButtonActiveFlag==0;按键时长未到，按键无效，ButtonActiveFlag==1，按键有效，ButtonTimeCount不再计时，防止溢出
	unsigned	int		ButtonTimeCount;			//按键计时时间---计时到ButtonEffectiveTime表示按键有效，过滤干扰使用
	unsigned	int		EncoderTimeCountA;		//引脚A计时时间---计时到EncoderEffectiveTime表示编码有效，过滤干扰使用
	unsigned	int		EncoderTimeCountB;		//引脚B计时时间---计时到EncoderEffectiveTime表示编码有效，过滤干扰使用
	

}EC11_ConfTypeDef;


//void TM1618_PinConf(TM1618_Pindef *Pinfo);
//void TM1618_WriteData(TM1618_Pindef *Pinfo,u16 Data);
//void TM1618_WriteDataN(TM1618_Pindef *Pinfo,u16 Data);		//反向信号写入


#endif

