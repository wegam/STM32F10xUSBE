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

#include "USART_ISP.H"

#include "STM32_USART.H"
#include "STM32_TOOLS.H"

//#define USB_TEST_BD				//USB_TEST板
//#define SPI_FLASH_OSTL
//#define CMSIS_CDC_BD			//CMSIS-CDC板

#include "string.h"
//#include "stdlib.h"
//#include "stdio.h"

#include "stm32f10x_type.h"
//#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"
//#include "stm32f10x_nvic.h"

//#include "STM32_WOW.H"
//#include "STM32_PWM.H"
//#include "STM32_SYS.H"
//#include "STM32_SPI.H"
//#include "STM32_GPIO.H"
//#include "STM32_SYSTICK.H"
//#include "STM32_USART.H"

//#define	 Usart_ISP_Simulation 		//模拟从机
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void Usart_ISP_Cof(ISP_Conf_TypeDef *ISP_Conf)
{
	USART_DMA_ConfigurationEV	(ISP_Conf->USARTx,115200,(u32*)ISP_Conf->ISP_DATA.ISP_RxBuffer,ISP_BufferSize);	//USART_DMA配置--查询方式，不开中断
	Usart_ISP_Reset(ISP_Conf);																																								//重置编程器---恢复所有参数为默认值
	ISP_Conf->ISP_FUN=ISP_SLAVE;			//测试---将模块设置为从机
}
/*******************************************************************************
* 函数名			:	Usart_ISP_CommandSend
* 功能描述		:	串口编程发送命令程序
* 输入			: void
* 返回值			: void
*******************************************************************************/
void Usart_ISP_Process(ISP_Conf_TypeDef *ISP_Conf)
{
	if(ISP_Conf->ISP_FUN==ISP_SLAVE)				//模块作为从机----更新程序
	{
		Usart_ISP_SlaveProcess(ISP_Conf);			//模块作为从机时的处理程序
	}
	else if(ISP_Conf->ISP_FUN==ISP_MASTER)	//模块作为主机----烧录目标板
	{
		Usart_ISP_SlaveProcess(ISP_Conf);			//模块作为主机时的处理程序
	}
	else
	{
		Usart_ISP_CheckFun(ISP_Conf);					//检测ISP模块工作模式---空闲时检测
	}
}
/*******************************************************************************
* 函数名			:	Usart_ISP_SlaveProcess
* 功能描述		:	模块作为从机时的处理程序
* 输入			: void
* 返回值			: void
*******************************************************************************/
void Usart_ISP_SlaveProcess(ISP_Conf_TypeDef *ISP_Conf)		//模块作为从机时的处理程序
{
	ISP_Conf->ISP_DATA.ReceivedLen=USART_ReadBufferIDLE(ISP_Conf->USARTx,(u32*)ISP_Conf->ISP_DATA.ISP_RvBuffer,(u32*)ISP_Conf->ISP_DATA.ISP_RxBuffer);	//串口空闲模式读串口接收缓冲区，如果有数据，将数据拷贝到RevBuffer,并返回接收到的数据个数，然后重新将接收缓冲区地址指向RxdBuffer
	if(ISP_Conf->ISP_DATA.ReceivedLen)
	{
		ISP_Conf->OverRunTime=0;									//超时时间
		if(ISP_Conf->ISP_DATA.ReceivedLen==1)			//单字节----识别波特率
		{
			if(ISP_Conf->ISP_DATA.ISP_RvBuffer[0]==0x7F)	//自举应答
			{
				Usart_ISP_Reset(ISP_Conf);														//重置编程器---恢复所有参数为默认值
				Usart_ISP_SetSlaveStatus(ISP_Conf,ISP_STATUS_WaitCommand);		//ISP等待命令（作为从机)
				Usart_ISP_ACK(ISP_Conf);															//ISP应答
			}
		}
		else if(ISP_Conf->ISP_DATA.ReceivedLen==2&&ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitCommand)				//2字节---接收命令过程
		{
			Usart_ISP_CommandRead(ISP_Conf);			//串口接收命令（主机->从机)
		}
		else if((ISP_Conf->ISP_DATA.ReceivedLen==5&&(ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitReadAddr||ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitWAddr||ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitGoAddr))||ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitEraseAddr)				//5个字节---地址类数据接收地址过程
		{
			Usart_ISP_GetAddr(ISP_Conf);						//ISP获取写数据起始地址(主机->从机)
		}
		else if(ISP_Conf->ISP_DATA.ReceivedLen==2&&ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitLengh)				//2字节---接收待处理数据长度，读数据时，传入地址后再传入待读取的数据长度
		{
			Usart_ISP_GetLengh(ISP_Conf);					//ISP获取需要读取的数据长度(主机->从机)，读数据时，传入地址后再传入待读取的数据长度
		}
		else if(ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitWData)	//ISP等待接收待写入数据,写数据时，传完地址后等待传入数据，再执行写操作
		{
			Usart_ISP_WriteMemory(ISP_Conf);			//ISP写数据（主机->从机）写数据时，传完地址后等待传入数据，再执行写操作
		}
				
	}
	else if(ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitReadData)	//等待数据读取完成
	{
		Usart_ISP_ReadMemory(ISP_Conf);															//ISP读数据（从机->主机）
	}	
	else if(ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitSData)			//ISP等待发送数据
	{
		Usart_ISP_SendBuffer(ISP_Conf);															//ISP上传数据（从机->主机)
	}
	else if(ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitErase||ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitErased)		//ISPISP等待擦除
	{
		Usart_ISP_Erase(ISP_Conf);						//ISP擦除操作，接收到擦除命令后，应答，等待输入待擦除地址（全部擦除或者页擦除）等待擦除完成，完成后应答
	}
	else if(ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitWrited)	//ISP等待接收待写入数据,写数据时，传完地址后等待传入数据，再执行写操作
	{
		Usart_ISP_WriteMemory(ISP_Conf);			//ISP写数据（主机->从机）写数据时，传完地址后等待传入数据，再执行写操作
	}
	else	if(ISP_Conf->ISP_SLAVE_STATUS!=ISP_STATUS_IDLE)				//非初始状态时超时复位
	{
		ISP_Conf->OverRunTime=ISP_Conf->OverRunTime+1;					//超时时间
		if(ISP_Conf->OverRunTime>=5000000)		//约5秒
		Usart_ISP_Reset(ISP_Conf);				//重置编程器---恢复所有参数为默认值
	}
//	ISP_Conf->ISP_DATA.NumHaveRead=0;	//接收的数据个数
}
/*******************************************************************************
* 函数名			:	Usart_ISP_MasterProcess
* 功能描述		:	模块作为主机时的处理程序
* 输入			: void
* 返回值			: void
*******************************************************************************/
void Usart_ISP_MasterProcess(ISP_Conf_TypeDef *ISP_Conf)		//模块作为主机时的处理程序
{
}
/*******************************************************************************
* 函数名			:	Usart_ISP_CheckFun
* 功能描述		:	检测ISP模块工作模式---空闲时检测
* 输入			: void
* 返回值			: void
*******************************************************************************/
void Usart_ISP_CheckFun(ISP_Conf_TypeDef *ISP_Conf)				//检测ISP模块工作模式---空闲时检测
{
	Usart_ISP_Reset(ISP_Conf);	//重置编程器---恢复所有参数为默认值---模式切换后复位
}
/*******************************************************************************
* 函数名			:	Usart_ISP_ACK
* 功能描述		:	ISP应答
* 输入			: void
* 返回值			: void
*******************************************************************************/
void Usart_ISP_ACK(ISP_Conf_TypeDef *ISP_Conf)	//ISP应答
{
	ISP_Conf->ISP_DATA.Command[0]=ISP_ANSWER_ACK;
	USART_DMASend(ISP_Conf->USARTx,(u32*)ISP_Conf->ISP_DATA.Command,1);	//串口DMA发送程序
}
/*******************************************************************************
* 函数名			:	Usart_ISP_NACK
* 功能描述		:	ISP不应答
* 输入			: void
* 返回值			: void
*******************************************************************************/
void Usart_ISP_NACK(ISP_Conf_TypeDef *ISP_Conf)		//ISP不应答
{
	ISP_Conf->ISP_DATA.Command[0]=ISP_ANSWER_NACK;
	USART_DMASend(ISP_Conf->USARTx,(u32*)ISP_Conf->ISP_DATA.Command,1);	//串口DMA发送程序
}
/*******************************************************************************
* 函数名			:	Usart_ISP_CommandSend
* 功能描述		:	串口编程发送命令程序
* 输入			: void
* 返回值			: void
*******************************************************************************/
void Usart_ISP_CommandSend(ISP_Conf_TypeDef *ISP_Conf,unsigned char Command)	//串口编程发送命令程序
{
	ISP_Conf->ISP_DATA.Command[0]=Command;
	ISP_Conf->ISP_DATA.Command[1]=Command^0XFF;
	USART_DMASend(ISP_Conf->USARTx,(u32*)ISP_Conf->ISP_DATA.Command,2);	//串口DMA发送程序
}
/*******************************************************************************
* 函数名			:	Usart_ISP_CommandSend
* 功能描述		:	串口编程发送命令程序
* 输入			: void
* 返回值			: void
*******************************************************************************/
void Usart_ISP_CommandRead(ISP_Conf_TypeDef *ISP_Conf)			//串口接收命令（主机->从机)
{
//	unsigned short rxNum=0;
	unsigned char C0=ISP_Conf->ISP_DATA.ISP_RvBuffer[0];
	unsigned char C1=ISP_Conf->ISP_DATA.ISP_RvBuffer[1];
	C1=C1^0XFF;
	if(C0!=C1)
	{
		return;
	}
	if(C0==ISP_COMMAND_Get)			//获取当前自举程序版本及允许使用的命令
	{
		ISP_Conf->ISP_DATA.ISP_TvBuffer[0]=ISP_ANSWER_ACK;			//应答
		ISP_Conf->ISP_DATA.ISP_TvBuffer[1]=0x0B;								//长度
		ISP_Conf->ISP_DATA.ISP_TvBuffer[2]=0x22;								//版本
		ISP_Conf->ISP_DATA.ISP_TvBuffer[3]=ISP_COMMAND_Get;		//Get 命令
		ISP_Conf->ISP_DATA.ISP_TvBuffer[4]=ISP_COMMAND_GetVS;	//Get Version and Read Protection Status
		ISP_Conf->ISP_DATA.ISP_TvBuffer[5]=ISP_COMMAND_GetID;	//Get ID
		ISP_Conf->ISP_DATA.ISP_TvBuffer[6]=ISP_COMMAND_RM;			//Read Memory 命令
		ISP_Conf->ISP_DATA.ISP_TvBuffer[7]=ISP_COMMAND_Go;			//Go 命令
		ISP_Conf->ISP_DATA.ISP_TvBuffer[8]=ISP_COMMAND_WM;			//Write Memory 命令
		ISP_Conf->ISP_DATA.ISP_TvBuffer[9]=ISP_COMMAND_Erase;	//Erase 命令
		ISP_Conf->ISP_DATA.ISP_TvBuffer[10]=ISP_COMMAND_WP;		//Write Protect 命令
		ISP_Conf->ISP_DATA.ISP_TvBuffer[11]=ISP_COMMAND_WU;		//Write Unprotect 命令
		ISP_Conf->ISP_DATA.ISP_TvBuffer[12]=ISP_COMMAND_RP;		//Readout Protect 命令
		ISP_Conf->ISP_DATA.ISP_TvBuffer[13]=ISP_COMMAND_RU;		//Readout Unprotect 命令
		ISP_Conf->ISP_DATA.ISP_TvBuffer[14]=ISP_ANSWER_ACK;		//应答
		
		Usart_ISP_SetSlaveStatus(ISP_Conf,ISP_STATUS_WaitCommand);		//ISP等待命令（作为从机)--处理完Get命令后等待下一下命令
		ISP_Conf->ISP_DATA.SendLen=15;												//串口需要发送的数据长度
		
		memcpy(ISP_Conf->ISP_DATA.ISP_TxBuffer, ISP_Conf->ISP_DATA.ISP_TvBuffer, ISP_Conf->ISP_DATA.SendLen);
		USART_DMASend(ISP_Conf->USARTx,(u32*)ISP_Conf->ISP_DATA.ISP_TxBuffer,ISP_Conf->ISP_DATA.SendLen);	//串口DMA发送程序	
			
	}
	else if(C0==ISP_COMMAND_GetVS)	//获取自举程序版本及 Flash 的读保护状态
	{
		ISP_Conf->ISP_DATA.ISP_TvBuffer[0]=ISP_ANSWER_ACK;		//应答
		ISP_Conf->ISP_DATA.ISP_TvBuffer[1]=0x22;							//自举程序版本（0 < 版本 ≤ 255），示例：0x10 = 版本 1.0
		ISP_Conf->ISP_DATA.ISP_TvBuffer[2]=0x00;							//选项字节 1：0x00，保持与通用自举程序协议的兼容性
		ISP_Conf->ISP_DATA.ISP_TvBuffer[3]=0x00;							//选项字节 2：0x00，保持与通用自举程序协议的兼容性
		ISP_Conf->ISP_DATA.ISP_TvBuffer[4]=ISP_ANSWER_ACK;		//应答
		
		Usart_ISP_SetSlaveStatus(ISP_Conf,ISP_STATUS_WaitCommand);		//ISP等待命令（作为从机)--处理完GetVS命令后等待下一下命令
		ISP_Conf->ISP_DATA.SendLen=5;												//串口需要发送的数据长度
		
		memcpy(ISP_Conf->ISP_DATA.ISP_TxBuffer, ISP_Conf->ISP_DATA.ISP_TvBuffer, ISP_Conf->ISP_DATA.SendLen);
		USART_DMASend(ISP_Conf->USARTx,(u32*)ISP_Conf->ISP_DATA.ISP_TxBuffer,ISP_Conf->ISP_DATA.SendLen);	//串口DMA发送程序	
	}
	else if(C0==ISP_COMMAND_GetID)	//获取芯片 ID
	{
		ISP_Conf->ISP_DATA.ISP_TvBuffer[0]=ISP_ANSWER_ACK;		//应答
		ISP_Conf->ISP_DATA.ISP_TvBuffer[1]=0x01;							//N = 字节数 – 1（对 STM32 N = 1），除当前字节和 ACK 之外。
		ISP_Conf->ISP_DATA.ISP_TvBuffer[2]=0x04;							//PID(1) 字节 3 = 0x04，字节 4 = 0xXX
		ISP_Conf->ISP_DATA.ISP_TvBuffer[3]=0x10;							//PID(1) 字节 3 = 0x04，字节 4 = 0xXX
		ISP_Conf->ISP_DATA.ISP_TvBuffer[4]=ISP_ANSWER_ACK;		//应答
		
		Usart_ISP_SetSlaveStatus(ISP_Conf,ISP_STATUS_WaitCommand);		//ISP等待命令（作为从机)--处理完GetID命令后等待下一下命令
		ISP_Conf->ISP_DATA.SendLen=5;												//串口需要发送的数据长度
		
		memcpy(ISP_Conf->ISP_DATA.ISP_TxBuffer, ISP_Conf->ISP_DATA.ISP_TvBuffer, ISP_Conf->ISP_DATA.SendLen);
		USART_DMASend(ISP_Conf->USARTx,(u32*)ISP_Conf->ISP_DATA.ISP_TxBuffer,ISP_Conf->ISP_DATA.SendLen);	//串口DMA发送程序	
	}
	else if(C0==ISP_COMMAND_RM)			//从应用程序指定的地址开始读取最多 256 个字节的存储器空间：读数据分五步：1-读数据命令，2-读数据起始地址，3-需要读取的长度，4-读数据过程，5-上报数据
	{
		Usart_ISP_SetSlaveStatus(ISP_Conf,ISP_STATUS_WaitReadAddr);			//ISP等待待读取地址---从机接收到读数据命令后应答，然后等待待读取数据地址
		Usart_ISP_ACK(ISP_Conf);															//ISP应答
		return;
	}
	else if(C0==ISP_COMMAND_Go)			//等待待跳转到内部 Flash 或 SRAM 内的应用程序代码地址//Go 命令用于从应用程序指定的地址开始执行已下载的代码或其它任何代码
	{
		Usart_ISP_SetSlaveStatus(ISP_Conf,ISP_STATUS_WaitGoAddr);			//接收到Go命令后，应答，然后等待开始执行起始地址
		Usart_ISP_ACK(ISP_Conf);	//ISP应答
	}
	else if(C0==ISP_COMMAND_WM)			//Write Memory:从应用程序指定的地址开始将最多 256 个字节的数据写入 RAM 或 Flash
	{
		Usart_ISP_SetSlaveStatus(ISP_Conf,ISP_STATUS_WaitWAddr);			//ISP等待待写入地址---接收到写命令后，应答，然后等待写入起始地址
		Usart_ISP_ACK(ISP_Conf);	//ISP应答
	}
	else if(C0==ISP_COMMAND_Erase)	//擦除一个到全部 Flash 页面
	{
		Usart_ISP_SetSlaveStatus(ISP_Conf,ISP_STATUS_WaitEraseAddr);	//ISP等待待擦除地址，接收到擦除命令后，应答，等待输入待擦除地址（全部擦除或者页擦除）
		Usart_ISP_ACK(ISP_Conf);	//ISP应答
	}
	else if(C0==ISP_COMMAND_EE)			//使用双字节寻址模式擦除一个到全部 Flash 页面（仅用于v3.0 usart 自举程序版本及以上版本）。
	{
		Usart_ISP_SetSlaveStatus(ISP_Conf,ISP_STATUS_WaitEraseAddr);	//ISP等待待擦除地址，接收到擦除命令后，应答，等待输入待擦除地址（全部擦除或者页擦除）
		Usart_ISP_ACK(ISP_Conf);	//ISP应答
	}
	else if(C0==ISP_COMMAND_WP)			//使能某些扇区的写保护
	{
		/*Write Protect 命令用于为一部分或所有 Flash 扇区使能写保护。自举程序接收到 Write Protect 命令后，
			会将 ACK 字节发送到主机。发送 ACK 字节后，自举程序将等待要接收的 字节数（受保护的扇区），之后从应用程序接收 Flash 扇区代码。
			在 Write Protect 命令结束时，自举程序会发送 ACK 字节并启动系统复位，以实施新的选项 字节配置。
		*/
		return;			//暂不处理
	}
	else if(C0==ISP_COMMAND_WU)			//关闭所有 Flash 扇区的写保护
	{
		/*Write Unprotect 命令用于禁止所有 Flash 扇区的写保护。自举程序接收到 Write Unprotect 命令后，
		会将 ACK 字节发送到主机。发送 ACK 字节后，自举程序会禁止所有 Flash 扇区的 写保护。执行禁止保护操作后，自举程序发送 ACK 字节。
		在 Write Unprotect 命令结束时，自举程序会发送 ACK 字节并启动系统复位，以实施新的选项 字节配置。
		*/
		return;			//暂不处理
	}
	else if(C0==ISP_COMMAND_RP)			//使能读保护
	{
		/*Readout Protect 命令用于使能 Flash 读保护。自举程序接收到 Readout Protect 命令后，
			会将 ACK 字节发送到主机。发送 ACK 字节后，自举程序将使能 Flash 的读保护。
			在 Readout Protect 命令结束时，自举程序会发送 ACK 字节并启动系统复位，以实施新的选 项字节配置。
		*/
		return;			//暂不处理
	}
	else if(C0==ISP_COMMAND_RU)			//关闭读保护
	{
		/*Readout Unprotect 命令用于禁止 Flash 读保护。自举程序接收到 Readout Unprotect 命令 后，会将 ACK 字节发送到主机。
			发送了 ACK 字节后，自举程序将擦除所有 Flash 扇区并禁 止整个 Flash 的读保护。如果擦除操作成功完成，自举程序将停用 RDP。
			如果擦除操作失败，自举程序会发送一个 NACK 且读保护仍然有效。
			在 Readout Unprotect 命令结束时，自举程序会发送 ACK 字节并启动系统复位，以实施新 的选项字节配置。
		*/
		return;			//暂不处理
	}
	else
	{
	}	
//	if(rxNum)
//	{
//		memcpy(ISP_Conf->ISP_DATA.ISP_TxBuffer, ISP_Conf->ISP_DATA.ISP_TvBuffer, rxNum);
//		USART_DMASend(ISP_Conf->USARTx,(u32*)ISP_Conf->ISP_DATA.ISP_TxBuffer,rxNum);	//串口DMA发送程序	
//	}
}
/*******************************************************************************
* 函数名			:	Usart_ISP_ReadAddr
* 功能描述		:	ISP读地址
* 输入			: void
* 返回值			: void
*******************************************************************************/
void Usart_ISP_GetAddr(ISP_Conf_TypeDef *ISP_Conf)		//ISP获取写数据起始地址(主机->从机)
{	
	//ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitReadAddr||ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitWAddr||ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitEraseAddr)
	//需要接收地址的指令：读数据，写数据，擦除
	if(ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitEraseAddr)			//ISP擦除操作，接收到擦除命令后，应答，等待输入待擦除地址（全部擦除或者页擦除）等待擦除完成，完成后应答
	{
		unsigned char C0=ISP_Conf->ISP_DATA.ISP_RvBuffer[0];
		unsigned char C1=ISP_Conf->ISP_DATA.ISP_RvBuffer[1];
		C1=C1^0XFF;
		if(C0!=C1)
		{
			return;
		}
		ISP_Conf->ISP_DATA.WriteAddr=C0;													//待写数据的起始地址--擦除为写入0xFF
		Usart_ISP_SetSlaveStatus(ISP_Conf,ISP_STATUS_WaitErase);						//等待接收待写入的数据
	}
	else
	{
		unsigned int addr=0x00;		//合并地址变量
		unsigned char Bcc=BCC8(ISP_Conf->ISP_DATA.ISP_RvBuffer,4);		//异或校验;
		if(Bcc!=ISP_Conf->ISP_DATA.ISP_RvBuffer[4])
		{
			return;
		}
		else		//合并地址---4字节
		{		
			addr=	(ISP_Conf->ISP_DATA.ISP_RvBuffer[0])<<24;
			addr+=(ISP_Conf->ISP_DATA.ISP_RvBuffer[1])<<16;
			addr+=(ISP_Conf->ISP_DATA.ISP_RvBuffer[2])<<8;
			addr+=(ISP_Conf->ISP_DATA.ISP_RvBuffer[3]);
		}
		
		if(ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitReadAddr)				//等待接收读数据起地址，接收到地址后应答，再等待待读取数据长度
		{
			ISP_Conf->ISP_DATA.ReadAddr=addr;														//待读数据的起始地址
			Usart_ISP_SetSlaveStatus(ISP_Conf,ISP_STATUS_WaitLengh);						//等待接收需要读取的数据长度
			Usart_ISP_ACK(ISP_Conf);	//ISP应答
		}
		else if(ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitWAddr)			//等待接收写数据地址，接收到地址后应答，再等待待写入的数据
		{
			if(ISP_Conf->ISP_DATA.OffsetAddr==0)
			{
				ISP_Conf->ISP_DATA.OffsetAddr=addr;
			}
		
			ISP_Conf->ISP_DATA.WriteAddr=addr;													//待写数据的起始地址
			Usart_ISP_SetSlaveStatus(ISP_Conf,ISP_STATUS_WaitWData);		//设置从机状态--等待接收待写入的数据
//			ISP_Conf->ISP_SLAVE_STATUS=ISP_STATUS_WaitWData;						//等待接收待写入的数据
			Usart_ISP_ACK(ISP_Conf);	//ISP应答
		}
		else if(ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitGoAddr)			//等待接收写数据地址，接收到地址后应答，再等待待写入的数据
		{
			ISP_Conf->ISP_DATA.GoAddr=addr;																//待写数据的起始地址
			Usart_ISP_SetSlaveStatus(ISP_Conf,ISP_STATUS_IDLE);						//ISP空闲状态，可以读写
			Usart_ISP_ACK(ISP_Conf);	//ISP应答
		}
	}
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void Usart_ISP_SetAddr(ISP_Conf_TypeDef *ISP_Conf)					//ISP设置写数据起始地址(主机->从机)
{
	#ifdef	Usart_ISP_Simulation 		//模拟从机
	{
	}
	#else
	{
	}
	#endif
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void Usart_ISP_GetLengh(ISP_Conf_TypeDef *ISP_Conf)				//ISP获取需要读取的数据长度(主机->从机)，读数据时，传入地址后再传入待读取的数据长度
{	
	//ISP获取需要读取的数据长度(主机->从机)，读数据时，传入地址后再传入待读取的数据长度，接收到长度数据后执行读数据操作，然后上传数据
	unsigned char C0=ISP_Conf->ISP_DATA.ISP_RvBuffer[0];
	unsigned char C1=ISP_Conf->ISP_DATA.ISP_RvBuffer[1];
	C1=C1^0XFF;
	if(C0!=C1)		//校验地址
	{
		return;
	}
	else
	{
		ISP_Conf->ISP_DATA.SendLen=C0;										//需要往串口发送的数据个数
		Usart_ISP_SetSlaveStatus(ISP_Conf,ISP_STATUS_WaitReadData);	//设置从机状态--ISP等待读数据操作
	}
//		ISP_Conf->ISP_SLAVE_STATUS=ISP_STATUS_WaitReadData;	//ISP等待读数据操作
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void Usart_ISP_SetLengh(ISP_Conf_TypeDef *ISP_Conf)				//ISP获取需要读取的数据长度(主机->从机)
{
	#ifdef	Usart_ISP_Simulation 		//模拟从机
	{
	}
	#else
	{
	}
	#endif
}

/*******************************************************************************
* 函数名			:	Usart_ISP_CommandSend
* 功能描述		:	串口编程发送命令程序；读数据分五步：1-读数据命令，2-读数据起始地址，3-需要读取的长度，4-读数据过程，5-上报数据
* 输入			: void
* 返回值			: void
*******************************************************************************/
void Usart_ISP_ReadMemory(ISP_Conf_TypeDef *ISP_Conf)		//ISP读数据（从机->主机）
{	
	#ifdef	Usart_ISP_Simulation 		//模拟从机
	{
		//*******************执行读数据操作
//		ISP_Conf->ISP_DATA.ISP_TxBuffer[0]=0x79;
//		memcpy(&ISP_Conf->ISP_DATA.ISP_TxBuffer[1], ISP_Conf->ISP_DATA.ISP_TvBuffer, ISP_Conf->ISP_DATA.SendLen+1);	//复制数据
		Usart_ISP_SetSlaveStatus(ISP_Conf,ISP_STATUS_WaitSData);		//ISP等待发送数据			
	}
	#else
	{
		//*******************执行读数据操作
		//----此为等待外部Flash读取数据完成，当数据读取完成后，由外部程序设置ISP_STATUS_WaitSData状态
		//1）外部检测到ISP_STATUS_WaitReadData状态后
		//2）根据地址ReadAddr和待发送长度SendLen读取相应数据
		//3）读取完成后，将数据复制到ISP_Conf->ISP_DATA.ISP_TvBuffer发送备份区
		//4）设置ISP_STATUS_WaitSData状态，表示数据准备完成，等待发送
	}
	#endif
}
/*******************************************************************************
* 函数名			:	Usart_ISP_CommandSend
* 功能描述		:	串口编程发送命令程序
* 输入			: void
* 返回值			: void
*******************************************************************************/
void Usart_ISP_WriteMemory(ISP_Conf_TypeDef *ISP_Conf)	//ISP写数据（主机->从机）写数据时，传完地址后等待传入数据，再执行写操作
{	
	#ifdef	Usart_ISP_Simulation 		//模拟从机
	{
		if(ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitWData)		//ISP等待接收待写入数据
		{
			unsigned char	Wlen=ISP_Conf->ISP_DATA.ISP_RvBuffer[0];						//要写入Flash的数据长度（字节数)
			unsigned char Bcc=BCC8(ISP_Conf->ISP_DATA.ISP_RvBuffer,Wlen+2);		//异或校验;					
			if(Bcc!=ISP_Conf->ISP_DATA.ISP_RvBuffer[Wlen+2])
			{
				return;
			}
			ISP_Conf->ISP_DATA.WriteLen=ISP_Conf->ISP_DATA.ISP_RvBuffer[0];		//要写入Flash的数据长度（字节数)	
			memcpy(ISP_Conf->ISP_DATA.ISP_TvBuffer, &ISP_Conf->ISP_DATA.ISP_RvBuffer[1], ISP_Conf->ISP_DATA.WriteLen+1);	//复制数据
			Usart_ISP_SetSlaveStatus(ISP_Conf,ISP_STATUS_WaitWrited);				//ISP等待写入完成
		}
		else if(ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitWrited)	//ISP等待写入完成
		{
			Usart_ISP_ACK(ISP_Conf);	//ISP应答
			Usart_ISP_SetSlaveStatus(ISP_Conf,ISP_STATUS_WaitCommand);				//ISP等待命令（作为从机)
		}
	}
	#else
	{
		if(ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitWData)		//ISP等待接收待写入数据
		{
			unsigned char	Wlen=ISP_Conf->ISP_DATA.ISP_RvBuffer[0];						//要写入Flash的数据长度（字节数)
			unsigned char Bcc=BCC8(ISP_Conf->ISP_DATA.ISP_RvBuffer,Wlen+2);		//异或校验;					
			if(Bcc!=ISP_Conf->ISP_DATA.ISP_RvBuffer[Wlen+2])
			{
				return;
			}
			ISP_Conf->ISP_DATA.WriteLen=ISP_Conf->ISP_DATA.ISP_RvBuffer[0];		//要写入Flash的数据长度（字节数)	
			//当外部检测到ISP_STATUS_WaitWrite状态时，根据ISP_Conf->ISP_DATA.WriteLen长度往Flash写入数据，写入完成到设置ISP_STATUS_WaitWrited状态
			memcpy(ISP_Conf->ISP_DATA.ISP_RvBuffer, &ISP_Conf->ISP_DATA.ISP_RvBuffer[1], ISP_Conf->ISP_DATA.WriteLen+1);	//复制数据
			Usart_ISP_SetSlaveStatus(ISP_Conf,ISP_STATUS_WaitWrite);				//ISP等待写数据
		}
		else if(ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitWrited)	//ISP等待写入完成
		{
			//当外部检测到ISP_STATUS_WaitWrite状态时，根据ISP_Conf->ISP_DATA.WriteLen长度往Flash写入数据，写入完成到设置ISP_STATUS_WaitWrited状态
			Usart_ISP_ACK(ISP_Conf);	//ISP应答
			Usart_ISP_SetSlaveStatus(ISP_Conf,ISP_STATUS_WaitCommand);				//ISP等待命令（作为从机)
		}
	}
	#endif
//	unsigned short Wlen=ISP_Conf->ISP_RvBuffer[0];	//获取要写入的数据长度（字节数)
//	unsigned char Bcc=BCC8(ISP_Conf->ISP_RvBuffer,Wlen+2);		//异或校验;
//	if(Bcc!=ISP_Conf->ISP_RvBuffer[Wlen+2])
//	{
//		return;
//	}
//	ISP_Conf->ISP_STATUS=ISP_STATUS_WRITE;		//FLASH执行写操作
//	memcpy(ISP_Conf->ISP_TvBuffer, &ISP_Conf->ISP_RvBuffer[1], Wlen+1);
//	Usart_ISP_ACK(ISP_Conf);						//ISP应答
}
/*******************************************************************************
* 函数名			:	Usart_ISP_CommandSend
* 功能描述		:	串口编程发送命令程序
* 输入			: void
* 返回值			: void
*******************************************************************************/
void Usart_ISP_SendBuffer(ISP_Conf_TypeDef *ISP_Conf)	//ISP上传数据（从机->主机)
{	

	ISP_Conf->ISP_DATA.ISP_TxBuffer[0]=0x79;
	memcpy(&ISP_Conf->ISP_DATA.ISP_TxBuffer[1], ISP_Conf->ISP_DATA.ISP_TvBuffer, ISP_Conf->ISP_DATA.SendLen+1);	//复制数据
	USART_DMASend(ISP_Conf->USARTx,(u32*)ISP_Conf->ISP_DATA.ISP_TxBuffer,ISP_Conf->ISP_DATA.SendLen+2);				//串口DMA发送程序
	Usart_ISP_SetSlaveStatus(ISP_Conf,ISP_STATUS_WaitCommand);				//已经完成数据发送命令，状态更新为等待下一个命令ISP_STATUS_WaitCommand

	
//	unsigned short Wlen=ISP_Conf->ISP_RvBuffer[0];	//获取要写入的数据长度（字节数)
//	unsigned char Bcc=BCC8(ISP_Conf->ISP_RvBuffer,Wlen+2);		//异或校验;
//	if(Bcc!=ISP_Conf->ISP_RvBuffer[Wlen+2])
//	{
//		return;
//	}
//	ISP_Conf->ISP_STATUS=ISP_STATUS_WRITE;		//FLASH执行写操作
//	memcpy(ISP_Conf->ISP_TvBuffer, &ISP_Conf->ISP_RvBuffer[1], Wlen+1);
//	Usart_ISP_ACK(ISP_Conf);						//ISP应答
}
/*******************************************************************************
* 函数名			:	Usart_ISP_CheckFun
* 功能描述		:	检测ISP模块工作模式---空闲时检测
* 输入			: void
* 返回值			: void
*******************************************************************************/
void Usart_ISP_Erase(ISP_Conf_TypeDef *ISP_Conf)				//ISP擦除操作，接收到擦除命令后，应答，等待输入待擦除地址（全部擦除或者页擦除）等待擦除完成，完成后应答
{
	//ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitErase||ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitErased
	#ifdef	Usart_ISP_Simulation 		//模拟从机
	{
		if(ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitErase)	//开始擦除
		{
			if(ISP_Conf->ISP_DATA.WriteAddr==0xFF)	//全部擦除
			{
				Usart_ISP_ACK(ISP_Conf);													//ISP应答---擦除完成
				Usart_ISP_Reset(ISP_Conf);												//重置编程器---恢复所有参数为默认值---模式切换后复位
				Usart_ISP_SetSlaveStatus(ISP_Conf,ISP_STATUS_WaitErased);	//状态为擦除完成
			}
		}
		else if(ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitErased)	//等待擦除完成
		{
			Usart_ISP_SetSlaveStatus(ISP_Conf,ISP_STATUS_WaitCommand);	//状态为擦除完成，ISP等待命令（作为从机)
		}
	}
	#else
	{
		if(ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitErase)	//开始擦除
		{
			if(ISP_Conf->ISP_DATA.WriteAddr==0xFF)	//全部擦除
			{
				Usart_ISP_SetSlaveStatus(ISP_Conf,ISP_STATUS_Eraseing);			//设置从机状态--ISP正在擦除
			}
		}
		else if(ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitErased)			//等待擦除完成
		{
			Usart_ISP_ACK(ISP_Conf);							//ISP应答---擦除完成
			Usart_ISP_SetSlaveStatus(ISP_Conf,ISP_STATUS_WaitCommand);	//状态为擦除完成，ISP等待命令（作为从机)
		}
		
	}	
	#endif
}
/*******************************************************************************
* 函数名			:	Usart_ISP_Reset
* 功能描述		:	重置编程器---恢复所有参数为默认值
* 输入			: void
* 返回值			: void
*******************************************************************************/
u8 Usart_ISP_GetSlaveStatus(ISP_Conf_TypeDef *ISP_Conf)	//返回从机状态值
{	
	return(ISP_Conf->ISP_SLAVE_STATUS);		//返回从机状态值
}
/*******************************************************************************
* 函数名			:	Usart_ISP_Reset
* 功能描述		:	重置编程器---恢复所有参数为默认值
* 输入			: void
* 返回值			: void
*******************************************************************************/
void Usart_ISP_SetSlaveStatus(ISP_Conf_TypeDef *ISP_Conf,ISP_SLAVE_STATUS_TypeDef Status)	//设置从机状态
{	
	ISP_Conf->ISP_SLAVE_STATUS=Status;	//设置状态值
}
/*******************************************************************************
* 函数名			:	Usart_ISP_Reset
* 功能描述		:	重置编程器---恢复所有参数为默认值
* 输入			: void
* 返回值			: void
*******************************************************************************/
void Usart_ISP_Reset(ISP_Conf_TypeDef *ISP_Conf)	//重置编程器---恢复所有参数为默认值
{	
	ISP_Conf->ISP_SLAVE_STATUS			=	ISP_STATUS_IDLE;	//FLASH空闲状态，可以读写
	ISP_Conf->OverRunTime						=	0;								//超时时间
	ISP_Conf->ISP_DATA.ReceivedLen	=	0;		//串口接收的数据个数
	ISP_Conf->ISP_DATA.SendLen			=	0;		//需要往串口发送的数据个数
	ISP_Conf->ISP_DATA.OffsetAddr		=	0;		//写从机时的地址偏移
	ISP_Conf->ISP_DATA.StartAddr		=	0;		//起始地址
	ISP_Conf->ISP_DATA.ReadAddr			=	0;		//读数据起始地址
	ISP_Conf->ISP_DATA.WriteAddr		=	0;		//写入数据起始地址
	ISP_Conf->ISP_DATA.WriteLen			=	0;		//需要写入的长度
	memset(ISP_Conf->ISP_DATA.ISP_RxBuffer,0xFF, ISP_BufferSize);	//接收缓冲区
	memset(ISP_Conf->ISP_DATA.ISP_RvBuffer,0xFF, ISP_BufferSize);	//接收缓冲区--备份区
	memset(ISP_Conf->ISP_DATA.ISP_TxBuffer,0xFF, ISP_BufferSize);	//发送缓冲区
	memset(ISP_Conf->ISP_DATA.ISP_TvBuffer,0xFF, ISP_BufferSize);	//发送缓冲区--备份区
}




