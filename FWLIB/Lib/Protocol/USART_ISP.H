#ifndef __USART_ISP_H
#define __USART_ISP_H

#include "stm32f10x_type.h"
#include "stm32f10x_usart.h"
//#include "stm32f10x_gpio.h"
//#include "stm32f10x_spi.h"

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

/*-------------------------通信安全--------------------
编程工具 (PC) 到器件的所有通信均通过如下方式验证：
1. 校验和：接收到的数据字节块进行异或运算。每个通信结尾增加一个字节（校验和字节），
	包含前面所有字节异或运算的结果。异或运算所有接收到的字节，即数据包加上校验和字节，结果必须为 0x00
2. 针对每条命令，主机都会发送一个字节及其补码（异或结果 = 0x00）
3. UART：激活奇偶校验（偶校验）
每个数据包或者被接受（ACK 应答）或者被丢弃（NACK 应答）：
--------------------------------------------------------------------------------------*/
#define	ISP_BufferSize	256
	
typedef	enum _ISP_COMMAND			//USART 自举程序命令
{
	ISP_COMMAND_Get		=0x00,		//Get:获取当前自举程序版本及允许使用的命令
	ISP_COMMAND_GetVS	=0x01,		//Get Version & Read Protection Status:获取自举程序版本及 Flash 的读保护状态
	ISP_COMMAND_GetID	=0x02,		//Get ID:获取芯片 ID
	ISP_COMMAND_RM		=0x11,		//Read Memory:从应用程序指定的地址开始读取最多 256 个字节的存储器空间
	ISP_COMMAND_Go		=0x21,		//Go:跳转到内部 Flash 或 SRAM 内的应用程序代码
	ISP_COMMAND_WM		=0x31,		//Write Memory:从应用程序指定的地址开始将最多 256 个字节的数据写入 RAM 或 Flash
	ISP_COMMAND_Erase	=0x43,		//Erase:擦除一个到全部 Flash 页面
	ISP_COMMAND_EE		=0x44,		//Extended Erase:使用双字节寻址模式擦除一个到全部 Flash 页面（仅用于v3.0 usart 自举程序版本及以上版本）。
	ISP_COMMAND_WP		=0x63,		//Write Protect:使能某些扇区的写保护
	ISP_COMMAND_WU		=0x73,		//Write Unprotect:禁止所有 Flash 扇区的写保护
	ISP_COMMAND_RP		=0x82,		//Readout Protect:使能读保护
	ISP_COMMAND_RU		=0x92			//Readout Unprotect:禁止读保护
	//说明：
	//1)如果接收到拒绝命令或在执行命令期间出现错误，自举程序则会发送 NACK 字节并返回检查命令状态。
	//2)读保护 - 激活 RDP（读保护）选项后，只能使用这一有限的命令子集。其它命令都会收到 NACK 应答，并且不会对器件起作用。取消 RDP 即可激活其它命令。
	//3)Erase (x043) 和 Extended Erase (0x44) 均为独占命令。一个器件可支持 Erase 命令或 Extended Erase 命令，但不能同时支持这两个命令。
}ISP_COMMAND_TypeDef;

typedef	enum _ISP_Answer	//USART 应答
{
	ISP_ANSWER_ACK		=0x79,		//ACK 应答(被接受）
	ISP_ANSWER_NACK		=0x1F			//NACK 应答（不接受或者不识别--丢弃)
}ISP_ANSWER_TypeDef;
typedef	enum _ISP_FUN	//USART 应答
{
	ISP_IDLE			=0xA0,	//ISP状态为空闲，根据RESET接口状态和BOOT0接口状态可以切换为MASTER或者SLAVE
	ISP_SLAVE,						//ISP作为从机
	ISP_MASTER						//ISP作为主机--更新自身程序
}ISP_FUN_TypeDef;

//typedef	enum _ISP_STEPS			//ISP执行步骤
//{
//	Step_Start				=	0,			//起始步骤，当前为空闲
//	Step_Get					=	1,			//等待写操作完成，只有写操作完成后才可以进行写或者擦除操作
//	Step_GetVS				=	2,			//写使能
//	Step_GetID				=	3,			//写状态：		读取状态为1表示正在执行写操作，否则表示写入完成
//	Step_RM						=	4,			//读
//	Step_WaitRaddr		=	5,			//等待接收需要读取的地址
//	Step_WaitRlen			=	6,			//等待接收需要读取的数据长度
//	Step_ReadData			=	7,			//等待接收需要读取的数据长度
//	Step_SendRData		=	8,			//发送读取的数据
//	Step_Erase				=	9,			//擦除操作
//	Step_WaitWaddr		=	10,			//等待接收需要写入的数据起始地址
//	Step_WriteData		=	11,			//在在写入数据
//	Step_EraseAll			=	12,			//全部擦除
//	Step_ErasePage		=	13,			//擦除页
//	Step_WaitErasePageA	=	14,		//WaitErasePageA要擦除的页数
//	Step_WaitWlen			=	15,			//等待接收需要写入的数据长度
//	Step_WaitWData		=	16			//等待接收要写入的数据
//}ISP_STEPS_TypeDef;

