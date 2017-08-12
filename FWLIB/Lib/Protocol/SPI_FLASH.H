#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#include "stm32f10x_type.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"

//#include	"stdio.h"				//用于printf
//#include	"string.h"			//用于printf
//#include	"stdarg.h"			//用于获取不确定个数的参数
//#include	"stdlib.h"			//malloc动态申请内存空间
//	
//#include	"stddef.h"
//#include	"stdint.h"

//#include "STM32F10x_BitBand.H"

//#include "STM32_SYSTICK.H"
//#include "STM32_SPI.H"

/*--------------------------------FLASH部分术语解析-----------------------------
Sector：比BLOCK更小的单位
Block：	大部分的FLASH都以64K为单位空间ERASE
Chip：	整片FLASH
Page：
例：(256-byte per page)(4K-byte per sector/16page per sector)(64K-byte per block/16-sector per block)(8-block per chip)
————————————————————————————————————————————*/
//typedef	enum _SPI_FLASH_PROCESS			//FLASH工作过程标志
//{
//	SPI_FLASH_WRITE		=	2,			//FLASH正在写状态
//	SPI_FLASH_READ		=	3,			//FLASH正在读状态
//	SPI_FLASH_ERASE		=	4				//FLASH正在执行擦除工作
//}SPI_FLASH_PROCESS;

#define 	Dummy_Byte 0xA5								//FLASH空操作数
#define 	FlASH_BufferSize				1024	//最小扇区大小 4K


/******************************Flash命令定义参考MX25L4006E*******************************/
#define	Flash_WREN		(unsigned char)0x06				//写使能
#define	Flash_WRDI		(unsigned char)0x04				//写失能
#define	Flash_WRSR		(unsigned char)0x01				//写状态寄存器
#define	Flash_RDID		(unsigned char)0x9F				//读设备信息，3byte；第一字节为制造商代码，第二第三字节为芯片代码
#define	Flash_RDSR		(unsigned char)0x05				//读状态寄存器
#define	Flash_READ		(unsigned char)0x03				//读数据
#define	Flash_FREAD		(unsigned char)0x0B				//快速读取数据
#define	Flash_RDSFDP	(unsigned char)0x5A				//
#define	Flash_RES			(unsigned char)0xAB				//读器件地址
#define	Flash_REMS		(unsigned char)0x90				//读制造商代码和产品代码
#define	Flash_DREAD		(unsigned char)0x3B				//
#define	Flash_SE			(unsigned char)0x20				//扇区擦除
#define	Flash_BE			(unsigned char)0x52				//块擦除		也可以用0XD8
#define	Flash_CE			(unsigned char)0x60				//芯片擦除; 整片擦除; 也可以用0XC7
#define	Flash_WPAGE		(unsigned char)0x02				//写页面
#define	Flash_DP			(unsigned char)0xB9				//深度省电状态
#define	Flash_RDP			(unsigned char)0xAB				//退出深度掉电
	
//标志位
#define	WIP_Flag			(unsigned char)1<<0				//Write In Progress (WIP) flag
#define	WEL_Flag			(unsigned char)1<<1				//write enable latch (WEL) flag
	


typedef	enum _SPI_FLASH_REQUEST			//请求标志
{
	SPI_FLASH_qIDLE		=	0,			//FLASH空闲
	SPI_FLASH_qWRITE	=	1,			//FLASH写请求
	SPI_FLASH_qREAD		=	2,			//FLASH读请求
	SPI_FLASH_qSERASE	=	3,			//FLASH扇区擦除请求
	SPI_FLASH_qBERASE	=	4,			//FLASH块擦除请求
	SPI_FLASH_qCERASE	=	5				//FLASH整片擦除请求
}SPI_FLASH_REQUEST;

