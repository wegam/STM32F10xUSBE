/******************************** User_library *********************************
* 文件名 	: STM32_SDCard.H
* 作者   	: wegam@sina.com
* 版本   	: V
* 日期   	: 2016/01/01
* 说明   	: 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


#include "STM32_USART.H"

#include "STM32_GPIO.H"
//#include	"stdio.h"			//用于printf
//#include	"string.h"			//用于printf
//#include	"stdarg.h"			//用于获取不确定个数的参数
//#include	"stdlib.h"		//malloc动态申请内存空间
/*##############################################################################
################################################################################
设置波特率时的误差计算
--------------------------------------------------------------------------------
波特率				|fPCLK = 36MHz	误差%			|		fPCLK = 72MHz	误差%			|
--------------------------------------------------------------------------------
2400				|						0%						|						0%							
--------------------------------------------------------------------------------
9600				|						0%						|						0%								
--------------------------------------------------------------------------------
19200				|						0%						|						0%							
--------------------------------------------------------------------------------
57600				|						0%						|						0%							
--------------------------------------------------------------------------------
115200			|						0.15%					|						0%							
--------------------------------------------------------------------------------
230400			|						0.16%					|						0.16%							
--------------------------------------------------------------------------------
460800			|						0.16%					|						0.16%							
--------------------------------------------------------------------------------
921600			|						0.16%					|						0.16%							
--------------------------------------------------------------------------------
2250000			|						0%						|						0%							
--------------------------------------------------------------------------------

################################################################################
###############################################################################*/
//u8 Usart_Buffer[32]={0};
//选择串口开启
char *USART_BUFFER;
u32	USART_BUFFERSIZE;
/*****************************USART DMA方式收发送配置过程*******************************/
//****1.RCC（复位和时钟控制寄存器）初始化，启用GPIO、DMA、USART时钟。********************/ 
//****2.NVIC（嵌套向量中断控制寄存器）初始化，完成各个硬件中断的配置。********************/
//****3.USART初始化，配置串口，设置DMA通道等。********************/ 
//****4.DMA初始化，完成DMA的配置。********************/

//#define USE_USART USARTSELECT_TypeDef*;
/*****************************USART DMA方式收发送配置过程*******************************/
/*******************************************************************************
*函数名		: USART_Configuration
*功能描述	:	串口配置
*输入			:	串口号	--------USARTx：USART1/USART2/USART3/UART4/UART5
						波特率	--------USART_BaudRate
						字长----------USART_WordLength
						停止位	--------USART_StopBits
						奇偶校验------USART_Parity
						串口DMA使能---UsartDMAState NewState
*输出			:	无
*返回值		:	无
*例程			:	USART_Configuration(USART1,19200,8,1,0);
*******************************************************************************/
void USART_BASIC_Configuration(USART_TypeDef* USARTx,u32 USART_BaudRate,u8 NVICPreemptionPriority,u8 NVIC_SubPriority)	//串口配置
{

	//1)**********定义变量
//	USART_InitTypeDef USART_InitStructure;				//USART结构体	
//	GPIO_InitTypeDef GPIO_InitStructure;					//GPIO结构体
//	NVIC_InitTypeDef NVIC_InitStructure; 					//NVIC结构体

//	
//	u16 TXD_Pin=0;																//串口发送脚
//	u16 RXD_Pin=0;																//串口接收脚
//	GPIO_TypeDef* GPIO_TX=0;
//	GPIO_TypeDef* GPIO_RX=0;
//	u8 USARTx_IRQChannel=0;
	
	
	//2)**********串口基础配置
	USART_Set(USARTx,USART_BaudRate,NVICPreemptionPriority,NVIC_SubPriority);				//根据串口号设置相关引脚和返回中断信号源
	
//	USART_Cmd(USARTx, ENABLE);
	//4)**********串口接收中断开
	//--若DMA接收未开启，则使用串口接收中断
	//--若DMA接收开启，串口接收中断应该关闭，在DMA配置中会将串口接收中断关闭
//	USART_ITConfig(USARTx,USART_IT_RXNE, ENABLE);		//接收中断--开
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);		//接收中断--开
//	USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);     //接收空闲中断使能
	USART_ClearITPendingBit(USARTx,USART_IT_IDLE); 	//清除空闲串口标志位