typedef enum _ISP_SLAVE_STATUS			//ISP状态标志---工作在从机状态时
{
	ISP_STATUS_IDLE=0x00,				//ISP空闲状态，可以读写
	ISP_STATUS_UnConnect,				//ISP未连接（作为从机)
	ISP_STATUS_WaitConnect,			//ISP等待连接（作为从机)
	ISP_STATUS_Connectting,			//ISP正在连接（作为从机)---执行格式化
	ISP_STATUS_Connectted,			//ISP已经连接（作为从机)
	ISP_STATUS_WaitCommand,			//ISP等待命令（作为从机)
	ISP_STATUS_WaitEraseAddr,		//ISP等待待擦除地址
	ISP_STATUS_WaitErase,				//ISP等待擦除
	ISP_STATUS_Eraseing,				//ISP正在擦除
	ISP_STATUS_WaitErased,			//ISP等待擦除完成
	ISP_STATUS_WaitWAddr,				//ISP等待待写入地址
	ISP_STATUS_WaitWData,				//ISP等待接收待写入数据
	ISP_STATUS_WaitWrite,				//ISP等待写数据
	ISP_STATUS_WaitWrited,			//ISP等待写入完成
	ISP_STATUS_WaitReadAddr,		//ISP等待待读取地址
	ISP_STATUS_WaitReadData,		//ISP等待读数据操作
	ISP_STATUS_WaitLengh,				//ISP等待待读取长度
	ISP_STATUS_WaitSData,				//ISP等待发送数据
	ISP_STATUS_WaitGoAddr,				//等待待跳转到内部 Flash 或 SRAM 内的应用程序代码地址//Go 命令用于从应用程序指定的地址开始执行已下载的代码或其它任何代码
	ISP_STATUS_WaitReset				//ISP等待发送数据
}ISP_SLAVE_STATUS_TypeDef;
typedef enum _ISP_MASTER_STATUS		//ISP状态标志---工作在主机状态时
{
	ISP_MSTATUS_IDLE=0x1E,			//ISP主机空闲状态----开始写前或者写完成后状态
	ISP_MSTATUS_ResetDevice,		//ISP主机准备连接----复位从机
	ISP_MSTATUS_WriteConnect,		//ISP主机准备连接----复位后检测从机
//	ISP_MSTATUS_Connectting,		//ISP主机与从机正在建立连接
	ISP_MSTATUS_Connectted,			//ISP主机与从机已经连接完成
	ISP_MSTATUS_WriteGet,				//ISP主机获取从机设备信息---主要为协议信息
	ISP_MSTATUS_WaitAck,				//ISP主机等待从机应答
	ISP_MSTATUS_WriteEraseCMD,	//ISP主机写擦除命令---需要应答
	ISP_MSTATUS_WriteEraseConf,	//ISP主机写擦除参数---整片擦除(0xFF 0x00)或者选择地址擦除，然后等待擦除完成，完成后从机返回一个ACK应答
	ISP_MSTATUS_WriteWM,				//ISP主机发送写命令，Write Memory:从应用程序指定的地址开始将最多 256 个字节的数据写入 RAM 或 Flash---需要应答
	ISP_MSTATUS_WriteAddr,			//ISP主机发送要写入数据的起始地址---需要应答
	ISP_MSTATUS_WriteData,			//ISP主机正在执行擦除工作
	ISP_MSTATUS_WriteRM,				//ISP主机发送读命令--主要为校验写入的数据，Read Memory:从应用程序指定的地址开始读取最多 256 个字节的存储器空间
	ISP_MSTATUS_WriteRMLengh,		//ISP主机发送需要读取的数据长度
	ISP_MSTATUS_Verify,					//数据校验，主机写完从机后校验数据
	ISP_MSTATUS_WriteGo,				//ISP主机发送Go命令---执行完Go全部命令后重启从机运行
	ISP_MSTATUS_ResetDevice2,		//ISP主机往从机写完全部数据后对从机进行复位重启
}ISP_MASTER_STATUS_TypeDef;

