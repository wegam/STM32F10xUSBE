/**************************************************************************************************
* Copyright (C), 1997-2016,  HAICHUANG IOT Tech. Co., Ltd.
* 文件<Bsp_LcdR61509V.h>
* 模块描述 :	定义、功能、说明
* 历史修改记录
*     <主要修改者>  <时间>   <版本 >   <说明>
*     xlyan        2016/9/26   1.0.0    创建文件
**************************************************************************************************/
#ifndef __R61509V_H_
#define __R61509V_H

#include "stm32f10x_type.h"
#include "stm32f10x_gpio.h"

#define R61509V_W 400
#define	R61509V_H 240

//****************************控制定义End**************************
typedef struct	_R61509V_Pin
{
	//____________背光控制定义	//1:开背光;0:关背光
	GPIO_TypeDef* 	R61509V_sBL_PORT;			//GPIOX
	unsigned short 	R61509V_sBL_Pin;				//GPIO_Pin_x
	
	//____________RD控制定义1:取消片选;0:片选
	GPIO_TypeDef* 	R61509V_sRD_PORT;							//GPIOX
	unsigned short 	R61509V_sRD_Pin;							//GPIO_Pin_x
	
	//____________REST控制定义 1:运行;0:复位
	GPIO_TypeDef* 	R61509V_sREST_PORT;						//GPIOX
	unsigned short 	R61509V_sREST_Pin;						//GPIO_Pin_x
	
	//____________
	GPIO_TypeDef* 	R61509V_sRS_PORT;							//GPIOX
	unsigned short 	R61509V_sRS_Pin;							//GPIO_Pin_x
	
	//____________读写数据（W/R）控制定义 1:读数据;0:写数据
	GPIO_TypeDef* 	R61509V_sWR_PORT;							//GPIOX
	unsigned short 	R61509V_sWR_Pin;							//GPIO_Pin_x
	
	//____________使能控制定义 1:失能;0:片选
	GPIO_TypeDef* 	R61509V_sCS_PORT;							//GPIOX
	unsigned short 	R61509V_sCS_Pin;							//GPIO_Pin_x
	
	//____________TE
	GPIO_TypeDef* 	R61509V_sTE_PORT;							//GPIOX
	unsigned short 	R61509V_sTE_Pin;							//GPIO_Pin_x
	
	//____________数据接口
	GPIO_TypeDef* 	R61509V_sDATABUS_PORT;				//GPIOX
	unsigned short 	R61509V_sDATABUS_Pin;					//GPIO_Pin_x

}R61509V_Pindef;

extern R61509V_Pindef *R61509V_sPinfo;

#define R61509V_CS_HIGH					(R61509V_sPinfo->R61509V_sCS_PORT->BSRR		= R61509V_sPinfo->R61509V_sCS_Pin)
#define R61509V_CS_LOW					(R61509V_sPinfo->R61509V_sCS_PORT->BRR 		= R61509V_sPinfo->R61509V_sCS_Pin)
#define R61509V_RS_HIGH					(R61509V_sPinfo->R61509V_sRS_PORT->BSRR 	= R61509V_sPinfo->R61509V_sRS_Pin)
#define R61509V_RS_LOW					(R61509V_sPinfo->R61509V_sRS_PORT->BRR 		= R61509V_sPinfo->R61509V_sRS_Pin)
#define R61509V_WR_HIGH					(R61509V_sPinfo->R61509V_sWR_PORT->BSRR 	= R61509V_sPinfo->R61509V_sWR_Pin)
#define R61509V_WR_LOW					(R61509V_sPinfo->R61509V_sWR_PORT->BRR 		= R61509V_sPinfo->R61509V_sWR_Pin)
#define R61509V_RD_HIGH					(R61509V_sPinfo->R61509V_sRD_PORT->BSRR 	= R61509V_sPinfo->R61509V_sRD_Pin)
#define R61509V_RD_LOW					(R61509V_sPinfo->R61509V_sRD_PORT->BRR 		= R61509V_sPinfo->R61509V_sRD_Pin)
#define R61509V_RST_HIGH				(R61509V_sPinfo->R61509V_sREST_PORT->BSRR = R61509V_sPinfo->R61509V_sREST_Pin)
#define R61509V_RST_LOW					(R61509V_sPinfo->R61509V_sREST_PORT->BRR 	= R61509V_sPinfo->R61509V_sREST_Pin)
#define R61509V_TE_HIGH					(R61509V_sPinfo->R61509V_sTE_PORT->BSRR 	= R61509V_sPinfo->R61509V_sTE_Pin)
#define R61509V_TE_LOW					(R61509V_sPinfo->R61509V_sTE_PORT->BRR 		= R61509V_sPinfo->R61509V_sTE_Pin)
#define R61509V_DATABUS_PORT		(R61509V_sPinfo->R61509V_sDATABUS_PORT)

