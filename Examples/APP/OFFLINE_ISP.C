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
#include "STM32_TOOLS.H"

#include "STM32_PWM.H"
#include "STM32_SYS.H"
#include "STM32_GPIO.H"
#include "STM32_SYSTICK.H"
#include "STM32_USART.H"
#include "STM32_WDG.H"

#include "string.h"

#define 	STM32_CDC

//#define 	USART_BufferSize				512



//ISP_Conf_TypeDef 	ISP_Conf;
//SPI_FLASH_TypeDef	SPI_FLASH;

//u8 RxdBuffe[USART_BufferSize]={0};
//u8 RevBuffe[USART_BufferSize]={0};
u16 PWM_Ratio=0;
u8 flag=0;
u32 timecunt=0;		//空闲计时

typedef struct
{
	ISP_Conf_TypeDef 	ISP_Conf;
	BSP_Conf_TypeDef	BSP_Conf;
	
	SPI_FLASH_TypeDef	SPI_FLASH;
	
}OFFLINE_ISP_TypeDef;

OFFLINE_ISP_TypeDef OFFLINE_Cof;


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
//	OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Request=SPI_FLASH_qCERASE;
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
//	u16 RxNum=0;
	IWDG_Feed();								//独立看门狗喂狗	
	timecunt++;		//空闲计时
	if(timecunt>=0xFFFFFFF5)
	{
	}
	BSP_Process(&(OFFLINE_Cof.BSP_Conf));								//事件处理函数
	