typedef	struct _ISP_Version				//USART 自举程序版本---主机通过复位从机后获取
{
	unsigned char ISP_VS				;		//自举程序版本（0 < 版本 < 255），示例：0x10 = 版本 1.0
	unsigned char ISP_VS_Get		;		//Get:获取当前自举程序版本及允许使用的命令
	unsigned char ISP_VS_GetVS	;		//Get Version & Read Protection Status:获取自举程序版本及 Flash 的读保护状态
	unsigned char ISP_VS_GetID	;		//Get ID:获取芯片 ID
	unsigned char ISP_VS_RM			;		//Read Memory:从应用程序指定的地址开始读取最多 256 个字节的存储器空间
	unsigned char ISP_VS_Go			;		//Go:跳转到内部 Flash 或 SRAM 内的应用程序代码
	unsigned char ISP_VS_WM			;		//Write Memory:从应用程序指定的地址开始将最多 256 个字节的数据写入 RAM 或 Flash
	unsigned char ISP_VS_Erase	;		//Erase:擦除一个到全部 Flash 页面
	unsigned char ISP_VS_WP			;		//Write Protect:使能某些扇区的写保护
	unsigned char ISP_VS_WU			;		//Write Unprotect:禁止所有 Flash 扇区的写保护
	unsigned char ISP_VS_RP			;		//Readout Protect:使能读保护
	unsigned char ISP_VS_RU			;		//Readout Unprotect:禁止读保护
}ISP_VERSION_TypeDef;

typedef	struct _ISP_DATA				//ISP数据存储区域
{
	unsigned int OffsetAddr				;		//写从机时的地址偏移
	unsigned int StartAddr				;		//起始地址
	unsigned int GoAddr						;		//复位运行起始地址
	
	unsigned int ReceivedLen			;		//串口接收的数据个数
	unsigned int USARTSendLen			;		//需要往串口发送的数据个数---//当检测到SendLen不为0时自动发送数据
	
	unsigned int FLASHNumToSave		;		//需要往FLASH里保存的数据个数---当此不为0时SPI-FLASH自动存储数据
	unsigned int FLASHNumToRead		;		//需要从FLASH里读取的数据个数---当此不为0时，SPI-FLASH自动通过WriteAddr地址从SPI-FLASH读取数据
	
	unsigned int ReadAddr					;				//读数据起始地址
	unsigned int ReadLen					;				//需要读取的长度
	
	unsigned int WriteAddr				;				//要写入Flash的数据起始地址--ISP接口
	unsigned int WriteLen					;				//要写入Flash的数据长度（字节数)--ISP接口
	

	unsigned int 	SumHaveReceived		;		//作为从机时总共接收到的数据个数
	unsigned int 	SumHaveWritten		;		//主机总共往从机写入的数据个数
		
	unsigned char Command[2]	;		//自举程序命令及异或校验码
	
	unsigned char ISP_RxBuffer[ISP_BufferSize]	;		//接收缓冲区
	unsigned char ISP_RvBuffer[ISP_BufferSize]	;		//接收缓冲区--备份区
	unsigned char ISP_TxBuffer[ISP_BufferSize]	;		//发送缓冲区
	unsigned char ISP_TvBuffer[ISP_BufferSize]	;		//发送缓冲区--备份区

}ISP_DATA_TypeDef;

typedef	struct _ISP_Conf					//USART 在线编程配置文件
{
	USART_TypeDef* 	USARTx															;		//USART1,USART2,USART3,UART4;//UART5
	//RESET控制脚
	GPIO_TypeDef* 	RESET_CTL_PORT;				//GPIOX
	unsigned short 	RESET_CTL_Pin;				//GPIO_Pin_x
	//BOOT0控制脚
	GPIO_TypeDef* 	BOOT0_CTL_PORT;				//GPIOX
	unsigned short 	BOOT0_CTL_Pin;				//GPIO_Pin_x
	
	ISP_SLAVE_STATUS_TypeDef	ISP_SLAVE_STATUS					;		//ISP状态标志---工作在从机状态时
	ISP_SLAVE_STATUS_TypeDef	SlaveLastStatus					;			//存储从机上一个状态，根据上一个状态进行下一步操作
	
	ISP_MASTER_STATUS_TypeDef	ISP_MASTER_STATUS					;		//ISP状态标志---工作在主机状态时
	ISP_MASTER_STATUS_TypeDef	MasterLastStatus					;		//存储主机上一个状态，根据上一个状态等到从机应答时进行下一步操作
	
	ISP_VERSION_TypeDef				SlaveVersion							;		//从机自举程序版本信息
	ISP_DATA_TypeDef					ISP_DATA									;		//ISP数据存储区域---从机
	ISP_DATA_TypeDef					ISP_MDATA									;		//ISP数据存储区域---主机
	ISP_FUN_TypeDef						ISP_FUN										;		//ISP模块工作状态（主机-烧录/从机-更新自身程序)
	
	unsigned int 	OverRunTime														;		//超时时间
	unsigned int 	TimeCount															;		//计时时间
	unsigned char Connected															;		//连接状态--从机更新固件时使用，如果Connected==0，表示未连接，需要格式化，如果Connected==1，表示已经连接	

}ISP_Conf_TypeDef;


