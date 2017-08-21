/******************************** User_library *********************************
* 文件名 	: SPI_FLASH.C
* 作者   	: wegam@sina.com
* 版本   	: V
* 日期   	: 2017/04/16
* 说明   	: 
********************************************************************************
SPI_FLASH使用功能测试
1）需要宏定义 SPI_FLASH
2）使用USB_TEST 板测试
3）需要宏定义SPI引脚
--------------------------------FLASH部分术语解析-----------------------------
Sector：比BLOCK更小的单位
Block：	大部分的FLASH都以64K为单位空间ERASE
Chip：	整片FLASH
Page：
例：(256-byte per page)(4K-byte per sector/16page per sector)(64K-byte per block/16-sector per block)(8-block per chip)
*******************************************************************************/

#include "SPI_FLASH.H"

#include "USART_ISP.H"

//#define USB_TEST_BD				//USB_TEST板
#define PD011				//PD011v1.0板
//#define SPI_FLASH_OSTL
//#define CMSIS_CDC_BD			//CMSIS-CDC板
#define USERSPI				//PD011v1.0板
#include "string.h"
//#include "stdlib.h"
//#include "stdio.h"

#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"

#include "STM32_PWM.H"
#include "STM32_SYS.H"
#include "STM32_SPI.H"
#include "STM32_GPIO.H"
#include "STM32_SYSTICK.H"
#include "STM32_USART.H"

#include "STM32F10x_BitBand.H"

//#define 	Dummy_Byte 0xA5					//FLASH空操作数
//#define		SPI_FLASH_PageSize   		0x100		//Flash页大小
//#define		FLASH_WriteAddress     	0x00
//#define		FLASH_ReadAddress      	FLASH_WriteAddress
//#define		FLASH_SectorToErase    	FLASH_WriteAddress
//#define 	FlASH_BufferSize				512
//#define 	USART_BufferSize				FlASH_BufferSize


u8 FLASH_Status1=0;
//u32 Temp = 0;
//SPI_FLASH_TypeDef	FLASH_Conf;

//ISP_Conf_TypeDef 	ISP_Conf;

//u16	FLASH_ChipEraseFlag=0;			//启动后擦除一次FLASH
//u16	RWF=0;			//读写标志位

//u32 FlashAddr=0;
//u32 FlashID=0;
//u8 FlashStatus=WEL_Flag;
//u8 RevBuffer[FlASH_BufferSize]={0};
////u8 SendBuffer[FlASH_BufferSize]={0x9F,0xFF,0XFF,0XFF};
////u8 testSbuffer[FlASH_BufferSize]={0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfA};
//u8 testRbuffer[FlASH_BufferSize]={0};

//u8 *testSbuffer=FLASH_Conf.SPI_FLASH_Info.MOSI_Buffer;

//u8 RxdBuffe[USART_BufferSize]={0};
//u8 RevBuffe[USART_BufferSize]={0};

//u8 TxdBuffe[USART_BufferSize]={0};
//u8 TevBuffe[USART_BufferSize]={0};

/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
//void SPI_FLASH_Configuration(void)
//{
//	SYS_Configuration();											//系统配置 STM32_SYS.H	
//	GPIO_DeInitAll();													//将所有的GPIO关闭----V20170605
//	PWM_OUT(TIM2,PWM_OUTChannel1,1,900);			//PWM设定-20161127版本
//	
//	USART_DMA_ConfigurationNR	(USART1,115200,(u32*)RxdBuffe,USART_BufferSize);	//USART_DMA配置--查询方式，不开中断
//	
//	SPI_FLASH_Conf(&FLASH_Conf);			//SPI设置
//	
//	ISP_Conf.USARTx=USART1;
//	
//	FlashStatus=WEL_Flag;	
//	SysTick_Configuration(50);							//系统嘀嗒时钟配置72MHz,单位为uS
//}

/*******************************************************************************
* 函数名		:
* 功能描述	:
* 输入		:
* 输出		:
* 返回 		:
*******************************************************************************/
//void SPI_FLASH_Server(void)
//{
//	u16 RxNum=0;
//	u32 Addr=0;
////	SPI_FLASH_Process(&FLASH_Conf);			//FLASH数据处理：所有的FLASH对外操作接口

//	RxNum=USART_ReadBufferIDLE			(USART1,(u32*)RevBuffe,(u32*)RxdBuffe);	//串口空闲模式读串口接收缓冲区，如果有数据，将数据拷贝到RevBuffer,并返回接收到的数据个数，然后重新将接收缓冲区地址指向RxdBuffer
////	Usart_ISP_CommandSend(&ISP_Conf,ISP_COMMAND_Get);	//串口编程发送命令程序
//	if(RxNum&&(FlashAddr<128*4*1024))
//	{		
////		SPI_FLASH_BufferWrite(&FLASH_Conf,	RevBuffe, 		FlashAddr, RxNum);	//FLASH写缓冲数据
////		SPI_FLASH_BufferRead(&FLASH_Conf,		testSbuffer,  FlashAddr, RxNum);
//		FlashAddr+=RxNum;
//	}
////	itoa(1234567890,TxdBuffe,10);//搜索10代表十进制
////	SPI_FLASH_Process(&FLASH_Conf);			//FLASH数据处理：所有的FLASH对外操作接口
//	if(Temp!=0)
//	{
//		Temp=0;

//	}
//	else
//	{
//		Temp=1;

//	}
//	
//	if(FLASH_ChipEraseFlag<=3000)
//	{
//		FLASH_ChipEraseFlag++;
//		if(FLASH_ChipEraseFlag==3000)
//		{
////			memset(testSbuffer,0xFF, FlASH_BufferSize);
////			SPI_FLASH_BufferWrite(&FLASH_Conf,	testSbuffer, 		0x00, FlASH_BufferSize);	//FLASH写缓冲数据
//			
////			FLASH_Conf.SPI_FLASH_Info.SPI_FLASH_Request=SPI_FLASH_qCERASE;		//请求整片擦除
////			SPI_FLASH_ChipErase(&FLASH_Conf);																	//FLASH整片擦除
//			
////			FLASH_Conf.SPI_FLASH_Info.SPI_FLASH_Request=SPI_FLASH_qSERASE;		//请求整片擦除
////			FLASH_Conf.SPI_FLASH_Info.SPI_FLASH_SectorAdrr=0;
//			
////			FLASH_Conf.SPI_FLASH_Info.SPI_FLASH_Request=SPI_FLASH_qBERASE;		//请求整片擦除
////			FLASH_Conf.SPI_FLASH_Info.SPI_FLASH_BlockAdrr=0;
////			FLASH_Conf.SPI_FLASH_Info.SPI_FLASH_Request=SPI_FLASH_qREAD;
////			FLASH_Conf.SPI_FLASH_Info.SPI_FLASH_LenghToRead=FlASH_BufferSize;

////			SPI_FLASH_ChipErase(&FLASH_Conf);												//FLASH整片擦除
//			
////			for(Addr=0;Addr<128*4*1024;)	//524288			//523776
////			{
////				SPI_FLASH_BufferRead(&FLASH_Conf,testRbuffer, Addr, FlASH_BufferSize);
////				Addr+=FlASH_BufferSize;
////			}
//			
////			for(Addr=0;Addr<128*4*1024;)	//524288			//523776
////			{
////				SPI_FLASH_BufferWrite(&FLASH_Conf,testSbuffer, Addr, FlASH_BufferSize);
////				Addr+=FlASH_BufferSize;
////			}
//			
//		
////		SPI_FLASH_BulkErase(&FLASH_Conf);										//FLASH整片擦除
////		SPI_FLASH_ChipErase(&FLASH_Conf);										//FLASH整片擦除
////		SPI_FLASH_SectorErase(&FLASH_Conf,FLASH_ReadAddress);	//Flash扇区擦除
//			
////			SPI_FLASH_BufferWrite(&FLASH_Conf,testSbuffer, FLASH_WriteAddress, FlASH_BufferSize);	//FLASH写缓冲数据
////			SPI_FLASH_BufferRead(&FLASH_Conf,testSbuffer, FLASH_WriteAddress, FlASH_BufferSize);
//		}
//	}
//	RWF++;
//	if(RWF==2000)
//	{
////		Usart_ISP_CommandSend(&ISP_Conf,ISP_COMMAND_Get);	//串口编程发送命令程序
//		Usart_ISP_CommandSend(&ISP_Conf,ISP_COMMAND_Go);	//串口编程发送命令程序
////		FLASH_Conf.SPI_FLASH_Info.SPI_FLASH_Request=SPI_FLASH_qREAD;
////		FLASH_Conf.SPI_FLASH_Info.SPI_FLASH_LenghToRead=FlASH_BufferSize;
////		SPI_FLASH_SectorErase(&FLASH_Conf,FLASH_SectorToErase);			//擦除扇区
////		SPI_FLASH_BulkErase(&FLASH_Conf);										//FLASH块擦除
////		SPI_FLASH_ChipErase(&FLASH_Conf);										//FLASH整片擦除

////		SPI_FLASH_BufferWrite(&FLASH_Conf,testSbuffer, FLASH_WriteAddress, FlASH_BufferSize);	//FLASH写缓冲数据
//		
//		
//	}
//	else if(RWF==3000)
//	{
//		RWF=0; 
////		SPI_FLASH_BufferRead(&FLASH_Conf,testRbuffer, FLASH_ReadAddress, FlASH_BufferSize);
//		
////		for(Addr=0;Addr<128*4*1024;)
////		{
////			SPI_FLASH_BufferRead(&FLASH_Conf,testRbuffer, Addr, FlASH_BufferSize);
////			Addr+=512;
////		}

//		
//	}
//}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void SPI_FLASH_Conf(SPI_FLASH_TypeDef *SPI_Conf)
{
//	#define USB_TEST_BD				//USB_TEST板
//	#define CMSIS_CDC_BD			//CMSIS-CDC板
//	GPIO_InitTypeDef GPIO_InitStructure;
	
	#ifdef	USB_TEST_BD
	SPI_Conf->SPIx=SPI2;
	SPI_Conf->SPI_CS_PORT=GPIOC;
	SPI_Conf->SPI_CS_PIN=GPIO_Pin_8;	
	SPI_Conf->SPI_BaudRatePrescaler_x=SPI_BaudRatePrescaler_256;
	
	#endif
	
	#ifdef	PD011
	SPI_Conf->SPIx=SPI1;
	SPI_Conf->SPI_CS_PORT=GPIOA;
	SPI_Conf->SPI_CS_PIN=GPIO_Pin_4;	
	SPI_Conf->SPI_BaudRatePrescaler_x=SPI_BaudRatePrescaler_256;
	
	#endif
	
	#ifdef	CMSIS_CDC_BD
	
	SPI_Conf->SPIx=SPI1;
	SPI_Conf->SPI_CS_PORT=GPIOA;
	SPI_Conf->SPI_CS_PIN=GPIO_Pin_4;	
	SPI_Conf->SPI_BaudRatePrescaler_x=SPI_BaudRatePrescaler_256;
	
	#endif
	
	#ifdef	USERSPI	
	
		#ifdef	USB_TEST_BD
		
		SPI_Conf->SPI_CS_PORT=GPIOC;
		SPI_Conf->SPI_CS_PIN=GPIO_Pin_8;	
		
		SPI_Conf->SPI_CLK_PORT=GPIOB;
		SPI_Conf->SPI_CLK_PIN=GPIO_Pin_13;
		
		SPI_Conf->SPI_MOSI_PORT=GPIOB;
		SPI_Conf->SPI_MOSI_PIN=GPIO_Pin_15;
		
		SPI_Conf->SPI_MISO_PORT=GPIOB;
		SPI_Conf->SPI_MISO_PIN=GPIO_Pin_14;

		
		#endif
		
		#ifdef	PD011
		SPI_Conf->SPI_CS_PORT=GPIOA;
		SPI_Conf->SPI_CS_PIN=GPIO_Pin_4;

		SPI_Conf->SPI_CLK_PORT=GPIOA;
		SPI_Conf->SPI_CLK_PIN=GPIO_Pin_5;
		
		SPI_Conf->SPI_MOSI_PORT=GPIOA;
		SPI_Conf->SPI_MOSI_PIN=GPIO_Pin_7;
		
		SPI_Conf->SPI_MISO_PORT=GPIOA;
		SPI_Conf->SPI_MISO_PIN=GPIO_Pin_6;

		
		#endif
		
		#ifdef	CMSIS_CDC_BD

		SPI_Conf->SPI_CS_PORT=GPIOA;
		SPI_Conf->SPI_CS_PIN=GPIO_Pin_4;

		SPI_Conf->SPI_CLK_PORT=GPIOA;
		SPI_Conf->SPI_CLK_PIN=GPIO_Pin_5;
		
		SPI_Conf->SPI_MOSI_PORT=GPIOA;
		SPI_Conf->SPI_MOSI_PIN=GPIO_Pin_7;
		
		SPI_Conf->SPI_MISO_PORT=GPIOA;
		SPI_Conf->SPI_MISO_PIN=GPIO_Pin_6;

		
		#endif
	GPIO_Configuration_OPP50(SPI_Conf->SPI_CS_PORT,SPI_Conf->SPI_CS_PIN);			SPI_Conf->SPI_CS_PORT->BSRR 		= SPI_Conf->SPI_CS_PIN;							//GPIO_SetBits(SPI_Conf->SPI_CS_PORT,SPI_Conf->SPI_CS_PIN);
	GPIO_Configuration_OPP50(SPI_Conf->SPI_CLK_PORT,SPI_Conf->SPI_CLK_PIN);		SPI_Conf->SPI_CLK_PORT->BSRR 		= SPI_Conf->SPI_CLK_PIN;						//GPIO_SetBits(SPI_Conf->SPI_CLK_PORT,SPI_Conf->SPI_CLK_PIN);
	GPIO_Configuration_OPP50(SPI_Conf->SPI_MOSI_PORT,SPI_Conf->SPI_MOSI_PIN);	SPI_Conf->SPI_MOSI_PORT->BSRR 	= SPI_Conf->SPI_MOSI_PIN;						//GPIO_SetBits(SPI_Conf->SPI_MOSI_PORT,SPI_Conf->SPI_MOSI_PIN);
	GPIO_Configuration_IPU(SPI_Conf->SPI_MISO_PORT,SPI_Conf->SPI_MISO_PIN);
	
//	SPI_Conf->SPI_CS_PORT->BSRR 	= SPI_Conf->SPI_CS_PIN;
//	SPI_Conf->SPI_CLK_PORT->BRR 	= SPI_Conf->SPI_CLK_PIN;
//	SPI_Conf->SPI_CLK_PORT->BSRR 	= SPI_Conf->SPI_CLK_PIN;
	
//	GPIO_SetBits(SPI_Conf->SPI_CS_PORT,SPI_Conf->SPI_CS_PIN);
//	GPIO_SetBits(SPI_Conf->SPI_CLK_PORT,SPI_Conf->SPI_CLK_PIN);
//	GPIO_SetBits(SPI_Conf->SPI_MOSI_PORT,SPI_Conf->SPI_MOSI_PIN);
	
	
//	GPIO_InitStructure.GPIO_Pin 	= SPI_Conf->SPI_CS_PIN;
//	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(SPI_Conf->SPI_CS_PORT, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin 	= SPI_Conf->SPI_CLK_PIN;
//	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(SPI_Conf->SPI_CLK_PORT, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin 	= SPI_Conf->SPI_MOSI_PIN;
//	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(SPI_Conf->SPI_MOSI_PORT, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin 	= SPI_Conf->SPI_MISO_PIN;
//	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(SPI_Conf->SPI_MISO_PORT, &GPIO_InitStructure);
	
	
	
	#else
	SPI_FLASH_ConfigurationNR(SPI_Conf);				//普通SPI接口配置
	#endif
//	SPI_FLASH_ConfigurationDMA(SPI_Conf);			//SPI_FLASH_DMA方式配置
	SPI_FLASH_GetInfo(SPI_Conf);								//获取FLASH信息---根据ID确定FLASH型号，配置页大小，扇区大小，块大小参数
	
	SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_IDLE;			//初始状态为空闲状态
	SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_IDLE;						//初始步骤为空闲步骤
	SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Request=SPI_FLASH_qIDLE;		//初始待写入数据为0
	SPI_Conf->SPI_FLASH_Info.SPI_FLASH_LenghToWrite=0;						//初始待写入数据为0
	
	SPI_Conf->SPI_FLASH_Info.SPI_FLASH_OffsetAdrr=256;	
	
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void SPI_FLASH_ConfigurationNR(SPI_FLASH_TypeDef *SPI_Conf)
{
		//1)**********定义相关结构体
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//2)**********相关GPIO配置
	if(SPI_Conf->SPIx==SPI1)
	{
		//PA4-NSS;PA5-SCK;PA6-MISO;PA7-MOSI;
		//2.1)**********打开SPI时钟	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);			//开启SPI时钟	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA, ENABLE);	
		
		if((SPI_Conf->SPI_CS_PORT==GPIOA)&&(SPI_Conf->SPI_CS_PIN==GPIO_Pin_4))			//如果SPI_NSS为SPI_NSS_Soft（软件控制方式）
		{
//			SPI_SSOutputCmd(Pinfo->sSPIx, ENABLE);			//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
			SPI_Conf->SPI_Flash_NSS_CsFlg=1;		//如果使用纯硬件SPI1（含CS脚），SPI1_CsFlg=1，否则SPI1_CsFlg=0；
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			
//			GPIO_InitStructure.GPIO_Pin 	= SPI_Conf->SPI_CS_PIN;
//			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
//			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//			GPIO_Init(SPI_Conf->SPI_CS_PORT, &GPIO_InitStructure);
		}
		else
		{
//			SPI_SSOutputCmd(Pinfo->sSPIx, DISABLE);			//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
			SPI_Conf->SPI_Flash_NSS_CsFlg=0;		//如果使用纯硬件SPI1（含CS脚），SPI1_CsFlg=1，否则SPI1_CsFlg=0；
			//开CS-GPIO时钟
			if(SPI_Conf->SPI_CS_PORT==GPIOA)
			{
				if((SPI_Conf->SPI_CS_PIN==GPIO_Pin_13)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_14)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_15))
				{
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
					//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);			//关闭SW功能
					GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		//关闭JTAG,SW功能开启
				}
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOB)
			{
				if((SPI_Conf->SPI_CS_PIN==GPIO_Pin_3)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_4))
				{
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
					GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);				//关闭JTAG
				}
				else
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOC)
			{
				if((SPI_Conf->SPI_CS_PIN==GPIO_Pin_14)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_15))
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_AFIO, ENABLE);
				else
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOD)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOE)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOF)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOG)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
			}
			//SCK,MISO,MOSI配置
			GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			//CS配置
			GPIO_InitStructure.GPIO_Pin 	= SPI_Conf->SPI_CS_PIN;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(SPI_Conf->SPI_CS_PORT, &GPIO_InitStructure);
		}
	}
	else if(SPI_Conf->SPIx==SPI2)
	{
		//PB12-NSS;PB13-SCK;PB14-MISO;PB15-MOSI;
		//2.2)**********打开SPI时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 ,ENABLE);				//开启SPI时钟			
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);
		
		if((SPI_Conf->SPI_CS_PORT==GPIOB)&&(SPI_Conf->SPI_CS_PIN==GPIO_Pin_12))			//如果SPI_NSS为SPI_NSS_Soft（软件控制方式）
		{
			SPI_Conf->SPI_Flash_NSS_CsFlg=1;		//如果使用纯硬件SPI2（含CS脚），SPI2_CsFlg=1，否则SPI2_CsFlg=0；
			SPI_SSOutputCmd(SPI_Conf->SPIx, ENABLE);			//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
			GPIO_Init(GPIOB, &GPIO_InitStructure);	
		}
		else
		{
			
//			SPI_SSOutputCmd(Pinfo->sSPIx, DISABLE);			//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
			SPI_Conf->SPI_Flash_NSS_CsFlg=0;		//如果使用纯硬件SPI2（含CS脚），SPI2_CsFlg=1，否则SPI2_CsFlg=0；
			//开CS-GPIO时钟
			if(SPI_Conf->SPI_CS_PORT==GPIOA)
			{
				if((SPI_Conf->SPI_CS_PIN==GPIO_Pin_13)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_14)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_15))
				{
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
					//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);			//关闭SW功能
					GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		//关闭JTAG,SW功能开启
				}
				else
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOB)
			{
				if((SPI_Conf->SPI_CS_PIN==GPIO_Pin_3)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_4))
				{
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
					GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);				//关闭JTAG
				}
				else
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOC)
			{
				if((SPI_Conf->SPI_CS_PIN==GPIO_Pin_14)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_15))
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_AFIO, ENABLE);
				else
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOD)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOE)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOF)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOG)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
			}
			//SCK,MISO,MOSI配置
			GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			//CS配置
			GPIO_InitStructure.GPIO_Pin 	= SPI_Conf->SPI_CS_PIN;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(SPI_Conf->SPI_CS_PORT, &GPIO_InitStructure);
		}
	}
	else if(SPI_Conf->SPIx==SPI3)
	{
		//PA15-NSS;PB3-SCK;PB4-MISO;PB5-MOSI;
		//2.2)**********打开SPI时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3 ,ENABLE);			
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
		
		if((SPI_Conf->SPI_CS_PORT==GPIOA)&&(SPI_Conf->SPI_CS_PIN==GPIO_Pin_15))			//如果SPI_NSS为SPI_NSS_Soft（软件控制方式）
		{
			SPI_Conf->SPI_Flash_NSS_CsFlg=1;		//如果使用纯硬件SPI3（含CS脚），SPI3_CsFlg=1，否则SPI3_CsFlg=0；
			
			GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;  		//复用推挽输出
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			
			//2.2)**********SPI_NSS配置		
			GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_15;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;  		//复用推挽输出
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		}
		else																										//如果SPI_NSS为SPI_NSS_Soft（软件控制方式）
		{
			SPI_Conf->SPI_Flash_NSS_CsFlg=0;		//如果使用纯硬件SPI3（含CS脚），UseSPI3_flg=1，否则UseSPI3_flg=0；
			//开CS-GPIO时钟
			if(SPI_Conf->SPI_CS_PORT==GPIOA)
			{
				if((SPI_Conf->SPI_CS_PIN==GPIO_Pin_13)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_14)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_15))
				{
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
					//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);			//关闭SW功能
					GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		//关闭JTAG,SW功能开启
				}
				else
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOB)
			{
				if((SPI_Conf->SPI_CS_PIN==GPIO_Pin_3)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_4))
				{
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
					GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);				//关闭JTAG
				}
				else
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOC)
			{
				if((SPI_Conf->SPI_CS_PIN==GPIO_Pin_14)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_15))
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_AFIO, ENABLE);
				else
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOD)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOE)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOF)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOG)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
			}
			//SCK,MISO,MOSI配置
			GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			//CS配置
			GPIO_InitStructure.GPIO_Pin 	= SPI_Conf->SPI_CS_PIN;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(SPI_Conf->SPI_CS_PORT, &GPIO_InitStructure);
		}			
	}
	//3)**********SPI配置选项
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;				//设置方向				（2线全双工、2线只接收、一线发送、一线接收）
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;															//模式         	（从或主设备）
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;													//宽度         	（8或16位）
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;																//时钟极性     	（低或高）
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;															//时钟相位     	（第一个或第二个跳变沿）
	if(SPI_Conf->SPI_Flash_NSS_CsFlg==1)																			//如果使用纯硬件SPI1（含CS脚），UseSPI1_flg=1，否则UseSPI1_flg=0；
	{
		SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;																//片选方式     	（硬件或软件方式）
	}
	else
	{
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;															//片选方式     	（硬件或软件方式）
	}	
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_Conf->SPI_BaudRatePrescaler_x;				//波特率预分频 	（从2---256分频）
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;												//最先发送的位 	（最低位，还是最高位在先）
	SPI_InitStructure.SPI_CRCPolynomial = 7;																	//设置crc多项式	（数字）如7
	SPI_Init(SPI_Conf->SPIx,&SPI_InitStructure);

	SPI_FLASH_DISALBE(SPI_Conf);				//关闭SPI接口，使用时再打开
	
	//3)**********使能SPIx_NESS为主输出模式
	if((SPI_Conf->SPIx->CR1&0X0200)!=SPI_NSS_Soft)						//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
	{
		SPI_SSOutputCmd(SPI_Conf->SPIx, ENABLE);								//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
//		SPI_SSOutputCmd(SPI_Conf->SPIx, DISABLE);								//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
	}
	else
	{
		SPI_SSOutputCmd(SPI_Conf->SPIx, DISABLE);								//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
	}
	
}