//	USART_ClearITPendingBit(USARTx,USART_IT_IDLE); 	//清除空闲串口标志位
	/* 开启DMA方式接收*/
	USART_DMACmd(USARTx,USART_DMAReq_Rx,ENABLE);
	/* 开启DMA方式发送*/
	USART_DMACmd(USARTx,USART_DMAReq_Tx,ENABLE);
	//5)**********使能串口
	USART_Cmd(USARTx, ENABLE);
}
/*******************************************************************************
*函数名		:	USART_DMA_Configuration
*功能描述	:	DMA串口配置，需要将串口接收中断关闭，然后开启串口空闲中断，利用DMA自动接收串口数据
*输入			:	RXDBuffer--接收缓冲区，可以与发送缓冲区共用,
						TXDBuffer--发送缓冲区，可以与接收缓冲区共用,
						BufferSize--缓存大小
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void USART_DMA_Configuration(USART_TypeDef* USARTx,u32 USART_BaudRate,u8 NVICPreemptionPriority,u8 NVIC_SubPriority,u32 *RXDBuffer,u32 *TXDBuffer,u32 BufferSize)	//USART_DMA配置
{
	//1)**********定义变量	
	DMA_InitTypeDef DMA_Initstructure;
	NVIC_InitTypeDef NVIC_Initstructure;
	
	DMA_Channel_TypeDef* DMAx_Channeltx=0;			//DMA发送通道请求信号---当DMA串口发送数据完成时，会发起DMA中断
	DMA_Channel_TypeDef* DMAx_Channelrx=0;			//DMA接收通道请求信号---DMA串口接收由串口发起中断，因此此处接收通道中断不使用
	u8 DMAx_Channelx_IRQChannel=0;							//DMA中断源
	u32 DMAx_FLAG_GLtx=0;												//DMA串口发送中断全局变量			
	u32 DMAx_FLAG_GLrx=0;												//DMA串口接收中断全局变量
	
	USART_BUFFERSIZE=BufferSize;
	
//	//2)**********关闭串口等待DMA配置完成	
//	USART_Cmd(USARTx, DISABLE);
	
//	//3)**********关闭串口等待DMA配置完成
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);	
	//2)**********串口基础配置
	USART_Set(USARTx,USART_BaudRate,NVICPreemptionPriority,NVIC_SubPriority);				//根据串口号设置相关引脚和返回中断信号源
	//4)**********根据串口索引相关DMA通道及其它参数
	if(USARTx==USART1)
	{
		DMAx_Channeltx=DMA1_Channel4;
		DMAx_Channelrx=DMA1_Channel5;
		DMAx_Channelx_IRQChannel=DMA1_Channel4_IRQChannel;
		DMAx_FLAG_GLtx=DMA1_FLAG_GL4;
		DMAx_FLAG_GLrx=DMA1_FLAG_GL5;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);	
	}
	else if(USARTx==USART2)
	{
		DMAx_Channeltx=DMA1_Channel7;
		DMAx_Channelrx=DMA1_Channel6;
		DMAx_Channelx_IRQChannel=DMA1_Channel7_IRQChannel;
		DMAx_FLAG_GLtx=DMA1_FLAG_GL7;
		DMAx_FLAG_GLrx=DMA1_FLAG_GL6;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);	
	}
	else if(USARTx==USART3)
	{
		DMAx_Channeltx=DMA1_Channel2;
		DMAx_Channelrx=DMA1_Channel3;
		DMAx_Channelx_IRQChannel=DMA1_Channel2_IRQChannel;
		DMAx_FLAG_GLtx=DMA1_FLAG_GL2;
		DMAx_FLAG_GLrx=DMA1_FLAG_GL3;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);	
	}
	else if(USARTx==UART4)
	{
		DMAx_Channeltx=DMA2_Channel5;
		DMAx_Channelrx=DMA2_Channel3;
		DMAx_Channelx_IRQChannel=DMA2_Channel3_IRQChannel;
		DMAx_FLAG_GLtx=DMA2_FLAG_GL5;
		DMAx_FLAG_GLrx=DMA2_FLAG_GL3;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);	
	}
	else if(USARTx==UART5)
	{
	}
	//5)**********DMA发送初始化，外设作为DMA的目的端
	DMA_Initstructure.DMA_PeripheralBaseAddr =  (u32)(&USARTx->DR);					//DMA外设源地址
	DMA_Initstructure.DMA_MemoryBaseAddr     = (u32)TXDBuffer;							//DMA数据内存地址
	DMA_Initstructure.DMA_DIR = DMA_DIR_PeripheralDST;											//DMA_DIR_PeripheralDST（外设作为DMA的目的端），DMA_DIR_PeripheralSRC（外设作为数据传输的来源）
	DMA_Initstructure.DMA_BufferSize = USART_BUFFERSIZE; 													//指定DMA通道的DMA缓存的大小
//	DMA_Initstructure.DMA_BufferSize = sizeof(MemoryAddr[0]); 						//指定DMA通道的DMA缓存的大小
	DMA_Initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//DMA_PeripheralInc_Enable（外设地址寄存器递增），DMA_PeripheralInc_Disable（外设地址寄存器不变），
	DMA_Initstructure.DMA_MemoryInc =DMA_MemoryInc_Enable;									//DMA_MemoryInc_Enable（内存地址寄存器递增），DMA_MemoryInc_Disable（内存地址寄存器不变）
	DMA_Initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//外设数据宽度--DMA_PeripheralDataSize_Byte（数据宽度为8位），DMA_PeripheralDataSize_HalfWord（数据宽度为16位），DMA_PeripheralDataSize_Word（数据宽度为32位）
	DMA_Initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//内存数据宽度--DMA_MemoryDataSize_Byte（数据宽度为8位），DMA_MemoryDataSize_HalfWord（数据宽度为16位），DMA_MemoryDataSize_Word（数据宽度为32位）
	DMA_Initstructure.DMA_Mode = DMA_Mode_Normal;														//DMA工作模式--DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular（不停地传送）
	DMA_Initstructure.DMA_Priority = DMA_Priority_High; 										//DMA通道的转输优先级--DMA_Priority_VeryHigh（非常高）DMA_Priority_High（高)，DMA_Priority_Medium（中），DMA_Priority_Low（低）
	DMA_Initstructure.DMA_M2M = DMA_M2M_Disable;														//DMA通道的内存到内存传输--DMA_M2M_Enable(设置为内存到内存传输)，DMA_M2M_Disable（非内存到内存传输）
	DMA_Init(DMAx_Channeltx,&DMA_Initstructure);														//初始化DMA

	//6)**********DMA接收初始化，外设作为DMA的源端
	DMA_Initstructure.DMA_PeripheralBaseAddr =  (u32)(&USARTx->DR);					//DMA外设源地址
	DMA_Initstructure.DMA_MemoryBaseAddr     = 	(u32)RXDBuffer;						//DMA数据内存地址
	DMA_Initstructure.DMA_DIR = DMA_DIR_PeripheralSRC;											//DMA_DIR_PeripheralDST（外设作为DMA的目的端），DMA_DIR_PeripheralSRC（外设作为数据传输的来源）
	DMA_Initstructure.DMA_BufferSize = USART_BUFFERSIZE; 													//指定DMA通道的DMA缓存的大小
//	DMA_Initstructure.DMA_BufferSize = sizeof(MemoryAddr); 								//指定DMA通道的DMA缓存的大小
	DMA_Initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//DMA_PeripheralInc_Enable（外设地址寄存器递增），DMA_PeripheralInc_Disable（外设地址寄存器不变），
	DMA_Initstructure.DMA_MemoryInc =DMA_MemoryInc_Enable;									//DMA_MemoryInc_Enable（内存地址寄存器递增），DMA_MemoryInc_Disable（内存地址寄存器不变）
	DMA_Initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//外设数据宽度--DMA_PeripheralDataSize_Byte（数据宽度为8位），DMA_PeripheralDataSize_HalfWord（数据宽度为16位），DMA_PeripheralDataSize_Word（数据宽度为32位）
	DMA_Initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//内存数据宽度--DMA_MemoryDataSize_Byte（数据宽度为8位），DMA_MemoryDataSize_HalfWord（数据宽度为16位），DMA_MemoryDataSize_Word（数据宽度为32位）
	DMA_Initstructure.DMA_Mode = DMA_Mode_Normal;														//DMA工作模式--DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular（不停地传送）
	DMA_Initstructure.DMA_Priority = DMA_Priority_High; 										//DMA通道的转输优先级--DMA_Priority_VeryHigh（非常高）DMA_Priority_High（高)，DMA_Priority_Medium（中），DMA_Priority_Low（低）
	DMA_Initstructure.DMA_M2M = DMA_M2M_Disable;														//DMA通道的内存到内存传输--DMA_M2M_Enable(设置为内存到内存传输)，DMA_M2M_Disable（非内存到内存传输）
	DMA_Init(DMAx_Channelrx,&DMA_Initstructure);														//初始化DMA
	
	//7)**********DMA通道中断初始化---此为DMA发送中断----DMA发送完成中断
	NVIC_Initstructure.NVIC_IRQChannel = DMAx_Channelx_IRQChannel;       		//通道设置为串口1中断
	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority=NVICPreemptionPriority;
	NVIC_Initstructure.NVIC_IRQChannelSubPriority = NVIC_SubPriority; 			//中断响应优先级0	
	NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;        									//打开中断
	NVIC_Init(&NVIC_Initstructure);
	
	
	//8)**********配置相关中断
	//8.1)**********串口接收中断配置
	//--将串口接收中断关闭，然后开启串口空闲中断，利用DMA自动接收串口数据
	//--若DMA接收未开启，则使用串口接收中断
	//--若DMA接收开启，串口接收中断应该关闭，在DMA配置中会将串口接收中断关闭
//	USART_ITConfig(USARTx,USART_IT_RXNE, DISABLE);				//接收中断--关：因为DMA自动接收，
	
	/* 启动DMA1通道5*/
	DMA_Cmd(DMAx_Channeltx,DISABLE);
