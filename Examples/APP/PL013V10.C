#ifdef PL013V10

#include "PL013V10.H"
#include "CD4511.H"


#include "STM32F10x_BitBand.H"
#include "STM32_GPIO.H"
#include "STM32_SYS.H"
#include "STM32_SYSTICK.H"
#include "STM32_WDG.H"
#include "STM32_PWM.H"


#include "STM32_CAN.H"
#include "STM32_USART.H"
#include "STM32_TOOLS.H"		//数据校验工具


#include "string.h"				//串和内存操作函数头文件

/*##############################################################################
################################################################################
# 项目名		:	MS0800_PC004V10	
# 功能描述	:	智能药架单元板
-------------------------------------------------------------------------------	
********************************功能说明***************************************
-------------------------------------------------------------------------------
● 与网关板通讯，传达指令到数码管显示板，
● 显示地址：柜机号
● 拔码方式：
● CAN通讯说明：
	波特率：	100K
	公有地址：0X000
	
● 485通讯说明：485与网关板通讯
	波特率：	9600
	串口号：	USART2
	控制脚：	PA1
	头码：		0XFA,0XFB
	私有地址：拔码开关地址
-------------------------------------------------------------------------------
********************************软件流程***************************************
-------------------------------------------------------------------------------
1）配置项目
2）上电后主动获取单元地址
3）	
4）	
5）	
-------------------------------------------------------------------------------
*****************************指令/协议说明*************************************
-------------------------------------------------------------------------------

--------------------------------CAN协议----------------------------------------
1.1）	CAN协议：				与数码管板通讯
			CAN—ID：					0X3FF(StdId=0X3FF;)
			长度：						3	(DLC=1;)
			层时间同步：			0XAA;(Data[0]=0XAA;)
			获取/显示ID指令：	0X01;(Data[0]=0X01;)
			自检指令：				0X02;(Data[0]=0X02;)
			熄屏指令：				0X03;(Data[0]=0X02;)
-------------------------------------------------------------------------------		
1.2）	数码管板上报地址：设定完数码管板ID后往单元板上他地址
			CAN—ID：			SW_ID（拔码开关地址）
			长度：				1	(DLC=1;)
			层时间同步：	0XAA;(Data[0]=0XAA;)
		
1.3）	数码管板接收数据：接收上级单元板数据，显示或者关闭
			CAN—ID：			SW_ID（拔码开关地址）
			长度：					3	(DLC=3;)
			数据显示标识：	0X08;(Data[0]=0X08;)
			显示数据内容：	最大显示999
										高位8位：Data[1]
										低位8位：Data[2]
									
1.4）	数码管板应答：数据完板接收到正确的数据后的回执
			CAN—ID：			SW_ID（拔码开关地址）
			长度：					1	(DLC=1;)
			层时间同步：		0XA0;(Data[0]=0XA0;)
--------------------------------485协议----------------------------------------
2.1）	485协议：			与网关板通讯
			数据长度：	13Byte
			RS232通讯说明：			FA F5 01 01 BCC8 00 06 01 01 01 00 01 25------->FA F5 01 01 27 00 06 01 01 01 00 01 25
//			格式：			HEAD(2)，命令，柜号，单元号，数值高位，数值低位，异或校验
			说明：
			HEAD:				0XFA,0XFB
			命令：			0X00--关闭显示，0X01--显示数值，0x02--获取连接设备地址（柜号和单元号）,0x03--显示单元ID，
			柜号：			单元柜号，1~0XFF
			单元号：		数码管ID号，10~77
			数值高位：	与低位连接
			数值低位：	与高位连接，连接后取值范围：0~999，数码管只有三位
			异或校验：	Byte0~Byte6校验
			接收数据单元无误，回执位为0X80|命令类型
			接收数据单元地址错误：回执位0XE0|命令类型
-------------------------------------------------------------------------------


********************************************************************************

################################################################################
###############################################################################*/


#define PL013V10_CANTEST

#ifndef	PL013V10_CANTEST
	#define PL013V10_485TEST
#endif

//#define PL013V10_Master

#define RS485TX	0
#define RS485RX	1
#define PC004V10_RS4851_CTL(n)	if(n==RS485RX){GPIO_ResetBits(GPIOA,GPIO_Pin_1);}else{GPIO_SetBits(GPIOA,GPIO_Pin_1);}