/*******************************************************************************
*函数名		:	SPI_DMA_Configuration
*功能描述	:	函数功能说明
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SPI_FLASH_ConfigurationDMA(SPI_FLASH_TypeDef *SPI_Conf)		//SPI_FLASH_DMA方式配置
{
/**-----------------------------------------------------------------------------------------------------
	********SPI_DMA的通信过程********
	● 设置外设地址
	● 设置存储器地址
	● 设置传输数据量
	● 设置通道的配置信息
	● 使能DMA通道，启动传输
	
	● 发送时，在每次TXE被设置为’1’时发出DMA请求，DMA控制器则写数据至SPI_DR寄存器，TXE标志因此而被清除。
	● 接收时，在每次RXNE被设置为’1’时发出DMA请求，DMA控制器则从SPI_DR寄存器读出数据，RXNE标志因此而被清除。
-----------------------------------------------------------------------------------------------------**/
	//1)**********定义相关结构体
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef	DMA_Initstructure;
	DMA_Channel_TypeDef* DMAx_Channeltx=0;				//DMA发送通道请求信号---当DMA串口发送数据完成时，会发起DMA中断
	DMA_Channel_TypeDef* DMAx_Channelrx=0;				//DMA接收通道请求信号---DMA串口接收由串口发起中断，因此此处接收通道中断不使用
	u8 Conf_Flag=0;																//需要配置标志，如果SPIx合法，则Conf_Flag==1，然后进行下一步DMA配置项
	
	//2)**********基本SPI配置
//	SPI_FLASH_ConfigurationNR(SPI_Conf);					//普通SPI接口配置--未开中断和DMA

	//2)**********相关GPIO配置
	if(SPI_Conf->SPIx==SPI1)
	{
		//PA4-NSS;PA5-SCK;PA6-MISO;PA7-MOSI;
		//2.1)**********打开SPI时钟	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);			//开启SPI时钟	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA, ENABLE);	
		
		if((SPI_Conf->SPI_CS_PORT==GPIOA)&&(SPI_Conf->SPI_CS_PIN==GPIO_Pin_4))			//如果SPI_NSS为SPI_NSS_Soft（软件控制方式）
		{
//			SPI_SSOutputCmd(Pinfo->sSPIx, ENABLE);			//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
			SPI_Conf->SPI_Flash_NSS_CsFlg=1;		//如果使用纯硬件SPI1（含CS脚），SPI1_CsFlg=1，否则SPI1_CsFlg=0；
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			
//			GPIO_InitStructure.GPIO_Pin 	= SPI_Conf->SPI_CS_PIN;
//			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
//			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//			GPIO_Init(SPI_Conf->SPI_CS_PORT, &GPIO_InitStructure);
		}
		else
		{
//			SPI_SSOutputCmd(Pinfo->sSPIx, DISABLE);			//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
			SPI_Conf->SPI_Flash_NSS_CsFlg=0;		//如果使用纯硬件SPI1（含CS脚），SPI1_CsFlg=1，否则SPI1_CsFlg=0；
			//开CS-GPIO时钟
			if(SPI_Conf->SPI_CS_PORT==GPIOA)
			{
				if((SPI_Conf->SPI_CS_PIN==GPIO_Pin_13)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_14)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_15))
				{
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
					//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);			//关闭SW功能
					GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		//关闭JTAG,SW功能开启
				}
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOB)
			{
				if((SPI_Conf->SPI_CS_PIN==GPIO_Pin_3)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_4))
				{
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
					GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);				//关闭JTAG
				}
				else
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOC)
			{
				if((SPI_Conf->SPI_CS_PIN==GPIO_Pin_14)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_15))
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_AFIO, ENABLE);
				else
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOD)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOE)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOF)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOG)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
			}
			//SCK,MISO,MOSI配置
			GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			//CS配置
			GPIO_InitStructure.GPIO_Pin 	= SPI_Conf->SPI_CS_PIN;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(SPI_Conf->SPI_CS_PORT, &GPIO_InitStructure);
		}
	}
	else if(SPI_Conf->SPIx==SPI2)
	{
		//PB12-NSS;PB13-SCK;PB14-MISO;PB15-MOSI;
		//2.2)**********打开SPI时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 ,ENABLE);				//开启SPI时钟			
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);
		
		if((SPI_Conf->SPI_CS_PORT==GPIOB)&&(SPI_Conf->SPI_CS_PIN==GPIO_Pin_12))			//如果SPI_NSS为SPI_NSS_Soft（软件控制方式）
		{
			SPI_Conf->SPI_Flash_NSS_CsFlg=1;		//如果使用纯硬件SPI2（含CS脚），SPI2_CsFlg=1，否则SPI2_CsFlg=0；
			SPI_SSOutputCmd(SPI_Conf->SPIx, ENABLE);			//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
			GPIO_Init(GPIOB, &GPIO_InitStructure);	
		}
		else
		{
			
//			SPI_SSOutputCmd(Pinfo->sSPIx, DISABLE);			//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
			SPI_Conf->SPI_Flash_NSS_CsFlg=0;		//如果使用纯硬件SPI2（含CS脚），SPI2_CsFlg=1，否则SPI2_CsFlg=0；
			//开CS-GPIO时钟
			if(SPI_Conf->SPI_CS_PORT==GPIOA)
			{
				if((SPI_Conf->SPI_CS_PIN==GPIO_Pin_13)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_14)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_15))
				{
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
					//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);			//关闭SW功能
					GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		//关闭JTAG,SW功能开启
				}
				else
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOB)
			{
				if((SPI_Conf->SPI_CS_PIN==GPIO_Pin_3)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_4))
				{
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
					GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);				//关闭JTAG
				}
				else
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOC)
			{
				if((SPI_Conf->SPI_CS_PIN==GPIO_Pin_14)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_15))
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_AFIO, ENABLE);
				else
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOD)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOE)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOF)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOG)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
			}
			//SCK,MISO,MOSI配置
			GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			//CS配置
			GPIO_InitStructure.GPIO_Pin 	= SPI_Conf->SPI_CS_PIN;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(SPI_Conf->SPI_CS_PORT, &GPIO_InitStructure);
		}
	}
	else if(SPI_Conf->SPIx==SPI3)
	{
		//PA15-NSS;PB3-SCK;PB4-MISO;PB5-MOSI;
		//2.2)**********打开SPI时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3 ,ENABLE);			
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
		
		if((SPI_Conf->SPI_CS_PORT==GPIOA)&&(SPI_Conf->SPI_CS_PIN==GPIO_Pin_15))			//如果SPI_NSS为SPI_NSS_Soft（软件控制方式）
		{
			SPI_Conf->SPI_Flash_NSS_CsFlg=1;		//如果使用纯硬件SPI3（含CS脚），SPI3_CsFlg=1，否则SPI3_CsFlg=0；
			
			GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;  		//复用推挽输出
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			
			//2.2)**********SPI_NSS配置		
			GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_15;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;  		//复用推挽输出
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		}
		else																										//如果SPI_NSS为SPI_NSS_Soft（软件控制方式）
		{
			SPI_Conf->SPI_Flash_NSS_CsFlg=0;		//如果使用纯硬件SPI3（含CS脚），UseSPI3_flg=1，否则UseSPI3_flg=0；
			//开CS-GPIO时钟
			if(SPI_Conf->SPI_CS_PORT==GPIOA)
			{
				if((SPI_Conf->SPI_CS_PIN==GPIO_Pin_13)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_14)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_15))
				{
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
					//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);			//关闭SW功能
					GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		//关闭JTAG,SW功能开启
				}
				else
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOB)
			{
				if((SPI_Conf->SPI_CS_PIN==GPIO_Pin_3)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_4))
				{
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
					GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);				//关闭JTAG
				}
				else
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOC)
			{
				if((SPI_Conf->SPI_CS_PIN==GPIO_Pin_14)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_15))
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_AFIO, ENABLE);
				else
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOD)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOE)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOF)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
			}
			else if(SPI_Conf->SPI_CS_PORT==GPIOG)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
			}
			//SCK,MISO,MOSI配置
			GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			//CS配置
			GPIO_InitStructure.GPIO_Pin 	= SPI_Conf->SPI_CS_PIN;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(SPI_Conf->SPI_CS_PORT, &GPIO_InitStructure);
		}			
	}
	//3)**********SPI配置选项
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;				//设置方向				（2线全双工、2线只接收、一线发送、一线接收）
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;															//模式         	（从或主设备）
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;													//宽度         	（8或16位）
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;																//时钟极性     	（低或高）
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;															//时钟相位     	（第一个或第二个跳变沿）
	if(SPI_Conf->SPI_Flash_NSS_CsFlg==1)																			//如果使用纯硬件SPI1（含CS脚），UseSPI1_flg=1，否则UseSPI1_flg=0；
	{
		SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;																//片选方式     	（硬件或软件方式）
	}
	else
	{
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;															//片选方式     	（硬件或软件方式）
	}	
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_Conf->SPI_BaudRatePrescaler_x;				//波特率预分频 	（从2---256分频）
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;												//最先发送的位 	（最低位，还是最高位在先）
	SPI_InitStructure.SPI_CRCPolynomial = 7;																	//设置crc多项式	（数字）如7
	SPI_Init(SPI_Conf->SPIx,&SPI_InitStructure);

	SPI_FLASH_DISALBE(SPI_Conf);				//关闭SPI接口，使用时再打开
	
	//3)**********使能SPIx_NESS为主输出模式
	if((SPI_Conf->SPIx->CR1&0X0200)!=SPI_NSS_Soft)						//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
	{
		SPI_SSOutputCmd(SPI_Conf->SPIx, ENABLE);								//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
//		SPI_SSOutputCmd(SPI_Conf->SPIx, DISABLE);								//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
	}
	else
	{
		SPI_SSOutputCmd(SPI_Conf->SPIx, DISABLE);								//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
	}
	
	//3)**********SPI通道选择
	if(SPI_Conf->SPIx==SPI1)
	{
		DMAx_Channeltx=DMA1_Channel3;
		DMAx_Channelrx=DMA1_Channel2;
//		DMAx_Channelx_IRQChannel=DMA1_Channel3_IRQChannel;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
		Conf_Flag=1;																			//需要配置标志，如果SPIx合法，则Conf_Flag==1，然后进行下一步DMA配置项
	}
	else if(SPI_Conf->SPIx==SPI2)
	{
		DMAx_Channeltx=DMA1_Channel5;
		DMAx_Channelrx=DMA1_Channel4;
//		DMAx_Channelx_IRQChannel=DMA1_Channel5_IRQChannel;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
		Conf_Flag=1;																			//需要配置标志，如果SPIx合法，则Conf_Flag==1，然后进行下一步DMA配置项		
	}
	else if(SPI_Conf->SPIx==SPI3)
	{
		DMAx_Channeltx=DMA2_Channel2;
		DMAx_Channelrx=DMA2_Channel1;
//		DMAx_Channelx_IRQChannel=DMA2_Channel2_IRQChannel;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);
		Conf_Flag=1;																			//需要配置标志，如果SPIx合法，则Conf_Flag==1，然后进行下一步DMA配置项
	}
	else
	{
		Conf_Flag=0;																			//需要配置标志，如果SPIx合法，则Conf_Flag==1，然后进行下一步DMA配置项
	}	
	//4)**********SPI_DMA配置
	if(Conf_Flag==1)																		//需要配置标志，如果SPIx合法，则Conf_Flag==1，然后进行下一步DMA配置项
	{
		//5)**********DMA发送初始化，外设作为DMA的目的端
		DMA_Initstructure.DMA_PeripheralBaseAddr =  (u32)(&(SPI_Conf->SPIx)->DR);	//DMA外设源地址
		DMA_Initstructure.DMA_MemoryBaseAddr     = (u32)SPI_Conf->SPI_FLASH_Info.MOSI_Buffer;						//DMA数据内存地址
		DMA_Initstructure.DMA_DIR = DMA_DIR_PeripheralDST;												//DMA_DIR_PeripheralDST（外设作为DMA的目的端），DMA_DIR_PeripheralSRC（外设作为数据传输的来源）
		DMA_Initstructure.DMA_BufferSize = 0; 																		//指定DMA通道的DMA缓存的大小
		DMA_Initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;					//DMA_PeripheralInc_Enable（外设地址寄存器递增），DMA_PeripheralInc_Disable（外设地址寄存器不变），
		DMA_Initstructure.DMA_MemoryInc =DMA_MemoryInc_Enable;										//DMA_MemoryInc_Enable（内存地址寄存器递增），DMA_MemoryInc_Disable（内存地址寄存器不变）
		DMA_Initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//外设数据宽度--DMA_PeripheralDataSize_Byte（数据宽度为8位），DMA_PeripheralDataSize_HalfWord（数据宽度为16位），DMA_PeripheralDataSize_Word（数据宽度为32位）
		DMA_Initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;						//内存数据宽度--DMA_MemoryDataSize_Byte（数据宽度为8位），DMA_MemoryDataSize_HalfWord（数据宽度为16位），DMA_MemoryDataSize_Word（数据宽度为32位）
		DMA_Initstructure.DMA_Mode = DMA_Mode_Normal;															//DMA工作模式--DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular（不停地传送）
		DMA_Initstructure.DMA_Priority = DMA_Priority_High; 											//DMA通道的转输优先级--DMA_Priority_VeryHigh（非常高）DMA_Priority_High（高)，DMA_Priority_Medium（中），DMA_Priority_Low（低）
		DMA_Initstructure.DMA_M2M = DMA_M2M_Disable;															//DMA通道的内存到内存传输--DMA_M2M_Enable(设置为内存到内存传输)，DMA_M2M_Disable（非内存到内存传输）
		DMA_Init(DMAx_Channeltx,&DMA_Initstructure);															//初始化DMA

		//6)**********DMA接收初始化，外设作为DMA的源端
		DMA_Initstructure.DMA_PeripheralBaseAddr =  (u32)(&(SPI_Conf->SPIx)->DR);	//DMA外设源地址
		DMA_Initstructure.DMA_MemoryBaseAddr     = 	(u32)SPI_Conf->SPI_FLASH_Info.MISO_Buffer;						//DMA数据内存地址
		DMA_Initstructure.DMA_DIR = DMA_DIR_PeripheralSRC;												//DMA_DIR_PeripheralDST（外设作为DMA的目的端），DMA_DIR_PeripheralSRC（外设作为数据传输的来源）
		DMA_Initstructure.DMA_BufferSize = 0; 																		//指定DMA通道的DMA缓存的大小
		DMA_Initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;					//DMA_PeripheralInc_Enable（外设地址寄存器递增），DMA_PeripheralInc_Disable（外设地址寄存器不变），
		DMA_Initstructure.DMA_MemoryInc =DMA_MemoryInc_Enable;										//DMA_MemoryInc_Enable（内存地址寄存器递增），DMA_MemoryInc_Disable（内存地址寄存器不变）
		DMA_Initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//外设数据宽度--DMA_PeripheralDataSize_Byte（数据宽度为8位），DMA_PeripheralDataSize_HalfWord（数据宽度为16位），DMA_PeripheralDataSize_Word（数据宽度为32位）
		DMA_Initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;						//内存数据宽度--DMA_MemoryDataSize_Byte（数据宽度为8位），DMA_MemoryDataSize_HalfWord（数据宽度为16位），DMA_MemoryDataSize_Word（数据宽度为32位）
		DMA_Initstructure.DMA_Mode = DMA_Mode_Normal;															//DMA工作模式--DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular（不停地传送）
		DMA_Initstructure.DMA_Priority = DMA_Priority_High; 											//DMA通道的转输优先级--DMA_Priority_VeryHigh（非常高）DMA_Priority_High（高)，DMA_Priority_Medium（中），DMA_Priority_Low（低）
		DMA_Initstructure.DMA_M2M = DMA_M2M_Disable;															//DMA通道的内存到内存传输--DMA_M2M_Enable(设置为内存到内存传输)，DMA_M2M_Disable（非内存到内存传输）
		DMA_Init(DMAx_Channelrx,&DMA_Initstructure);															//初始化DMA
		
		//7)**********DMA通道中断初始化---此为DMA发送中断----DMA发送完成中断
			
		SPI_I2S_DMACmd(SPI_Conf->SPIx, SPI_I2S_DMAReq_Tx, ENABLE);								//开启DMA发送
		SPI_I2S_DMACmd(SPI_Conf->SPIx, SPI_I2S_DMAReq_Rx, ENABLE);								//开启DMA接收
		//使能SPIx
		SPI_Cmd(SPI_Conf->SPIx, ENABLE);
		
		//9.2)**********使能相关DMA通道传输完成中断
		DMA_Cmd(DMAx_Channelrx,DISABLE);	
		DMA_Cmd(DMAx_Channeltx,DISABLE);
	}
	SPI_Conf->SPI_Flash_USER_DMAFlg=1;	//如果使用DMA，	SPI_Flash_USER_DMAFlg=1，	否则SPI_Flash_USER_DMAFlg=0；此由SPI_FLASH_ConfigurationDMA根据SPI配置判断设置此值
	//使能SPIx
	SPI_Cmd(SPI_Conf->SPIx, DISABLE);
		//3)**********使能SPIx_NESS为主输出模式
	if((SPI_Conf->SPIx->CR1&0X0200)!=SPI_NSS_Soft)						//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
	{
		SPI_SSOutputCmd(SPI_Conf->SPIx, ENABLE);								//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
	}
	else
	{
		SPI_SSOutputCmd(SPI_Conf->SPIx, DISABLE);								//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
	}
}


