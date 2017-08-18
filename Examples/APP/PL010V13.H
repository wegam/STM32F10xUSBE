#ifndef __PL010V13_H
#define __PL010V13_H



void PL010V13_Configuration(void);
void PL010V13_Server(void);
void PL010V13_PinSet(void);
unsigned int LCD_PrintfString(u16 x,u16 y,u8 font,const char *format,...);				//后边的省略号就是可变参数

#endif

