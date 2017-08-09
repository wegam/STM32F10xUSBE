//#ifdef	SSD1963

/********************************************************************************
***SSD1963 LCD屏驱动器
********************************************************************************/
#include "SSD1963.H"

#include "STM32_GPIO.H"

#include "stm32f10x_rcc.h"
#include "stm32f10x_type.h"

#include "font.h"

#include	"stdio.h"			//用于printf
#include	"string.h"			//用于printf
#include	"stdarg.h"			//用于获取不确定个数的参数
#include	"stdlib.h"			//malloc动态申请内存空间

//#include <reg51.h>
//#include "intrins.h"
//#include "font\font.h"
//#include "sys\sys.h"
//#include "lcd\lcd.h"
/* http://ttmcu.taobao.com 雨亭电子
代码测试环境:JME-2核心板+1T指令周期的STC单片机（51内核STC12LE5A60S2)+33M晶振	  单片机工作电压3.3V
程序默认IO连接方式：
控制线：RS-P3^5;    WR-P3^6;   RD-P3^7;   CS-P1^0;   REST-P1^2;
数据线: DB0-DB7依次连接P0^0-P0^7;  DB8-DB15依次连接P2^0-P2^7;（8位模式下DB0-DB7可以不连接）
触摸功能连接方式：(不使用触摸可不连接)
D_CLK-P1^7;  D_CS-P1^4;  D_DIN-P3^0;  D_OUT-P3^1;  D_PENIRQ-P3^4;
*/
//u16 BACK_COLOR=GREEN;		//背景色
//u16 POINT_COLOR=BLUE;   //画笔色

//u16  HDP=800;
//u16  HT=928;
//u16  HPS=46;
//u16  LPS=15;
//u8   HPW=48;

//u16  VDP=480;
//u16  VT=525;
//u16  VPS=16;
//u16  FPS=8;
//u8   VPW=16;

//char *Char_Buffer;
unsigned char hanzidata;
SSD1963_Pindef *SSD1963_sPinfo=NULL;

u16 SSD1963_BACK_COLOR=	SSD1963_BLACK;		//背景色
u16 SSD1963_POINT_COLOR=SSD1963_RED;   //画笔色

