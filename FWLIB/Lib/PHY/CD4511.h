
#ifndef __CD4511_H
#define __CD4511_H
//#include <stdint.h>
#include "stm32f10x_gpio.h"



typedef struct	_CD4511_Pin
{
	//CS-PIN
	GPIO_TypeDef* 	CD4511_A0_PORT;				//GPIOX
	unsigned short 	CD4511_A0_Pin;				//GPIO_Pin_x
	
	//SDI-PIN
	GPIO_TypeDef* 	CD4511_A1_PORT;				//GPIOX
	unsigned short 	CD4511_A1_Pin;				//GPIO_Pin_x
	
	//SDO-PIN
	GPIO_TypeDef* 	CD4511_A2_PORT;				//GPIOX
	unsigned short 	CD4511_A2_Pin;				//GPIO_Pin_x
	
	//CS-PIN
	GPIO_TypeDef* 	CD4511_A3_PORT;				//GPIOX
	unsigned short 	CD4511_A3_Pin;				//GPIO_Pin_x
	
	//ENABLE-PIN
	GPIO_TypeDef* 	CD4511_EN_PORT;				//GPIOX
	unsigned short 	CD4511_EN_Pin;				//GPIO_Pin_x

}CD4511_Pindef;




void CD4511_PinConf(CD4511_Pindef *Pinfo);
void CD4511_WriteData(CD4511_Pindef *Pinfo,u8 num);	//写入数据并显示

void CD4511_DisplayOFF(CD4511_Pindef *Pinfo);			//关闭显示---关NPN三极管











#endif