//BAC
//#define R61509V_CS_HIGH		(LCD_CS_PORT->BSRR = LCD_CS_PIN)
//#define R61509V_CS_LOW		(LCD_CS_PORT->BRR = LCD_CS_PIN)
//#define R61509V_RS_HIGH		(LCD_RS_PORT->BSRR = LCD_RS_PIN)
//#define R61509V_RS_LOW		(LCD_RS_PORT->BRR = LCD_RS_PIN)
//#define R61509V_WR_HIGH		(LCD_WR_PORT->BSRR = LCD_WR_PIN)
//#define R61509V_WR_LOW		(LCD_WR_PORT->BRR = LCD_WR_PIN)
//#define R61509V_RD_HIGH		(LCD_RD_PORT->BSRR = LCD_RD_PIN)
//#define R61509V_RD_LOW		(LCD_RD_PORT->BRR = LCD_RD_PIN)
//#define R61509V_RST_HIGH	(LCD_RST_PORT->BSRR = LCD_RST_PIN)
//#define R61509V_RST_LOW		(LCD_RST_PORT->BRR = LCD_RST_PIN)
//#define LCD_TE_HIGH	(LCD_TE_PORT->BSRR = LCD_TE_PIN)
//#define LCD_TE_LOW	(LCD_TE_PORT->BRR = LCD_TE_PIN)

//#define LCD_61509_EN
//#ifdef 	LCD_61509_EN

#define LCD_PHY_WIDTH	( (u16) 240)
#define LCD_PHY_HEIGHT	( (u16) 400)
#define LCD_TOTAL_PIX	96000   /*240 * 400 */

#define LCD_DISP_ANGLE	GUI_DISP_ANGLE
#define LCD_START_LINE	0       /*the partial image 1 is displayed from the first line of the base image. */
#define LCD_LINE_NUM	LCD_PHY_HEIGHT

//#define LCD_FORE_COLOR	0x31a6
//#define LCD_BACK_COLOR	0xef5d
//#define R61509V_FORE_COLOR	0xef5d 		//前景色
//#define R61509V_BACK_COLOR	0x0000		//背景色

#define LCD_ALARM_COLOR  0xF800
#define LCD_TRANSPARENT_COLOR 0xFFFF


//画笔颜色
#define R61509V_WHITE         	 	0xFFFF
#define R61509V_BLACK         	 	0x0000	  
#define R61509V_BLUE         	 		0x001F  
#define R61509V_BRED             	0XF81F
#define R61509V_GRED 			 				0XFFE0
#define R61509V_GBLUE							0X07FF
#define R61509V_RED           	 	0xF800
#define R61509V_MAGENTA       	 	0xF81F
#define R61509V_GREEN         	 	0x07E0		//0X07E0
#define R61509V_CYAN          	 	0x7FFF
#define R61509V_YELLOW        	 	0xFFE0
#define R61509V_BROWN 			 			0XBC40 //棕色
#define R61509V_BRRED 			 			0XFC07 //棕红色
#define R61509V_GRAY  			 			0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 	0X01CF	//深蓝色
#define LIGHTBLUE      	 	0X7D7C	//浅蓝色  
#define GRAYBLUE       	 	0X5458 //灰蓝色
//以上三色为PANEL的颜色