void Usart_ISP_Cof(ISP_Conf_TypeDef *ISP_Conf);
void Usart_ISP_ConfigurationNR(ISP_Conf_TypeDef *ISP_Conf,u32 USART_BaudRate,u32 *RXDBuffer,u32 BufferSize);	//
void Usart_ISP_Process(ISP_Conf_TypeDef *ISP_Conf);

void Usart_ISP_SlaveProcess(ISP_Conf_TypeDef *ISP_Conf);		//模块作为从机时的处理程序
void Usart_ISP_MasterProcess(ISP_Conf_TypeDef *ISP_Conf);		//模块作为主机时的处理程序
void Usart_ISP_CheckFun(ISP_Conf_TypeDef *ISP_Conf);				//检测ISP模块工作模式---空闲时检测

void Usart_ISP_ACK(ISP_Conf_TypeDef *ISP_Conf);	//ISP应答
void Usart_ISP_NACK(ISP_Conf_TypeDef *ISP_Conf);		//ISP不应答
void Usart_ISP_CommandSend(ISP_Conf_TypeDef *ISP_Conf,unsigned char Command);	//串口编程发送命令程序
void Usart_ISP_CommandRead(ISP_Conf_TypeDef *ISP_Conf);			//串口接收命令（主机->从机)
void Usart_ISP_GetAddr(ISP_Conf_TypeDef *ISP_Conf);					//ISP获取写数据起始地址(主机->从机)
void Usart_ISP_SetAddr(ISP_Conf_TypeDef *ISP_Conf);					//ISP设置写数据起始地址(主机->从机)
void Usart_ISP_GetLengh(ISP_Conf_TypeDef *ISP_Conf);				//ISP获取需要读取的数据长度(主机->从机)，读数据时，传入地址后再传入待读取的数据长度
void Usart_ISP_SetLengh(ISP_Conf_TypeDef *ISP_Conf);				//ISP获取需要读取的数据长度(主机->从机)
void Usart_ISP_ReadMemory(ISP_Conf_TypeDef *ISP_Conf);			//ISP读数据（从机->主机）
void Usart_ISP_WriteMemory(ISP_Conf_TypeDef *ISP_Conf);			//ISP写数据（主机->从机）写数据时，传完地址后等待传入数据，再执行写操作
void Usart_ISP_SendBuffer(ISP_Conf_TypeDef *ISP_Conf);			//ISP上传数据（从机->主机)
void Usart_ISP_Erase(ISP_Conf_TypeDef *ISP_Conf);						//ISP擦除操作，接收到擦除命令后，应答，等待输入待擦除地址（全部擦除或者页擦除）等待擦除完成，完成后应答
u8 Usart_ISP_GetSlaveStatus(ISP_Conf_TypeDef *ISP_Conf);	//返回从机状态值
void Usart_ISP_SetSlaveStatus(ISP_Conf_TypeDef *ISP_Conf,ISP_SLAVE_STATUS_TypeDef Status);	//设置从机状态
//------------------------------------从机专有函数

//------------------------------------主机专有函数
void Usart_MISP_GetDevice(ISP_Conf_TypeDef *ISP_Conf);				//获取从机芯片信息
void Usart_MISP_Connect(ISP_Conf_TypeDef *ISP_Conf);					//主机连接从机函数
void Usart_MISP_ResetDevice(ISP_Conf_TypeDef *ISP_Conf);			//复位从机
void Usart_MISP_SetDevice(ISP_Conf_TypeDef *ISP_Conf);				//启动从机接收BOOT0使从机进行下载状态
void Usart_MISP_RunDevice(ISP_Conf_TypeDef *ISP_Conf);				//启动从机设备使从机正常运行
bool Usart_MISP_ReadAck(ISP_Conf_TypeDef *ISP_Conf);					//主机读取从机应答:有应答返回TRUE，否则返回FALSE
void Usart_MISP_StatusProcess(ISP_Conf_TypeDef *ISP_Conf);			//启动从机设备使从机运行


//------------------------------------公共函数
void Usart_ISP_Send(ISP_Conf_TypeDef *ISP_Conf);						//发送函数包括从机发送应答、发送地址、发送命令、发送数据
void Usart_ISP_Reset(ISP_Conf_TypeDef *ISP_Conf);						//重置编程器---恢复所有参数为默认值


#endif