//	DMA_Cmd(DMA1_Channel7,DISABLE);									//DMA发送开启
	//10.2)**********使能串口
	DMA_Cmd(DMAx_Channelrx,ENABLE);	
	//9.1)**********关闭DMA发送	
	
	
	//8.2)**********使能串口DMA方式接收
	USART_DMACmd(USARTx,USART_DMAReq_Rx,ENABLE);
	//8.3)**********使能串口DMA方式发送
	USART_DMACmd(USARTx,USART_DMAReq_Tx,ENABLE);
	//9)**********清除相关中断标志位	
	//	DMA_Cmd(DMAx_Channeltx,ENABLE);
	//9.2)**********使能相关DMA通道传输完成中断
	DMA_ITConfig(DMAx_Channeltx,DMA_IT_TC, ENABLE);
	//9.3)**********清除串口DMA方式发送中断全局标志
	DMA_ClearFlag(DMAx_FLAG_GLtx);                                 					// 清除DMA所有标志
	//9.3)**********清除串口DMA方式接收中断全局标志
	DMA_ClearFlag(DMAx_FLAG_GLrx);                                 					// 清除DMA所有标志	
	//10.1)**********启动串口DMA方式接收	
	
	USART_ITConfig(USARTx,USART_IT_IDLE, ENABLE);					//使用空闲中断，DMA自动接收，检测到总线空闲表示发送端已经发送完成，数据保存在DMA缓冲器中
	USART_ClearITPendingBit(USARTx,USART_IT_IDLE); 				//清除空闲串口标志位
	
	USART_Cmd(USARTx, ENABLE);
}
/*******************************************************************************
*函数名		:USART_Set
*功能描述	:串口基础配置
*输入			: 
*输出			:无
*返回值		:无
*例程			:
*******************************************************************************/
void USART_Set(USART_TypeDef* USARTx,u32 USART_BaudRate,u8 NVICPreemptionPriority,u8 NVIC_SubPriority)
{
	//1)**********定义变量
	USART_InitTypeDef USART_InitStructure;				//USART结构体	
	GPIO_InitTypeDef GPIO_InitStructure;					//GPIO结构体
	NVIC_InitTypeDef NVIC_InitStructure; 					//NVIC结构体

	
	u16 TXD_Pin=0;																//串口发送脚
	u16 RXD_Pin=0;																//串口接收脚
	GPIO_TypeDef* GPIO_TX=0;
	GPIO_TypeDef* GPIO_RX=0;
	u8 USARTx_IRQChannel=0;
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);				//关闭AFIO时钟,为关闭JTAG功能

//>>>>>>>>>>>>>>>>>>>>>初始化参数-开始>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	//2)**********初始化变量及开启相关时钟
	//2.1)**********USART1
	if(USARTx==USART1)
	{
		TXD_Pin=GPIO_Pin_9;											//USART1-TX>PA9
		RXD_Pin=GPIO_Pin_10;										//USART1-RX>PA10
		
		GPIO_TX=GPIOA;
		GPIO_RX=GPIOA;
		
		USARTx_IRQChannel=USART1_IRQChannel;		//中断
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//USART1时钟开启

	}
	//2.2)**********USART2
	else if(USARTx==USART2)
	{
		TXD_Pin=GPIO_Pin_2;		//USART2-TX>PA2
		RXD_Pin=GPIO_Pin_3;		//USART2-RX>PA3
		
		GPIO_TX=GPIOA;
		GPIO_RX=GPIOA;
		
		USARTx_IRQChannel=USART2_IRQChannel;		//中断
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//USART1时钟开启

	}
	//2.3)**********USART3
	else if(USARTx==USART3)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);				//关闭AFIO时钟,为关闭JTAG功能
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //关闭JTAG功能
//		GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);  		//关闭SWD功能
		
		
		TXD_Pin=GPIO_Pin_10;	//USART3-TX>PB10
		RXD_Pin=GPIO_Pin_11;	//USART3-RX>PB11
		
		GPIO_TX=GPIOB;
		GPIO_RX=GPIOB;
		
		USARTx_IRQChannel=USART3_IRQChannel;		//中断
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//USART1时钟开启
		

	}
	//2.4)**********USART4
	else if(USARTx==UART4)
	{
		TXD_Pin=GPIO_Pin_10;	//USART1-TX>PC10
		RXD_Pin=GPIO_Pin_11;	//USART1-RX>PC11
		
		GPIO_TX=GPIOC;
		GPIO_RX=GPIOC;
		
		USARTx_IRQChannel=UART4_IRQChannel;		//中断
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	//USART1时钟开启

	}
	//2.5)**********USART5
	else if(USARTx==UART5)
	{
		TXD_Pin=GPIO_Pin_12;	//USART1-TX>PC12
		RXD_Pin=GPIO_Pin_2;		//USART1-RX>PD2
		
		GPIO_TX=GPIOC;
		GPIO_RX=GPIOD;
		
		USARTx_IRQChannel=UART5_IRQChannel;		//中断
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);	//USART1时钟开启
	}

