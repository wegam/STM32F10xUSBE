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
#ifdef OFFLINE_ISP							//如果定义了SPI_FLASH 此功能生效

#include "OFFLINE_ISP.H"

#include "SPI_FLASH.H"
#include "USART_ISP.H"

#include "STM32_PWM.H"
#include "STM32_SYS.H"
#include "STM32_GPIO.H"
#include "STM32_SYSTICK.H"
#include "STM32_USART.H"
#include "STM32_WDG.H"

#include "string.h"


#define 	USART_BufferSize				512

//ISP_Conf_TypeDef 	ISP_Conf;
//SPI_FLASH_TypeDef	SPI_FLASH;

u8 RxdBuffe[USART_BufferSize]={0};
u8 RevBuffe[USART_BufferSize]={0};
u16 PWM_Ratio=0;
u8 flag=0;
u32 timecunt=0;		//空闲计时

typedef struct
{
	ISP_Conf_TypeDef 	ISP_Conf;
	SPI_FLASH_TypeDef	SPI_FLASH;
	
}OFFLINE_ISP_TypeDef;

OFFLINE_ISP_TypeDef OFFLINE_Cof;

void OFFLINE_ISP_Conf(void);
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void OFFLINE_ISP_Configuration(void)
{
	SYS_Configuration();											//系统配置 STM32_SYS.H	
	GPIO_DeInitAll();													//将所有的GPIO关闭----V20170605
	PWM_OUT(TIM2,PWM_OUTChannel1,1,900);			//PWM设定-20161127版本
	
	OFFLINE_ISP_Conf();
	
	IWDG_Configuration(2000);					//独立看门狗配置---参数单位ms	
	SysTick_Configuration(10);				//系统嘀嗒时钟配置72MHz,单位为uS
	timecunt=0;		//空闲计时
	OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Request=SPI_FLASH_qCERASE;
}
/*******************************************************************************
* 函数名		:
* 功能描述	:
* 输入		:
* 输出		:
* 返回 		:
*******************************************************************************/
void OFFLINE_ISP_Server(void)
{
	u16 RxNum=0;
	IWDG_Feed();								//独立看门狗喂狗	
	timecunt++;		//空闲计时
	if(timecunt>=0xFFFFFFF5)
	{
	}
//	RxNum=USART_ReadBufferIDLE(USART1,(u32*)RevBuffe,(u32*)RxdBuffe);	//串口空闲模式读串口接收缓冲区，如果有数据，将数据拷贝到RevBuffer,并返回接收到的数据个数，然后重新将接收缓冲区地址指向RxdBuffer
	if(RxNum)
	{
		Usart_ISP_Process(&(OFFLINE_Cof.ISP_Conf));
		SPI_FLASH_Process(&(OFFLINE_Cof.SPI_FLASH));			//FLASH数据处理：所有的FLASH对外操作接口
		
//		SPI_FLASH_BufferWrite	(&(OFFLINE_Cof.SPI_FLASH),	RevBuffe, OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_WriteAdrr, RxNum);	//FLASH写缓冲数据
//		SPI_FLASH_BufferRead	(&(OFFLINE_Cof.SPI_FLASH),	RevBuffe, OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_WriteAdrr, RxNum);
//		OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_WriteAdrr+=RxNum;
	}
	OFFLINE_ISP_StatusProcess();		//状态处理
	Usart_ISP_Process(&(OFFLINE_Cof.ISP_Conf));
	SPI_FLASH_Process(&(OFFLINE_Cof.SPI_FLASH));			//FLASH数据处理：所有的FLASH对外操作接口
	
//	PWM_Ratio+=1;
//	if(PWM_Ratio>=3600)
//	{
//		flag=1;
//		PWM_Ratio=0;
//	}
//	if(flag==0)
//	{
//		PWM_Ratio++;
//		if(PWM_Ratio>=3600)
//		{
//			flag=1;
////			PWM_Ratio=0;
//		}
//	}
//	else
//	{
//		PWM_Ratio--;
//		if(PWM_Ratio<=1800)
//		{
//			flag=0;
//		}
//	}
//	
//	SetPWM_Ratio(PWM_Ratio);		//设置占空比
	
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void OFFLINE_ISP_StatusProcess(void)		//状态处理
{
	if(Usart_ISP_GetSlaveStatus(&(OFFLINE_Cof.ISP_Conf))==ISP_STATUS_WaitReadData)	//返回从机状态值
	{
//		memcpy((OFFLINE_Cof.ISP_Conf.ISP_DATA.ISP_TvBuffer), OFFLINE_Cof.ISP_Conf.ISP_DATA.ISP_RvBuffer, OFFLINE_Cof.ISP_Conf.ISP_DATA.WriteLen+1);	//复制数据
		if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus==SPI_FLASH_IDLE)
		{
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_READ;
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_ReadAdrr=OFFLINE_Cof.ISP_Conf.ISP_DATA.ReadAddr-OFFLINE_Cof.ISP_Conf.ISP_DATA.OffsetAddr+OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_OffsetAdrr;
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_LenghToRead=OFFLINE_Cof.ISP_Conf.ISP_DATA.SendLen+1;
		}
		else if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Steps==Step_IDLE)
		{
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_IDLE;
			memcpy(OFFLINE_Cof.ISP_Conf.ISP_DATA.ISP_TvBuffer, OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MISO_Buffer, OFFLINE_Cof.ISP_Conf.ISP_DATA.SendLen+1);	//复制数据
			Usart_ISP_SetSlaveStatus(&(OFFLINE_Cof.ISP_Conf),ISP_STATUS_WaitSData);	//设置从机状态
		}
	}
	else if(Usart_ISP_GetSlaveStatus(&(OFFLINE_Cof.ISP_Conf))==ISP_STATUS_WaitWrite)	//ISP等待写数据
	{
		timecunt=0;
		if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus==SPI_FLASH_IDLE)
		{
			
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_WRITE;
			memcpy(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MOSI_Buffer,OFFLINE_Cof.ISP_Conf.ISP_DATA.ISP_RvBuffer,  OFFLINE_Cof.ISP_Conf.ISP_DATA.WriteLen+1);	//复制数据
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_WriteAdrr=OFFLINE_Cof.ISP_Conf.ISP_DATA.WriteAddr-OFFLINE_Cof.ISP_Conf.ISP_DATA.OffsetAddr+OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_OffsetAdrr;			
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_LenghToWrite=OFFLINE_Cof.ISP_Conf.ISP_DATA.WriteLen+1;
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_SumReceived+=OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_LenghToWrite;			//数量增加
		}
		else if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Steps==Step_WRITE)
		{
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Steps=Step_IDLE;
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Request=SPI_FLASH_qIDLE;
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_IDLE;
			
			Usart_ISP_SetSlaveStatus(&(OFFLINE_Cof.ISP_Conf),ISP_STATUS_WaitWrited);	//设置从机状态--ISP等待写入完成
			
		}