typedef enum _SPI_FLASH_STATUS			//FLASH状态标志
{
	SPI_FLASH_IDLE		=	0,			//FLASH空闲状态，可以读写
	SPI_FLASH_BUSY		=	1,			//FLASH忙状态
	SPI_FLASH_WRITE		=	2,			//FLASH执行写操作
	SPI_FLASH_READ		=	3,			//FLASH在读操作过程
	SPI_FLASH_ERASE		=	4				//FLASH正在执行擦除工作	
}SPI_FLASH_STATUS;
typedef	enum _SPI_FLASH_STEPS			//FLASH状态标志
{
	Step_IDLE			=	0,			//起始步骤，当前为空闲
	Step_WIP0			=	1,			//等待写操作完成，只有写操作完成后才可以进行写或者擦除操作
	Step_WEL			=	2,			//写使能
	Step_WRITE		=	3,			//写状态：		读取状态为1表示正在执行写操作，否则表示写入完成
	Step_WIP1			=	4,			//等待写操作完成，进行写操作或者擦除操作后，需要判断FLASH状态，写入完成后根据是否还有待写入数据进行相应的步骤跳转
	Step_READ			=	5,			//读操作步骤
	Step_ERASE		=	6				//擦除操作
}SPI_FLASH_STEPS;
typedef struct _SPI_FLASH_INF			//FLASH状态标志
{
	unsigned int	SPI_FLASH_PageSize;						//(256-byte per page)
	unsigned int	SPI_FLASH_SectorSize;					//(4K-byte per sector)
	unsigned int	SPI_FLASH_BlockSize;					//(64K-byte per block)
	
	unsigned int	SPI_FLASH_LenghToWrite;				//待写入字节长度
	unsigned int	SPI_FLASH_LenghToRead;				//待读取字节长度
	unsigned int	SPI_FLASH_SumToWrited;				//已经写入的数据个数
	unsigned int	SPI_FLASH_SumOfData;					//需要写入的数据个数
	
	unsigned int	SPI_FLASH_WriteAdrr;					//写入地址
	unsigned int	SPI_FLASH_ReadAdrr;						//读取地址
	unsigned int	SPI_FLASH_OffsetAdrr;					//偏移地址---一页256字节，用来存储程序信息--长度
	unsigned int	SPI_FLASH_SumReceived;				//总共接收到的数据个数	
	unsigned int	SPI_FLASH_ISPStartAddr;				//烧录时的起始地址
	
	unsigned int	SPI_FLASH_SectorAdrr;					//扇区地址---擦除扇区时使用
	unsigned int	SPI_FLASH_BlockAdrr;					//块地址-----块擦除时使用
	
	unsigned char MISO_Buffer[FlASH_BufferSize];
	unsigned char MOSI_Buffer[FlASH_BufferSize];
	
	unsigned char SPI_FLASH_ReadFlg;						//SPI_FLASH读数标志，如果SPI_FLASH_ReadFlg==1：读到数据，SPI_FLASH_ReadFlg==0，未读到数据
	
	SPI_FLASH_STATUS	SPI_FLASH_Staus;					//FLASH状态
	SPI_FLASH_STEPS		SPI_FLASH_Steps;					//FLASH执行步骤
	SPI_FLASH_REQUEST	SPI_FLASH_Request;				//FLASH操作请求类型	
}SPI_FLASH_INF;

//----SPI管脚配置--------------------------------------------------------------
typedef struct	_SPI_Flash
{
	//____________端口定义
	SPI_TypeDef *			SPIx;				//SPI接口配置 x=0、1、2、3、

	//____________SPI_CS定义
	GPIO_TypeDef* 	SPI_CS_PORT;		//GPIOX
	unsigned short 	SPI_CS_PIN;			//GPIO_Pin_x

	//____________SPI_CLK定义
	GPIO_TypeDef* 	SPI_CLK_PORT;		//GPIOX
	unsigned short 	SPI_CLK_PIN;		//GPIO_Pin_x

	//____________SPI_MISO定义
	GPIO_TypeDef* 	SPI_MISO_PORT;	//GPIOX
	unsigned short 	SPI_MISO_PIN;		//GPIO_Pin_x

	//____________SPI_MOSI定义
	GPIO_TypeDef* 	SPI_MOSI_PORT;	//GPIOX
	unsigned short 	SPI_MOSI_PIN;		//GPIO_Pin_x
	
	//____________SPI_波特率
	unsigned short 	SPI_BaudRatePrescaler_x;		//x=2/4/8/16/32/64/128/256
	
	unsigned char SPI_Flash_NSS_CsFlg;									//如果使用纯硬件SPI（含CS脚），	SPI_Flash_NSS_CsFlg=1，		否则SPI_Flash_NSS_CsFlg=0；此由SPI_Flash_ConfigurationNR根据SPI管脚判断设置此值
	unsigned char SPI_Flash_USER_PotFlg;								//如果使用硬件SPI接口，				SPI_Flash_USER_PotFlg=1，	否则SSPI_Flash_USER_PotFlg=0；此由SPI_Flash_ConfigurationNR根据SPI管脚判断设置此值
	unsigned char SPI_Flash_USER_DMAFlg;								//如果使用DMA，								SPI_Flash_USER_DMAFlg=1，	否则SPI_Flash_USER_DMAFlg=0；此由SPI_FLASH_ConfigurationDMA根据SPI配置判断设置此值
	//------------配置完成后读取FLASH_ID确认FLASH型号后自动补充以下数据
	
	//SPI_FLASH状态值-----由软件自动设置内部参数
	SPI_FLASH_INF SPI_FLASH_Info;
	
}SPI_FLASH_TypeDef;

	

