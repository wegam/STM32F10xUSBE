#ifndef __A3987_H
#define __A3987_H

#include "stm32f10x_gpio.h"




typedef struct	_A3987_Pin
{
	//���������ת�ٶȿ���
	GPIO_TypeDef* 	A3987_PUL_PORT;				//GPIOX
	unsigned short 	A3987_PUL_Pin;				//GPIO_Pin_x
	
	//DIR���������ת�������
	GPIO_TypeDef* 	A3987_DIR_PORT;				//GPIOX
	unsigned short 	A3987_DIR_Pin;				//GPIO_Pin_x
	
	//EN�������ʹ�ܿ��� 1:��ֹ���;	0��ʹ��
	GPIO_TypeDef* 	A3987_EN_PORT;				//GPIOX
	unsigned short 	A3987_EN_Pin;				//GPIO_Pin_x
	
	//MS1 ��λ1
	GPIO_TypeDef* 	A3987_MS1_PORT;				//GPIOX
	unsigned short 	A3987_MS1_Pin;				//GPIO_Pin_x
	
	//MS2 ��λ2
	GPIO_TypeDef* 	A3987_MS2_PORT;				//GPIOX
	unsigned short 	A3987_MS2_Pin;				//GPIO_Pin_x
	
	//SLEEP/RESET	˯�߻��߸�λ 1���������� 0������ģʽ
	GPIO_TypeDef* 	A3987_RS_PORT;				//GPIOX
	unsigned short 	A3987_RS_Pin;				//GPIO_Pin_x


}A3987_Pindef;

void A3987_Delay(u16 time);
void A3987_PinConf(A3987_Pindef *Pinfo);
void A3987_StepDriver(A3987_Pindef *Pinfo);



#endif