//<<<<<<<<<<<<<<<<<<<<<初始化参数-结束<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	//3.1)**********初始化TXD引脚
	GPIO_InitStructure.GPIO_Pin = TXD_Pin;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_TX,&GPIO_InitStructure);

	//3.2)**********初始化RXD引脚
	GPIO_InitStructure.GPIO_Pin = RXD_Pin;
	//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;							//上拉输入
	GPIO_Init(GPIO_RX,&GPIO_InitStructure);
	

	//4)**********串口全局中断初始化
	NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVICPreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=NVIC_SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//3)**********初始化USART
	//设置参数
	USART_DeInit(USARTx);
	USART_InitStructure.USART_BaudRate = USART_BaudRate; 					//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;				//停止位
	USART_InitStructure.USART_Parity = USART_Parity_No ; 					//奇偶校验
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//流控
	USART_Init(USARTx, &USART_InitStructure);											//初始化串口
	
//	//4)**********返回中断信号源
//	return 0;

	
}
/*******************************************************************************
*函数名		:USART_DMASet
*功能描述	:串口接收服务函数
*输入			: 
*输出			:无
*返回值		:无
*例程			:
*******************************************************************************/
void USART_DMASet(USART_TypeDef* USARTx,u32 USART_BaudRate,u8 NVICPreemptionPriority,u8 NVIC_SubPriority,u32 *RXDBuffer,u32 *TXDBuffer,u32 BufferSize)	//USART_DMA配置
{
	//1)**********定义变量	
	DMA_InitTypeDef DMA_Initstructure;
	NVIC_InitTypeDef NVIC_Initstructure;
	
	DMA_Channel_TypeDef* DMAx_Channeltx=0;			//DMA发送通道请求信号---当DMA串口发送数据完成时，会发起DMA中断
	DMA_Channel_TypeDef* DMAx_Channelrx=0;			//DMA接收通道请求信号---DMA串口接收由串口发起中断，因此此处接收通道中断不使用
	u8 DMAx_Channelx_IRQChannel=0;							//DMA中断源
	u32 DMAx_FLAG_GLtx=0;												//DMA串口发送中断全局变量			
	u32 DMAx_FLAG_GLrx=0;												//DMA串口接收中断全局变量
	
//	//2)**********关闭串口等待DMA配置完成	
//	USART_Cmd(USARTx, DISABLE);
	
//	//3)**********关闭串口等待DMA配置完成
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);	
	//2)**********串口基础配置
	USART_Set(USARTx,USART_BaudRate,NVICPreemptionPriority,NVIC_SubPriority);				//根据串口号设置相关引脚和返回中断信号源
	//4)**********根据串口索引相关DMA通道及其它参数
	if(USARTx==USART1)
	{
		DMAx_Channeltx=DMA1_Channel4;
		DMAx_Channelrx=DMA1_Channel5;
		DMAx_Channelx_IRQChannel=DMA1_Channel4_IRQChannel;
		DMAx_FLAG_GLtx=DMA1_FLAG_GL4;
		DMAx_FLAG_GLrx=DMA1_FLAG_GL5;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);	
	}
	else if(USARTx==USART2)
	{
		DMAx_Channeltx=DMA1_Channel7;
		DMAx_Channelrx=DMA1_Channel6;
		DMAx_Channelx_IRQChannel=DMA1_Channel7_IRQChannel;
		DMAx_FLAG_GLtx=DMA1_FLAG_GL7;
		DMAx_FLAG_GLrx=DMA1_FLAG_GL6;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);	
	}
	else if(USARTx==USART3)
	{
		DMAx_Channeltx=DMA1_Channel2;
		DMAx_Channelrx=DMA1_Channel3;
		DMAx_Channelx_IRQChannel=DMA1_Channel2_IRQChannel;
		DMAx_FLAG_GLtx=DMA1_FLAG_GL2;
		DMAx_FLAG_GLrx=DMA1_FLAG_GL3;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);	
	}
	else if(USARTx==UART4)
	{
		DMAx_Channeltx=DMA2_Channel5;
		DMAx_Channelrx=DMA2_Channel3;
		DMAx_Channelx_IRQChannel=DMA2_Channel3_IRQChannel;
		DMAx_FLAG_GLtx=DMA2_FLAG_GL5;
		DMAx_FLAG_GLrx=DMA2_FLAG_GL3;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);	
	}
	else if(USARTx==UART5)
	{
	}
	//5)**********DMA发送初始化，外设作为DMA的目的端
	DMA_Initstructure.DMA_PeripheralBaseAddr =  (u32)(&USARTx->DR);					//DMA外设源地址
	DMA_Initstructure.DMA_MemoryBaseAddr     = (u32)TXDBuffer;							//DMA数据内存地址
	DMA_Initstructure.DMA_DIR = DMA_DIR_PeripheralDST;											//DMA_DIR_PeripheralDST（外设作为DMA的目的端），DMA_DIR_PeripheralSRC（外设作为数据传输的来源）
	DMA_Initstructure.DMA_BufferSize = BufferSize; 													//指定DMA通道的DMA缓存的大小
