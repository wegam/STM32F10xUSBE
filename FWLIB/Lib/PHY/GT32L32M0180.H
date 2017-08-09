/******************************** User_library *********************************
* 文件名 	: STM32_GT32L32M0180.H
* 作者   	: wegam@sina.com
* 版本   	: V
* 日期   	: 2017/01/15
* 说明   	: 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#ifndef __GT32L32M0180_H
#define __GT32L32M0180_H

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_nvic.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_dma.h"


//#include "STM32_WOW.H"		//使用位绑定

#include	"stdio.h"				//用于printf
#include	"string.h"			//用于printf
#include	"stdarg.h"			//用于获取不确定个数的参数
#include	"stdlib.h"			//malloc动态申请内存空间


/* Private define ------------------------------------------------------------*/
//____________项目配置
#define	STM32_GT32L32M0180
//#define	GT32L32_BITBAND				//位绑定
#define	GT32L32_SPIPORT_EN		//使用SPI接口
#define	STM32_LCD_GT32L32M0180
#define	HC_PL010V11			//LCD稳重板


//____________SPI速度定义
#define GT32L32_CLK_SpeedL	0
#define GT32L32_CLK_SpeedH	1

//#ifdef STM32_LCD_GT32L32M0180

	//____________端口定义
//	#define GT32L32_SPI_PORT			SPI1

//	#define GT32L32_SPI_Periph		RCC_APB2Periph_SPI1
//	//____________SPI_CS定义
//	#define GT32L32_CS_PORT				GPIOB
//	#define GT32L32_CS_PIN				GPIO_Pin_14
//	#define GT32L32_CS_Periph			RCC_APB2Periph_GPIOB
//	//____________SPI_CLK定义
//	#define GT32L32_CLK_PORT			GPIOA
//	#define GT32L32_CLK_PIN				GPIO_Pin_5
//	#define GT32L32_CLK_Periph		RCC_APB2Periph_GPIOA
//	//____________SPI_MISO定义
//	#define GT32L32_MISO_PORT			GPIOA
//	#define GT32L32_MISO_PIN			GPIO_Pin_6
//	#define GT32L32_MISO_Periph		RCC_APB2Periph_GPIOA
//	//____________SPI_MOSI定义
//	#define GT32L32_MOSI_PORT			GPIOA
//	#define GT32L32_MOSI_PIN			GPIO_Pin_7
//	#define GT32L32_MOSI_Periph		RCC_APB2Periph_GPIOA

	//____________GPIO控制定义


		
//		//____________SPI_CS控制定义
//		#define GT32L32_CS_DisSelect				GPIO_SetBits(GT32L32_CS_PORT,				GT32L32_CS_PIN)			//取消片选
//		#define GT32L32_CS_Select						GPIO_ResetBits(GT32L32_CS_PORT,			GT32L32_CS_PIN)		//使能片选
//	//____________SPI_CLK/SCLK控制定义
//		#define GT32L32_CLK_L								GPIO_ResetBits(GT32L32_CLK_PORT,	GT32L32_CLK_PIN)		//低电平
//		#define GT32L32_CLK_H								GPIO_SetBits(GT32L32_CLK_PORT,		GT32L32_CLK_PIN)			//高电平
//	//____________SPI_MISO/SO控制定义
//		#define GT32L32_SO									GPIO_ReadOutputDataBit(GT32L32_MISO_PORT,GT32L32_MISO_PIN)
//	//____________SPI_MOSI/SI控制定义
//		#define GT32L32_SI_L								GPIO_ResetBits(GT32L32_MOSI_PORT,	GT32L32_MOSI_PIN)		//低电平
//		#define GT32L32_SI_H								GPIO_SetBits(GT32L32_MOSI_PORT,		GT32L32_MOSI_PIN)			//高电平



//#else
//	//____________端口定义
//	#define GT32L32_SPI_PORT			SPI1
//	#define GT32L32_SPI_Periph		RCC_APB2Periph_SPI1
//	//____________SPI_CS定义
//	#define GT32L32_CS_PORT				GPIOA
//	#define GT32L32_CS_PIN				GPIO_Pin_4
//	#define GT32L32_CS_Periph			RCC_APB2Periph_GPIOB
//	//____________SPI_CLK定义
//	#define GT32L32_CLK_PORT			GPIOA
//	#define GT32L32_CLK_PIN				GPIO_Pin_5
//	#define GT32L32_CLK_Periph		RCC_APB2Periph_GPIOA
//	//____________SPI_MISO定义
//	#define GT32L32_MISO_PORT			GPIOA
//	#define GT32L32_MISO_PIN			GPIO_Pin_6
//	#define GT32L32_MISO_Periph		RCC_APB2Periph_GPIOA
//	//____________SPI_MOSI定义
//	#define GT32L32_MOSI_PORT			GPIOA
//	#define GT32L32_MOSI_PIN			GPIO_Pin_7
//	#define GT32L32_MOSI_Periph		RCC_APB2Periph_GPIOA

