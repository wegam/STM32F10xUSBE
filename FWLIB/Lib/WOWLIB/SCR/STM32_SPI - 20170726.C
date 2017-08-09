/******************************** User_library *********************************
* 文件名 	: STM32_SPI.H
* 作者   	: wegam@sina.com
* 版本   	: V
* 日期   	: 2016/01/01
* 说明   	: 
********************************************************************************
SPI配置
1）使用前需要配置结构体参数 SPI_ConfTypeDef
SPI_ConfTypeDef包含GPIO结构体（4个GPIO管脚），SPI结构体,DMA结构体，接收缓冲区，发送缓冲区，DMA使用与否标志

SPI需要配置基本内容：
1)GPIO
2)SPI
当使用DMA时：
3)DMA


*******************************************************************************/
#include "STM32_SPI.H"
#include "stm32f10x_dma.h"
#include "stm32f10x_exti.h"
#include "STM32_EXTI.H"

#include "STM32_WOW.H"
//#include "STM32F10x_BitBand.H"

char *SPI_BUFFER;
u32	SPI_BUFFERSIZE;
//#define	SPI_BUFFERSIZE	4

//char SPI_TX_Buffer[4]={0X9F,0X9F,0X9F,0X9F};
//char SPI_RX_Buffer[4]={0};
/*******************************************************************************
*函数名		:	SPI_Configuration
*功能描述	:	串口配置
*输入			:	MemoryAddr--内存地址,
						BufferSize--缓存大小
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SPI_BASIC_Configuration(SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct)							//SPI常规使用方式配置
{
	//1)**********SPI管脚配置及初始化
	SPI_PinSet(SPIx,SPI_InitStruct);																//SPI管脚配置初始化
	//2)**********判断SPI主机是否开启SPI_NSS硬件使能输出	
	//使能SPIx
	SPI_Cmd(SPIx, ENABLE); 
}
void SPI_Server(void)
{
	WOW_Server(); 
}
/*******************************************************************************
*函数名		:	SPI_DMA_Configuration
*功能描述	:	函数功能说明
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SPI_DMA_Configuration(SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct,u32* SPI_TX_Buffer,u32* SPI_RX_Buffer,u32 SPI_BUFFERSIZE)		//SPI_DMA方式配置
{
/**-----------------------------------------------------------------------------------------------------
	********SPI_DMA的通信过程********
	● 设置外设地址
	● 设置存储器地址
	● 设置传输数据量
	● 设置通道的配置信息
	● 使能DMA通道，启动传输
	
	● 发送时，在每次TXE被设置为’1’时发出DMA请求，DMA控制器则写数据至SPI_DR寄存器，TXE标志因此而被清除。
	● 接收时，在每次RXNE被设置为’1’时发出DMA请求，DMA控制器则从SPI_DR寄存器读出数据，RXNE标志因此而被清除。
-----------------------------------------------------------------------------------------------------**/
	//1)**********定义相关结构体
	DMA_InitTypeDef	DMA_Initstructure;
//	NVIC_InitTypeDef NVIC_Initstructure;
	
//	u32 *SPI_RX_Buffer;
//	u32 *SPI_TX_Buffer;
//	u32 SPI_BUFFERSIZE=5;

	DMA_Channel_TypeDef* DMAx_Channeltx=0;			//DMA发送通道请求信号---当DMA串口发送数据完成时，会发起DMA中断
	DMA_Channel_TypeDef* DMAx_Channelrx=0;			//DMA接收通道请求信号---DMA串口接收由串口发起中断，因此此处接收通道中断不使用
//	u8 DMAx_Channelx_IRQChannel=0;							//DMA中断源
	u8 Conf_Flag=0;															//需要配置标志，如果SPIx合法，则Conf_Flag==1，然后进行下一步DMA配置项
//	u32 DMAx_FLAG_GLtx=0;												//DMA串口发送中断全局变量			
//	u32 DMAx_FLAG_GLrx=0;												//DMA串口接收中断全局变量
	
//	SPI_BUFFERSIZE=BufferSize;
	