//	DMA_Initstructure.DMA_BufferSize = sizeof(MemoryAddr[0]); 						//指定DMA通道的DMA缓存的大小
	DMA_Initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//DMA_PeripheralInc_Enable（外设地址寄存器递增），DMA_PeripheralInc_Disable（外设地址寄存器不变），
	DMA_Initstructure.DMA_MemoryInc =DMA_MemoryInc_Enable;									//DMA_MemoryInc_Enable（内存地址寄存器递增），DMA_MemoryInc_Disable（内存地址寄存器不变）
	DMA_Initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//外设数据宽度--DMA_PeripheralDataSize_Byte（数据宽度为8位），DMA_PeripheralDataSize_HalfWord（数据宽度为16位），DMA_PeripheralDataSize_Word（数据宽度为32位）
	DMA_Initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//内存数据宽度--DMA_MemoryDataSize_Byte（数据宽度为8位），DMA_MemoryDataSize_HalfWord（数据宽度为16位），DMA_MemoryDataSize_Word（数据宽度为32位）
	DMA_Initstructure.DMA_Mode = DMA_Mode_Normal;														//DMA工作模式--DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular（不停地传送）
	DMA_Initstructure.DMA_Priority = DMA_Priority_High; 										//DMA通道的转输优先级--DMA_Priority_VeryHigh（非常高）DMA_Priority_High（高)，DMA_Priority_Medium（中），DMA_Priority_Low（低）
	DMA_Initstructure.DMA_M2M = DMA_M2M_Disable;														//DMA通道的内存到内存传输--DMA_M2M_Enable(设置为内存到内存传输)，DMA_M2M_Disable（非内存到内存传输）
	DMA_Init(DMAx_Channeltx,&DMA_Initstructure);														//初始化DMA

	//6)**********DMA接收初始化，外设作为DMA的源端
	DMA_Initstructure.DMA_PeripheralBaseAddr =  (u32)(&USARTx->DR);					//DMA外设源地址
	DMA_Initstructure.DMA_MemoryBaseAddr     = 	(u32)RXDBuffer;						//DMA数据内存地址
	DMA_Initstructure.DMA_DIR = DMA_DIR_PeripheralSRC;											//DMA_DIR_PeripheralDST（外设作为DMA的目的端），DMA_DIR_PeripheralSRC（外设作为数据传输的来源）
	DMA_Initstructure.DMA_BufferSize = BufferSize; 													//指定DMA通道的DMA缓存的大小
//	DMA_Initstructure.DMA_BufferSize = sizeof(MemoryAddr); 								//指定DMA通道的DMA缓存的大小
	DMA_Initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//DMA_PeripheralInc_Enable（外设地址寄存器递增），DMA_PeripheralInc_Disable（外设地址寄存器不变），
	DMA_Initstructure.DMA_MemoryInc =DMA_MemoryInc_Enable;									//DMA_MemoryInc_Enable（内存地址寄存器递增），DMA_MemoryInc_Disable（内存地址寄存器不变）
	DMA_Initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//外设数据宽度--DMA_PeripheralDataSize_Byte（数据宽度为8位），DMA_PeripheralDataSize_HalfWord（数据宽度为16位），DMA_PeripheralDataSize_Word（数据宽度为32位）
	DMA_Initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//内存数据宽度--DMA_MemoryDataSize_Byte（数据宽度为8位），DMA_MemoryDataSize_HalfWord（数据宽度为16位），DMA_MemoryDataSize_Word（数据宽度为32位）
	DMA_Initstructure.DMA_Mode = DMA_Mode_Normal;														//DMA工作模式--DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular（不停地传送）
	DMA_Initstructure.DMA_Priority = DMA_Priority_High; 										//DMA通道的转输优先级--DMA_Priority_VeryHigh（非常高）DMA_Priority_High（高)，DMA_Priority_Medium（中），DMA_Priority_Low（低）
	DMA_Initstructure.DMA_M2M = DMA_M2M_Disable;														//DMA通道的内存到内存传输--DMA_M2M_Enable(设置为内存到内存传输)，DMA_M2M_Disable（非内存到内存传输）
	DMA_Init(DMAx_Channelrx,&DMA_Initstructure);														//初始化DMA
	
	//7)**********DMA通道中断初始化---此为DMA发送中断----DMA发送完成中断
	NVIC_Initstructure.NVIC_IRQChannel = DMAx_Channelx_IRQChannel;       		//通道设置为串口1中断
	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority=NVICPreemptionPriority;
	NVIC_Initstructure.NVIC_IRQChannelSubPriority = NVIC_SubPriority; 			//中断响应优先级0	
	NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;        									//打开中断
	NVIC_Init(&NVIC_Initstructure);
	
	
	//8)**********配置相关中断
	//8.1)**********串口接收中断配置
	//--将串口接收中断关闭，然后开启串口空闲中断，利用DMA自动接收串口数据
	//--若DMA接收未开启，则使用串口接收中断
	//--若DMA接收开启，串口接收中断应该关闭，在DMA配置中会将串口接收中断关闭