/*******************************************************************************
*函数名		:	LCD_Writ_Bus
*功能描述	:	STM32内部温度传感器配置
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SSD1963_Delay(u16 xms)
{
	while(xms--);
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void SSD1963_PinConf(SSD1963_Pindef *Pinfo)
{
	GPIO_Configuration_OPP50	(Pinfo->SSD1963_sBACKLIGHT_PORT,		Pinfo->SSD1963_sBACKLIGHT_Pin);		//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->SSD1963_sRD_PORT,						Pinfo->SSD1963_sRD_Pin);					//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->SSD1963_sREST_PORT,					Pinfo->SSD1963_sREST_Pin);				//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->SSD1963_sDC_PORT,						Pinfo->SSD1963_sDC_Pin);					//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->SSD1963_sWR_PORT,						Pinfo->SSD1963_sWR_Pin);					//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->SSD1963_sCS_PORT,						Pinfo->SSD1963_sCS_Pin);					//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->SSD1963_sDATABUS_PORT,			Pinfo->SSD1963_sDATABUS_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	
	SSD1963_sPinfo=Pinfo;
}

/*******************************************************************************
*函数名		:	SSD1963_Configuration
*功能描述	:	管脚初始化
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SSD1963_Configuration(void)
{
	//-----------旧STM_GPIO配置程序
//	GPIO_Configuration(SSD1963_RD_PORT,					SSD1963_RD_PIN,					GPIO_Mode_Out_PP,	GPIO_Speed_50MHz);		//RD
//	GPIO_Configuration(SSD1963_REST_PORT,				SSD1963_REST_PIN,				GPIO_Mode_Out_PP,	GPIO_Speed_50MHz);		//REST
//	GPIO_Configuration(SSD1963_DC_PORT,					SSD1963_DC_PIN,					GPIO_Mode_Out_PP,	GPIO_Speed_50MHz);		//RS
//	GPIO_Configuration(SSD1963_WR_PORT,					SSD1963_WR_PIN,					GPIO_Mode_Out_PP,	GPIO_Speed_50MHz);		//WR
//	GPIO_Configuration(SSD1963_CS_PORT,					SSD1963_CS_PIN,					GPIO_Mode_Out_PP,	GPIO_Speed_50MHz);		//RS
//	GPIO_Configuration(SSD1963_DATABUS_PORT,		SSD1963_DATABUS_PIN,		GPIO_Mode_Out_PP,	GPIO_Speed_50MHz);		//WR
//	GPIO_Configuration(SSD1963_BACKLIGHT_PORT,	SSD1963_BACKLIGHT_PIN,	GPIO_Mode_Out_PP,	GPIO_Speed_50MHz);		//背光
	
//	//RD
//	GPIO_Configuration_OPP50(SSD1963_RD_PORT,					SSD1963_RD_PIN);						//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
//	//REST
//	GPIO_Configuration_OPP50(SSD1963_REST_PORT,				SSD1963_REST_PIN);					//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
//	//RS
//	GPIO_Configuration_OPP50(SSD1963_DC_PORT,					SSD1963_DC_PIN);						//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
//	//WR
//	GPIO_Configuration_OPP50(SSD1963_WR_PORT,					SSD1963_WR_PIN);						//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
//	//RS
//	GPIO_Configuration_OPP50(SSD1963_CS_PORT,					SSD1963_CS_PIN);						//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
//	//WR
//	GPIO_Configuration_OPP50(SSD1963_DATABUS_PORT,		SSD1963_DATABUS_PIN);				//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
//	//背光
//	GPIO_Configuration_OPP50(SSD1963_BACKLIGHT_PORT,	SSD1963_BACKLIGHT_PIN);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	SSD1963_Init0();						//LCD初始化
}
/*******************************************************************************
*函数名		:	LCD_REST
*功能描述	:	管脚初始化
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void LCD_REST(void)
{
	u16 time=500;
	u16	temp=time;
	
	SSD1963_REST;				//
	while(temp--);	
	SSD1963_SET;
	temp=time;
	while(temp--);
}
//条件编译-16位数据模式 
/*******************************************************************************
*函数名		:	LCD_Writ_Bus
*功能描述	:	16位总线写入
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
#ifdef  Bus_16
void SSD1963_Writ_Bus(unsigned short BUS_DATA)   //并行数据写入函数
{
//	LCD_DataPortH=VH;	
//	LCD_DataPortL=VL;
	SSD1963_WR_Write; 		//0--写开启，1--写关闭
	GPIO_Write(SSD1963_sPinfo->SSD1963_sDATABUS_PORT,BUS_DATA);
	SSD1963_WR_Read; 			//0--写开启，1--写关闭
}
#else			//条件编译-8位数据模式
/*******************************************************************************
*函数名		:	LCD_Writ_Bus
*功能描述	:	8位总线写入
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SSD1963_Writ_Bus(unsigned short BUS_DATA)   //并行数据写入函数
{	
	unsigned char VH,VL;
	VH=BUS_DATA>>8;
	VL=BUS_DATA&0XFF;
	
	LCD_DataPortH=VH;	
	SSD1963_WR(0);		//0--写开启，1--写关闭
	SSD1963_WR(1);		//0--写开启，1--写关闭
	SSD1963_DataPortH=VL;		
	SSD1963_WR(0);		//0--写开启，1--写关闭
	SSD1963_WR(1);		//0--写开启，1--写关闭
}
#endif
/*******************************************************************************
*函数名		:	LCD_WR_DATA8
*功能描述	:	STM32内部温度传感器配置
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
//void LCD_WR_DATA8(char VH,char VL) //发送数据-8位参数
//{
//	LCD_RS(1);
//	LCD_Writ_Bus(VH,VL);
//}
/*******************************************************************************
*函数名		:	LCD_WR_DATA
*功能描述	:	写数据
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
 void SSD1963_WR_DATA(unsigned short data)
{
	SSD1963_DC_Data;					//数据/命令切换	//1-数据，0-命令
	SSD1963_Writ_Bus(data);
}
/*******************************************************************************
*函数名		:	LCD_WR_REG
*功能描述	:	写命令
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SSD1963_WR_COMMAND(unsigned short Command)	 
{	
	SSD1963_DC_Command;				//数据/命令切换	//1-数据，0-命令
	SSD1963_Writ_Bus(Command);
}
/*******************************************************************************
*函数名		:	LCD_WR_REG_DATA
*功能描述	:	写命令和数据
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
 void SSD1963_WR_REG_DATA(int reg,int da)
{
	SSD1963_WR_COMMAND(reg);
	SSD1963_WR_DATA(da);
}
/*******************************************************************************
*函数名		:	Address_set
*功能描述	:	STM32内部温度传感器配置
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SSD1963_Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{ 
	SSD1963_WR_COMMAND(0x002A);			//设置列地址
	SSD1963_WR_DATA(x1>>8);	    
	SSD1963_WR_DATA(x1&0x00ff);
	SSD1963_WR_DATA(x2>>8);	    
	SSD1963_WR_DATA(x2&0x00ff);
	SSD1963_WR_COMMAND(0x002b);			//设置页地址	
	SSD1963_WR_DATA(y1>>8);	    
	SSD1963_WR_DATA(y1&0x00ff);
	SSD1963_WR_DATA(y2>>8);	    
	SSD1963_WR_DATA(y2&0x00ff);
	SSD1963_WR_COMMAND(0x002c);			//写内存起始地址 					 						 
}
/*******************************************************************************
*函数名		:	Lcd_Init
*功能描述	:	STM32内部温度传感器配置
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SSD1963_Init(void)
{

////调用一次这些函数，免得编译的时候提示警告
////	LCD_CS (0);
////	delayms(500);	
////	if(LCD_CS==0)
////	{
////		LCD_WR_REG_DATA(0,0);
////		LCD_ShowString(0,0,"01");
////		LCD_ShowNum(0,0,0,0);
////		LCD_Show2Num(0,0,0,0);
////		LCD_DrawPoint_big(0,0);
////		LCD_DrawRectangle(0,0,0,0);
////		Draw_Circle(0,0,0);
////	}
//	//1）——————————复位
//	SSD1963_REST;				//
//	delayms(500);	
//	SSD1963_SET;
//	delayms(500);
//	SSD1963_REST;
//	//2）——————————复位
//	LCD_CS(1);			//0--打开片选使能，1--取消片选
//	LCD_RD(1);
//	LCD_WR(1);			//0--写开启，1--写关闭
//	LCD_CS(0);  		//0--打开片选使能，1--取消片选
//	//3）——————————复位
//	LCD_WR_COMMAND(0x00E2);						//PLL multiplier, set PLL clock to 120M
//	LCD_WR_DATA(0x0023);	    				//N=0x36 for 6.5M, 0x23 for 10M crystal
//	LCD_WR_DATA(0x0002);
//	LCD_WR_DATA(0x0004);
//	
//	LCD_WR_COMMAND(0x00E0);  					// PLL enable
//	LCD_WR_DATA(0x0001);
//	
//	LCD_WR_COMMAND(0x00E0);
//	LCD_WR_DATA(0x0003);
//	
//	LCD_WR_COMMAND(0x0001);  					// software reset
//	delayms(500);
//	LCD_WR_COMMAND(0x00E6);						//PLL setting for PCLK, depends on resolution
//	LCD_WR_DATA(0x0003);
//	LCD_WR_DATA(0x00ff);
//	LCD_WR_DATA(0x00ff);

//	LCD_WR_COMMAND(0x00B0);						//LCD SPECIFICATION
//	LCD_WR_DATA(0x0000);
//	LCD_WR_DATA(0x0000);
//	LCD_WR_DATA((LCD_HDP>>8)&0X00FF);  		//Set HDP
//	LCD_WR_DATA(LCD_HDP&0X00FF);
//	LCD_WR_DATA((LCD_VDP>>8)&0X00FF);  		//Set VDP
//	LCD_WR_DATA(LCD_VDP&0X00FF);
//	LCD_WR_DATA(0x0000);

//	LCD_WR_COMMAND(0x00B4);	//HSYNC
//	LCD_WR_DATA((LCD_HT>>8)&0X00FF);  //Set HT
//	LCD_WR_DATA(LCD_HT&0X00FF);
//	LCD_WR_DATA((LCD_HPS>>8)&0X00FF);  //Set HPS
//	LCD_WR_DATA(LCD_HPS&0X00FF);
//	LCD_WR_DATA(LCD_HPW);			   //Set HPW
//	LCD_WR_DATA((LCD_LPS>>8)&0X00FF);  //Set HPS
//	LCD_WR_DATA(LCD_LPS&0X00FF);
//	LCD_WR_DATA(0x0000);

//	LCD_WR_COMMAND(0x00B6);							//VSYNC
//	LCD_WR_DATA((LCD_VT>>8)&0X00FF);   //Set VT
//	LCD_WR_DATA(LCD_VT&0X00FF);
//	LCD_WR_DATA((LCD_VPS>>8)&0X00FF);  //Set VPS
//	LCD_WR_DATA(LCD_VPS&0X00FF);
//	LCD_WR_DATA(LCD_VPW);			   //Set VPW
//	LCD_WR_DATA((LCD_FPS>>8)&0X00FF);  //Set FPS
//	LCD_WR_DATA(LCD_FPS&0X00FF);

//	LCD_WR_COMMAND(0x00BA);
//	LCD_WR_DATA(0x0005);    //GPIO[3:0] out 1

//	LCD_WR_COMMAND(0x00B8);
//	LCD_WR_DATA(0x0007);    //GPIO3=input, GPIO[2:0]=output
//	LCD_WR_DATA(0x0001);    //GPIO0 normal

//	LCD_WR_COMMAND(0x0036); //rotation
//	LCD_WR_DATA(0x0000);

//	LCD_WR_COMMAND(0x00F0); //pixel data interface
//	LCD_WR_DATA(0x0003);


//	delayms(500);

//	LCD_WR_COMMAND(0x0029); //display on



//	LCD_WR_COMMAND(0x00d0); 
//	LCD_WR_DATA(0x000d);
//	
////	LCD_Clear(BACK_COLOR);	//以背景色清屏
}
/*******************************************************************************
*函数名		:	Lcd_Init
*功能描述	:	STM32内部温度传感器配置
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SSD1963_Init0(void)
{

	u16 time=500;
	u16	temp=time;
//调用一次这些函数，免得编译的时候提示警告
	//1）——————————复位
//	SSD1963_BACKLIGHT_OFF;	//关背光
//	LCD_REST();
	SSD1963_REST;
//	delayms(10);
	SSD1963_SET;
	while(temp--);
	//2）——————————复位
	SSD1963_CS_Disable;			//先取消片选
	SSD1963_RD_H;
	SSD1963_WR_Write;				//总线功能为写数据
	SSD1963_CS_Enable;  		//使能	
	//3）——————————设置系统时钟  晶振频率 10MHz  250MHz < VCO < 800MHz
	SSD1963_WR_COMMAND(0x00E2);						//PLL multiplier, set PLL clock to 120M Start the PLL. Before the start, the system was operated with the crystal oscillator or clock input
	SSD1963_WR_DATA(0x0023);	    				//设置倍频 N=0x36 for 6.5M, 0x23 for 10M crystal
	SSD1963_WR_DATA(0x0001);							//设置分频
	SSD1963_WR_DATA(0x0004);							//完成设置
	//4）——————————使能PLL
	SSD1963_WR_COMMAND(0x00E0);  					//PLL enable
	SSD1963_WR_DATA(0x0001);
	
	SSD1963_WR_COMMAND(0x00E0);
	SSD1963_WR_DATA(0x0003);
	//5）——————————软件复位
	SSD1963_WR_COMMAND(0x0001);  					//software reset
//	delayms(5000);
	//6）——————————设置扫描频率
	SSD1963_WR_COMMAND(0x00E6);						//PLL setting for PCLK, depends on resolution
	SSD1963_WR_DATA(0x0003);
	SSD1963_WR_DATA(0x00FF);
	SSD1963_WR_DATA(0x00FF);
	//7）——————————设置LCD面板模式 Set the LCD panel mode (RGB TFT or TTL)
	SSD1963_WR_COMMAND(0x00B0);						//LCD SPECIFICATION
	SSD1963_WR_DATA(0x0000);
	SSD1963_WR_DATA(0x0000);
	SSD1963_WR_DATA((LCD_HDP>>8)&0X00FF);  		//设置水平像素点个数高8位		Set HDP 
	SSD1963_WR_DATA(LCD_HDP&0X00FF);					//设置水平像素点个数低8位
	SSD1963_WR_DATA((LCD_VDP>>8)&0X00FF);  		//设置垂直像素点个数高8位		Set VDP
	SSD1963_WR_DATA(LCD_VDP&0X00FF);					//设置垂直像素点个数低8位
	SSD1963_WR_DATA(0x0000);									//设置奇偶行RGB顺序，默认0，Even line RGB sequence&Odd line RGB sequence
	//8）——————————设置水平期 Set Horizontal Period
	SSD1963_WR_COMMAND(0x00B4);							//HSYNC
	SSD1963_WR_DATA((LCD_HT>>8)&0X00FF);  	//High byte of horizontal total period
	SSD1963_WR_DATA(LCD_HT&0X00FF);					//Low byte of the horizontal total period (display + non-display) in pixel clock (POR = 10101111)
																			//Horizontal total period = (HT + 1) pixels
	SSD1963_WR_DATA((LCD_HPS>>8)&0X00FF);  	//High byte of the non-display period between the start of the horizontal sync (LLINE) signal and the first
																			//display data. (POR = 000)
	SSD1963_WR_DATA(LCD_HPS&0X00FF);
	SSD1963_WR_DATA(LCD_HPW);			   //Set HPW
	SSD1963_WR_DATA((LCD_LPS>>8)&0X00FF);  //Set HPS
	SSD1963_WR_DATA(LCD_LPS&0X00FF);
	SSD1963_WR_DATA(0x0000);
	//9）——————————设置垂直期	Set Vertical Period
	SSD1963_WR_COMMAND(0x00B6);							//VSYNC
	SSD1963_WR_DATA((LCD_VT>>8)&0X00FF);   //Set VT
	SSD1963_WR_DATA(LCD_VT&0X00FF);
	SSD1963_WR_DATA((LCD_VPS>>8)&0X00FF);  //Set VPS
	SSD1963_WR_DATA(LCD_VPS&0X00FF);
	SSD1963_WR_DATA(LCD_VPW);			   //Set VPW
	SSD1963_WR_DATA((LCD_FPS>>8)&0X00FF);  //Set FPS
	SSD1963_WR_DATA(LCD_FPS&0X00FF);	
	//10）——————————配置GPIO
	SSD1963_WR_COMMAND(0x00B8);
	SSD1963_WR_DATA(0x0007);    //GPIO3=input, GPIO[2:0]=output //输出模式
	SSD1963_WR_DATA(0x0001);   	//0 GPIO0 is used to control the panel power with Enter Sleep Mode 0x10 or Exit Sleep Mode 0x11.
													//1 GPIO0 is used as normal GPIO
	//11）——————————设置GPIO（设置扫描方向） Set GPIO value for GPIO configured as output
	SSD1963_WR_COMMAND(0x00BA);
	SSD1963_WR_DATA((LCD_LR&0XFF)|(LCD_UD&0XFF));    //GPIO[3:0] out 1
	//12）——————————设置地址模式	Set Address Mode
	SSD1963_WR_COMMAND(0x0036); //rotation
	SSD1963_WR_DATA(0x0000);
	//13）——————————设置数据接口 Set Pixel Data Interface/Pixel Data Interface Format
	SSD1963_WR_COMMAND(0x00F0); //pixel data interface
	SSD1963_WR_DATA(0x0003);
	
//	delayms(5000);
	//14）——————————设置垂直期
	SSD1963_WR_COMMAND(0x0029); //display on
	//15）——————————设置垂直期
	SSD1963_WR_COMMAND(0x00d0); 
	SSD1963_WR_DATA(0x000D);

	SSD1963_Clear(SSD1963_BACK_COLOR);	//以背景色清屏
	
	SSD1963_BACKLIGHT_ON;		//开背光
}
/*******************************************************************************
*函数名		:	LCD_Clear
*功能描述	:	清屏函数
*输入			: Color:要清屏的填充色
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SSD1963_Clear(u16 Color)
{
//	u8 VH,VL;
	u16 i,j;
//	VH=Color>>8;
//	VL=Color;	
	SSD1963_Address_set(0,0,LCD_W-1,LCD_H-1);
	for(i=0;i<LCD_W;i++)
	{
		for (j=0;j<LCD_H;j++)
		{
			SSD1963_WR_DATA(Color);
		}
	}
}
/*******************************************************************************
*函数名		:	showhanzi
*功能描述	:	在指定位置显示一个汉字(32*33大小)
*输入			: dcolor:内容颜色
						gbcolor:背静颜色
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void showhanzi(unsigned int x,unsigned int y,unsigned char index)	
{  
	unsigned char i,j;
	unsigned char *temp=&hanzidata;    
	SSD1963_Address_set(x,y,x+31,y+31); //设置区域      
	temp+=index*128;	
	for(j=0;j<128;j++)
	{
		for(i=0;i<8;i++)
		{ 		     
			if((*temp&(1<<i))!=0)
			{
				SSD1963_WR_DATA(SSD1963_POINT_COLOR);
			} 
			else
			{
				SSD1963_WR_DATA(SSD1963_BACK_COLOR);
			}   
		}
		temp++;
	}
}
/*******************************************************************************
*函数名		:	LCD_DrawPoint
*功能描述	:	画点
*输入			: POINT_COLOR:此点的颜色
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SSD1963_DrawPoint(u16 x,u16 y)
{
	SSD1963_Address_set(x,y,x,y);//设置光标位置 
	SSD1963_WR_DATA(SSD1963_POINT_COLOR); 	    
}
/*******************************************************************************
*函数名		:	LCD_DrawPoint_big
*功能描述	:	画一个大点
*输入			: POINT_COLOR:此点的颜色
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SSD1963_DrawPoint_big(u16 x,u16 y)
{
	SSD1963_Fill(x-1,y-1,x+1,y+1,SSD1963_POINT_COLOR);
}
/*******************************************************************************
*函数名		:	LCD_Fill
*功能描述	:	在指定区域内填充指定颜色
*输入			: 区域大小:(xend-xsta)*(yend-ysta)
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SSD1963_Fill(
							u16 xsta,u16 ysta,
							u16 xend,u16 yend,u16 color
)
{          
	u16 i,j; 
	SSD1963_Address_set(xsta,ysta,xend,yend);      //设置光标位置 
	for(i=ysta;i<=yend;i++)
	{													   	 	
		for(j=xsta;j<=xend;j++)
			SSD1963_WR_DATA(color);//设置光标位置 	    
	} 					  	    
}
/*******************************************************************************
*函数名		:	LCD_DrawLine
*功能描述	:	画线
*输入			: x1,y1:起点坐标
						x2,y2:终点坐标
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SSD1963_DrawLine(
									u16 x1, u16 y1, 	//x1,y1:起点坐标
									u16 x2, u16 y2		//x2,y2:终点坐标
)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1;
	
	uRow=x1; 
	uCol=y1; 
	
	if(delta_x>0)
		incx=1; //设置单步方向 
	else if(delta_x==0)
		incx=0;//垂直线 
	else
	{
		incx=-1;
		delta_x=-delta_x;
	}
	
	if(delta_y>0)
		incy=1; 
	else if(delta_y==0)
		incy=0;//水平线 
	else
	{
			incy=-1;
			delta_y=-delta_y;
	} 
		
	if( delta_x>delta_y)
		distance=delta_x; //选取基本增量坐标轴 
	else
		distance=delta_y; 
	
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		SSD1963_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}
/*******************************************************************************
*函数名		:	LCD_DrawRectangle
*功能描述	:	画矩形
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SSD1963_DrawRectangle(
												u16 x1, 		//x1
												u16 y1, 		//y1
												u16 x2, 		//x2
												u16 y2			//y2
)
{
	SSD1963_DrawLine(x1,y1,x2,y1);		
	SSD1963_DrawLine(x1,y1,x1,y2);
	SSD1963_DrawLine(x1,y2,x2,y2);
	SSD1963_DrawLine(x2,y1,x2,y2);
}
/*******************************************************************************
*函数名		:	Draw_Circle
*功能描述	:	在指定位置画一个指定大小的圆
*输入			: (x,y):中心点
						r    :半径
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SSD1963_DrawCircle(
												u16 x0,		//(x,y):中心点
												u16 y0,		//(x,y):中心点
												u8 r			//r    :半径
)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		SSD1963_DrawPoint(x0-b,y0-a);             //3           
		SSD1963_DrawPoint(x0+b,y0-a);             //0           
		SSD1963_DrawPoint(x0-a,y0+b);             //1       
		SSD1963_DrawPoint(x0-b,y0-a);             //7           
		SSD1963_DrawPoint(x0-a,y0-b);             //2             
		SSD1963_DrawPoint(x0+b,y0+a);             //4               
		SSD1963_DrawPoint(x0+a,y0-b);             //5
		SSD1963_DrawPoint(x0+a,y0+b);             //6 
		SSD1963_DrawPoint(x0-b,y0+a);             
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		SSD1963_DrawPoint(x0+a,y0+b);
	}
}


///*******************************************************************************
//*函数名		:	LCD_ShowChar
//*功能描述	:	在指定位置显示一个字符
//*输入			: x,y		:起点坐标
//						font	:字体大小
//						num		:要显示的字符:" "--->"~"
//						Buffer	:缓冲
//*输出			:	无
//*返回值		:	无
//*例程			:
//*******************************************************************************/
//void SSD1963_ShowCharGT(
//										u16 x,			//x				:起点x坐标
//										u16 y,			//y				:起点y坐标
//										u8 font,		//font		:字体大小
//										u8 num,			//num			:字节数
//										u8 *Buffer	//Buffer	:显示的内容缓存
//)						//高通字库测试程序
//{
//	u8 temp;
//	u8 mode=0;
//	u8 i,j;
//	u16 colortemp=SSD1963_POINT_COLOR;
//	if(font==12)
//	{
//		if(x>LCD_W-12||y>LCD_H-12)
//			return;
//		SSD1963_Address_set(x,y,x+12-1,y+12-1);      //设置光标位置 
//	}
//	else if(font==16)
//	{
//		if(x>LCD_W-16||y>LCD_H-16)
//			return;
//		SSD1963_Address_set(x,y,x+16-1,y+16-1);      //设置光标位置 
//	}
//	else if(font==24)
//	{
//		if(x>LCD_W-24||y>LCD_H-24)
//			return;
//		SSD1963_Address_set(x,y,x+24-1,y+24-1);      //设置光标位置 
//	}
//	else if(font==32)
//	{
//		if(x>LCD_W-32||y>LCD_H-32)
//			return;
//		SSD1963_Address_set(x,y,x+32-1,y+32-1);      //设置光标位置 
//	}
//	else
//	{
//		return ;
//	}
//	SSD1963_DC_Data;																//数据/命令切换	//1-数据，0-命令
//	for(i=0;i<num;i++)
//	{ 
//		temp=Buffer[i];		 														//调用1608字体--二维数组形式--字库使用时取消  
//		for(j=0;j<8;j++)
//		{
//			if((temp&0x80)==0X80)
//			{
//				colortemp=SSD1963_POINT_COLOR;						//SSD1963_POINT_COLOR
//			}
//			else
//				colortemp=SSD1963_BACK_COLOR;

