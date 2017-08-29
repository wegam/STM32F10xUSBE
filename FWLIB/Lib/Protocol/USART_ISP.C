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
#include "STM32_GPIO.H"

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
	GPIO_Configuration_OPP50	(ISP_Conf->RESET_CTL_PORT,ISP_Conf->RESET_CTL_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	GPIO_Configuration_OPP50	(ISP_Conf->BOOT0_CTL_PORT,ISP_Conf->BOOT0_CTL_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	Usart_ISP_Reset(ISP_Conf);																																								//重置编程器---恢复所有参数为默认值
//	ISP_Conf->ISP_FUN=ISP_SLAVE;			//测试---将模块设置为从机
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
		Usart_ISP_MasterProcess(ISP_Conf);			//模块作为主机时的处理程序
	}
	else if(ISP_Conf->ISP_FUN==ISP_WaitDeliver)			//等待释放从机)
	{
		Usart_ISP_CheckFun(ISP_Conf);					//检测ISP模块工作模式---空闲时检测
	}
	else
	{
		Usart_ISP_CheckFun(ISP_Conf);					//检测ISP模块工作模式---空闲时检测
	}	
//	Usart_ISP_MasterProcess(ISP_Conf);			//模块作为主机时的处理程序
//	Usart_ISP_SlaveProcess(ISP_Conf);			//模块作为从机时的处理程序
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
				if(ISP_Conf->Connected==ISP_STATUS_UnConnect)
				{
					Usart_ISP_Reset(ISP_Conf);														//重置编程器---恢复所有参数为默认值
					ISP_Conf->Connected=ISP_STATUS_WaitConnect;						//ISP等待连接（作为从机)
				}
				else if(ISP_Conf->Connected==ISP_STATUS_Connectting||ISP_Conf->Connected==ISP_STATUS_WaitConnect)
				{
				}
				else if(ISP_Conf->Connected==ISP_STATUS_Connectted)
				{					
					Usart_ISP_SetSlaveStatus(ISP_Conf,ISP_STATUS_WaitCommand);		//ISP等待命令（作为从机)
					Usart_ISP_ACK(ISP_Conf);
				}																							//ISP应答
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
	else if(ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitReset)	//程序下载完后从机需要重置
	{
		Usart_ISP_CheckFun(ISP_Conf);					//检测ISP模块工作模式---空闲时检测
	}
	else	if((ISP_Conf->ISP_SLAVE_STATUS!=ISP_STATUS_IDLE)||(ISP_Conf->ISP_FUN==ISP_SLAVE))				//非初始状态时超时复位
	{
		ISP_Conf->OverRunTime=ISP_Conf->OverRunTime+1;					//超时时间
		if(ISP_Conf->OverRunTime>=5000000)				//约5秒
		Usart_ISP_Reset(ISP_Conf);								//重置编程器---恢复所有参数为默认值
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
	Usart_MISP_StatusProcess(ISP_Conf);			//启动从机设备使从机运行
	Usart_ISP_Send(ISP_Conf);						//发送函数包括从机发送应答、发送地址、发送命令、发送数据
}
/*******************************************************************************
* 函数名			:	Usart_ISP_CheckFun
* 功能描述		:	检测ISP模块工作模式---空闲时检测
* 输入			: void
* 返回值			: void
*******************************************************************************/
void Usart_ISP_CheckFun(ISP_Conf_TypeDef *ISP_Conf)				//检测ISP模块工作模式---空闲时检测
{
	u32 delaytime=0x00;
//	GPIO_Configuration_OPP50	(ISP_Conf->RESET_CTL_PORT,ISP_Conf->RESET_CTL_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
//	GPIO_Configuration_OPP50	(ISP_Conf->BOOT0_CTL_PORT,ISP_Conf->BOOT0_CTL_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
//	GPIO_Configuration_INA		(ISP_Conf->RESET_CTL_PORT,ISP_Conf->RESET_CTL_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
//	GPIO_Configuration_INA		(ISP_Conf->BOOT0_CTL_PORT,ISP_Conf->BOOT0_CTL_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
//	GPIO_ReadInputDataBit(ISP_Conf->RESET_CTL_PORT,ISP_Conf->RESET_CTL_Pin);
//	GPIO_ReadInputDataBit(ISP_Conf->BOOT0_CTL_PORT,ISP_Conf->BOOT0_CTL_Pin);
//	GPIO_Configuration_IPD(ISP_Conf->RESET_CTL_PORT,ISP_Conf->RESET_CTL_Pin);			//将GPIO相应管脚配置为下拉输入模式----V20170605
//	GPIO_Configuration_IPD(ISP_Conf->BOOT0_CTL_PORT,ISP_Conf->BOOT0_CTL_Pin);			//将GPIO相应管脚配置为下拉输入模式----V20170605
//	GPIO_Configuration_IPU(ISP_Conf->RESET_CTL_PORT,ISP_Conf->RESET_CTL_Pin);			//将GPIO相应管脚配置为上拉输入模式----V20170605
//	GPIO_Configuration_IPU(ISP_Conf->BOOT0_CTL_PORT,ISP_Conf->BOOT0_CTL_Pin);			//将GPIO相应管脚配置为上拉输入模式----V20170605
	//判断主机还是从机
	//---------------------判断是否为从机：编程器将RESET脚拉低，将BOOT0脚拉高
//	GPIO_Configuration_IPU(ISP_Conf->RESET_CTL_PORT,ISP_Conf->RESET_CTL_Pin);			//将GPIO相应管脚配置为上拉输入模式----V20170605
	
	
	if(ISP_Conf->ISP_SLAVE_STATUS==ISP_STATUS_WaitReset)	//作为从机时程序下载完后从机需要重置
	{
		GPIO_Configuration_IPD(ISP_Conf->BOOT0_CTL_PORT,ISP_Conf->BOOT0_CTL_Pin);				//将GPIO相应管脚配置为下拉输入模式----V20170605
		while((GPIO_ReadInputDataBit(ISP_Conf->BOOT0_CTL_PORT,ISP_Conf->BOOT0_CTL_Pin)==Bit_SET));
		Usart_ISP_Reset(ISP_Conf);						//重置编程器---恢复所有参数为默认值
	}
	else if(ISP_Conf->ISP_MASTER_STATUS==ISP_MSTATUS_WaitDeliver)	//作为主机时烧录完成后等待从机离开
	{
		GPIO_Configuration_IPD(ISP_Conf->RESET_CTL_PORT,ISP_Conf->RESET_CTL_Pin);			//将GPIO相应管脚配置为下拉输入模式----V20170605
		if(GPIO_ReadInputDataBit(ISP_Conf->RESET_CTL_PORT,ISP_Conf->RESET_CTL_Pin)==Bit_SET)
		{
//			Usart_ISP_Reset(ISP_Conf);						//重置编程器---恢复所有参数为默认值
			ISP_Conf->TimeCount=0;
		}
		else
		{
			if(ISP_Conf->TimeCount++>20000)
				Usart_ISP_Reset(ISP_Conf);						//重置编程器---恢复所有参数为默认值
		}
	}
	else
	{	
		GPIO_Configuration_IPD(ISP_Conf->BOOT0_CTL_PORT,ISP_Conf->BOOT0_CTL_Pin);				//将GPIO相应管脚配置为下拉输入模式----V20170605		
		while(delaytime++<0xFFFF)
		{
			if((GPIO_ReadInputDataBit(ISP_Conf->BOOT0_CTL_PORT,ISP_Conf->BOOT0_CTL_Pin)==Bit_SET))
			{
				ISP_Conf->ISP_FUN=ISP_SLAVE;		//ISP作为从机
				return;
			}
		}
		//---------------------判断是否为主机(不为从机时则认为是从机）：从机正常工作时，RESET脚带上拉，BOOT0脚为低
		{
			Usart_ISP_Reset(ISP_Conf);			//重置编程器---恢复所有参数为默认值
			ISP_Conf->ISP_FUN=ISP_MASTER;		//ISP作为主机--更新自身程序
			ISP_Conf->ISP_MASTER_STATUS	=ISP_MSTATUS_IDLE;
			GPIO_Configuration_OPP50	(ISP_Conf->RESET_CTL_PORT,ISP_Conf->RESET_CTL_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
			GPIO_Configuration_OPP50	(ISP_Conf->BOOT0_CTL_PORT,ISP_Conf->BOOT0_CTL_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605		
		}
	}		
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
		ISP_Conf->ISP_DATA.USARTSendLen=15;												//串口需要发送的数据长度
		
		memcpy(ISP_Conf->ISP_DATA.ISP_TxBuffer, ISP_Conf->ISP_DATA.ISP_TvBuffer, ISP_Conf->ISP_DATA.USARTSendLen);
		USART_DMASend(ISP_Conf->USARTx,(u32*)ISP_Conf->ISP_DATA.ISP_TxBuffer,ISP_Conf->ISP_DATA.USARTSendLen);	//串口DMA发送程序	
			
	}
	else if(C0==ISP_COMMAND_GetVS)	//获取自举程序版本及 Flash 的读保护状态
	{
		ISP_Conf->ISP_DATA.ISP_TvBuffer[0]=ISP_ANSWER_ACK;		//应答
		ISP_Conf->ISP_DATA.ISP_TvBuffer[1]=0x22;							//自举程序版本（0 < 版本 ≤ 255），示例：0x10 = 版本 1.0
		ISP_Conf->ISP_DATA.ISP_TvBuffer[2]=0x00;							//选项字节 1：0x00，保持与通用自举程序协议的兼容性
		ISP_Conf->ISP_DATA.ISP_TvBuffer[3]=0x00;							//选项字节 2：0x00，保持与通用自举程序协议的兼容性
		ISP_Conf->ISP_DATA.ISP_TvBuffer[4]=ISP_ANSWER_ACK;		//应答
		
		Usart_ISP_SetSlaveStatus(ISP_Conf,ISP_STATUS_WaitCommand);		//ISP等待命令（作为从机)--处理完GetVS命令后等待下一下命令
		ISP_Conf->ISP_DATA.USARTSendLen=5;												//串口需要发送的数据长度
		
		memcpy(ISP_Conf->ISP_DATA.ISP_TxBuffer, ISP_Conf->ISP_DATA.ISP_TvBuffer, ISP_Conf->ISP_DATA.USARTSendLen);
		USART_DMASend(ISP_Conf->USARTx,(u32*)ISP_Conf->ISP_DATA.ISP_TxBuffer,ISP_Conf->ISP_DATA.USARTSendLen);	//串口DMA发送程序	
	}
	else if(C0==ISP_COMMAND_GetID)	//获取芯片 ID
	{
		ISP_Conf->ISP_DATA.ISP_TvBuffer[0]=ISP_ANSWER_ACK;		//应答
		ISP_Conf->ISP_DATA.ISP_TvBuffer[1]=0x01;							//N = 字节数 – 1（对 STM32 N = 1），除当前字节和 ACK 之外。
		ISP_Conf->ISP_DATA.ISP_TvBuffer[2]=0x04;							//PID(1) 字节 3 = 0x04，字节 4 = 0xXX
		ISP_Conf->ISP_DATA.ISP_TvBuffer[3]=0x10;							//PID(1) 字节 3 = 0x04，字节 4 = 0xXX
		ISP_Conf->ISP_DATA.ISP_TvBuffer[4]=ISP_ANSWER_ACK;		//应答
		
		Usart_ISP_SetSlaveStatus(ISP_Conf,ISP_STATUS_WaitCommand);		//ISP等待命令（作为从机)--处理完GetID命令后等待下一下命令
		ISP_Conf->ISP_DATA.USARTSendLen=5;												//串口需要发送的数据长度
		
		memcpy(ISP_Conf->ISP_DATA.ISP_TxBuffer, ISP_Conf->ISP_DATA.ISP_TvBuffer, ISP_Conf->ISP_DATA.USARTSendLen);
		USART_DMASend(ISP_Conf->USARTx,(u32*)ISP_Conf->ISP_DATA.ISP_TxBuffer,ISP_Conf->ISP_DATA.USARTSendLen);	//串口DMA发送程序	
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
			u32 timedelay=0;
			ISP_Conf->ISP_DATA.GoAddr=addr;																//待写数据的起始地址
			ISP_Conf->Connected=ISP_STATUS_UnConnect;											//ISP未连接（作为从机)---此为断开连接状态
//			Usart_ISP_SetSlaveStatus(ISP_Conf,ISP_STATUS_IDLE);						//ISP空闲状态，可以读写
			Usart_ISP_ACK(ISP_Conf);	//ISP应答
			while(timedelay++<0xFF);
//			Usart_ISP_Reset(ISP_Conf);						//重置编程器---恢复所有参数为默认值
		}
	}
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
		ISP_Conf->ISP_DATA.USARTSendLen=C0;										//需要往串口发送的数据个数
		Usart_ISP_SetSlaveStatus(ISP_Conf,ISP_STATUS_WaitReadData);	//设置从机状态--ISP等待读数据操作
	}
//		ISP_Conf->ISP_SLAVE_STATUS=ISP_STATUS_WaitReadData;	//ISP等待读数据操作
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
	memcpy(&ISP_Conf->ISP_DATA.ISP_TxBuffer[1], ISP_Conf->ISP_DATA.ISP_TvBuffer, ISP_Conf->ISP_DATA.USARTSendLen+1);	//复制数据
	USART_DMASend(ISP_Conf->USARTx,(u32*)ISP_Conf->ISP_DATA.ISP_TxBuffer,ISP_Conf->ISP_DATA.USARTSendLen+2);				//串口DMA发送程序
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














//------------------------------------主机专有函数
/*******************************************************************************
*函数名			:	Usart_MISP_StatusProcess
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void Usart_MISP_StatusProcess(ISP_Conf_TypeDef *ISP_Conf)			//启动从机设备使从机运行
{
	if(ISP_Conf->OverRunTime++>2000000)			//10S超时---应答0.5S超时
	{
		Usart_ISP_Reset(ISP_Conf);						//重置编程器---恢复所有参数为默认值
	}
	if(ISP_Conf->ISP_MASTER_STATUS==ISP_MSTATUS_IDLE)								//工作在主机模式下的空闲---准备开始检查有无从机连接
	{
		ISP_Conf->ISP_MASTER_STATUS=ISP_MSTATUS_ResetDevice;
		Usart_MISP_RunDevice(ISP_Conf);						//启动从机设备使从机正常运行
		ISP_Conf->OverRunTime=0;									//超时时间
		Usart_MISP_GetFirmwareInf(ISP_Conf);			//从Flash中获取固件信息
	}
	else if(ISP_Conf->ISP_MASTER_STATUS==ISP_MSTATUS_ResetDevice)		//执行上电复位
	{
		if(ISP_Conf->TimeCount++<10000)					//计时时间--100mS
		{			
			Usart_MISP_ResetDevice(ISP_Conf);			//复位从机设备--使从机进行ISP模式			
		}
		else
		{
			ISP_Conf->TimeCount=0;																			//计时时间--清零
			ISP_Conf->ISP_MASTER_STATUS=ISP_MSTATUS_WriteConnect;				//ISP主机准备连接----复位后检测从机
			Usart_MISP_SetDevice(ISP_Conf);															//启动从机接收BOOT0使从机进行下载状态
		}
	}
	else if(ISP_Conf->ISP_MASTER_STATUS==ISP_MSTATUS_WriteConnect)	//ISP主机准备连接----复位后检测从机
	{
		ISP_Conf->MasterLastStatus=ISP_MSTATUS_WriteConnect;					//存储主机上一个状态，根据上一个状态等到从机应答时进行下一步操作:等待应答
		ISP_Conf->ISP_MASTER_STATUS=ISP_MSTATUS_WaitAck;							//ISP主机等待从机应答
		Usart_MISP_Connect(ISP_Conf);																	//主机连接从机函数
	}
	else if(ISP_Conf->ISP_MASTER_STATUS==ISP_MSTATUS_Connectted)		//ISP主机与从机已经连接完成---准备写入数据/检查有没有读写保护，有则需要解除保护，没有就开始写数据
	{
		ISP_Conf->MasterLastStatus=ISP_MSTATUS_WriteGet;							//存储主机上一个状态，根据上一个状态等到从机应答时进行下一步操作:禁止所有Flash扇区的写保护
		ISP_Conf->ISP_MASTER_STATUS=ISP_MSTATUS_WaitAck;							//ISP主机等待从机应答
		Usart_MISP_GetDevice(ISP_Conf);																//Write Unprotect命令用于禁止所有Flash扇区的写保护。
	}
	else if(ISP_Conf->ISP_MASTER_STATUS==ISP_MSTATUS_WriteUnprotect)		//ISP主机与从机已经连接完成---准备写入数据/检查有没有读写保护，有则需要解除保护，没有就开始写数据
	{
		ISP_Conf->MasterLastStatus=ISP_MSTATUS_WriteUnprotect;				//存储主机上一个状态，根据上一个状态等到从机应答时进行下一步操作:禁止所有Flash扇区的写保护
		ISP_Conf->ISP_MASTER_STATUS=ISP_MSTATUS_WaitAck;							//ISP主机等待从机应答
		Usart_MISP_WriteUnprotect(ISP_Conf);													//Write Unprotect命令用于禁止所有Flash扇区的写保护。
	}
	else if(ISP_Conf->ISP_MASTER_STATUS==ISP_MSTATUS_WaitUnprotect)		//ISP主机与从机已经连接完成---准备写入数据/检查有没有读写保护，有则需要解除保护，没有就开始写数据
	{
		ISP_Conf->MasterLastStatus=ISP_MSTATUS_WaitUnprotect;				//存储主机上一个状态，根据上一个状态等到从机应答时进行下一步操作:禁止所有Flash扇区的写保护
		ISP_Conf->ISP_MASTER_STATUS=ISP_MSTATUS_WaitAck;							//ISP主机等待从机应答
	}
	else if(ISP_Conf->ISP_MASTER_STATUS==ISP_MSTATUS_FlashUnprotected)		//已禁止所有Flash扇区的写保护，可以进行写操作
	{
		ISP_Conf->MasterLastStatus	=ISP_MSTATUS_WriteEraseCMD;							//存储主机上一个状态，根据上一个状态等到从机应答时进行下一步操作:擦除
		ISP_Conf->ISP_MASTER_STATUS	=ISP_MSTATUS_WaitAck;										//ISP主机等待从机应答
		Usart_MISP_CommandSend(ISP_Conf,ISP_COMMAND_Erase);									//串口编程发送命令程序---擦除
	}
	else if(ISP_Conf->ISP_MASTER_STATUS==ISP_MSTATUS_WriteEraseConf)						//发送擦除参数
	{
		ISP_Conf->MasterLastStatus	=ISP_MSTATUS_WriteEraseConf;								//存储主机上一个状态，根据上一个状态等到从机应答时进行下一步操作:从应用程序指定的地址开始将最多 256 个字节的数据写入 RAM 或 Flash---需要应答
		ISP_Conf->ISP_MASTER_STATUS	=ISP_MSTATUS_WaitAck;												//ISP主机等待从机应答
		Usart_MISP_CommandSend(ISP_Conf,0xFF);																	//全部擦除
	}	
	else if(ISP_Conf->ISP_MASTER_STATUS==ISP_MSTATUS_EraseDone)						//已经完成擦除
	{
		ISP_Conf->MasterLastStatus	=ISP_MSTATUS_GetFirmwareInf;								//存储主机上一个状态，根据上一个状态等到从机应答时进行下一步操作:从应用程序指定的地址开始将最多 256 个字节的数据写入 RAM 或 Flash---需要应答
		ISP_Conf->ISP_MASTER_STATUS	=ISP_MSTATUS_GetFirmwareInf;							//ISP主机等待从机应答
	}	
	else if(ISP_Conf->ISP_MASTER_STATUS==ISP_MSTATUS_GetedFirmwareInf)
	{
		ISP_Conf->MasterLastStatus	=ISP_MSTATUS_WriteWM;	//ISP_MSTATUS_WriteData
		ISP_Conf->ISP_MASTER_STATUS	=ISP_MSTATUS_WriteWM;
		ISP_Conf->ISP_MDATA.WriteAddr=ISP_Conf->ISP_MDATA.GoAddr;							//起始地址
//		Usart_MISP_CommandSend(ISP_Conf,ISP_COMMAND_WM);										//写MM命令
	}
	else if(ISP_Conf->ISP_MASTER_STATUS==ISP_MSTATUS_ReadFirmware)
	{
		ISP_Conf->MasterLastStatus	=ISP_MSTATUS_ReadFirmware;
		ISP_Conf->ISP_MASTER_STATUS	=ISP_MSTATUS_ReadFirmware;
	}
	else if(ISP_Conf->ISP_MASTER_STATUS==ISP_MSTATUS_WaitFirmware)
	{
		if(ISP_Conf->OverRunTime++>400000)				//复位后2秒内没连接上从机重新复位模块
		{
			Usart_ISP_Reset(ISP_Conf);							//重置编程器---恢复所有参数为默认值
		}
	}
	else if(ISP_Conf->ISP_MASTER_STATUS==ISP_MSTATUS_GettedFirmware)	//获取到固件
	{
		ISP_Conf->ISP_MASTER_STATUS=ISP_MSTATUS_WriteData;
//		ISP_Conf->ISP_MASTER_STATUS	=ISP_MSTATUS_WaitAck;
		ISP_Conf->ISP_MDATA.WriteAddr+=0xFF;
	}
	else if(ISP_Conf->ISP_MASTER_STATUS==ISP_MSTATUS_WriteWM)
	{
		ISP_Conf->MasterLastStatus	=ISP_MSTATUS_WriteWM;
		ISP_Conf->ISP_MASTER_STATUS	=ISP_MSTATUS_WaitAck;
		Usart_MISP_CommandSend(ISP_Conf,ISP_COMMAND_WM);										//写MM命令
	}
	else if(ISP_Conf->ISP_MASTER_STATUS==ISP_MSTATUS_WriteAddr)						//写地址
	{
		ISP_Conf->MasterLastStatus	=ISP_MSTATUS_WriteAddr;
		ISP_Conf->ISP_MASTER_STATUS	=ISP_MSTATUS_WaitAck;
		ISP_Conf->ISP_DATA.ISP_TvBuffer[0]=ISP_Conf->ISP_MDATA.WriteAddr>>24;
		ISP_Conf->ISP_DATA.ISP_TvBuffer[1]=ISP_Conf->ISP_MDATA.WriteAddr>>16;
		ISP_Conf->ISP_DATA.ISP_TvBuffer[2]=ISP_Conf->ISP_MDATA.WriteAddr>>8;
		ISP_Conf->ISP_DATA.ISP_TvBuffer[3]=ISP_Conf->ISP_MDATA.WriteAddr>>0;
		ISP_Conf->ISP_DATA.ISP_TvBuffer[4]=BCC8(ISP_Conf->ISP_DATA.ISP_TvBuffer,4);		//异或校验
//		ISP_Conf->ISP_MDATA.WriteAddr+=ISP_BufferSize;
		ISP_Conf->ISP_DATA.USARTSendLen=5;
//		Usart_MISP_CommandSend(ISP_Conf,ISP_COMMAND_WM);										//写MM命令
	}
	else if(ISP_Conf->ISP_MASTER_STATUS==ISP_MSTATUS_WriteData)						//写地址
	{
		ISP_Conf->MasterLastStatus	=ISP_MSTATUS_WriteData;
		ISP_Conf->ISP_MASTER_STATUS	=ISP_MSTATUS_WaitAck;
		ISP_Conf->ISP_DATA.USARTSendLen=ISP_Conf->ISP_MDATA.WriteLen;
//		ISP_Conf->ISP_MDATA.SumHaveWritten+=ISP_Conf->ISP_MDATA.WriteLen-2;
//		Usart_MISP_CommandSend(ISP_Conf,ISP_COMMAND_WM);										//写MM命令
	}
	else if(ISP_Conf->ISP_MASTER_STATUS==ISP_MSTATUS_WriteGo)						//写地址
	{
		ISP_Conf->MasterLastStatus	=ISP_MSTATUS_WriteGo;
		ISP_Conf->ISP_MASTER_STATUS	=ISP_MSTATUS_WaitAck;
		Usart_MISP_CommandSend(ISP_Conf,ISP_COMMAND_Go);										//写MM命令
	}
	else if(ISP_Conf->ISP_MASTER_STATUS==ISP_MSTATUS_WriteGoAddr)						//写地址
	{
		ISP_Conf->MasterLastStatus	=ISP_MSTATUS_WriteGoAddr;
		ISP_Conf->ISP_MASTER_STATUS	=ISP_MSTATUS_WaitAck;
		
		ISP_Conf->ISP_DATA.ISP_TvBuffer[0]=ISP_Conf->ISP_MDATA.StartAddr>>24;
		ISP_Conf->ISP_DATA.ISP_TvBuffer[1]=ISP_Conf->ISP_MDATA.StartAddr>>16;
		ISP_Conf->ISP_DATA.ISP_TvBuffer[2]=ISP_Conf->ISP_MDATA.StartAddr>>8;
		ISP_Conf->ISP_DATA.ISP_TvBuffer[3]=ISP_Conf->ISP_MDATA.StartAddr>>0;
		ISP_Conf->ISP_DATA.ISP_TvBuffer[4]=BCC8(ISP_Conf->ISP_DATA.ISP_TvBuffer,4);		//异或校验
		ISP_Conf->ISP_DATA.USARTSendLen=5;
		
//		Usart_MISP_CommandSend(ISP_Conf,ISP_COMMAND_Go);										//写MM命令
	}
	else if(ISP_Conf->ISP_MASTER_STATUS==ISP_MSTATUS_ResetDevice2)						//写地址
	{
		u32 timedelay=0;
		Usart_MISP_ResetDevice(ISP_Conf);				//复位从机
		while(timedelay++<0xFFF);
		Usart_MISP_RunDevice(ISP_Conf);					//启动从机设备使从机正常运行
		while(timedelay++<0xFFF);
		ISP_Conf->ISP_MASTER_STATUS=ISP_MSTATUS_WaitDeliver;		//等待释放从机
		ISP_Conf->ISP_FUN=ISP_WaitDeliver;			//等待释放从机
//		GPIO_Configuration_IPD(ISP_Conf->RESET_CTL_PORT,ISP_Conf->RESET_CTL_Pin);			//将GPIO相应管脚配置为下拉输入模式----V20170605
//		while(1);
		
	}
	
	else if(ISP_Conf->ISP_MASTER_STATUS==ISP_MSTATUS_GetFirmwareInf||ISP_Conf->ISP_MASTER_STATUS==ISP_MSTATUS_WaitFirmwareInf)
	{
		if(ISP_Conf->OverRunTime++>400000)				//复位后2秒内没连接上从机重新复位模块
		{
			Usart_ISP_Reset(ISP_Conf);							//重置编程器---恢复所有参数为默认值
		}
	}
	else if(ISP_Conf->ISP_MASTER_STATUS==ISP_MSTATUS_WaitAck)				//ISP主机等待从机应答以及等待超时处理
	{
		bool Result=FALSE;				//定义布尔变量
		Result=Usart_MISP_ReadAck(ISP_Conf);					//主机读取从机应答:有应答返回TRUE，否则返回FALSE
		if(Result==TRUE)					//接收到应答
		{
			if(ISP_Conf->MasterLastStatus==ISP_MSTATUS_WriteConnect)			//存储主机上一个状态，根据上一个状态等到从机应答时进行下一步操作:等待应答
			{
				ISP_Conf->ISP_MASTER_STATUS=ISP_MSTATUS_Connectted;					//ISP主机与从机已经连接完成
			}
			else if(ISP_Conf->MasterLastStatus==ISP_MSTATUS_WriteGet)	//禁止所有Flash扇区的写保护
			{
				ISP_Conf->ISP_MASTER_STATUS=ISP_MSTATUS_WriteUnprotect;			//已禁止所有Flash扇区的写保护，可以进行写操作
			}
			else if(ISP_Conf->MasterLastStatus==ISP_MSTATUS_WriteUnprotect)	//禁止所有Flash扇区的写保护
			{
				ISP_Conf->ISP_MASTER_STATUS=ISP_MSTATUS_WaitUnprotect;			//已禁止所有Flash扇区的写保护，可以进行写操作
			}
			else if(ISP_Conf->MasterLastStatus==ISP_MSTATUS_WaitUnprotect)	//禁止所有Flash扇区的写保护
			{
				ISP_Conf->ISP_MASTER_STATUS=ISP_MSTATUS_FlashUnprotected;			//已禁止所有Flash扇区的写保护，可以进行写操作
			}
			else if(ISP_Conf->MasterLastStatus==ISP_MSTATUS_WriteEraseCMD)	//擦除应答
			{
				ISP_Conf->ISP_MASTER_STATUS=ISP_MSTATUS_WriteEraseConf;				//输入擦除参数
			}
			else if(ISP_Conf->MasterLastStatus==ISP_MSTATUS_WriteEraseConf)	//擦除参数
			{
				ISP_Conf->ISP_MASTER_STATUS=ISP_MSTATUS_EraseDone;						//擦除完成
			}
			else if(ISP_Conf->MasterLastStatus==ISP_MSTATUS_WriteWM)	//擦除参数
			{
				ISP_Conf->ISP_MASTER_STATUS=ISP_MSTATUS_WriteAddr;						//擦除完成
			}
			else if(ISP_Conf->MasterLastStatus==ISP_MSTATUS_WriteAddr)	//写地址
			{
				ISP_Conf->ISP_MASTER_STATUS=ISP_MSTATUS_ReadFirmware;						//擦除完成
			}
			else if(ISP_Conf->MasterLastStatus==ISP_MSTATUS_WriteData)	//擦除参数
			{
				ISP_Conf->ISP_MDATA.SumHaveWritten+=ISP_Conf->ISP_MDATA.WriteLen-2;
				if(ISP_Conf->ISP_MDATA.SumHaveWritten<ISP_Conf->ISP_MDATA.FirmwareLen)
				{
					ISP_Conf->ISP_MASTER_STATUS=ISP_MSTATUS_WriteWM;						//擦除完成	
					ISP_Conf->ISP_MDATA.WriteAddr+=1;					
				}
				else
				{
					ISP_Conf->ISP_MASTER_STATUS=ISP_MSTATUS_WriteGo;						//擦除完成
				}
			}
			else if(ISP_Conf->MasterLastStatus==ISP_MSTATUS_WriteGo)				//写运行地址
			{
				ISP_Conf->ISP_MASTER_STATUS	=ISP_MSTATUS_WriteGoAddr;
			}
			else if(ISP_Conf->MasterLastStatus==ISP_MSTATUS_WriteGoAddr)		//完成
			{
				ISP_Conf->ISP_MASTER_STATUS	=ISP_MSTATUS_ResetDevice2;
			}
		}
		else											//无应答
		{
			if(ISP_Conf->MasterLastStatus==ISP_MSTATUS_WriteConnect)			//存储主机上一个状态，根据上一个状态等到从机应答时进行下一步操作:等待应答
			{
				if(ISP_Conf->TimeCount++>1000)						//计时时间--100mS,超时无应答，重新发送连接命令
				{
					ISP_Conf->TimeCount=0;									//计时时间--清零
					ISP_Conf->ISP_MASTER_STATUS=ISP_MSTATUS_WriteConnect;		//ISP主机准备连接----复位后检测从机
				}
				if(ISP_Conf->OverRunTime++>100000)				//复位后0.5秒内没连接上从机重新复位模块
				{
					Usart_ISP_Reset(ISP_Conf);							//重置编程器---恢复所有参数为默认值
				}
			}
//			else if(ISP_Conf->MasterLastStatus==ISP_MSTATUS_WriteUnprotect)	//禁止所有Flash扇区的写保护
//			{
//				ISP_Conf->ISP_MASTER_STATUS=ISP_MSTATUS_WriteUnprotect;			//已禁止所有Flash扇区的写保护，可以进行写操作
//			}
		}
	}
	
}
u8 Usart_MISP_GetStatus(ISP_Conf_TypeDef *ISP_Conf)	//返回从机状态值
{	
	return(ISP_Conf->ISP_MASTER_STATUS);		//返回从机状态值
}
/*******************************************************************************
*函数名			:	Usart_MISP_GetFirmwareInf
*功能描述		:	从Flash中获取固件信息
*输入				: 
*返回值			:	无
*******************************************************************************/
void Usart_MISP_GetFirmwareInf(ISP_Conf_TypeDef *ISP_Conf)			//从Flash中获取固件信息
{
	
}
/*******************************************************************************
*函数名			:	Usart_ISP_GetDevice
*功能描述		:	启动从机设备使从机运行
*输入				: 
*返回值			:	无
*******************************************************************************/
void Usart_MISP_GetDevice(ISP_Conf_TypeDef *ISP_Conf)			//获取从机芯片信息
{
	Usart_MISP_CommandSend(ISP_Conf,ISP_COMMAND_Get);				//串口编程发送命令程序//Get:获取当前自举程序版本及允许使用的命令
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void Usart_MISP_WriteUnprotect(ISP_Conf_TypeDef *ISP_Conf)			//Write Unprotect命令用于禁止所有Flash扇区的写保护。
{
	Usart_MISP_CommandSend(ISP_Conf,ISP_COMMAND_WU);	//串口编程发送命令程序
}
/*******************************************************************************
*函数名			:	Usart_ISP_Connect
*功能描述		:	主机连接从机函数
*输入				: 
*返回值			:	无
*******************************************************************************/
void Usart_MISP_Connect(ISP_Conf_TypeDef *ISP_Conf)					//主机连接从机函数
{
		ISP_Conf->ISP_DATA.USARTSendLen=1;										//发送一字节0x7F
		ISP_Conf->ISP_DATA.ISP_TvBuffer[0]=0x7F;							//连接符
}
/*******************************************************************************
*函数名			:	Usart_ISP_Send
*功能描述		:	复位从机设备--使从机进行ISP模式
*输入				: 
*返回值			:	无
*******************************************************************************/
void Usart_MISP_ResetDevice(ISP_Conf_TypeDef *ISP_Conf)			//复位从机设备--使从机进行ISP模式
{
	GPIO_SetBits(ISP_Conf->BOOT0_CTL_PORT,ISP_Conf->BOOT0_CTL_Pin);		//BOOT0拉高
	GPIO_ResetBits(ISP_Conf->RESET_CTL_PORT,ISP_Conf->RESET_CTL_Pin);	//RESET脚拉低
}
/*******************************************************************************
*函数名			:	Usart_ISP_SetDevice
*功能描述		:	启动从机接收BOOT0使从机进行下载状态
*输入				: 
*返回值			:	无
*******************************************************************************/
void Usart_MISP_SetDevice(ISP_Conf_TypeDef *ISP_Conf)			//启动从机接收BOOT0使从机进行下载状态
{
	GPIO_SetBits(ISP_Conf->BOOT0_CTL_PORT,ISP_Conf->BOOT0_CTL_Pin);			//BOOT0拉低
	GPIO_SetBits(ISP_Conf->RESET_CTL_PORT,ISP_Conf->RESET_CTL_Pin);			//RESET脚拉高
}
/*******************************************************************************
*函数名			:	Usart_MISP_RunDevice
*功能描述		:	启动从机设备使从机正常运行
*输入				: 
*返回值			:	无
*******************************************************************************/
void Usart_MISP_RunDevice(ISP_Conf_TypeDef *ISP_Conf)			//启动从机设备使从机正常运行
{
	GPIO_ResetBits(ISP_Conf->BOOT0_CTL_PORT,ISP_Conf->BOOT0_CTL_Pin);		//BOOT0拉低
	GPIO_SetBits(ISP_Conf->RESET_CTL_PORT,ISP_Conf->RESET_CTL_Pin);			//RESET脚拉高
}
/*******************************************************************************
* 函数名			:	Usart_MISP_CommandSend
* 功能描述		:	串口编程发送命令程序
* 输入			: void
* 返回值			: void
*******************************************************************************/
void Usart_MISP_CommandSend(ISP_Conf_TypeDef *ISP_Conf,unsigned char Command)	//串口编程发送命令程序
{
	ISP_Conf->ISP_DATA.Command[0]=Command;
	ISP_Conf->ISP_DATA.Command[1]=Command^0XFF;
	USART_DMASend(ISP_Conf->USARTx,(u32*)ISP_Conf->ISP_DATA.Command,2);	//串口DMA发送程序
}
/*******************************************************************************
*函数名			:	Usart_MISP_ReadAck
*功能描述		:	主机读取从机应答
*输入				: 
*返回值			:	TRUE：有应答；FALSE：无应答
*******************************************************************************/
bool Usart_MISP_ReadAck(ISP_Conf_TypeDef *ISP_Conf)			//主机读取从机应答:有应答返回TRUE，否则返回FALSE
{
	ISP_Conf->ISP_DATA.ReceivedLen=USART_ReadBufferIDLE(ISP_Conf->USARTx,(u32*)ISP_Conf->ISP_DATA.ISP_RvBuffer,(u32*)ISP_Conf->ISP_DATA.ISP_RxBuffer);	//串口空闲模式读串口接收缓冲区，如果有数据，将数据拷贝到RevBuffer,并返回接收到的
	if(ISP_Conf->ISP_DATA.ReceivedLen)
	{
		if(ISP_Conf->ISP_DATA.ISP_RvBuffer[0]==0x79)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	return FALSE;
}

//------------------------------------公共函数

/*******************************************************************************
*函数名			:	function
*功能描述		:	发送函数包括从机发送应答、发送地址、发送命令、发送数据
*输入				: 
*返回值			:	无
*******************************************************************************/
void Usart_ISP_Send(ISP_Conf_TypeDef *ISP_Conf)						//发送函数包括从机发送应答、发送地址、发送命令、发送数据
{
	if(ISP_Conf->ISP_DATA.USARTSendLen)
	{
		memcpy(ISP_Conf->ISP_DATA.ISP_TxBuffer, ISP_Conf->ISP_DATA.ISP_TvBuffer,ISP_Conf->ISP_DATA.USARTSendLen);		//复制数据
		memset(ISP_Conf->ISP_DATA.ISP_TvBuffer,0xFF, ISP_BufferSize);	//接收缓冲区
		USART_DMASend(ISP_Conf->USARTx,(u32*)ISP_Conf->ISP_DATA.ISP_TxBuffer,ISP_Conf->ISP_DATA.USARTSendLen);			//串口DMA发送程序
		ISP_Conf->ISP_DATA.USARTSendLen=0;
	}
}

/*******************************************************************************
* 函数名			:	Usart_ISP_Reset
* 功能描述		:	重置编程器---恢复所有参数为默认值
* 输入			: void
* 返回值			: void
*******************************************************************************/
void Usart_ISP_Reset(ISP_Conf_TypeDef *ISP_Conf)	//重置编程器---恢复所有参数为默认值
{	
	//----------从机
	ISP_Conf->ISP_DATA.OffsetAddr				=	0;			//写从机时的地址偏移
	ISP_Conf->ISP_DATA.StartAddr				=	0;			//起始地址
	ISP_Conf->ISP_DATA.GoAddr						=	0;			//复位运行起始地址
	
	ISP_Conf->ISP_DATA.ReceivedLen			=	0;			//串口接收的数据个数
	ISP_Conf->ISP_DATA.USARTSendLen			=	0;			//需要往串口发送的数据个数---//当检测到SendLen不为0时自动发送数据
	
	ISP_Conf->ISP_DATA.FLASHNumToSave		=	0;			//需要往FLASH里保存的数据个数---当此不为0时SPI-FLASH自动存储数据
	ISP_Conf->ISP_DATA.FLASHNumToRead		=	0;			//需要从FLASH里读取的数据个数---当此不为0时，SPI-FLASH自动通过WriteAddr地址从SPI-FLASH读取数据
	
	ISP_Conf->ISP_DATA.ReadAddr					=	0;			//读数据起始地址
	
	ISP_Conf->ISP_DATA.WriteAddr				=	0;			//写入数据起始地址
	ISP_Conf->ISP_DATA.WriteLen					=	0;			//需要写入的长度	

	ISP_Conf->ISP_DATA.SumHaveReceived	=	0;			//作为从机时总共接收到的数据个数
	ISP_Conf->ISP_DATA.SumHaveWritten		=	0;			//主机总共往从机写入的数据个数

	
	
	ISP_Conf->ISP_FUN										=	ISP_IDLE;						//ISP状态为空闲，根据RESET接口状态和BOOT0接口状态可以切换为MASTER或者SLAVE
	ISP_Conf->ISP_SLAVE_STATUS					=	ISP_STATUS_IDLE;		//FLASH空闲状态，可以读写
	ISP_Conf->SlaveLastStatus						=	ISP_STATUS_IDLE;		//存储从机上一个状态，根据上一个状态进行下一步操作
	
	//---------主机
	ISP_Conf->ISP_MDATA.OffsetAddr			=0;
	ISP_Conf->ISP_MDATA.StartAddr				=0;
	ISP_Conf->ISP_MDATA.GoAddr					=0;
	
	ISP_Conf->ISP_MDATA.ReceivedLen			=0;
	ISP_Conf->ISP_MDATA.USARTSendLen		=0;
	
	ISP_Conf->ISP_MDATA.FLASHNumToSave	=0;
	ISP_Conf->ISP_MDATA.FLASHNumToRead	=0;
	
	ISP_Conf->ISP_MDATA.ReadAddr				=0;
	
	ISP_Conf->ISP_MDATA.WriteAddr				=0;
	ISP_Conf->ISP_MDATA.WriteLen				=0;
	
	ISP_Conf->ISP_MDATA.SumHaveReceived	=0;
	ISP_Conf->ISP_MDATA.SumHaveWritten	=0;
	
	ISP_Conf->ISP_MASTER_STATUS					=	ISP_MSTATUS_IDLE;		//ISP主机空闲状态----开始写前或者写完成后状态
	ISP_Conf->MasterLastStatus					=	ISP_MSTATUS_IDLE;		//存储主机上一个状态，根据上一个状态等到从机应答时进行下一步操作	
	
	
	
	ISP_Conf->OverRunTime								=	0;									//超时时间	
	ISP_Conf->TimeCount									=	0;									//计时时间	
	ISP_Conf->Connected=ISP_STATUS_UnConnect;							//ISP未连接（作为从机)
	
	memset(ISP_Conf->ISP_DATA.ISP_RxBuffer,0xFF, ISP_BufferSize);	//接收缓冲区
	memset(ISP_Conf->ISP_DATA.ISP_RvBuffer,0xFF, ISP_BufferSize);	//接收缓冲区--备份区
	memset(ISP_Conf->ISP_DATA.ISP_TxBuffer,0xFF, ISP_BufferSize);	//发送缓冲区
	memset(ISP_Conf->ISP_DATA.ISP_TvBuffer,0xFF, ISP_BufferSize);	//发送缓冲区--备份区
}

//---------------------公共函数
/*******************************************************************************
*函数名			:	BSP_Conf
*功能描述		:	配置函数
*输入				: 
*返回值			:	无
*******************************************************************************/
void BSP_Configuration(BSP_Conf_TypeDef *BSP_Conf)		//配置函数
{
	USART_DMA_ConfigurationEV	(BSP_Conf->BSP_Port.USARTx,115200,(u32*)BSP_Conf->BSP_DATA.BSP_RxBuffer,ISP_BufferSize);	//USART_DMA配置--查询方式，不开中断
	GPIO_Configuration_OPP50	(BSP_Conf->BSP_Port.RESET_CTL_PORT,BSP_Conf->BSP_Port.RESET_CTL_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	GPIO_Configuration_OPP50	(BSP_Conf->BSP_Port.BOOT0_CTL_PORT,BSP_Conf->BSP_Port.BOOT0_CTL_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	BSP_Reset(BSP_Conf);																																								//重置编程器---恢复所有参数为默认值
//	USM_BSP_SetStatus(BSP_Conf,BSP_MSTATUS_ResetDevice);		//更新状态---BSP主机准备连接----复位后检测从机----测试
//	ISP_Conf->ISP_FUN=ISP_SLAVE;			//测试---将模块设置为从机
}
/*******************************************************************************
*函数名			:	BSP_Process
*功能描述		:	事件处理函数
*输入				: 
*返回值			:	无
*******************************************************************************/
void BSP_Process(BSP_Conf_TypeDef *BSP_Conf)			//事件处理函数
{
	USM_BSP_PROCESS(BSP_Conf);					//主机事件处理函数
	USS_BSP_PROCESS(BSP_Conf);					//从机事件处理函数
	
}
void BSP_SendBuffer(BSP_Conf_TypeDef *BSP_Conf)						//通过串口发送数据，当检测到USARTSendLen不为0时自动发送
{
	if(BSP_Conf->BSP_DATA.USARTSendLen!=0)
	{
		memcpy(BSP_Conf->BSP_DATA.BSP_TxBuffer,BSP_Conf->BSP_DATA.BSP_TvBuffer,BSP_Conf->BSP_DATA.USARTSendLen);					//从TvBuffer拷贝数据到TxBuffer		
		USART_DMASend(BSP_Conf->BSP_Port.USARTx,(u32*)BSP_Conf->BSP_DATA.BSP_TxBuffer,BSP_Conf->BSP_DATA.USARTSendLen);		//串口DMA发送程序
		memset(BSP_Conf->BSP_DATA.BSP_TvBuffer,0xFF, BSP_Conf->BSP_DATA.USARTSendLen);																		//清空发送备份区
		BSP_Conf->BSP_DATA.USARTSendLen=0;			//清空发送区
	}
}
/*******************************************************************************
* 函数名			:	BSP_Reset
* 功能描述		:	重置编程器---恢复所有参数为默认值
* 输入			: void
* 返回值			: void
*******************************************************************************/
void BSP_Reset(BSP_Conf_TypeDef *BSP_Conf)	//重置编程器---恢复所有参数为默认值
{	
	//----------BSPM_Info_TypeDef		BSPM_Info
	BSP_Conf->BSPM_Info.BSP_MASTER_STATUS	=	BSP_MSTATUS_IDLE;		//BSP主机空闲状态----开始写前或者写完成后状态
	BSP_Conf->BSPM_Info.MasterLastStatus	=	BSP_MSTATUS_IDLE;		//BSP主机空闲状态----开始写前或者写完成后状态
	BSP_Conf->BSPM_Info.ACK								=	BSP_NACK;						//默认无应答状态
	BSP_Conf->BSPM_Info.ACK2							=	BSP_NACK;						//默认无应答状态
	//----------BSPM_Info_TypeDef		BSPM_Info
	BSP_Conf->BSPS_Info.BSP_SLAVE_STATUS	=	BSP_STATUS_IDLE;		//ISP空闲状态，可以读写
	BSP_Conf->BSPS_Info.SlaveLastStatus		=	BSP_STATUS_IDLE;		//ISP空闲状态，可以读写
	//----------BSPM_Info_TypeDef		BSPM_Info
	BSP_Conf->BSP_DATA.OverRunTime=0;			//超时时间
	BSP_Conf->BSP_DATA.TimeCount=0;				//计时时间
	BSP_Conf->BSP_DATA.Connected=0;				//连接状态--从机更新固件时使用，如果Connected==0，表示未连接，需要格式化，如果Connected==1，表示已经连接
	BSP_Conf->BSP_DATA.OffsetAddr=0;			//写从机时的地址偏移
	BSP_Conf->BSP_DATA.StartAddr=0;				//起始地址
	BSP_Conf->BSP_DATA.StepLen	=256;			//每次写入的字节长，备地址自增用
	BSP_Conf->BSP_DATA.GoAddr=0;					//复位运行起始地址
	BSP_Conf->BSP_DATA.FirmwareLen=0;			//固件长度---需要ISP下载的总数据
	BSP_Conf->BSP_DATA.FLASHNumToSave=0;	//需要往FLASH里保存的数据个数---当此不为0时SPI-FLASH自动存储数据
	BSP_Conf->BSP_DATA.FLASHNumToRead=0;	//需要从FLASH里读取的数据个数---当此不为0时，SPI-FLASH自动通过WriteAddr地址从SPI-FLASH读取数据
	BSP_Conf->BSP_DATA.ReadAddr=0;				//读数据起始地址
	BSP_Conf->BSP_DATA.ReadLen=0;					//需要读取的长度
	BSP_Conf->BSP_DATA.WriteAddr=0;				//要写入Flash的数据起始地址--ISP接口
	BSP_Conf->BSP_DATA.WriteLen=0;				//要写入Flash的数据长度（字节数)--ISP接口
	BSP_Conf->BSP_DATA.ReceivedLen=0;			//串口接收的数据个数
	BSP_Conf->BSP_DATA.USARTSendLen=0;		//需要往串口发送的数据个数---//当检测到SendLen不为0时自动发送数据
	BSP_Conf->BSP_DATA.SumHaveReceived=0;	//作为从机时总共接收到的数据个数
	BSP_Conf->BSP_DATA.SumHaveWritten=0;	//主机总共往从机写入的数据个数
	BSP_Conf->BSP_DATA.Command[0]=0;			//自举程序命令及异或校验码
	BSP_Conf->BSP_DATA.Command[1]=0;
	

	memset(BSP_Conf->BSP_DATA.BSP_RxBuffer,0xFF, BSP_BufferSize);	//接收缓冲区
	memset(BSP_Conf->BSP_DATA.BSP_RvBuffer,0xFF, BSP_BufferSize);	//接收缓冲区--备份区
	memset(BSP_Conf->BSP_DATA.BSP_TxBuffer,0xFF, BSP_BufferSize);	//发送缓冲区
	memset(BSP_Conf->BSP_DATA.BSP_TvBuffer,0xFF, BSP_BufferSize);	//发送缓冲区--备份区
	
}







//---------------------主机专用函数
/*******************************************************************************
*函数名			:	USM_BSP_PROCESS
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void USM_BSP_PROCESS(BSP_Conf_TypeDef *BSP_Conf)
{
	if(BSP_Conf->BSPM_Info.BSP_MASTER_STATUS==BSP_MSTATUS_IDLE)
	{
		BSP_Conf->BSPM_Info.BSP_MASTER_STATUS=BSP_MSTATUS_ResetDevice;
	}

	USM_BSP_GetAck(BSP_Conf);					//获取从机应答	
	
	USM_BSP_RESET(BSP_Conf);					//复位从机
	USM_BSP_Connect(BSP_Conf);				//连接从机---间隔5ms连接发送0x7F，让从机识别波特率
//	
	USM_BSP_Get(BSP_Conf);						//获取当前自举程序版本及允许使用的命令
	USM_BSP_GetVR(BSP_Conf);					//获取自举程序版本及 Flash 的读保护状态
	USM_BSP_GetId(BSP_Conf);					//获取芯片 ID
	USM_BSP_EraseMemory(BSP_Conf);		//擦除一个到全部 Flash 页面（擦除FLASH后开始写入地址）
	USM_BSP_EEraseMemory(BSP_Conf);		//使用双字节寻址模式擦除一个到全部 Flash 页面（仅用于v3.0 usart 自举程序版本及以上版本）	
	USM_BSP_WriteMemory(BSP_Conf);		//从应用程序指定的地址开始将最多 256 个字节的数据写入 RAM 或 Flash
	USM_BSP_WriteAddr(BSP_Conf);			//写地址----包含地址自动增加及完成判断
//	USM_BSP_WriteData(BSP_Conf);			//写数据----向从机写入FLASH数据
	
	BSP_SendBuffer(BSP_Conf);					//通过串口发送数据，当检测到USARTSendLen不为0时自动发送
}
/*******************************************************************************
*函数名			:	USM_BSP_RESET
*功能描述		:	复位从机：
							A模式--进入BOOT：RST高，BOT低（设置约100ms）->RST低，BOT高->进入BOOT
							B模式--进入运行：RST高，BOT低（设置约50ms）->RST低，BOT低（设置约50ms）->RST高，BOT低->运行程序
*输入				: 
*返回值			:	无
*******************************************************************************/
void USM_BSP_RESET(BSP_Conf_TypeDef *BSP_Conf)		//复位从机
{
	USM_BSP_ResetToBSP(BSP_Conf);			//复位从机设备--使从机进行ISP模式		

	USM_BSP_ResetToRUN(BSP_Conf);			//启动从机接收BOOT0使从机正常运行
}
/*******************************************************************************
*函数名			:	USM_BSP_ResetDevice
*功能描述		:	复位从机设备--使从机进行ISP模式
*输入				: 
*返回值			:	无
*******************************************************************************/
void USM_BSP_ResetToBSP(BSP_Conf_TypeDef *BSP_Conf)			//复位从机设备--使从机进行ISP模式
{
	if(USM_BSP_GetStatus(BSP_Conf)==BSP_MSTATUS_ResetDevice)		//BSP主机准备连接----复位从机
	{
		BSP_Conf->BSP_DATA.TimeCount++;
	}
	else
	{
		return;
	}
	if(BSP_Conf->BSP_DATA.TimeCount<=100)	//拉高约1ms
	{	
		GPIO_SetBits	(BSP_Conf->BSP_Port.BOOT0_CTL_PORT,BSP_Conf->BSP_Port.BOOT0_CTL_Pin);		//BOOT0脚拉高
		GPIO_SetBits	(BSP_Conf->BSP_Port.RESET_CTL_PORT,BSP_Conf->BSP_Port.RESET_CTL_Pin);		//RESET脚拉低				
	}
	else if(BSP_Conf->BSP_DATA.TimeCount>100&&BSP_Conf->BSP_DATA.TimeCount<=10000)	//拉高约50ms		//进入BSP模式			//进入BSP模式
	{
		GPIO_SetBits	(BSP_Conf->BSP_Port.BOOT0_CTL_PORT,BSP_Conf->BSP_Port.BOOT0_CTL_Pin);		//BOOT0脚拉高
		GPIO_ResetBits	(BSP_Conf->BSP_Port.RESET_CTL_PORT,BSP_Conf->BSP_Port.RESET_CTL_Pin);		//RESET脚拉高，进入BSP模式		
	}
	else
	{
		GPIO_SetBits	(BSP_Conf->BSP_Port.RESET_CTL_PORT,BSP_Conf->BSP_Port.RESET_CTL_Pin);		//RESET脚拉高，进入BSP模式
		USM_BSP_SetStatus(BSP_Conf,BSP_MSTATUS_WriteConnect);		//更新状态---BSP主机准备连接----复位后检测从机
		BSP_Conf->BSP_DATA.TimeCount=0;		//清除计数
	}
}
/*******************************************************************************
*函数名			:	USM_BSP_SetDevice
*功能描述		:	启动从机接收BOOT0使从机进行下载状态
*输入				: 
*返回值			:	无
*******************************************************************************/
void USM_BSP_ResetToRUN(BSP_Conf_TypeDef *BSP_Conf)			//启动从机接收BOOT0使从机进行下载状态
{
	if(USM_BSP_GetStatus(BSP_Conf)==BSP_MSTATUS_StartDevice)
	{
		BSP_Conf->BSP_DATA.TimeCount++;
	}
	else
	{
		return;
	}
	if(BSP_Conf->BSP_DATA.TimeCount<=5000)	//拉底约50ms
	{
		GPIO_ResetBits(BSP_Conf->BSP_Port.BOOT0_CTL_PORT,BSP_Conf->BSP_Port.BOOT0_CTL_Pin);		//BOOT0脚拉低
		GPIO_ResetBits(BSP_Conf->BSP_Port.RESET_CTL_PORT,BSP_Conf->BSP_Port.RESET_CTL_Pin);		//RESET脚拉低
	}
	else if(BSP_Conf->BSP_DATA.TimeCount>5000&&BSP_Conf->BSP_DATA.TimeCount<10000)	//拉高约50ms		//进入BSP模式
	{
		GPIO_SetBits	(BSP_Conf->BSP_Port.RESET_CTL_PORT,BSP_Conf->BSP_Port.RESET_CTL_Pin);		//RESET脚拉高，进入BSP模式		
	}
	else
	{
		USM_BSP_SetStatus(BSP_Conf,BSP_MSTATUS_WriteConnect);		//更新状态---等待释放从机
		BSP_Conf->BSP_DATA.TimeCount=0;		//清除计数
	}
}
/*******************************************************************************
*函数名			:	USM_BSP_SetStatus
*功能描述		:	更新/设置主机状态
*输入				: 
*返回值			:	无
*******************************************************************************/
void USM_BSP_SetStatus(BSP_Conf_TypeDef *BSP_Conf,BSPM_STATUS_TypeDef BSPM_STATUS)		//更新/设置主机状态
{
	BSP_Conf->BSPM_Info.BSP_MASTER_STATUS	=	BSPM_STATUS;				//新状态值
}
/*******************************************************************************
*函数名			:	USM_BSP_SetStatus
*功能描述		:	获取主机状态
*输入				: 
*返回值			:	无
*******************************************************************************/
BSPM_STATUS_TypeDef USM_BSP_GetStatus(BSP_Conf_TypeDef *BSP_Conf)		//获取主机状态
{
	return BSP_Conf->BSPM_Info.BSP_MASTER_STATUS;				//主机状态值
}
/*******************************************************************************
*函数名			:	USM_BSP_Connect
*功能描述		:	连接从机---间隔5ms连接发送0x7F，让从机识别波特率
*输入				: 
*返回值			:	无
*******************************************************************************/
void USM_BSP_Connect(BSP_Conf_TypeDef *BSP_Conf)		//连接从机---间隔5ms连接发送0x7F，让从机识别波特率
{
	if(USM_BSP_GetStatus(BSP_Conf)==BSP_MSTATUS_WriteConnect)
	{
		BSP_Conf->BSP_DATA.TimeCount++;
	}
	else
	{
		return;
	}
	
	if(BSP_Conf->BSP_DATA.TimeCount==100)				//前1ms发送0x7F，后9ms检测应答，让从机识别波特率
	{
		BSP_Conf->BSP_DATA.BSP_TvBuffer[0]=0x7F;
		BSP_Conf->BSP_DATA.USARTSendLen=1;				//发送一个字节
	}
	else if(BSP_Conf->BSP_DATA.TimeCount>1000)	//剩下5ms检测应答
	{
		BSP_Conf->BSP_DATA.RetryTimes++;	//重试次数不超过50次，否则判定为失败
		if(BSP_Conf->BSP_DATA.RetryTimes>50)//重试次数不超过50次，否则判定为失败
		{
			USM_BSP_SetStatus(BSP_Conf,BSP_MSTATUS_ERROR);		//更新状态---等待释放从机
		}
		BSP_Conf->BSP_DATA.TimeCount=0;		//清除计数		
	}
	else
	{
		if(BSP_Conf->BSPM_Info.ACK==BSP_ACK)
		{
			USM_BSP_SetStatus(BSP_Conf,BSP_MSTATUS_WriteGet);		//更新状态---等待释放从机
			BSP_Conf->BSP_DATA.TimeCount=0;		//清除计数
			BSP_Conf->BSP_DATA.RetryTimes=0;	//重试计数清零
			BSP_Conf->BSPM_Info.ACK=BSP_NACK;	//清除应答标志
		}		
	}
}
/*******************************************************************************
*函数名			:	USM_BSP_Get
*功能描述		:	获取当前自举程序版本及允许使用的命令
*输入				: 
*返回值			:	无
*******************************************************************************/
void USM_BSP_Get(BSP_Conf_TypeDef *BSP_Conf)		//获取当前自举程序版本及允许使用的命令
{
	if(USM_BSP_GetStatus(BSP_Conf)==BSP_MSTATUS_WriteGet)
	{
		BSP_Conf->BSP_DATA.TimeCount++;
	}
	else
	{
		return;
	}
	if(BSP_Conf->BSP_DATA.TimeCount==100)				//前1ms发送0x7F，后9ms检测应答，让从机识别波特率
	{
		USM_BSP_SendCommand(BSP_Conf,BSP_COMMAND_Get);	//串口编程发送命令程序
	}
	else if(BSP_Conf->BSP_DATA.TimeCount>1000)	//剩下5ms检测应答
	{
		BSP_Conf->BSP_DATA.RetryTimes++;	//重试次数不超过50次，否则判定为失败
		if(BSP_Conf->BSP_DATA.RetryTimes>50)//重试次数不超过50次，否则判定为失败
		{
			USM_BSP_SetStatus(BSP_Conf,BSP_MSTATUS_ERROR);		//更新状态---等待释放从机
		}
		BSP_Conf->BSP_DATA.TimeCount=0;		//清除计数		
	}
	else
	{
		if(BSP_Conf->BSPM_Info.ACK==BSP_ACK)
		{
			BSP_Conf->BSPM_Info.SlaveVersion.BSP_VS					=	BSP_Conf->BSP_DATA.BSP_RvBuffer[2];		//自举程序版本（0 < 版本 < 255），示例：0x10 = 版本 1.0
			BSP_Conf->BSPM_Info.SlaveVersion.BSP_VS_Get			=	BSP_Conf->BSP_DATA.BSP_RvBuffer[3];		//Get:获取当前自举程序版本及允许使用的命令
			BSP_Conf->BSPM_Info.SlaveVersion.BSP_VS_GetVS		=	BSP_Conf->BSP_DATA.BSP_RvBuffer[4];		//Get Version & Read Protection Status:获取自举程序版本及 Flash 的读保护状态
			BSP_Conf->BSPM_Info.SlaveVersion.BSP_VS_GetID		=	BSP_Conf->BSP_DATA.BSP_RvBuffer[5];		//Get ID:获取芯片 ID
			BSP_Conf->BSPM_Info.SlaveVersion.BSP_VS_RM			=	BSP_Conf->BSP_DATA.BSP_RvBuffer[6];		//Read Memory:从应用程序指定的地址开始读取最多 256 个字节的存储器空间
			BSP_Conf->BSPM_Info.SlaveVersion.BSP_VS_Go			=	BSP_Conf->BSP_DATA.BSP_RvBuffer[7];		//Go:跳转到内部 Flash 或 SRAM 内的应用程序代码
			BSP_Conf->BSPM_Info.SlaveVersion.BSP_VS_WM			=	BSP_Conf->BSP_DATA.BSP_RvBuffer[8];		//Write Memory:从应用程序指定的地址开始将最多 256 个字节的数据写入 RAM 或 Flash
			BSP_Conf->BSPM_Info.SlaveVersion.BSP_VS_Erase		=	BSP_Conf->BSP_DATA.BSP_RvBuffer[9];		//Erase:擦除一个到全部 Flash 页面
			BSP_Conf->BSPM_Info.SlaveVersion.BSP_VS_WP			=	BSP_Conf->BSP_DATA.BSP_RvBuffer[10];	//Write Protect:使能某些扇区的写保护
			BSP_Conf->BSPM_Info.SlaveVersion.BSP_VS_WU			=	BSP_Conf->BSP_DATA.BSP_RvBuffer[11];	//Write Unprotect:禁止所有 Flash 扇区的写保护
			BSP_Conf->BSPM_Info.SlaveVersion.BSP_VS_RP			=	BSP_Conf->BSP_DATA.BSP_RvBuffer[12];	//Readout Protect:使能读保护
			BSP_Conf->BSPM_Info.SlaveVersion.BSP_VS_RU			=	BSP_Conf->BSP_DATA.BSP_RvBuffer[13];	//Readout Unprotect:禁止读保护
			
			USM_BSP_SetStatus(BSP_Conf,BSP_MSTATUS_WriteGetVR);		//更新状态---等待释放从机
			BSP_Conf->BSP_DATA.TimeCount=0;		//清除计数
			BSP_Conf->BSP_DATA.RetryTimes=0;	//重试计数清零
			BSP_Conf->BSPM_Info.ACK=BSP_NACK;	//清除应答标志
		}		
	}
}
/*******************************************************************************
*函数名			:	Get Version & Read Protection Status
*功能描述		:	获取自举程序版本及 Flash 的读保护状态
*输入				: 
*返回值			:	无
*******************************************************************************/
void USM_BSP_GetVR(BSP_Conf_TypeDef *BSP_Conf)		//获取自举程序版本及 Flash 的读保护状态
{
	if(USM_BSP_GetStatus(BSP_Conf)==BSP_MSTATUS_WriteGetVR)
	{
		BSP_Conf->BSP_DATA.TimeCount++;
	}
	else
	{
		return;
	}
	if(BSP_Conf->BSP_DATA.TimeCount==100)				//前1ms发送0x7F，后9ms检测应答，让从机识别波特率
	{
		USM_BSP_SendCommand(BSP_Conf,BSP_COMMAND_GetVR);	//串口编程发送命令程序
	}
	else if(BSP_Conf->BSP_DATA.TimeCount>1000)	//剩下5ms检测应答
	{
		BSP_Conf->BSP_DATA.RetryTimes++;	//重试次数不超过50次，否则判定为失败
		if(BSP_Conf->BSP_DATA.RetryTimes>50)//重试次数不超过50次，否则判定为失败
		{
			USM_BSP_SetStatus(BSP_Conf,BSP_MSTATUS_ERROR);		//更新状态---等待释放从机
		}	
		BSP_Conf->BSP_DATA.TimeCount=0;		//清除计数		
	}
	else
	{
		if(BSP_Conf->BSPM_Info.ACK==BSP_ACK)
		{
			
			USM_BSP_SetStatus(BSP_Conf,BSP_MSTATUS_WriteGetID);		//更新状态---等待释放从机
			BSP_Conf->BSP_DATA.TimeCount=0;		//清除计数
			BSP_Conf->BSP_DATA.RetryTimes=0;	//重试计数清零
			BSP_Conf->BSPM_Info.ACK=BSP_NACK;	//清除应答标志
		}		
	}
}
/*******************************************************************************
*函数名			:	USM_BSP_GetId
*功能描述		:	获取芯片 ID
*输入				: 
*返回值			:	无
*******************************************************************************/
void USM_BSP_GetId(BSP_Conf_TypeDef *BSP_Conf)			//获取芯片 ID
{
	if(USM_BSP_GetStatus(BSP_Conf)==BSP_MSTATUS_WriteGetID)
	{
		BSP_Conf->BSP_DATA.TimeCount++;
	}
	else
	{
		return;
	}
	if(BSP_Conf->BSP_DATA.TimeCount==100)				//前1ms发送0x7F，后9ms检测应答，让从机识别波特率
	{
		USM_BSP_SendCommand(BSP_Conf,BSP_COMMAND_GetID);	//串口编程发送命令程序
	}
	else if(BSP_Conf->BSP_DATA.TimeCount>1000)	//剩下5ms检测应答
	{
		BSP_Conf->BSP_DATA.RetryTimes++;	//重试次数不超过50次，否则判定为失败
		if(BSP_Conf->BSP_DATA.RetryTimes>50)//重试次数不超过50次，否则判定为失败
		{
			USM_BSP_SetStatus(BSP_Conf,BSP_MSTATUS_ERROR);		//更新状态---等待释放从机
		}	
		BSP_Conf->BSP_DATA.TimeCount=0;		//清除计数		
	}
	else
	{
		if(BSP_Conf->BSPM_Info.ACK==BSP_ACK)
		{
			
			USM_BSP_SetStatus(BSP_Conf,BSP_MSTATUS_WriteErase);		//更新状态---等待释放从机
			BSP_Conf->BSP_DATA.TimeCount=0;		//清除计数
			BSP_Conf->BSP_DATA.RetryTimes=0;	//重试计数清零
			BSP_Conf->BSPM_Info.ACK=BSP_NACK;	//清除应答标志
		}		
	}
}
/*******************************************************************************
*函数名			:	USM_BSP_ReadMemory
*功能描述		:	从应用程序指定的地址开始读取最多 256 个字节的存储器空间
*输入				: 
*返回值			:	无
*******************************************************************************/
void USM_BSP_ReadMemory(BSP_Conf_TypeDef *BSP_Conf)		//从应用程序指定的地址开始读取最多 256 个字节的存储器空间
{

}
/*******************************************************************************
*函数名			:	USM_BSP_Go
*功能描述		:	跳转到内部 Flash 或 SRAM 内的应用程序代码
*输入				: 
*返回值			:	无
*******************************************************************************/
void USM_BSP_Go(BSP_Conf_TypeDef *BSP_Conf)		//跳转到内部 Flash 或 SRAM 内的应用程序代码
{

}
/*******************************************************************************
*函数名			:	USM_BSP_EraseMemory
*功能描述		:	从应用程序指定的地址开始将最多 256 个字节的数据写入 RAM 或 Flash
						:	1-检测起始写入地址是否为空（0x0000)::起始地址存储在FLASH，检测到BSP_MSTATUS_WriteWM状态时读取FLASH得到起始地址存入StartAddr
						:	2-
*输入				: 
*返回值			:	无
*******************************************************************************/
void USM_BSP_WriteMemory(BSP_Conf_TypeDef *BSP_Conf)		//从应用程序指定的地址开始将最多 256 个字节的数据写入 RAM 或 Flash
{
	if(USM_BSP_GetStatus(BSP_Conf)==BSP_MSTATUS_WriteWM)	//BSP主机发送写命令，Write Memory:从应用程序指定的地址开始将最多 256 个字节的数据写入 RAM 或 Flash---需要应答
	{
		BSP_Conf->BSP_DATA.TimeCount++;						//计时周期10uS
		if(BSP_Conf->BSP_DATA.TimeCount<=5000)		//超时时间50mS
		{
			if(BSP_Conf->BSP_DATA.StartAddr!=0)			//接收到起始地址--设定写地址状态
			{
				BSP_Conf->BSP_DATA.TimeCount=0;		//清除计数
				BSP_Conf->BSP_DATA.WriteAddr=BSP_Conf->BSP_DATA.StartAddr;		//WriteAddr	//要写入Flash的数据起始地址--ISP接口
				BSP_Conf->BSP_DATA.SumHaveWritten=0;	//主机总共往从机写入的数据个数---清零
				USM_BSP_SetStatus(BSP_Conf,BSP_MSTATUS_WriteAddr);		//更新状态---BSP主机发送要写入数据的起始地址---需要应答
			}
			else
			{
				return;			//未检测到起始地址，退出，继续等待
			}
		}
		else		//超时---状态更新为错误状态
		{
			USM_BSP_SetStatus(BSP_Conf,BSP_MSTATUS_ERROR);		//更新状态---错误
			return;
		}
	}
	else
	{
		return;
	}
}
/*******************************************************************************
*函数名			:	USM_BSP_WriteAddr
*功能描述		:	写地址----包含地址自动增加及完成判断
						:	检测串口是否有待发送数据，如果无，将地址存入发送备份区
						:	检测从机应答，并重试10次，
						:	应答成功后将状态设置为写数据，并准备下一个等写数据地址
*输入				: 
*返回值			:	无
*******************************************************************************/
void USM_BSP_WriteAddr(BSP_Conf_TypeDef *BSP_Conf)		//写地址----包含地址自动增加及完成判断
{
	if(USM_BSP_GetStatus(BSP_Conf)==BSP_MSTATUS_WriteAddr&&BSP_Conf->BSP_DATA.USARTSendLen==0)	//BSP主机发送要写入数据的起始地址并且串口无数据等待发送---需要应答
	{
		BSP_Conf->BSP_DATA.TimeCount++;												//计时周期10uS	
	}
	else		//跳过
	{
		return;
	}
	if(BSP_Conf->BSP_DATA.TimeCount==5)		//发送数据
	{	
		BSP_Conf->BSP_DATA.BSP_TvBuffer[0]=BSP_Conf->BSP_DATA.WriteAddr>>24;
		BSP_Conf->BSP_DATA.BSP_TvBuffer[1]=BSP_Conf->BSP_DATA.WriteAddr>>16;
		BSP_Conf->BSP_DATA.BSP_TvBuffer[2]=BSP_Conf->BSP_DATA.WriteAddr>>8;
		BSP_Conf->BSP_DATA.BSP_TvBuffer[3]=BSP_Conf->BSP_DATA.WriteAddr>>0;
		BSP_Conf->BSP_DATA.BSP_TvBuffer[4]=BCC8(BSP_Conf->BSP_DATA.BSP_TvBuffer,4);		//异或校验;
		BSP_Conf->BSP_DATA.USARTSendLen=5;
	}
	else if(BSP_Conf->BSP_DATA.TimeCount>200)		//2ms未检测到应答重试一次
	{
		BSP_Conf->BSP_DATA.RetryTimes++;			//重试次数不超过10次，否则判定为失败
		if(BSP_Conf->BSP_DATA.RetryTimes>10)	//重试次数不超过10次，否则判定为失败
		{
			USM_BSP_SetStatus(BSP_Conf,BSP_MSTATUS_ERROR);		//更新状态---等待释放从机
		}	
		BSP_Conf->BSP_DATA.TimeCount=0;				//清除计数--重新开时进入擦除操作
	}
	else if(BSP_Conf->BSP_DATA.TimeCount>5)		//等待应答
	{
		if(BSP_Conf->BSPM_Info.ACK==BSP_ACK)		//检测到应答
		{	
			USM_BSP_SetStatus(BSP_Conf,BSP_MSTATUS_WriteData);		//更新状态---BSP主机写数据，最多 256 个字节
			BSP_Conf->BSP_DATA.TimeCount=0;			//清除计数
			BSP_Conf->BSP_DATA.RetryTimes=0;		//重试计数清零
			BSP_Conf->BSPM_Info.ACK=BSP_NACK;		//清除应答标志
			BSP_Conf->BSP_DATA.WriteAddr+=BSP_Conf->BSP_DATA.StepLen;		//根据每次写入的字节长度增加地址，备下一个地址写入
			return;		//退出
		}
		else		//跳过
		{
			return;
		}
	}
	else		//跳过
	{
		return;
	}
}
/*******************************************************************************
*函数名			:	USM_BSP_WriteAddr
*功能描述		:	写地址----包含地址自动增加及完成判断
*输入				: 
*返回值			:	无
*******************************************************************************/
void USM_BSP_WriteData(BSP_Conf_TypeDef *BSP_Conf)		//写数据----向从机写入FLASH数据
{
	if(
		USM_BSP_GetStatus(BSP_Conf)==BSP_MSTATUS_WriteData&&	//BSP主机发送数据，---需要应答
		BSP_Conf->BSP_DATA.USARTSendLen==0&&		//串口无待发送数据
		BSP_Conf->BSP_DATA.WriteLen							//FLASH已经准备好数据，可以通过串口发往从机
	)	//BSP主机发送数据，---需要应答
	{
		BSP_Conf->BSP_DATA.TimeCount++;												//计时周期10uS	
	}
	else if(BSP_Conf->BSP_DATA.TimeCount>5000)			//50ms超时
	{
		USM_BSP_SetStatus(BSP_Conf,BSP_MSTATUS_ERROR);		//更新状态---错误
	}
	else	//跳过
	{
		return;
	}
	if(BSP_Conf->BSP_DATA.TimeCount==1)
	{
		BSP_Conf->BSP_DATA.USARTSendLen=BSP_Conf->BSP_DATA.WriteLen;				//待写入的数据长度
		BSP_Conf->BSP_DATA.SumHaveWritten+=BSP_Conf->BSP_DATA.USARTSendLen;	//主机总共往从机写入的数据个数---计数
		memcpy(BSP_Conf->BSP_DATA.BSP_TxBuffer, BSP_Conf->BSP_DATA.BSP_FlashBuffer,BSP_Conf->BSP_DATA.USARTSendLen);	//复制数据---将读取的FLASH数据复制到串口发送缓冲区
	}
	else	//检测应答
	{
		if(BSP_Conf->BSPM_Info.ACK==BSP_ACK)	//检测到应答
		{
			if(BSP_Conf->BSP_DATA.SumHaveWritten<BSP_Conf->BSP_DATA.FirmwareLen)	//还有待写入数据
			{
				USM_BSP_SetStatus(BSP_Conf,BSP_MSTATUS_WriteAddr);		//更新状态---BSP主机发送要写入数据的起始地址---需要应答
				BSP_Conf->BSP_DATA.TimeCount=0;			//清除计数
				BSP_Conf->BSPM_Info.ACK=BSP_NACK;		//清除应答标志
				BSP_Conf->BSP_DATA.WriteLen=0;			//待写入的数据长度---清零，以备FLAHS准备下一组数据
			}
			else	//全部数据写入完成---后续为重启从机，从GoAddr开始运行从机
			{
				USM_BSP_SetStatus(BSP_Conf,BSP_MSTATUS_WriteGo);		//更新状态---BSP主机发送Go命令---执行完Go全部命令后重启从机运行
				BSP_Conf->BSP_DATA.TimeCount=0;			//清除计数
				BSP_Conf->BSPM_Info.ACK=BSP_NACK;		//清除应答标志
				BSP_Conf->BSP_DATA.WriteLen=0;			//待写入的数据长度---清零，以备FLAHS准备下一组数据
			}			
		}
		else
		{
			return;
		}
	}
}
/*******************************************************************************
*函数名			:	USM_BSP_EraseMemory
*功能描述		:	擦除一个到全部 Flash 页面（擦除FLASH后开始写入地址）
*输入				: 
*返回值			:	无
*******************************************************************************/
void USM_BSP_EraseMemory(BSP_Conf_TypeDef *BSP_Conf)		//擦除一个到全部 Flash 页面（擦除FLASH后开始写入地址）
{
	if(USM_BSP_GetStatus(BSP_Conf)==BSP_MSTATUS_WriteErase)
	{
		BSP_Conf->BSP_DATA.TimeCount++;
	}
	else
	{
		return;
	}
	if(BSP_Conf->BSP_DATA.TimeCount==100)				//前1ms发送0x7F，后9ms检测应答，让从机识别波特率
	{
		if(BSP_Conf->BSPM_Info.ACK2!=BSP_ACK)
		{
			USM_BSP_SendCommand(BSP_Conf,BSP_COMMAND_Erase);	//串口编程发送命令程序---擦除命令
		}
		else
		{
			BSP_Conf->BSP_DATA.BSP_TvBuffer[0]=0xFF;
			BSP_Conf->BSP_DATA.BSP_TvBuffer[1]=0x00;
			BSP_Conf->BSP_DATA.USARTSendLen=2;				//发送一个字节
		}
	}
	else if(BSP_Conf->BSP_DATA.TimeCount>1000)	//剩下5ms检测应答
	{
		BSP_Conf->BSP_DATA.RetryTimes++;			//重试次数不超过50次，否则判定为失败
		if(BSP_Conf->BSP_DATA.RetryTimes>50)	//重试次数不超过50次，否则判定为失败
		{
			USM_BSP_SetStatus(BSP_Conf,BSP_MSTATUS_ERROR);		//更新状态---错误
		}	
		BSP_Conf->BSP_DATA.TimeCount=0;				//清除计数--重新开时进入擦除操作		
	}
	else	//以下过程为不断检测应答标志---USM_BSP_GetAck(BSP_Conf);	//获取从机应答
	{
		if(BSP_Conf->BSPM_Info.ACK==BSP_ACK)		//检测到应答
		{	
			if(BSP_Conf->BSPM_Info.ACK2!=BSP_ACK)	//第二次未设置应答标志，表示为第一次应答
			{
				BSP_Conf->BSP_DATA.TimeCount=0;		//清除计数
				BSP_Conf->BSP_DATA.RetryTimes=0;	//重试计数清零
				BSP_Conf->BSPM_Info.ACK=BSP_NACK;	//清除应答标志--应答检测标志
				BSP_Conf->BSPM_Info.ACK2=BSP_ACK;	//存储上次应答标志
			}
			else		//检测到第二次成功应答，表示此项成功完成
			{
				USM_BSP_SetStatus(BSP_Conf,BSP_MSTATUS_WriteWM);		//更新状态---BSP主机发送写命令，Write Memory:从应用程序指定的地址开始将最多 256 个字节的数据写入 RAM 或 Flash---需要应答
				BSP_Conf->BSP_DATA.TimeCount=0;			//清除计数
				BSP_Conf->BSP_DATA.RetryTimes=0;		//重试计数清零
				BSP_Conf->BSPM_Info.ACK=BSP_NACK;		//清除应答标志
				BSP_Conf->BSPM_Info.ACK2=BSP_NACK;	//清除应答标志
			}			
		}		
	}
}
/*******************************************************************************
*函数名			:	USM_BSP_EEraseMemory
*功能描述		:	使用双字节寻址模式擦除一个到全部 Flash 页面（仅用于v3.0 usart 自举程序版本及以上版本）
*输入				: 
*返回值			:	无
*******************************************************************************/
void USM_BSP_EEraseMemory(BSP_Conf_TypeDef *BSP_Conf)		//使用双字节寻址模式擦除一个到全部 Flash 页面（仅用于v3.0 usart 自举程序版本及以上版本）
{

}
/*******************************************************************************
*函数名			:	USM_BSP_WriteProtect
*功能描述		:	使能某些扇区的写保护
*输入				: 
*返回值			:	无
*******************************************************************************/
void USM_BSP_WriteProtect(BSP_Conf_TypeDef *BSP_Conf)		//使能某些扇区的写保护
{

}
/*******************************************************************************
*函数名			:	USM_BSP_WriteUnProtect
*功能描述		:	禁止所有 Flash 扇区的写保护
*输入				: 
*返回值			:	无
*******************************************************************************/
void USM_BSP_WriteUnProtect(BSP_Conf_TypeDef *BSP_Conf)		//禁止所有 Flash 扇区的写保护
{

}
/*******************************************************************************
*函数名			:	USM_BSP_ReadProtect
*功能描述		:	使能读保护
*输入				: 
*返回值			:	无
*******************************************************************************/
void USM_BSP_ReadProtect(BSP_Conf_TypeDef *BSP_Conf)		//使能读保护
{

}
/*******************************************************************************
*函数名			:	USM_BSP_ReadUnProtect
*功能描述		:	禁止读保护
*输入				: 
*返回值			:	无
*******************************************************************************/
void USM_BSP_ReadUnProtect(BSP_Conf_TypeDef *BSP_Conf)		//禁止读保护
{

}
/*******************************************************************************
*函数名			:	USM_BSP_GetAck
*功能描述		:	获取从机应答
*输入				: 
*返回值			:	无
*******************************************************************************/
void USM_BSP_GetAck(BSP_Conf_TypeDef *BSP_Conf)		//获取从机应答
{
	unsigned char RxNum=0;
	RxNum=USART_ReadBufferIDLE(BSP_Conf->BSP_Port.USARTx,(u32*)BSP_Conf->BSP_DATA.BSP_RvBuffer,(u32*)BSP_Conf->BSP_DATA.BSP_RxBuffer);	//串口空闲模式读串口接收缓冲区，如果有数据，将数据拷贝到RevBuffer,并返回接收到的
	if((RxNum!=0)&&(BSP_Conf->BSP_DATA.BSP_RvBuffer[0]==BSP_ACK))
	{
		BSP_Conf->BSP_DATA.BSP_RvBuffer[0]=0xFF;		//清除应答标识
		BSP_Conf->BSPM_Info.ACK=BSP_ACK;		//应答
	}
}
/*******************************************************************************
* 函数名			:	Usart_MISP_CommandSend
* 功能描述		:	串口编程发送命令程序
* 输入			: void
* 返回值			: void
*******************************************************************************/
void USM_BSP_SendCommand(BSP_Conf_TypeDef *BSP_Conf,BSP_COMMAND_TypeDef Command)	//串口编程发送命令程序
{
	BSP_Conf->BSP_DATA.Command[0]=Command;
	BSP_Conf->BSP_DATA.Command[1]=Command^0XFF;
	USART_DMASend(BSP_Conf->BSP_Port.USARTx,(u32*)BSP_Conf->BSP_DATA.Command,2);	//串口DMA发送程序
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	发送函数包括从机发送应答、发送地址、发送命令、发送数据
*输入				: 
*返回值			:	无
*******************************************************************************/
void USM_BSP_SendBuffer(BSP_Conf_TypeDef *BSP_Conf)						//发送函数包括从机发送应答、发送地址、发送命令、发送数据
{
	if(BSP_Conf->BSP_DATA.USARTSendLen)
	{
		memcpy(BSP_Conf->BSP_DATA.BSP_TxBuffer, BSP_Conf->BSP_DATA.BSP_TvBuffer,BSP_Conf->BSP_DATA.USARTSendLen);				//复制数据
		memset(BSP_Conf->BSP_DATA.BSP_TvBuffer,0xFF, BSP_BufferSize);				//接收缓冲区
		USART_DMASend(BSP_Conf->BSP_Port.USARTx,(u32*)BSP_Conf->BSP_DATA.BSP_TxBuffer,BSP_Conf->BSP_DATA.USARTSendLen);	//串口DMA发送程序
		BSP_Conf->BSP_DATA.USARTSendLen=0;
	}
}
















//---------------------从机专用函数
/*******************************************************************************
*函数名			:	USS_BSP_PROCESS
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void USS_BSP_PROCESS(BSP_Conf_TypeDef *BSP_Conf)
{

}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void USS_BSP_RESET(BSP_Conf_TypeDef *BSP_Conf)
{

}
/*******************************************************************************
*函数名			:	USS_BSP_Get
*功能描述		:	获取当前自举程序版本及允许使用的命令
*输入				: 
*返回值			:	无
*******************************************************************************/
void USS_BSP_Get(BSP_Conf_TypeDef *BSP_Conf)		//获取当前自举程序版本及允许使用的命令
{

}
/*******************************************************************************
*函数名			:	Get Version & Read Protection Status
*功能描述		:	获取自举程序版本及 Flash 的读保护状态
*输入				: 
*返回值			:	无
*******************************************************************************/
void USS_BSP_GetVR(BSP_Conf_TypeDef *BSP_Conf)		//获取自举程序版本及 Flash 的读保护状态
{

}
/*******************************************************************************
*函数名			:	USS_BSP_GetId
*功能描述		:	获取芯片 ID
*输入				: 
*返回值			:	无
*******************************************************************************/
void USS_BSP_GetId(BSP_Conf_TypeDef *BSP_Conf)			//获取芯片 ID
{

}
/*******************************************************************************
*函数名			:	USS_BSP_ReadMemory
*功能描述		:	从应用程序指定的地址开始读取最多 256 个字节的存储器空间
*输入				: 
*返回值			:	无
*******************************************************************************/
void USS_BSP_ReadMemory(BSP_Conf_TypeDef *BSP_Conf)		//从应用程序指定的地址开始读取最多 256 个字节的存储器空间
{

}
/*******************************************************************************
*函数名			:	USS_BSP_Go
*功能描述		:	跳转到内部 Flash 或 SRAM 内的应用程序代码
*输入				: 
*返回值			:	无
*******************************************************************************/
void USS_BSP_Go(BSP_Conf_TypeDef *BSP_Conf)		//跳转到内部 Flash 或 SRAM 内的应用程序代码
{

}
/*******************************************************************************
*函数名			:	USS_BSP_WriteMemory
*功能描述		:	从应用程序指定的地址开始将最多 256 个字节的数据写入 RAM 或 Flash
*输入				: 
*返回值			:	无
*******************************************************************************/
void USS_BSP_WriteMemory(BSP_Conf_TypeDef *BSP_Conf)		//从应用程序指定的地址开始将最多 256 个字节的数据写入 RAM 或 Flash
{

}
/*******************************************************************************
*函数名			:	USS_BSP_EraseMemory
*功能描述		:	擦除一个到全部 Flash 页面
*输入				: 
*返回值			:	无
*******************************************************************************/
void USS_BSP_EraseMemory(BSP_Conf_TypeDef *BSP_Conf)		//擦除一个到全部 Flash 页面
{

}
/*******************************************************************************
*函数名			:	USS_BSP_EEraseMemory
*功能描述		:	使用双字节寻址模式擦除一个到全部 Flash 页面（仅用于v3.0 usart 自举程序版本及以上版本）
*输入				: 
*返回值			:	无
*******************************************************************************/
void USS_BSP_EEraseMemory(BSP_Conf_TypeDef *BSP_Conf)		//使用双字节寻址模式擦除一个到全部 Flash 页面（仅用于v3.0 usart 自举程序版本及以上版本）
{

}
/*******************************************************************************
*函数名			:	USS_BSP_WriteProtect
*功能描述		:	使能某些扇区的写保护
*输入				: 
*返回值			:	无
*******************************************************************************/
void USS_BSP_WriteProtect(BSP_Conf_TypeDef *BSP_Conf)		//使能某些扇区的写保护
{

}
/*******************************************************************************
*函数名			:	USS_BSP_WriteUnProtect
*功能描述		:	禁止所有 Flash 扇区的写保护
*输入				: 
*返回值			:	无
*******************************************************************************/
void USS_BSP_WriteUnProtect(BSP_Conf_TypeDef *BSP_Conf)		//禁止所有 Flash 扇区的写保护
{

}
/*******************************************************************************
*函数名			:	USS_BSP_ReadProtect
*功能描述		:	使能读保护
*输入				: 
*返回值			:	无
*******************************************************************************/
void USS_BSP_ReadProtect(BSP_Conf_TypeDef *BSP_Conf)		//使能读保护
{

}
/*******************************************************************************
*函数名			:	USS_BSP_ReadUnProtect
*功能描述		:	禁止读保护
*输入				: 
*返回值			:	无
*******************************************************************************/
void USS_BSP_ReadUnProtect(BSP_Conf_TypeDef *BSP_Conf)		//禁止读保护
{

}
/*******************************************************************************
*函数名			:	USS_BSP_ACK
*功能描述		:	向主机发送应答(0x79)
*输入				: 
*返回值			:	无
*******************************************************************************/
void USS_BSP_ACK(BSP_Conf_TypeDef *BSP_Conf)		//向主机发送应答(0x79)
{

}
/*******************************************************************************
*函数名			:	USS_BSP_NACK
*功能描述		:	向主机发送非应答(0x1F)
*输入				: 
*返回值			:	无
*******************************************************************************/
void USS_BSP_NACK(BSP_Conf_TypeDef *BSP_Conf)		//向主机发送非应答(0x1F)
{

}
























