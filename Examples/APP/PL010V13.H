#ifndef __PL010V13_H
#define __PL010V13_H



void PL010V13_Configuration(void);
void PL010V13_Server(void);
void PL010V13_PinSet(void);
unsigned int PL010V13_PrintfString(unsigned short x,unsigned short y,unsigned char font,const char *format,...);				//后边的省略号就是可变参数
void PL010Delay(unsigned long time);
#endif