//			SSD1963_WR_DATA(colortemp);
//			temp=temp<<1;
//		}
//	}	
//}
/*******************************************************************************
*函数名		:	LCD_ShowChar
*功能描述	:	在指定位置显示一个字符
*输入			: x,y		:起点坐标
						font	:字体大小
						num		:要显示的字符:" "--->"~"
						Buffer	:缓冲
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SSD1963_ShowString(
										u16 x,			//x				:起点x坐标
										u16 y,			//y				:起点y坐标
										u8 font,		//font		:字体大小
										u8 num,			//num			:字节数
										u8 *Buffer	//Buffer	:显示的内容缓存
)						//高通字库测试程序
{
	u8 temp;
	u8 mode=0;
	u8 i,j;
	u16 colortemp=SSD1963_POINT_COLOR;
	if(font==12)
	{
		if(x>LCD_W-12||y>LCD_H-12)
			return;
		SSD1963_Address_set(x,y,x+12-1,y+12-1);      //设置光标位置 
	}
	else if(font==16)
	{
		if(x>LCD_W-16||y>LCD_H-16)
			return;
		SSD1963_Address_set(x,y,x+16-1,y+16-1);      //设置光标位置 
	}
	else if(font==24)
	{
		if(x>LCD_W-24||y>LCD_H-24)
			return;
		SSD1963_Address_set(x,y,x+24-1,y+24-1);      //设置光标位置 
	}
	else if(font==32)
	{
		if(x>LCD_W-32||y>LCD_H-32)
			return;
		SSD1963_Address_set(x,y,x+32-1,y+32-1);      //设置光标位置 
	}
	else
	{
		return ;
	}
	SSD1963_DC_Data;																//数据/命令切换	//1-数据，0-命令
	for(i=0;i<num;i++)
	{ 
		temp=Buffer[i];		 														//调用1608字体--二维数组形式--字库使用时取消  
		for(j=0;j<8;j++)
		{
			if((temp&0x80)==0X80)
			{
				colortemp=SSD1963_POINT_COLOR;						//SSD1963_POINT_COLOR
			}
			else
				colortemp=SSD1963_BACK_COLOR;

			SSD1963_WR_DATA(colortemp);
			temp=temp<<1;
		}
	}
}

/*******************************************************************************
*函数名		:	LCD_ShowChar
*功能描述	:	在指定位置显示一个字符
*输入			: x,y		:起点坐标
						num		:要显示的字符:" "--->"~"
						mode	:叠加方式(1)还是非叠加方式(0)
						num		:要显示的字符:" "--->"~"
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SSD1963_ShowChar(u16 x,u16 y,u8 num,u8 mode)
{
	u8 temp;
	u8 pos,t;
	u16 x0=x;
	u16 colortemp=SSD1963_POINT_COLOR;      
	if(x>LCD_W-16||y>LCD_H-16)return;	    
	//设置窗口		   
	num=num-' ';//得到偏移后的值
//	num=41;//得到偏移后的值
	SSD1963_Address_set(x,y,x+8-1,y+16-1);      //设置光标位置 
	if(!mode) //非叠加方式
	{
		for(pos=0;pos<16;pos++)
		{ 
			temp=code_asc2_1608[(u16)num*16+pos];		 	//调用1608字体
//			temp=code_num_1608[(u16)num*16+pos];		 //调用1608字体
			for(t=0;t<8;t++)
			{                 
				if(temp&0x01)
					SSD1963_POINT_COLOR=colortemp;
				else
					SSD1963_POINT_COLOR=SSD1963_BACK_COLOR;
				SSD1963_WR_DATA(SSD1963_POINT_COLOR);	
				temp>>=1; 
				x++;
			}
			x=x0;
			y++;
		}	
	}else//叠加方式
	{
		for(pos=0;pos<16;pos++)
		{
			temp=code_asc2_1608[(u16)num*16+pos];		 //调用1608字体
			for(t=0;t<8;t++)
			{                 
				if(temp&0x01)
					SSD1963_DrawPoint(x+t,y+pos);				//画一个点     
				temp>>=1; 
			}
		}
	}
	SSD1963_POINT_COLOR=colortemp;	    	   	 	  
}
//#endif
/*******************************************************************************
*函数名		:	mypow
*功能描述	:	m^n函数
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)
		result*=m;    
	return result;
}
/*******************************************************************************
*函数名		:	LCD_ShowNum
*功能描述	:	显示数字
*输入			: x,y		:起点坐标
						len 	:数字的位数
						color	:颜色
						num		:数值(0~4294967295);
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SSD1963_ShowNum(u16 x,u16 y,u32 num,u8 len)
{         	
	u8 t,temp;
	u8 enshow=0;
	num=(u32)num;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				SSD1963_ShowChar(x+8*t,y,' ',0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	SSD1963_ShowChar(x+8*t,y,temp+48,0); 
	}
}
/*******************************************************************************
*函数名		:	LCD_ShowEn
*功能描述	:	显示数字
*输入			: x,y		:起点坐标
						len 	:数字的位数
						color	:颜色
						num		:数值(0~4294967295);
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SSD1963_ShowEn(u16 x,u16 y,u32 num)
{         	
	u8 t=0,len=0;
	u32 num2=num;
	u8 enshow=0;
	while(num2)
	{
		num2=num2/10;
		len++;		
	}
	for(t=len;t>0;t--)
	{
		num2=mypow(10,t-1);
		if(num>9)
		{
			enshow=num/num2;
			num=num%num2;
		}
		else
			enshow=(char)num;
	 	SSD1963_ShowChar(x+8*(len-t),y,enshow+48,0); 
	}
}
/*******************************************************************************
*函数名		:	LCD_Show2Num
*功能描述	:	显示2个数字
*输入			: x,y:起点坐标
						num:数值(0~99);
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SSD1963_Show2Num(u16 x,u16 y,u16 num,u8 len)
{         	
	u8 t,temp;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
	 	SSD1963_ShowChar(x+8*t,y,temp+'0',0); 
	}
}
/*******************************************************************************
*函数名		:	LCD_ShowString
*功能描述	:	显示字符串
*输入			: x,y:起点坐标
						*p:字符串起始地址
						用16字体
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
unsigned int SSD1963_PrintfStringbac(
																	u16 x,u16 y,
																	const char *format,...
)								//后边的省略号就是可变参数
{ 

	
//		va_list ap; 										//VA_LIST 是在C语言中解决变参问题的一组宏，所在头文件：#include <stdarg.h>,用于获取不确定个数的参数
//    static char string[ 256 ];			//定义数组，
//    va_start( ap, format );
//    vsprintf( string , format, ap );    
//    va_end( ap );
	
	char	*Char_Buffer;

	//1)**********获取数据宽度
	u16 num=strlen((const char*)format);		//获取数据宽度
	//2)**********定义缓冲区大小变量
	unsigned int BufferSize;
	//3)**********args为定义的一个指向可变参数的变量，va_list以及下边要用到的va_start,va_end都是是在定义，可变参数函数中必须要用到宏， 在stdarg.h头文件中定义
	va_list args;                                        
	//4)**********申请动态空间
	Char_Buffer = (char*)malloc(sizeof(char) * num);
//	if(Char_Buffer==NULL)
//	{
//		Char_Buffer=NULL;
//		return;
//	}
	//5)**********初始化args的函数，使其指向可变参数的第一个参数，format是可变参数的前一个参数
	va_start(args, format);
	//6)**********正常情况下返回生成字串的长度(除去\0),错误情况返回负值
	BufferSize = vsprintf(Char_Buffer, format, args);
	num=BufferSize;
	//7)**********结束可变参数的获取
	va_end(args);                                      		
	//8)**********将等发送缓冲区大小（数据个数）及缓冲区地址发给DMA开启发送
//	while(*Char_Buffer!='\0')
	while(num>0)
	{       
		if(x>LCD_W-16)
		{
			x=0;
			y+=16;
		}
		if(y>LCD_H-16)
		{
			y=x=0;
			SSD1963_Clear(SSD1963_RED);
		}
		SSD1963_ShowChar(x,y,*Char_Buffer,0);
		x+=8;
		Char_Buffer++;
		num--;
	}
	//9)**********DMA发送完成后注意应该释放缓冲区：free(USART_BUFFER);
//	Char_Buffer=NULL;
	Char_Buffer-=BufferSize;
	free(Char_Buffer);		//发送完成后注意应该释放缓冲区：free(Char_Buffer); 
	return BufferSize;
//	Char_Buffer=NULL;
//	free(format);		//发送完成后注意应该释放缓冲区：free(Char_Buffer);	
}



/*******************************************************************************
*函数名		:	LCD_ShowString
*功能描述	:	显示字符串
*输入			: x,y:起点坐标
						*p:字符串起始地址
						用16字体
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SSD1963_ShowStringbac(
												u16 x,		
												u16 y,
												const u8 *p
)						//显示字符串
{         
	while(*p!='\0')
	{       
		if(x>LCD_W-16)
		{x=0;y+=16;}
		if(y>LCD_H-16)
		{y=x=0;SSD1963_Clear(SSD1963_RED);}
		SSD1963_ShowChar(x,y,*p,0);
		x+=8;
		p++;
	}  
}

/*******************************************************************************
*函数名		:	RGB888ToRGB565
*功能描述	:	RGB888转RGB565颜色(分别取高位)
*输入			: x,y:起点坐标
						*p:字符串起始地址
						用16字体
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void RGB888ToRGB565(
										unsigned char Color_R,		//RED	
										unsigned char Color_G,		//GREEN
										unsigned char Color_B,		//BLUE
										u16 *RGB565
)				//RGB888转RGB565颜色(分别取高位)
{  
	u16 temp=0;
	temp|=(u16)(Color_R&0XF8)<<8;
	temp|=(u16)(Color_G&0XFC)<<3;
	temp|=(u16)(Color_B>>3);
	*RGB565=temp;
}










///*******************************************************************************
//*函数名			:	function
//*功能描述		:	关背光
//*输入				: 
//*返回值			:	无
//*******************************************************************************/
//void SSD1963_BACKLIGHToff(SSD1963_Pindef *Pinfo)
//{
//	GPIO_ResetBits(Pinfo->SSD1963_sBACKLIGHT_PORT, Pinfo->SSD1963_sBACKLIGHT_Pin);
//}
///*******************************************************************************
//*函数名			:	function
//*功能描述		:	函数功能说明
//*输入				: 
//*返回值			:	无
//*******************************************************************************/
//void SSD1963_BACKLIGHTon(SSD1963_Pindef *Pinfo)
//{
//	GPIO_SetBits(Pinfo->SSD1963_sBACKLIGHT_PORT, Pinfo->SSD1963_sBACKLIGHT_Pin);
//}
///*******************************************************************************
//*函数名			:	function
//*功能描述		:	函数功能说明
//*输入				: 
//*返回值			:	无
//*******************************************************************************/
//void SSD1963_REST(SSD1963_Pindef *Pinfo)
//{
//	GPIO_ResetBits(Pinfo->SSD1963_sREST_PORT, Pinfo->SSD1963_sREST_Pin);
//}
///*******************************************************************************
//*函数名			:	function
//*功能描述		:	函数功能说明
//*输入				: 
//*返回值			:	无
//*******************************************************************************/
//void SSD1963_EST(SSD1963_Pindef *Pinfo)
//{
//	GPIO_SetBits(Pinfo->SSD1963_sREST_PORT, Pinfo->SSD1963_sREST_Pin);
//}
///*******************************************************************************
//*函数名			:	function
//*功能描述		:	函数功能说明
//*输入				: 
//*返回值			:	无
//*******************************************************************************/
//void SSD1963_cRDlow(SSD1963_Pindef *Pinfo)
//{
//	GPIO_ResetBits(Pinfo->SSD1963_sRD_PORT, Pinfo->SSD1963_sRD_Pin);
//}
///*******************************************************************************
//*函数名			:	function
//*功能描述		:	函数功能说明
//*输入				: 
//*返回值			:	无
//*******************************************************************************/
//void SSD1963_cRDhigh(SSD1963_Pindef *Pinfo)
//{
//	GPIO_SetBits(Pinfo->SSD1963_sRD_PORT, Pinfo->SSD1963_sRD_Pin);
//}
///*******************************************************************************
//*函数名			:	function
//*功能描述		:	函数功能说明
//*输入				: 
//*返回值			:	无
//*******************************************************************************/
//void SSD1963_cDClow(SSD1963_Pindef *Pinfo)
//{
//	GPIO_ResetBits(Pinfo->SSD1963_sDC_PORT, Pinfo->SSD1963_sDC_Pin);
//}
///*******************************************************************************
//*函数名			:	function
//*功能描述		:	函数功能说明
//*输入				: 
//*返回值			:	无
//*******************************************************************************/
//void SSD1963_cDChigh(SSD1963_Pindef *Pinfo)
//{
//	GPIO_SetBits(Pinfo->SSD1963_sDC_PORT, Pinfo->SSD1963_sDC_Pin);
//}
///*******************************************************************************
//*函数名			:	function
//*功能描述		:	函数功能说明
//*输入				: 
//*返回值			:	无
//*******************************************************************************/
//void SSD1963_cWRlow(SSD1963_Pindef *Pinfo)
//{
//	GPIO_ResetBits(Pinfo->SSD1963_sWR_PORT, Pinfo->SSD1963_sWR_Pin);
//}
///*******************************************************************************
//*函数名			:	function
//*功能描述		:	函数功能说明
//*输入				: 
//*返回值			:	无
//*******************************************************************************/
//void SSD1963_cWRhigh(SSD1963_Pindef *Pinfo)
//{
//	GPIO_SetBits(Pinfo->SSD1963_sWR_PORT, Pinfo->SSD1963_sWR_Pin);
//}
///*******************************************************************************
//*函数名			:	function
//*功能描述		:	函数功能说明
//*输入				: 
//*返回值			:	无
//*******************************************************************************/
//void SSD1963_cCSlow(SSD1963_Pindef *Pinfo)
//{
//	GPIO_ResetBits(Pinfo->SSD1963_sCS_PORT, Pinfo->SSD1963_sCS_Pin);
//}
///*******************************************************************************
//*函数名			:	function
//*功能描述		:	函数功能说明
//*输入				: 
//*返回值			:	无
//*******************************************************************************/
//void SSD1963_cCShigh(SSD1963_Pindef *Pinfo)
//{
//	GPIO_SetBits(Pinfo->SSD1963_sCS_PORT, Pinfo->SSD1963_sCS_Pin);
//}












//#endif

