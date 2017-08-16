#ifndef __PD014V15_H
#define __PD014V15_H

#include "stm32f10x_gpio.h"

//发药电磁铁控制
#define	ct_pmos1	PA7
#define	ct_pmos2	PC5
#define	ct_pmos3	PB1
#define	ct_pmos4	PB11
#define	ct_pmos5	PB13
#define	ct_pmos6	PB15
#define	ct_pmos7	PC7
#define	ct_pmos8	PC9

//发药传感器供电开关
#define	P_Sens	PA5

//发药传感器
#define	Sens_In1	PA6in
#define	Sens_In2	PC4in
#define	Sens_In3	PB0in
#define	Sens_In4	PB10in
#define	Sens_In5	PB12in
#define	Sens_In6	PB14in
#define	Sens_In7	PC6in
#define	Sens_In8	PC8in

//拨码开关 S1~S3为序号 0x00~0x03，S4~S7为层号，S8为CAN总线启用号
#define	PD014_Switch1	PB7in		//LSB
#define	PD014_Switch2	PB6in		
#define	PD014_Switch3	PB5in
#define	PD014_Switch4	PB4in
#define	PD014_Switch5	PB3in
#define	PD014_Switch6	PC12in
#define	PD014_Switch7	PC11in
#define	PD014_Switch8	PC10in	//MSB



#define RS485BufferSize 32

//数据格式:B0-SWITCHID，B1-CMD，B2~B9:数据，B10:前面所有数据异或校验


typedef	enum _PD014_DefaultData		//PD014命令(状态查询、读发药头、发药、上报发药结果、设置电磁铁吸合时间、重置）
{
	RelayOnTime		=	100,		//继电器吸合时间		
	RelayOutTime	=	1000,	//继电器单次工作时间
	MinSensTime		=	20,		//传感器最小感应时长----增加抗干扰
	TryTimes			=	5			//重试次数
}PD014_DefaultData_TypeDef;

typedef	enum _PD014_COMMAND		//PD014命令(状态查询、读发药头、发药、上报发药结果、设置电磁铁吸合时间、重置）
{
//说明：
//1)发药控制板工作在从机模式，所有的动作都由上层控制，包括命令下发和结果上报都通过命令来执行
//2)协议执行程序，接收到上位机的命令，
//3)
	PD014_CMD_IDLE=0x00,			//空命令--无需要执行的动作
	PD014_CMD_GetOnlieDevice,	//获取在线发药头（可带参数，如果带参数，参数为模式）
	PD014_CMD_GetStatus,			//获取状态命令
	PD014_CMD_SetWSD,					//设置发药数量命令
	PD014_CMD_GetSED,					//获取已发药数量	
	PD014_CMD_SetStatus,			//设置状态	
}PD014_CMD_TypeDef;

typedef	enum _PD014_STATUS		//PD014状态(空闲、忙、正在发药、发药完成、缺药、设置参数、上报数据、上报完成）
{
//说明：
//1)发药控制板工作在从机模式，所有的动作都由上层控制，包括命令下发和结果上报都通过命令来执行
//2)协议执行程序，接收到上位机的命令，
//3)	
	PD014_STA_IDLE=0x00,				//空闲----可以接收命令或者执行动作
	PD014_STA_BUSY,				//忙
	PD014_STA_UPDATA,			//正在上报数据
	PD014_STA_SEND,				//发药
	PD014_STA_SENDING,		//正在发药
	PD014_STA_SENDED,			//发药完成
	PD014_STA_SHORTAGE,		//缺药
	PD014_STA_CLOGGED,		//堵药/卡药
	PD014_STA_CONF,				//配置参数
	PD014_STA_OffLine,		//离线状态
	PD014_STA_CONFING,		//正在配置参数
	PD014_STA_WaitAck			//正在配置参数
}PD014_STATUS_TypeDef;

typedef	enum _PD014_Answer	//USART 应答
{
	PD014_ACK			=0x79,		//ACK 应答(被接受）
	PD014_NACK		=0x1F,		//NACK 应答（不接受或者不识别--丢弃)
	PD014_NoError,					//ACK 应答(被接受）
	PD014_BccError,				//NACK 应答（不接受或者不识别--丢弃)
	PD014_BUSY							//从机忙状态
}PD014_ANSWER_TypeDef;

typedef	enum _PD014_MODE	//USART 应答
{
	NORMEL=0x00,									//ACK 应答(被接受）
	TEST										//NACK 应答（不接受或者不识别--丢弃)
}PD014_MODE_TypeDef;

typedef	struct _PD014_DATA	//USART 应答
{	
	unsigned char 	CANFLG;					//CAN开关---CANFLG==1，使用CAN--为与上层连接的板
	unsigned char 	SWITCHID;				//拨码开关
	unsigned char 	OnlieDevice;		//发药头标志---存储是否接入发药头，有，当前位为1，否则为0	//1(最左边为最高位)
	
	unsigned int 		TimeCount;			//运行次数计时器
	unsigned int 		TimeOut;				//支行超时计时器
	
	unsigned char 	WSD[8];					//待发药数量
	unsigned char 	SED[8];					//已发药数量	
	
	unsigned short 	RELAYTIME[8];		//继电器需要吸合时间---默认100ms
	unsigned short 	RELAYCOUNT[8];	//继电器已经吸合时间---达到RELAYTIME后释放
	unsigned short 	TRYTIME[8];			//未感应到药品时的重试次数	
	
	unsigned short 	SENSTIME[8];		//传感器感应时间----与NOISETIME配置使用计算传感器感应时间，主要过滤干扰信号
	unsigned short 	NOISETIME[8];		//传感器感噪声过滤时间	
	
	
	unsigned char RxdBuffe[RS485BufferSize];
	unsigned char RevBuffe[RS485BufferSize];
	unsigned char TxdBuffe[RS485BufferSize];
	unsigned char TevBuffe[RS485BufferSize];
	
	
	PD014_CMD_TypeDef Command;						//命令
	PD014_STATUS_TypeDef	STATUS[8];			//状态
	PD014_DefaultData_TypeDef	DDATA;			//初始化配置数据
	
}PD014_DATA_TypeDef;


typedef	struct _PD014_INF				//
{
	PD014_MODE_TypeDef MODE;			//工作模式：NORMEL正常工作模式，TEST测试模式
	PD014_DATA_TypeDef PD014_DATA;
	
	GPIO_TypeDef* GPmos[8];
	unsigned short PPmos[8];
	
	GPIO_TypeDef* GSens[8];
	unsigned short Psens[8];
	
}PD014_INF_TypeDef;


void PD014V14_Configuration(void);
void PD014V14_Server(void);
void PD014V14_Process(void);												//PD014V14所有板内处理数理函数
PD014_CMD_TypeDef PD014V14_CommandProcess(void);		//PD014V14命令处理函数

void PD014V14_PinSet(void);
void Lock_Toggle(void);			//双向电子锁控制

void PD014V14_GetSwitchID(void);				//获取拨码开关地址
void PD014V14_GetOnlieDevice(void);			//获取在线发药头
void PD014V14_GetOnlieStatus(void);			//获取状态

void PD014V14_SetWSD(void);							//设置待发药数量

void PD014V14_ACK(void);
void PD014V14_NACK(void);
void PD014V14_SendM(void);							//发药
void PD014V14_Module(unsigned char Num);	//发药头模块---控制，传感器，超时
void PD014V14_ResetData(void);					//复位数据


#endif