//	assert_param(IS_SPI_ALL_PERIPH(SPI5));
	
	//2)**********SPI管脚配置及初始化
	SPI_PinSet(SPIx,SPI_InitStruct);																		//SPI管脚配置初始化
	
	//3)**********SPI通道选择
	if(SPIx==SPI1)
	{
		DMAx_Channeltx=DMA1_Channel3;
		DMAx_Channelrx=DMA1_Channel2;
//		DMAx_Channelx_IRQChannel=DMA1_Channel3_IRQChannel;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
		Conf_Flag=1;																			//需要配置标志，如果SPIx合法，则Conf_Flag==1，然后进行下一步DMA配置项
	}
	else if(SPIx==SPI2)
	{
		DMAx_Channeltx=DMA1_Channel5;
		DMAx_Channelrx=DMA1_Channel4;
//		DMAx_Channelx_IRQChannel=DMA1_Channel5_IRQChannel;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
		Conf_Flag=1;																			//需要配置标志，如果SPIx合法，则Conf_Flag==1，然后进行下一步DMA配置项		
	}
	else if(SPIx==SPI3)
	{
		DMAx_Channeltx=DMA2_Channel2;
		DMAx_Channelrx=DMA2_Channel1;
//		DMAx_Channelx_IRQChannel=DMA2_Channel2_IRQChannel;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);
		Conf_Flag=1;																			//需要配置标志，如果SPIx合法，则Conf_Flag==1，然后进行下一步DMA配置项
	}
	else
	{
		Conf_Flag=0;																			//需要配置标志，如果SPIx合法，则Conf_Flag==1，然后进行下一步DMA配置项
	}
	
	//4)**********SPI_DMA配置
	if(Conf_Flag==1)																		//需要配置标志，如果SPIx合法，则Conf_Flag==1，然后进行下一步DMA配置项
	{
		//5)**********DMA发送初始化，外设作为DMA的目的端
		DMA_Initstructure.DMA_PeripheralBaseAddr =  (u32)(&SPIx->DR);						//DMA外设源地址
		DMA_Initstructure.DMA_MemoryBaseAddr     = (u32)SPI_TX_Buffer;							//DMA数据内存地址
		DMA_Initstructure.DMA_DIR = DMA_DIR_PeripheralDST;											//DMA_DIR_PeripheralDST（外设作为DMA的目的端），DMA_DIR_PeripheralSRC（外设作为数据传输的来源）
		DMA_Initstructure.DMA_BufferSize = SPI_BUFFERSIZE; 											//指定DMA通道的DMA缓存的大小
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
		DMA_Initstructure.DMA_PeripheralBaseAddr =  (u32)(&SPIx->DR);					//DMA外设源地址
		DMA_Initstructure.DMA_MemoryBaseAddr     = 	(u32)SPI_RX_Buffer;						//DMA数据内存地址
		DMA_Initstructure.DMA_DIR = DMA_DIR_PeripheralSRC;											//DMA_DIR_PeripheralDST（外设作为DMA的目的端），DMA_DIR_PeripheralSRC（外设作为数据传输的来源）
		DMA_Initstructure.DMA_BufferSize = SPI_BUFFERSIZE; 													//指定DMA通道的DMA缓存的大小
	//	DMA_Initstructure.DMA_BufferSize = sizeof(MemoryAddr); 								//指定DMA通道的DMA缓存的大小
		DMA_Initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//DMA_PeripheralInc_Enable（外设地址寄存器递增），DMA_PeripheralInc_Disable（外设地址寄存器不变），
		DMA_Initstructure.DMA_MemoryInc =DMA_MemoryInc_Enable;									//DMA_MemoryInc_Enable（内存地址寄存器递增），DMA_MemoryInc_Disable（内存地址寄存器不变）
		DMA_Initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//外设数据宽度--DMA_PeripheralDataSize_Byte（数据宽度为8位），DMA_PeripheralDataSize_HalfWord（数据宽度为16位），DMA_PeripheralDataSize_Word（数据宽度为32位）
		DMA_Initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//内存数据宽度--DMA_MemoryDataSize_Byte（数据宽度为8位），DMA_MemoryDataSize_HalfWord（数据宽度为16位），DMA_MemoryDataSize_Word（数据宽度为32位）
		DMA_Initstructure.DMA_Mode = DMA_Mode_Normal;														//DMA工作模式--DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular（不停地传送）
		DMA_Initstructure.DMA_Priority = DMA_Priority_High; 										//DMA通道的转输优先级--DMA_Priority_VeryHigh（非常高）DMA_Priority_High（高)，DMA_Priority_Medium（中），DMA_Priority_Low（低）
		DMA_Initstructure.DMA_M2M = DMA_M2M_Disable;														//DMA通道的内存到内存传输--DMA_M2M_Enable(设置为内存到内存传输)，DMA_M2M_Disable（非内存到内存传输）
		DMA_Init(DMAx_Channelrx,&DMA_Initstructure);														//初始化DMA
		
//		//7)**********DMA通道中断初始化---此为DMA发送中断----DMA发送完成中断
//		NVIC_Initstructure.NVIC_IRQChannel = DMA1_Channel4_IRQChannel;       		//通道设置为串口1中断
//		NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority=1;
//		NVIC_Initstructure.NVIC_IRQChannelSubPriority = 1; 			//中断响应优先级0	
//		NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;        									//打开中断
//		NVIC_Init(&NVIC_Initstructure);
//		
//		DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);	//使能或者失能指定的通道x中断
//		DMA_Cmd(DMA1_Channel4, ENABLE);
			
		SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, ENABLE);
		SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, ENABLE);
		
		
		
		
		//使能SPIx
		SPI_Cmd(SPIx, ENABLE);
		
		//9.2)**********使能相关DMA通道传输完成中断
