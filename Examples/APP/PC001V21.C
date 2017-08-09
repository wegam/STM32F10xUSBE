/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : PC001V21.c
* Author             : WOW
* Version            : V2.0.1
* Date               : 06/26/2017
* Description        : PC001V21层控制板.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/********************************开锁命令*************************************/
//	命令	SwitchID	锁号
//	锁1：07 ID 01	
//	锁2：07 ID 02
//	锁3：07 ID 03
//	锁4：07 ID 04

/********************************读数命令*************************************/
//	命令	SwitchID
//	05 ID	


#ifdef PC001V21				//托盘药盒主控板

#include "PC001V21.H"



#include "STM32_GPIO.H"
#include "STM32_SYS.H"
#include "STM32_SYSTICK.H"
#include "STM32_WDG.H"
#include "STM32_USART.H"
#include "STM32_PWM.H"
#include "STM32F10x_BitBand.H"

#include "string.h"				//串和内存操作函数头文件
#include "stm32f10x_dma.h"

#ifdef	USART_TO_RS485
	#define BufferSize	1024
#else
	#define BufferSize	34
#endif
#define Command_ReadData	0x05
#define Command_SendData	0x06
#define DR_CTRL	0x07			//PMOS控制

#define	DR_OUT1	PA6
#define	DR_OUT2	PA5
#define	DR_OUT3	PA4
#define	DR_OUT4	PA7

#define	DR_IN1	PC13in
#define	DR_IN2	PC14in
#define	DR_IN3	PC15in
#define	DR_IN4	PB0in

u16 dr_ctime=0;

//#define	USART_TO_RS485		//USART转485，485转USART测试

u16 sysledcnt=0;			//系统运行指示灯扫描计数  0.5秒

u8 txBuffer1[BufferSize]={0};			//USART1-TXBF
u8 rxBuffer1[BufferSize]={0};			//USART1-RXBF

u8 txBuffer_Bus[BufferSize]={0};			//USART3-TXBF
u8 rxBuffer_Bus[BufferSize]={0};			//USART3-RXBF

u8 txBuffer_PD[BufferSize]={0};				//USART2-TXBF
u8 rxBuffer_PD[BufferSize]={0};				//USART2-RXBF

u8	txflg1=0;	//USART1发送标志
u8	txflg2=0;	//485BUS发送标志
u8	txflg3=0;	//485PD发送标志

u8	flg3_txEN=0;	//与称重板发送数据使能
u8	flg3_txtime=0;	//与称重板发送数据使能

u8	rxflg=0;	//接收标志
u8	bpflg=0;	//蜂鸣器使能标志

u8 SwitchID=0;	//拔码开关地址
u16	tx1_tcont=0;	//USART1发送超时-计时
u16	tx2_tcont=0;	//485BUS发送超时-计时
u16	tx3_tcont=0;	//485PD发送超时-计时

u16	getArray_tcont=0;	//获取AD值廷时

#define RS485_Bus_RXEN	PB1=0			//rs485总线接收使能
#define RS485_Bus_TXEN	PB1=1			//rs485总线发送使能
u8	RS485_Bus_txflg=0;						//发送标志
u8	RS485_Bus_rxflg=0;						//接收标志
u8	Command_RS485_Bus=0;

#define RS485_PD_RXEN		PA1=0				//rs485接收称重板使能
#define RS485_PD_TXEN		PA1=1				//rs485发送称重板使能

u8	RS485_PD_txflg=0;							//发送标志
u8	RS485_PD_rxflg=0;							//接收标志
u8	RS485_PD_txAddr=0;						//发送序号
u8	RS485_PD_rxAddr=0;						//接收序号
u8	RS485_PD_tryCont=0;						//重试计数
u16	RS485_PD_trytime=0;						//超时计时
u16	GetBufferDelayTime=0;						//超时计时

