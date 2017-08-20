#ifndef __EC11Encoder_H
#define __EC11Encoder_H
#include "stm32f10x_gpio.h"



typedef enum	_EC11_Status
{
	IDLE=	0,						//�޴���
	Button,							//����
	ClockWise,					//CW˳ʱ���ź�
	AntiClockWise				//CCW��ʱ���ź�
}EC11_StatusTypeDef;				//���������ֵ


typedef struct	_EC11_Pin			//���������1usʱ�����
{
	//��Ҫ���ò�����
	//Button
	GPIO_TypeDef* 	EC11_Button_PORT;			//GPIOX
	unsigned short 	EC11_Button_Pin;			//GPIO_Pin_x	
	//KEY-A
	GPIO_TypeDef* 	EC11_A_PORT;					//GPIOX
	unsigned short 	EC11_A_Pin;						//GPIO_Pin_x	
	//KEY-B
	GPIO_TypeDef* 	EC11_B_PORT;					//GPIOX
	unsigned short 	EC11_B_Pin;						//GPIO_Pin_x

	unsigned	char	LongPressFlag;				//������־��LongPressFlag=0��ʾÿ��ֻ����һ�ΰ�ѹ������ֻ��һ�Σ�LongPressFlag=1�������ﵽLongPressStartTimeʱ����ʾ����������ÿ��ʱ�ﵽLongPressEffectiveTime�󰴼���������һ��
	unsigned 	int		LongPressStartTime;				//������Ч��ʼʱ��--LongPressStartTime��ʱ����Чʱ���ʾ������Ч
	unsigned 	int		LongPressEffectiveTime;		//������Чʱ��--ButtonTimeCount��ʱ����Чʱ���ʾ������Ч
	
	unsigned 	int		ButtonEffectiveTime;	//������Чʱ��--ButtonTimeCount��ʱ����Чʱ���ʾ������Ч---Ĭ�ϸ���ʱ��10us
	unsigned 	int		EncoderEffectiveTime;	//��������Чʱ��---Ĭ�ϸ���ʱ��10us
	
	//����Ҫ���ò���
	unsigned	char	ButtonActiveFlag;			//������Ч��־----��Ե��ΰ���,ButtonActiveFlag==0;����ʱ��δ����������Ч��ButtonActiveFlag==1��������Ч��ButtonTimeCount���ټ�ʱ����ֹ���
	unsigned	int		ButtonTimeCount;			//������ʱʱ��---��ʱ��ButtonEffectiveTime��ʾ������Ч�����˸���ʹ��
	unsigned	int		EncoderTimeCountA;		//����A��ʱʱ��---��ʱ��EncoderEffectiveTime��ʾ������Ч�����˸���ʹ��
	unsigned	int		EncoderTimeCountB;		//����B��ʱʱ��---��ʱ��EncoderEffectiveTime��ʾ������Ч�����˸���ʹ��
	

}EC11_ConfTypeDef;


//void TM1618_PinConf(TM1618_Pindef *Pinfo);
//void TM1618_WriteData(TM1618_Pindef *Pinfo,u16 Data);
//void TM1618_WriteDataN(TM1618_Pindef *Pinfo,u16 Data);		//�����ź�д��


#endif
