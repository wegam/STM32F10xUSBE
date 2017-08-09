#ifdef MS0800_PM001V20

#include "MS0800_PM001V20.H"

/*##############################################################################
################################################################################
# 项目名		:	MS0800_PC004V10	
# 功能描述	:	智能药架单元板
-------------------------------------------------------------------------------	
********************************功能说明***************************************
-------------------------------------------------------------------------------
● 与PCB和单元控制板连接
● RS232通讯说明：			FA F5 01 01 BCC8 00 06 01 01 01 00 01 25/FA F5 01 01 27 00 06 01 01 01 00 01 25
	波特率：	9600
	串口号：	USART1
	数据长度：13Byte
	头码：		0XFA,0XF5
	
● 485通讯说明：485与单元控制板通讯
	波特率：	9600
	串口号：	USART2
	控制脚：	PA1
	头码：		0XFA,0XFB
	私有地址：拔码开关地址
	
● 485通讯说明：485与单元控制板通讯（备用）
	波特率：	9600
	串口号：	USART3
	控制脚：	PB1
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
			长度：						1	(DLC=1;)
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
			长度：				3	(DLC=3;)
			数据显示标识：	0X08;(Data[0]=0X08;)
			显示数据内容：最大显示999
									高位8位：Data[1]
									低位8位：Data[2]
									
1.4）	数码管板应答：数据完板接收到正确的数据后的回执
			CAN—ID：			SW_ID（拔码开关地址）
			长度：				1	(DLC=1;)
			层时间同步：	0XA0;(Data[0]=0XA0;)
--------------------------------485协议----------------------------------------
2.1）	485协议：			与网关板通讯
			数据长度：	8Byte
			格式：			HEAD(2)，命令，柜号，单元号，数值高位，数值低位，异或校验
			说明：
			HEAD:				0XFA,0XFB
			命令：			0X00--关闭显示，0X01--显示数值，0x02--获取连接设备地址（柜号和单元号）,0x03--显示单元ID，
			柜号：			单元柜号，1~0XFF
			单元号：		数码管ID号，10~77
			数值高位：	与低位连接
			数值低位：	与高位连接，连接后取值范围：0~999，数码管只有三位
			异或校验：	Byte0~Byte6校验
-------------------------------------------------------------------------------


********************************************************************************

################################################################################
###############################################################################*/

#define RS485TX	0
#define RS485RX	1
#define PM001V20_RS4851_CTL(n)	if(n==RS485RX){GPIO_ResetBits(GPIOA,GPIO_Pin_1);}else{GPIO_SetBits(GPIOA,GPIO_Pin_1);}
#define PM001V20_RS4852_CTL(n)	if(n==RS485RX){GPIO_ResetBits(GPIOB,GPIO_Pin_1);}else{GPIO_SetBits(GPIOB,GPIO_Pin_1);}
	

#define	PM001V20_BufferSize 32		//DMA1缓冲大小
u8 PM001V20_RXBuffer[PM001V20_BufferSize]={0};
u8 PM001V20_TXBuffer[PM001V20_BufferSize]={0};

u16 SYS_Time=0;
u8 Buzzer_time=0;
//u16 ci_test=0;
//u8 	RxData[8]={0};
//u16 txTemp=0;
//CanRxMsg RxMessage;

//u8 TX_FLag=0;
//u16 TX_Time=0;

//u8 ID_ARR[8][8]={0,0};
//u8 ON_line[8][8]={0,0};

