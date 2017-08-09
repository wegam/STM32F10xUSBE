#ifndef _gui_H_
#define _gui_H_

#include "FAT32.h"

#define RED 	0xf800
#define GREEN 	0x07e0
#define BLUE    0x001f
#define YELLOW  0xffe0
#define WHITE   0xffff
#define BLACK   0x0000  


/******º¯ÊýÉùÃ÷*************/

extern void lcd_paint(u16 color);
extern void lcd_show_half_char(u16 x,u16 y,u8 size,u16 f_color,u16 b_color,u8 pchar);
extern void lcd_show_full_char(u16 x,u16 y,u8 size,u16 f_color,u16 b_color,u8 *str);
extern void lcd_show_str(u16 x,u16 y,u8 size,u16 f_color,u16 b_color,u8 *str);
extern void lcd_draw_box(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);
//extern void lcd_draw_hline(u16 x1,u16 y1,u16 length,u16 color);
//extern void lcd_draw_vline(u16 x1,u16 y1,u16 length,u16 color);
//extern void lcd_draw_rect(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);

extern void lcd_show_map(u16 x,u16 y,u16 width,u16 height,ITEM_INFO_type *file_info);
//extern void lcd_show_map(u16 x,u16 y,u16 width,u16 height,u8 *dat);




#endif