#ifndef __SSD1963_H
#define __SSD1963_H


//#include "stm32f10x_type.h"
#include "stm32f10x_gpio.h"
//#include "stm32f10x_rcc.h"
//#include "stm32f10x_type.h"
//#include "STM32_GPIO.H"
//#include "font.h"

//#include	"stdio.h"			//用于printf
//#include	"string.h"			//用于printf
//#include	"stdarg.h"			//用于获取不确定个数的参数
//#include	"stdlib.h"			//malloc动态申请内存空间





#define SSD1963
#define SSD1963_BITBAND


//#include "STM32_PWM.H"
//#include "STM32_SYSTICK.H"
//#include "STM32_GPIO.H"
//#include "STM32_EXTI.H"

//#include "sys\sys.h"
/* http://ttmcu.taobao.com 雨亭电子
代码测试环境:JME-2核心板+1T指令周期的STC单片机（51内核STC12LE5A60S2)+33M晶振	  单片机工作电压3.3V
程序默认IO连接方式：
控制线：RS-P3^5;    WR-P3^6;   RD-P3^7;   CS-P1^0;   REST-P1^2;
数据线: DB0-DB7依次连接P0^0-P0^7;  DB8-DB15依次连接P2^0-P2^7;（8位模式下DB0-DB7可以不连接）
触摸功能连接方式：(不使用触摸可不连接)
D_CLK-P1^7;  D_CS-P1^4;  D_DIN-P3^0;  D_OUT-P3^1;  D_PENIRQ-P3^4;
*/ 
 //定义LCD的尺寸	
#define LCD_W 800
#define LCD_H 480


#define	Bus_16        //16位数据模式,如果使用8位模式，请注释此语句，如果使用16位模式，请打开此句,修改8位模式之前，请确认你手里的模块是否是8位总线接口
#define ShowCharAscii	//二维数组查询
#define Mode8080	//如果CONF引脚连接于VDDIO，单片机接口将被配置在8080模式。
//#define Mode6800	//如果CONF引脚连接于GND，单片机接口将被配置为6800模式接口


//IO连接
//#define	LCD_DataPortH P2     //高8位数据口,8位模式下只使用高8位 
//#define	LCD_DataPortL P0     //低8位数据口	,8位模式下低8位可以不接线,请确认P0口已经上拉10K电阻,不宜太小，最小4.7K,推荐10K.


//画笔颜色
#define SSD1963_WHITE         	 	0xFFFF
#define SSD1963_BLACK         	 	0x0000	  
#define SSD1963_BLUE         	 		0x001F  
#define SSD1963_BRED             	0XF81F
#define SSD1963_GRED 			 				0XFFE0
#define SSD1963_GBLUE							0X07FF
#define SSD1963_RED           	 	0xF800
#define SSD1963_MAGENTA       	 	0xF81F
#define SSD1963_GREEN         	 	0x07E0		//0X07E0
#define SSD1963_CYAN          	 	0x7FFF
#define SSD1963_YELLOW        	 	0xFFE0
#define SSD1963_BROWN 			 			0XBC40 //棕色
#define SSD1963_BRRED 			 			0XFC07 //棕红色
#define SSD1963_GRAY  			 			0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 	0X01CF	//深蓝色
#define LIGHTBLUE      	 	0X7D7C	//浅蓝色  
#define GRAYBLUE       	 	0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 	0X841F //浅绿色
#define LGRAY 			 			0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        	0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           	0X2B12 //浅棕蓝色(选择条目的反色)

//extern	char *Char_Buffer;
extern  u16 BACK_COLOR, POINT_COLOR;   //背景色，画笔色

//sbit LCD_RS = P3^5;  		 //数据/命令切换
//sbit LCD_WR = P3^6;		  //写控制
//sbit LCD_RD =P3^7;		     //读控制
//sbit LCD_CS=P1^0;		//片选	
//sbit LCD_REST = P1^2;	      //复位 




#define OFF 0
#define	ON	1

#define Status_L	0
#define Status_H	1



#define LCD_HDP 799			//水平像素点个数
#define LCD_HT	928			//显示时长 928
#define LCD_HPS	46			//消影时长		46
#define LCD_LPS	15			//15
#define LCD_HPW	48			//48

#define LCD_VDP	479			//垂直像素点个数
#define LCD_VT	525
#define LCD_VPS	16
#define LCD_FPS	8
#define LCD_VPW	16

#define	LCD_LR	0X05		//左向右扫描
#define	LCD_RL	0X04		//左向右扫描
#define	LCD_UD	0X04		//上往下扫描
#define	LCD_DU	0X06		//下往上扫描

//--------------------SD卡命令
#define	SD_CMD0	0X40;