typedef	enum _SPI_FLASH_CLASS		//FLASH_MX25L4006E
{
	ChipSize		=	0x01,
	SectorSize	=	4*1024,						//扇区大小4K Byte
	BlockSize		=	64*1024,					//块大小64K
	PageSize		=	256,							//页大小256 Byte
	
	MX25L4006E_PageSize		=	256,		//(256-byte per page)
	MX25L4006E_SectorSize,					//(4K-byte per sector)
	MX25L4006E_BlockSize,						//(64K-byte per block)
}SPI_FLASH_CLASS;		


	

//void SPI_FLASH_Configuration(void);
//void SPI_FLASH_Server(void);

//------------配置函数
void SPI_FLASH_Conf(SPI_FLASH_TypeDef *SPI_Conf);			//SPI参数设置
//---------------------------------------------------------------------------------

void SPI_FLASH_ConfigurationNR(SPI_FLASH_TypeDef *SPI_Conf);			//普通SPI接口配置--未开中断和DMA
void SPI_FLASH_ConfigurationDMA(SPI_FLASH_TypeDef *SPI_Conf);			//SPI_FLASH_DMA方式配置
void SPI_FLASH_GetInfo(SPI_FLASH_TypeDef *SPI_Conf);							//获取FLASH信息---根据ID确定FLASH型号，配置页大小，扇区大小，块大小参数
//------------SPI_FLASH_Process
void SPI_FLASH_Process	(SPI_FLASH_TypeDef *SPI_Conf);			//FLASH数据处理：所有的FLASH对外操作接口
//------------数据读写函数
void SPI_FLASH_PageWrite		(SPI_FLASH_TypeDef *SPI_Conf,u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);		//FLASH写一页数据
void SPI_FLASH_BufferWrite	(SPI_FLASH_TypeDef *SPI_Conf,u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);		//FLASH写缓冲数据
void SPI_FLASH_BufferRead		(SPI_FLASH_TypeDef *SPI_Conf,u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);			//SPI读缓冲区

//------------擦除函数
void SPI_FLASH_SectorErase	(SPI_FLASH_TypeDef *SPI_Conf,u32 SectorAddr);			//Flash扇区擦除Fast erase time: 60ms(typ.)/sector (4K-byte per sector) ; 0.7s(typ.)/block (64K-byte per block)
void SPI_FLASH_BulkErase		(SPI_FLASH_TypeDef *SPI_Conf,u32 BulkAddr);				//Flash块擦除Fast erase time: 60ms(typ.)/sector (4K-byte per sector) ; 0.7s(typ.)/block (64K-byte per block)
void SPI_FLASH_ChipErase		(SPI_FLASH_TypeDef *SPI_Conf);										//FLASH整片擦除


//************************************以下为内部调用函数

//------------控制函数
void SPI_FLASH_ENALBE		(SPI_FLASH_TypeDef 	*SPI_Conf);								//SPI_FLASH_使能
void SPI_FLASH_DISALBE	(SPI_FLASH_TypeDef 	*SPI_Conf);								//SPI_FLASH_关闭
//------------
u8 SPI_USER_SendData(SPI_FLASH_TypeDef *SPI_Conf,u8 byte);			//自定义写单字节数据
u8 	SPI_FLASH_WriteReadByte	(SPI_FLASH_TypeDef *SPI_Conf,u8 byte);			//Flash发送一字节并读取一字节数据
//------------
void SPI_FLASH_WriteEnable	(SPI_FLASH_TypeDef *SPI_Conf);							//0x06写使能
void SPI_FLASH_WriteDisable	(SPI_FLASH_TypeDef *SPI_Conf);							//0x写保护--写禁止
void SPI_FLASH_WaitForWriteEnd(SPI_FLASH_TypeDef *SPI_Conf);					//等待FLASH写完成
//------------状态函数
void SPI_FLASH_ReadRequest(SPI_FLASH_TypeDef *SPI_Conf);							//获取FLASH操作请求
u8 SPI_FLASH_ReadStatus(SPI_FLASH_TypeDef *SPI_Conf);									//读Flash状态寄存器
void SPI_FLASH_WriteStatus(SPI_FLASH_TypeDef *SPI_Conf,u8 Status);		//写Flash状态寄存器
//------------
u32 SPI_FLASH_ReadID(SPI_FLASH_TypeDef *SPI_Conf);										//读FlashID