//		Usart_ISP_SetSlaveStatus(&(OFFLINE_Cof.ISP_Conf),ISP_STATUS_WaitWrited);	//设置从机状态--ISP等待写入完成

//		memcpy((OFFLINE_Cof.ISP_Conf.ISP_DATA.ISP_TvBuffer), OFFLINE_Cof.ISP_Conf.ISP_DATA.ISP_RvBuffer, OFFLINE_Cof.ISP_Conf.ISP_DATA.WriteLen+1);	//复制数据
//		Usart_ISP_SetSlaveStatus(&(OFFLINE_Cof.ISP_Conf),ISP_STATUS_WaitWrited);	//设置从机状态--ISP等待写入完成
	}
	else if(Usart_ISP_GetSlaveStatus(&(OFFLINE_Cof.ISP_Conf))==ISP_STATUS_Eraseing)	//ISP正在擦除---SPI_FLASH执行擦除操作
	{
//		if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus==SPI_FLASH_IDLE)
//		{
//			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_ERASE;		//擦除
//			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Request=SPI_FLASH_qCERASE;	//整片擦除
//		}
//		else if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Request==SPI_FLASH_qIDLE)
//		{
//			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_IDLE;
//			Usart_ISP_SetSlaveStatus(&(OFFLINE_Cof.ISP_Conf),ISP_STATUS_WaitErased);			//设置从机状态--等待擦除完成
//		}
		Usart_ISP_SetSlaveStatus(&(OFFLINE_Cof.ISP_Conf),ISP_STATUS_WaitErased);			//设置从机状态--等待擦除完成
		
	}
}

/*******************************************************************************
* 函数名		:
* 功能描述	:
* 输入		:
* 输出		:
* 返回 		:
*******************************************************************************/
void OFFLINE_ISP_Conf(void)
{
	OFFLINE_Cof.ISP_Conf.USARTx=USART1;
	
	SPI_FLASH_Conf(&(OFFLINE_Cof.SPI_FLASH));
	Usart_ISP_Cof(&(OFFLINE_Cof.ISP_Conf));	
}




#endif