//****************************管脚定义End**************************

//****************************控制定义Start************************
//--------------背光  //高电平---开背光
//#define LCD_BACKLIGHT(n)	{if(0)	GPIO_ResetBits(LCD_BACKLIGHT_PORT,	LCD_BACKLIGHT_PIN);	else	GPIO_SetBits(LCD_BACKLIGHT_PORT,	LCD_BACKLIGHT_PIN);}

//--------------读控制 //低电平 8080模式，读选通信号
//#define LCD_RD(n)	{if(0)	GPIO_ResetBits(LCD_RD_PORT,	LCD_RD_PIN);	else	GPIO_SetBits(LCD_RD_PORT,	LCD_RD_PIN);}

//--------------复位  //低电平有效
//#define LCD_REST(n)	{if(0)	GPIO_ResetBits(LCD_REST_PORT,	LCD_REST_PIN);	else	GPIO_SetBits(LCD_REST_PORT,	LCD_REST_PIN);}
//#define LCD_REST(0)	GPIO_ResetBits(LCD_REST_PORT,	LCD_REST_PIN)
//#define LCD_REST(1)	GPIO_SetBits(LCD_REST_PORT,	LCD_REST_PIN)

//--------------数据/命令切换   //数据--高电平//命令--低电平
//#define LCD_RS(n)	{if(0)	GPIO_ResetBits(LCD_RS_PORT,	LCD_RS_PIN);	else	GPIO_SetBits(LCD_RS_PORT,	LCD_RS_PIN);}  		 //数据/命令切换

//--------------写控制  	//0--写开启，1--写关闭
//#define LCD_WR(n)	{if(0)	GPIO_ResetBits(LCD_WR_PORT,	LCD_WR_PIN);	else	GPIO_SetBits(LCD_WR_PORT,	LCD_WR_PIN);}

//--------------片选，//芯片使能--低电平有效
//#define LCD_CS(n)	{if(0)	GPIO_ResetBits(LCD_CS_PORT,	LCD_CS_PIN);	else	GPIO_SetBits(LCD_CS_PORT,	LCD_CS_PIN);}

//****************************控制定义End**************************
typedef struct	_SSD1963_Pin
{
	//____________背光控制定义	//1:开背光;0:关背光
	GPIO_TypeDef* 	SSD1963_sBACKLIGHT_PORT;			//GPIOX
	unsigned short 	SSD1963_sBACKLIGHT_Pin;				//GPIO_Pin_x
	
	//____________RD控制定义1:取消片选;0:片选
	GPIO_TypeDef* 	SSD1963_sRD_PORT;							//GPIOX
	unsigned short 	SSD1963_sRD_Pin;							//GPIO_Pin_x
	
	//____________REST控制定义 1:运行;0:复位
	GPIO_TypeDef* 	SSD1963_sREST_PORT;						//GPIOX
	unsigned short 	SSD1963_sREST_Pin;						//GPIO_Pin_x
	
	//____________写数据/命令(D/C)控制定义 1:传数据;0:传命令
	GPIO_TypeDef* 	SSD1963_sDC_PORT;							//GPIOX
	unsigned short 	SSD1963_sDC_Pin;							//GPIO_Pin_x
	
	//____________读写数据（W/R）控制定义 1:读数据;0:写数据
	GPIO_TypeDef* 	SSD1963_sWR_PORT;							//GPIOX
	unsigned short 	SSD1963_sWR_Pin;							//GPIO_Pin_x
	
	//____________使能控制定义 1:失能;0:片选
	GPIO_TypeDef* 	SSD1963_sCS_PORT;							//GPIOX
	unsigned short 	SSD1963_sCS_Pin;							//GPIO_Pin_x
	
	//____________数据接口
	GPIO_TypeDef* 	SSD1963_sDATABUS_PORT;				//GPIOX
	unsigned short 	SSD1963_sDATABUS_Pin;					//GPIO_Pin_x

}SSD1963_Pindef;

extern SSD1963_Pindef *SSD1963_sPinfo;