//		DMA_ITConfig(DMAx_Channeltx,DMA_IT_TC, ENABLE);
			/* 启动DMA1通道5*/
		DMA_Cmd(DMAx_Channelrx,DISABLE);	
		DMA_Cmd(DMAx_Channeltx,DISABLE);
		

		//9.2)**********使能相关DMA通道传输完成中断
//		DMA_ITConfig(DMAx_Channeltx,DMA_IT_TC, ENABLE);
	}
		
//	NVIC_Initstructure.NVIC_IRQChannel = SPI_I2S_IT_TXE;       		//通道设置为串口1中断
//	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority=1;
//	NVIC_Initstructure.NVIC_IRQChannelSubPriority = 1; 			//中断响应优先级0	
//	NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;        									//打开中断
//	NVIC_Init(&NVIC_Initstructure);
//	
//	SPI_I2S_ITConfig(SPIx,SPI_I2S_IT_TXE,ENABLE);
	
	//使能SPIx
	SPI_Cmd(SPIx, DISABLE);
}
/*******************************************************************************
*函数名		:	SPI_PinSet
*功能描述	:	SPI管脚配置初始化
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SPI_PinSet(SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct)													//SPI管脚配置及初始化
{
	//1)**********定义相关结构体
//	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//2)**********相关GPIO配置
	if(SPIx==SPI1)
	{
		//2.1)**********打开SPI时钟	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA, ENABLE);		
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		if(SPI_InitStruct->SPI_NSS == SPI_NSS_Soft)			//如果SPI_NSS为SPI_NSS_Soft（软件控制方式）
		{
			
			//2.2)**********SPI_NSS_Hard配置
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		}
	}
	else if(SPIx==SPI2)
	{
		//2.2)**********打开SPI时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 ,ENABLE);			
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
		
		if(SPI_InitStruct->SPI_NSS == SPI_NSS_Soft)			//如果SPI_NSS为SPI_NSS_Soft（软件控制方式）
		{
			//2.2)**********SPI_NSS配置
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
			GPIO_Init(GPIOB, &GPIO_InitStructure);
		}
	}
		else if(SPIx==SPI3)
	{
		//2.2)**********打开SPI时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3 ,ENABLE);			
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		if(SPI_InitStruct->SPI_NSS == SPI_NSS_Soft)			//如果SPI_NSS为SPI_NSS_Soft（软件控制方式）
		{			
			//2.2)**********SPI_NSS配置		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		}
		else	
		{			
			//2.2)**********SPI_NSS配置
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		}			
	}
	//3)**********SPI配置选项
//	SPI_InitStruct->SPI_Direction = SPI_Direction_2Lines_FullDuplex;				//设置方向				（2线全双工、2线只接收、一线发送、一线接收）
//	SPI_InitStruct->SPI_Mode = SPI_Mode_Master;															//模式         	（从或主设备）
//	SPI_InitStruct->SPI_DataSize = SPI_DataSize_8b;													//宽度         	（8或16位）
//	SPI_InitStruct->SPI_CPOL = SPI_CPOL_High;																//时钟极性     	（低或高）
//	SPI_InitStruct->SPI_CPHA = SPI_CPHA_2Edge;															//时钟相位     	（第一个或第二个跳变沿）
////	SPI_InitStructure->SPI_NSS = SPI_NSS_Soft;																	//片选方式     	（硬件或软件方式）
//	SPI_InitStruct->SPI_NSS = SPI_NSS_Hard;																	//片选方式     	（硬件或软件方式）
//	SPI_InitStruct->SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;				//波特率预分频 	（从2---256分频）
//	SPI_InitStruct->SPI_FirstBit = SPI_FirstBit_MSB;												//最先发送的位 	（最低位，还是最高位在先）
//	SPI_InitStruct->SPI_CRCPolynomial = 7;																	//设置crc多项式	（数字）如7
	SPI_Init(SPIx, SPI_InitStruct);	
	
	//3)**********使能SPIx_NESS为主输出模式
	if((SPIx->CR1&0X0200)!=SPI_NSS_Soft)						//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
	{
		SPI_SSOutputCmd(SPIx, ENABLE);
	}

}
/*******************************************************************************
*函数名		: function
*功能描述	:	SPI_DMA发送函数
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:	USART_DMASend(USART2,"中文ENG=%d\n",num);
*特别说明	:	在DMA发送完成后需要释放动态空间，free(USART_BUFFER);
					:	USART_BUFFER定义在STM32_USART.H
*******************************************************************************/
int SPI_DMAPrintf(SPI_TypeDef* SPIx,const char *format,...)		//SPI_DMA发送函数----后边的省略号就是可变参数
{
	
//		va_list ap; 										//VA_LIST 是在C语言中解决变参问题的一组宏，所在头文件：#include <stdarg.h>,用于获取不确定个数的参数
//    static char string[ 256 ];			//定义数组，
//    va_start( ap, format );
//    vsprintf( string , format, ap );    
//    va_end( ap );
//	char *SPI_BUFFER;
	//1)**********获取数据宽度
	u16 num=strlen((const char*)format);		//获取数据宽度
	//2)**********定义缓冲区大小变量
	unsigned int BufferSize;
	//3)**********args为定义的一个指向可变参数的变量，va_list以及下边要用到的va_start,va_end都是是在定义，可变参数函数中必须要用到宏， 在stdarg.h头文件中定义
	va_list args;                                        
	//4)**********申请动态空间
	SPI_BUFFER = (char*)malloc(sizeof(char) * num);	
	//5)**********初始化args的函数，使其指向可变参数的第一个参数，format是可变参数的前一个参数
	va_start(args, format);
	//6)**********正常情况下返回生成字串的长度(除去\0),错误情况返回负值
	BufferSize = vsprintf(SPI_BUFFER, format, args);
	//7)**********结束可变参数的获取
	va_end(args);                                      		
	//8)**********将等发送缓冲区大小（数据个数）及缓冲区地址发给DMA开启发送
	//8)**********DMA发送完成后注意应该释放缓冲区：free(USART_BUFFER);
	if(SPIx==SPI1)
	{
		DMA1_Channel3->CNDTR =BufferSize;										//设定待发送缓冲区大小
		DMA1_Channel3->CMAR =(u32)SPI_BUFFER;							//发送缓冲区	
		DMA_Cmd(DMA1_Channel3,ENABLE);											//DMA发送开启3
	}
	else if(SPIx==SPI2)
	{
		DMA1_Channel5->CNDTR =BufferSize;										//设定待发送缓冲区大小
		DMA1_Channel5->CMAR =(u32)SPI_BUFFER;							//发送缓冲区	
		DMA_Cmd(DMA1_Channel5,ENABLE);											//DMA发送开启3
	}
	else if(SPIx==SPI3)
	{
		DMA2_Channel2->CNDTR =BufferSize;										//设定待发送缓冲区大小
		DMA2_Channel2->CMAR =(u32)SPI_BUFFER;							//发送缓冲区	
		DMA_Cmd(DMA2_Channel2,ENABLE);											//DMA发送开启3
	}
	return BufferSize;
}
/*******************************************************************************
*函数名		: function
*功能描述	:	SPI_DMA发送函数
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:	USART_DMASend(USART2,"中文ENG=%d\n",num);
*特别说明	:	在DMA发送完成后需要释放动态空间，free(USART_BUFFER);
					:	USART_BUFFER定义在STM32_USART.H
*******************************************************************************/
int SPI_DMA_BufferWrite(SPI_TypeDef* SPIx,u32 *SPI_TX_Buffer,u16 BufferSize)		//SPI_DMA发送函数----后边的省略号就是可变参数
{              		
	//8)**********将等发送缓冲区大小（数据个数）及缓冲区地址发给DMA开启发送
	//8)**********DMA发送完成后注意应该释放缓冲区：free(USART_BUFFER);
	if(SPIx==SPI1)
	{
		DMA1_Channel3->CNDTR =BufferSize;										//设定待发送缓冲区大小
		DMA1_Channel3->CMAR =(u32)SPI_TX_Buffer;						//发送缓冲区	
//		DMA1_Channel2->CNDTR =BufferSize;										//设定待发送缓冲区大小
//		DMA1_Channel2->CMAR =(u32)SPI_RX_Buffer;						//发送缓冲区	
//		DMA_Cmd(DMA1_Channel2,ENABLE);
		DMA_Cmd(DMA1_Channel3,ENABLE);											//DMA发送开启3
	}
	else if(SPIx==SPI2)
	{
		DMA1_Channel5->CNDTR =BufferSize;										//设定待发送缓冲区大小
		DMA1_Channel5->CMAR =(u32)SPI_TX_Buffer;							//发送缓冲区	
		DMA_Cmd(DMA1_Channel5,ENABLE);											//DMA发送开启3
	}
	else if(SPIx==SPI3)
	{
		DMA1_Channel2->CNDTR =BufferSize;										//设定待发送缓冲区大小
		DMA1_Channel2->CMAR =(u32)SPI_TX_Buffer;							//发送缓冲区	
		DMA_Cmd(DMA2_Channel2,ENABLE);											//DMA发送开启3
	}
	return BufferSize;
}
/*******************************************************************************
*函数名		: function
*功能描述	:	SPI_DMA_BufferRead发送函数
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:	USART_DMASend(USART2,"中文ENG=%d\n",num);
*特别说明	:	在DMA发送完成后需要释放动态空间，free(USART_BUFFER);
					:	USART_BUFFER定义在STM32_USART.H
*******************************************************************************/
void SPI_DMA_BufferRead(u32 *ReadAddr)
{
	SPI_InitTypeDef  SPI_InitStructure;
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	/* Send "Read from Memory " instruction */
	SPI_I2S_SendData(SPI1,0X9F);
	
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_RxOnly;
	SPI_Init(SPI1, &SPI_InitStructure);

	DMA1_Channel2->CNDTR =20;						//发送缓冲区
	DMA1_Channel2->CMAR =(u32)ReadAddr;						//发送缓冲区

	//SPI_FLASH_SendByte(0xff);
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);

	while(DMA_GetFlagStatus(DMA1_FLAG_TC2) == RESET);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
}
/*******************************************************************************
*函数名		:	SPI_RX_FlagClear
*功能描述	:	函数功能说明
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
u32 SPI_RX_FlagClear(SPI_TypeDef* SPIx)											//SPI DMA配置及初始化
{
	u32 num=0;
	if(SPIx==SPI1)
	{
	}
	else if(SPIx==SPI2)
	{
	}
	else if(SPIx==SPI3)
	{
	}
	else
	{
		num=0;
	}
	return num;
}
/*******************************************************************************
*函数名		:	SPI_TX_DMAFlagClear
*功能描述	:	清除SPI_DMA发送完成中断标志
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
ErrorStatus SPI_TX_DMAFlagClear(SPI_TypeDef* SPIx)					//清除SPI_DMA发送完成中断标志
{
	ErrorStatus status = ERROR;
	if(SPIx==SPI1)
	{
		if(DMA_GetITStatus(DMA1_IT_TC3))
		{
			DMA_ClearFlag(DMA1_IT_TC3);
			DMA_Cmd(DMA1_Channel3,DISABLE);			//DMA发送关闭
			status = SUCCESS;
		}
		else
		{
			status = ERROR;
		}
	}
	else if(SPIx==SPI2)
	{
		if(DMA_GetITStatus(DMA1_IT_TC5))
		{
			DMA_ClearFlag(DMA1_IT_TC5);
			DMA_Cmd(DMA1_Channel5,DISABLE);			//DMA发送关闭
			status = SUCCESS;
		}
		else
		{
			status = ERROR;
		}
	}
	else if(SPIx==SPI3)
	{
		if(DMA_GetITStatus(DMA2_IT_TC2))
		{
			DMA_ClearFlag(DMA2_IT_TC2);
			DMA_Cmd(DMA2_Channel2,DISABLE);			//DMA发送关闭
			status = SUCCESS;
		}
		else
		{
			status = ERROR;
		}
	}
	else
	{
		status = ERROR;
	}
	return status;
}
/*******************************************************************************
*函数名		:	
*功能描述	:	
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
u8 SPI_SendByte(u8 byte)
{
  /* 等待DR寄存器空 */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(SPI1, byte);

  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI1);
}