//	USART_ITConfig(USARTx,USART_IT_RXNE, DISABLE);				//接收中断--关：因为DMA自动接收，
	USART_ITConfig(USARTx,USART_IT_IDLE, ENABLE);					//使用空闲中断，DMA自动接收，检测到总线空闲表示发送端已经发送完成，数据保存在DMA缓冲器中
	USART_ClearITPendingBit(USARTx,USART_IT_IDLE); 				//清除空闲串口标志位

	//8.2)**********使能串口DMA方式接收
	USART_DMACmd(USARTx,USART_DMAReq_Rx,ENABLE);
	//8.3)**********使能串口DMA方式发送
	USART_DMACmd(USARTx,USART_DMAReq_Tx,ENABLE);
	//9)**********清除相关中断标志位	
	//	DMA_Cmd(DMAx_Channeltx,ENABLE);
	//9.2)**********使能相关DMA通道传输完成中断
	DMA_ITConfig(DMAx_Channeltx,DMA_IT_TC, ENABLE);
	//9.3)**********清除串口DMA方式发送中断全局标志
	DMA_ClearFlag(DMAx_FLAG_GLtx);                                 					// 清除DMA所有标志
	//9.3)**********清除串口DMA方式接收中断全局标志
	DMA_ClearFlag(DMAx_FLAG_GLrx);                                 					// 清除DMA所有标志	
	//10.1)**********启动串口DMA方式接收
	/* 启动DMA1通道5*/
	DMA_Cmd(DMAx_Channelrx,ENABLE);	
	//9.1)**********关闭DMA发送
	DMA_Cmd(DMAx_Channeltx,ENABLE);
	//10.2)**********使能串口
	USART_Cmd(USARTx, ENABLE);
	

}
/*******************************************************************************
*函数名		: function
*功能描述	:	串口接收服务函数
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:	USART_DMASend(USART2,"中文ENG=%d\n",num);
*特别说明	:	在DMA发送完成后需要释放动态空间，free(USART_BUFFER);
					:	USART_BUFFER定义在STM32_USART.H
*******************************************************************************/
int USART_DMAPrintf(USART_TypeDef* USARTx,const char *format,...)		//后边的省略号就是可变参数
{
	
//		va_list ap; 										//VA_LIST 是在C语言中解决变参问题的一组宏，所在头文件：#include <stdarg.h>,用于获取不确定个数的参数
//    static char string[ 256 ];			//定义数组，
//    va_start( ap, format );
//    vsprintf( string , format, ap );    
//    va_end( ap );

	//1)**********获取数据宽度
	u16 num=strlen((const char*)format);		//获取数据宽度
	//2)**********定义缓冲区大小变量
	unsigned int BufferSize;
	//3)**********args为定义的一个指向可变参数的变量，va_list以及下边要用到的va_start,va_end都是是在定义，可变参数函数中必须要用到宏， 在stdarg.h头文件中定义
	va_list args;                                        
	//4)**********申请动态空间
	USART_BUFFER = (char*)malloc(sizeof(char) * num);	
	//5)**********初始化args的函数，使其指向可变参数的第一个参数，format是可变参数的前一个参数
	va_start(args, format);
	//6)**********正常情况下返回生成字串的长度(除去\0),错误情况返回负值
	BufferSize = vsprintf(USART_BUFFER, format, args);
	//7)**********结束可变参数的获取
	va_end(args);                                      		
	//8)**********将等发送缓冲区大小（数据个数）及缓冲区地址发给DMA开启发送
	//8)**********DMA发送完成后注意应该释放缓冲区：free(USART_BUFFER);
	if(USARTx==USART1)
	{
		DMA1_Channel4->CNDTR =BufferSize;										//设定待发送缓冲区大小
		DMA1_Channel4->CMAR =(u32)USART_BUFFER;							//发送缓冲区	
		DMA_Cmd(DMA1_Channel4,ENABLE);											//DMA发送开启3
	}
	else if(USARTx==USART2)
	{
		DMA1_Channel7->CNDTR =BufferSize;										//设定待发送缓冲区大小
		DMA1_Channel7->CMAR =(u32)USART_BUFFER;							//发送缓冲区	
		DMA_Cmd(DMA1_Channel7,ENABLE);											//DMA发送开启3
	}
	else if(USARTx==USART3)
	{
		DMA1_Channel2->CNDTR =BufferSize;										//设定待发送缓冲区大小
		DMA1_Channel2->CMAR =(u32)USART_BUFFER;							//发送缓冲区	
		DMA_Cmd(DMA1_Channel2,ENABLE);											//DMA发送开启3
	}
	else if(USARTx==UART4)
	{
		DMA1_Channel5->CNDTR =BufferSize;										//设定待发送缓冲区大小
		DMA1_Channel5->CMAR =(u32)USART_BUFFER;							//发送缓冲区	
		DMA_Cmd(DMA1_Channel5,ENABLE);											//DMA发送开启3
	}
//	else if(USARTx==UART5)
//	{
//		DMA1_Channel7->CNDTR =BufferSize;										//设定待发送缓冲区大小
//		DMA1_Channel7->CMAR =(u32)USART_BUFFER;							//发送缓冲区	
//		DMA_Cmd(DMA1_Channel7,ENABLE);											//DMA发送开启3
//	}
	
	return BufferSize;

}
/*******************************************************************************
*函数名		: function
*功能描述	:	串口接收服务函数
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:	USART_DMASend(USART2,"中文ENG=%d\n",num);
*特别说明	:	在DMA发送完成后需要释放动态空间，free(USART_BUFFER);
					:	USART_BUFFER定义在STM32_USART.H
*******************************************************************************/
int USART_DMASend(USART_TypeDef* USARTx,u32 *USART_BUFFER,u16 BufferSize)		//
{
                                  		
	//8)**********将等发送缓冲区大小（数据个数）及缓冲区地址发给DMA开启发送
	//8)**********DMA发送完成后注意应该释放缓冲区：free(USART_BUFFER);
	if(USARTx==USART1)
	{
		DMA1_Channel4->CNDTR =BufferSize;										//设定待发送缓冲区大小
		DMA1_Channel4->CMAR =(u32)USART_BUFFER;							//发送缓冲区	
		DMA_Cmd(DMA1_Channel4,ENABLE);											//DMA发送开启3
	}
	else if(USARTx==USART2)
	{
		DMA1_Channel7->CNDTR =BufferSize;										//设定待发送缓冲区大小
		DMA1_Channel7->CMAR =(u32)USART_BUFFER;							//发送缓冲区	
		DMA_Cmd(DMA1_Channel7,ENABLE);											//DMA发送开启3
	}
	else if(USARTx==USART3)
	{
		DMA1_Channel2->CNDTR =BufferSize;										//设定待发送缓冲区大小
		DMA1_Channel2->CMAR =(u32)USART_BUFFER;							//发送缓冲区	
		DMA_Cmd(DMA1_Channel2,ENABLE);											//DMA发送开启3
	}
	else if(USARTx==UART4)
	{
		DMA1_Channel5->CNDTR =BufferSize;										//设定待发送缓冲区大小
		DMA1_Channel5->CMAR =(u32)USART_BUFFER;							//发送缓冲区	
		DMA_Cmd(DMA1_Channel5,ENABLE);											//DMA发送开启3
	}
//	else if(USARTx==UART5)
//	{
//		DMA1_Channel7->CNDTR =BufferSize;										//设定待发送缓冲区大小
//		DMA1_Channel7->CMAR =(u32)USART_BUFFER;							//发送缓冲区	
//		DMA_Cmd(DMA1_Channel7,ENABLE);											//DMA发送开启3
//	}
	
	return BufferSize;

}
/*******************************************************************************
*函数名		:	USART_RX_DMAFlagClear
*功能描述	:	清除串口DMA接收中断标志
						->USART_IT_IDLE:当检测到总线空闲时，该位被硬件置位。如果USART_CR1中的IDLEIE为’1’，则产生中断。由软件序列清除该位(先读USART_SR，然后读USART_DR)。
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
u32 USART_RX_FlagClear(USART_TypeDef* USARTx)				//清除串口接收中断标志
{
//	ErrorStatus status = ERROR;
	u32 num=0;

	if(USARTx==USART1)
	{
		if(USART_GetITStatus(USART1,USART_IT_IDLE))
		{
			USART_ClearITPendingBit(USART1,USART_IT_IDLE); 									//清除空闲串口标志位
			DMA_Cmd(DMA1_Channel5,DISABLE);    														//关闭接收DMA
			num = USART1->DR; 																							//读出数据以完成清除标志			
			num = USART_BUFFERSIZE -  DMA_GetCurrDataCounter(DMA1_Channel5);	//得到真正接收数据个数
//			DMA1_Channel5->CNDTR=10;       										//重新设置接收数据个数	
			DMA1_Channel5->CNDTR=USART_BUFFERSIZE;	//得到真正接收数据个数			
			DMA_Cmd(DMA1_Channel5,ENABLE);  																//开启接收DMA			
			return num;
		}
	}
	else if(USARTx==USART2)
	{
		if(USART_GetITStatus(USART2,USART_IT_IDLE))
		{
			USART_ClearITPendingBit(USART2,USART_IT_IDLE); 									//清除空闲串口标志位
			DMA_ClearFlag(DMA1_FLAG_TC6);
			DMA_Cmd(DMA1_Channel6,DISABLE);			//DMA发送关闭
		//			free(USART_BUFFER);									//释放动态空间
			return num;
		}
	}
	else if(USARTx==USART3)
	{
		if(USART_GetITStatus(USART3,USART_IT_IDLE))
		{
			USART_ClearITPendingBit(USART3,USART_IT_IDLE); 									//清除空闲串口标志位
			DMA_ClearFlag(DMA1_FLAG_TC3);
			DMA_Cmd(DMA1_Channel3,DISABLE);			//DMA发送关闭
		//			free(USART_BUFFER);									//释放动态空间
			return num;
		}
	}
	else if(USARTx==UART4)
	{
		if(USART_GetITStatus(UART4,USART_IT_IDLE))
		{
			USART_ClearITPendingBit(UART4,USART_IT_IDLE); 									//清除空闲串口标志位
			DMA_ClearFlag(DMA2_FLAG_TC3);
			DMA_Cmd(DMA2_Channel3,DISABLE);			//DMA发送关闭
		//			free(USART_BUFFER);									//释放动态空间
			return num;
		}
	}
	else if(USARTx==UART5)
	{
//		if(DMA_GetFlagStatus(DMA1_FLAG_TC4))
//		{
//			DMA_ClearFlag(DMA1_FLAG_TC4);
//			DMA_Cmd(DMA1_Channel4,DISABLE);			//DMA发送关闭
//		//			free(USART_BUFFER);									//释放动态空间
			return num;
	}
	else
	{
		return 0;
	}
	
	return num;
//	if(status==SUCCESS)
//	{
//		free(USART_BUFFER);						//释放动态空间
//	}	
//	return status;			//返回状态值
}
/*******************************************************************************
*函数名		:	USART_TX_DMAFlagClear
*功能描述	:	清除串口DMA发送全局中断标志
						->当传输完成DMA控制器指定的数据量时，DMA控制器在该DMA通道的中断向量上产生一中断。
						->在发送模式下，当DMA传输完所有要发送的数据时，DMA控制器设置DMA_ISR寄存器的TCIF标志；
						->监视USART_SR寄存器的TC标志可以确认USART通信是否结束，这样可以在关闭USART或进入停机模式之前避免破坏最后一次传输的数据；
						->软件需要先等待TXE=1，再等待TC=1。
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
ErrorStatus USART_TX_DMAFlagClear(USART_TypeDef* USARTx)				//清除串口DMA发送全局中断标志
{
	ErrorStatus status = ERROR;
//	status = SUCCESS;
//	if(DMA_GetFlagStatus(DMA1_FLAG_GL4))
//	{
//		DMA_ClearFlag(DMA1_FLAG_GL4);
//		DMA_Cmd(DMA1_Channel4,DISABLE);			//DMA发送关闭
//		free(USART_BUFFER);						//释放动态空间
//	}
	//4)**********根据串口索引相关DMA通道及其它参数
//	u8 DMAx_Channelx_IRQChannel=0;							//DMA中断源
//	u32 DMAx_FLAG_GLtx=0;												//DMA串口发送中断全局变量			
//	u32 DMAx_FLAG_GLrx=0;												//DMA串口接收中断全局变量
	if(USARTx==USART1)
	{
		if(DMA_GetFlagStatus(DMA1_FLAG_TC4))
		{
			DMA_ClearFlag(DMA1_FLAG_TC4);
			DMA_Cmd(DMA1_Channel4,DISABLE);			//DMA发送关闭
		//			free(USART_BUFFER);									//释放动态空间
			status = SUCCESS;
		}
		else
		{
			status = ERROR;
		}
	}
	else if(USARTx==USART2)
	{
		if(DMA_GetFlagStatus(DMA1_FLAG_TC7))
		{
			DMA_ClearFlag(DMA1_FLAG_TC7);
			DMA_Cmd(DMA1_Channel7,DISABLE);			//DMA发送关闭
		//			free(USART_BUFFER);									//释放动态空间
			status = SUCCESS;
		}
		else
		{
			status = ERROR;
		}
	}
	else if(USARTx==USART3)
	{
		if(DMA_GetFlagStatus(DMA1_FLAG_TC2))
		{
			DMA_ClearFlag(DMA1_FLAG_TC2);
			DMA_Cmd(DMA1_Channel2,DISABLE);			//DMA发送关闭
		//			free(USART_BUFFER);									//释放动态空间
			status = SUCCESS;
		}
		else
		{
			status = ERROR;
		}
	}
	else if(USARTx==UART4)
	{
		if(DMA_GetFlagStatus(DMA2_FLAG_TC5))
		{
			DMA_ClearFlag(DMA2_FLAG_TC5);
			DMA_Cmd(DMA2_Channel5,DISABLE);			//DMA发送关闭
		//			free(USART_BUFFER);									//释放动态空间
			status = SUCCESS;
		}
		else
		{
			status = ERROR;
		}
	}
	else if(USARTx==UART5)
	{
//		if(DMA_GetFlagStatus(DMA1_FLAG_TC4))
//		{
//			DMA_ClearFlag(DMA1_FLAG_TC4);
//			DMA_Cmd(DMA1_Channel4,DISABLE);			//DMA发送关闭
//		//			free(USART_BUFFER);									//释放动态空间
//			status = SUCCESS;
//		}
//		else
//		{
			status = ERROR;
//		}
	}
	if(status==SUCCESS)
	{
		free(USART_BUFFER);						//释放动态空间
	}	
	return status;			//返回状态值

}

/*******************************************************************************
* 函数名		:	
* 功能描述	:	printf重定义 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
int fputc(int ch, FILE *f)				//printf重定义
{

	USART_SendData(USART2, (unsigned char) ch);// USART1 可以换成 USART2 等

	while(!(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == SET));

	return (ch);

}

  
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
//int fgetc(FILE *f)
//{
// 
//	while(!(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET))
//	{
//	}
//	return (USART_ReceiveData(USART1));
//}

/*******************************************************************************
*函数名		:USART_RX_Server
*功能描述	:串口接收服务函数
*输入			: 
*输出			:无
*返回值		:无
*例程			：
*******************************************************************************/