//	//____________GPIO控制定义
//	#ifdef		GT32L32_BITBAND		//*************位绑定
//		//____________SPI_CS控制定义
//			#define GT32L32_CS_DisSelect				PA4=1			//取消片选
//			#define GT32L32_CS_Select						PA4=0			//片选
//		//____________SPI_CLK/SCLK控制定义
//			#define GT32L32_CLK_L								PA5=0			//低电平
//			#define GT32L32_CLK_H								PA5=1			//高电平
//		//____________SPI_MISO/SO控制定义
//			#define GT32L32_SO									PA6in
//		//____________SPI_MOSI/SI控制定义
//			#define GT32L32_SI_L								PA7=0			//低电平
//			#define GT32L32_SI_H								PA7=1			//高电平
//	#else		//*************未进行位绑定
//		//____________SPI_CS控制定义
//			#define GT32L32_CS_DisSelect				GPIO_SetBits(GT32L32_CS_PORT,				GT32L32_CS_PIN)			//取消片选
//			#define GT32L32_CS_Select						GPIO_ResetBits(GT32L32_CS_PORT,			GT32L32_CS_PIN)		//使能片选
//		//____________SPI_CLK/SCLK控制定义
//			#define GT32L32_CLK_L								GPIO_ResetBits(GT32L32_CLK_PORT,	GT32L32_CLK_PIN)		//低电平
//			#define GT32L32_CLK_H								GPIO_SetBits(GT32L32_CLK_PORT,		GT32L32_CLK_PIN)			//高电平
//		//____________SPI_MISO/SO控制定义
//			#define GT32L32_SO									GPIO_ReadOutputDataBit(GT32L32_MISO_PORT,GT32L32_MISO_PIN)
//		//____________SPI_MOSI/SI控制定义
//			#define GT32L32_SI_L								GPIO_ResetBits(GT32L32_MOSI_PORT,	GT32L32_MOSI_PIN)		//低电平
//			#define GT32L32_SI_H								GPIO_SetBits(GT32L32_MOSI_PORT,		GT32L32_MOSI_PIN)			//高电平
//	#endif	
//#endif

