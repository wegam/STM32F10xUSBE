#ifdef USBtoSPI

#include "USBtoSPI.H"
#include "STM32_SYSTICK.H"

#include "STM32_USART.H"
#include "STM32_DMA.H"
#include "STM32_TIM.H"
//#include "STM32_WDG.H"
#include "STM32_PWM.H"
#include "STM32_SPI.H"

//#include "usb_core.h"			//USB总线数据处理的核心文件
//#include "usb_pwr.h"
//#include "stm32f10x_gpio.h"

#define SPI_BufferSize 16

//GPIO_InitTypeDef GPIO_InitStructure;
SPI_InitTypeDef  SPI_InitStructure;


u8 SPI_Buffer[SPI_BufferSize]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A};

u16 SPI1_NSS=0;
u16 SPI2_NSS=0;


//GPIO_InitTypeDef GPIO_InitStructure;

u16 time=0;
extern u32 count_out;
extern u8 buffer_out[64];

void SPI_StructConf(SPI_InitTypeDef* SPI_InitStruct);
	
//u8 itf=0;
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void USBtoSPI_Configuration(void)
{
  Set_System();

	USB_EN();
	
  Set_USBClock();
	
  USB_Interrupts_Config();
	
  USB_Init();

	SysTick_Configuration(1000);	//系统嘀嗒时钟配置72MHz,单位为uS
	
	
	
	
//	TIM_Configuration(TIM3,7200,500);						//定时时间设定
	PWM_Configuration(TIM2,7200,10000,50);
	SPI_StructConf(&SPI_InitStructure);
//	SPI_BASIC_Configuration(SPI1,&SPI_InitStructure);						//SPI配置
	
//	SPI_DMA_Configuration(SPI1,&SPI_InitStructure,(u32*)SPI_Buffer,(u32*)SPI_Buffer,SPI_BUFFERSIZE);		//SPI_DMA配置
	SPI_DMA_Configuration(SPI2,&SPI_InitStructure,(u32*)SPI_Buffer,(u32*)SPI_Buffer,SPI_BUFFERSIZE);		//SPI_DMA配置

	if((SPI1->CR1&0X0200)==SPI_NSS_Soft)						//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
	{
		SPI1_NSS=SPI_NSS_Soft;
	}
	else
	{
		SPI1_NSS=SPI_NSS_Hard;
		SPI_SSOutputCmd(SPI1, ENABLE);
	}
	if((SPI2->CR1&0X0200)==SPI_NSS_Soft)						//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
	{
		SPI2_NSS=SPI_NSS_Soft;
	}
	else
	{
		SPI2_NSS=SPI_NSS_Hard;
		SPI_SSOutputCmd(SPI2, ENABLE);
	}
//	SPI_I2S_ITConfig(SPI2,SPI_I2S_FLAG_BSY,ENABLE);
//	if((SPIx->CR1&0X0200)!=SPI_NSS_Soft)						//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
//	{
//		SPI_SSOutputCmd(SPIx, ENABLE);
//	}
//	SPI1_NSS=SPI_NSS_Soft;
	
//SPI_Cmd(SPI2, ENABLE);
	
}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void USBtoSPI_Server(void)
{
	time++;
	if (count_out > 0)
	{	
		if(bDeviceState == CONFIGURED)
		{
			time=0;
			if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_0))
			{
				GPIO_ResetBits(GPIOB,GPIO_Pin_0);
				GPIO_SetBits(GPIOB,GPIO_Pin_10);
			}
			else
			{				
				GPIO_SetBits(GPIOB,GPIO_Pin_0);
				GPIO_ResetBits(GPIOB,GPIO_Pin_10);
			}
			GPIO_ResetBits(GPIOB,GPIO_Pin_12);
			SPI_Cmd(SPI2, ENABLE);
			SPI_DMA_BufferWrite(SPI2,(u32*)buffer_out,count_out);		//SPI_DMA发送函数----后边的省略号就是可变参数
			while(SPI_TX_DMAFlagClear(SPI2)==ERROR);
			while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
			while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET);
			SPI_Cmd(SPI2, DISABLE);
			GPIO_SetBits(GPIOB,GPIO_Pin_12);
			count_out = 0;
		}
	}
	else if((SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == SET)&&time>500)
	{
		time=0;
		/*
		SPI_I2S_ClearFlag(SPI2, SPI_I2S_FLAG_TXE);
		if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_0))
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);
			GPIO_SetBits(GPIOB,GPIO_Pin_10);
		}
		else
		{				
			GPIO_SetBits(GPIOB,GPIO_Pin_0);
			GPIO_ResetBits(GPIOB,GPIO_Pin_10);
		}
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
		SPI_Cmd(SPI2, ENABLE);
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
		SPI_DMA_BufferWrite(SPI2,(u32*)SPI_Buffer,10);		//SPI_DMA发送函数----后边的省略号就是可变参数
		while(SPI_TX_DMAFlagClear(SPI2)==ERROR);
		while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
		while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET);		
		SPI_Cmd(SPI2, DISABLE);
		GPIO_SetBits(GPIOB,GPIO_Pin_12);
		*/
	}