//------------其它--暂未调试
void SPI_FLASH_StartReadSequence(SPI_FLASH_TypeDef *SPI_Conf,u32 ReadAddr);	//开始读数据
u16 SPI_FLASH_SendHalfWord(SPI_FLASH_TypeDef *SPI_Conf,u16 HalfWord);				//发送16位数据
//----------------------------------------------------------------------
//void SPI_FLASH_WriteCommand(SPI_FLASH_TypeDef *SPI_Conf,u8 Flash_Command);				//写命令


#endif






































































////#ifndef __SPI_FLASH_H
////#define __SPI_FLASH_H

//////#include "STM32_USART.H"
//////#include "STM32_TIM.H"
//////#include "STM32_DMA.H"
//////#include "STM32_PWM.H"
//////#include "STM32_GPIO.H"
//////#include "STM32_ADC.H"
//////#include "STM32_DAC.H"
//////#include "STM32_SPI.H"

////#include "stm32f10x_type.h"
////#include "stm32f10x_gpio.h"
////#include "stm32f10x_spi.h"

////#include	"stdio.h"				//用于printf
////#include	"string.h"			//用于printf
////#include	"stdarg.h"			//用于获取不确定个数的参数
////#include	"stdlib.h"			//malloc动态申请内存空间
////	
////#include	"stddef.h"
////#include	"stdint.h"

////#include "STM32F10x_BitBand.H"

//////#include "STM32_SYSTICK.H"
//////#include "STM32_SPI.H"

/////*--------------------------------FLASH部分术语解析-----------------------------
////Sector：比BLOCK更小的单位
////Block：	大部分的FLASH都以64K为单位空间ERASE
////Chip：	整片FLASH
////Page：
////例：(256-byte per page)(4K-byte per sector/16page per sector)(64K-byte per block/16-sector per block)(8-block per chip)
////————————————————————————————————————————————*/
////typedef	enum _SPI_FLASH_PROCESS			//FLASH工作过程标志
////{
////	SPI_FLASH_WRITE		=	2,			//FLASH正在写状态
////	SPI_FLASH_READ		=	3,			//FLASH正在读状态
////	SPI_FLASH_ERASE		=	4				//FLASH正在执行擦除工作
////}SPI_FLASH_PROCESS;
////typedef	enum _SPI_FLASH_STATUS			//FLASH状态标志
////{
////	SPI_FLASH_IDLE		=	0,			//FLASH空闲状态，可以读写
////	SPI_FLASH_BUSY		=	1,			//FLASH忙状态，正在执行读写操作
////	SPI_FLASH_WEL			=	2,			//可写标志：读取状态为1表示可以进行写操作，0表示不可以进行写操作
////	SPI_FLASH_WIP			=	3,			//写状态：		读取状态为1表示正在执行写操作，否则表示写入完成
////}SPI_FLASH_STATUS;
////typedef	enum _SPI_FLASH_REQUEST			//请求标志
////{
////	SPI_FLASH_qIDLE		=	0,			//FLASH空闲状态，可以读写
////	SPI_FLASH_qBUSY		=	1,			//FLASH忙状态，正在执行读写操作
////	SPI_FLASH_qWRITED	=	2,			//FLASH检查是否写完成/是否在写状态（在写或者擦除操作后，需要检查FLASH是否完成写操作或者在其它操作前需要检查FLASH是否正在进行写操作）
////	SPI_FLASH_qWRITEN	=	3				//FLASH检查是否可以进行写操作
////}SPI_FLASH_REQUEST;