//************CAN通讯数据
#define	CAN_ID				PC004V10_Num[8]*10+PC004V10_Num[9]		//单元地址	由两位数合并，十位为层，个位为序号
#define	CAN_Command		PC004V10_Num[5]												//命令类型----上位机用户码
#define	Data_H				PC004V10_Num[11]											//数据高位
#define	Data_L				PC004V10_Num[12]											//数据低位
//************485通讯数据
//#define	

#define RxBufferSize	5

u8 RevBuffer[RxBufferSize]={0};
u8 RxdBuffer[RxBufferSize]={0};
u8 TxdBuffer[RxBufferSize]={0};

u8 ID_ARR[8][8]={0,0};			//存储已连接数码管ID列表
u8 ON_line[8][8]={0,0};			//对应ID列表标志位，0--无此ID，1--有对应ID

u8 SwitchID=0;				//拔码开关地址 //单元板ID----从右到左依次为增高，最右边为最低位

u16	SYSTime=0;							//循环计时变量

u16 Time=0;
u16 data=0;

u8 Self_Dsp=0;							//拔码开关为0时，自检测试显示标识变量

u16 DSPNUM=0;
u8 CtxNUM=0;

CanRxMsg RxMessage;

CD4511_Pindef CD4511_Pin1;		//第一位
CD4511_Pindef CD4511_Pin2;		//第二位
CD4511_Pindef CD4511_Pin3;		//第三位

RS485_TypeDef RS485_2;

