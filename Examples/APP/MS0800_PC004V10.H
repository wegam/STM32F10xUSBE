#ifndef __MS0800_PC004V10_H
#define __MS0800_PC004V10_H

//#include "STM32_USART.H"
//#include "STM32_TIM.H"
//#include "STM32_DMA.H"
//#include "STM32_PWM.H"
//#include "STM32_GPIO.H"
//#include "STM32_ADC.H"
//#include "STM32_DAC.H"
#include "STM32_CAN.H"
#include "STM32_PWM.H"
#include "STM32_CD4511BM.H"
#include "STM32_GPIO.H"
#include "STM32_USART.H"

#include "STM32_TOOLS.H"		//数据校验工具


void MS0800_PC004V10_Configuration(void);
void MS0800_PC004V10_Server(void);
u8 PC004V10_485_TR(void);				//485收发
void PC004V10_CAN_RX(void);			//主要接收数码管板地址，
void PC004V10_CAN_TX(void);			//CAN发送数据，地址，命令类型，数据
void PC004V10_CAN_COMMAD(void);	//CAN发送命令函数，地址，命令类型，数据--时间同步
void WORK_LED(u16 time);				//WORK_LED
void PC004V10_BUZZER(u16 time);	//蜂鸣器
u8 PC004V10_Get_ID(void);				//获取当前设备ID//机柜号

void Self_Test(u16 time);			//拔码开关为0时，计数显示测试

void PC004V10_delay(u16 time);


#endif

