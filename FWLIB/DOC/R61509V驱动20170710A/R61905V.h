#ifndef _R61905V_H_
#define _R61905V_H_


	           	

/******º¯ÊıÉùÃ÷*************/
																	 
extern void lcd_write_com(u16 reg_add);
extern void lcd_write_dat(u16 reg_dat);
extern void lcd_write_reg(u16 reg_add,u16 reg_dat);
extern void lcd_set_window(u16 x1,u16 y1,u16 x2,u16 y2);
extern void lcd_init();



#endif