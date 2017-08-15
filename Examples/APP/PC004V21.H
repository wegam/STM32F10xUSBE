#ifndef __PC004V21_H
#define __PC004V21_H

#include "stm32f10x_type.h"

void PC004V21_Configuration(void);
void PC004V21_Server(void);
void PC004V21_PinSet(void);


void PC004V21_KEYData(void);		//按键发药处理函数
	


u8 PC004V10_485_TR(void);				//485收发
void PC004V10_CAN_RX(void);			//主要接收数码管板地址，
void PC004V10_CAN_TX(void);			//CAN发送数据，地址，命令类型，数据
void PC004V10_CAN_COMMAD(void);	//CAN发送命令函数，地址，命令类型，数据--时间同步

u8 PC004V21_GetSwitchID(void);				//获取当前设备ID//机柜号

void Self_Test(u16 time);			//拔码开关为0时，计数显示测试

void PC004V10_delay(u16 time);


#endif