/*********************************************************************************************************************/
/*******************************************************************************
*函数名		:	
*功能描述	:	
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
//unsigned char SPI_Read(unsigned char* data,unsigned char bytesNumber)
// {
//     unsigned char byte = 0;
//     for(byte = 0; byte < bytesNumber; byte++)
//     {
//				SPI_Buffer_Tx[byte]=0;
//     }
//     GPIO_ResetBits(GPIOB,GPIO_Pin_12);
//     SPI2_ReceiveSendByte(bytesNumber);
//     GPIO_SetBits(GPIOB, GPIO_Pin_12);
//     for(byte = 0; byte < bytesNumber; byte++)
//     {
//				data[byte]=SPI_Buffer_Rx[byte];
//     }
//     return bytesNumber;
// }
/*******************************************************************************
*函数名		:	
*功能描述	:	
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
 void SPI_DMA_ReceiveSendByte(SPI_TypeDef* SPIx,u16 num)			//DMA收发数据
 {
	SPI_Cmd(SPIx, ENABLE);
	SPI_DMA_Conf(SPIx,num);											//设置SPI_DMA：设置缓存大小及清除DMA标志

	SPIx->DR ;             											//接送前读一次SPI1->DR，保证接收缓冲区为空

	while((SPIx->SR&1<<1)==0);										//TXE：等待发送缓冲为空 (Transmit buffer empty)
	SPI_DMA_Cmd(SPIx,ENABLE);										//SPI_DMA使能或者失能---开启DMA

//     while( !DMA_GetFlagStatus(DMA1_FLAG_TC4));		//等待接收完成
	while(SPI_DMA_TX_Flag(SPI1)==0);					//查询SPI_DMA接收完成标志;0--未完成，1--完成
	 
	SPI_DMA_Cmd(SPIx,DISABLE);										//SPI_DMA使能或者失能---关闭DMA
	SPI_Cmd(SPIx, DISABLE);
 }
/*******************************************************************************
*函数名		:	SPI_DMA_Conf
*功能描述	:	设置SPI_DMA：设置缓存大小及清除DMA标志
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SPI_DMA_Conf(SPI_TypeDef* SPIx,u16 num)		//设置SPI_DMA：设置缓存大小及清除DMA标志
{
	if(SPIx==SPI1)
	{
		//1)**********设置缓存大小
		DMA1_Channel3->CNDTR =num;										//设定待发送缓冲区大小
		DMA1_Channel2->CNDTR =num;										//设定待接收缓冲区大小
		//2)**********清除DMA标志
		DMA_ClearFlag(DMA1_FLAG_GL3|DMA1_FLAG_TC3|DMA1_FLAG_HT3|DMA1_FLAG_TE3);		//清除DMA发送标志
		DMA_ClearFlag(DMA1_FLAG_GL2|DMA1_FLAG_TC2|DMA1_FLAG_HT2|DMA1_FLAG_TE2);		//清除DMA接收标志
	}
	else if(SPIx==SPI2)
	{
		//1)**********设置缓存大小
		DMA1_Channel5->CNDTR =num;										//设定待发送缓冲区大小
		DMA1_Channel4->CNDTR =num;										//设定待接收缓冲区大小
		//2)**********清除DMA标志
		DMA_ClearFlag(DMA1_FLAG_GL5|DMA1_FLAG_TC5|DMA1_FLAG_HT5|DMA1_FLAG_TE5);		//清除DMA发送标志
		DMA_ClearFlag(DMA1_FLAG_GL4|DMA1_FLAG_TC4|DMA1_FLAG_HT4|DMA1_FLAG_TE4);		//清除DMA接收标志
	}
	else if(SPIx==SPI3)
	{
		//1)**********设置缓存大小
		DMA2_Channel2->CNDTR =num;										//设定待发送缓冲区大小
		DMA2_Channel1->CNDTR =num;										//设定待接收缓冲区大小
		//2)**********清除DMA标志
		DMA_ClearFlag(DMA2_FLAG_GL2|DMA2_FLAG_TC2|DMA2_FLAG_HT2|DMA2_FLAG_TE2);		//清除DMA发送标志
		DMA_ClearFlag(DMA2_FLAG_GL1|DMA2_FLAG_TC1|DMA2_FLAG_HT1|DMA2_FLAG_TE1);		//清除DMA接收标志
	}
}
/*******************************************************************************
*函数名		:	function
*功能描述	:	SPI_DMA使能或者失能
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SPI_DMA_Cmd(SPI_TypeDef* SPIx,FunctionalState NewState)		//SPI_DMA使能或者失能
{
	//1)使能SPI_DMA通道
	if (NewState != DISABLE)
	{
		if(SPIx==SPI1)
		{
			DMA_Cmd(DMA1_Channel3, ENABLE);
			DMA_Cmd(DMA1_Channel2, ENABLE);
		}
		else if(SPIx==SPI2)
		{
			DMA_Cmd(DMA1_Channel5, ENABLE);
			DMA_Cmd(DMA1_Channel4, ENABLE);
		}
		else if(SPIx==SPI3)
		{
			DMA_Cmd(DMA2_Channel2, ENABLE);
			DMA_Cmd(DMA2_Channel1, ENABLE);
		}
	}
	//2)关闭SPI_DMA通道
	else
	{
		if(SPIx==SPI1)
		{
			DMA_Cmd(DMA1_Channel3, DISABLE);
			DMA_Cmd(DMA1_Channel2, DISABLE);
		}
		else if(SPIx==SPI2)
		{
			DMA_Cmd(DMA1_Channel5, DISABLE);
			DMA_Cmd(DMA1_Channel4, DISABLE);
		}
		else if(SPIx==SPI3)
		{
			DMA_Cmd(DMA2_Channel2, DISABLE);
			DMA_Cmd(DMA2_Channel1, DISABLE);
		}
	}
}
/*******************************************************************************
*函数名		:	SPI_DMA_TX_Flag
*功能描述	:	查询SPI_DMA接收完成标志
*输入			: 
*输出			:	无
*返回值		:	0--未完成，1--完成
*例程			:
*******************************************************************************/
unsigned char SPI_DMA_TX_Flag(SPI_TypeDef* SPIx)		//查询SPI_DMA接收完成标志
{
	u8 DMA_TX_Complete_Flag=0;							//DMA通道传输完成标志变量，0--未完成，1--完成
	if(SPIx==SPI1)
	{
		if(DMA_GetFlagStatus(DMA1_FLAG_TC2)!=RESET)		//传输完成
		{
			DMA_TX_Complete_Flag=1;							//DMA通道传输完成标志变量，0--未完成，1--完成
			
			//2)**********清除DMA标志
		DMA_ClearFlag(DMA1_FLAG_GL3|DMA1_FLAG_TC3|DMA1_FLAG_HT3|DMA1_FLAG_TE3);		//清除DMA发送标志
		DMA_ClearFlag(DMA1_FLAG_GL2|DMA1_FLAG_TC2|DMA1_FLAG_HT2|DMA1_FLAG_TE2);		//清除DMA接收标志
			
		}
		else		//传输未完成
		{
			DMA_TX_Complete_Flag=0;							//DMA通道传输完成标志变量，0--未完成，1--完成
		}
	}
	else if(SPIx==SPI2)
	{
		if(DMA_GetFlagStatus(DMA1_FLAG_TC4)!=RESET)
		{
			DMA_TX_Complete_Flag=1;							//DMA通道传输完成标志变量，0--未完成，1--完成
			
			//2)**********清除DMA标志
		DMA_ClearFlag(DMA1_FLAG_GL5|DMA1_FLAG_TC5|DMA1_FLAG_HT5|DMA1_FLAG_TE5);		//清除DMA发送标志
		DMA_ClearFlag(DMA1_FLAG_GL4|DMA1_FLAG_TC4|DMA1_FLAG_HT4|DMA1_FLAG_TE4);		//清除DMA接收标志
		}
		else		//传输未完成
		{
			DMA_TX_Complete_Flag=0;							//DMA通道传输完成标志变量，0--未完成，1--完成
		}
	}
	else if(SPIx==SPI3)
	{
		if(DMA_GetFlagStatus(DMA2_FLAG_TC1)!=RESET)
		{
			DMA_TX_Complete_Flag=1;							//DMA通道传输完成标志变量，0--未完成，1--完成
			
			//2)**********清除DMA标志
		DMA_ClearFlag(DMA2_FLAG_GL2|DMA2_FLAG_TC2|DMA2_FLAG_HT2|DMA2_FLAG_TE2);		//清除DMA发送标志
		DMA_ClearFlag(DMA2_FLAG_GL1|DMA2_FLAG_TC1|DMA2_FLAG_HT1|DMA2_FLAG_TE1);		//清除DMA接收标志
		}
		else		//传输未完成
		{
			DMA_TX_Complete_Flag=0;							//DMA通道传输完成标志变量，0--未完成，1--完成
		}
	}
	return	DMA_TX_Complete_Flag;							//DMA通道传输完成标志变量，0--未完成，1--完成

}