//u8	TX_ID=0;
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void MS0800_PM001V20_Configuration(void)
{
	CAN_Configuration();	//CAN配置
	
	CAN_FilterInitConfiguration(0,0X000,0X000);		//CAN滤波器配置
	
	USART_DMA_Configuration(USART1,9600,1,1,(u32*)PM001V20_RXBuffer,(u32*)PM001V20_TXBuffer,PM001V20_BufferSize);	//USART1_DMA配置 RS232
		
	USART_DMA_Configuration(USART2,9600,1,1,(u32*)PM001V20_TXBuffer,(u32*)PM001V20_RXBuffer,PM001V20_BufferSize);	//USART2_DMA配置 RS485

//	USART_DMA_Configuration(USART3,9600,1,1,(u32*)PM001V20_Buffer,(u32*)PM001V20_Buffer,PM001V20_BufferSize);	//USART2_DMA配置 RS485	
	
	GPIO_Configuration(GPIOA,GPIO_Pin_1,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);			//USART2_CTL
	GPIO_Configuration(GPIOB,GPIO_Pin_1,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);			//USART3_CTL
	
	GPIO_Configuration(GPIOC,GPIO_Pin_1,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);			//GPIO配置--BUZZER
	GPIO_Configuration(GPIOC,GPIO_Pin_0,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);			//GPIO配置--WORK_LED
	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
	
	PM001V20_RS4851_CTL(RS485TX);
	PM001V20_RS4851_CTL(RS485RX);

}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void MS0800_PM001V20_Server(void)
{
	u8 status=0;
	SYS_Time++;	
	if(SYS_Time>=30000)
		SYS_Time=0;
	
	status=MS0800_PM001V20_TR();
	if(status)
		SYS_Time=0;
	
	if(SYS_Time==20000)
	{
		Time_Sync();
	}

	PM001V20_BUZZER(SYS_Time);					//BUZZER
	PM001V20_WORK_LED(SYS_Time);				//WORK_LED	
	MS0800_PM001V20_PROCESS(SYS_Time);
	//***********计时转换485控制引脚方式
	/***********************
	if(TX_FLag==1)
	{
		TX_Time--;
		if(TX_Time==0)
		{
			TX_FLag=0;
//			TX_Time=0;
			PM001V20_RS4851_CTL(RS485RX);
		}
	}
	***********************/
	//***********串口状态检测控制485脚为接收方式
//	if(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET)
//		PM001V20_RS4851_CTL(RS485RX);

	
//	if(USART_RX_FlagClear(USART2))							//清除串口DMA发送全局中断标志
//	{
//		PM001V20_RS4851_CTL(RS485TX);
//		USART_DMAPrintf(USART1,"RS485收发测试\n");		//自定义printf串口DMA发送程序
//	}
//	else if(USART_TX_DMAFlagClear(USART1))
//	{	
//		memset(PM001V20_Buffer,0,PM001V20_BufferSize);			//初始化缓冲		
//		PM001V20_RS4851_CTL(RS485RX);
////		GPIO_ResetBits(GPIOA,GPIO_Pin_1);
////		USART_DMAPrintf(USART1,"%d",18);		//自定义printf串口DMA发送程序
//	}
	
	

}
/*******************************************************************************
* 函数名		:
* 功能描述	: 
* 输入			: 无
* 输出			: 无
* 返回			: 无
*******************************************************************************/
void MS0800_PM001V20_PROCESS(u16 time)
{
}
/*******************************************************************************
* 函数名		:
* 功能描述	: 
* 输入			: 无
* 输出			: 无
* 返回			: 无
*******************************************************************************/
u8 MS0800_PM001V20_TR(void)				//通讯接口
{
//***********串口状态检测控制485脚为接收方式
	u16 num1=0,num2=0;
	
	num1=USART_RX_FlagClear(USART1);							//清除串口DMA发送全局中断标志
	num2=USART_RX_FlagClear(USART2);							//清除串口DMA发送全局中断标志
	//1)**********根据串口2接收到数据
	if(num1)			//串口1接收到上位机数据通过串口2转发到单元板
	{	
		PM001V20_RS4851_CTL(RS485TX);
		PM001V20_delay(1000);
		USART_DMASend(USART2,(u32*)PM001V20_RXBuffer,(u32)num1);
		return 1;
	}
	else if(num2)		//串口2 接收到数据通过串口1转发到上位机
	{
		USART_DMASend(USART1,(u32*)PM001V20_TXBuffer,(u32)num2);
		return 1;
	}
	//2)**********DMA发送完成后清规BUFFER
	else if(USART_TX_DMAFlagClear(USART1))
	{
//		PM001V20_delay(8000);
		memset(PM001V20_TXBuffer,0,PM001V20_BufferSize);			//初始化缓冲
		return 1;
	}
	else if(USART_TX_DMAFlagClear(USART2))
	{
//		PM001V20_delay(8000);
		memset(PM001V20_RXBuffer,0,PM001V20_BufferSize);			//初始化缓冲
		return 1;
	}
	//3)**********数据发送完成后将485控制脚拉低转为接收模式并且清除标志和数据
	else if(USART_GetFlagStatus(USART2,USART_FLAG_TC)==SET)
	{
		PM001V20_RS4851_CTL(RS485RX);
		USART_ClearFlag(USART2,USART_FLAG_TC);
//		memset(PM001V20_RXBuffer,0,PM001V20_BufferSize);			//初始化缓冲
//		memset(PM001V20_RXBuffer,0,PM001V20_BufferSize);			//初始化缓冲
		return 0;
	}
	//4)**********USART1回执给电脑后清除标志及清空数据
	else if(USART_GetFlagStatus(USART1,USART_FLAG_TC)==SET)
	{
		USART_ClearFlag(USART1,USART_FLAG_TC);
//		memset(PM001V20_TXBuffer,0,PM001V20_BufferSize);			//初始化缓冲
//		memset(PM001V20_RXBuffer,0,PM001V20_BufferSize);			//初始化缓冲
		return 0;
	}
	else
	{
		return 0;
	}
/****************计时转换485控制引脚方式	
	u16 num1=0,num2=0;
	
	num1=USART_RX_FlagClear(USART1);							//清除串口DMA发送全局中断标志
	num2=USART_RX_FlagClear(USART2);							//清除串口DMA发送全局中断标志
	//1)**********根据串口2接收到数据
	if(num1)
	{	
		PM001V20_RS4851_CTL(RS485TX);
		PM001V20_delay(8000);
		TX_FLag=1;
		TX_Time=num1+5;
		USART_DMASend(USART2,(u32*)PM001V20_Buffer,(u32)num1);
		return 1;
	}
	else if(num2)
	{
		PM001V20_RS4851_CTL(RS485TX);
		PM001V20_delay(8000);
		TX_FLag=1;
		TX_Time=num2+5;
		USART_DMASend(USART1,(u32*)PM001V20_Buffer,(u32)num2);
//		USART_DMAPrintf(USART1,"RS485收发测试\n");		//自定义printf串口DMA发送程序
		return 1;
	}
	//2)**********根据串口2DMA发送完成中断
	else if(USART_TX_DMAFlagClear(USART1))
	{
		PM001V20_delay(5000);
//		PM001V20_RS4851_CTL(RS485RX);
		memset(PM001V20_Buffer,0,PM001V20_BufferSize);			//初始化缓冲
//		PM001V20_RS4851_CTL(RS485RX);
		return 1;
	}
	else if(USART_TX_DMAFlagClear(USART2))
	{
		PM001V20_delay(5000);
//		PM001V20_RS4851_CTL(RS485RX);
		memset(PM001V20_Buffer,0,PM001V20_BufferSize);			//初始化缓冲
//		PM001V20_RS4851_CTL(RS485RX);
		return 1;
	}
	else
	{
		return 0;
	}
*************************************/
}