//	OFFLINE_ISP_StatusProcess();		//状态处理
//	Usart_ISP_Process(&(OFFLINE_Cof.ISP_Conf));
//	SPI_FLASH_Process(&(OFFLINE_Cof.SPI_FLASH));			//FLASH数据处理：所有的FLASH对外操作接口
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
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_LenghToRead=OFFLINE_Cof.ISP_Conf.ISP_DATA.USARTSendLen+1;
		}
		else if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Steps==Step_IDLE)	//读数据完成
		{
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_IDLE;
			memcpy(OFFLINE_Cof.ISP_Conf.ISP_DATA.ISP_TvBuffer, OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MISO_Buffer, OFFLINE_Cof.ISP_Conf.ISP_DATA.USARTSendLen+1);	//复制数据
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
	else if(OFFLINE_Cof.ISP_Conf.Connected==ISP_STATUS_WaitConnect||OFFLINE_Cof.ISP_Conf.Connected==ISP_STATUS_Connectting)			//ISP等待连接（作为从机)
	{
		if(OFFLINE_Cof.ISP_Conf.Connected==ISP_STATUS_WaitConnect)
		{
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Request=SPI_FLASH_qCERASE;
			OFFLINE_Cof.ISP_Conf.Connected=ISP_STATUS_Connectting;						//ISP正在连接（作为从机)---执行格式化
		}
		else if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Request==SPI_FLASH_qIDLE)
		{
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_IDLE;
			OFFLINE_Cof.ISP_Conf.Connected=ISP_STATUS_Connectted;						//ISP正在连接（作为从机)---执行格式化
//			Usart_ISP_SetSlaveStatus(&(OFFLINE_Cof.ISP_Conf),ISP_STATUS_WaitErased);			//设置从机状态--等待擦除完成
		}
	}
	else if(Usart_ISP_GetSlaveStatus(&(OFFLINE_Cof.ISP_Conf))==ISP_STATUS_WaitGoAddr)
	{
		if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus==SPI_FLASH_IDLE)
		{
			u32 temp=0;
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Request=SPI_FLASH_qWRITE;
//			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_SumReceived;																							//总共接收到的数据个数---4字节0x00-0x03存储
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_ISPStartAddr=OFFLINE_Cof.ISP_Conf.ISP_DATA.OffsetAddr;		//烧录时的起始地址---4字节0x04-0x07存储
			
			
			temp=OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_SumReceived;																							//总共接收到的数据个数---4字节0x00-0x03存储
			memset(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MOSI_Buffer, 0xFF, 256);
			
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MOSI_Buffer[0]=0xFF&(temp>>24);
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MOSI_Buffer[1]=0xFF&(temp>>16);
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MOSI_Buffer[2]=0xFF&(temp>>8);
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MOSI_Buffer[3]=0xFF&(temp>>0);
			
			
			
			temp=OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_ISPStartAddr;																							//烧录时的起始地址---4字节0x04-0x07存储
			
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MOSI_Buffer[4]=0xFF&(temp>>24);
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MOSI_Buffer[5]=0xFF&(temp>>16);
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MOSI_Buffer[6]=0xFF&(temp>>8);
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MOSI_Buffer[7]=0xFF&(temp>>0);

			
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_LenghToWrite=8;		//待写入长度
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_WriteAdrr=0;				//FLASH起始地址
		}
		else if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Steps==Step_WRITE)
		{
//			u32 timedelay=0;
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Steps=Step_IDLE;
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Request=SPI_FLASH_qIDLE;
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_IDLE;
			Usart_ISP_ACK(&(OFFLINE_Cof.ISP_Conf));	//ISP应答
			Usart_ISP_SetSlaveStatus(&(OFFLINE_Cof.ISP_Conf),ISP_STATUS_WaitReset);	//设置从机状态
//			while(timedelay++<0xFFFF);
//			Usart_ISP_Reset(&(OFFLINE_Cof.ISP_Conf));						//重置编程器---恢复所有参数为默认值
//			Usart_ISP_SetSlaveStatus(&(OFFLINE_Cof.ISP_Conf),ISP_STATUS_IDLE);	//ISP空闲状态，可以读写
		}		
	}
	else if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_SumOfData==0)//总共接收到的数据个数---4字节0x00-0x03存储//刚开机时此应该为0
	{
		if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus==SPI_FLASH_IDLE)
		{
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_READ;
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_ReadAdrr=0;				//读待写数据信息从0开始
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_LenghToRead=10;
			memset(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MISO_Buffer, 0xFF, 256);
		}
		else if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Steps==Step_IDLE)
		{
			u32 temp=0;
			temp+=OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MISO_Buffer[0];
			temp=(temp<<8)|OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MISO_Buffer[1];
			temp=(temp<<8)|OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MISO_Buffer[2];
			temp=(temp<<8)|OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MISO_Buffer[3];

			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_SumOfData=temp;			//需要写入的数据个数
			
			
			temp=0;
			temp+=OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MISO_Buffer[4];
			temp=(temp<<8)|OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MISO_Buffer[5];
			temp=(temp<<8)|OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MISO_Buffer[6];
			temp=(temp<<8)|OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MISO_Buffer[7];
			
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_ISPStartAddr=temp;			//需要写入的起始地址
			
			
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_IDLE;
			memcpy(OFFLINE_Cof.ISP_Conf.ISP_DATA.ISP_TvBuffer, OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MISO_Buffer, OFFLINE_Cof.ISP_Conf.ISP_DATA.USARTSendLen+1);	//复制数据
		}
	}
	