//////----SPI管脚配置--------------------------------------------------------------
////typedef struct	_SPI_Flash
////{
////	//____________端口定义
////	SPI_TypeDef *			SPIx;				//SPI接口配置 x=0、1、2、3、

////	//____________SPI_CS定义
////	GPIO_TypeDef* 	SPI_CS_PORT;		//GPIOX
////	unsigned short 	SPI_CS_PIN;			//GPIO_Pin_x

////	//____________SPI_CLK定义
////	GPIO_TypeDef* 	SPI_CLK_PORT;		//GPIOX
////	unsigned short 	SPI_CLK_PIN;		//GPIO_Pin_x

////	//____________SPI_MISO定义
////	GPIO_TypeDef* 	SPI_MISO_PORT;	//GPIOX
////	unsigned short 	SPI_MISO_PIN;		//GPIO_Pin_x

////	//____________SPI_MOSI定义
////	GPIO_TypeDef* 	SPI_MOSI_PORT;	//GPIOX
////	unsigned short 	SPI_MOSI_PIN;		//GPIO_Pin_x
////	
////	//____________SPI_波特率
////	unsigned short 	SPI_BaudRatePrescaler_x;		//x=2/4/8/16/32/64/128/256
////	
////	unsigned char SPI_Flash_NSS_CsFlg;									//如果使用纯硬件SPI（含CS脚），	SPI_Flash_NSS_CsFlg=1，		否则SPI_Flash_NSS_CsFlg=0；此由SPI_Flash_ConfigurationNR根据SPI管脚判断设置此值
////	unsigned char SPI_Flash_USER_PotFlg;								//如果使用硬件SPI接口，				SPI_Flash_USER_PotFlg=1，	否则SSPI_Flash_USER_PotFlg=0；此由SPI_Flash_ConfigurationNR根据SPI管脚判断设置此值
////	//------------配置完成后读取FLASH_ID确认FLASH型号后自动补充以下数据
////	unsigned int	SPI_FLASH_PageSize;						//(256-byte per page)
////	unsigned int	SPI_FLASH_SectorSize;					//(4K-byte per sector)
////	unsigned int	SPI_FLASH_BlockSize;					//(64K-byte per block)
////	
////	SPI_FLASH_PROCESS	SPI_FLASH_Process;				//FLASH工作过程标志
////	SPI_FLASH_STATUS 	SPI_FLASH_Status;					//FLASH工作状态标志//根据FLASH当前的状态对FLASH进行操作
////	SPI_FLASH_REQUEST	SPI_FLASH_Request;				//请求标志---根据请求标志检测FLASH状态，
////	
////}SPI_FLASH_TypeDef;

////	

////typedef	enum _SPI_FLASH_CLASS		//FLASH_MX25L4006E
////{
////	ChipSize		=	0x01,
////	SectorSize	=	4*1024,		//扇区大小4K Byte
////	BlockSize		=	64*1024,	//块大小64K
////	PageSize		=	256,		//页大小256 Byte
////	
////	MX25L4006E_PageSize		=	256,		//(256-byte per page)
////	MX25L4006E_SectorSize,					//(4K-byte per sector)
////	MX25L4006E_BlockSize,						//(64K-byte per block)
////	
////	
////	sat
////}SPI_FLASH_CLASS;		

/////******************************Flash命令定义参考MX25L4006E*******************************/
////#define	Flash_WREN		(unsigned char)0x06				//写使能
////#define	Flash_WRDI		(unsigned char)0x04				//写失能
////#define	Flash_WRSR		(unsigned char)0x01				//写状态寄存器
////#define	Flash_RDID		(unsigned char)0x9F				//读设备信息，3byte；第一字节为制造商代码，第二第三字节为芯片代码
////#define	Flash_RDSR		(unsigned char)0x05				//读状态寄存器
////#define	Flash_READ		(unsigned char)0x03				//读数据
////#define	Flash_FREAD		(unsigned char)0x0B				//快速读取数据
////#define	Flash_RDSFDP	(unsigned char)0x5A				//
////#define	Flash_RES			(unsigned char)0xAB				//读器件地址
////#define	Flash_REMS		(unsigned char)0x90				//读制造商代码和产品代码
////#define	Flash_DREAD		(unsigned char)0x3B				//
////#define	Flash_SE			(unsigned char)0x20				//扇区擦除
////#define	Flash_BE			(unsigned char)0x52				//块擦除		也可以用0XD8
////#define	Flash_CE			(unsigned char)0x60				//芯片擦除; 整片擦除; 也可以用0XC7
////#define	Flash_WPAGE		(unsigned char)0x02				//写页面
////#define	Flash_DP			(unsigned char)0xB9				//深度省电状态
////#define	Flash_RDP			(unsigned char)0xAB				//退出深度掉电
////	
//////标志位
////#define	WIP_Flag			(unsigned char)1<<0				//Write In Progress (WIP) flag
////#define	WEL_Flag			(unsigned char)1<<1				//write enable latch (WEL) flag
////	

