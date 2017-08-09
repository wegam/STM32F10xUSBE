#ifndef __PC001V21_H
#define __PC001V21_H



void PC001V21_Configuration(void);
void PC001V21_Server(void);
void PC001V21_PinSet(void);
void PC001V21_USART_Conf(void);
void PC001V21_USART1_Server(void);
void PC001V21_4851_Server(void);
void PC001V21_4852_Server(void);
unsigned char PC001V21_GetSwitchID(void);				//获取拔码开关地址
unsigned char PC001V21_GetBufferArray(void);		//获取各抽屉的AD值
void DR_LOCK_OUT(unsigned char dr);	//电子锁控制
void DR_LOCK_Check(void);						//电子锁状态检测
#endif

