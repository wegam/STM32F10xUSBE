#ifndef __DRV8801_H
#define __DRV8801_H
#include "stm32f10x_gpio.h"




typedef struct	_DRV8801_Pin
{
	//VPORTI(ADC输出)
	GPIO_TypeDef* 	DRV8801_VPORTI_PORT;			//GPIOX
	unsigned short 	DRV8801_VPORTI_Pin;				//GPIO_Pin_x
	
	//ENABLE(1-运行，0-放电）
	GPIO_TypeDef* 	DRV8801_EN_PORT;					//GPIOX
	unsigned short 	DRV8801_EN_Pin;						//GPIO_Pin_x
	
	//nSLEEP(0-休眠，输出高阻，1-运行）
	GPIO_TypeDef* 	DRV8801_SLEEP_PORT;				//GPIOX
	unsigned short 	DRV8801_SLEEP_Pin;				//GPIO_Pin_x
	
	//PHASE(1-正转，0-反转，快速放电方向）
	GPIO_TypeDef* 	DRV8801_PHASE_PORT;				//GPIOX
	unsigned short 	DRV8801_PHASE_Pin;				//GPIO_Pin_x
	
	//MODE1(急停-慢速/快速放电）
	GPIO_TypeDef* 	DRV8801_MODE1_PORT;				//GPIOX
	unsigned short 	DRV8801_MODE1_Pin;				//GPIO_Pin_x
	
	//MODE2(0-PMOS放电，1-NMOS放电）
	GPIO_TypeDef* 	DRV8801_MODE2_PORT;				//GPIOX
	unsigned short 	DRV8801_MODE2_Pin;				//GPIO_Pin_x
	
	//nFAULT(错误低输出)
	GPIO_TypeDef* 	DRV8801_FAULT_PORT;				//GPIOX
	unsigned short 	DRV8801_FAULT_Pin;				//GPIO_Pin_x


}DRV8801_Pindef;

void DRV8801_Delay(u16 time);
void DRV8801_PinConf(DRV8801_Pindef *Pinfo);
void DRV8801_CW(DRV8801_Pindef *Pinfo);					//顺时针转
void DRV8801_CCW(DRV8801_Pindef *Pinfo);				//逆时针转
void DRV8801_Brake_SN(DRV8801_Pindef *Pinfo);		//慢急停 N通道放电
void DRV8801_Brake_SP(DRV8801_Pindef *Pinfo);		//慢急停 P通道放电
void DRV8801_Brake_FCW(DRV8801_Pindef *Pinfo);	//顺时针快速急停
void DRV8801_Brake_FCCW(DRV8801_Pindef *Pinfo);	//逆时针快速急停
void DRV8801_Sleep(DRV8801_Pindef *Pinfo);			//睡眠---调速使用
void DRV8801_WKup(DRV8801_Pindef *Pinfo);				//唤醒---调速使用


#endif