//#define LCD_DATA_PORT	GPIOB
////#define LCD_CTRL_PORT	GPIOB

//#define LCD_CS_PORT	GPIOA
//#define LCD_CS_PIN	GPIO_Pin_12

//#define LCD_RS_PORT	GPIOA
//#define LCD_RS_PIN	GPIO_Pin_8

//#define LCD_WR_PORT	GPIOA
//#define LCD_WR_PIN	GPIO_Pin_15

//#define LCD_RD_PORT	GPIOC
//#define LCD_RD_PIN	GPIO_Pin_5

//#define LCD_RST_PORT	GPIOD
//#define LCD_RST_PIN	GPIO_Pin_2

//#define LCD_BL_PORT	GPIOA
//#define LCD_BL_PIN	GPIO_Pin_3

//#define LCD_TE_PORT	GPIOC
//#define LCD_TE_PIN	GPIO_Pin_4



#if 1
	#define R61509V_BL_ON		PWM_OUT((TIM_TypeDef*) TIM2_BASE,PWM_OUTChannel4,1000,200)	//(LCD_BL_PORT->BSRR = LCD_BL_PIN)
	#define R61509V_BL_OFF	PWM_OUT((TIM_TypeDef*) TIM2_BASE,PWM_OUTChannel4,2000,0)		//(LCD_BL_PORT->BRR = LCD_BL_PIN)
#else
	#define R61509V_BL_ON		(LCD_BL_PORT->BSRR = LCD_BL_PIN)
	#define R61509V_BL_OFF	(LCD_BL_PORT->BRR = LCD_BL_PIN)

#endif


/*功能位 */
#define LCD_DC1_IMAGE_ENABLE_BIT	0x1000
#define LCD_DC1_PARTIAL_ENABLE_BIT	0x0100

