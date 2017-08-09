#ifdef FX2N_14MR
#include "FX2N_14MR.H"


#include "stm32f10x_map.h"

#include "STM32_GPIO.H"
#include "STM32_SYS.H"
#include "STM32F10x_BitBand.H"
#include "STM32_SYSTICK.H"
#include "STM32_USART.H"


u32	num_temp=0;
u16	tema=0;
u8	USART_TX_FLAG=0;		//485发送标志
u8	USART_RX_FLAG=0;		//485接收标志
u32 DMASTAST=0;
u8 input_num=0;		//读取光耦变量
u8 send_num=0;		//485发送变量
u8 recev_num=0;		//485发送变量
ErrorStatus status = ERROR;

#define	FX2N_14MR_BufferSize 1		//DMA1缓冲大小

u8 FX2N_14MR_RXBuffer[FX2N_14MR_BufferSize]={0};
u8 FX2N_14MR_TXBuffer[FX2N_14MR_BufferSize]={0};


/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void FX2N_14MR_Configuration(void)
{
	SYS_Configuration();	//系统配置 STM32_SYS.H
	SysTick_Configuration(100000);	//系统嘀嗒时钟配置72MHz,单位为uS
	USART_DMA_Configuration(UART4,9600,1,1,(u32*)FX2N_14MR_RXBuffer,(u32*)FX2N_14MR_TXBuffer,FX2N_14MR_BufferSize);	//USART_DMA配置
	FX2N_14MR_GPIOSET();
	PA15=0;
	USART_TX_FLAG=0;		//485发送标志
	USART_RX_FLAG=0;		//485接收标志
}

