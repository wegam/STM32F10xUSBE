#ifndef __PD014V14_H
#define __PD014V14_H


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



typedef	enum _PD014_COMMAND		//PD014命令(状态查询、读发药头、发药、上报发药结果、设置电磁铁吸合时间、重置）
{
//说明：
//1)发药控制板工作在从机模式，所有的动作都由上层控制，包括命令下发和结果上报都通过命令来执行
//2)协议执行程序，接收到上位机的命令，
//3)
	PD014_CMD_IDLE,				//空命令--无需要执行的动作
	PD014_CMD_GetDevice,	//获取在线发药头（可带参数，如果带参数，参数为模式）
	PD014_CMD_GetSta,			//获取状态
	PD014_CMD_GetVS,			//Get Version & Read Protection Status:获取自举程序版本及 Flash 的读保护状态
	PD014_CMD_GetID,			//Get ID:获取芯片 ID
	PD014_CMD_RM,					//Read Memory:从应用程序指定的地址开始读取最多 256 个字节的存储器空间
	PD014_CMD_Go,					//Go:跳转到内部 Flash 或 SRAM 内的应用程序代码
	PD014_CMD_WM,					//Write Memory:从应用程序指定的地址开始将最多 256 个字节的数据写入 RAM 或 Flash
	PD014_CMD_Erase,			//Erase:擦除一个到全部 Flash 页面
	PD014_CMD_EE,					//Extended Erase:使用双字节寻址模式擦除一个到全部 Flash 页面（仅用于v3.0 usart 自举程序版本及以上版本）。
	PD014_CMD_WP,					//Write Protect:使能某些扇区的写保护
	PD014_CMD_WU,					//Write Unprotect:禁止所有 Flash 扇区的写保护
	PD014_CMD_RP,					//Readout Protect:使能读保护
	PD014_CMD_RU					//Readout Unprotect:禁止读保护
}PD014_CMD_TypeDef;

typedef	enum _PD014_STATUS		//PD014状态(空闲、忙、正在发药、发药完成、缺药、设置参数、上报数据、上报完成）
{
//说明：
//1)发药控制板工作在从机模式，所有的动作都由上层控制，包括命令下发和结果上报都通过命令来执行
//2)协议执行程序，接收到上位机的命令，
//3)	
	PD014_STA_IDLE,				//空闲----可以接收命令或者执行动作
	PD014_STA_BUSY,				//忙
	PD014_STA_UPDATA,			//正在上报数据
	PD014_STA_SEND,				//发药
	PD014_STA_SENDING,		//正在发药
	PD014_STA_SENDED,			//发药完成
	PD014_STA_SHORTAGE,		//缺药
	PD014_STA_CLOGGED,		//堵药/卡药
	PD014_STA_CONF,				//配置参数
	PD014_STA_CONFING			//正在配置参数
}PD014_STA_TypeDef;

typedef	enum _PD014_Answer	//USART 应答
{
	PD014_ACK			=0x79,		//ACK 应答(被接受）
	PD014_NACK		=0x1F			//NACK 应答（不接受或者不识别--丢弃)
}PD014_ANSWER_TypeDef;
typedef	enum _PD014_MODE	//USART 应答
{
	NORMEL,									//ACK 应答(被接受）
	TEST										//NACK 应答（不接受或者不识别--丢弃)
}PD014_MODE_TypeDef;

typedef	struct _PD014_DATA	//USART 应答
{	
	unsigned char 	CANFLG;					//CAN开关---CANFLG==1，使用CAN--为与上层连接的板
	unsigned char 	SWITCHID;				//拨码开关
	unsigned char 	DeviceOnlie;		//发药头标志---存储是否接入发药头，有，当前位为1，否则为0
	
	unsigned char 	WSD[8];					//待发药数量
	unsigned char 	SED[8];					//已发药数量	
	
	unsigned short 	RELAYTIME[8];		//继电器需要吸合时间---默认100ms
	unsigned short 	RELAYCOUNT[8];	//继电器已经吸合时间---达到RELAYTIME后释放
	
	unsigned short 	SENSTIME[8];		//传感器感应时间----与NOISETIME配置使用计算传感器感应时间，主要过滤干扰信号
	unsigned short 	NOISETIME[8];		//传感器感噪声过滤时间
	
	PD014_CMD_TypeDef CMD[8];				//命令
	PD014_STA_TypeDef	STA[8];				//状态
	
}PD014_DATA_TypeDef;

typedef	struct _PD014_INF				//
{
	PD014_MODE_TypeDef MODE;			//工作模式：NORMEL正常工作模式，TEST测试模式
	PD014_DATA_TypeDef PD014_DATA;
}PD014_INF_TypeDef;


void PD014V14_Configuration(void);
void PD014V14_Server(void);
void PD014V14_Process(void);		//PD014V14所有板内处理数理函数



void PD014V14_GetSwitchID(void);				//获取拨码开关地址
void PD014V14_GetOnlieDevice(void);			//获取在线发药头
void PD014V14_ResetData(void);					//复位数据


#endif

