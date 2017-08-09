#ifdef MS0800_PL001V20


#include "MS0800_PL001V20.H"


/*##############################################################################
################################################################################
# 项目名		:	MS0800_PL001V20	
# 功能描述	:	智能药架数码管显示板
-------------------------------------------------------------------------------	
********************************功能说明***************************************
-------------------------------------------------------------------------------
● 显示数值0~999
● 显示地址：将6位拔码开关分为3+3，高三位为十位地址位，低三位为个位地址为，地址个数，7*7
						十位为层号，个位为序号
● 拔码方式：SW1~SW3为高三位，SW1为高位，SW4~SW6为低三位，SW6为最低位，
● CAN通讯说明：
	波特率：	100K
	公有地址：0X3FF
	私有地址：拔码开关地址
-------------------------------------------------------------------------------
********************************软件流程***************************************
-------------------------------------------------------------------------------
1）配置项目
2）上电或者复位后，先做数码管显示检测（从000~999显示）
3）读取拔码开关地址，设定私有地址并显示，
4）检测CAN数据
5）空闲DP点显示及定时进行整柜时间同步
-------------------------------------------------------------------------------
********************************指令说明***************************************
-------------------------------------------------------------------------------
1）公共指令：			单元板下发
		CAN—ID：					0X3FF(StdId=0X3FF;)
		长度：						1	(DLC=1;)
		层时间同步：			0XAA;(Data[0]=0XAA;)
		获取ID指令：			0X01;(Data[0]=0X01;)
		自检指令：				0X02;(Data[0]=0X02;)
		熄屏指令：				0X03;(Data[0]=0X02;)
		显示ID指令：			0X05;(Data[0]=0X02;)
-------------------------------------------------------------------------------		
2）数码管板上报地址：设定完数码管板ID后往单元板上他地址
		CAN—ID：		SW_ID（拔码开关地址）
		长度：				1	(DLC=1;)
		层时间同步：	0XAA;(Data[0]=0XAA;)
		
3）数码管板接收数据：接收上级单元板数据，显示或者关闭
		CAN—ID：		SW_ID（拔码开关地址）
		长度：				3	(DLC=3;)
		数据显示标识：	0X05;(Data[0]=0X05;)
		显示数据内容：最大显示999
									高位8位：Data[1]
									低位8位：Data[2]
									
4）数码管板应答：数据完板接收到正确的数据后的回执
		CAN—ID：		SW_ID（拔码开关地址）
		长度：				1	(DLC=1;)
		层时间同步：	0XA0;(Data[0]=0XA0;)
		
********bxCAN主要特点:********

-------------------------------------------------------------------------------


********************************************************************************

################################################################################
###############################################################################*/


//************CAN命令标识符
#define	GET_ID					0x01			//获取ID及上报ID指令---然后待机显示
#define	Self_Check			0x02			//自检指令--从000到999循环显示
#define	DSP_CLS					0x03			//熄屏指令/确认指令
#define	DSP_ID					0x04			//显示ID指令---上报ID，然后ID一直闪烁
#define	DSP_DATA				0x05			//显示数值指令---0~999显示
#define	Time_Updata			0x06			//时间同步/显示同步
#define	Updata_Addr			0xAA			//单元板上报地址标志

