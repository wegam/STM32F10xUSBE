#ifndef _R61509V_H_
#define _R61509V_H_

typedef unsigned long  u32;
typedef unsigned short u16;
typedef unsigned char  u8;

	           	

/******º¯ÊıÉùÃ÷*************/
																	 
void lcd_write_com(u16 reg_add);
void lcd_write_dat(u16 reg_dat);
void lcd_write_reg(u16 reg_add,u16 reg_dat);
void lcd_set_window(u16 x1,u16 y1,u16 x2,u16 y2);
void lcd_init();



#endif