void SPI2_Conf_TEST(u32* SPI_TX_Buffer,u32* SPI_RX_Buffer,u32 SPI_BUFFERSIZE)
{
	DMA_InitTypeDef  DMA_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	NVIC_InitTypeDef NVIC_Initstructure;
	EXTI_InitTypeDef EXTI_Initstructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 

	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;			//设置方向				（2线全双工、2线只接收、一线发送、一线接收）
//	SPI_InitStructure.SPI_Direction=SPI_Direction_1Line_Rx;			//设置方向				（只读）
//	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;														//模式         	（从或主设备）
	SPI_InitStructure.SPI_Mode=SPI_Mode_Slave;													//模式         	（从或主设备）
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;												//宽度         	（8或16位）
//	SPI_InitStructure.SPI_CPOL=SPI_CPOL_High;															//时钟极性     	（低或高）
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low;															//时钟极性     	（低或高）
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;														//时钟相位     	（第一个或第二个跳变沿）
//	SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;														//时钟相位     	（第一个或第二个跳变沿）
//	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;															//片选方式     	（硬件或软件方式）软件控制
	SPI_InitStructure.SPI_NSS=SPI_NSS_Hard;																//片选方式     	（硬件或软件方式）硬件控制
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_4;		//波特率预分频 	（从2---256分频）
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;											//最先发送的位 	（最低位，还是最高位在先）
	SPI_InitStructure.SPI_CRCPolynomial=0X07;															//设置crc多项式	（其复位值为0x0007，根据应用可以设置其他数值。）
	
	SPI_Init(SPI2, &SPI_InitStructure);
	
	
	SPI_Cmd(SPI2, ENABLE);

	SPI_PinSet(SPI2,&SPI_InitStructure);	
	
	
	
	//5)**********DMA发送初始化，外设作为DMA的目的端
	DMA_InitStructure.DMA_PeripheralBaseAddr =  (u32)(&SPI2->DR);						//DMA外设源地址
	DMA_InitStructure.DMA_MemoryBaseAddr     = (u32)SPI_TX_Buffer;							//DMA数据内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;											//DMA_DIR_PeripheralDST（外设作为DMA的目的端），DMA_DIR_PeripheralSRC（外设作为数据传输的来源）
	DMA_InitStructure.DMA_BufferSize = SPI_BUFFERSIZE; 													//指定DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//DMA_PeripheralInc_Enable（外设地址寄存器递增），DMA_PeripheralInc_Disable（外设地址寄存器不变），
	DMA_InitStructure.DMA_MemoryInc =DMA_MemoryInc_Enable;									//DMA_MemoryInc_Enable（内存地址寄存器递增），DMA_MemoryInc_Disable（内存地址寄存器不变）
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//外设数据宽度--DMA_PeripheralDataSize_Byte（数据宽度为8位），DMA_PeripheralDataSize_HalfWord（数据宽度为16位），DMA_PeripheralDataSize_Word（数据宽度为32位）
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//内存数据宽度--DMA_MemoryDataSize_Byte（数据宽度为8位），DMA_MemoryDataSize_HalfWord（数据宽度为16位），DMA_MemoryDataSize_Word（数据宽度为32位）
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;														//DMA工作模式--DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular（不停地传送）
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; 										//DMA通道的转输优先级--DMA_Priority_VeryHigh（非常高）DMA_Priority_High（高)，DMA_Priority_Medium（中），DMA_Priority_Low（低）
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;														//DMA通道的内存到内存传输--DMA_M2M_Enable(设置为内存到内存传输)，DMA_M2M_Disable（非内存到内存传输）
	DMA_Init(DMA1_Channel5,&DMA_InitStructure);														//初始化DMA
		
//6)**********DMA接收初始化，外设作为DMA的源端--SPI接收
	DMA_InitStructure.DMA_PeripheralBaseAddr =  (u32)&SPI2->DR;					//DMA外设源地址
	DMA_InitStructure.DMA_MemoryBaseAddr     = 	(u32)SPI_RX_Buffer;						//DMA数据内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;											//DMA_DIR_PeripheralDST（外设作为DMA的目的端），DMA_DIR_PeripheralSRC（外设作为数据传输的来源）
	DMA_InitStructure.DMA_BufferSize = SPI_BUFFERSIZE; 													//指定DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//DMA_PeripheralInc_Enable（外设地址寄存器递增），DMA_PeripheralInc_Disable（外设地址寄存器不变），
	DMA_InitStructure.DMA_MemoryInc =DMA_MemoryInc_Enable;									//DMA_MemoryInc_Enable（内存地址寄存器递增），DMA_MemoryInc_Disable（内存地址寄存器不变）
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//外设数据宽度--DMA_PeripheralDataSize_Byte（数据宽度为8位），DMA_PeripheralDataSize_HalfWord（数据宽度为16位），DMA_PeripheralDataSize_Word（数据宽度为32位）
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//内存数据宽度--DMA_MemoryDataSize_Byte（数据宽度为8位），DMA_MemoryDataSize_HalfWord（数据宽度为16位），DMA_MemoryDataSize_Word（数据宽度为32位）
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;														//DMA工作模式--DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular（不停地传送）
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; 										//DMA通道的转输优先级--DMA_Priority_VeryHigh（非常高）DMA_Priority_High（高)，DMA_Priority_Medium（中），DMA_Priority_Low（低）
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;														//DMA通道的内存到内存传输--DMA_M2M_Enable(设置为内存到内存传输)，DMA_M2M_Disable（非内存到内存传输）
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);														//初始化DMA