void USART_RX_Server(void)		//串口接收服务程序	
{
//	u16 num=0;
	WOW_Server(); 
//	USART_ClearITPendingBit(USART1,USART_IT_IDLE); 				//清除空闲串口标志位
//	USART_ClearITPendingBit(USART1,USART_IT_TC); 					//传输完成中断
//	USART_ClearITPendingBit(USART1,USART_IT_RXNE); 				//清除空闲串口标志位
	
//    if(USART_GetITStatus(USART1,USART_IT_IDLE) == SET)
//    {
////#ifdef	USART1_DMA
////		u16 num=0;
//		USART_ClearITPendingBit(USART1,USART_IT_IDLE); 				//清除空闲串口标志位
////		USART_ClearITPendingBit(USART1,USART_IT_RXNE); 				//清除空闲串口标志位
//		num = USART1->DR; 																		//读出数据以完成清除标志
//		DMA_Cmd(DMA1_Channel5,DISABLE);    										//关闭接收DMA
//		num = DMA1_BufferSize -  DMA_GetCurrDataCounter(DMA1_Channel5);  	//得到真正接收数据个数
//		DMA1_Channel4->CNDTR =num;														//设定待发送数据个数
////		temp =num;   
////		data_uart1[num] = '\0';
//		DMA1_Channel5->CNDTR=DMA1_BufferSize;       				//重新设置接收数据个数
//			
//		DMA_Cmd(DMA1_Channel5,ENABLE);  										//开启接收DMA
//		DMA_Cmd(DMA1_Channel4,ENABLE);											//DMA发送开启
////#endif
//		}
//			
//	if(USART_GetITStatus(USART2,USART_IT_IDLE) == SET)
//    {
//		USART_ClearITPendingBit(USART2,USART_IT_IDLE); 				//清除空闲串口标志位
//		num = USART2->DR; 																		//读出数据以完成清除标志
//		DMA_Cmd(DMA1_Channel6,DISABLE);    										//关闭接收DMA
//		num = DMA1_BufferSize -  DMA_GetCurrDataCounter(DMA1_Channel6);  	//得到真正接收数据个数
//		DMA1_Channel7->CNDTR =num;														//设定待发送数据个数
//		DMA1_Channel6->CNDTR=DMA1_BufferSize;       				//重新设置接收数据个数
//			
//		DMA_Cmd(DMA1_Channel6,ENABLE);  										//开启接收DMA			
//		DMA_Cmd(DMA1_Channel7,ENABLE);											//DMA发送开启
//		}
/************************************************************************
***********************串口2收发****************************************

if(USART_GetITStatus(USART2,USART_IT_IDLE))
  {
			USART_ClearITPendingBit(USART2,USART_IT_IDLE); 					//清除空闲串口标志位
			num = USART2->DR; 												//读出数据以完成清除标志
			DMA_Cmd(DMA1_Channel6,DISABLE);    										//关闭接收DMA
			num = DMA1_BufferSize -  DMA_GetCurrDataCounter(DMA1_Channel6);	//得到真正接收数据个数
			DMA1_Channel7->CNDTR =num;										//设定待发送数据个数
			DMA1_Channel6->CNDTR=DMA1_BufferSize;       					//重新设置接收数据个数				
			DMA_Cmd(DMA1_Channel6,ENABLE);  								//开启接收DMA			
			DMA_Cmd(DMA1_Channel7,ENABLE);									//DMA发送开启
	}
	else if(DMA_GetFlagStatus(DMA1_FLAG_GL7))
	{
		DMA_ClearFlag(DMA1_FLAG_GL7);
		DMA_Cmd(DMA1_Channel7,DISABLE);									//DMA发送开启
	}
*************************************************************************
************************************************************************/
}




