#ifndef __DRV8801_H
#define __DRV8801_H
#include "stm32f10x_gpio.h"




typedef struct	_DRV8801_Pin
{
	//VPORTI(ADC���)
	GPIO_TypeDef* 	DRV8801_VPORTI_PORT;			//GPIOX
	unsigned short 	DRV8801_VPORTI_Pin;				//GPIO_Pin_x
	
	//ENABLE(1-���У�0-�ŵ磩
	GPIO_TypeDef* 	DRV8801_EN_PORT;					//GPIOX
	unsigned short 	DRV8801_EN_Pin;						//GPIO_Pin_x
	
	//nSLEEP(0-���ߣ�������裬1-���У�
	GPIO_TypeDef* 	DRV8801_SLEEP_PORT;				//GPIOX
	unsigned short 	DRV8801_SLEEP_Pin;				//GPIO_Pin_x
	
	//PHASE(1-��ת��0-��ת�����ٷŵ緽��
	GPIO_TypeDef* 	DRV8801_PHASE_PORT;				//GPIOX
	unsigned short 	DRV8801_PHASE_Pin;				//GPIO_Pin_x
	
	//MODE1(��ͣ-����/���ٷŵ磩
	GPIO_TypeDef* 	DRV8801_MODE1_PORT;				//GPIOX
	unsigned short 	DRV8801_MODE1_Pin;				//GPIO_Pin_x
	
	//MODE2(0-PMOS�ŵ磬1-NMOS�ŵ磩
	GPIO_TypeDef* 	DRV8801_MODE2_PORT;				//GPIOX
	unsigned short 	DRV8801_MODE2_Pin;				//GPIO_Pin_x
	
	//nFAULT(��������)
	GPIO_TypeDef* 	DRV8801_FAULT_PORT;				//GPIOX
	unsigned short 	DRV8801_FAULT_Pin;				//GPIO_Pin_x


}DRV8801_Pindef;

void DRV8801_Delay(u16 time);
void DRV8801_PinConf(DRV8801_Pindef *Pinfo);
void DRV8801_CW(DRV8801_Pindef *Pinfo);					//˳ʱ��ת
void DRV8801_CCW(DRV8801_Pindef *Pinfo);				//��ʱ��ת
void DRV8801_Brake_SN(DRV8801_Pindef *Pinfo);		//����ͣ Nͨ���ŵ�
void DRV8801_Brake_SP(DRV8801_Pindef *Pinfo);		//����ͣ Pͨ���ŵ�
void DRV8801_Brake_FCW(DRV8801_Pindef *Pinfo);	//˳ʱ����ټ�ͣ
void DRV8801_Brake_FCCW(DRV8801_Pindef *Pinfo);	//��ʱ����ټ�ͣ
void DRV8801_Sleep(DRV8801_Pindef *Pinfo);			//˯��---����ʹ��
void DRV8801_WKup(DRV8801_Pindef *Pinfo);				//����---����ʹ��


#endif