////void SPI_FLASH_Configuration(void);
////void SPI_FLASH_Server(void);

//////------------配置函数
////void SPI_FLASH_Conf(SPI_FLASH_TypeDef *SPI_Conf);			//SPI参数设置
//////---------------------------------------------------------------------------------

////void SPI_FLASH_ConfigurationNR(SPI_FLASH_TypeDef *SPI_Conf);			//普通SPI接口配置--未开中断和DMA
////void SPI_FLASH_GetInfo(SPI_FLASH_TypeDef *SPI_Conf);							//获取FLASH信息---根据ID确定FLASH型号，配置页大小，扇区大小，块大小参数

//////------------数据读写函数
////void SPI_FLASH_PageWrite		(SPI_FLASH_TypeDef *SPI_Conf,u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);		//FLASH写一页数据
////void SPI_FLASH_BufferWrite	(SPI_FLASH_TypeDef *SPI_Conf,u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);		//FLASH写缓冲数据
////void SPI_FLASH_BufferRead		(SPI_FLASH_TypeDef *SPI_Conf,u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);			//SPI读缓冲区

//////------------擦除函数
////void SPI_FLASH_SectorErase	(SPI_FLASH_TypeDef *SPI_Conf,u32 SectorAddr);			//Flash扇区擦除Fast erase time: 60ms(typ.)/sector (4K-byte per sector) ; 0.7s(typ.)/block (64K-byte per block)
////void SPI_FLASH_BulkErase		(SPI_FLASH_TypeDef *SPI_Conf);										//Flash块擦除Fast erase time: 60ms(typ.)/sector (4K-byte per sector) ; 0.7s(typ.)/block (64K-byte per block)
////void SPI_FLASH_ChipErase		(SPI_FLASH_TypeDef *SPI_Conf);										//FLASH整片擦除


//////************************************以下为内部调用函数

//////------------控制函数
////void SPI_FLASH_ENALBE		(SPI_FLASH_TypeDef 	*SPI_Conf);								//SPI_FLASH_使能
////void SPI_FLASH_DISALBE	(SPI_FLASH_TypeDef 	*SPI_Conf);								//SPI_FLASH_关闭
//////------------
////u8 	SPI_FLASH_WriteReadByte(SPI_FLASH_TypeDef *SPI_Conf,u8 byte);			//Flash发送一字节并读取一字节数据
//////------------
////void SPI_FLASH_WriteEnable(SPI_FLASH_TypeDef *SPI_Conf);							//0x06写使能
////void SPI_FLASH_WriteDisable(SPI_FLASH_TypeDef *SPI_Conf);							//0x写保护--写禁止
////void SPI_FLASH_WaitForWriteEnd(SPI_FLASH_TypeDef *SPI_Conf);					//等待FLASH写完成
//////------------状态函数
////u8 SPI_FLASH_ReadStatus(SPI_FLASH_TypeDef *SPI_Conf);									//读Flash状态寄存器
////void SPI_FLASH_WriteStatus(SPI_FLASH_TypeDef *SPI_Conf,u8 Status);		//写Flash状态寄存器
//////------------
////u32 SPI_FLASH_ReadID(SPI_FLASH_TypeDef *SPI_Conf);										//读FlashID


//////------------其它--暂未调试
////void SPI_FLASH_StartReadSequence(SPI_FLASH_TypeDef *SPI_Conf,u32 ReadAddr);	//开始读数据
////u16 SPI_FLASH_SendHalfWord(SPI_FLASH_TypeDef *SPI_Conf,u16 HalfWord);				//发送16位数据
//////----------------------------------------------------------------------
//////void SPI_FLASH_WriteCommand(SPI_FLASH_TypeDef *SPI_Conf,u8 Flash_Command);				//写命令


////#endif