/*******************************************************************************
* Function Name  : SPI_FLASH_PageWrite
* Description    : Writes more than one byte to the FLASH with a single WRITE
*                  cycle(Page WRITE sequence). The number of byte can't exceed
*                  the FLASH page size.
* Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH,
*                    must be equal or less than "SPI_FLASH_PageSize" value.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_PageWrite(SPI_FLASH_TypeDef *SPI_Conf,u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)	//FLASH写一页数据
{
	if(SPI_Conf->SPI_Flash_USER_DMAFlg==1)			//使用DMA
	{
		if(NumByteToWrite)
		{
			SPI_FLASH_WriteEnable(SPI_Conf);											//0x06写使能
			
			SPI_FLASH_ENALBE(SPI_Conf);														//SPI_FLASH_使能
			
			SPI_FLASH_WriteReadByte(SPI_Conf,Flash_WPAGE);										//0x02页写命令
			SPI_FLASH_WriteReadByte(SPI_Conf,(WriteAddr & 0xFF0000) >> 16);		//写入高8位地址
			SPI_FLASH_WriteReadByte(SPI_Conf,(WriteAddr & 0xFF00) >> 8);			//写入中间8位地址
			SPI_FLASH_WriteReadByte(SPI_Conf,WriteAddr & 0xFF);								//写入低8位地址
			
			if(SPI_Conf->SPIx==SPI1)
			{
				//DMA接收设置						
				DMA1_Channel2->CMAR =(u32)SPI_Conf->SPI_FLASH_Info.MISO_Buffer;			//接收缓冲区
				DMA1_Channel2->CNDTR =NumByteToWrite;																//设定接收送缓冲区大小
				DMA_Cmd(DMA1_Channel2,ENABLE);																			//DMA发送开启3
				//DMA发送设置
				DMA1_Channel3->CMAR=(u32)pBuffer;			//发送缓冲区
				DMA1_Channel3->CNDTR=NumByteToWrite;																//设定待发送缓冲区大小		
				DMA_Cmd(DMA1_Channel3,ENABLE);  																		//开启接收DMA
				while (DMA_GetCurrDataCounter(DMA1_Channel2)!=0);										//开始写入数据，直到写完(接收缓冲区满）
				DMA_Cmd(DMA1_Channel2,DISABLE);																	//DMA接收关闭
				DMA_Cmd(DMA1_Channel3,DISABLE);  																//DMA发送关闭
				DMA_ClearFlag(DMA1_FLAG_GL2);			//清除DMA标志
				DMA_ClearFlag(DMA1_FLAG_GL3);			//清除DMA标志
			}
			else if(SPI_Conf->SPIx==SPI2)
			{
				//DMA接收设置						
				DMA1_Channel4->CMAR =(u32)SPI_Conf->SPI_FLASH_Info.MISO_Buffer;			//接收缓冲区	
				DMA1_Channel4->CNDTR =NumByteToWrite;																//设定接收送缓冲区大小
				DMA_Cmd(DMA1_Channel4,ENABLE);																			//DMA接收开启3
				//DMA发送设置
				DMA1_Channel5->CMAR=(u32)pBuffer;			//发送缓冲区
				DMA1_Channel5->CNDTR=NumByteToWrite;																//设定待发送缓冲区大小
				DMA_Cmd(DMA1_Channel5,ENABLE);  																		//DMA发送开启
				while (DMA_GetCurrDataCounter(DMA1_Channel4)!=0);										//开始写入数据，直到写完(接收缓冲区满）
				DMA_Cmd(DMA1_Channel4,DISABLE);																	//DMA接收关闭
				DMA_Cmd(DMA1_Channel5,DISABLE);  																//DMA发送关闭
				DMA_ClearFlag(DMA1_FLAG_GL4);			//清除DMA标志
				DMA_ClearFlag(DMA1_FLAG_GL5);			//清除DMA标志
			}
			else if(SPI_Conf->SPIx==SPI3)
			{
				//DMA接收设置						
				DMA2_Channel1->CMAR =(u32)SPI_Conf->SPI_FLASH_Info.MISO_Buffer;			//接收缓冲区
				DMA2_Channel1->CNDTR =NumByteToWrite;																//设定接收送缓冲区大小
				DMA_Cmd(DMA2_Channel1,ENABLE);																			//DMA发送开启3
				//DMA发送设置
				DMA2_Channel2->CMAR=(u32)pBuffer;																		//发送缓冲区
				DMA2_Channel2->CNDTR=NumByteToWrite;																//设定待发送缓冲区大小		
				DMA_Cmd(DMA2_Channel2,ENABLE);  																		//开启接收DMA
				while (DMA_GetCurrDataCounter(DMA2_Channel1)!=0);											//开始写入数据，直到写完(接收缓冲区满）
				DMA_Cmd(DMA2_Channel1,DISABLE);																	//DMA接收关闭
				DMA_Cmd(DMA2_Channel2,DISABLE);  																//DMA发送关闭
				DMA_ClearFlag(DMA2_FLAG_GL1);			//清除DMA标志
				DMA_ClearFlag(DMA2_FLAG_GL2);			//清除DMA标志
			}

			SPI_FLASH_DISALBE(SPI_Conf);											//SPI_FLASH_关闭			
			SPI_FLASH_WaitForWriteEnd(SPI_Conf);						//等待写入完成
		}
	}
	else
	{
		if(NumByteToWrite)
		{
			SPI_FLASH_WriteEnable(SPI_Conf);											//0x06写使能
			
			SPI_FLASH_ENALBE(SPI_Conf);														//SPI_FLASH_使能
			
			SPI_FLASH_WriteReadByte(SPI_Conf,Flash_WPAGE);											//页写命令
			SPI_FLASH_WriteReadByte(SPI_Conf,(WriteAddr & 0xFF0000) >> 16);		//写入高8位地址
			SPI_FLASH_WriteReadByte(SPI_Conf,(WriteAddr & 0xFF00) >> 8);				//写入中间8位地址
			SPI_FLASH_WriteReadByte(SPI_Conf,WriteAddr & 0xFF);								//写入低8位地址
			
			while (NumByteToWrite--)		//开始写入数据，直到写完
			{
				SPI_FLASH_WriteReadByte(SPI_Conf,*pBuffer);				//写入数据
				pBuffer++;
			}
			SPI_FLASH_DISALBE(SPI_Conf);											//SPI_FLASH_关闭
			
			SPI_FLASH_WaitForWriteEnd(SPI_Conf);						//等待写入完成
		}
	}
		
}
/*******************************************************************************
* Function Name  : SPI_FLASH_BufferWrite
* Description    : Writes block of data to the FLASH. In this function, the
*                  number of WRITE cycles are reduced, using Page WRITE sequence.
* Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_BufferWrite(SPI_FLASH_TypeDef *SPI_Conf,u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  u16 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	u32	SPI_FLASH_PageSize=SPI_Conf->SPI_FLASH_Info.SPI_FLASH_PageSize;		//获取此芯片的页大小
	
	SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_IDLE;		//写状态
  Addr = WriteAddr % SPI_FLASH_PageSize;								//计算页地址是否对齐，可以整除则对齐
  count = SPI_FLASH_PageSize - Addr;										//计算页内剩余空间大小（例：页大小256，地址256，则WriteAddr % SPI_FLASH_PageSize==1；在页内剩余255个存储空间）
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;			//计算需要用到的页数量
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;		//计算剩余不完整页需要写入的数量

  if (Addr == 0) 					/* WriteAddr is SPI_FLASH_PageSize aligned  */	//页地址对齐
  {
    if (NumOfPage == 0) 	/* NumByteToWrite < SPI_FLASH_PageSize */				//数据不到一页
    {
      SPI_FLASH_PageWrite(SPI_Conf,pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */												//写入的数据大于一页
    {
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(SPI_Conf,pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;		//地址加一页
        pBuffer += SPI_FLASH_PageSize;			//缓冲区数据地址偏移一页
      }
      SPI_FLASH_PageWrite(SPI_Conf,pBuffer, WriteAddr, NumOfSingle);		//写入剩余非整页倍数据
    }
  }
  else 				/* WriteAddr is not SPI_FLASH_PageSize aligned  */				//页地址未对齐
  {
    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */				//数据不到一页大小
    {
      if (NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */			//需要跨页（需要写入的数据大于页内剩余空间）
      {
        temp = NumOfSingle - count;		//计算下一页待写入的数据个数（总数-当前页空间==下一页待写入的数据个数）

        SPI_FLASH_PageWrite(SPI_Conf,pBuffer, WriteAddr, count);	//将当前页空间写满
        WriteAddr +=  count;					//地址+
        pBuffer += count;

        SPI_FLASH_PageWrite(SPI_Conf,pBuffer, WriteAddr, temp);		//将剩余的数据写入下一页
      }
      else		//不需要跨页
      {
        SPI_FLASH_PageWrite(SPI_Conf,pBuffer, WriteAddr, NumByteToWrite);		//不需要跨页（剩余的空间足够存储当前数据）则直接写入
      }
    }
    else /* NumByteToWrite > SPI_FLASH_PageSize */											//写入的数据大于一页并且地址不对齐
    {
      NumByteToWrite -= count;														//计算将数据写入地址不对齐页后剩余数据个数
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;		//计算剩余数据需要占用的完整页个数
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;	//计算剩余数据中不完整页的数据个数
			
			//------先将前面一部分数据将起始地址不对齐数据写满
      SPI_FLASH_PageWrite(SPI_Conf,pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)			//将剩余的整页数据写入FLASH
      {
        SPI_FLASH_PageWrite(SPI_Conf,pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }

      if (NumOfSingle != 0)		//判断是否还有剩余（不够一页）待写入
      {
        SPI_FLASH_PageWrite(SPI_Conf,pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
	SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_WRITE;		//写状态
//	SPI_Conf->SPI_FLASH_Info.SPI_FLASH_LenghToWrite=0;
//	SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_IDLE;					//写状态
//	SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_IDLE;		//空闲状态
}

/*******************************************************************************
* Function Name  : SPI_FLASH_BufferRead
* Description    : Reads a block of data from the FLASH.
* Input          : - pBuffer : pointer to the buffer that receives the data read
*                    from the FLASH.
*                  - ReadAddr : FLASH's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_BufferRead(
													SPI_FLASH_TypeDef *SPI_Conf,
													u8* pBuffer, 					//数据的数据存储缓冲区
													u32 ReadAddr, 				//读取的起始地址
													u16 NumByteToRead			//待读取字节数
)
{
	#ifdef SPI_FLASH_OSTL
		if(NumByteToRead)
		{
			if(SPI_Conf->SPI_Flash_USER_DMAFlg==1)			//使用DMA
			{				
//				DMA_GetCurrDataCounter(DMA_Channel_TypeDef* DMAy_Channelx);																//返回当前DMA通道x剩余的待传输数据数目
				if(SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps==Step_IDLE)			//第一阶段
				{
					memset(SPI_Conf->SPI_FLASH_Info.MOSI_Buffer,Dummy_Byte, NumByteToRead);
					SPI_FLASH_WaitForWriteEnd(SPI_Conf);			//等待FLASH写完成			
					SPI_FLASH_ENALBE(SPI_Conf);								//SPI_FLASH_使能
					SPI_FLASH_WriteReadByte(SPI_Conf,Flash_READ);										//0x03	读数据
					SPI_FLASH_WriteReadByte(SPI_Conf,(ReadAddr & 0xFF0000) >> 16);	//写入地址高8位
					SPI_FLASH_WriteReadByte(SPI_Conf,(ReadAddr& 0xFF00) >> 8);			//写入地址中间8位
					SPI_FLASH_WriteReadByte(SPI_Conf,ReadAddr & 0xFF);							//写入地址低8位
					
					if(SPI_Conf->SPIx==SPI1)
					{
//						DMA_Cmd(DMA1_Channel2,DISABLE);																	//DMA接收关闭
//						DMA_Cmd(DMA1_Channel3,DISABLE);  																//DMA发送关闭
//						DMA_ClearFlag(DMA1_FLAG_GL2);																		//清除DMA标志
//						DMA_ClearFlag(DMA1_FLAG_GL3);																		//清除DMA标志
						//设置空闲操作符
						
						//DMA接收设置						
						DMA1_Channel2->CMAR =(u32)pBuffer;																	//接收缓冲区	
						DMA1_Channel2->CNDTR =NumByteToRead;																//设定接收送缓冲区大小
						DMA_Cmd(DMA1_Channel2,ENABLE);																			//DMA发送开启3
						//DMA发送设置
						DMA1_Channel3->CMAR=(u32)SPI_Conf->SPI_FLASH_Info.MOSI_Buffer;			//发送缓冲区
						DMA1_Channel3->CNDTR=NumByteToRead;																	//设定待发送缓冲区大小		
						DMA_Cmd(DMA1_Channel3,ENABLE);  																		//开启接收DMA
						SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_READ;
					}
					else if(SPI_Conf->SPIx==SPI2)
					{
//						DMA_Cmd(DMA1_Channel4,DISABLE);																	//DMA接收关闭
//						DMA_Cmd(DMA1_Channel5,DISABLE);  																//DMA发送关闭
//						DMA_ClearFlag(DMA1_FLAG_GL4);			//清除DMA标志
//						DMA_ClearFlag(DMA1_FLAG_GL5);			//清除DMA标志
						//设置空闲操作符
						//DMA接收设置						
						DMA1_Channel4->CMAR =(u32)pBuffer;																	//接收缓冲区	
						DMA1_Channel4->CNDTR =NumByteToRead;																//设定接收送缓冲区大小
						DMA_Cmd(DMA1_Channel4,ENABLE);																			//DMA接收开启3
						//DMA发送设置
						DMA1_Channel5->CMAR=(u32)SPI_Conf->SPI_FLASH_Info.MOSI_Buffer;			//发送缓冲区
						DMA1_Channel5->CNDTR=NumByteToRead;																	//设定待发送缓冲区大小
						DMA_Cmd(DMA1_Channel5,ENABLE);  																		//DMA发送开启
//						SPI_Cmd(SPI_Conf->SPIx, ENABLE);
						SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_READ;
					}
					else if(SPI_Conf->SPIx==SPI3)
					{
//						DMA_Cmd(DMA2_Channel1,DISABLE);																	//DMA接收关闭
//						DMA_Cmd(DMA2_Channel2,DISABLE);  																//DMA发送关闭
//						DMA_ClearFlag(DMA2_FLAG_GL1);			//清除DMA标志
//						DMA_ClearFlag(DMA2_FLAG_GL2);			//清除DMA标志
						//设置空闲操作符
						//DMA接收设置						
						DMA2_Channel1->CMAR =(u32)pBuffer;																	//接收缓冲区
						DMA2_Channel1->CNDTR =NumByteToRead;																//设定接收送缓冲区大小
						DMA_Cmd(DMA2_Channel1,ENABLE);																			//DMA发送开启3
						//DMA发送设置
						DMA2_Channel2->CMAR=(u32)SPI_Conf->SPI_FLASH_Info.MOSI_Buffer;			//发送缓冲区
						DMA2_Channel2->CNDTR=NumByteToRead;																	//设定待发送缓冲区大小		
						DMA_Cmd(DMA2_Channel2,ENABLE);  																		//开启接收DMA
						SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_READ;
					}
				}
				else if(SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps==Step_READ)	//第二阶段
				{
					if(SPI_Conf->SPIx==SPI1)
					{
						if(DMA_GetCurrDataCounter(DMA1_Channel2)==0)	//DMA_GetCurrDataCounter(DMA_Channel_TypeDef* DMAy_Channelx);																//返回当前DMA通道x剩余的待传输数据数目
						{
							SPI_FLASH_DISALBE(SPI_Conf);																		//SPI_FLASH_关闭
							DMA_Cmd(DMA1_Channel2,DISABLE);																	//DMA接收关闭
							DMA_Cmd(DMA1_Channel3,DISABLE);  																//DMA发送关闭
							DMA_ClearFlag(DMA1_FLAG_GL2);			//清除DMA标志
							DMA_ClearFlag(DMA1_FLAG_GL3);			//清除DMA标志
							
							SPI_Conf->SPI_FLASH_Info.SPI_FLASH_ReadFlg=1;										//SPI_FLASH读数标志，如果SPI_FLASH_ReadFlg==1：读到数据，SPI_FLASH_ReadFlg==0，未读到数据
							SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Request=SPI_FLASH_qIDLE;			//清除FLASH擦除请求	
							SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_IDLE;				//FLASH回到空闲状态
							SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_IDLE;							//继续检查写允许标志
						}
					}
					else if(SPI_Conf->SPIx==SPI2)
					{
						if(DMA_GetCurrDataCounter(DMA1_Channel4)==0)	//DMA_GetCurrDataCounter(DMA_Channel_TypeDef* DMAy_Channelx);																//返回当前DMA通道x剩余的待传输数据数目
						{
							SPI_FLASH_DISALBE(SPI_Conf);																		//SPI_FLASH_关闭
							DMA_Cmd(DMA1_Channel4,DISABLE);																	//DMA接收关闭
							DMA_Cmd(DMA1_Channel5,DISABLE);  																//DMA发送关闭
							DMA_ClearFlag(DMA1_FLAG_GL4);			//清除DMA标志
							DMA_ClearFlag(DMA1_FLAG_GL5);			//清除DMA标志
							SPI_Conf->SPI_FLASH_Info.SPI_FLASH_ReadFlg=1;										//SPI_FLASH读数标志，如果SPI_FLASH_ReadFlg==1：读到数据，SPI_FLASH_ReadFlg==0，未读到数据
							SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Request=SPI_FLASH_qIDLE;			//清除FLASH擦除请求	
							SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_IDLE;				//FLASH回到空闲状态
							SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_IDLE;							//继续检查写允许标志
						}
					}
					else if(SPI_Conf->SPIx==SPI3)
					{
						if(DMA_GetCurrDataCounter(DMA2_Channel1)==0)	//DMA_GetCurrDataCounter(DMA_Channel_TypeDef* DMAy_Channelx);																//返回当前DMA通道x剩余的待传输数据数目
						{
							SPI_FLASH_DISALBE(SPI_Conf);																		//SPI_FLASH_关闭
							DMA_Cmd(DMA2_Channel1,DISABLE);																	//DMA接收关闭
							DMA_Cmd(DMA2_Channel2,DISABLE);  																//DMA发送关闭
							DMA_ClearFlag(DMA2_FLAG_GL1);			//清除DMA标志
							DMA_ClearFlag(DMA2_FLAG_GL2);			//清除DMA标志
							SPI_Conf->SPI_FLASH_Info.SPI_FLASH_ReadFlg=1;										//SPI_FLASH读数标志，如果SPI_FLASH_ReadFlg==1：读到数据，SPI_FLASH_ReadFlg==0，未读到数据
							SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Request=SPI_FLASH_qIDLE;			//清除FLASH擦除请求	
							SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_IDLE;				//FLASH回到空闲状态
							SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_IDLE;							//继续检查写允许标志
						}
					}
//					SPI_Conf->SPI_FLASH_Info.SPI_FLASH_ReadFlg=1;										//SPI_FLASH读数标志，如果SPI_FLASH_ReadFlg==1：读到数据，SPI_FLASH_ReadFlg==0，未读到数据
//					SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Request=SPI_FLASH_qIDLE;			//清除FLASH擦除请求	
//					SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_IDLE;				//FLASH回到空闲状态
//					SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_IDLE;							//继续检查写允许标志
				}
				
			}
			else
			{
				SPI_FLASH_WaitForWriteEnd(SPI_Conf);			//等待FLASH写完成			
				SPI_FLASH_ENALBE(SPI_Conf);								//SPI_FLASH_使能
				SPI_FLASH_WriteReadByte(SPI_Conf,Flash_READ);										//0x03	读数据
				SPI_FLASH_WriteReadByte(SPI_Conf,(ReadAddr & 0xFF0000) >> 16);	//写入地址高8位
				SPI_FLASH_WriteReadByte(SPI_Conf,(ReadAddr& 0xFF00) >> 8);			//写入地址中间8位
				SPI_FLASH_WriteReadByte(SPI_Conf,ReadAddr & 0xFF);							//写入地址低8位
				
				while (NumByteToRead--) 	//循环读数据
				{
					*pBuffer = SPI_FLASH_WriteReadByte(SPI_Conf,Dummy_Byte);			//读数据
					pBuffer++;																										//地址++
				}
				SPI_FLASH_DISALBE(SPI_Conf);																		//SPI_FLASH_关闭
				SPI_Conf->SPI_FLASH_Info.SPI_FLASH_ReadFlg=1;										//SPI_FLASH读数标志，如果SPI_FLASH_ReadFlg==1：读到数据，SPI_FLASH_ReadFlg==0，未读到数据
				SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Request=SPI_FLASH_qIDLE;			//清除FLASH擦除请求	
				SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_IDLE;				//FLASH回到空闲状态
				SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_IDLE;							//继续检查写允许标志
			}			
		}		
	#else
		if(NumByteToRead)
		{
			SPI_FLASH_WaitForWriteEnd(SPI_Conf);			//等待FLASH写完成			
			SPI_FLASH_ENALBE(SPI_Conf);								//SPI_FLASH_使能

			SPI_FLASH_WriteReadByte(SPI_Conf,Flash_READ);										//0x03	读数据
			SPI_FLASH_WriteReadByte(SPI_Conf,(ReadAddr & 0xFF0000) >> 16);	//写入地址高8位
			SPI_FLASH_WriteReadByte(SPI_Conf,(ReadAddr& 0xFF00) >> 8);			//写入地址中间8位
			SPI_FLASH_WriteReadByte(SPI_Conf,ReadAddr & 0xFF);							//写入地址低8位

			if(SPI_Conf->SPI_Flash_USER_DMAFlg==1)			//使用DMA
			{
				
			}
			else
			{
				while (NumByteToRead--) 	//循环读数据
				{
					*pBuffer = SPI_FLASH_WriteReadByte(SPI_Conf,Dummy_Byte);			//读数据
					pBuffer++;																										//地址++
				}
				SPI_FLASH_DISALBE(SPI_Conf);																		//SPI_FLASH_关闭
			}
		}		
	#endif	
}


/*******************************************************************************
* Function Name  : SPI_FLASH_StartReadSequence
* Description    : Initiates a read data byte (READ) sequence from the Flash.
*                  This is done by driving the /CS line low to select the device,
*                  then the READ instruction is transmitted followed by 3 bytes
*                  address. This function exit and keep the /CS line low, so the
*                  Flash still being selected. With this technique the whole
*                  content of the Flash is read with a single READ instruction.
* Input          : - ReadAddr : FLASH's internal address to read from.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_StartReadSequence(SPI_FLASH_TypeDef *SPI_Conf,u32 ReadAddr)
{
//  /* Select the FLASH: Chip Select low */
//  SPI_FLASH_CS_LOW(SPI_Conf);

  /* Send "Read from Memory " instruction */
  SPI_FLASH_WriteReadByte(SPI_Conf,Flash_READ);

  /* Send the 24-bit address of the address to read from -----------------------*/
  /* Send ReadAddr high nibble address byte */
  SPI_FLASH_WriteReadByte(SPI_Conf,(ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte */
  SPI_FLASH_WriteReadByte(SPI_Conf,(ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte */
  SPI_FLASH_WriteReadByte(SPI_Conf,ReadAddr & 0xFF);
}
/*******************************************************************************
* Function Name  : SPI_FLASH_SendHalfWord
* Description    : Sends a Half Word through the SPI interface and return the
*                  Half Word received from the SPI bus.
* Input          : Half Word : Half Word to send.
* Output         : None
* Return         : The value of the received Half Word.
*******************************************************************************/
u16 SPI_FLASH_SendHalfWord(SPI_FLASH_TypeDef *SPI_Conf,u16 HalfWord)
{
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI_Conf->SPIx, SPI_I2S_FLAG_TXE) == RESET);

  /* Send Half Word through the SPI1 peripheral */
  SPI_I2S_SendData(SPI_Conf->SPIx, HalfWord);

  /* Wait to receive a Half Word */
  while (SPI_I2S_GetFlagStatus(SPI_Conf->SPIx, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the Half Word read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI_Conf->SPIx);
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void SPI_FLASH_GetInfo(SPI_FLASH_TypeDef *SPI_Conf)		//获取FLASH信息---根据ID确定FLASH型号，配置页大小，扇区大小，块大小参数
{
	u32 SPI_Flash_ID=0;
	SPI_Flash_ID=SPI_FLASH_ReadID(SPI_Conf);	//获取读FlashID
	//根据ID信息配置相当参数
	switch((u8)(SPI_Flash_ID>>16))		//厂商编号
	{
		case 0xC2:	//MXIC Manufacturer ID
		{
			switch((u8)(SPI_Flash_ID>>8))	//memory type ID
			{
				case	0x20:			//SPI_FLASH
				{
					switch((u8)SPI_Flash_ID)	//芯片型号
					{
						case	0x13:	//MX25L4006E
						{
							SPI_Conf->SPI_FLASH_Info.SPI_FLASH_PageSize			=	256;			//页大小
							SPI_Conf->SPI_FLASH_Info.SPI_FLASH_SectorSize		=	4096;			//扇区大小
							SPI_Conf->SPI_FLASH_Info.SPI_FLASH_BlockSize		=	65536;		//块大小
						}
						break;						
						default	:break;
					}
				}
				break;
				default :break;
			}
		}
		break;
		default:		//默认配置
		{
//			SPI_Conf->SPI_FLASH_PageSize=256;			//页大小
//			SPI_Conf->SPI_FLASH_SectorSize=4096;	//扇区大小
//			SPI_Conf->SPI_FLASH_BlockSize=65536;	//块大小
		}
		break;
	}
}
/*******************************************************************************
* 函数名			:	SPI_FLASH_Process
* 功能描述		:	FLASH数据处理：所有的FLASH对外操作接口 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void SPI_FLASH_Process(SPI_FLASH_TypeDef *SPI_Conf)			//FLASH数据处理：所有的FLASH对外操作接口
{
	//1)FLASH状态处理
	if(SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Staus==SPI_FLASH_IDLE)	//FLASH为空闲状态
	{
		SPI_FLASH_ReadRequest(SPI_Conf);														//获取FLASH操作请求
	}
	else if(SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Staus==SPI_FLASH_READ)			//读状态
	{
		SPI_FLASH_BufferRead(SPI_Conf,SPI_Conf->SPI_FLASH_Info.MISO_Buffer, SPI_Conf->SPI_FLASH_Info.SPI_FLASH_ReadAdrr, SPI_Conf->SPI_FLASH_Info.SPI_FLASH_LenghToRead);
	}
	else if(SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Staus==SPI_FLASH_ERASE)		//擦除
	{
		if(SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Request==SPI_FLASH_qCERASE)										//整片擦除请求
			SPI_FLASH_ChipErase		(SPI_Conf);																									//FLASH整片擦除
		else if(SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Request==SPI_FLASH_qBERASE)							//块擦除请求
			SPI_FLASH_BulkErase		(SPI_Conf,SPI_Conf->SPI_FLASH_Info.SPI_FLASH_BlockAdrr);		//Flash块擦除Fast erase time: 60ms(typ.)/sector (4K-byte per sector) ; 0.7s(typ.)/block (64K-byte per block)
		else if(SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Request==SPI_FLASH_qSERASE)							//扇区擦除请求
			SPI_FLASH_SectorErase	(SPI_Conf,SPI_Conf->SPI_FLASH_Info.SPI_FLASH_SectorAdrr);		//Flash扇区擦除Fast erase time: 60ms(typ.)/sector (4K-byte per sector) ; 0.7s(typ.)/block (64K-byte per block)
		else
		{
			SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Request=SPI_FLASH_qIDLE;		//清除FLASH擦除请求	
			SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_IDLE;			//FLASH回到空闲状态
			SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_IDLE;						//继续检查写允许标志
		}
			
	}
	else if(SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Staus==SPI_FLASH_WRITE)		//写
	{
		if(SPI_Conf->SPI_FLASH_Info.SPI_FLASH_LenghToWrite==0)
		{
			SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_IDLE;
		}
		else
		{
			SPI_FLASH_BufferWrite(SPI_Conf,SPI_Conf->SPI_FLASH_Info.MOSI_Buffer, SPI_Conf->SPI_FLASH_Info.SPI_FLASH_WriteAdrr, SPI_Conf->SPI_FLASH_Info.SPI_FLASH_LenghToWrite);	//FLASH写缓冲数据
			SPI_Conf->SPI_FLASH_Info.SPI_FLASH_WriteAdrr=0;
			SPI_Conf->SPI_FLASH_Info.SPI_FLASH_LenghToWrite=0;
//			SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_IDLE;//FLASH空闲状态，可以读写
//			SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_IDLE;//起始步骤，当前为空闲
//			SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Request=SPI_FLASH_qIDLE;//FLASH空闲
		}
	}	
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void SPI_FLASH_ENALBE(SPI_FLASH_TypeDef *SPI_Conf)				//SPI_FLASH_使能
{
//	if(SPI_Conf->SPI_Flash_NSS_CsFlg==1)														//如果使用纯硬件SPI1（含CS脚），UseSPI1_flg=1，否则UseSPI1_flg=0；
//	{
//		SPI_Cmd(SPI_Conf->SPIx, ENABLE);															//使能SPI--CS自动拉低
//	}
//	else
//	{
//		u8 num=10;
//		SPI_Cmd(SPI_Conf->SPIx, ENABLE);	//使能SPI
//		while(num--);
//		GPIO_ResetBits(SPI_Conf->SPI_CS_PORT, SPI_Conf->SPI_CS_PIN);	//CS_LOW片选使能
//	
//		GPIO_ResetBits(SPI_Conf->SPI_CLK_PORT,SPI_Conf->SPI_CLK_PIN);
//		GPIO_ResetBits(SPI_Conf->SPI_MOSI_PORT,SPI_Conf->SPI_MOSI_PIN);
	
//	GPIO_ResetBits(SPI_Conf->SPI_MOSI_PORT,SPI_Conf->SPI_MOSI_PIN);
//	GPIO_ResetBits(SPI_Conf->SPI_CLK_PORT,SPI_Conf->SPI_CLK_PIN);
//	GPIO_ResetBits(SPI_Conf->SPI_CS_PORT, SPI_Conf->SPI_CS_PIN);	//CS_LOW片选使能
	
	SPI_Conf->SPI_MOSI_PORT->BRR 	= SPI_Conf->SPI_MOSI_PIN;
	SPI_Conf->SPI_CLK_PORT->BRR 	= SPI_Conf->SPI_CLK_PIN;
	SPI_Conf->SPI_CS_PORT->BRR 		= SPI_Conf->SPI_CS_PIN;
	
		
	
		
//	}
	
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void SPI_FLASH_DISALBE(SPI_FLASH_TypeDef *SPI_Conf)				//SPI_FLASH_关闭
{	
//	if(SPI_Conf->SPI_Flash_NSS_CsFlg==1)														//如果使用纯硬件SPI1（含CS脚），UseSPI1_flg=1，否则UseSPI1_flg=0；
//	{
//		SPI_Cmd(SPI_Conf->SPIx, DISABLE);															//关闭SPI--CS自动拉高
//	}
//	else
//	{
//		SPI_Cmd(SPI_Conf->SPIx, DISABLE);															//关闭SPI				
//		GPIO_SetBits(SPI_Conf->SPI_CS_PORT, SPI_Conf->SPI_CS_PIN);		//CS_HIGH禁止片选
	
//		GPIO_SetBits(SPI_Conf->SPI_MOSI_PORT,SPI_Conf->SPI_MOSI_PIN);
//		GPIO_SetBits(SPI_Conf->SPI_CLK_PORT,SPI_Conf->SPI_CLK_PIN);
//	GPIO_SetBits(SPI_Conf->SPI_MOSI_PORT,SPI_Conf->SPI_MOSI_PIN);
//	GPIO_SetBits(SPI_Conf->SPI_CLK_PORT,SPI_Conf->SPI_CLK_PIN);
//	GPIO_SetBits(SPI_Conf->SPI_CS_PORT, SPI_Conf->SPI_CS_PIN);		//CS_HIGH禁止片选
//	GPIO_ResetBits(SPI_Conf->SPI_CLK_PORT,SPI_Conf->SPI_CLK_PIN);
//	GPIO_SetBits(SPI_Conf->SPI_CLK_PORT,SPI_Conf->SPI_CLK_PIN);
	
	SPI_Conf->SPI_CS_PORT->BSRR 	= SPI_Conf->SPI_CS_PIN;
	SPI_Conf->SPI_CLK_PORT->BRR 	= SPI_Conf->SPI_CLK_PIN;
	SPI_Conf->SPI_CLK_PORT->BSRR 	= SPI_Conf->SPI_CLK_PIN;
		
		
//	}	
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
u8 SPI_USER_SendData(SPI_FLASH_TypeDef *SPI_Conf,u8 byte)			//自定义写单字节数据
{
//	void GPIO_SetBits(GPIO_TypeDef* GPIOx, u16 GPIO_Pin);
//	void GPIO_ResetBits(GPIO_TypeDef* GPIOx, u16 GPIO_Pin);
//	u8 GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, u16 GPIO_Pin);
	
//	u8 timedl=0;
	u8 i=0;
	u8 Redata=0;

//	GPIO_ResetBits(SPI_Conf->SPI_CLK_PORT,SPI_Conf->SPI_CLK_PIN);
	for(i=0;i<8;i++)
	{
		Redata<<=1;	
		SPI_Conf->SPI_CLK_PORT->BRR = SPI_Conf->SPI_CLK_PIN;
//		GPIO_ResetBits(SPI_Conf->SPI_CLK_PORT,SPI_Conf->SPI_CLK_PIN);
//		while(timedl++<0x02);timedl=0;
		if(byte&0x80)
			SPI_Conf->SPI_MOSI_PORT->BSRR = SPI_Conf->SPI_MOSI_PIN;
//			GPIO_SetBits(SPI_Conf->SPI_MOSI_PORT,SPI_Conf->SPI_MOSI_PIN);
		else
			SPI_Conf->SPI_MOSI_PORT->BRR = SPI_Conf->SPI_MOSI_PIN;
//			GPIO_ResetBits(SPI_Conf->SPI_MOSI_PORT,SPI_Conf->SPI_MOSI_PIN);
//		while(timedl++<0x01);		timedl=0;
//		
		SPI_Conf->SPI_CLK_PORT->BSRR = SPI_Conf->SPI_CLK_PIN;
//		GPIO_SetBits(SPI_Conf->SPI_CLK_PORT,SPI_Conf->SPI_CLK_PIN);
//		while(timedl++<0x01);timedl=0;
		
//		if(GPIO_ReadInputDataBit(SPI_Conf->SPI_MISO_PORT,SPI_Conf->SPI_MISO_PIN))
		if(SPI_Conf->SPI_MISO_PORT->IDR & SPI_Conf->SPI_MISO_PIN)			
			Redata+=0x01;
		
		byte<<=1;			
	}
//	GPIO_ResetBits(SPI_Conf->SPI_CLK_PORT,SPI_Conf->SPI_CLK_PIN);
//	GPIO_ResetBits(SPI_Conf->SPI_MOSI_PORT,SPI_Conf->SPI_MOSI_PIN);
	return Redata;
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendByte
* Description    : Sends a byte through the SPI interface and return the byte
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
u8 SPI_FLASH_WriteReadByte(SPI_FLASH_TypeDef *SPI_Conf,u8 byte)
{
  
	#ifdef	USERSPI
	return SPI_USER_SendData(SPI_Conf,byte);
	#else
	/* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI_Conf->SPIx, SPI_I2S_FLAG_TXE) == RESET);
  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(SPI_Conf->SPIx, byte);
  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPI_Conf->SPIx, SPI_I2S_FLAG_RXNE) == RESET);
  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI_Conf->SPIx);
	#endif
}
/*******************************************************************************
* Function Name  : SPI_FLASH_WriteEnable
* Description    : Enables the write access to the FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_WriteEnable(SPI_FLASH_TypeDef *SPI_Conf)		//0x06
{
	u8 Status=0;
	SPI_FLASH_WaitForWriteEnd(SPI_Conf);			//等待FLASH写完成	
	
	SPI_FLASH_ENALBE(SPI_Conf);								//SPI_FLASH_使能
	SPI_Cmd(SPI_Conf->SPIx, ENABLE);					//使能SPI
	SPI_FLASH_WriteReadByte(SPI_Conf,Flash_WREN);	//0x06使能写命令
	SPI_FLASH_DISALBE(SPI_Conf);							//SPI_FLASH_关闭
	
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	
	SPI_FLASH_ENALBE(SPI_Conf);										//SPI_FLASH_使能
	SPI_FLASH_WriteReadByte(SPI_Conf,Flash_WRSR);	//0x01写状态寄存器命令
	SPI_FLASH_WriteReadByte(SPI_Conf,0x00);				//使能写状态寄存器参数--清除写保护
	SPI_FLASH_DISALBE(SPI_Conf);									//SPI_FLASH_关闭
	
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	
//	SPI_FLASH_ENALBE(SPI_Conf);										//SPI_FLASH_使能
//	SPI_Cmd(SPI_Conf->SPIx, ENABLE);							//使能SPI
//	SPI_FLASH_WriteReadByte(SPI_Conf,Flash_WREN);	//0x06使能写命令
//	SPI_FLASH_DISALBE(SPI_Conf);									//SPI_FLASH_关闭
	
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	
	do																				//等待写允许
	{
		SPI_FLASH_ENALBE(SPI_Conf);										//SPI_FLASH_使能
		SPI_Cmd(SPI_Conf->SPIx, ENABLE);							//使能SPI
		SPI_FLASH_WriteReadByte(SPI_Conf,Flash_WREN);	//0x06使能写命令
		SPI_FLASH_DISALBE(SPI_Conf);									//SPI_FLASH_关闭
		
		SPI_FLASH_ENALBE(SPI_Conf);													//SPI_FLASH_使能		
		SPI_FLASH_WriteReadByte(SPI_Conf,Flash_RDSR);						//写入读状态命令 */		
		Status = SPI_FLASH_WriteReadByte(SPI_Conf,Dummy_Byte);	//读状态 */			
		SPI_FLASH_DISALBE(SPI_Conf);									//SPI_FLASH_关闭
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
	}while((Status&WEL_Flag)!=WEL_Flag);
	
//	SPI_FLASH_DISALBE(SPI_Conf);										//SPI_FLASH_关闭
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
}
/*******************************************************************************
* Function Name  : SPI_FLASH_WriteDisable
* Description    : Enables the write access to the FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_WriteDisable(SPI_FLASH_TypeDef *SPI_Conf)			//0x写保护--写禁止
{
	u32 Status=0x00;

	SPI_FLASH_ENALBE(SPI_Conf);															//SPI_FLASH_使能	
	SPI_FLASH_WriteReadByte(SPI_Conf,Flash_RDSR);						//0x05写入读状态命令
	Status = SPI_FLASH_WriteReadByte(SPI_Conf,Dummy_Byte);	//读取状态值	
	SPI_FLASH_DISALBE(SPI_Conf);														//SPI_FLASH_关闭
	
	Status=Status|0x1C;														//所有的保护
	
  SPI_FLASH_ENALBE(SPI_Conf);										//SPI_FLASH_使能
	
	SPI_FLASH_WriteReadByte(SPI_Conf,Flash_WRDI);	//发送写保护命令
	SPI_FLASH_WriteStatus(SPI_Conf,Status);				//写Flash状态寄存器
	
	SPI_FLASH_DISALBE(SPI_Conf);									//SPI_FLASH_关闭
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	
}
/*******************************************************************************
* Function Name  : SPI_FLASH_WaitForWriteEnd
* Description    : Polls the status of the Write In Progress (WIP) flag in the
*                  FLASH's status  register  and  loop  until write  opertaion
*                  has completed.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_WaitForWriteEnd(SPI_FLASH_TypeDef *SPI_Conf)	//等待FLASH写完成
{
  u8 FLASH_Status = 0;

  SPI_FLASH_ENALBE(SPI_Conf);										//SPI_FLASH_使能
	
  SPI_FLASH_WriteReadByte(SPI_Conf,Flash_RDSR);						//发送读状态寄存器命令
  do			//循环读取状态值，直到芯片退出忙状态
  {
    FLASH_Status = SPI_FLASH_WriteReadByte(SPI_Conf,Dummy_Byte);		//循环读取状态值，
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
		__nop();
  }
  while ((FLASH_Status & WIP_Flag) == WIP_Flag); 			

	SPI_FLASH_DISALBE(SPI_Conf);									//SPI_FLASH_关闭
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	获取FLASH操作请求
*输入				: 
*返回值			:	无
*******************************************************************************/
void SPI_FLASH_ReadRequest(SPI_FLASH_TypeDef *SPI_Conf)
{
		//读请求
		if(SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Request==SPI_FLASH_qREAD)			//读请求
		{
			SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_READ;					//状态值：读
		}
		else if(SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Request==SPI_FLASH_qCERASE)		//擦除请求
		{
			SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_ERASE;					//状态值：擦除
		}
		else if(SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Request==SPI_FLASH_qBERASE)		//擦除请求
		{
			SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_ERASE;					//状态值：擦除
		}
		else if(SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Request==SPI_FLASH_qSERASE)		//擦除请求
		{
			SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_ERASE;					//状态值：擦除
		}
		else if(SPI_Conf->SPI_FLASH_Info.SPI_FLASH_LenghToWrite!=0)					//写请求
		{
			SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_WRITE;					//状态值：写
		}
		SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_IDLE;		//执行状态为初始状态
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
u8 SPI_FLASH_ReadStatus(SPI_FLASH_TypeDef *SPI_Conf)
{
	u8 FLASH_Status=0x00;	
	//--------------读FLASH状态
	SPI_FLASH_ENALBE(SPI_Conf);																		//SPI_FLASH_使能		
	SPI_FLASH_WriteReadByte(SPI_Conf,Flash_RDSR);									//0x05写入读状态命令
	FLASH_Status = SPI_FLASH_WriteReadByte(SPI_Conf,Dummy_Byte);	//读取状态值		
	SPI_FLASH_DISALBE(SPI_Conf);																	//SPI_FLASH_关闭
	
	return FLASH_Status;
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void SPI_FLASH_WriteStatus(SPI_FLASH_TypeDef *SPI_Conf,u8 Status)		//写Flash状态寄存器
{
	SPI_FLASH_WriteEnable(SPI_Conf);					//0x06写使能
	
	SPI_FLASH_ENALBE(SPI_Conf);								//SPI_FLASH_使能
	
	SPI_FLASH_WriteReadByte(SPI_Conf,Flash_WRSR);	//0x01写状态寄存器命令
	SPI_FLASH_WriteReadByte(SPI_Conf,Status);			//写状态寄存器参数
	
	SPI_FLASH_DISALBE(SPI_Conf);							//SPI_FLASH_关闭	
}

/*******************************************************************************
* Function Name  : SPI_FLASH_ReadID
* Description    : Reads FLASH identification.
* Input          : None
* Output         : None
* Return         : FLASH identification
*******************************************************************************/
u32 SPI_FLASH_ReadID(SPI_FLASH_TypeDef *SPI_Conf)
{
  u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;
	
  SPI_FLASH_ENALBE(SPI_Conf);												//SPI_FLASH_使能
	
  SPI_FLASH_WriteReadByte(SPI_Conf,Flash_RDID);			//0x9F,读设备信息命令

  Temp0 = SPI_FLASH_WriteReadByte(SPI_Conf,Dummy_Byte);
  Temp1 = SPI_FLASH_WriteReadByte(SPI_Conf,Dummy_Byte);
  Temp2 = SPI_FLASH_WriteReadByte(SPI_Conf,Dummy_Byte);


	SPI_FLASH_DISALBE(SPI_Conf);											//SPI_FLASH_关闭	  

  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;	

  return Temp;
}


/*******************************************************************************
* Function Name  : SPI_FLASH_SectorErase
* Description    : Erases the specified FLASH sector.
* Input          : SectorAddr: address of the sector to erase.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_SectorErase(SPI_FLASH_TypeDef *SPI_Conf,u32 SectorAddr)
{	
	#ifdef SPI_FLASH_OSTL
	if(SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Staus==SPI_FLASH_ERASE)
	{
		if(SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps==Step_IDLE)
		{
			SPI_FLASH_ENALBE(SPI_Conf);													//SPI_FLASH_使能
			SPI_Cmd(SPI_Conf->SPIx, ENABLE);										//使能SPI
			SPI_FLASH_WriteReadByte(SPI_Conf,Flash_WREN);				//0x06使能写命令
			SPI_FLASH_DISALBE(SPI_Conf);												//SPI_FLASH_关闭
			
			SPI_FLASH_ENALBE(SPI_Conf);													//SPI_FLASH_使能		
			SPI_FLASH_WriteReadByte(SPI_Conf,Flash_RDSR);				//0x05写入读状态命令 */		
			SPI_FLASH_WriteReadByte(SPI_Conf,Dummy_Byte);				//读状态 */			
			SPI_FLASH_DISALBE(SPI_Conf);												//SPI_FLASH_关闭
			
			SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_WIP0;			//进入擦除步骤	

		}
		else if(SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps==Step_WIP0)
		{
			u8 FLASH_Status=0;
			
			SPI_FLASH_ENALBE(SPI_Conf);																		//SPI_FLASH_使能
			SPI_Cmd(SPI_Conf->SPIx, ENABLE);															//使能SPI
			SPI_FLASH_WriteReadByte(SPI_Conf,Flash_WREN);									//0x06使能写命令
			SPI_FLASH_DISALBE(SPI_Conf);																	//SPI_FLASH_关闭
			
			SPI_FLASH_ENALBE(SPI_Conf);																		//SPI_FLASH_使能		
			SPI_FLASH_WriteReadByte(SPI_Conf,Flash_RDSR);									//0x05写入读状态命令 */		
			FLASH_Status = SPI_FLASH_WriteReadByte(SPI_Conf,Dummy_Byte);	//读状态 */			
			SPI_FLASH_DISALBE(SPI_Conf);																	//SPI_FLASH_关闭
			
			if((FLASH_Status&WEL_Flag)==WEL_Flag)		//写允许标志
			{
				//写入状态寄存器值---去除写保护
				SPI_FLASH_ENALBE(SPI_Conf);										//SPI_FLASH_使能
				SPI_FLASH_WriteReadByte(SPI_Conf,Flash_SE);		//0x20	//芯片擦除; 扇区擦除
				SPI_FLASH_WriteReadByte(SPI_Conf,(SectorAddr & 0xFF0000) >> 16);		//扇区地址高8位	
				SPI_FLASH_WriteReadByte(SPI_Conf,(SectorAddr & 0xFF00) >> 8);				//扇区地址中间8位
				SPI_FLASH_WriteReadByte(SPI_Conf,SectorAddr & 0xFF);								//扇区地址低8位
				SPI_FLASH_DISALBE(SPI_Conf);									//SPI_FLASH_关闭
				
				SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_ERASE;			//进入擦除步骤				
			}
			else		//写未允许
			{
				SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_WIP0;				//继续检查写允许标志
			}
		}
		else if(SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps==Step_ERASE)	//检查擦除完成
		{
			u8 FLASH_Status = 0;

			SPI_FLASH_ENALBE(SPI_Conf);																			//SPI_FLASH_使能			
			SPI_FLASH_WriteReadByte(SPI_Conf,Flash_RDSR);										//0x05发送读状态寄存器命令
			FLASH_Status = SPI_FLASH_WriteReadByte(SPI_Conf,Dummy_Byte);		//读状态值，
			SPI_FLASH_DISALBE(SPI_Conf);																		//SPI_FLASH_关闭
			
			if((FLASH_Status&WIP_Flag)!=WIP_Flag)						//不在写状态，表明擦除完成，退出擦除状态
			{	
				SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Request=SPI_FLASH_qIDLE;		//清除FLASH擦除请求	
				SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_IDLE;			//FLASH回到空闲状态
				SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_IDLE;						//继续检查写允许标志				
			}
			else		//写未允许
			{
				SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_ERASE;				//继续检查写允许标志
			}
		}
	}
	#else
		/* Send write enable instruction */
		SPI_FLASH_WaitForWriteEnd(SPI_Conf);			//等待FLASH写完成
		SPI_FLASH_WriteEnable(SPI_Conf);					//写使能
		
		SPI_FLASH_ENALBE(SPI_Conf);								//SPI_FLASH_使能
		/* Send Sector Erase instruction */
		SPI_FLASH_WriteReadByte(SPI_Conf,Flash_SE);
		/* Send SectorAddr high nibble address byte */
		SPI_FLASH_WriteReadByte(SPI_Conf,(SectorAddr & 0xFF0000) >> 16);
		/* Send SectorAddr medium nibble address byte */
		SPI_FLASH_WriteReadByte(SPI_Conf,(SectorAddr & 0xFF00) >> 8);
		/* Send SectorAddr low nibble address byte */
		SPI_FLASH_WriteReadByte(SPI_Conf,SectorAddr & 0xFF);
		
		SPI_FLASH_DISALBE(SPI_Conf);							//SPI_FLASH_关闭;
		
		/* Wait the end of Flash writing */
		SPI_FLASH_WaitForWriteEnd(SPI_Conf);
	#endif
}

/*******************************************************************************
* Function Name  : SPI_FLASH_BulkErase
* Description    : Erases the entire FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_BulkErase(SPI_FLASH_TypeDef *SPI_Conf,u32 BulkAddr)
{
	#ifdef SPI_FLASH_OSTL
	if(SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Staus==SPI_FLASH_ERASE)
	{
		if(SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps==Step_IDLE)
		{
			SPI_FLASH_ENALBE(SPI_Conf);													//SPI_FLASH_使能
			SPI_Cmd(SPI_Conf->SPIx, ENABLE);										//使能SPI
			SPI_FLASH_WriteReadByte(SPI_Conf,Flash_WREN);				//0x06使能写命令
			SPI_FLASH_DISALBE(SPI_Conf);												//SPI_FLASH_关闭
			
			SPI_FLASH_ENALBE(SPI_Conf);													//SPI_FLASH_使能		
			SPI_FLASH_WriteReadByte(SPI_Conf,Flash_RDSR);				//0x05写入读状态命令 */		
			SPI_FLASH_WriteReadByte(SPI_Conf,Dummy_Byte);				//读状态 */			
			SPI_FLASH_DISALBE(SPI_Conf);												//SPI_FLASH_关闭
			
			SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_WIP0;			//进入擦除步骤	

		}
		else if(SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps==Step_WIP0)
		{
			u8 FLASH_Status=0;
			
			SPI_FLASH_ENALBE(SPI_Conf);																		//SPI_FLASH_使能
			SPI_Cmd(SPI_Conf->SPIx, ENABLE);															//使能SPI
			SPI_FLASH_WriteReadByte(SPI_Conf,Flash_WREN);									//0x06使能写命令
			SPI_FLASH_DISALBE(SPI_Conf);																	//SPI_FLASH_关闭
			
			SPI_FLASH_ENALBE(SPI_Conf);																		//SPI_FLASH_使能		
			SPI_FLASH_WriteReadByte(SPI_Conf,Flash_RDSR);									//0x05写入读状态命令 */		
			FLASH_Status = SPI_FLASH_WriteReadByte(SPI_Conf,Dummy_Byte);	//读状态 */			
			SPI_FLASH_DISALBE(SPI_Conf);																	//SPI_FLASH_关闭
			
			if((FLASH_Status&WEL_Flag)==WEL_Flag)		//写允许标志
			{
				//写入状态寄存器值---去除写保护
				SPI_FLASH_ENALBE(SPI_Conf);																				//SPI_FLASH_使能
				SPI_FLASH_WriteReadByte(SPI_Conf,Flash_BE);												//0x52	//块擦除		也可以用0XD8
				SPI_FLASH_WriteReadByte(SPI_Conf,(BulkAddr & 0xFF0000) >> 16);		//块地址高8位	
				SPI_FLASH_WriteReadByte(SPI_Conf,(BulkAddr & 0xFF00) >> 8);				//块地址中间8位
				SPI_FLASH_WriteReadByte(SPI_Conf,BulkAddr & 0xFF);								//块地址低8位
				SPI_FLASH_DISALBE(SPI_Conf);									//SPI_FLASH_关闭
				
				SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_ERASE;			//进入擦除步骤				
			}
			else		//写未允许
			{
				SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_WIP0;				//继续检查写允许标志
			}
		}
		else if(SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps==Step_ERASE)	//检查擦除完成
		{
			u8 FLASH_Status = 0;

			SPI_FLASH_ENALBE(SPI_Conf);																			//SPI_FLASH_使能			
			SPI_FLASH_WriteReadByte(SPI_Conf,Flash_RDSR);										//0x05发送读状态寄存器命令
			FLASH_Status = SPI_FLASH_WriteReadByte(SPI_Conf,Dummy_Byte);		//读状态值，
			SPI_FLASH_DISALBE(SPI_Conf);																		//SPI_FLASH_关闭
			
			if((FLASH_Status&WIP_Flag)!=WIP_Flag)						//不在写状态，表明擦除完成，退出擦除状态
			{	
				SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Request=SPI_FLASH_qIDLE;		//清除FLASH擦除请求	
				SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_IDLE;			//FLASH回到空闲状态
				SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_IDLE;						//继续检查写允许标志				
			}
			else		//写未允许
			{
				SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_ERASE;				//继续检查写允许标志
			}
		}
	}
	#else
		SPI_FLASH_WriteEnable(SPI_Conf);							//FLASH写使能--等待空闲
		SPI_FLASH_WaitForWriteEnd(SPI_Conf);					//等待FLASH写完成--等待空闲
		
		SPI_FLASH_ENALBE(SPI_Conf);										//SPI_FLASH_使能
		SPI_FLASH_WriteReadByte(SPI_Conf,Flash_BE);		//写入块擦除命令
		SPI_FLASH_WriteReadByte(SPI_Conf,(BulkAddr & 0xFF0000) >> 16);		//扇区地址高8位	
		SPI_FLASH_WriteReadByte(SPI_Conf,(BulkAddr & 0xFF00) >> 8);				//扇区地址中间8位
		SPI_FLASH_WriteReadByte(SPI_Conf,BulkAddr & 0xFF);								//扇区地址低8位
		SPI_FLASH_DISALBE(SPI_Conf);									//SPI_FLASH_关闭	

		SPI_FLASH_WaitForWriteEnd(SPI_Conf);					//等待FLASH写完成--等待空闲	
	#endif
}
/*******************************************************************************
* Function Name  : SPI_FLASH_BulkErase
* Description    : Erases the entire FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_ChipErase(SPI_FLASH_TypeDef *SPI_Conf)
{
#ifndef SPI_FLASH_OSTL
	if(SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Staus==SPI_FLASH_ERASE)
	{
		u32 timee=0;
		if(SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps==Step_IDLE)
		{			
			SPI_FLASH_ENALBE(SPI_Conf);													//SPI_FLASH_使能		
			SPI_FLASH_WriteReadByte(SPI_Conf,Flash_RDSR);				//0x05写入读状态命令 */		
			SPI_FLASH_WriteReadByte(SPI_Conf,Dummy_Byte);				//读状态 */			
			SPI_FLASH_DISALBE(SPI_Conf);												//SPI_FLASH_关闭
			
			while(timee++<0x2F);
			timee=0;
			
			SPI_FLASH_ENALBE(SPI_Conf);													//SPI_FLASH_使能
			SPI_Cmd(SPI_Conf->SPIx, ENABLE);										//使能SPI
			SPI_FLASH_WriteReadByte(SPI_Conf,Flash_WREN);				//0x06使能写命令
			SPI_FLASH_DISALBE(SPI_Conf);												//SPI_FLASH_关闭
			
			while(timee++<0x2F);
			timee=0;
			
			SPI_FLASH_ENALBE(SPI_Conf);										//SPI_FLASH_使能
			SPI_FLASH_WriteReadByte(SPI_Conf,Flash_WRSR);	//0x01写状态寄存器命令
			SPI_FLASH_WriteReadByte(SPI_Conf,0x00);				//使能写状态寄存器参数--清除写保护
			SPI_FLASH_DISALBE(SPI_Conf);
			
			while(timee++<0x2F);
			timee=0;

			SPI_FLASH_ENALBE(SPI_Conf);													//SPI_FLASH_使能
			SPI_Cmd(SPI_Conf->SPIx, ENABLE);										//使能SPI
			SPI_FLASH_WriteReadByte(SPI_Conf,Flash_WREN);				//0x06使能写命令
			SPI_FLASH_DISALBE(SPI_Conf);												//SPI_FLASH_关闭			
			
			while(timee++<0x2F);
			timee=0;
			
//			SPI_FLASH_ENALBE(SPI_Conf);										//SPI_FLASH_使能
//			SPI_FLASH_WriteReadByte(SPI_Conf,Flash_CE);		//(unsigned char)0x60				//芯片擦除; 整片擦除; 也可以用0XC7	
//			SPI_FLASH_DISALBE(SPI_Conf);									//SPI_FLASH_关闭
			
			SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_WIP0;			//进入擦除步骤	

		}
		else if(SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps==Step_WIP0)
		{
			u8 FLASH_Status=0;
			
			SPI_FLASH_ENALBE(SPI_Conf);																		//SPI_FLASH_使能
			SPI_Cmd(SPI_Conf->SPIx, ENABLE);															//使能SPI
			SPI_FLASH_WriteReadByte(SPI_Conf,Flash_WREN);									//0x06使能写命令
			SPI_FLASH_DISALBE(SPI_Conf);																	//SPI_FLASH_关闭
			
			while(timee++<0x2F);
			timee=0;
			
			SPI_FLASH_ENALBE(SPI_Conf);																		//SPI_FLASH_使能		
			SPI_FLASH_WriteReadByte(SPI_Conf,Flash_RDSR);									//0x05写入读状态命令 */		
			FLASH_Status = SPI_FLASH_WriteReadByte(SPI_Conf,Dummy_Byte);	//读状态 */			
			SPI_FLASH_DISALBE(SPI_Conf);																	//SPI_FLASH_关闭
			
			while(timee++<0x2F);
			timee=0;
			
			if((FLASH_Status&WEL_Flag)==WEL_Flag)		//写允许标志
			{
				//写入状态寄存器值---去除写保护
				SPI_FLASH_ENALBE(SPI_Conf);										//SPI_FLASH_使能
				SPI_FLASH_WriteReadByte(SPI_Conf,Flash_CE);		//0x60	//芯片擦除; 整片擦除; 也可以用0XC7
				SPI_FLASH_DISALBE(SPI_Conf);									//SPI_FLASH_关闭
				SPI_FLASH_ENALBE(SPI_Conf);																			//SPI_FLASH_使能
				
				while(timee++<0x2F);
				timee=0;
				
				SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_ERASE;			//进入擦除步骤				
			}
			else		//写未允许
			{
				SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_WIP0;				//继续检查写允许标志
			}
		}
		else if(SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps==Step_ERASE)	//检查擦除完成
		{
//			u8 FLASH_Status = 0;

//			SPI_FLASH_ENALBE(SPI_Conf);																			//SPI_FLASH_使能			
			SPI_FLASH_WriteReadByte(SPI_Conf,Flash_RDSR);										//0x05发送读状态寄存器命令
			FLASH_Status1 = SPI_FLASH_WriteReadByte(SPI_Conf,Dummy_Byte);		//读状态值，
//			SPI_FLASH_DISALBE(SPI_Conf);																		//SPI_FLASH_关闭
			
			if((FLASH_Status1&WIP_Flag)!=WIP_Flag)						//不在写状态，表明擦除完成，退出擦除状态
			{	
				SPI_FLASH_DISALBE(SPI_Conf);																		//SPI_FLASH_关闭
				SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Request=SPI_FLASH_qIDLE;		//清除FLASH擦除请求	
//				SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_IDLE;			//FLASH回到空闲状态
				SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_IDLE;						//继续检查写允许标志				
			}
			else		//写未允许
			{
				SPI_Conf->SPI_FLASH_Info.SPI_FLASH_Steps=Step_ERASE;				//继续检查写允许标志
			}
		}
	}
	#else
		/* Send write enable instruction */
		SPI_FLASH_WriteEnable(SPI_Conf);							//写允许
	//	SPI_FLASH_WaitForWriteEnd(SPI_Conf);					//等待FLASH写完成--写允许
		SPI_FLASH_ENALBE(SPI_Conf);										//SPI_FLASH_使能
		SPI_FLASH_WriteReadByte(SPI_Conf,Flash_CE);		//(unsigned char)0x60				//芯片擦除; 整片擦除; 也可以用0XC7	
		SPI_FLASH_DISALBE(SPI_Conf);									//SPI_FLASH_关闭
		SPI_FLASH_WaitForWriteEnd(SPI_Conf);					//等待擦除完成
	#endif
}



















































































////#ifdef SPI_FLASH							//如果定义了SPI_FLASH 此功能生效
////#include "SPI_FLASH.H"

////#define USB_TEST_BD				//USB_TEST板
//////#define CMSIS_CDC_BD			//CMSIS-CDC板
//////#include "stdlib.h"
//////#include "stdio.h"


////#include "stm32f10x_spi.h"
////#include "stm32f10x_gpio.h"
////#include "stm32f10x_rcc.h"
////#include "stm32f10x_dma.h"
//////#include "stm32f10x_nvic.h"

//////#include "STM32_WOW.H"
////#include "STM32_PWM.H"
////#include "STM32_SYS.H"
////#include "STM32_SPI.H"
////#include "STM32_GPIO.H"
////#include "STM32_SYSTICK.H"
////#include "STM32_USART.H"

////#define 	Dummy_Byte 0xA5					//FLASH空操作数
//////#define		SPI_FLASH_PageSize   		0x100		//Flash页大小
////#define		FLASH_WriteAddress     	0x00
////#define		FLASH_ReadAddress      	FLASH_WriteAddress
////#define		FLASH_SectorToErase    	FLASH_WriteAddress
////#define 	FlASH_BufferSize				512
////#define 	USART_BufferSize				FlASH_BufferSize

////u32 Temp = 0;
////SPI_FLASH_TypeDef	FLASH_Conf;

////u16	FLASH_ChipEraseFlag=0;			//启动后擦除一次FLASH
////u16	RWF=0;			//读写标志位

////u32 FlashAddr=0;
////u32 FlashID=0;
////u8 FlashStatus=WEL_Flag;
////u8 RevBuffer[FlASH_BufferSize]={0};
////u8 SendBuffer[FlASH_BufferSize]={0x9F,0xFF,0XFF,0XFF};
////u8 testSbuffer[FlASH_BufferSize]={0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfA};
////u8 testRbuffer[FlASH_BufferSize]={0};

////u8 RxdBuffe[USART_BufferSize]={0};
////u8 RevBuffe[USART_BufferSize]={0};

////u8 TxdBuffe[USART_BufferSize]={0};
////u8 TevBuffe[USART_BufferSize]={0};

/////*******************************************************************************
////* 函数名		:	
////* 功能描述	:	 
////* 输入		:	
////* 输出		:
////* 返回 		:
////*******************************************************************************/
////void SPI_FLASH_Configuration(void)
////{
////	SYS_Configuration();											//系统配置 STM32_SYS.H	
////	GPIO_DeInitAll();																							//将所有的GPIO关闭----V20170605
////	PWM_OUT(TIM2,PWM_OUTChannel1,1,500);			//PWM设定-20161127版本
////	
////	USART_DMA_ConfigurationNR	(USART1,115200,(u32*)RxdBuffe,USART_BufferSize);	//USART_DMA配置--查询方式，不开中断
////	
////	SPI_FLASH_Conf(&FLASH_Conf);			//SPI设置

////	FlashStatus=WEL_Flag;	
////	SysTick_Configuration(100);							//系统嘀嗒时钟配置72MHz,单位为uS
////}

/////*******************************************************************************
////* 函数名		:
////* 功能描述	:
////* 输入		:
////* 输出		:
////* 返回 		:
////*******************************************************************************/
////void SPI_FLASH_Server(void)
////{
////	u16 RxNum=0;
////	u32 Addr=0;
////	
////	RxNum=USART_ReadBufferIDLE			(USART1,(u32*)RevBuffe,(u32*)RxdBuffe);	//串口空闲模式读串口接收缓冲区，如果有数据，将数据拷贝到RevBuffer,并返回接收到的数据个数，然后重新将接收缓冲区地址指向RxdBuffer
////	
////	if(RxNum&&(FlashAddr<128*4*1024))
////	{		
////		SPI_FLASH_BufferWrite(&FLASH_Conf,	RevBuffe, 		FlashAddr, RxNum);	//FLASH写缓冲数据
////		SPI_FLASH_BufferRead(&FLASH_Conf,		testSbuffer,  FlashAddr, RxNum);
////		FlashAddr+=RxNum;
////	}
//////	itoa(1234567890,TxdBuffe,10);//搜索10代表十进制
////	
////	if(Temp!=0)
////	{
////		Temp=0;

////	}
////	else
////	{
////		Temp=1;

////	}
////	
////	if(FLASH_ChipEraseFlag<=3000)
////	{
////		FLASH_ChipEraseFlag++;
////		if(FLASH_ChipEraseFlag==3000)
////		{
//////			for(Addr=0;Addr<128*4*1024;)	//524288			//523776
//////			{
//////				SPI_FLASH_BufferRead(&FLASH_Conf,testRbuffer, Addr, FlASH_BufferSize);
//////				Addr+=FlASH_BufferSize;
//////			}
////			
//////			for(Addr=0;Addr<128*4*1024;)	//524288			//523776
//////			{
//////				SPI_FLASH_BufferWrite(&FLASH_Conf,testSbuffer, Addr, FlASH_BufferSize);
//////				Addr+=FlASH_BufferSize;
//////			}
////			
////		
//////		SPI_FLASH_BulkErase(&FLASH_Conf);										//FLASH整片擦除
//////		SPI_FLASH_ChipErase(&FLASH_Conf);										//FLASH整片擦除
//////		SPI_FLASH_SectorErase(&FLASH_Conf,FLASH_ReadAddress);	//Flash扇区擦除
////			
//////			SPI_FLASH_BufferWrite(&FLASH_Conf,testSbuffer, FLASH_WriteAddress, FlASH_BufferSize);	//FLASH写缓冲数据
//////			SPI_FLASH_BufferRead(&FLASH_Conf,testSbuffer, FLASH_WriteAddress, FlASH_BufferSize);
////		}
////	}
////	RWF++;
////	if(RWF==2000)
////	{

//////		SPI_FLASH_SectorErase(&FLASH_Conf,FLASH_SectorToErase);			//擦除扇区
//////		SPI_FLASH_BulkErase(&FLASH_Conf);										//FLASH块擦除
//////		SPI_FLASH_ChipErase(&FLASH_Conf);										//FLASH整片擦除

//////		SPI_FLASH_BufferWrite(&FLASH_Conf,testSbuffer, FLASH_WriteAddress, FlASH_BufferSize);	//FLASH写缓冲数据
////		
////		
////	}
////	else if(RWF==3000)
////	{
////		RWF=0; 
//////		SPI_FLASH_BufferRead(&FLASH_Conf,testRbuffer, FLASH_ReadAddress, FlASH_BufferSize);
////		
//////		for(Addr=0;Addr<128*4*1024;)
//////		{
//////			SPI_FLASH_BufferRead(&FLASH_Conf,testRbuffer, Addr, FlASH_BufferSize);
//////			Addr+=512;
//////		}

////		
////	}
//////	Temp=0;
//////	Temp=SPI_FLASH_ReadID();
////	
//////	SPI_FLASH_CS_ENABLE;
//////	SPI_FLASH_SendByte(0x9F);
//////	SPI_FLASH_CS_DISABLE;
//////	u16 num=0xFFFE;
//////	SPI_Cmd(SPI1, ENABLE);
////	/*
////	//1）********SPI发送
////	if(SPI1_NSS==SPI_NSS_Hard)		//1）**********NSS片选方式为硬件方式（SPI_NSS_Hard）时发送数据
////	{
////		SPI_Cmd(SPI1, ENABLE);
////		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
////		SPI_I2S_SendData(SPI1,0X55);
////		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
////		SPI_Cmd(SPI1, DISABLE);
////	}
////	else				//2）**********NSS片选方式为软件件方式（SPI_NSS_Soft）时发送数据
////	{
////		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
////		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
////		SPI_I2S_SendData(SPI1,0X55);
////		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
////		GPIO_SetBits(GPIOA,GPIO_Pin_4);
////	}
////	//1.2）********SPI2发送
////	if(SPI2_NSS==SPI_NSS_Hard)		//1）**********NSS片选方式为硬件方式（SPI_NSS_Hard）时发送数据
////	{	
////		SPI_Cmd(SPI2, ENABLE);
////		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
////		SPI_I2S_SendData(SPI2,0XAA);
////		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET);
////		SPI_Cmd(SPI2, DISABLE);
////	}
////	else													//2）**********NSS片选方式为软件件方式（SPI_NSS_Soft）时发送数据
////	{
////		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
////		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
////		SPI_I2S_SendData(SPI2,0XAA);
////		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET);
////		GPIO_SetBits(GPIOB,GPIO_Pin_12);
////	}
////	*/


//////		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
//////		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
//////		SPI_I2S_SendData(SPI1,0X55);
//////		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
//////		GPIO_SetBits(GPIOA,GPIO_Pin_4);
////		
////		
//////	SPI_TX_DMAFlagClear(SPI1);
////	//2）********SPI_DMA发送

//////		SPI_Buffer
//////		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
//////		SPI_Cmd(SPI1, ENABLE);
////////		SPI_DMAPrintf(SPI1,"%d",num);		//SPI_DMA发送函数----后边的省略号就是可变参数
//////		//SPI_DMAPrintf(SPI1,"%s",SPI_Buffer);		//SPI_DMA发送函数----后边的省略号就是可变参数
//////		SPI_DMA_BufferWrite(SPI1,(u32*)SPI_Buffer,10);		//SPI_DMA发送函数----后边的省略号就是可变参数
//////		while(SPI_TX_DMAFlagClear(SPI1)==ERROR);
//////		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
//////		SPI_Cmd(SPI1, DISABLE);
//////		free(SPI_BUFFER);						//释放动态空间
//////		GPIO_SetBits(GPIOA,GPIO_Pin_4);
//////		
////////		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
//////		SPI_Cmd(SPI2, ENABLE);
////////		SPI_DMAPrintf(SPI1,"%d",num);		//SPI_DMA发送函数----后边的省略号就是可变参数
//////		//SPI_DMAPrintf(SPI1,"%s",SPI_Buffer);		//SPI_DMA发送函数----后边的省略号就是可变参数
//////		SPI_DMA_BufferWrite(SPI2,(u32*)SPI_Buffer,10);		//SPI_DMA发送函数----后边的省略号就是可变参数
//////		while(SPI_TX_DMAFlagClear(SPI2)==ERROR);
//////		while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
//////		SPI_Cmd(SPI2, DISABLE);
//////		free(SPI_BUFFER);						//释放动态空间
//////		GPIO_SetBits(GPIOB,GPIO_Pin_12);


//////	free(SPI_BUFFER);						//释放动态空间
////}
/////*******************************************************************************
////*函数名			:	function
////*功能描述		:	函数功能说明
////*输入				: 
////*返回值			:	无
////*******************************************************************************/
////void SPI_FLASH_Conf(SPI_FLASH_TypeDef *SPI_Conf)
////{
//////	#define USB_TEST_BD				//USB_TEST板
//////	#define CMSIS_CDC_BD			//CMSIS-CDC板
////	#ifdef	USB_TEST_BD
////	SPI_Conf->SPIx=SPI2;
////	SPI_Conf->SPI_CS_PORT=GPIOC;
////	SPI_Conf->SPI_CS_PIN=GPIO_Pin_8;	
////	SPI_Conf->SPI_BaudRatePrescaler_x=SPI_BaudRatePrescaler_2;
////	
////	#else
////	
////	SPI_Conf->SPIx=SPI1;
////	SPI_Conf->SPI_CS_PORT=GPIOA;
////	SPI_Conf->SPI_CS_PIN=GPIO_Pin_4;	
////	SPI_Conf->SPI_BaudRatePrescaler_x=SPI_BaudRatePrescaler_2;
////	
////	#endif
////	
////	SPI_FLASH_ConfigurationNR(SPI_Conf);				//普通SPI接口配置
////	SPI_FLASH_GetInfo(SPI_Conf);		//获取FLASH信息---根据ID确定FLASH型号，配置页大小，扇区大小，块大小参数

////}
/////*******************************************************************************
////*函数名			:	function
////*功能描述		:	函数功能说明
////*输入				: 
////*返回值			:	无
////*******************************************************************************/
////void SPI_FLASH_ConfigurationNR(SPI_FLASH_TypeDef *SPI_Conf)
////{
////		//1)**********定义相关结构体
////	SPI_InitTypeDef  SPI_InitStructure;
////	GPIO_InitTypeDef GPIO_InitStructure;

////	//2)**********相关GPIO配置
////	if(SPI_Conf->SPIx==SPI1)
////	{
////		//PA4-NSS;PA5-SCK;PA6-MISO;PA7-MOSI;
////		//2.1)**********打开SPI时钟	
////		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);			//开启SPI时钟	
////		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA, ENABLE);	
////		
////		if((SPI_Conf->SPI_CS_PORT==GPIOA)&&(SPI_Conf->SPI_CS_PIN==GPIO_Pin_4))			//如果SPI_NSS为SPI_NSS_Soft（软件控制方式）
////		{
//////			SPI_SSOutputCmd(Pinfo->sSPIx, ENABLE);			//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
////			SPI_Conf->SPI_Flash_NSS_CsFlg=1;		//如果使用纯硬件SPI1（含CS脚），SPI1_CsFlg=1，否则SPI1_CsFlg=0；
////			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
////			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
////			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////			GPIO_Init(GPIOA, &GPIO_InitStructure);
////		}
////		else
////		{
//////			SPI_SSOutputCmd(Pinfo->sSPIx, DISABLE);			//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
////			SPI_Conf->SPI_Flash_NSS_CsFlg=0;		//如果使用纯硬件SPI1（含CS脚），SPI1_CsFlg=1，否则SPI1_CsFlg=0；
////			//开CS-GPIO时钟
////			if(SPI_Conf->SPI_CS_PORT==GPIOA)
////			{
////				if((SPI_Conf->SPI_CS_PIN==GPIO_Pin_13)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_14)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_15))
////				{
////					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
////					//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);			//关闭SW功能
////					GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		//关闭JTAG,SW功能开启
////				}
////			}
////			else if(SPI_Conf->SPI_CS_PORT==GPIOB)
////			{
////				if((SPI_Conf->SPI_CS_PIN==GPIO_Pin_3)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_4))
////				{
////					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
////					GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);				//关闭JTAG
////				}
////				else
////					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
////			}
////			else if(SPI_Conf->SPI_CS_PORT==GPIOC)
////			{
////				if((SPI_Conf->SPI_CS_PIN==GPIO_Pin_14)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_15))
////					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_AFIO, ENABLE);
////				else
////					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
////			}
////			else if(SPI_Conf->SPI_CS_PORT==GPIOD)
////			{
////				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
////			}
////			else if(SPI_Conf->SPI_CS_PORT==GPIOE)
////			{
////				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
////			}
////			else if(SPI_Conf->SPI_CS_PORT==GPIOF)
////			{
////				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
////			}
////			else if(SPI_Conf->SPI_CS_PORT==GPIOG)
////			{
////				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
////			}
////			//SCK,MISO,MOSI配置
////			GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
////			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
////			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////			GPIO_Init(GPIOA, &GPIO_InitStructure);
////			//CS配置
////			GPIO_InitStructure.GPIO_Pin 	= SPI_Conf->SPI_CS_PIN;
////			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
////			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////			GPIO_Init(SPI_Conf->SPI_CS_PORT, &GPIO_InitStructure);
////		}
////	}
////	else if(SPI_Conf->SPIx==SPI2)
////	{
////		//PB12-NSS;PB13-SCK;PB14-MISO;PB15-MOSI;
////		//2.2)**********打开SPI时钟
////		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 ,ENABLE);				//开启SPI时钟			
////		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);
////		
////		if((SPI_Conf->SPI_CS_PORT==GPIOB)&&(SPI_Conf->SPI_CS_PIN==GPIO_Pin_12))			//如果SPI_NSS为SPI_NSS_Soft（软件控制方式）
////		{
////			SPI_Conf->SPI_Flash_NSS_CsFlg=1;		//如果使用纯硬件SPI2（含CS脚），SPI2_CsFlg=1，否则SPI2_CsFlg=0；
////			SPI_SSOutputCmd(SPI_Conf->SPIx, ENABLE);			//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
////			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
////			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
////			GPIO_Init(GPIOB, &GPIO_InitStructure);	
////		}
////		else
////		{
////			
//////			SPI_SSOutputCmd(Pinfo->sSPIx, DISABLE);			//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
////			SPI_Conf->SPI_Flash_NSS_CsFlg=0;		//如果使用纯硬件SPI2（含CS脚），SPI2_CsFlg=1，否则SPI2_CsFlg=0；
////			//开CS-GPIO时钟
////			if(SPI_Conf->SPI_CS_PORT==GPIOA)
////			{
////				if((SPI_Conf->SPI_CS_PIN==GPIO_Pin_13)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_14)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_15))
////				{
////					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
////					//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);			//关闭SW功能
////					GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		//关闭JTAG,SW功能开启
////				}
////				else
////					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
////			}
////			else if(SPI_Conf->SPI_CS_PORT==GPIOB)
////			{
////				if((SPI_Conf->SPI_CS_PIN==GPIO_Pin_3)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_4))
////				{
////					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
////					GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);				//关闭JTAG
////				}
////				else
////					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
////			}
////			else if(SPI_Conf->SPI_CS_PORT==GPIOC)
////			{
////				if((SPI_Conf->SPI_CS_PIN==GPIO_Pin_14)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_15))
////					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_AFIO, ENABLE);
////				else
////					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
////			}
////			else if(SPI_Conf->SPI_CS_PORT==GPIOD)
////			{
////				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
////			}
////			else if(SPI_Conf->SPI_CS_PORT==GPIOE)
////			{
////				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
////			}
////			else if(SPI_Conf->SPI_CS_PORT==GPIOF)
////			{
////				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
////			}
////			else if(SPI_Conf->SPI_CS_PORT==GPIOG)
////			{
////				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
////			}
////			//SCK,MISO,MOSI配置
////			GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
////			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
////			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////			GPIO_Init(GPIOB, &GPIO_InitStructure);
////			//CS配置
////			GPIO_InitStructure.GPIO_Pin 	= SPI_Conf->SPI_CS_PIN;
////			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
////			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////			GPIO_Init(SPI_Conf->SPI_CS_PORT, &GPIO_InitStructure);
////		}
////	}
////	else if(SPI_Conf->SPIx==SPI3)
////	{
////		//PA15-NSS;PB3-SCK;PB4-MISO;PB5-MOSI;
////		//2.2)**********打开SPI时钟
////		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3 ,ENABLE);			
////		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
////		
////		if((SPI_Conf->SPI_CS_PORT==GPIOA)&&(SPI_Conf->SPI_CS_PIN==GPIO_Pin_15))			//如果SPI_NSS为SPI_NSS_Soft（软件控制方式）
////		{
////			SPI_Conf->SPI_Flash_NSS_CsFlg=1;		//如果使用纯硬件SPI3（含CS脚），SPI3_CsFlg=1，否则SPI3_CsFlg=0；
////			
////			GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
////			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;  		//复用推挽输出
////			GPIO_Init(GPIOB, &GPIO_InitStructure);
////			
////			//2.2)**********SPI_NSS配置		
////			GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_15;
////			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;  		//复用推挽输出
////			GPIO_Init(GPIOA, &GPIO_InitStructure);
////		}
////		else																										//如果SPI_NSS为SPI_NSS_Soft（软件控制方式）
////		{
////			SPI_Conf->SPI_Flash_NSS_CsFlg=0;		//如果使用纯硬件SPI3（含CS脚），UseSPI3_flg=1，否则UseSPI3_flg=0；
////			//开CS-GPIO时钟
////			if(SPI_Conf->SPI_CS_PORT==GPIOA)
////			{
////				if((SPI_Conf->SPI_CS_PIN==GPIO_Pin_13)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_14)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_15))
////				{
////					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
////					//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);			//关闭SW功能
////					GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		//关闭JTAG,SW功能开启
////				}
////				else
////					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
////			}
////			else if(SPI_Conf->SPI_CS_PORT==GPIOB)
////			{
////				if((SPI_Conf->SPI_CS_PIN==GPIO_Pin_3)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_4))
////				{
////					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
////					GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);				//关闭JTAG
////				}
////				else
////					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
////			}
////			else if(SPI_Conf->SPI_CS_PORT==GPIOC)
////			{
////				if((SPI_Conf->SPI_CS_PIN==GPIO_Pin_14)||(SPI_Conf->SPI_CS_PIN==GPIO_Pin_15))
////					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_AFIO, ENABLE);
////				else
////					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
////			}
////			else if(SPI_Conf->SPI_CS_PORT==GPIOD)
////			{
////				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
////			}
////			else if(SPI_Conf->SPI_CS_PORT==GPIOE)
////			{
////				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
////			}
////			else if(SPI_Conf->SPI_CS_PORT==GPIOF)
////			{
////				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
////			}
////			else if(SPI_Conf->SPI_CS_PORT==GPIOG)
////			{
////				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
////			}
////			//SCK,MISO,MOSI配置
////			GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
////			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
////			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////			GPIO_Init(GPIOB, &GPIO_InitStructure);
////			//CS配置
////			GPIO_InitStructure.GPIO_Pin 	= SPI_Conf->SPI_CS_PIN;
////			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
////			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////			GPIO_Init(SPI_Conf->SPI_CS_PORT, &GPIO_InitStructure);
////		}			
////	}
////	//3)**********SPI配置选项
////	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;				//设置方向				（2线全双工、2线只接收、一线发送、一线接收）
////	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;															//模式         	（从或主设备）
////	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;													//宽度         	（8或16位）
////	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;																//时钟极性     	（低或高）
////	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;															//时钟相位     	（第一个或第二个跳变沿）
////	if(SPI_Conf->SPI_Flash_NSS_CsFlg==1)																			//如果使用纯硬件SPI1（含CS脚），UseSPI1_flg=1，否则UseSPI1_flg=0；
////	{
////		SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;																//片选方式     	（硬件或软件方式）
////	}
////	else
////	{
////		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;															//片选方式     	（硬件或软件方式）
////	}	
////	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_Conf->SPI_BaudRatePrescaler_x;				//波特率预分频 	（从2---256分频）
////	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;												//最先发送的位 	（最低位，还是最高位在先）
////	SPI_InitStructure.SPI_CRCPolynomial = 7;																	//设置crc多项式	（数字）如7
////	SPI_Init(SPI_Conf->SPIx,&SPI_InitStructure);

////	SPI_FLASH_DISALBE(SPI_Conf);				//关闭SPI接口，使用时再打开
////	
////	//3)**********使能SPIx_NESS为主输出模式
////	if((SPI_Conf->SPIx->CR1&0X0200)!=SPI_NSS_Soft)						//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
////	{
////		SPI_SSOutputCmd(SPI_Conf->SPIx, ENABLE);								//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
////	}
////	else
////	{
////		SPI_SSOutputCmd(SPI_Conf->SPIx, DISABLE);								//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
////	}
////}

/////*******************************************************************************
////*函数名		:	SPI_DMA_Configuration
////*功能描述	:	函数功能说明
////*输入			: 
////*输出			:	无
////*返回值		:	无
////*例程			:
////*******************************************************************************/
////void SPI_FLASH_ConfigurationDMA(
////																SPI_FLASH_TypeDef *SPI_Conf,		//SPI结构体
////																u32* SPI_RX_Buffer,							//接收缓冲区地址::发送缓冲区地址在发送数据时设定，配置时借用接收缓冲区地址完成配置
////																u32 SPI_BUFFERSIZE							//缓冲区大小
////)		//SPI_FLASH_DMA方式配置
////{
/////**-----------------------------------------------------------------------------------------------------
////	********SPI_DMA的通信过程********
////	● 设置外设地址
////	● 设置存储器地址
////	● 设置传输数据量
////	● 设置通道的配置信息
////	● 使能DMA通道，启动传输
////	
////	● 发送时，在每次TXE被设置为’1’时发出DMA请求，DMA控制器则写数据至SPI_DR寄存器，TXE标志因此而被清除。
////	● 接收时，在每次RXNE被设置为’1’时发出DMA请求，DMA控制器则从SPI_DR寄存器读出数据，RXNE标志因此而被清除。
////-----------------------------------------------------------------------------------------------------**/
////	//1)**********定义相关结构体
////	DMA_InitTypeDef	DMA_Initstructure;

////	DMA_Channel_TypeDef* DMAx_Channeltx=0;				//DMA发送通道请求信号---当DMA串口发送数据完成时，会发起DMA中断
////	DMA_Channel_TypeDef* DMAx_Channelrx=0;				//DMA接收通道请求信号---DMA串口接收由串口发起中断，因此此处接收通道中断不使用

////	u8 Conf_Flag=0;																//需要配置标志，如果SPIx合法，则Conf_Flag==1，然后进行下一步DMA配置项
////	
////	//2)**********基本SPI配置
////	SPI_FLASH_ConfigurationNR(SPI_Conf);					//普通SPI接口配置--未开中断和DMA
////	
////	//3)**********SPI通道选择
////	if(SPI_Conf->SPIx==SPI1)
////	{
////		DMAx_Channeltx=DMA1_Channel3;
////		DMAx_Channelrx=DMA1_Channel2;
//////		DMAx_Channelx_IRQChannel=DMA1_Channel3_IRQChannel;
////		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
////		Conf_Flag=1;																			//需要配置标志，如果SPIx合法，则Conf_Flag==1，然后进行下一步DMA配置项
////	}
////	else if(SPI_Conf->SPIx==SPI2)
////	{
////		DMAx_Channeltx=DMA1_Channel5;
////		DMAx_Channelrx=DMA1_Channel4;
//////		DMAx_Channelx_IRQChannel=DMA1_Channel5_IRQChannel;
////		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
////		Conf_Flag=1;																			//需要配置标志，如果SPIx合法，则Conf_Flag==1，然后进行下一步DMA配置项		
////	}
////	else if(SPI_Conf->SPIx==SPI3)
////	{
////		DMAx_Channeltx=DMA2_Channel2;
////		DMAx_Channelrx=DMA2_Channel1;
//////		DMAx_Channelx_IRQChannel=DMA2_Channel2_IRQChannel;
////		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);
////		Conf_Flag=1;																			//需要配置标志，如果SPIx合法，则Conf_Flag==1，然后进行下一步DMA配置项
////	}
////	else
////	{
////		Conf_Flag=0;																			//需要配置标志，如果SPIx合法，则Conf_Flag==1，然后进行下一步DMA配置项
////	}	
////	//4)**********SPI_DMA配置
////	if(Conf_Flag==1)																		//需要配置标志，如果SPIx合法，则Conf_Flag==1，然后进行下一步DMA配置项
////	{
////		//5)**********DMA发送初始化，外设作为DMA的目的端
////		DMA_Initstructure.DMA_PeripheralBaseAddr =  (u32)(&(SPI_Conf->SPIx)->DR);	//DMA外设源地址
////		DMA_Initstructure.DMA_MemoryBaseAddr     = (u32)SPI_RX_Buffer;						//DMA数据内存地址
////		DMA_Initstructure.DMA_DIR = DMA_DIR_PeripheralDST;												//DMA_DIR_PeripheralDST（外设作为DMA的目的端），DMA_DIR_PeripheralSRC（外设作为数据传输的来源）
////		DMA_Initstructure.DMA_BufferSize = SPI_BUFFERSIZE; 												//指定DMA通道的DMA缓存的大小
////		DMA_Initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;					//DMA_PeripheralInc_Enable（外设地址寄存器递增），DMA_PeripheralInc_Disable（外设地址寄存器不变），
////		DMA_Initstructure.DMA_MemoryInc =DMA_MemoryInc_Enable;										//DMA_MemoryInc_Enable（内存地址寄存器递增），DMA_MemoryInc_Disable（内存地址寄存器不变）
////		DMA_Initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//外设数据宽度--DMA_PeripheralDataSize_Byte（数据宽度为8位），DMA_PeripheralDataSize_HalfWord（数据宽度为16位），DMA_PeripheralDataSize_Word（数据宽度为32位）
////		DMA_Initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;						//内存数据宽度--DMA_MemoryDataSize_Byte（数据宽度为8位），DMA_MemoryDataSize_HalfWord（数据宽度为16位），DMA_MemoryDataSize_Word（数据宽度为32位）
////		DMA_Initstructure.DMA_Mode = DMA_Mode_Normal;															//DMA工作模式--DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular（不停地传送）
////		DMA_Initstructure.DMA_Priority = DMA_Priority_High; 											//DMA通道的转输优先级--DMA_Priority_VeryHigh（非常高）DMA_Priority_High（高)，DMA_Priority_Medium（中），DMA_Priority_Low（低）
////		DMA_Initstructure.DMA_M2M = DMA_M2M_Disable;															//DMA通道的内存到内存传输--DMA_M2M_Enable(设置为内存到内存传输)，DMA_M2M_Disable（非内存到内存传输）
////		DMA_Init(DMAx_Channeltx,&DMA_Initstructure);															//初始化DMA

////		//6)**********DMA接收初始化，外设作为DMA的源端
////		DMA_Initstructure.DMA_PeripheralBaseAddr =  (u32)(&(SPI_Conf->SPIx)->DR);	//DMA外设源地址
////		DMA_Initstructure.DMA_MemoryBaseAddr     = 	(u32)SPI_RX_Buffer;						//DMA数据内存地址
////		DMA_Initstructure.DMA_DIR = DMA_DIR_PeripheralSRC;												//DMA_DIR_PeripheralDST（外设作为DMA的目的端），DMA_DIR_PeripheralSRC（外设作为数据传输的来源）
////		DMA_Initstructure.DMA_BufferSize = SPI_BUFFERSIZE; 												//指定DMA通道的DMA缓存的大小
////		DMA_Initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;					//DMA_PeripheralInc_Enable（外设地址寄存器递增），DMA_PeripheralInc_Disable（外设地址寄存器不变），
////		DMA_Initstructure.DMA_MemoryInc =DMA_MemoryInc_Enable;										//DMA_MemoryInc_Enable（内存地址寄存器递增），DMA_MemoryInc_Disable（内存地址寄存器不变）
////		DMA_Initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//外设数据宽度--DMA_PeripheralDataSize_Byte（数据宽度为8位），DMA_PeripheralDataSize_HalfWord（数据宽度为16位），DMA_PeripheralDataSize_Word（数据宽度为32位）
////		DMA_Initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;						//内存数据宽度--DMA_MemoryDataSize_Byte（数据宽度为8位），DMA_MemoryDataSize_HalfWord（数据宽度为16位），DMA_MemoryDataSize_Word（数据宽度为32位）
////		DMA_Initstructure.DMA_Mode = DMA_Mode_Normal;															//DMA工作模式--DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular（不停地传送）
////		DMA_Initstructure.DMA_Priority = DMA_Priority_High; 											//DMA通道的转输优先级--DMA_Priority_VeryHigh（非常高）DMA_Priority_High（高)，DMA_Priority_Medium（中），DMA_Priority_Low（低）
////		DMA_Initstructure.DMA_M2M = DMA_M2M_Disable;															//DMA通道的内存到内存传输--DMA_M2M_Enable(设置为内存到内存传输)，DMA_M2M_Disable（非内存到内存传输）
////		DMA_Init(DMAx_Channelrx,&DMA_Initstructure);															//初始化DMA
////		
////		//7)**********DMA通道中断初始化---此为DMA发送中断----DMA发送完成中断
////			
////		SPI_I2S_DMACmd(SPI_Conf->SPIx, SPI_I2S_DMAReq_Tx, ENABLE);								//开启DMA发送
////		SPI_I2S_DMACmd(SPI_Conf->SPIx, SPI_I2S_DMAReq_Rx, ENABLE);								//开启DMA接收
////		//使能SPIx
////		SPI_Cmd(SPI_Conf->SPIx, ENABLE);
////		
////		//9.2)**********使能相关DMA通道传输完成中断
////		DMA_Cmd(DMAx_Channelrx,DISABLE);	
////		DMA_Cmd(DMAx_Channeltx,DISABLE);
////	}
////	//使能SPIx
////	SPI_Cmd(SPI_Conf->SPIx, DISABLE);
////}


/////*******************************************************************************
////* Function Name  : SPI_FLASH_PageWrite
////* Description    : Writes more than one byte to the FLASH with a single WRITE
////*                  cycle(Page WRITE sequence). The number of byte can't exceed
////*                  the FLASH page size.
////* Input          : - pBuffer : pointer to the buffer  containing the data to be
////*                    written to the FLASH.
////*                  - WriteAddr : FLASH's internal address to write to.
////*                  - NumByteToWrite : number of bytes to write to the FLASH,
////*                    must be equal or less than "SPI_FLASH_PageSize" value.
////* Output         : None
////* Return         : None
////*******************************************************************************/
////void SPI_FLASH_PageWrite(SPI_FLASH_TypeDef *SPI_Conf,u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)	//FLASH写一页数据
////{
////	if(NumByteToWrite)
////	{
////		SPI_FLASH_WriteEnable(SPI_Conf);											//0x06写使能
////		
////		SPI_FLASH_ENALBE(SPI_Conf);														//SPI_FLASH_使能
////		
////		SPI_FLASH_WriteReadByte(SPI_Conf,Flash_WPAGE);											//页写命令
////		SPI_FLASH_WriteReadByte(SPI_Conf,(WriteAddr & 0xFF0000) >> 16);		//写入高8位地址
////		SPI_FLASH_WriteReadByte(SPI_Conf,(WriteAddr & 0xFF00) >> 8);				//写入中间8位地址
////		SPI_FLASH_WriteReadByte(SPI_Conf,WriteAddr & 0xFF);								//写入低8位地址
////		
////		while (NumByteToWrite--)		//开始写入数据，直到写完
////		{
////			SPI_FLASH_WriteReadByte(SPI_Conf,*pBuffer);				//写入数据
////			pBuffer++;
////		}
////		SPI_FLASH_DISALBE(SPI_Conf);											//SPI_FLASH_关闭
////		
////		SPI_FLASH_WaitForWriteEnd(SPI_Conf);						//等待写入完成
////	}	
////}
/////*******************************************************************************
////* Function Name  : SPI_FLASH_BufferWrite
////* Description    : Writes block of data to the FLASH. In this function, the
////*                  number of WRITE cycles are reduced, using Page WRITE sequence.
////* Input          : - pBuffer : pointer to the buffer  containing the data to be
////*                    written to the FLASH.
////*                  - WriteAddr : FLASH's internal address to write to.
////*                  - NumByteToWrite : number of bytes to write to the FLASH.
////* Output         : None
////* Return         : None
////*******************************************************************************/
////void SPI_FLASH_BufferWrite(SPI_FLASH_TypeDef *SPI_Conf,u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
////{
////  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
////	u32	SPI_FLASH_PageSize=SPI_Conf->SPI_FLASH_PageSize;		//获取此芯片的页大小
////	
////  Addr = WriteAddr % SPI_FLASH_PageSize;							//计算页地址是否对齐，可以整除则对齐
////  count = SPI_FLASH_PageSize - Addr;									//计算页内剩余空间大小（例：页大小256，地址256，则WriteAddr % SPI_FLASH_PageSize==1；在页内剩余255个存储空间）
////  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;		//计算需要用到的页数量
////  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;	//计算剩余不完整页需要写入的数量

////  if (Addr == 0) 					/* WriteAddr is SPI_FLASH_PageSize aligned  */	//页地址对齐
////  {
////    if (NumOfPage == 0) 	/* NumByteToWrite < SPI_FLASH_PageSize */				//数据不到一页
////    {
////      SPI_FLASH_PageWrite(SPI_Conf,pBuffer, WriteAddr, NumByteToWrite);
////    }
////    else /* NumByteToWrite > SPI_FLASH_PageSize */												//写入的数据大于一页
////    {
////      while (NumOfPage--)
////      {
////        SPI_FLASH_PageWrite(SPI_Conf,pBuffer, WriteAddr, SPI_FLASH_PageSize);
////        WriteAddr +=  SPI_FLASH_PageSize;		//地址加一页
////        pBuffer += SPI_FLASH_PageSize;			//缓冲区数据地址偏移一页
////      }
////      SPI_FLASH_PageWrite(SPI_Conf,pBuffer, WriteAddr, NumOfSingle);		//写入剩余非整页倍数据
////    }
////  }
////  else 				/* WriteAddr is not SPI_FLASH_PageSize aligned  */				//页地址未对齐
////  {
////    if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */				//数据不到一页大小
////    {
////      if (NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */			//需要跨页（需要写入的数据大于页内剩余空间）
////      {
////        temp = NumOfSingle - count;		//计算下一页待写入的数据个数（总数-当前页空间==下一页待写入的数据个数）

////        SPI_FLASH_PageWrite(SPI_Conf,pBuffer, WriteAddr, count);	//将当前页空间写满
////        WriteAddr +=  count;					//地址+
////        pBuffer += count;

////        SPI_FLASH_PageWrite(SPI_Conf,pBuffer, WriteAddr, temp);		//将剩余的数据写入下一页
////      }
////      else		//不需要跨页
////      {
////        SPI_FLASH_PageWrite(SPI_Conf,pBuffer, WriteAddr, NumByteToWrite);		//不需要跨页（剩余的空间足够存储当前数据）则直接写入
////      }
////    }
////    else /* NumByteToWrite > SPI_FLASH_PageSize */											//写入的数据大于一页并且地址不对齐
////    {
////      NumByteToWrite -= count;														//计算将数据写入地址不对齐页后剩余数据个数
////      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;		//计算剩余数据需要占用的完整页个数
////      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;	//计算剩余数据中不完整页的数据个数
////			
////			//------先将前面一部分数据将起始地址不对齐数据写满
////      SPI_FLASH_PageWrite(SPI_Conf,pBuffer, WriteAddr, count);
////      WriteAddr +=  count;
////      pBuffer += count;

////      while (NumOfPage--)			//将剩余的整页数据写入FLASH
////      {
////        SPI_FLASH_PageWrite(SPI_Conf,pBuffer, WriteAddr, SPI_FLASH_PageSize);
////        WriteAddr +=  SPI_FLASH_PageSize;
////        pBuffer += SPI_FLASH_PageSize;
////      }

////      if (NumOfSingle != 0)		//判断是否还有剩余（不够一页）待写入
////      {
////        SPI_FLASH_PageWrite(SPI_Conf,pBuffer, WriteAddr, NumOfSingle);
////      }
////    }
////  }
////}

/////*******************************************************************************
////* Function Name  : SPI_FLASH_BufferRead
////* Description    : Reads a block of data from the FLASH.
////* Input          : - pBuffer : pointer to the buffer that receives the data read
////*                    from the FLASH.
////*                  - ReadAddr : FLASH's internal address to read from.
////*                  - NumByteToRead : number of bytes to read from the FLASH.
////* Output         : None
////* Return         : None
////*******************************************************************************/
////void SPI_FLASH_BufferRead(
////													SPI_FLASH_TypeDef *SPI_Conf,
////													u8* pBuffer, 					//数据的数据存储缓冲区
////													u32 ReadAddr, 				//读取的起始地址
////													u16 NumByteToRead			//待读取字节数
////)
////{
////	if(NumByteToRead)
////	{
////		SPI_FLASH_WaitForWriteEnd(SPI_Conf);	//等待FLASH写完成
////		
////		SPI_FLASH_ENALBE(SPI_Conf);								//SPI_FLASH_使能
////		
////		/* Send "Read from Memory " instruction */
////		SPI_FLASH_WriteReadByte(SPI_Conf,Flash_READ);

////		/* Send ReadAddr high nibble address byte to read from */
////		SPI_FLASH_WriteReadByte(SPI_Conf,(ReadAddr & 0xFF0000) >> 16);
////		/* Send ReadAddr medium nibble address byte to read from */
////		SPI_FLASH_WriteReadByte(SPI_Conf,(ReadAddr& 0xFF00) >> 8);
////		/* Send ReadAddr low nibble address byte to read from */
////		SPI_FLASH_WriteReadByte(SPI_Conf,ReadAddr & 0xFF);

////		while (NumByteToRead--) /* while there is data to be read */
////		{
////			/* Read a byte from the FLASH */
////			*pBuffer = SPI_FLASH_WriteReadByte(SPI_Conf,Dummy_Byte);
////			/* Point to the next location where the byte read will be saved */
////			pBuffer++;
////		}
////		SPI_FLASH_DISALBE(SPI_Conf);											//SPI_FLASH_关闭
////	}
////}


/////*******************************************************************************
////* Function Name  : SPI_FLASH_StartReadSequence
////* Description    : Initiates a read data byte (READ) sequence from the Flash.
////*                  This is done by driving the /CS line low to select the device,
////*                  then the READ instruction is transmitted followed by 3 bytes
////*                  address. This function exit and keep the /CS line low, so the
////*                  Flash still being selected. With this technique the whole
////*                  content of the Flash is read with a single READ instruction.
////* Input          : - ReadAddr : FLASH's internal address to read from.
////* Output         : None
////* Return         : None
////*******************************************************************************/
////void SPI_FLASH_StartReadSequence(SPI_FLASH_TypeDef *SPI_Conf,u32 ReadAddr)
////{
//////  /* Select the FLASH: Chip Select low */
//////  SPI_FLASH_CS_LOW(SPI_Conf);

////  /* Send "Read from Memory " instruction */
////  SPI_FLASH_WriteReadByte(SPI_Conf,Flash_READ);

////  /* Send the 24-bit address of the address to read from -----------------------*/
////  /* Send ReadAddr high nibble address byte */
////  SPI_FLASH_WriteReadByte(SPI_Conf,(ReadAddr & 0xFF0000) >> 16);
////  /* Send ReadAddr medium nibble address byte */
////  SPI_FLASH_WriteReadByte(SPI_Conf,(ReadAddr& 0xFF00) >> 8);
////  /* Send ReadAddr low nibble address byte */
////  SPI_FLASH_WriteReadByte(SPI_Conf,ReadAddr & 0xFF);
////}
/////*******************************************************************************
////* Function Name  : SPI_FLASH_SendHalfWord
////* Description    : Sends a Half Word through the SPI interface and return the
////*                  Half Word received from the SPI bus.
////* Input          : Half Word : Half Word to send.
////* Output         : None
////* Return         : The value of the received Half Word.
////*******************************************************************************/
////u16 SPI_FLASH_SendHalfWord(SPI_FLASH_TypeDef *SPI_Conf,u16 HalfWord)
////{
////  /* Loop while DR register in not emplty */
////  while (SPI_I2S_GetFlagStatus(SPI_Conf->SPIx, SPI_I2S_FLAG_TXE) == RESET);

////  /* Send Half Word through the SPI1 peripheral */
////  SPI_I2S_SendData(SPI_Conf->SPIx, HalfWord);

////  /* Wait to receive a Half Word */
////  while (SPI_I2S_GetFlagStatus(SPI_Conf->SPIx, SPI_I2S_FLAG_RXNE) == RESET);

////  /* Return the Half Word read from the SPI bus */
////  return SPI_I2S_ReceiveData(SPI_Conf->SPIx);
////}
/////*******************************************************************************
////*函数名			:	function
////*功能描述		:	函数功能说明
////*输入				: 
////*返回值			:	无
////*******************************************************************************/
////void SPI_FLASH_GetInfo(SPI_FLASH_TypeDef *SPI_Conf)		//获取FLASH信息---根据ID确定FLASH型号，配置页大小，扇区大小，块大小参数
////{
////	u32 SPI_Flash_ID=0;
////	SPI_Flash_ID=SPI_FLASH_ReadID(SPI_Conf);	//获取读FlashID
////	//根据ID信息配置相当参数
////	switch((u8)(SPI_Flash_ID>>16))		//厂商编号
////	{
////		case 0xC2:	//MXIC Manufacturer ID
////		{
////			switch((u8)(SPI_Flash_ID>>8))	//memory type ID
////			{
////				case	0x20:			//SPI_FLASH
////				{
////					switch((u8)SPI_Flash_ID)	//芯片型号
////					{
////						case	0x13:	//MX25L4006E
////						{
////							SPI_Conf->SPI_FLASH_PageSize		=	256;			//页大小
////							SPI_Conf->SPI_FLASH_SectorSize	=	4096;			//扇区大小
////							SPI_Conf->SPI_FLASH_BlockSize		=	65536;		//块大小
////						}
////						break;						
////						default	:break;
////					}
////				}
////				break;
////				default :break;
////			}
////		}
////		break;
////		default:		//默认配置
////		{
//////			SPI_Conf->SPI_FLASH_PageSize=256;			//页大小
//////			SPI_Conf->SPI_FLASH_SectorSize=4096;	//扇区大小
//////			SPI_Conf->SPI_FLASH_BlockSize=65536;	//块大小
////		}
////		break;
////	}
////}
/////*******************************************************************************
////*函数名			:	function
////*功能描述		:	函数功能说明
////*输入				: 
////*返回值			:	无
////*******************************************************************************/
////void SPI_FLASH_ENALBE(SPI_FLASH_TypeDef *SPI_Conf)				//SPI_FLASH_使能
////{
////	if(SPI_Conf->SPI_Flash_NSS_CsFlg==1)														//如果使用纯硬件SPI1（含CS脚），UseSPI1_flg=1，否则UseSPI1_flg=0；
////	{
////		SPI_Cmd(SPI_Conf->SPIx, ENABLE);															//使能SPI--CS自动拉低
////	}
////	else
////	{
////		
////		SPI_Cmd(SPI_Conf->SPIx, ENABLE);	//使能SPI
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		GPIO_ResetBits(SPI_Conf->SPI_CS_PORT, SPI_Conf->SPI_CS_PIN);	//CS_LOW片选使能
////																	
////	}
////	
////}
/////*******************************************************************************
////*函数名			:	function
////*功能描述		:	函数功能说明
////*输入				: 
////*返回值			:	无
////*******************************************************************************/
////void SPI_FLASH_DISALBE(SPI_FLASH_TypeDef *SPI_Conf)				//SPI_FLASH_关闭
////{	
////	if(SPI_Conf->SPI_Flash_NSS_CsFlg==1)														//如果使用纯硬件SPI1（含CS脚），UseSPI1_flg=1，否则UseSPI1_flg=0；
////	{
////		SPI_Cmd(SPI_Conf->SPIx, DISABLE);															//关闭SPI--CS自动拉高
////	}
////	else
////	{
////		SPI_Cmd(SPI_Conf->SPIx, DISABLE);															//关闭SPI
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		GPIO_SetBits(SPI_Conf->SPI_CS_PORT, SPI_Conf->SPI_CS_PIN);		//CS_HIGH禁止片选
////	}	
////}

/////*******************************************************************************
////* Function Name  : SPI_FLASH_SendByte
////* Description    : Sends a byte through the SPI interface and return the byte
////*                  received from the SPI bus.
////* Input          : byte : byte to send.
////* Output         : None
////* Return         : The value of the received byte.
////*******************************************************************************/
////u8 SPI_FLASH_WriteReadByte(SPI_FLASH_TypeDef *SPI_Conf,u8 byte)
////{
////  /* Loop while DR register in not emplty */
////  while (SPI_I2S_GetFlagStatus(SPI_Conf->SPIx, SPI_I2S_FLAG_TXE) == RESET);
////  /* Send byte through the SPI1 peripheral */
////  SPI_I2S_SendData(SPI_Conf->SPIx, byte);
////  /* Wait to receive a byte */
////  while (SPI_I2S_GetFlagStatus(SPI_Conf->SPIx, SPI_I2S_FLAG_RXNE) == RESET);
////  /* Return the byte read from the SPI bus */
////  return SPI_I2S_ReceiveData(SPI_Conf->SPIx);
////}
/////*******************************************************************************
////* Function Name  : SPI_FLASH_WriteEnable
////* Description    : Enables the write access to the FLASH.
////* Input          : None
////* Output         : None
////* Return         : None
////*******************************************************************************/
////void SPI_FLASH_WriteEnable(SPI_FLASH_TypeDef *SPI_Conf)		//0x06
////{
////	u8 Status=0;
////	SPI_FLASH_WaitForWriteEnd(SPI_Conf);			//等待FLASH写完成	
////	
////	SPI_FLASH_ENALBE(SPI_Conf);								//SPI_FLASH_使能
////	SPI_Cmd(SPI_Conf->SPIx, ENABLE);					//使能SPI
////	SPI_FLASH_WriteReadByte(SPI_Conf,Flash_WREN);	//0x06使能写命令
////	SPI_FLASH_DISALBE(SPI_Conf);							//SPI_FLASH_关闭
////	
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	
////	SPI_FLASH_ENALBE(SPI_Conf);										//SPI_FLASH_使能
////	SPI_FLASH_WriteReadByte(SPI_Conf,Flash_WRSR);	//0x01写状态寄存器命令
////	SPI_FLASH_WriteReadByte(SPI_Conf,0x00);				//使能写状态寄存器参数--清除写保护
////	SPI_FLASH_DISALBE(SPI_Conf);									//SPI_FLASH_关闭
////	
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	
//////	SPI_FLASH_ENALBE(SPI_Conf);										//SPI_FLASH_使能
//////	SPI_Cmd(SPI_Conf->SPIx, ENABLE);							//使能SPI
//////	SPI_FLASH_WriteReadByte(SPI_Conf,Flash_WREN);	//0x06使能写命令
//////	SPI_FLASH_DISALBE(SPI_Conf);									//SPI_FLASH_关闭
////	
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	
////	do																				//等待写允许
////	{
////		SPI_FLASH_ENALBE(SPI_Conf);										//SPI_FLASH_使能
////		SPI_Cmd(SPI_Conf->SPIx, ENABLE);							//使能SPI
////		SPI_FLASH_WriteReadByte(SPI_Conf,Flash_WREN);	//0x06使能写命令
////		SPI_FLASH_DISALBE(SPI_Conf);									//SPI_FLASH_关闭
////		
////		SPI_FLASH_ENALBE(SPI_Conf);													//SPI_FLASH_使能		
////		SPI_FLASH_WriteReadByte(SPI_Conf,Flash_RDSR);						//写入读状态命令 */		
////		Status = SPI_FLASH_WriteReadByte(SPI_Conf,Dummy_Byte);	//读状态 */			
////		SPI_FLASH_DISALBE(SPI_Conf);									//SPI_FLASH_关闭
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////	}while((Status&WEL_Flag)!=WEL_Flag);
////	
//////	SPI_FLASH_DISALBE(SPI_Conf);										//SPI_FLASH_关闭
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////}
/////*******************************************************************************
////* Function Name  : SPI_FLASH_WriteDisable
////* Description    : Enables the write access to the FLASH.
////* Input          : None
////* Output         : None
////* Return         : None
////*******************************************************************************/
////void SPI_FLASH_WriteDisable(SPI_FLASH_TypeDef *SPI_Conf)			//0x写保护--写禁止
////{
////	u32 Status=0x00;

////	SPI_FLASH_ENALBE(SPI_Conf);															//SPI_FLASH_使能	
////	SPI_FLASH_WriteReadByte(SPI_Conf,Flash_RDSR);						//0x05写入读状态命令
////	Status = SPI_FLASH_WriteReadByte(SPI_Conf,Dummy_Byte);	//读取状态值	
////	SPI_FLASH_DISALBE(SPI_Conf);														//SPI_FLASH_关闭
////	
////	Status=Status|0x1C;														//所有的保护
////	
////  SPI_FLASH_ENALBE(SPI_Conf);										//SPI_FLASH_使能
////	
////	SPI_FLASH_WriteReadByte(SPI_Conf,Flash_WRDI);	//发送写保护命令
////	SPI_FLASH_WriteStatus(SPI_Conf,Status);				//写Flash状态寄存器
////	
////	SPI_FLASH_DISALBE(SPI_Conf);									//SPI_FLASH_关闭
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	
////}
/////*******************************************************************************
////* Function Name  : SPI_FLASH_WaitForWriteEnd
////* Description    : Polls the status of the Write In Progress (WIP) flag in the
////*                  FLASH's status  register  and  loop  until write  opertaion
////*                  has completed.
////* Input          : None
////* Output         : None
////* Return         : None
////*******************************************************************************/
////void SPI_FLASH_WaitForWriteEnd(SPI_FLASH_TypeDef *SPI_Conf)	//等待FLASH写完成
////{
////  u8 FLASH_Status = 0;

////  SPI_FLASH_ENALBE(SPI_Conf);										//SPI_FLASH_使能
////	
////  SPI_FLASH_WriteReadByte(SPI_Conf,Flash_RDSR);						//发送读状态寄存器命令
////  do			//循环读取状态值，直到芯片退出忙状态
////  {
////    FLASH_Status = SPI_FLASH_WriteReadByte(SPI_Conf,Dummy_Byte);		//循环读取状态值，
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////		__nop();
////  }
////  while ((FLASH_Status & WIP_Flag) == WIP_Flag); 			

////	SPI_FLASH_DISALBE(SPI_Conf);									//SPI_FLASH_关闭
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////	__nop();
////}
/////*******************************************************************************
////*函数名			:	function
////*功能描述		:	函数功能说明
////*输入				: 
////*返回值			:	无
////*******************************************************************************/
////u8 SPI_FLASH_ReadStatus(SPI_FLASH_TypeDef *SPI_Conf)
////{
////	u32 FLASH_Status=0x00;	
////	if((SPI_Conf->SPI_FLASH_Status)==SPI_FLASH_IDLE)						//在FLASH非忙状态时读状态
////	{
////		//--------------读FLASH状态
////		SPI_FLASH_ENALBE(SPI_Conf);																//SPI_FLASH_使能		
////		SPI_FLASH_WriteReadByte(SPI_Conf,Flash_RDSR);							//0x05写入读状态命令
////		FLASH_Status = SPI_FLASH_WriteReadByte(SPI_Conf,Dummy_Byte);	//读取状态值		
////		SPI_FLASH_DISALBE(SPI_Conf);															//SPI_FLASH_关闭
////		
////		//--------------根据FLASH之前的工作状态复位相关标志
////		//1）-----------写状态后，检测是否定完成
////		switch(SPI_Conf->SPI_FLASH_Request)		//根据请求检查状态后设置标志位
////		{
////			case	SPI_FLASH_qWRITED	:			//等待写完成
////			{
////				if((FLASH_Status & WIP_Flag) != WIP_Flag)							//复位写入完成标志，工作状态为写入时，检查是否写入完成，如果完成，状态进入空闲状态
////				{
////					SPI_Conf->SPI_FLASH_Status=SPI_FLASH_WIP;						//写入完成，进入空闲状态
////					return FLASH_Status;																//返回状态值
////				}
////			}	break;
////			case	SPI_FLASH_qWRITEN	:			//写请求
////			{
////				if((FLASH_Status & WIP_Flag) != WIP_Flag)							//复位写入完成标志，工作状态为写入时，检查是否写入完成，如果完成，状态进入空闲状态
////				{
////					SPI_Conf->SPI_FLASH_Status=SPI_FLASH_WIP;						//写入完成，进入空闲状态
////					return FLASH_Status;																//返回状态值
////				}
////			}	break;
////		}
////		
////		
////	}
////	else
////	{
////		return 0;																					//返回状态值
////	}
////	return 0;
////}
/////*******************************************************************************
////*函数名			:	function
////*功能描述		:	函数功能说明
////*输入				: 
////*返回值			:	无
////*******************************************************************************/
////void SPI_FLASH_WriteStatus(SPI_FLASH_TypeDef *SPI_Conf,u8 Status)		//写Flash状态寄存器
////{
////	SPI_FLASH_WriteEnable(SPI_Conf);					//0x06写使能
////	
////	SPI_FLASH_ENALBE(SPI_Conf);								//SPI_FLASH_使能
////	
////	SPI_FLASH_WriteReadByte(SPI_Conf,Flash_WRSR);	//0x01写状态寄存器命令
////	SPI_FLASH_WriteReadByte(SPI_Conf,Status);			//写状态寄存器参数
////	
////	SPI_FLASH_DISALBE(SPI_Conf);							//SPI_FLASH_关闭	
////}

/////*******************************************************************************
////* Function Name  : SPI_FLASH_ReadID
////* Description    : Reads FLASH identification.
////* Input          : None
////* Output         : None
////* Return         : FLASH identification
////*******************************************************************************/
////u32 SPI_FLASH_ReadID(SPI_FLASH_TypeDef *SPI_Conf)
////{
////  u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;
////	
////  SPI_FLASH_ENALBE(SPI_Conf);												//SPI_FLASH_使能
////	
////  SPI_FLASH_WriteReadByte(SPI_Conf,Flash_RDID);			//0x9F,读设备信息命令

////  Temp0 = SPI_FLASH_WriteReadByte(SPI_Conf,Dummy_Byte);
////  Temp1 = SPI_FLASH_WriteReadByte(SPI_Conf,Dummy_Byte);
////  Temp2 = SPI_FLASH_WriteReadByte(SPI_Conf,Dummy_Byte);


////	SPI_FLASH_DISALBE(SPI_Conf);											//SPI_FLASH_关闭	  

////  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;	

////  return Temp;
////}


/////*******************************************************************************
////* Function Name  : SPI_FLASH_SectorErase
////* Description    : Erases the specified FLASH sector.
////* Input          : SectorAddr: address of the sector to erase.
////* Output         : None
////* Return         : None
////*******************************************************************************/
////void SPI_FLASH_SectorErase(SPI_FLASH_TypeDef *SPI_Conf,u32 SectorAddr)
////{	
////  /* Send write enable instruction */
////	SPI_FLASH_WaitForWriteEnd(SPI_Conf);			//等待FLASH写完成
////  SPI_FLASH_WriteEnable(SPI_Conf);					//写使能
////	
////  SPI_FLASH_ENALBE(SPI_Conf);								//SPI_FLASH_使能
////  /* Send Sector Erase instruction */
////  SPI_FLASH_WriteReadByte(SPI_Conf,Flash_SE);
////  /* Send SectorAddr high nibble address byte */
////  SPI_FLASH_WriteReadByte(SPI_Conf,(SectorAddr & 0xFF0000) >> 16);
////  /* Send SectorAddr medium nibble address byte */
////  SPI_FLASH_WriteReadByte(SPI_Conf,(SectorAddr & 0xFF00) >> 8);
////  /* Send SectorAddr low nibble address byte */
////  SPI_FLASH_WriteReadByte(SPI_Conf,SectorAddr & 0xFF);
////	
////	SPI_FLASH_DISALBE(SPI_Conf);							//SPI_FLASH_关闭;
////	
////  /* Wait the end of Flash writing */
////  SPI_FLASH_WaitForWriteEnd(SPI_Conf);	
////}

/////*******************************************************************************
////* Function Name  : SPI_FLASH_BulkErase
////* Description    : Erases the entire FLASH.
////* Input          : None
////* Output         : None
////* Return         : None
////*******************************************************************************/
////void SPI_FLASH_BulkErase(SPI_FLASH_TypeDef *SPI_Conf)
////{
////  SPI_FLASH_WriteEnable(SPI_Conf);					//FLASH写使能--等待空闲
////	SPI_FLASH_WaitForWriteEnd(SPI_Conf);			//等待FLASH写完成--等待空闲
////	
////	SPI_FLASH_ENALBE(SPI_Conf);								//SPI_FLASH_使能
////  SPI_FLASH_WriteReadByte(SPI_Conf,Flash_BE);		//写入块擦除命令
////	SPI_FLASH_DISALBE(SPI_Conf);							//SPI_FLASH_关闭	

////  SPI_FLASH_WaitForWriteEnd(SPI_Conf);//等待FLASH写完成--等待空闲	
////}
/////*******************************************************************************
////* Function Name  : SPI_FLASH_BulkErase
////* Description    : Erases the entire FLASH.
////* Input          : None
////* Output         : None
////* Return         : None
////*******************************************************************************/
////void SPI_FLASH_ChipErase(SPI_FLASH_TypeDef *SPI_Conf)
////{
////  /* Send write enable instruction */
////  SPI_FLASH_WriteEnable(SPI_Conf);
////	/* 等待FLASH写完成*/
////	SPI_FLASH_WaitForWriteEnd(SPI_Conf);		//等待FLASH写完成
////	/* Chip Erase */
////	/* Select the FLASH: Chip Select low */
////  SPI_FLASH_ENALBE(SPI_Conf);												//SPI_FLASH_使能
////	
////  /* Send Bulk Erase instruction  */
////  SPI_FLASH_WriteReadByte(SPI_Conf,Flash_CE);	//(unsigned char)0x60				//芯片擦除; 整片擦除; 也可以用0XC7	
////	
////	SPI_FLASH_DISALBE(SPI_Conf);											//SPI_FLASH_关闭
////	
////  /* Wait the end of Flash writing */
////  SPI_FLASH_WaitForWriteEnd(SPI_Conf);	
////}
































