
#ifndef __74HC595_H
#define __74HC595_H
//#include <stdint.h>
#include "stm32f10x_gpio.h"



typedef struct	_HC595_Pin
{
//	//SDA-PIN		//14脚，串行数据输入
	GPIO_TypeDef* 	HC595_SDA_PORT;				//GPIOX
	unsigned short 	HC595_SDA_Pin;				//GPIO_Pin_x
	
	//SCK-PIN		//11脚，数据输入时钟线,上升沿移位
	GPIO_TypeDef* 	HC595_SCK_PORT;				//GPIOX
	unsigned short 	HC595_SCK_Pin;				//GPIO_Pin_x
	
	//RCK-PIN		//12脚，输出存储器锁存时钟线，上升沿时移位寄存器的数据进入数据存储寄存器，下降沿时存储寄存器数据不变
	GPIO_TypeDef* 	HC595_RCK_PORT;				//GPIOX
	unsigned short 	HC595_RCK_Pin;				//GPIO_Pin_x
	
	//CLR-PIN		//10脚，主复位（低电平），低电平时将移位寄存器的数据清零。通常将它接Vcc。
	GPIO_TypeDef* 	HC595_CLR_PORT;				//GPIOX
	unsigned short 	HC595_CLR_Pin;				//GPIO_Pin_x
	
	//OE-PIN		//13脚，输出有效（低电平），高电平时禁止输出(高阻态)。如果单片机的引脚不紧张，
	//																		用一个引脚控制它，可以方便地产生闪烁和熄灭效果。比通过数据端移位控制要省时省力。
	GPIO_TypeDef* 	HC595_OE_PORT;				//GPIOX
	unsigned short 	HC595_OE_Pin;				//GPIO_Pin_x

}HC595_Pindef;





void HC595_PinConf(HC595_Pindef *Pinfo);
void HC595_WriteNumber(HC595_Pindef *Pinfo,u8 num);
void HC595_WriteNumberN(HC595_Pindef *Pinfo,u8 num);		//反向信号写入











#endif