//	//7)**********DMA通道中断初始化---此为DMA发送中断----DMA发送完成中断
//	NVIC_Initstructure.NVIC_IRQChannel = SPI2_IRQChannel;       		//通道设置为串口1中断
//	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority=1;
//	NVIC_Initstructure.NVIC_IRQChannelSubPriority = 1; 			//中断响应优先级0	
//	NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;        									//打开中断
//	NVIC_Init(&NVIC_Initstructure);
//	SPI_I2S_ITConfig(SPI2,SPI_I2S_IT_RXNE, ENABLE);			//接收完成中断

	
	//NSS上升沿中断************************************************************************
	EXTI_Configuration(GPIOB,GPIO_Pin_12,EXTI_Mode_Interrupt);
	
	EXTI_Initstructure.EXTI_Trigger=EXTI_Trigger_Rising;					//触发方式-
	EXTI_Initstructure.EXTI_LineCmd=ENABLE;												//连接使能
	EXTI_Init(&EXTI_Initstructure);
	
	EXTI_GenerateSWInterrupt(EXTI_Line12);													//使能中断


	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource12);

	
	EXTI_Initstructure.EXTI_Line=EXTI_Line12;												//外部中断线路
	EXTI_Initstructure.EXTI_Mode=EXTI_Mode_Interrupt;						//中断模式