/* Private typedef -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
//----GPIO管脚配置--------------------------------------------------------------
typedef struct	_GT32L32_Pin
{
	//____________端口定义
	SPI_TypeDef *			sSPIx;				//SPI接口配置 x=0、1、2、3、

	//____________SPI_CS定义
	GPIO_TypeDef* 	sGT32L32_CS_PORT;
	unsigned short 	sGT32L32_CS_PIN;

	//____________SPI_CLK定义
	GPIO_TypeDef* 	sGT32L32_CLK_PORT;
	unsigned short 	sGT32L32_CLK_PIN;

	//____________SPI_MISO定义
	GPIO_TypeDef* 	sGT32L32_MISO_PORT;
	unsigned short 	sGT32L32_MISO_PIN;

	//____________SPI_MOSI定义
	GPIO_TypeDef* 	sGT32L32_MOSI_PORT;
	unsigned short 	sGT32L32_MOSI_PIN;
	
	//____________SPI_波特率
	unsigned short 	SPI_BaudRatePrescaler_x;		//x=2/4/8/16/32/64/128/256
	
}GT32L32_Init_TypeDef;

//----字符点阵字库基地址表------------------------------------------------------------
typedef enum
{
	ASCII_BaseAddr_None					=	(u32)0x000000,			//未配置地址
	
	ASCII_BaseAddr_zf5x7				=	(u32)0x100000,			//5x7点阵ASCII标准字符
	ASCII_BaseAddr_zfs7x8				=	(u32)0x100300,			//7x8点阵ASCII标准字符
	ASCII_BaseAddr_zfm7x8				=	(u32)0x100600,			//7x8点阵ASCII粗体字符
	ASCII_BaseAddr_zf6x12				=	(u32)0x100900,			//6x12点阵ASCII字符
	ASCII_BaseAddr_zfs8x16			=	(u32)0x100D80,			//8x16点阵ASCII标准字符
	ASCII_BaseAddr_zfm8x16			=	(u32)0x101580,			//8x16点阵ASCII粗体字符
	ASCII_BaseAddr_zf12x24			=	(u32)0x101B80,			//12x24点阵ASCII标准字符
	ASCII_BaseAddr_zfs16x32			=	(u32)0x102D80,			//16x32点阵ASCII标准字符
	ASCII_BaseAddr_zfm16x32			=	(u32)0x104580,			//16x32点阵ASCII粗体字符
	
	ASCII_BaseAddr_ar12					=	(u32)0x105D80,			//12点阵不等宽ASCII方头（Arial）字符
	ASCII_BaseAddr_ar16					=	(u32)0x106740,			//16点阵不等宽ASCII方头（Arial）字符
	ASCII_BaseAddr_ar24					=	(u32)0x107400,			//24点阵不等宽ASCII方头（Arial）字符
	ASCII_BaseAddr_ar32					=	(u32)0x108FC0,			//32点阵不等宽ASCII方头（Arial）字符
	
	ASCII_BaseAddr_TNR12				=	(u32)0x10C080,			//12点阵不等宽ASCII白正（Times New Roman）字符
	ASCII_BaseAddr_TNR16				=	(u32)0x10CA50,			//16点阵不等宽ASCII白正（Times New Roman）字符
	ASCII_BaseAddr_TNR24				=	(u32)0x10D740,			//24点阵不等宽ASCII白正（Times New Roman）字符
	ASCII_BaseAddr_TNR32				=	(u32)0x10F340,			//32点阵不等宽ASCII白正（Times New Roman）字符
	
	Digital_BaseAddr_sz14x28		=	(u32)0x112400,			//14x28数字符号字符
	Digital_BaseAddr_sz20x40		=	(u32)0x112748,			//20x40数字符号字符
	Digital_BaseAddr_sz28				=	(u32)0x112CE8,			//28点阵不等宽数字符号字符
	Digital_BaseAddr_sz40				=	(u32)0x113396,			//40点阵不等宽数字符号字符
	
	GB18030_BaseAddr_zf12x12		=	(u32)0x113D0E,			//12x12点阵GB18030字符
	GB18030_BaseAddr_zf16x16		=	(u32)0x194FDE,			//16x16点阵GB18030字符
	GB18030_BaseAddr_zf24x24		=	(u32)0x2743DE,			//24x24点阵GB18030字符
	GB18030_BaseAddr_zf32x32		=	(u32)0x47AE10,			//32x32点阵GB18030字符
	
	GB18030_BaseAddr_hz12x12		=	(u32)0x113D0E,			//12x12点阵GB18030汉字
	GB18030_BaseAddr_hz16x16		=	(u32)0x194FDE,			//16x16点阵GB18030汉字
	GB18030_BaseAddr_hz24x24		=	(u32)0x2743DE,			//24x24点阵GB18030汉字
	GB18030_BaseAddr_hz32x32		=	(u32)0x47AE10,			//32x32点阵GB18030汉字
	
	Unicode_To_GBK_BaseAddr			=	(u32)0x46A90E,			//Unicode->GBK转码表
	BIG5_To_GBK_BaseAddr				=	(u32)0x7F1E10,			//BIG5->GBK转码表
	
	BarCode_BaseAddr_12x27			=	(u32)0x478FD2,			//12x27条形码字符 EAN13
	BarCode_BaseAddr_16x20			=	(u32)0x479C7A,			//16x20条形码字符 CODE128
	
	AntennaCode_BaseAddr_12X12	=(u32)0x47AD32,				//12x12天线符号
	BatteryCode_BaseAddr_12X12	=(u32)0x47ADAA,				//12x12电池符号
	
	Reserved_BaseAddr_LSB				=(u32)0x7F8760,				//保留区低地址
	Reserved_BaseAddr_MSB				=(u32)0x7FFFFF,				//保留区高地址		
}	GT32L32_BaseAddr_TypeDef;		//字符点阵字库基地址表
//----错误列表------------------------------------------------------------
typedef enum
{
	GT32L32_Error_None					=	(u8)0x00,			//无错误
	GT32L32_Error_Addr					=	(u8)0x01,			//地址错误
	GT32L32_Error_ASCIICode			=	(u8)0x02,			//ASCII码错误
	GT32L32_Error_HzEmp					=	(u8)0x03,			//汉字内码低8位为0x7F,为空内容
	
}	GT32L32_Error_TypeDef;		//错误代码列表
//----消息结构体------------------------------------------------------------
typedef struct      /*Card Identification Data*/
{
	GT32L32_BaseAddr_TypeDef	GT32L32_BaseAddr;		//字符点阵字库基地址表
	GT32L32_Error_TypeDef			GT32L32_Error;			//错误列表
	u32	GT32L32_Address;													//字符地址
	u8	GT32L32_BufferSize;												//点阵数据长度
} GT32L32_Info_TypeDef;
/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */

void 	GT32L32_ConfigurationNR(GT32L32_Init_TypeDef *GT32L32_Init);				//普通SPI通讯方式配置
void 	GT32L32_DMA_ConfigurationNR(GT32L32_Init_TypeDef *GT32L32_Init);		//SPI-DMA通讯方式配置---20170722未完成
u8 GT32L32_ReadStatus(GT32L32_Init_TypeDef *GT32L32_Init);								//读取芯片状态


void 	GT32L32_PinConf(GT32L32_Init_TypeDef *GT32L32_Init);
void	GT32L32_Server(void);


u8  GT32L32_GetDig_Info(u8 Sequence,GT32L32_Info_TypeDef *GT32L32_Info);			//点阵数字符号字符
u8  GT32L32_GetASCII_Info(u8 ASCIICode,GT32L32_Info_TypeDef *GT32L32_Info);		//ASCII字符信息
u8	GT32L32_GetGB18030_Info(u16 word,GT32L32_Info_TypeDef *GT32L32_Info);			//GB18030汉字&字符信息
u8 	GT32L32_Get_Info(u16 word,GT32L32_Info_TypeDef *GT32L32_Info);						//获取字符地址和点阵长度



u32 GT32L32_GetAddress(u8 font, u8 c1, u8 c2, u8 c3, u8 c4);//获取地址
u32 GT32L32_GetBufferLen(u8 font, u8 c1, u8 c2, u8 c3, u8 c4);//获取长度
u16  GT32L32_ReadBuffer(GT32L32_Init_TypeDef *GT32L32_Init,u8 font,u16 word,unsigned char *Buffer);		//从字库中读数据函数


u32 GT32L32_GetGB18030_12(u8 c1, u8 c2, u8 c3, u8 c4);		//12x12点阵GB18030汉字&字符地址计算
u32 GT32L32_GetGB18030_16(u8 c1, u8 c2, u8 c3, u8 c4);		//16x16点阵GB18030汉字&字符地址计算
u32 GT32L32_GetGB18030_24(u8 c1, u8 c2, u8 c3, u8 c4);		//24x24点阵GB18030汉字&字符地址计算
u32 GT32L32_GetGB18030_32(u8 c1, u8 c2, u8 c3, u8 c4);		//32x32点阵GB18030汉字&字符地址计算
u32 GT32L32_GetBarCode_13(u8 * BAR_NUM);															//EAN13条形码调用程序
u32 GT32L32_GetBarCode_128(u8	*BAR_NUM,u8	flag);											//GB/T 18347-2001(CODE128)条形码调用程序
u32 GT32L32_GetAntennaCode_12X12(u8	NUM,u8 *GetBuffer);								//天线图标调用程序
u32 GT32L32_GetBatteryCode_12X12(u8	NUM,u8 *GetBuffer);								//电池图标调用程序

u16	GT32L32_U2G_13(u16 Unicode);																			//UNICODE转GBK码表映射算法,仅1&3字符区
u32 GT32L32_UNICODE_To_GBK(u16 unicode,u8 *GetBuffer);								//UNICODE转GBK码表映射算法（不含符号区,转换全部双字节区） 
u32 GT32L32_BIG5_To_GBK(u16 BIG5_Code,u8 *GetBuffer);									//BIG5转GBK转换算法




u8 GT32L32_ReadStatus(GT32L32_Init_TypeDef *GT32L32_Init);																					//读取芯片状态
void GT32L32_ChipErase(GT32L32_Init_TypeDef *GT32L32_Init);																					//探除用户区数据
u8  GT32L32_ReadBufferbac(GT32L32_Init_TypeDef *GT32L32_Init,u32 address,u16 len,unsigned char *Buffer);		//从字库中读数据函数
u8  GT32L32_ReadWriteByte(u8 Data);		//读写一个字节
void GT32L32_SendByte(GT32L32_Init_TypeDef *GT32L32_Init,u8 ByteData);		//发送一个字节数据,先发送高位
u8 GT32L32_ReadByte(GT32L32_Init_TypeDef *GT32L32_Init);		//读一个字节,先读高位



#endif

/*******************	wegam@sina.com	*******************/
/*********************	2017/01/21	*********************/
/**********************	END OF FILE	*********************/