#define R61509V_R000_IR			( (u16) 0x0000) 	//Index (IR)/*[10:0]REG ID */
#define R61509V_R000_DCR		( (u16) 0x0000) 	//Device code read (R000h)/*[15:0]	B509H Read */			//Device code read
#define R61509V_R001_DOC		( (u16) 0x0001) 	//Driver Output Control (R001h)/*[8]SS,[10]SM */
#define R61509V_R002_DWC		( (u16) 0x0002) 	//LCD Drive Wave Control (R002h)/*[8]BC */
#define R61509V_R003_EM			( (u16) 0x0003) 	//Entry Mode (R003h)/*[3]AM,[5:4]ID,[7]ORG,[12]BGR,[14]DFM,[15]TRI */
#define R61509V_R007_DC1		( (u16) 0x0007) 	//Display Control 1 (R007h)/*[8]BASEE,[12]PTDE */
#define R61509V_R008_DC2		( (u16) 0x0008) 	//Display Control 2 (R008h)/*[15:8]FP,[7:0]BP */
#define R61509V_R009_DC3		( (u16) 0x0009) 	//Display Control 3 (R009h)/*[10]PTS,[11]PTV */
#define R61509V_R00B_ECC		( (u16) 0x000B)		//Eight Color Control (R00Bh) /*[0]COL */
#define R61509V_R00C_EDIC1	( (u16) 0x000C) 	//External Display Interface Control 1 (R00Ch)/*[0]RIM,[5:4]DM,[8]RM,[14:12]ENC */
#define R61509V_R00F_EDIC2	( (u16) 0x000F) 	//External Display Interface Control 2 (R00Fh)//*[0]DPL,[1]EPL,[3]HSPL,[4]VSPL */
#define R61509V_R010_PIC1		( (u16) 0x0010) 	//Panel Interface Control 1 (R010h)	/*[5:0]RTNI,[]DIV */
#define R61509V_R011_PIC2		( (u16) 0x0011) 	//Panel Interface Control 2 (R011h)	/*[2:0]STDI,[10:8]NOWI */
#define R61509V_R012_PIC3		( (u16) 0x0012) 	//Panel Interface Control 3 (R012h)	/*[2:0]SEQWI,[10:8]VEQWI */
#define R61509V_R013_PIC4		( (u16) 0x0013) 	//Panel Interface Control 4 (R013h)	/*[2:0]MCPI */
#define R61509V_R014_PIC5		( (u16) 0x0014) 	//Panel Interface Control 5 (R014h)	/*[2:0]PCDIVL,[10:8]PCDIVH */
#define R61509V_R020_PIC6		( (u16) 0x0020) 	//Panel Interface Control 6 (R020h)	/*[] */
#define R61509V_R021_PIC7		( (u16) 0x0021) 	//Panel Interface Control 7 (R021h)	/*[2:0]STDE,[10:8]NOWE */
#define R61509V_R022_PIC8		( (u16) 0x0022) 	//Panel Interface Control 8 (R022h)	/*[2:0]SEQWE,[10:8]VEQWE */
#define R61509V_R023_PIC9		( (u16) 0x0023) 	//Panel Interface Control 9 (R023h)	/*[7:0]MCPE */
#define R61509V_R090_FMC		( (u16) 0x0090) 	//Frame Marker Control (R090h)	/*[8:0]FMP,[14:12]FMI,[15]FMKM */
#define R61509V_R100_PC1		( (u16) 0x0100) 	//Power Control 1 (R100h)	/*[2]DSTB,[5:4]AP,[10:8]BT */
#define R61509V_R101_PC2		( (u16) 0x0101)		//Power Control 2 (R101h) /*[2:0]VC,[6:4]DCO,[10:8]DC1 */
#define R61509V_R102_PC3		( (u16) 0x0102) 	//Power Control3 (R102h)	/*[4]PON,[5]PSON,[8]VCMR,[15:11]VRH */
#define R61509V_R103_PC4		( (u16) 0x0103) 	//Power Control 4 (R103h)	/*[12:8]VDV */
#define R61509V_R200_HA			( (u16) 0x0200)		//RAM Address Set (Horizontal Address) (R200h)	 /*[7:0] */
#define R61509V_R201_VA			( (u16) 0x0201) 	//RAM Address Set (Vertical Address) (R201h)	/*[8:0] */
#define R61509V_R202_GDRW		( (u16) 0x0202)		//GRAM Data Write (R202h)/GRAM Data Read (R202h)
#define R61509V_R280_NVM		( (u16) 0x0280) 	//NVM Data Read / NVM Data Write (R280h)	/*[7:0]UID,[14:8]VCM */
#define R61509V_R210_HSA		( (u16) 0x0210) 	//Window Horizontal RAM Address Start (R210h)		/*[7:0] */
#define R61509V_R211_HEA		( (u16) 0x0211) 	//Window Horizontal RAM Address End(R211h)	/*[7:0] */
#define R61509V_R212_VSA		( (u16) 0x0212)		//Window Vertical RAM Address Start (R212h) /*[8:0] */
#define R61509V_R213_VEA		( (u16) 0x0213) 	//Window Vertical RAM Address End (R213h)	/*[8:0] */
#define R61509V_R300_YC1		( (u16) 0x0300)		//γ Control 1(R300)	
#define R61509V_R301_YC2		( (u16) 0x0301)		//γ Control 2(R301)	
#define R61509V_R302_YC3		( (u16) 0x0302)		//γ Control 3(R302)	
#define R61509V_R303_YC4		( (u16) 0x0303)		//γ Control 4(R303)	
#define R61509V_R304_YC5		( (u16) 0x0304)		//γ Control 5(R304)	
#define R61509V_R305_YC6		( (u16) 0x0305)		//γ Control 6(R305)	
#define R61509V_R306_YC7		( (u16) 0x0306)		//γ Control 7(R306)	
#define R61509V_R307_YC8		( (u16) 0x0307)		//γ Control 8(R307)	
#define R61509V_R308_YC9		( (u16) 0x0308)		//γ Control 9(R308)	
#define R61509V_R309_YC10		( (u16) 0x0309)		//γ Control 10(R309)	
#define R61509V_R400_BINL		( (u16) 0x0400)		//Base Image Number of Line (R400h) /*[15]GS,[14:9]NL,[6:1]SCN */
#define R61509V_R401_BIDC		( (u16) 0x0401) 	//Base Image Display Control (R401h)	/*[2]NDL,[1]VLE,[0]REV */
#define R61509V_R404_BIVSC	( (u16) 0x0404) 	//Base Image Vertical Scroll Control (R404h)	/*[8:0]VL */
#define R61509V_R500_PTDP		( (u16) 0x0500) 	//Display Position (R500h)	/*[8:0] */
#define R61509V_R501_PTSA		( (u16) 0x0501) 	//RAM Address 1 (Start Line Address) (R501h)	/*[8:0] */
#define R61509V_R502_PTEA		( (u16) 0x0502) 	//RAM Address 1 (End Line Address) (R502h)	/*[8:0] */
#define R61509V_R600_TR			( (u16) 0x0600) 	//Test Register (Software Reset) (R600h)	/*[0]TRSR */
#define R61509V_R6F0_NVMC1	( (u16) 0x06F0)  	//NVM Access Control 1 (R6F0h),	/*[5:4]EOP,[6]CALB,[7]TE */
#define R61509V_R6F1_NVMC2	( (u16) 0x06F1)  	//NVM Access Control 2 (R6F1h),	/*[15:0]NVDAT */
#define R61509V_R6F2_NVMC3	( (u16) 0x06F2)  	//NVM Access Control 3 (R6F2h)/*[3]NVVRF */