/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PC001V21_Configuration(void)
{
	SYS_Configuration();					//系统配置---打开系统时钟 STM32_SYS.H
	
	GPIO_DeInitAll();							//将所有的GPIO关闭----V20170605
	
	PC001V21_PinSet();
	
	SysTick_Configuration(100);		//系统嘀嗒时钟配置72MHz,单位为uS
	
	IWDG_Configuration(1000);			//独立看门狗配置---参数单位ms	
	
	GPIO_Configuration_OPP2	(GPIOC,GPIO_Pin_1);			//BUZZER//将GPIO相应管脚配置为OD(开漏)输出模式，最大速度2MHz----V20170605
	
	GPIO_Configuration_IPU	(GPIOB,GPIO_Pin_9);			//S1//将GPIO相应管脚配置为上拉输入模式----V20170605
	GPIO_Configuration_IPU	(GPIOB,GPIO_Pin_8);			//S2//将GPIO相应管脚配置为上拉输入模式----V20170605
	GPIO_Configuration_IPU	(GPIOB,GPIO_Pin_7);			//S3//将GPIO相应管脚配置为上拉输入模式----V20170605
	GPIO_Configuration_IPU	(GPIOB,GPIO_Pin_6);			//S4//将GPIO相应管脚配置为上拉输入模式----V20170605
	GPIO_Configuration_IPU	(GPIOB,GPIO_Pin_5);			//S5//将GPIO相应管脚配置为上拉输入模式----V20170605
	GPIO_Configuration_IPU	(GPIOB,GPIO_Pin_4);			//S6//将GPIO相应管脚配置为上拉输入模式----V20170605
	GPIO_Configuration_IPU	(GPIOB,GPIO_Pin_3);			//S7//将GPIO相应管脚配置为上拉输入模式----V20170605
	GPIO_Configuration_IPU	(GPIOA,GPIO_Pin_15);		//S8//将GPIO相应管脚配置为上拉输入模式----V20170605
	
	
	PC001V21_USART_Conf();
	
	PWM_OUT(TIM2,PWM_OUTChannel1,1,900);						//PWM设定-20161127版本
}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PC001V21_Server(void)
{	
	IWDG_Feed();													//独立看门狗喂狗
	SwitchID=PC001V21_GetSwitchID();			//获取拔码开关地址
	PC001V21_USART1_Server();
	PC001V21_4851_Server();
	PC001V21_4852_Server();
	DR_LOCK_Check();//电子锁状态检测	

	if(GetBufferDelayTime++>1000)			//100ms
	{

//		RS485_PD_rxAddr=0;
//		RS485_PD_txAddr=0;
//		getArray_tcont=0;
//		RS485_PD_trytime=0;
//		flg3_txEN=1;
//		RS485_PD_tryCont=0;
		PC001V21_GetBufferArray();						//获取各抽屉的AD值
	}
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void PC001V21_PinSet(void)
{
	//pmos输出驱动
	GPIO_Configuration_OPP50	(GPIOA,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);					//485(uart3-en)//将GPIO相应管脚配置为OD(开漏)输出模式，最大速度2MHz----V20170605
	//pmos状态检测
	GPIO_Configuration_IPU(GPIOB,	GPIO_Pin_0);			//将GPIO相应管脚配置为上拉输入模式----V20170605
	GPIO_Configuration_IPU(GPIOC,	GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);			//将GPIO相应管脚配置为上拉输入模式----V20170605
}

/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void PC001V21_USART_Conf(void)
{
	//********************BTL（USART1)***********************************
	USART_DMA_Configuration(USART1,19200,1,1,(u32*)rxBuffer1,(u32*)txBuffer1,BufferSize);	//USART_DMA配置
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC, DISABLE);
	USART_ITConfig(USART1,USART_IT_IDLE, DISABLE);				//使用空闲中断，DMA自动接收，检测到总线空闲表示发送端已经发送完成，数据保存在DMA缓冲器中
	
	//*********************总线485（与单元板连接--USART3)***********************************
	USART_DMA_Configuration(USART3,19200,1,1,(u32*)rxBuffer_Bus,(u32*)txBuffer_Bus,BufferSize);	//USART_DMA配置
	DMA_ITConfig(DMA1_Channel2,DMA_IT_TC, DISABLE);
	USART_ITConfig(USART3,USART_IT_IDLE, DISABLE);				//使用空闲中断，DMA自动接收，检测到总线空闲表示发送端已经发送完成，数据保存在DMA缓冲器中
	
	//*********************内部485（与称重连接--USART2)***********************************
	USART_DMA_Configuration(USART2,19200,1,1,(u32*)rxBuffer_PD,(u32*)txBuffer_PD,BufferSize);	//USART_DMA配置
	DMA_ITConfig(DMA1_Channel7,DMA_IT_TC, DISABLE);
	USART_ITConfig(USART2,USART_IT_IDLE, DISABLE);				//使用空闲中断，DMA自动接收，检测到总线空闲表示发送端已经发送完成，数据保存在DMA缓冲器中
	
	GPIO_Configuration_OPP50	(GPIOA,GPIO_Pin_1);					//485(uart2-en)//将GPIO相应管脚配置为OD(开漏)输出模式，最大速度2MHz----V20170605
	GPIO_Configuration_OPP50	(GPIOB,GPIO_Pin_1);					//485(uart3-en)//将GPIO相应管脚配置为OD(开漏)输出模式，最大速度2MHz----V20170605
	
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void PC001V21_USART1_Server(void)
{
	u16 num=0;
	num=USART_RX_FlagClear(USART1);																//清除串口接收中断标志--返回DMA剩余缓冲区大小
	//*********************BTL（USART1)***********************************
	if(num!=0xFFFF)
	{
		num=BufferSize-num;																	//得到真正接收数据个数

#ifdef	USART_TO_RS485
		txflg2=1;																						//发送标志
		tx2_tcont=0;
		RS485_Bus_TXEN;																			//rs485总线发送使能
		memcpy(txBuffer_Bus, rxBuffer1,num);
		USART_DMASend(USART3,(u32*)txBuffer_Bus,num);				//串口DMA发送程序
#else
		if((rxBuffer1[0]==Command_ReadData)&&((rxBuffer1[1]==SwitchID)||(rxBuffer_Bus[1]==0x00)))
		{
			txflg1=1;	//发送标志
			tx1_tcont=0;
			txBuffer1[0]=Command_ReadData;
			txBuffer1[1]=SwitchID;			
			USART_DMASend(USART1,(u32*)txBuffer1,BufferSize);	//串口DMA发送程序						
		}
		else if((rxBuffer1[0]==DR_CTRL)&&(rxBuffer1[1]==SwitchID))
		{
			DR_LOCK_OUT(rxBuffer1[2]);
		}
#endif
		memset(rxBuffer1, 0x00, BufferSize);
		USART_DMA_RxEN(USART1,(u32*)rxBuffer1,BufferSize);	//重新设定接收缓冲区地址及大小并使能DMA接收		
	}
	//*********************清除发送标志***********************************
	if(txflg1==1)
	{
		if(tx1_tcont++>250)		//100ms---超时
		{
			txflg1=0;
			tx1_tcont=0;
			USART_ClearFlag(USART1,USART_FLAG_TC); 										//清除空闲串口标志位
			DMA_ClearFlag(DMA1_FLAG_GL4);
			DMA_Cmd(DMA1_Channel4,DISABLE);  													//关闭DMA发送
			memset(txBuffer1, 0x00, BufferSize);
		}
		else
		{
			u8 flg_tx=0;
			flg_tx=USART_TX_FlagClear(USART1);									//清除串口DMA发送全局中断标志
			if(flg_tx)
			{
					txflg1=0;
					tx1_tcont=0;
					memset(txBuffer1, 0x00, BufferSize);
			}
		}
	}	
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	称重板
* 输入			: void
* 返回值			: void
*******************************************************************************/
void PC001V21_4851_Server(void)
{
	u16 num=0;
	num=USART_RX_FlagClear(USART2);																//清除串口接收中断标志--返回DMA剩余缓冲区大小
	//*********************DMA接收处理***********************************
	if(num!=0xFFFF)
	{
		num=BufferSize-num;																	//得到真正接收数据个数
		//-----------------------接收称重板上报的数据
		if(rxBuffer_PD[0]==0x05)
		{
			u8 bufBase=0;
			u8 i=0;
			RS485_PD_rxAddr=rxBuffer_PD[1];
			if(rxBuffer_PD[1]==0x01)		//抽屉1双通道
			{
				bufBase=1;
			}
			else if(rxBuffer_PD[1]==0x03)	//抽屉2双通道
			{
				bufBase=9;
			}
			else if(rxBuffer_PD[1]==0x05)	//抽屉3双通道
			{
				bufBase=17;
			}
			else if(rxBuffer_PD[1]==0x07)	//抽屉4双通道
			{
				bufBase=25;
			}
			
			for(i=1;i<9;i++)							//将数据放入数组相应位置(RS485总线缓冲区和USART1）---覆盖式更新
			{
				txBuffer_Bus[bufBase+i]=rxBuffer_PD[i];				//RS485-BUS
				txBuffer1[bufBase+i]=rxBuffer_PD[i];					//USART1
			}		
		}
		memset(rxBuffer_PD, 0x00, num);
		USART_DMA_RxEN(USART2,(u32*)rxBuffer_PD,BufferSize);	//重新设定接收缓冲区地址及大小并使能DMA接收		
	}
	//*********************DMA发送处理***********************************
	if(flg3_txEN)
	{
		RS485_PD_TXEN;
		txflg3=1;											//发送标志
		flg3_txEN=0;
		txBuffer_PD[0]=Command_ReadData;
		txBuffer_PD[1]=RS485_PD_txAddr;
		USART_DMASend(USART2,(u32*)txBuffer_PD,2);						//自定义printf串口DMA发送程序
	}
	if(txflg3==1)
	{
		if(tx3_tcont++>500)		//50ms
		{
			txflg3=0;
			tx3_tcont=0;
			flg3_txEN=0;
			USART_ClearFlag(USART2,USART_FLAG_TC); 										//清除空闲串口标志位
			DMA_ClearFlag(DMA1_FLAG_GL7);
			DMA_Cmd(DMA1_Channel7,DISABLE);  													//关闭DMA发送
			memset(txBuffer_PD, 0x00, BufferSize);
			RS485_PD_RXEN;
		}	
		else			
		{
			u8 flg_tx=0;
			flg_tx=USART_TX_FlagClear(USART2);									//清除串口DMA发送全局中断标志
			if(flg_tx)
			{
					txflg3=0;
					tx3_tcont=0;
					flg3_txEN=0;
					memset(txBuffer_PD, 0x00, BufferSize);
					RS485_PD_RXEN;
			}
		}
	}
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	RS485外部总线 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void PC001V21_4852_Server(void)
{
	u16 num=0;
	num=USART_RX_FlagClear(USART3);												//清除串口接收中断标志--返回DMA剩余缓冲区大小
	//*********************DMA接收处理***********************************
	if(num!=0xFFFF)
	{
		num=BufferSize-num;																	//得到真正接收数据个数

#ifdef	USART_TO_RS485
		txflg1=1;																						//发送标志
		tx1_tcont=0;
		memcpy(txBuffer1, rxBuffer_Bus,num);
		USART_DMASend(USART1,(u32*)txBuffer1,num);				//串口DMA发送程序		
#else
		if((rxBuffer_Bus[0]==Command_ReadData)&&((rxBuffer_Bus[1]==SwitchID)||(rxBuffer_Bus[1]==0x00)))
		{
			txflg2=1;	//发送标志
			tx2_tcont=0;
			RS485_Bus_TXEN;		//rs485总线发送使能
			txBuffer_Bus[0]=Command_ReadData;
			txBuffer_Bus[1]=SwitchID;
			USART_DMASend(USART3,(u32*)txBuffer_Bus,BufferSize);			//自定义printf串口DMA发送程序
		}
#endif
		memset(rxBuffer_Bus, 0x00, BufferSize);
		USART_DMA_RxEN(USART3,(u32*)rxBuffer_Bus,BufferSize);	//重新设定接收缓冲区地址及大小并使能DMA接收
		
	}
	//*********************DMA发送处理***********************************
	else if(txflg2==1)
	{
		if(tx2_tcont++>100)		//100ms
		{
			txflg2=0;
			tx2_tcont=0;
			RS485_Bus_RXEN;		//rs485总线接收使能
			USART_ClearFlag(USART3,USART_FLAG_TC); 										//清除空闲串口标志位
			DMA_ClearFlag(DMA1_FLAG_GL2);
			DMA_Cmd(DMA1_Channel2,DISABLE);  													//关闭DMA发送
			memset(txBuffer_Bus, 0x00, BufferSize);
		}
		else
		{
			u8 flg_tx=0;
			flg_tx=USART_TX_FlagClear(USART3);									//清除串口DMA发送全局中断标志
			if(flg_tx)
			{
				txflg2=0;
				tx2_tcont=0;
				RS485_Bus_RXEN;		//rs485总线接收使能
				memset(txBuffer_Bus, 0x00, BufferSize);
			}
		}
	}	
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		: 获取拔码开关地址
* 输入			: void
* 返回值			: void
*******************************************************************************/
u8 PC001V21_GetSwitchID(void)
{
	u8 SwitchTemp=0;	
	
//	if(PB9in==0)				//S1
//		SwitchTemp|=1<<0;
//	if(PB8in==0)				//S2
//		SwitchTemp|=1<<1;
//	if(PB7in==0)				//S3
//		SwitchTemp|=1<<2;
//	if(PB6in==0)				//S4
//		SwitchTemp|=1<<3;
//	if(PB5in==0)				//S5
//		SwitchTemp|=1<<4;
//	if(PB4in==0)				//S6
//		SwitchTemp|=1<<5;
//	if(PB3in==0)				//S7
//		SwitchTemp|=1<<6;
//	if(PA15in==0)				//S8
//		SwitchTemp|=1<<7;
	

		SwitchTemp|=PB9in<<0;		//S1
		SwitchTemp|=PB8in<<1;		//S2
		SwitchTemp|=PB7in<<2;		//S3
		SwitchTemp|=PB6in<<3;		//S4
		SwitchTemp|=PB5in<<4;		//S5
		SwitchTemp|=PB4in<<5;		//S6
		SwitchTemp|=PB3in<<6;		//S7
		SwitchTemp|=PA15in<<7;	//S8
		
		SwitchTemp=~SwitchTemp;
	
	return SwitchTemp;
}
/*******************************************************************************
* 函数名			:	PC001V21_GetBufferArray
* 功能描述		: 获取4个抽屉8个通道的AD值，一个一个抽屉获取
* 输入			: void
* 返回值			: void
*******************************************************************************/
u8 PC001V21_GetBufferArray(void)
{
//	RS485_PD_tryCont;
	//发送地址设置 
	if(RS485_PD_rxAddr==0x00)			//开始第一个抽屉
	{
//		RS485_PD_tryCont=0;
		RS485_PD_txAddr=0x01;				//第一个抽屉地址
	}
	else if(RS485_PD_rxAddr==0x01)	//第一个抽屉已执行，开始第二个抽屉
	{
		RS485_PD_txAddr=0x03;				//第二个抽屉地址
	}
	else if(RS485_PD_rxAddr==0x03)	//第二个抽屉已执行，开始第三个抽屉
	{
		RS485_PD_txAddr=0x05;				//第三个抽屉地址
	}
	else if(RS485_PD_rxAddr==0x05)	//第三个抽屉已执行，开始第四个抽屉
	{
		RS485_PD_txAddr=0x07;				//第四个抽屉地址
	}
	else if(RS485_PD_rxAddr==0x07)	//四个抽屉都执行完成
	{
		GetBufferDelayTime=0;
		RS485_PD_txAddr=0x00;				
		RS485_PD_rxAddr=0x00;	
	}
	else
	{
//		GetBufferDelayTime=0;
		RS485_PD_txAddr=0x00;				
		RS485_PD_rxAddr=0x00;	
	}
	
	//异常重试发送：超过50ms未接收到相应抽屉数据当作通讯失败，重试3次
	
	if((flg3_txEN==0)&&(GetBufferDelayTime>=1000))
	{
		if(getArray_tcont++>200)
		{
			txflg3=0;											//发送标志
			flg3_txEN=1;
			getArray_tcont=0;
			RS485_PD_trytime=0;
		}
		if(RS485_PD_rxAddr!=RS485_PD_txAddr)		//重试
		{
			if(RS485_PD_tryCont==0)
			{
				RS485_PD_tryCont++;
				RS485_PD_trytime=0;
				getArray_tcont=0;
				flg3_txEN=1;
			}
			if(RS485_PD_trytime++>100)		//超过50ms未接收到相应抽屉数据当作通讯失败，重试3次
			{
				RS485_PD_trytime=0;
				getArray_tcont=0;
				flg3_txEN=1;
				if(RS485_PD_tryCont++>=3)
				{
					txflg3=0;
					tx3_tcont=0;
					RS485_PD_tryCont=0;
					getArray_tcont=0;
					RS485_PD_rxAddr=RS485_PD_txAddr;		//跳过，强制修改RS485_PD_rxAddr
				}
			}
		}
		
	}
	return 0;
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void DR_LOCK_OUT(unsigned char dr)
{
	dr_ctime=100;
	switch(dr)
	{
		case 0:DR_OUT1=0;DR_OUT2=0;DR_OUT3=0;DR_OUT4=0;
		break;
		case 1:DR_OUT1=1;
		break;
		case 2:DR_OUT2=1;
		break;
		case 3:DR_OUT3=1;
		break;
		case 4:DR_OUT4=1;
		break;
		default:DR_OUT1=0;DR_OUT2=0;DR_OUT3=0;DR_OUT4=0;
		break;
	}
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void DR_LOCK_Check(void)
{
	if(dr_ctime>0)
	{
		if(--dr_ctime==0)
		{
			DR_OUT1=0;
			DR_OUT2=0;
			DR_OUT3=0;
			DR_OUT4=0;
		}
	}
	if(DR_IN1==0)
	{
		DR_OUT1=0;
	}
	if(DR_IN2==0)
	{
		DR_OUT2=0;
	}
	if(DR_IN3==0)
	{
		DR_OUT3=0;
	}
	if(DR_IN4==0)
	{
		DR_OUT4=0;
	}
	
}
#endif
