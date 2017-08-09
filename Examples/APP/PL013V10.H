#ifndef __PL013V10_H
#define __PL013V10_H

#include "stm32f10x_type.h"

void PL013V10_Configuration(void);
void PL013V10_Server(void);
void PL013V10_PinSet(void);

void PL013V10_CAN_RX(void);			//主要接收数码管板地址，
void PL013V10_CAN_TX(void);			//CAN发送数据，地址，命令类型，数据
void PL013V10_CAN_COMMAD(void);	//CAN发送命令函数，地址，命令类型，数据--时间同步

u8 PL013V10_GetSwitchID(void);				//获取当前设备ID//机柜号

void PL013V10_DISPALY(u8 wei,u16 num);



void PL013V10_delay(u16 time);


#endif