#define SSD1963_BACKLIGHT_ON						GPIO_SetBits	(SSD1963_sPinfo->SSD1963_sBACKLIGHT_PORT, SSD1963_sPinfo->SSD1963_sBACKLIGHT_Pin)			//开背光
#define SSD1963_BACKLIGHT_OFF						GPIO_ResetBits(SSD1963_sPinfo->SSD1963_sBACKLIGHT_PORT, SSD1963_sPinfo->SSD1963_sBACKLIGHT_Pin)			//关背光
//____________RD控制定义
#define SSD1963_RD_H										GPIO_SetBits	(SSD1963_sPinfo->SSD1963_sRD_PORT, 				SSD1963_sPinfo->SSD1963_sRD_Pin)						//取消片选
#define SSD1963_RD_L										GPIO_ResetBits(SSD1963_sPinfo->SSD1963_sRD_PORT, 				SSD1963_sPinfo->SSD1963_sRD_Pin)						//使能片选
//____________REST控制定义
#define SSD1963_REST										GPIO_ResetBits(SSD1963_sPinfo->SSD1963_sREST_PORT, 			SSD1963_sPinfo->SSD1963_sREST_Pin)					//低电平
#define SSD1963_SET											GPIO_SetBits	(SSD1963_sPinfo->SSD1963_sREST_PORT, 			SSD1963_sPinfo->SSD1963_sREST_Pin)					//高电平
//____________写数据/命令(D/C)控制定义
#define SSD1963_DC_Command							GPIO_ResetBits(SSD1963_sPinfo->SSD1963_sDC_PORT, 				SSD1963_sPinfo->SSD1963_sDC_Pin)						//低电平
#define SSD1963_DC_Data									GPIO_SetBits	(SSD1963_sPinfo->SSD1963_sDC_PORT, 				SSD1963_sPinfo->SSD1963_sDC_Pin)						//高电平
//____________读写数据（W/R）控制定义
#define SSD1963_WR_Write								GPIO_ResetBits(SSD1963_sPinfo->SSD1963_sWR_PORT, 				SSD1963_sPinfo->SSD1963_sWR_Pin)						//低电平
#define SSD1963_WR_Read									GPIO_SetBits	(SSD1963_sPinfo->SSD1963_sWR_PORT, 				SSD1963_sPinfo->SSD1963_sWR_Pin)						//高电平
//____________使能控制定义
#define SSD1963_CS_Disable							GPIO_SetBits	(SSD1963_sPinfo->SSD1963_sCS_PORT, 				SSD1963_sPinfo->SSD1963_sCS_Pin)						//高电平
#define SSD1963_CS_Enable								GPIO_ResetBits(SSD1963_sPinfo->SSD1963_sCS_PORT, 				SSD1963_sPinfo->SSD1963_sCS_Pin)						//低电平


			





void SSD1963_Delay(u16 xms);

void SSD1963_PinConf(SSD1963_Pindef *Pinfo);

void SSD1963_Configuration(void);		//管脚初始化

void LCD_REST(void);					//复位

void SSD1963_Writ_Bus(unsigned short BUS_DATA);   //并行数据写入函数
void SSD1963_WR_DATA8(char VH,char VL); //发送数据-8位参数
void SSD1963_WR_DATA8(char VH,char VL); //发送数据-8位参数
	
//void SSD1963_Init(void); 
void SSD1963_Init0(void);
void SSD1963_Clear(u16 Color);
void SSD1963_Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
//void LCD_WR_DATA8(char VH,char VL); //发送数据-8位参数
void SSD1963_WR_DATA(unsigned short data);				//写数据
void SSD1963_WR_COMMAND(unsigned short Command);			//写命令


void SSD1963_ShowString(u16 x,u16 y,u8 font,u8 num,u8 *Buffer);		//显示字符串
void SSD1963_ShowChar(u16 x,u16 y,u8 num,u8 mode);								//显示一个字符



void SSD1963_DrawPoint(u16 x,u16 y);//画点
void SSD1963_DrawPoint_big(u16 x,u16 y);//画一个大点
u16  SSD1963_ReadPoint(u16 x,u16 y); //读点
void SSD1963_DrawCircle(u16 x0,u16 y0,u8 r);
void SSD1963_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);						//画直线
void SSD1963_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);				//画矩形框	   
void SSD1963_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);	//填充--清屏
//void SSD1963_ShowCharGT(u16 x,u16 y,u8 font,u8 num,u8 *Buffer);		//高通字库测试程序-----备删除/SSD1963_ShowString


void SSD1963_ShowNum(u16 x,u16 y,u32 num,u8 len);									//显示数字
void SSD1963_ShowEn(u16 x,u16 y,u32 num);													//显示单字节字符
void SSD1963_Show2Num(u16 x,u16 y,u16 num,u8 len);//显示2个数字串，包含数字

unsigned int SSD1963_PrintfStringbac(u16 x,u16 y,const char *format,...);			//后边的省略号就是可变参数

void SSD1963_ShowStringbac(u16 x,u16 y,const u8 *p);		 //显示一个字符串,16字体
 
void showhanzi(unsigned int x,unsigned int y,unsigned char index);
void RGB888ToRGB565(unsigned char Color_R,unsigned char Color_G,unsigned char Color_B,u16 *RGB565);			//RGB888转RGB565颜色(分别取高位)


void SSD1963_Configuration(void);
void SSD1963_Server(void);


#endif



