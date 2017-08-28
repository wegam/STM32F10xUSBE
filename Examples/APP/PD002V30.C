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
#include "74HC595.H"

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
u16 RunTime=0;
HC595_Pindef HC595_Conf;

u8 dnum=0;

void TEMPdelay(u32 time);
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
	
	Seg_Conf();
	
	SysTick_Configuration(1000);	//系统嘀嗒时钟配置72MHz,单位为uS
	
//	IWDG_Configuration(1000);			//独立看门狗配置---参数单位ms	
	
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
	
	RunTime++;
	if(RunTime>=1000)
	{
		dnum++;
		if(dnum>9)
			dnum=0;
		RunTime=0;
		TEMPdelay(0XFF);
		HC595_RCK_HIGH(&HC595_Conf);
		TEMPdelay(0XFF);
		HC595_WriteNumberN(&HC595_Conf,dnum);		//反向信号写入
		TEMPdelay(0XFF);
		HC595_RCK_LOW(&HC595_Conf);
		
	}

}

/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void PD002V30_USART_Conf(void)
{


}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void PD002V30_USART1_Server(void)
{

}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	称重板
* 输入			: void
* 返回值			: void
*******************************************************************************/
void PD002V30_485_Server(void)
{

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
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void Seg_Conf(void)
{
	HC595_Conf.HC595_SDA_PORT=GPIOB;
	HC595_Conf.HC595_SDA_Pin=GPIO_Pin_5;
	
	HC595_Conf.HC595_SCK_PORT=GPIOB;
	HC595_Conf.HC595_SCK_Pin=GPIO_Pin_4;
	
	HC595_Conf.HC595_RCK_PORT=GPIOB;
	HC595_Conf.HC595_RCK_Pin=GPIO_Pin_3;
	HC595_PinConf(&HC595_Conf);
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void TEMPdelay(u32 time)
{
	while(time--);
}
#endif
