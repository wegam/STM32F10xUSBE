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

#ifdef PD002V30				//称重托盘控制器

#include "PD002V30.H"
#include "CS5530.H"

#include "STM32F10x_BitBand.H"
#include "STM32_GPIO.H"
#include "STM32_SYS.H"
#include "STM32_SYSTICK.H"
#include "STM32_WDG.H"
#include "STM32_USART.H"
#include "STM32_PWM.H"

#include "string.h"				//串和内存操作函数头文件
#include "stm32f10x_dma.h"

#define BufferSize	12
#define Command_ReadData	(unsigned char)0x05
#define Command_SendData	(unsigned char)0x06
//#define	USART_TO_RS485		//USART转485，485转USART测试

//#define PD002V30TEST

unsigned long sysledcnt=0;			//系统运行指示灯扫描计数  0.5秒

u8 txBuffer1[BufferSize]={0};			//USART1-TXBF
u8 rxBuffer1[BufferSize]={0};			//USART1-RXBF
u8 txBuffer_PD[BufferSize]={0};			//USART1-TXBF
u8 rxBuffer_PD[BufferSize]={0};			//USART1-RXBF


u8	txflg1=0;	//USART1发送标志
u8	txflg3=0;	//485BUS发送标志

u16	tx1_tcont=0;
u16	tx3_tcont=0;



u8 SwitchID=0;	//拔码开关地址

#define RS485_PD_RXEN		PA1=0				//rs485接收称重板使能
#define RS485_PD_TXEN		PA1=1				//rs485发送称重板使能

u8	RS485_PD_txAddr=0;						//发送序号
u8	RS485_PD_rxAddr=0;						//接收序号


CS5530_Pindef CS5530_1,CS5530_2,CS5530_3;
u32 Value_AD1=0;
u32 Value_AD2=0;

u8 swid=0;


/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PD002V30_Configuration(void)
{
	SYS_Configuration();					//系统配置---打开系统时钟 STM32_SYS.H
	
	GPIO_DeInitAll();							//将所有的GPIO关闭----V20170605
	
	
	
	GPIO_Configuration_OPP2	(GPIOC,GPIO_Pin_1);			//BUZZER//将GPIO相应管脚配置为OD(开漏)输出模式，最大速度2MHz----V20170605
	
	GPIO_Configuration_IPU	(GPIOA,GPIO_Pin_4);			//S1//将GPIO相应管脚配置为上拉输入模式----V20170605
	GPIO_Configuration_IPU	(GPIOA,GPIO_Pin_5);			//S2//将GPIO相应管脚配置为上拉输入模式----V20170605
	GPIO_Configuration_IPU	(GPIOA,GPIO_Pin_6);			//S3//将GPIO相应管脚配置为上拉输入模式----V20170605
	GPIO_Configuration_IPU	(GPIOA,GPIO_Pin_7);			//S4//将GPIO相应管脚配置为上拉输入模式----V20170605
	
	PD002V30_USART_Conf();	

	cs5530init();
	
	SysTick_Configuration(1000);	//系统嘀嗒时钟配置72MHz,单位为uS
	
	IWDG_Configuration(1000);			//独立看门狗配置---参数单位ms	
	
	PWM_OUT(TIM2,PWM_OUTChannel1,1,900);						//PWM设定-20161127版本
}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PD002V30_Server(void)
{	

	IWDG_Feed();								//独立看门狗喂狗
	SwitchID=PD002V30_GetSwitchID();			//获取拔码开关地址
	PD002V30_USART1_Server();
	PD002V30_485_Server();
	cs5530test();

}

/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void PD002V30_USART_Conf(void)
{
	//********************BTL（USART1)***********************************
	USART_DMA_Configuration(USART1,115200,1,1,(u32*)rxBuffer1,(u32*)txBuffer1,BufferSize);	//USART_DMA配置
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC, DISABLE);
	USART_ITConfig(USART1,USART_IT_IDLE, DISABLE);				//使用空闲中断，DMA自动接收，检测到总线空闲表示发送端已经发送完成，数据保存在DMA缓冲器中
	
	//*********************内部485（与称重连接--USART2)***********************************
	USART_DMA_Configuration(USART2,19200,1,1,(u32*)rxBuffer_PD,(u32*)txBuffer_PD,BufferSize);	//USART_DMA配置
	DMA_ITConfig(DMA1_Channel7,DMA_IT_TC, DISABLE);
	USART_ITConfig(USART2,USART_IT_IDLE, DISABLE);				//使用空闲中断，DMA自动接收，检测到总线空闲表示发送端已经发送完成，数据保存在DMA缓冲器中
	
	GPIO_Configuration_OPP50	(GPIOA,GPIO_Pin_1);						//485(uart2-en)//将GPIO相应管脚配置为OD(开漏)输出模式，最大速度2MHz----V20170605

}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void PD002V30_USART1_Server(void)
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
		if((rxBuffer1[0]==Command_ReadData)&&(rxBuffer1[1]==SwitchID))
		{
			txflg1=1;	//发送标志
			tx1_tcont=0;
			txBuffer1[0]=Command_ReadData;
//			txBuffer1[1]=SwitchID;			
			USART_DMASend(USART1,(u32*)txBuffer1,BufferSize);	//串口DMA发送程序						
		}