/*******************************************************************************
* 函数名		:
* 功能描述	: 
* 输入			: 无
* 输出			: 无
* 返回			: 无
*******************************************************************************/
void PM001V20_WORK_LED(u16 time)				//WORK_LED
{
	if(time%1000<500)
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_0);				//WORK_LED		
	}
	else
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_0);			//WORK_LED
	}
}
/*******************************************************************************
* 函数名		:
* 功能描述	: 
* 输入			: 无
* 输出			: 无
* 返回			: 无
*******************************************************************************/
void PM001V20_BUZZER(u16 time)				//WORK_LED
{
	if(Buzzer_time<6)
	{
		if(time%1000<500)
		{
			GPIO_SetBits(GPIOC,GPIO_Pin_1);				//Buzzer_ON		
		}
		else
		{
			GPIO_ResetBits(GPIOC,GPIO_Pin_1);			//Buzzer_OFF
		}
		if(time%500==0)
		{
			Buzzer_time++;
		}
	}
	else
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_1);			//Buzzer_OFF
	}
}
/*******************************************************************************
* 函数名		:
* 功能描述	: 
* 输入			: 无
* 输出			: 无
* 返回			: 无
*******************************************************************************/
void PM001V20_delay(u16 time)
{
	while(time--);
}
/*******************************************************************************
* 函数名		:
* 功能描述	: 
* 输入			: 无
* 输出			: 无
* 返回			: 无
*******************************************************************************/
void Time_Sync(void)
{
	PM001V20_RS4851_CTL(RS485TX);
	PM001V20_delay(500);
	//设置数据
	PM001V20_RXBuffer[0]=0xFA;
	PM001V20_RXBuffer[1]=0xF5;
	PM001V20_RXBuffer[2]=~PM001V20_RXBuffer[0];
	PM001V20_RXBuffer[3]=~PM001V20_RXBuffer[1];
	PM001V20_RXBuffer[5]=0x06;	
	USART_DMASend(USART2,(u32*)PM001V20_RXBuffer,13);
}


#endif