typedef struct
{
	u16 x;
	u16 y;
}t_Point;

typedef struct
{
	t_Point top;
	t_Point bottom;
}t_Rect;

typedef enum
{
	SCREEN_ROTATE_0D = 0,
	SCREEN_ROTATE_90D,
	SCREEN_ROTATE_180D,
	SCREEN_ROTATE_270D,
}t_Sreen_Rotate;			//扫描方式
typedef enum
{
	DRAW_ROTATE_0D = 0,
	DRAW_ROTATE_90D,
	DRAW_ROTATE_180D,
	DRAW_ROTATE_270D,
} t_DRAW_ROTATE;

typedef struct
{
	u16 XPixNum;
	u16 YPixNum;
}t_PixCfg;
typedef struct
{

	t_Sreen_Rotate ScrnRotat;//旋转角度
	t_PixCfg PixCfg;//像素宽高
	u16		ForeColor;
	struct
	{
		u8	Rotat : 2;
		u8	Mirror : 1;
		u8	Rev    : 1;
		u8	Mode    : 1;
		u8	Rsvd    : 3;
	} DrawMode;
}t_LcdCfg;
typedef struct
{
	u16 Width;
	u16 Height;
	u8  PixBits;
	u8  *pData;
	
}	IMAGE_PARA;



//void R61509V_Delay(u16 xms);
void R61509V_PinConf(R61509V_Pindef *Pinfo);

void R61509V_Write_Bus( u16 Com );
void R61509V_WriteData( u16 Dat );
void R61509V_WriteCMD( u16 addr, u16 Data );		//向lcd写入指令数据
u16 R61509V_ReadData( void );					//从LCD读取数据
void R61509V_SetDrawWindow( t_Point top_p, t_Point bottom_p );	//LCD 屏显示扫描方式

void R61509V_DrawDotest(u16 x,u16 y, u16 color );
	