//-----------------------------主机
	else if(Usart_MISP_GetStatus(&(OFFLINE_Cof.ISP_Conf))==ISP_MSTATUS_GetFirmwareInf||Usart_MISP_GetStatus(&(OFFLINE_Cof.ISP_Conf))==ISP_MSTATUS_WaitFirmwareInf)
	{
		OFFLINE_Cof.ISP_Conf.ISP_MDATA.StartAddr	=OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_ISPStartAddr;		//起始地址
		OFFLINE_Cof.ISP_Conf.ISP_MDATA.GoAddr			=OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_ISPStartAddr;		//重运行地址
		OFFLINE_Cof.ISP_Conf.ISP_MDATA.FirmwareLen=OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_SumOfData;			//需要ISP下载的总数据大小
		OFFLINE_Cof.ISP_Conf.ISP_MASTER_STATUS=ISP_MSTATUS_GetedFirmwareInf;			//状态---已获取到固件信息
	}
	else if(Usart_MISP_GetStatus(&(OFFLINE_Cof.ISP_Conf))==ISP_MSTATUS_ReadFirmware||Usart_MISP_GetStatus(&(OFFLINE_Cof.ISP_Conf))==ISP_MSTATUS_WaitFirmware)
	{
//		OFFLINE_Cof.ISP_Conf.ISP_MASTER_STATUS=ISP_MSTATUS_GettedFirmware;							//状态---已获取到固件
//		OFFLINE_Cof.ISP_Conf.ISP_MDATA.WriteLen=OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_LenghToRead;
		if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus==SPI_FLASH_IDLE)
		{
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_READ;
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_ReadAdrr=OFFLINE_Cof.ISP_Conf.ISP_MDATA.WriteAddr-OFFLINE_Cof.ISP_Conf.ISP_MDATA.OffsetAddr+OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_OffsetAdrr;
			if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_SumOfData-OFFLINE_Cof.ISP_Conf.ISP_MDATA.SumHaveWritten>256)
			{
				OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_LenghToRead=0xFF+1;
			}
			else
			{
				OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_LenghToRead=OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_SumOfData-OFFLINE_Cof.ISP_Conf.ISP_MDATA.SumHaveWritten+1;
			}
		}
		else if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Steps==Step_IDLE)	//读数据完成
		{
			unsigned char Bcc=0xFF;
			
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_IDLE;
			OFFLINE_Cof.ISP_Conf.ISP_DATA.ISP_TvBuffer[0]=OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_LenghToRead-1;			//数据长度
			
			memcpy(&(OFFLINE_Cof.ISP_Conf.ISP_DATA.ISP_TvBuffer[1]), OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MISO_Buffer, OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_LenghToRead+1);	//复制数据
			
			Bcc=BCC8(OFFLINE_Cof.ISP_Conf.ISP_DATA.ISP_TvBuffer,OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_LenghToRead+1);		//异或校验;
			OFFLINE_Cof.ISP_Conf.ISP_DATA.ISP_TvBuffer[OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_LenghToRead+1]=Bcc;
			
			OFFLINE_Cof.ISP_Conf.ISP_MASTER_STATUS=ISP_MSTATUS_GettedFirmware;							//状态---已获取到固件
			OFFLINE_Cof.ISP_Conf.ISP_MDATA.WriteLen=OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_LenghToRead+2;
		}
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
	
	OFFLINE_Cof.ISP_Conf.RESET_CTL_PORT=GPIOB;
	OFFLINE_Cof.ISP_Conf.RESET_CTL_Pin=GPIO_Pin_0;
	
	OFFLINE_Cof.ISP_Conf.BOOT0_CTL_PORT=GPIOA;
	OFFLINE_Cof.ISP_Conf.BOOT0_CTL_Pin=GPIO_Pin_8;
	
	OFFLINE_Cof.BSP_Conf.BSP_Port.USARTx=USART1;
	OFFLINE_Cof.BSP_Conf.BSP_Port.BOOT0_CTL_PORT=GPIOB;
	OFFLINE_Cof.BSP_Conf.BSP_Port.BOOT0_CTL_Pin=GPIO_Pin_0;
	OFFLINE_Cof.BSP_Conf.BSP_Port.RESET_CTL_PORT=GPIOA;
	OFFLINE_Cof.BSP_Conf.BSP_Port.RESET_CTL_Pin=GPIO_Pin_8;
	
	BSP_Configuration(&(OFFLINE_Cof.BSP_Conf));									//配置函数
	
	SPI_FLASH_Conf(&(OFFLINE_Cof.SPI_FLASH));
//	Usart_ISP_Cof(&(OFFLINE_Cof.ISP_Conf));	
}




#endif