u16	SYSTime=0;
u16	DSPLAY_time=0;
u16	DSPLAY_NUM=0;
u8	STATUS_FLAG=0;			//状态标志位
u8 	RxData[8]={0};
u8 	SW_ID=0;
u16 txTemp=0;
CanRxMsg RxMessage;
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入			:	
* 输出			:
* 返回 			:
*******************************************************************************/
void MS0800_PL001V20_Configuration(void)
{
//	CAN_Configuration();	//CAN配置
	
//	CD4511BM_Configuration();
	
	//********************拔码开关配置
//	GPIO_Configuration(GPIOB,GPIO_Pin_8,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//GPIO配置
//	GPIO_Configuration(GPIOB,GPIO_Pin_9,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//GPIO配置
//	GPIO_Configuration(GPIOB,GPIO_Pin_10,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//GPIO配置
//	GPIO_Configuration(GPIOB,GPIO_Pin_11,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//GPIO配置
//	GPIO_Configuration(GPIOB,GPIO_Pin_12,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//GPIO配置
//	GPIO_Configuration(GPIOB,GPIO_Pin_13,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//GPIO配置
	
	GPIO_Configuration(GPIOA,GPIO_Pin_8,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//左键
	GPIO_Configuration(GPIOB,GPIO_Pin_11,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//右
	
	GPIO_Configuration(GPIOA,GPIO_Pin_15,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);			//GPIO配置

	GPIO_SetBits(GPIOA,GPIO_Pin_15);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_15);
	//********************系统运行指示灯配置
	PWM_Configuration(TIM2,7200,10000,50);

}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void MS0800_PL001V20_Server(void)
{
	SYSTime++;

	if(SYSTime>=10000)
		SYSTime=0;
	
	if(SYSTime%500==0)
	{
		if((SW_ID!=Get_SW_ID())&&(STATUS_FLAG!=0))
		{
			SW_ID=Get_SW_ID();
			STATUS_FLAG=1;					//返回拔码模式
			DSPLAY_time=0;
		}
		STATUS_FLAG=4;
	}
	
	MS0800_PL001V20_PROCESS(SYSTime);

}
/*******************************************************************************
*函数名		:	function
*功能描述	:	函数功能说明
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void MS0800_PL001V20_PROCESS(u16 time)
{
	//1)***********************开机/上电自检状态
	if(STATUS_FLAG==0)						//上电数码管显示自检
	{
		CD4511BM_DISPLAY(0,0);							//清除显示---消影	
		CD4511BM_DISPLAY(DSPLAY_NUM,time%3+31);
		if(time%200==0)
		{
			DSPLAY_NUM+=111;
		}
		if(DSPLAY_NUM>999)		//启动显示自检
		{			
			DSPLAY_NUM=0;
			STATUS_FLAG=1;				//显示自检完成
			SW_ID=Get_SW_ID();
		}
	}
	//2)***********************显示当前拔码开关状态（ID），高3位（SW1~SW3）为十位数，低三位（SW~SW6)为个位数
	else if(STATUS_FLAG==1)			//显示当前拔码开关状态（ID），高3位（SW1~SW3）为十位数，低三位（SW~SW6)为个位数
	{		
		if(time%1000==0)
		{
			DSPLAY_time++;
		}
		if(DSPLAY_time<10)			//10--10秒钟无新值，进入下一环节
		{
			if(time%1000<500)
			{
				CD4511BM_DISPLAY(0,0);							//清除显示---消影		
				CD4511BM_DISPLAY(SW_ID,time%3+21);				
			}
			else
			{
				CD4511BM_DISPLAY(0,0);							//清除显示
				
			}
		}
		else
		{
			STATUS_FLAG=2;				//显示自检完成
			DSPLAY_time=0;
		}
	}
	//3)***********************根据ID地址注册滤波器，为组0
	else if(STATUS_FLAG==2)								//上报地址及根据地址初始化滤波器
	{		
		PL001V20_CAN_TX();									//上报地址
		CAN_FilterInitConfiguration(0,SW_ID,SW_ID);		//注册地址，接收数据唯一地址
		CAN_FilterInitConfiguration(3,0X3FF,0X3FF);		//CAN滤波器配置-----重置指令ID----所有终端通用
		STATUS_FLAG=5;						//地址上报完成
	}
	//4)***********************接收数据判断
	else if(STATUS_FLAG==3)
	{	

	}
	//5)***********************显示接收到的数据
	else if(STATUS_FLAG==4)
	{
		CD4511BM_DISPLAY(0,0);							//清除显示---消影	
		CD4511BM_DISPLAY(txTemp,time%3+1);
	}
	//6)***********************熄屏/待机显示---小数点闪烁
	else if(STATUS_FLAG==5)	
	{	
		if(time%500==0)
		{
			
			CD4511BM_DISPLAY_DP();	
		}
	}
	//6)***********************显示ID--闪烁
	else if(STATUS_FLAG==6)
	{		
		if(time%1000<500)
		{
			CD4511BM_DISPLAY(0,0);							//清除显示---消影		
			CD4511BM_DISPLAY(SW_ID,time%3+21);				
		}
		else
		{
			CD4511BM_DISPLAY(SW_ID,0);			
		}				
	}
	//6)***********************自检---数码管从000循环显示到999
	else if(STATUS_FLAG==7)			//数码管最后一位小数点闪烁
	{
		CD4511BM_DISPLAY(0,0);							//清除显示---消影		
		CD4511BM_DISPLAY(DSPLAY_NUM,time%3+31);
		if(time%200==0)
		{
			DSPLAY_NUM+=111;
		}
		if(DSPLAY_NUM>999)		//启动显示自检
		{			
			DSPLAY_NUM=0;
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
void PL001V20_CAN_TX(void)
{
		CanTxMsg TxMessage;
		u8	TransmitMailbox = 0;
		u32	i;			
		TxMessage.StdId=SW_ID;			//设备ID
		//	TxMessage.ExtId=0XFF;
		TxMessage.RTR=CAN_RTR_DATA;
		TxMessage.IDE=CAN_ID_STD;
		TxMessage.DLC=1;
		TxMessage.Data[0]=Updata_Addr;			//数据类型
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
void PL001V20_CAN_ACK(void)
{
		CanTxMsg TxMessage;
		u8	TransmitMailbox = 0;
		u32	i;			
		TxMessage.StdId=Get_SW_ID();			//设备ID
		//	TxMessage.ExtId=0XFF;
		TxMessage.RTR=CAN_RTR_DATA;
		TxMessage.IDE=CAN_ID_STD;
		TxMessage.DLC=1;
		TxMessage.Data[0]=Updata_Addr;			//数据类型
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
void PL001V20_CAN_RX(void)
{
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
//----------------------------------------------------------------------------------------		
		if(((RxMessage.StdId==SW_ID)||(RxMessage.StdId==0X3FF))&&RxMessage.DLC==3)
		{	

			CD4511BM_DISPLAY(SW_ID,0);
			//1)************显示数据指令0x05
			if(RxMessage.Data[0]==DSP_DATA)
			{
//				txTemp=(RxMessage.Data[1]*100+RxMessage.Data[2]);
				txTemp=0;
				txTemp=RxMessage.Data[1];
				txTemp<<=8;
				txTemp+=RxMessage.Data[2];
				STATUS_FLAG=4;
			}
			//2)************获取ID指令0x01
			else if(RxMessage.Data[0]==GET_ID)				//获取ID指令
			{
				STATUS_FLAG=1;
			}
			//4.1)************自检指令0x02---数码管从000循环显示到999
			else if(RxMessage.Data[0]==Self_Check)		//自检指令
			{
				STATUS_FLAG=7;
			}
			//4.1)************熄屏指令--小数点闪烁0x03
			else if(RxMessage.Data[0]==DSP_CLS)				//熄屏指令
			{
				STATUS_FLAG=5;
			}
			//4.1)************显示ID指令0x04
			else if(RxMessage.Data[0]==DSP_ID)							//显示ID指令--更新ID
			{
				SW_ID=Get_SW_ID();
				PL001V20_CAN_TX();														//上报地址
				CAN_FilterInitConfiguration(0,SW_ID,SW_ID);		//注册地址，接收数据唯一地址
				STATUS_FLAG=6;
			}
			//4.1)************层时间同步0x06----根据ID高位设定初始时间达到行显示同步
			else if(RxMessage.Data[0]==Time_Updata)			//层时间同步
			{
				SYSTime=(SW_ID/10)*200;					//
			}			
			//4.1)************无相关指令--熄屏
			else
			{
				STATUS_FLAG=5;
			
			}
		}
//----------------------------------------------------------------------------------------
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
u8 Get_SW_ID(void)
{
	u8 temp1=0,temp2=0;
	//1)***************获取十位数ID，SW1,SW2,SW3（前三位做十位）
	if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8))		//SW1最高位，十位最高位
	{
		temp1|=1;
	}
	temp1<<=1;
	if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9))		//SW2
	{
		temp1|=1;
	}
	temp1<<=1;
	if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10))		//SW3十位最低位
	{
		temp1|=1;
	}

	temp1=temp1*10;
	
	//2)***************获取个位数ID，SW4,SW5,SW6（后三位做个位）
	if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11))		//SW4个位最高位
	{
		temp2|=1;
	}
	temp2<<=1;
	if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12))		//SW5
	{
		temp2|=1;
	}
	temp2<<=1;
	if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13))		//SW6个位最低位
	{
		temp2|=1;
	}
	
	//3)***************返回获取的ID值
	return (temp1+temp2);

}






#endif
