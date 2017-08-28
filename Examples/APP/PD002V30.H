#ifndef __PD002V30_H
#define __PD002V30_H



void PD002V30_Configuration(void);
void Seg_Conf(void);
void PD002V30_Server(void);
void PD002V30_USART_Conf(void);
void PD002V30_USART1_Server(void);
void PD002V30_485_Server(void);
unsigned char PD002V30_GetSwitchID(void);				//获取拔码开关地址
unsigned char PD002V30_GetBufferArray(void);		//获取各抽屉的AD值

void cs5530init(void);
void cs5530test(void);
	


#endif