/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PL013V10_Configuration(void)
{
	
	SYS_Configuration();					//系统配置---打开系统时钟 STM32_SYS.H
	
	GPIO_DeInitAll();							//将所有的GPIO关闭----V20170605
	
	PL013V10_PinSet();
	
	SysTick_Configuration(1000);	//系统嘀嗒时钟配置72MHz,单位为uS
	
	IWDG_Configuration(1000);			//独立看门狗配置---参数单位ms	
	
	PWM_OUT(TIM2,PWM_OUTChannel1,1,900);						//PWM设定-20161127版本
	
#ifdef	PL013V10_CANTEST
	
	CAN_Configuration_NR(10000);									//CAN配置---标志位查询方式，不开中断
	
//	CAN_Configuration_IT(10000);										//CAN配置---标志位查询方式，不开中断
	
//	CAN_FilterInitConfiguration(0,0X000003FF,0X0000);			//CAN滤波器配置
	CAN_FilterInitConfiguration_StdData(0,0X015,0X015);	//CAN滤波器配置---标准数据帧模式
//	CAN_FilterInitConfiguration(1,0X0000,0X0000);			//CAN滤波器配置
#endif


#ifdef	PL013V10_485TEST
	RS485_DMA_ConfigurationNR	(&RS485_2,19200,(u32*)RxdBuffer,RxBufferSize);	//USART_DMA配置--查询方式，不开中断
#endif	

	SysTick_Configuration(1000);							//系统嘀嗒时钟配置72MHz,单位为uS
}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PL013V10_Server(void)
{
	//循环周期1mS
	u8 status=0;	
	
	IWDG_Feed();								//独立看门狗喂狗	
	Time++;	
	if(Time>=500)
	{
		Time=0;	
		data++;	
		TxdBuffer[0]++;	
	}
	if(data>999)
		data=0;
	if(TxdBuffer[0]>999)
		TxdBuffer[0]=0;

#ifdef	PL013V10_CANTEST
	#ifdef PL013V10_Master				//主机---发送
		PL013V10_DISPALY(Time%3,CtxNUM);
		if(Time==0)
			PL013V10_CAN_TX();									//CAN发送数据，地址，命令类型，数据
	#else													//从机---接收
		PL013V10_CAN_RX();									//主要接收数码管板地址
		PL013V10_DISPALY(Time%3,DSPNUM);
	#endif		
#endif
		
#ifdef	PL013V10_485TEST
	#ifdef PL013V10_Master
		PL013V10_DISPALY(Time%3,TxdBuffer[0]);
		if(Time==0)
				RS485_DMASend(&RS485_2,(u32*)TxdBuffer,RxBufferSize);	//RS485-DMA发送程序
	#else
		RS485_ReadBufferIDLE(&RS485_2,(u32*)RevBuffer,(u32*)RxdBuffer);	//串口空闲模式读串口接收缓冲区，如果有数据，将数据拷贝到RevBuffer,并返回接收到的数据个数，然后重新将接收缓冲区地址指向RxdBuffer
		PL013V10_DISPALY(Time%3,RevBuffer[0]);
	#endif
#endif
	
#ifndef	PL013V10_CANTEST
		#ifndef	PL013V10_485TEST
			PL013V10_DISPALY(Time%3,data);
		#endif
#endif
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void PL013V10_PinSet(void)
{
	//********************RS485_CTL配置
//	GPIO_Configuration_OPP50(GPIOA,GPIO_Pin_1);				//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
//	
//	//********************拔码开关配置
//	GPIO_Configuration_IPU(GPIOC,GPIO_Pin_0);						//将GPIO相应管脚配置为上拉输入模式----V20170605
//	GPIO_Configuration_IPU(GPIOC,GPIO_Pin_1);						//将GPIO相应管脚配置为上拉输入模式----V20170605
//	GPIO_Configuration_IPU(GPIOC,GPIO_Pin_2);						//将GPIO相应管脚配置为上拉输入模式----V20170605
//	GPIO_Configuration_IPU(GPIOC,GPIO_Pin_3);						//将GPIO相应管脚配置为上拉输入模式----V20170605
//	GPIO_Configuration_IPU(GPIOC,GPIO_Pin_4);						//将GPIO相应管脚配置为上拉输入模式----V20170605
//	GPIO_Configuration_IPU(GPIOC,GPIO_Pin_5);						//将GPIO相应管脚配置为上拉输入模式----V20170605
//	GPIO_Configuration_IPU(GPIOC,GPIO_Pin_6);						//将GPIO相应管脚配置为上拉输入模式----V20170605
//	GPIO_Configuration_IPU(GPIOC,GPIO_Pin_7);						//将GPIO相应管脚配置为上拉输入模式----V20170605
	
	//********************数码管
	//位
	GPIO_Configuration_OPP50	(GPIOB,		GPIO_Pin_5);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	GPIO_Configuration_OPP50	(GPIOB,		GPIO_Pin_14);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	GPIO_Configuration_OPP50	(GPIOB,		GPIO_Pin_15);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	
	//Pin1
	CD4511_Pin1.CD4511_A0_PORT=GPIOB;
	CD4511_Pin1.CD4511_A0_Pin=GPIO_Pin_0;
	
	CD4511_Pin1.CD4511_A1_PORT=GPIOB;
	CD4511_Pin1.CD4511_A1_Pin=GPIO_Pin_1;
	
	CD4511_Pin1.CD4511_A2_PORT=GPIOB;
	CD4511_Pin1.CD4511_A2_Pin=GPIO_Pin_2;
	
	CD4511_Pin1.CD4511_A3_PORT=GPIOB;
	CD4511_Pin1.CD4511_A3_Pin=GPIO_Pin_3;
	
	CD4511_Pin1.CD4511_EN_PORT=GPIOB;
	CD4511_Pin1.CD4511_EN_Pin=GPIO_Pin_15;
	
	CD4511_PinConf(&CD4511_Pin1);
	
	//Pin2
	CD4511_Pin2.CD4511_A0_PORT=GPIOB;
	CD4511_Pin2.CD4511_A0_Pin=GPIO_Pin_0;
	
	CD4511_Pin2.CD4511_A1_PORT=GPIOB;
	CD4511_Pin2.CD4511_A1_Pin=GPIO_Pin_1;
	
	CD4511_Pin2.CD4511_A2_PORT=GPIOB;
	CD4511_Pin2.CD4511_A2_Pin=GPIO_Pin_2;
	
	CD4511_Pin2.CD4511_A3_PORT=GPIOB;
	CD4511_Pin2.CD4511_A3_Pin=GPIO_Pin_3;
	
	CD4511_Pin2.CD4511_EN_PORT=GPIOB;
	CD4511_Pin2.CD4511_EN_Pin=GPIO_Pin_14;
	
	CD4511_PinConf(&CD4511_Pin2);
	
	//Pin1
	CD4511_Pin3.CD4511_A0_PORT=GPIOB;
	CD4511_Pin3.CD4511_A0_Pin=GPIO_Pin_0;
	
	CD4511_Pin3.CD4511_A1_PORT=GPIOB;
	CD4511_Pin3.CD4511_A1_Pin=GPIO_Pin_1;
	
	CD4511_Pin3.CD4511_A2_PORT=GPIOB;
	CD4511_Pin3.CD4511_A2_Pin=GPIO_Pin_2;
	
	CD4511_Pin3.CD4511_A3_PORT=GPIOB;
	CD4511_Pin3.CD4511_A3_Pin=GPIO_Pin_3;
	
	CD4511_Pin3.CD4511_EN_PORT=GPIOB;
	CD4511_Pin3.CD4511_EN_Pin=GPIO_Pin_5;
	
	CD4511_PinConf(&CD4511_Pin3);

	RS485_2.USARTx=USART2;
	RS485_2.RS485_CTL_PORT=GPIOA;
	RS485_2.RS485_CTL_Pin=GPIO_Pin_1;
}

/*******************************************************************************
* 函数名		:
* 功能描述	: 
* 输入			: 无
* 输出			: 无
* 返回			: 无
*******************************************************************************/
void PL013V10_CAN_RX(void)									//主要接收数码管板地址，
{
	u8 ID_Temp=0;
	if(SET == CAN_GetITStatus(CAN_IT_FF0))
	{
		CAN_ClearITPendingBit(CAN_IT_FF0);
	}
	else if(SET == CAN_GetITStatus(CAN_IT_FOV0))
	{
		CAN_ClearITPendingBit(CAN_IT_FOV0);
	}
	else if(SET == CAN_GetITStatus(CAN_IT_FMP0))
	{
		CAN_ClearITPendingBit(CAN_IT_FMP0);
	}
//	else if()
//	{
//		CAN_Receive(CAN_FIFO0,&RxMessage);
////		if(RxMessage.DLC==8)
////		{
////			PL013V10_DISPALY(0,123);			
////		}
//	}
	else
	{
		RxMessage.DLC=0;
		RxMessage.Data[0]=0;
		CAN_Receive(CAN_FIFO0,&RxMessage);
		data=RxMessage.Data[0];
//		PL013V10_DISPALY(1,RxMessage.Data[0]);
//		PL013V10_DISPALY(0,2);
		if(RxMessage.DLC==8)
		{
				DSPNUM=RxMessage.Data[0];

//			ID_Temp=RxMessage.StdId;
//			ID_ARR[ID_Temp/10][ID_Temp%10]=ID_Temp;
//			ON_line[ID_Temp/10][ID_Temp%10]=1;
			
		}
		if(RxMessage.DLC==4)
		{
//			PL013V10_DISPALY(0,RxMessage.Data[3]);	
//			PL013V10_DISPALY(0,3);
		}
		else
		{
//			PL013V10_DISPALY(0,0);
		}
		CAN_FIFORelease(CAN_FIFO0);
		CAN_FIFORelease(CAN_FIFO1);
		
	}
}
/*******************************************************************************
* 函数名		:
* 功能描述	: 
* 输入			: 无
* 输出			: 无
* 返回			: 无
*******************************************************************************/
void PL013V10_CAN_TX(void)					//CAN发送数据，地址，命令类型，数据
{
	CanTxMsg TxMessage;
	
	u8	TransmitMailbox = 0;
	u32	i;
	CtxNUM++;
	if(CtxNUM>=255)
		CtxNUM=0;

	TxMessage.StdId=0X015;					//单元地址
//	TxMessage.ExtId=0XFF;
	TxMessage.RTR=CAN_RTR_DATA;			//数据帧
	TxMessage.IDE=CAN_ID_STD;				//使用标准标识符
	TxMessage.DLC=8;
	TxMessage.Data[0]=CtxNUM;			//命令类型
	TxMessage.Data[1]=0x01;				//数据高位
	TxMessage.Data[2]=0x02;				//数据低位
	TxMessage.Data[3]=0x03;
	TxMessage.Data[4]=0x04;
	TxMessage.Data[5]=0x05;
	TxMessage.Data[6]=0x06;
	TxMessage.Data[7]=0x07;
	TransmitMailbox = CAN_Transmit(&TxMessage);
	i = 0;
	// 用于检查消息传输是否正常
	while((CAN_TransmitStatus(TransmitMailbox) != CANTXOK) && (i != 0xFF))
	{
		i++;
	}
	i = 0;
	// 检查返回的挂号的信息数目
	while((CAN_MessagePending(CAN_FIFO0) < 1) && (i != 0xFF))
	{
		i++;
	}

}
/*******************************************************************************
* 函数名		:
* 功能描述	: 
* 输入			: 无
* 输出			: 无
* 返回			: 无
*******************************************************************************/
void PL013V10_CAN_COMMAD(void)					//CAN发送命令函数，地址，命令类型，数据--时间同步
{
	CanTxMsg TxMessage;
	
	u8	TransmitMailbox = 0;
	u32	i;

	TxMessage.StdId=0X015;								//单元地址
//	TxMessage.ExtId=0XFF;
	TxMessage.RTR=CAN_RTR_DATA;						//数据帧
	TxMessage.IDE=CAN_ID_STD;							//使用标准标识符
	TxMessage.DLC=3;
	TxMessage.Data[0]=0x01;	//命令类型
	TxMessage.Data[1]=0x00;						//数据高位
	TxMessage.Data[2]=0x00;						//数据低位
//	TxMessage.Data[3]=0x00;
//	TxMessage.Data[4]=0x00;
//	TxMessage.Data[5]=0x00;
//	TxMessage.Data[6]=0x00;
//	TxMessage.Data[7]=0x00;
	TransmitMailbox = CAN_Transmit(&TxMessage);
	i = 0;
	// 用于检查消息传输是否正常
	while((CAN_TransmitStatus(TransmitMailbox) != CANTXOK) && (i != 0xFF))
	{
		i++;
	}
	i = 0;
	// 检查返回的挂号的信息数目
	while((CAN_MessagePending(CAN_FIFO0) < 1) && (i != 0xFF))
	{
		i++;
	}
}

/*******************************************************************************
*函数名		:	function
*功能描述	:	函数功能说明
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
u8 PL013V10_GetSwitchID(void)
{
	u8	SW_ID=0;
	//1)***************获取H拔码开关地址，从右到左为从低到高
	if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4))		//SWH-1
	{
		SW_ID|=1;
	}
	SW_ID<<=1;
	if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5))		//SWH-2
	{
		SW_ID|=1;
	}
	SW_ID<<=1;
	if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6))		//SWH-3
	{
		SW_ID|=1;
	}
	SW_ID<<=1;
	if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7))		//SWH-4
	{
		SW_ID=1;
	}
	
	//2)***************获取L拔码开关地址，从右到左为从低到高
	SW_ID<<=1;
	if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0))		//SWL-1
	{
		SW_ID|=1;
	}
	SW_ID<<=1;
	if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1))		//SWL-2
	{
		SW_ID|=1;
	}
	SW_ID<<=1;
	if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2))		//SWL-3
	{
		SW_ID|=1;
	}
	SW_ID<<=1;
	if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3))		//SWL-4
	{
		SW_ID|=1;
	}
	
	//3)***************返回获取的ID值
	return (SW_ID);

}
/*******************************************************************************
*函数名		:	function
*功能描述	:	函数功能说明
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void PL013V10_DISPALY(u8 wei,u16 num)
{
	CD4511_DisplayOFF(&CD4511_Pin1);			//关闭显示---关NPN三极管
	CD4511_DisplayOFF(&CD4511_Pin2);			//关闭显示---关NPN三极管
	CD4511_DisplayOFF(&CD4511_Pin3);			//关闭显示---关NPN三极管
	if(wei==2)
	{
//		if(num>100)
//		{
			CD4511_WriteData(&CD4511_Pin3,num/100);
//		}
	}
	else if(wei==1)
	{
//		if(num>=10||num>=100)
//		{
//			if(num>=100)
//			{
				num=num%100;
//			}
			CD4511_WriteData(&CD4511_Pin2,num/10);
//		}
	}
	else
	{
		CD4511_WriteData(&CD4511_Pin1,num%10);
	}
}

/*******************************************************************************
* 函数名		:
* 功能描述	: 
* 输入			: 无
* 输出			: 无
* 返回			: 无
*******************************************************************************/
void PL013V10_delay(u16 time)
{
	while(time--);
}



#endif
