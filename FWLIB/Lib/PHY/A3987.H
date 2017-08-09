#ifndef __A3987_H
#define __A3987_H

#include "stm32f10x_gpio.h"




typedef struct	_A3987_Pin
{
	//步进电机运转速度控制
	GPIO_TypeDef* 	A3987_PUL_PORT;				//GPIOX
	unsigned short 	A3987_PUL_Pin;				//GPIO_Pin_x
	
	//DIR步进电机运转方向控制
	GPIO_TypeDef* 	A3987_DIR_PORT;				//GPIOX
	unsigned short 	A3987_DIR_Pin;				//GPIO_Pin_x
	
	//EN步进电机使能控制 1:禁止输出;	0：使能
	GPIO_TypeDef* 	A3987_EN_PORT;				//GPIOX
	unsigned short 	A3987_EN_Pin;				//GPIO_Pin_x
	
	//MS1 相位1
	GPIO_TypeDef* 	A3987_MS1_PORT;				//GPIOX
	unsigned short 	A3987_MS1_Pin;				//GPIO_Pin_x
	
	//MS2 相位2
	GPIO_TypeDef* 	A3987_MS2_PORT;				//GPIOX
	unsigned short 	A3987_MS2_Pin;				//GPIO_Pin_x
	
	//SLEEP/RESET	睡眠或者复位 1：正常工作 0：掉电模式
	GPIO_TypeDef* 	A3987_RS_PORT;				//GPIOX
	unsigned short 	A3987_RS_Pin;				//GPIO_Pin_x


}A3987_Pindef;

void A3987_Delay(u16 time);
void A3987_PinConf(A3987_Pindef *Pinfo);
void A3987_StepDriver(A3987_Pindef *Pinfo);



#endif