//void R61509V_DrawDot( t_Point StartPos, u16 color );		//画点
void R61509V_EndDrawDot(void);
void R61509V_SetGramAddress( t_Point point );			//设置当前操作坐标点
void R61509V_Setbackground( u32 TotalPix, u16 BackColor );	//设置背景颜色
void R61509V_SetDrawMode( void );
void R61509V_PowerOff( void );		//关闭LCD 电源
void R61509V_DispOff( void );			//关闭LCD显示( 黑屏?)
void R61509V_PowerOn( void );			//LCD 上电并初始化相关寄存器
void R61509V_DrawPixelEx( u16 x, u16 y, u16 color );
u16 R61509V_GetPixel( t_Sreen_Rotate ScrnRotat, t_PixCfg PixCfg, t_Point p );
void R61509V_DrawHLine( u16 x, u16 y, u16 Len, u16 color );
void R61509V_DrawVLine( u16 x, u16 y, u16 Len, u16 color );
void R61509V_SetDispalyRange( u16 StartLine, u16 LineNum );
void R61509V_DrawBox( t_Point Top, t_Point Bottom );				//同过顶点和底部坐标画一个矩形
//void R61509V_DrawLine( t_Point *pPointA, t_Point *pPointB, u16 color );				//AB 两个坐标画一条直线
//void R61509V_DrawRectangle( t_Point *pTop_p, t_Point *pBotton_p, u16 color );	//画一个矩形框
//void R61509V_DrawCircle( t_Point point, u16 R, u8 Filled, u16 color );				//画一个圆形框
void R61509V_DrawFullFillBuf( u8 *pdata, u16 color );												//全屏像素点单色填充显示
void R61509V_DrawFullFillColorBuf( u8 *pdata );
void R61509V_DrawRectangleFillBuf( t_LcdCfg *pcfg, t_Rect *pRect, u8 *pdata, u16 color );	//矩形像素点单色填充显示
void R61509V_DrawRectangleColorBuf( t_Rect *pRect, u8 *pdata );
void R61509V_Init( t_LcdCfg **pLcdpara );






//void R61509V_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);

void R61509V_Delay(u32 xms);
void R61509V_WriteData16(unsigned int dashuju);				//写16位数据	
void R61509V_WriteIndex16(unsigned int damingling);		//写16位命令索引编号

void R61509V_WriteCommand(unsigned int index,unsigned int Command);	//写完整控制命令

void R61509V_Initialize(void);					//按照主控芯片R61509V的power supply on sequence 进行配置
void R61509V_Clean(u16 COLOR); //清除屏幕函数
void R61509V_SetWindowAddress(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);//设置窗地址
void R61509V_CHINESE(unsigned char zimo[720],unsigned int backcolor);  //写入字符

void R61509V_DrawDot(u16 x,u16 y,u16 COLOR);			//画点
void R61509V_DrawDot_big(u16 x,u16 y,u16 color);	//画一个大点
void R61509V_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
void R61509V_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);						//AB 两个坐标画一条直线
void R61509V_DrawCircle(u16 x1,u16 y1, u16 R, u8 Filled, u16 color );		//画一个圆形框
void R61509V_DrawRectangle(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);			//画一个矩形框
void R61509V_ShowChar(u16 x,u16 y,u8 font,u8 num,u8 *Buffer);						//高通字库测试程序
void R61509V_ShowCharT(u16 x,u16 y,u8 num,u8 mode);
void R61509V_ShowEn(u16 x,u16 y,u32 num);















//void R61509V_PowerOn( void );
//void R61509V_SetDispalyRange(  u16 StartLine, u16 LineNum );

//void R61509V_Init( t_LcdCfg **pLcdpara );
//void R61509V_DrawHLine( u16 x,u16 y, u16 Len ,u16 color);
//void R61509V_DrawVLine( u16 x,u16 y, u16 Len ,u16 color);
//void R61509V_Setbackground(u32 TotalPix,u16 BackColor );


//void R61509V_DrawPixelEx( u16 x,u16 y, u16 color );

//void R61509V_PowerOff(void);
//void R61509V_DispOff(void);
//void R61509V_SetGramAddress( t_Point point );
//void R61509V_SetDrawWindow( t_Point top_p, t_Point bottom_p );
//void R61509V_SetDrawMode( void );
//void R61509V_DrawDot(t_Point StartPos,u16 color);
//void R61509V_EndDrawDot(void);

//#endif//LCD_61509_EN
#endif//_BSP_LCDR61509V_H_2016_
/************************************** The End Of FILE **************************************/