//	if(time<250)
//		GPIO_SetBits(GPIOA,GPIO_Pin_15);
//	else
//		GPIO_ResetBits(GPIOA,GPIO_Pin_15);
}
/*******************************************************************************
* 函数名		:	USB_EN
* 功能描述	:	USB 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void USB_EN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	  /* Enable USB_DISCONNECT GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		//关闭JTAG,SW功能开启

  /* Configure USB pull-up pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	GPIO_SetBits(GPIOA,GPIO_Pin_15);
	GPIO_ResetBits(GPIOA,GPIO_Pin_15);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	  /* Configure USB pull-up pin */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	GPIO_ResetBits(GPIOB,GPIO_Pin_10);
}
/*******************************************************************************
* 函数名		:	USART2_IRQHandler
* 功能描述	:	中断服务程序 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
//void USART2_IRQHandler(void)
//{
//	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//	{
//		/* Send the received data to the PC Host*/
//		USART_To_USB_Send_Data();
//	}
//	
//	/* If overrun condition occurs, clear the ORE flag and recover communication */  
//	if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)
//	{
//		(void)USART_ReceiveData(USART2);
//	}
//}

/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void SPI_StructConf(SPI_InitTypeDef* SPI_InitStruct)
{
//	SPI4->GPIO_NSS	=	GPIOA;
//	SPI4->SPI_NSSP	=	GPIO_Pin_4;
//	SPI4->GPIO_SCK	=	GPIOA;
//	SPI4->SPI_SCKP	=	GPIO_Pin_5;
//	SPI4->GPIO_MISO	=	GPIOA;
//	SPI4->SPI_MISOP	=	GPIO_Pin_6;
//	SPI4->GPIO_MOSI	=	GPIOA;
//	SPI4->SPI_MOSIP	=	GPIO_Pin_71;	
	
	SPI_InitStruct->SPI_Direction=SPI_Direction_2Lines_FullDuplex;			//设置方向				（2线全双工、2线只接收、一线发送、一线接收）
	SPI_InitStruct->SPI_Mode=SPI_Mode_Master;														//模式         	（从或主设备）
//	SPI_InitStruct->SPI_Mode=SPI_Mode_Slave;													//模式         	（从或主设备）
	SPI_InitStruct->SPI_DataSize=SPI_DataSize_8b;												//宽度         	（8或16位）
//	SPI_InitStruct->SPI_CPOL=SPI_CPOL_High;															//时钟极性     	（低或高）
	SPI_InitStruct->SPI_CPOL=SPI_CPOL_Low;															//时钟极性     	（低或高）
	SPI_InitStruct->SPI_CPHA=SPI_CPHA_2Edge;														//时钟相位     	（第一个或第二个跳变沿）
	SPI_InitStruct->SPI_NSS=SPI_NSS_Soft;																//片选方式     	（硬件或软件方式）软件控制
//	SPI_InitStruct->SPI_NSS=SPI_NSS_Hard;															//片选方式     	（硬件或软件方式）硬件控制
	SPI_InitStruct->SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_4;			//波特率预分频 	（从2---256分频）
	SPI_InitStruct->SPI_FirstBit=SPI_FirstBit_MSB;											//最先发送的位 	（最低位，还是最高位在先）
	SPI_InitStruct->SPI_CRCPolynomial=0X07;															//设置crc多项式	（其复位值为0x0007，根据应用可以设置其他数值。）
}
#endif