/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void FX2N_14MR_Server(void)
{
	u32 num=0;	
	input_num=FX2N_14MR_INPUT();
	if(input_num)
		FX2N_14MR_LEDDSP(input_num);	//输入
	else
		FX2N_14MR_LEDDSP(recev_num);	//485输入
	
	if(USART_RX_FlagClear(UART4))						//清除串口DMA发送全局中断标志
	{
		USART_RX_FLAG=1;
		tema=0;
		recev_num=USART_ReceiveData(UART4);		
	}
	
	if(USART_RX_FLAG)
	{
		if(tema++>10)
		{
			tema=0;
			USART_RX_FLAG=0;
			recev_num=0;
		}
	}
	else
	{
		USART_RX_FLAG=0;
		recev_num=0;
	}
	
	
	USART_TX_DMAFlagClear(UART4);			//清除串口DMA发送全局中断标志
	
	if(input_num!=send_num)
	{
		PA15=1;		//使能发送
		USART_TX_FLAG=1;
		tema=0;
		send_num=input_num;
	}
	
	if(USART_TX_FLAG==1)
	{
		if(tema++>=1)
		{
//			USART_DMASend(UART4,(u32*)&send_num,1);
			USART_SendData(UART4,send_num);
		}
		if(tema++>3)
		{
			PA15=0;		//使能接收
			tema=0;
			USART_TX_FLAG=0;
		}		
	}

	
	
	
	if(tema>200)
	{
		num_temp=0;
		tema=0;
		USART_TX_FLAG=0;		//485发送标志
		USART_RX_FLAG=0;		//485接收标志
		DMASTAST=0;
		input_num=0;		//读取光耦变量
		send_num=0;		//485发送变量
		recev_num=0;		//485发送变量
	}
	
//	num=USART_RX_FlagClear(UART4);						//清除串口DMA发送全局中断标志
//	status=USART_TX_DMAFlagClear(UART4);			//清除串口DMA发送全局中断标志
//	DMASTAST=*(vu32*)DMA2_Channel5_BASE;
////	USART_TX_FLAG=USART_TX_DMAFlagClear(USART1);						//清除串口DMA发送全局中断标志
////	USART_RX_FlagClear(USART1);				//清除串口接收中断标志
//	if(num!=0)
//	{
//		PA15=1;
//		USART_DMAPrintf(UART4,"接收到的数据个数:%d;接收到的数据:%s\n",num,FX2N_14MR_RXBuffer);
//		USART_DMASend(UART4,(u32*)FX2N_14MR_TXBuffer,num);
//		DMASTAST=*(vu32*)DMA2_Channel5_BASE;
//		tema=0;
//	}
//	else if(status)
//	{
//		PA15=0;
//		memset(FX2N_14MR_RXBuffer,0,FX2N_14MR_BufferSize);			//初始化缓冲
//		if(tema<=100)
//		{		
//			USART_DMAPrintf(UART4,"测试开始测试开始测试开始测试开始测试开始测试开始测试开始测试开始测试开始测试开始测试开始测试开始测试开始测试开始测试开始测试开始测试开始测试开始测试开始测试开始测试开始测试开始测试开始测试开始测试开始测试开始:%d\n",tema);
//			DMASTAST=*(vu32*)DMA2_Channel5_BASE;
//			tema++;
//		}
//		free(USART_BUFFER);						//释放动态空间
//	}
//	else
//		free(USART_BUFFER);						//释放动态空间
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void FX2N_14MR_GPIOSET(void)
{
	//输入端口--光耦--低有效
//	GPIO_Configuration(GPIOA,GPIO_Pin_0,GPIO_Mode_IPU,GPIO_Speed_2MHz);			//GPIO配置
	
	GPIO_Configuration(GPIOB,GPIO_Pin_0|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//GPIO配置
	GPIO_Configuration(GPIOC,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//GPIO配置

	PB0=1;
	PB12=1;
	PB13=1;
	PB14=1;
	PB15=1;
	PC6=1;
	PC7=1;
	PC8=1;	
	
	//输出端口--ULN2003--高有效
	GPIO_Configuration(GPIOB,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);			//GPIO配置
	GPIO_Configuration(GPIOC,GPIO_Pin_12,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);			//GPIO配置

	PB5=0;
	PB6=0;
	PB7=0;
	PB8=0;
	PB9=0;
	PC12=0;

	
	//输出端口--LED--低有效
	//ERR
	GPIO_Configuration(GPIOA,GPIO_Pin_8,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);			//GPIO配置
	PA8=0;
	//485
	GPIO_Configuration(GPIOC,GPIO_Pin_9,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);			//GPIO配置
	PC9=0;
	//485CTL
	GPIO_Configuration(GPIOA,GPIO_Pin_15,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);			//GPIO配置
	PA15=0;
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void FX2N_14MR_LEDDSP(unsigned char Dsp_num)
{
	u32	num_out=0;
	if(num_temp++>42)
	{
		num_temp=0;
//		PB0=0;
//		PB12=0;
//		PB13=0;
//		PB14=0;
//		PB15=0;
//		PC6=0;
//		PC7=0;
//		PC8=0;
		
		PB5=0;
		PB6=0;
		PB7=0;
		PB8=0;
		PB9=0;
		PC12=0;
	}
	num_out=num_temp;
	
	//继电器驱动
	PB5=0;
	PB6=0;
	PB7=0;
	PB8=0;
	PB9=0;
	PC12=0;
	
	//指示灯	
	PA8=0;
	PC9=0;
	
	//RUN_LED
	if((num_out%4)==0)
	{
		PA8=1;
	}
	else	if((num_out%4)==2)
	{
		PC9=1;
	}
	
//	if(num_out==0)
//		PB0=1;
//	else	if(num_out==1)
//		PB12=1;
//	else	if(num_out==2)
//		PB13=0;
//	else	if(num_out==3)
//		PB14=1;
//	else	if(num_out==4)
//		PB15=1;
//	else	if(num_out==5)
//		PC6=1;
//	else	if(num_out==6)
//		PC7=1;
//	else	if(num_out==7)
//		PC8=1;
	
	
//	else	if(num_out==8)
//		PB5=1;
//	else	if(num_out==9)
//		PB6=1;
//	else	if(num_out==10)
//		PB7=0;
//	else	if(num_out==11)
//		PB8=1;
//	else	if(num_out==12)
//		PB9=1;
//	else	if(num_out==13)
//		PC12=1;

	if(Dsp_num==1)
		PB5=1;
	else	if(Dsp_num==2)
		PB6=1;
	else	if(Dsp_num==3)
		PB7=1;
	else	if(Dsp_num==4)
		PB8=1;
	else	if(Dsp_num==5)
		PB9=1;
	else	if(Dsp_num==6)
		PC12=1;
	else	if(Dsp_num==7)
	{
		PB5=1;
		PB6=1;
		PB7=1;
	}
	else	if(Dsp_num==8)
	{
		PB8=1;
		PB9=1;
		PC12=1;
	}
	else
	{
		PB5=0;
		PB6=0;
		PB7=0;
		PB8=0;
		PB9=0;
		PC12=0;
	}
}

unsigned char FX2N_14MR_INPUT(void)
{
	if(!PB0in)
	{
		return 1;
	}
	else if(!PB12in)
	{
		return 2;
	}
	else if(!PB13in)
	{
		return 3;
	}
	else if(!PB14in)
	{
		return 4;
	}
	else if(!PB15in)
	{
		return 5;
	}
	else if(!PC6in)
	{
		return 6;
	}
	else if(!PC7in)
	{
		return 7;
	}
	else if(!PC8in)
	{
		return 8;
	}
	else
		return 0;
}


#endif