#endif
		memset(rxBuffer1, 0x00, BufferSize);
		USART_DMA_RxEN(USART1,(u32*)rxBuffer1,BufferSize);	//重新设定接收缓冲区地址及大小并使能DMA接收		
	}
	//*********************清除发送标志***********************************
	if(txflg1==1)
	{
		if(tx1_tcont++>100)		//100ms---超时
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
void PD002V30_485_Server(void)
{
	u16 num=0;	
	num=USART_RX_FlagClear(USART2);																//清除串口接收中断标志--返回DMA剩余缓冲区大小
	//*********************DMA接收处理***********************************
	if(num<=BufferSize)
	{
		num=BufferSize-num;																	//得到真正接收数据个数
		//-----------------------接收称重板上报的数据
#ifdef	PD002V30TEST
		RS485_PD_TXEN;
		txflg3=1;	//发送标志
		tx3_tcont=0;
		txBuffer_PD[0]=rxBuffer_PD[0];
		txBuffer_PD[1]=rxBuffer_PD[1];
		txBuffer_PD[2]=0xA1+rxBuffer_PD[1];
		txBuffer_PD[3]=0xA2+rxBuffer_PD[1];
		txBuffer_PD[4]=0xA3+rxBuffer_PD[1];
		txBuffer_PD[5]=rxBuffer_PD[1]+1;
		txBuffer_PD[6]=0xB1+rxBuffer_PD[1];
		txBuffer_PD[7]=0xB2+rxBuffer_PD[1];
		txBuffer_PD[8]=0xB3+rxBuffer_PD[1];
		
		USART_DMASend(USART2,(u32*)txBuffer_PD,9);	//串口DMA发送程序
#else
		if((rxBuffer_PD[0]==(u8)Command_ReadData)&&(rxBuffer_PD[1]==SwitchID))
		{
			RS485_PD_TXEN;
			__nop();
			__nop();
			__nop();
			txflg3=1;	//发送标志
			tx3_tcont=0;
			txBuffer_PD[0]=Command_ReadData;
			txBuffer_PD[1]=SwitchID;			
			USART_DMASend(USART2,(u32*)txBuffer_PD,9);	//串口DMA发送程序						
		}
#endif
		memset(rxBuffer_PD, 0x00, BufferSize);
		USART_DMA_RxEN(USART2,(u32*)rxBuffer_PD,BufferSize);	//重新设定接收缓冲区地址及大小并使能DMA接收		
	}
	//*********************DMA发送处理***********************************
	else if(txflg3==1)
	{
		if(tx3_tcont++>500)		//50ms
		{
			txflg3=0;
			tx3_tcont=0;
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
					memset(txBuffer_PD, 0x00, BufferSize);
					RS485_PD_RXEN;
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
u8 PD002V30_GetSwitchID(void)
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

		SwitchTemp|=PA4in<<0;		//S1
		SwitchTemp|=PA5in<<1;		//S2
		SwitchTemp|=PA6in<<2;		//S3
		SwitchTemp|=PA7in<<3;		//S4
		
		SwitchTemp=~SwitchTemp;
		SwitchTemp=SwitchTemp&0x0F;
	
	return SwitchTemp;
}
/*******************************************************************************
* 函数名			:	PC001V21_GetBufferArray
* 功能描述		: 获取4个抽屉8个通道的AD值，一个一个抽屉获取
* 输入			: void
* 返回值			: void
*******************************************************************************/
u8 PD002V30_GetBufferArray(void)
{
	return 0;
}
/*******************************************************************************
* 函数名			:	PC001V21_GetBufferArray
* 功能描述		: 获取4个抽屉8个通道的AD值，一个一个抽屉获取
* 输入			: void
* 返回值			: void
*******************************************************************************/
void cs5530init(void)
{
	CS5530_1.CS5530_CS_PORT=GPIOB;
	CS5530_1.CS5530_CS_Pin=GPIO_Pin_12;
	
	CS5530_1.CS5530_SDI_PORT=GPIOB;
	CS5530_1.CS5530_SDI_Pin=GPIO_Pin_15;
	
	CS5530_1.CS5530_SDO_PORT=GPIOB;
	CS5530_1.CS5530_SDO_Pin=GPIO_Pin_14;
	
	CS5530_1.CS5530_SCLK_PORT=GPIOB;
	CS5530_1.CS5530_SCLK_Pin=GPIO_Pin_13;
	
	
	
	CS5530_2.CS5530_CS_PORT=GPIOA;
	CS5530_2.CS5530_CS_Pin=GPIO_Pin_8;
	
	CS5530_2.CS5530_SDI_PORT=GPIOB;
	CS5530_2.CS5530_SDI_Pin=GPIO_Pin_15;
	
	CS5530_2.CS5530_SDO_PORT=GPIOB;
	CS5530_2.CS5530_SDO_Pin=GPIO_Pin_14;
	
	CS5530_2.CS5530_SCLK_PORT=GPIOB;
	CS5530_2.CS5530_SCLK_Pin=GPIO_Pin_13;
	
	CS5530_PinConf(&CS5530_1);
	CS5530_PinConf(&CS5530_2);
	
	CS5530_PowerUp(&CS5530_1);
	CS5530_PowerUp(&CS5530_2);
}
/*******************************************************************************
* 函数名			:	PC001V21_GetBufferArray
* 功能描述		: 获取4个抽屉8个通道的AD值，一个一个抽屉获取
* 输入			: void
* 返回值			: void
*******************************************************************************/
void cs5530test(void)
{
	sysledcnt++;
//	if((sysledcnt>=5)&&(sysledcnt<=50))
//	{
//		CS5530_CS_LOW(&CS5530_1);
//		if(CS5530_SDO_STATE(&CS5530_1) == 0)
//		{			
//			Value_AD1=CS5530_GetADData(&CS5530_1)>>8;
//			txBuffer_PD[4]=(unsigned char)(Value_AD1);
//			txBuffer_PD[3]=(unsigned char)(Value_AD1>>8);
//			txBuffer_PD[2]=(unsigned char)(Value_AD1>>16);
//			txBuffer_PD[1]=(unsigned char)(SwitchID);			
//		}
//	}
//	else if(sysledcnt>=100)
//	{
//		sysledcnt=0;
//	}
//	else if(sysledcnt>55)
//	{
//		CS5530_CS_LOW(&CS5530_2);
//		if(CS5530_SDO_STATE(&CS5530_2) == 0)
//		{
//			Value_AD2=CS5530_GetADData(&CS5530_2)>>8;
//			txBuffer_PD[8]=(unsigned char)(Value_AD2);
//			txBuffer_PD[7]=(unsigned char)(Value_AD2>>8);
//			txBuffer_PD[6]=(unsigned char)(Value_AD2>>16);
//			txBuffer_PD[5]=(unsigned char)(SwitchID+1);
//		}
//	}
//	else
//	{
//		CS5530_CS_HIGH(&CS5530_1);
//		CS5530_CS_HIGH(&CS5530_2);
//	}

	
	if(sysledcnt>=500)
	{
		swid=0;
		sysledcnt=0;
		CS5530_CS_HIGH(&CS5530_1);
		CS5530_CS_HIGH(&CS5530_2);
	}
	
	if(swid==0)
	{
		CS5530_CS_HIGH(&CS5530_2);
		CS5530_CS_LOW(&CS5530_1);		
		if(CS5530_SDO_STATE(&CS5530_1) == 0)
		{			
			Value_AD1=CS5530_GetADData(&CS5530_1)>>8;
			txBuffer_PD[4]=(unsigned char)(Value_AD1);
			txBuffer_PD[3]=(unsigned char)(Value_AD1>>8);
			txBuffer_PD[2]=(unsigned char)(Value_AD1>>16);
			txBuffer_PD[1]=(unsigned char)(SwitchID);
			swid=1;			
		}
	}
	else if(swid==1)
	{
		CS5530_CS_HIGH(&CS5530_1);
		CS5530_CS_LOW(&CS5530_2);
		if(CS5530_SDO_STATE(&CS5530_2) == 0)
		{
			Value_AD2=CS5530_GetADData(&CS5530_2)>>8;
			txBuffer_PD[8]=(unsigned char)(Value_AD2);
			txBuffer_PD[7]=(unsigned char)(Value_AD2>>8);
			txBuffer_PD[6]=(unsigned char)(Value_AD2>>16);
			txBuffer_PD[5]=(unsigned char)(SwitchID+1);
			swid=0;
			
			

			memcpy(txBuffer1, txBuffer_PD,9);			
			txflg1=1;	//发送标志
			tx1_tcont=0;
			
			
			USART_DMASend(USART1,(u32*)txBuffer1,9);	//串口DMA发送程序
		}
	}
}
#endif
