#ifdef MS0800_PC004V10

#include "MS0800_PC004V10.H"

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

#define	PC004V10_BufferSize 32															//DMA1缓冲大小

u8 PC004V10_Buffer[PC004V10_BufferSize]={0};								//RS485缓存
u8 PC004V10_Num[13]={0};																		//接收到的数据

u8 ID_ARR[8][8]={0,0};			//存储已连接数码管ID列表
u8 ON_line[8][8]={0,0};			//对应ID列表标志位，0--无此ID，1--有对应ID

u8 PC004V10_ID=0;						//单元板ID----从右到左依次为增高，最右边为最低位

u16	SYSTime=0;							//循环计时变量
u8 Buzzer_time=0;						//开机蜂鸣器响应次数

u8 Self_Dsp=0;							//拔码开关为0时，自检测试显示标识变量

CanRxMsg RxMessage;

/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void MS0800_PC004V10_Configuration(void)
{
	CAN_Configuration();	//CAN配置
	
	CAN_FilterInitConfiguration(0,0X000,0X000);					//CAN滤波器配置
	
	USART_DMA_Configuration(USART2,9600,1,1,(u32*)PC004V10_Buffer,(u32*)PC004V10_Buffer,PC004V10_BufferSize);	//USART_DMA配置
	GPIO_Configuration(GPIOA,GPIO_Pin_1,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);			//RS485_CTL
	
	GPIO_Configuration(GPIOB,GPIO_Pin_14,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);			//GPIO配置--BUZZER
	GPIO_Configuration(GPIOB,GPIO_Pin_15,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);			//GPIO配置--WORK_LED
	
	//********************拔码开关配置
	GPIO_Configuration(GPIOC,GPIO_Pin_0,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//GPIO配置---拔码开关
	GPIO_Configuration(GPIOC,GPIO_Pin_1,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//GPIO配置
	GPIO_Configuration(GPIOC,GPIO_Pin_2,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//GPIO配置
	GPIO_Configuration(GPIOC,GPIO_Pin_3,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//GPIO配置
	GPIO_Configuration(GPIOC,GPIO_Pin_4,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//GPIO配置
	GPIO_Configuration(GPIOC,GPIO_Pin_5,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//GPIO配置
	GPIO_Configuration(GPIOC,GPIO_Pin_6,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//GPIO配置
	GPIO_Configuration(GPIOC,GPIO_Pin_7,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//GPIO配置	

	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	PC004V10_RS4851_CTL(RS485RX);
	
	PC004V10_Num[8]=1;
	PC004V10_Num[9]=0;
	PC004V10_Num[11]=0;
	PC004V10_Num[12]=0;

}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void MS0800_PC004V10_Server(void)
{
	//循环周期1mS
	u8 status=0;
	SYSTime++;
		
	if(SYSTime>=10000)
		SYSTime=0;
	//**************检查通讯标志位，查看是否为通讯中断，如果是，则此次时间增量无效
	status=PC004V10_485_TR();				//485收发程序//通讯接口
	if(status&&(SYSTime>0))
		SYSTime--;	
	
	PC004V10_BUZZER(SYSTime);				//蜂鸣器
	
	//**************检测设备ID有无变化，有就更新
	if(SYSTime%500==0)
	{
		if(PC004V10_ID!=PC004V10_Get_ID())
		{
			PC004V10_ID=PC004V10_Get_ID();
			PC004V10_Buffer[5]=0x01;							//获取ID指令0x01
			PC004V10_CAN_COMMAD();					//CAN发送命令函数，地址，命令类型，数据--时间同步--发送获取D命令
		}
	}
	if(PC004V10_ID==0)
	{
		Self_Test(SYSTime);
	}
	WORK_LED(SYSTime);				//WORK_LED
}
/*******************************************************************************
* 函数名		:
* 功能描述	: 
* 输入			: 无
* 输出			: 无
* 返回			: 无
*******************************************************************************/
u8 PC004V10_485_TR(void)				//485收发程序//通讯接口
{
	//***********串口状态检测控制485脚为接收方式
	u16 num=0;
//	u8 temp=0;
	num=USART_RX_FlagClear(USART2);						//清除串口DMA发送全局中断标志
	//1)**********对接收到的数据进行规格检测
	if(num)
	{	
		//1.1)**********RS485转发发送模式，防止有数据进来干扰数据
		PC004V10_RS4851_CTL(RS485TX);						//485在发送模式，不接收数据
		PC004V10_delay(2000);										//延时，使得485收发控制引脚电平正确转换
		//1.3)**********命令类信息
		if((PC004V10_Buffer[0]==0xFA)&&(PC004V10_Buffer[1]==0xF5)&&(PC004V10_Buffer[2]==0x05)&&(PC004V10_Buffer[3]==0x0A))
		{
			if(PC004V10_Buffer[5]==0x04)					//更新ID指令
			{
				memset(ID_ARR,0,sizeof(ID_ARR));		//清空地址存储空间
				memset(ON_line,0,sizeof(ON_line));	//清空地址在线标志
			}
			PC004V10_CAN_COMMAD();								//CAN发送命令函数，地址，命令类型，数据--时间同步
			PC004V10_RS4851_CTL(RS485RX);					//485转为接收模式
		}
		//1.2)**********检查数据正确性
		else if((PC004V10_Buffer[0]==0xFA)&&(PC004V10_Buffer[1]==0xF5)&&(PC004V10_Buffer[7]==PC004V10_ID)&&((PC004V10_Buffer[4]==BCC8(&PC004V10_Buffer[7],6))))			//检测头标识、机柜号和异或校验位
		{
//			/********************************************/
//			//***************判断单元地址正确与否
			u8 temp=0;
			temp=PC004V10_Buffer[8]*10+PC004V10_Buffer[9];						//将单元地址的层号和序号合成
			//1.2.1）***************判断单元地址正确与否
			if(memchr(ID_ARR,temp,sizeof(ID_ARR)))										//单元地址正确	//判断单元地址有没存在，在数组x的前n个字节中搜索字符ch。返回值为指向s中首次出现ch的指针位置。如果ch没有在s数组中出现。返回NULL			
			{
				memcpy(PC004V10_Num,PC004V10_Buffer,13);								//复制数据						

				PC004V10_Buffer[3]|=0x80;						//修改回执位
				USART_DMASend(USART2,(u32*)PC004V10_Buffer,(u32)num);		//返回回执给网关板
				
				PC004V10_CAN_TX();					//CAN发送数据，地址，命令类型，数据
			}
			else		//单元地址错误
			{
				PC004V10_Buffer[3]|=0xE0;																//修改回执位
				USART_DMASend(USART2,(u32*)PC004V10_Buffer,(u32)num);		//返回回执给网关板
			}
			/**********************************************/
		}
		
		else			//接收数据无效，恢复485为接收数据状态
		{
			memset(PC004V10_Buffer,0,PC004V10_BufferSize);	//初始化缓冲
			PC004V10_RS4851_CTL(RS485RX);										//485转为接收模式			
		}
		return 1;		
	}
	//2)**********根据串口2DMA发送完成中断
	else if(USART_TX_DMAFlagClear(USART2))
	{
		memset(PC004V10_Buffer,0,PC004V10_BufferSize);			//初始化缓冲		
		return 1;
	}
	//3)**********数据发送完成后将485控制脚拉低转为接收模式
	else	if(USART_GetFlagStatus(USART2,USART_FLAG_TC)==SET)
	{
		PC004V10_RS4851_CTL(RS485RX);
		USART_ClearFlag(USART2,USART_FLAG_TC);
		return 0;
	}
	else
	{
		return 0;
	}
}
/*******************************************************************************
* 函数名		:
* 功能描述	: 
* 输入			: 无
* 输出			: 无
* 返回			: 无
*******************************************************************************/
void PC004V10_CAN_RX(void)									//主要接收数码管板地址，
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
	else
	{
		CAN_Receive(CAN_FIFO0,&RxMessage);
		if(RxMessage.DLC==1&&RxMessage.Data[0]==0XAA)
		{
			ID_Temp=RxMessage.StdId;
			ID_ARR[ID_Temp/10][ID_Temp%10]=ID_Temp;
			ON_line[ID_Temp/10][ID_Temp%10]=1;
		}
	}
}
/*******************************************************************************
* 函数名		:
* 功能描述	: 
* 输入			: 无
* 输出			: 无
* 返回			: 无
*******************************************************************************/
void PC004V10_CAN_TX(void)					//CAN发送数据，地址，命令类型，数据
{
	CanTxMsg TxMessage;
	
	u8	TransmitMailbox = 0;
	u32	i;

	TxMessage.StdId=CAN_ID;					//单元地址
//	TxMessage.ExtId=0XFF;
	TxMessage.RTR=CAN_RTR_DATA;			//数据帧
	TxMessage.IDE=CAN_ID_STD;				//使用标准标识符
	TxMessage.DLC=3;
	TxMessage.Data[0]=CAN_Command;	//命令类型
	TxMessage.Data[1]=Data_H;				//数据高位
	TxMessage.Data[2]=Data_L;				//数据低位
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
* 函数名		:
* 功能描述	: 
* 输入			: 无
* 输出			: 无
* 返回			: 无
*******************************************************************************/
void PC004V10_CAN_COMMAD(void)					//CAN发送命令函数，地址，命令类型，数据--时间同步
{
	CanTxMsg TxMessage;
	
	u8	TransmitMailbox = 0;
	u32	i;

	TxMessage.StdId=0X3FF;								//单元地址
//	TxMessage.ExtId=0XFF;
	TxMessage.RTR=CAN_RTR_DATA;						//数据帧
	TxMessage.IDE=CAN_ID_STD;							//使用标准标识符
	TxMessage.DLC=3;
	TxMessage.Data[0]=PC004V10_Buffer[5];	//命令类型
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
	memset(PC004V10_Buffer,0,PC004V10_BufferSize);	//初始化缓冲
}
/*******************************************************************************
* 函数名		:
* 功能描述	: 
* 输入			: 无
* 输出			: 无
* 返回			: 无
*******************************************************************************/
void WORK_LED(u16 time)				//WORK_LED
{
	if(time%1000<500)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_15);				//WORK_LED		
	}
	else
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_15);			//WORK_LED
	}
}
/*******************************************************************************
* 函数名		:
* 功能描述	: 
* 输入			: 无
* 输出			: 无
* 返回			: 无
*******************************************************************************/
void PC004V10_BUZZER(u16 time)				//WORK_LED
{
	if(Buzzer_time<6)
	{
		if(time%1000<500)
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_14);				//Buzzer_ON		
		}
		else
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_14);			//Buzzer_OFF
		}
		if(time%500==0)
		{
			Buzzer_time++;
		}
	}
	else
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_14);			//Buzzer_OFF
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
u8 PC004V10_Get_ID(void)
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
void Self_Test(u16 time)			//拔码开关为0时，计数显示测试
{
//	//************CAN通讯数据
//#define	CAN_ID				PC004V10_Num[8]*10+PC004V10_Num[9]		//单元地址	由两位数合并，十位为层，个位为序号
//#define	CAN_Command		PC004V10_Num[5]												//命令类型----上位机用户码
//#define	Data_H				PC004V10_Num[11]											//数据高位
//#define	Data_L				PC004V10_Num[12]											//数据低位
//	Self_Dsp=0;							//拔码开关为0时，自检测试显示标识变量
	if((time%10==0)&&(Self_Dsp==0))
	{
		PC004V10_Num[5]=0x05;				//命令类型----上位机用户码
		
//		PC004V10_Num[12]++;
		if(PC004V10_Num[12]++>=99)
		{
			PC004V10_Num[12]=0;
			if(PC004V10_Num[11]++>=9)
			{
				PC004V10_Num[11]=0;
				if(PC004V10_Num[9]++>=3)
				{
					PC004V10_Num[9]=0;
					if(PC004V10_Num[8]++>=5)
					{
						PC004V10_Num[8]=1;
						PC004V10_Num[9]=0;
						PC004V10_Num[11]=0;
						PC004V10_Num[12]=0;
						Self_Dsp=1;							//拔码开关为0时，自检测试显示标识变量，1--显示ID
						SYSTime=0;
					}
				}
			}
		}		
		PC004V10_CAN_TX();					//CAN发送数据，地址，命令类型，数据
	}
	else if(Self_Dsp==1)
	{
		if(time<5)
		{
			PC004V10_Num[5]=0x04;				//命令类型----显示ID
//			PC004V10_CAN_TX();					//CAN发送数据，地址，命令类型，数据
			PC004V10_CAN_COMMAD();			//CAN发送命令函数，地址，命令类型，数据--时间同步
		}
		else if(time>=9999)
		{
			PC004V10_Num[5]=0x05;				//命令类型----上位机用户码
			PC004V10_Num[8]=1;
			PC004V10_Num[9]=0;
			PC004V10_Num[11]=0;
			PC004V10_Num[12]=0;
			PC004V10_CAN_COMMAD();			//CAN发送命令函数，地址，命令类型，数据--时间同步
			Self_Dsp=0;							//拔码开关为0时，自检测试显示标识变量，0--计数
		}
	}
}

/*******************************************************************************
* 函数名		:
* 功能描述	: 
* 输入			: 无
* 输出			: 无
* 返回			: 无
*******************************************************************************/
void PC004V10_delay(u16 time)
{
	while(time--);
}



#endif
