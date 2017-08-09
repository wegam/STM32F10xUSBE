#ifndef __STM32_LCD_H
#define __STM32_LCD_H


//#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
//#include "stm32f10x_nvic.h"
//#include "stm32f10x_spi.h"

//#include "STM32_GPIO.H"


//#include	"stdio.h"			//用于printf
//#include	"string.h"			//用于printf
//#include	"stdarg.h"			//用于获取不确定个数的参数
//#include	"stdlib.h"			//malloc动态申请内存空间


#define SD_CLK_SpeedL	0
#define SD_CLK_SpeedH	1

#define LCD_SD_CS_PORT				GPIOC
#define LCD_SD_CS_PIN					GPIO_Pin_6	

extern unsigned char STM32_LCD_Steep;			//程序运行阶段--LCD部分
extern unsigned char STM32_SD_Steep;			//程序运行阶段--SD卡部分

typedef enum
{
//#define 	STM32_SD_CMD0 (unsigned char)0x40;
//Class0 :(卡的识别、初始化等基本命令集)
//Class2 (读卡命令集):
//Class4(写卡命令集) :
//Class5 (擦除卡命令集):
//Class6(写保护命令集):
//class7：卡的锁定，解锁功能命令集
//class8：申请特定命令集。
//class10 －11 ：保留
//其中　class1,    class3,class9：SPI模式不支持
	
	//**************Class0
	STM32_SD_CMD0=(unsigned char)0x40,			//Class0	复位SD卡
	STM32_SD_CMD1,													//Class0	读OCR寄存器
	STM32_SD_CMD2,													//
	STM32_SD_CMD3,													//
	STM32_SD_CMD4,													//
	STM32_SD_CMD5,													//
	STM32_SD_CMD6,													//
	STM32_SD_CMD7,													//
	STM32_SD_CMD8,													//
	STM32_SD_CMD9,													//Class0	读CSD寄存器
	STM32_SD_CMD10,													//Class0	读CID寄存器
	STM32_SD_CMD11,													//
	STM32_SD_CMD12,													//Class0	停止读多块时的数据传输
	STM32_SD_CMD13,													//Class0	读 Card_Status 寄存器
	STM32_SD_CMD14,													//
	STM32_SD_CMD15,													//
	//**************Class2
	STM32_SD_CMD16,													//Class2	设置块的长度
	STM32_SD_CMD17,													//Class2	读单块
	STM32_SD_CMD18,													//Class2	读多块,直至主机发送CMD12为止
	//**************Class3
	STM32_SD_CMD19,													//
	STM32_SD_CMD20,													//
	STM32_SD_CMD21,													//
	STM32_SD_CMD22,													//
	STM32_SD_CMD23,													//
	//**************Class4
	STM32_SD_CMD24,													//Class4	写单块
	STM32_SD_CMD25,													//Class4	写多块
	STM32_SD_CMD26,													//
	STM32_SD_CMD27,													//Class4	写CSD寄存器 
	STM32_SD_CMD28,													//Class6	设置写保护块的地址
	STM32_SD_CMD29,													//Class6	擦除写保护块的地址
	STM32_SD_CMD30,													//Class6	Ask the card for the status of the write protection bits
	STM32_SD_CMD31,													//
	STM32_SD_CMD32,													//Class5	设置擦除块的起始地址
	STM32_SD_CMD33,													//Class5	设置擦除块的终止地址
	STM32_SD_CMD34,													//
	STM32_SD_CMD35,													//
	STM32_SD_CMD36,													//
	STM32_SD_CMD37,													//
	STM32_SD_CMD38,													//Class5	擦除所选择的块
	STM32_SD_CMD39,													//
	STM32_SD_CMD40,													//
	STM32_SD_CMD41,													//
	STM32_SD_CMD42,													//
	STM32_SD_CMD43,													//
	STM32_SD_CMD44,													//
	STM32_SD_CMD45,													//
	STM32_SD_CMD46,													//
	STM32_SD_CMD47,													//
	STM32_SD_CMD48,													//
	STM32_SD_CMD49,													//
	STM32_SD_CMD50,													//
	STM32_SD_CMD51,													//
	STM32_SD_CMD52,													//
	STM32_SD_CMD53,													//
	STM32_SD_CMD54,													//
	STM32_SD_CMD55,													//
	STM32_SD_CMD56,													//
	STM32_SD_CMD57,													//
	STM32_SD_CMD58,													//
	STM32_SD_CMD69,													//
	STM32_SD_CMD60,													//
	STM32_SD_CMD61													//
}STM32_SD_CMD_Typedef;

extern void STM32_LCD_Configuration(void);
extern void STM32_LCD_Server(void);
void STM32_LCD_PinConf(void);
void TM1618_PinSet(void);
void SSD1963_PinSet(void);
void TM1618_DIS(void);

extern void STM32_LCD_SPI_Configuration(char Speed);		//SPI接口配置
extern void STM32_SD_CS(char Status);						//SD_CS脚控制
extern void STM32_SD_SendByte(u16 Data);					//发送一字节数据
extern unsigned char STM32_SD_ReceiveByte(void);
extern void STM32_SD_SendData(unsigned int BufferSize);		//DMA方式向SD写入数据，开启DMA发送和接收 
extern unsigned char STM32_SD_Command(STM32_SD_CMD_Typedef CMD,u32 arg,u8 crc);
//unsigned char STM32_SD_Command(unsigned char *STM32_LCD_SD_CMD);
extern void STM32_SD_GetCID(void);			//获取CID信息，返回128位，16字节
extern u8 STM32_SD_GetCSD(void);			//获取CSD信息，返回128位，16字节
extern u32 STM32_SD_GetCapacity(void);	//获取SD卡的容量
extern u8 STM32_SD_ReadSingleBlock(u32 sector);//读SD卡的一个block
extern unsigned char STM32_SD_Init(void);
unsigned int LCD_PrintfString(u16 x,u16 y,u8 font,const char *format,...);			//后边的省略号就是可变参数
#endif
