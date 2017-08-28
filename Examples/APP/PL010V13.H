#ifndef __PL010V13_H
#define __PL010V13_H



void PL010V13_Configuration(void);
void PL010V13_Server(void);
void PL010V13_PinSet(void);
unsigned int PL010V13_PrintfString(unsigned short x,unsigned short y,unsigned char font,const char *format,...);				//后边的省略号就是可变参数
void PL010Delay(unsigned long time);
void LCD_WS(void);		//位闪烁
void LCD_WXS(void);		//位显示
void LCD_DDSP(void);		//显示总共请求数量和已发数量
#endif