//	EXTI_Initstructure.EXTI_Mode=EXTI_Mode_Event;						//中断模式
	EXTI_Initstructure.EXTI_Trigger=EXTI_Trigger_Rising;					//触发方式-
	EXTI_Initstructure.EXTI_LineCmd=ENABLE;												//连接使能
	EXTI_Init(&EXTI_Initstructure);
	
	EXTI_GenerateSWInterrupt(EXTI_Line12);													//使能中断
	
	//7)**********配置中断
	NVIC_Initstructure.NVIC_IRQChannel = EXTI15_10_IRQChannel;					//选择中断通道-中断源
	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority = 0;			//抢占优先级
	NVIC_Initstructure.NVIC_IRQChannelSubPriority = 0;						//响应优先级
	NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;								//使能
	NVIC_Init(&NVIC_Initstructure);	

	EXTI_GenerateSWInterrupt(EXTI_Line12);													//使能中断
	EXTI_ClearITPendingBit(EXTI_Line12);
	//NSS上升沿中断************************************************************************	
	
	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);	
	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx, ENABLE);
	
	DMA_Cmd(DMA1_Channel5,ENABLE);			//SPI-TX
	DMA_Cmd(DMA1_Channel4,ENABLE);			//SPI-RX
	
	SPI_Cmd(SPI2, ENABLE);
	
	

}



/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void SPI_Configuratio(SPI_TypeDef* SPIx)
{

}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void SPI_General_Configuratio(void)
{

}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void SPI_GPIO_Configuratio(SPI_TypeDef* SPIx)
{


}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void SPI_DMA_Configuratio(SPI_TypeDef* SPIx)
{

}

























