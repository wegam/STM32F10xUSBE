#include "USARTH20160808.H"

//u8 Usart_Buffer[32]={0};
//选择串口开启

/*****************************USART DMA方式收发送配置过程*******************************/
//****1.RCC（复位和时钟控制寄存器）初始化，启用GPIO、DMA、USART时钟。********************/ 
//****2.NVIC（嵌套向量中断控制寄存器）初始化，完成各个硬件中断的配置。********************/
//****3.USART初始化，配置串口，设置DMA通道等。********************/ 
//****4.DMA初始化，完成DMA的配置。********************/

//#define USE_USART USARTSELECT_TypeDef*;
/*****************************USART DMA方式收发送配置过程*******************************/
/*******************************************************************************
*函数名		: USART_Configuration
*功能描述	:串口配置
*输入			:串口号	--USARTx：USART1/USART2/USART3/UART4/UART5
					波特率	--USART_BaudRate
					字长		--USART_WordLength
					停止位	--USART_StopBits
					奇偶校验-USART_Parity
*输出			:无
*返回值		:无
*例程			：USART_Configuration(USART1,19200,8,1,0);
*******************************************************************************/
void USART_Configuration(USART_TypeDef* USARTx,u32 USART_BaudRate,u16 USART_WordLength,u16 USART_StopBits,u16 USART_Parity)	//串口配置
{
	GPIO_InitTypeDef GPIO_InitStructure;				//GPIO结构体
	USART_InitTypeDef USART_InitStructure;				//USART结构体
	NVIC_InitTypeDef NVIC_InitStructure; 				//NVIC结构体
	
	u16 TXD_Pin=0;																//串口发送脚
	u16 RXD_Pin=0;																//串口接收脚
	GPIO_TypeDef* GPIO_TX=0;
	GPIO_TypeDef* GPIO_RX=0;
	u8 USARTx_IRQChannel=0;
//	USART_TypeDef* USARTx=0;
//	USART=USART1;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//USART1时钟开启
//	u32 RCC_USART_GPIO_Periph;
//	DMA_InitTypeDefDMA_Initstructure;
//>>>>>>>>>>>>>>>>>>>>>初始化参数-开始>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//	switch(USART)
//	{
//(*(u32*)&USARTx);
//	}
	if(USARTx==USART1)
	{
		TXD_Pin=GPIO_Pin_9;		//USART1-TX>PA9
		RXD_Pin=GPIO_Pin_10;	//USART1-RX>PA10
		
		GPIO_TX=GPIOA;
		GPIO_RX=GPIOA;
		
		USARTx_IRQChannel=USART1_IRQChannel;		//中断
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//USART1时钟开启

	}
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
	else if(USARTx==UART4)
	{
		TXD_Pin=GPIO_Pin_10;	//USART1-TX>PC10
		RXD_Pin=GPIO_Pin_11;	//USART1-RX>PC11
		
		GPIO_TX=GPIOC;
		GPIO_RX=GPIOC;
		
		USARTx_IRQChannel=UART4_IRQChannel;		//中断
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	//USART1时钟开启

	}
	else if(USARTx==UART5)
	{
		TXD_Pin=GPIO_Pin_12;	//USART1-TX>PC12
		RXD_Pin=GPIO_Pin_2;		//USART1-RX>PD2
		
		GPIO_TX=GPIOC;
		GPIO_RX=GPIOD;
		
		USARTx_IRQChannel=UART5_IRQChannel;		//中断
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);	//USART1时钟开启
	}

//<<<<<<<<<<<<<<<<<<<<<初始化参数-结束<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//////////////GPIO配置
	//开启时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//USART1时钟开启
	//配置TX端口
//	GPIO_InitStructure.GPIO_Pin = TXD_Pin;
	GPIO_InitStructure.GPIO_Pin = TXD_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_TX,&GPIO_InitStructure);
	//配置RX端口
//	GPIO_InitStructure.GPIO_Pin = RXD_Pin;
	GPIO_InitStructure.GPIO_Pin = RXD_Pin;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;							//上拉输入
	GPIO_Init(GPIO_RX,&GPIO_InitStructure);

/////////////USART配置////////////////////////////////////////////////
	//设置参数
	USART_InitStructure.USART_BaudRate = USART_BaudRate; 					//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;				//停止位
	USART_InitStructure.USART_Parity = USART_Parity_No ; 					//奇偶校验
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//流控
	USART_Init(USARTx, &USART_InitStructure);											//初始化串口
/////////////NVIC配置////////////////////////////////////////////////

	NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

//	USART_ITConfig(USART1,USART_IT_IDLE, ENABLE);
//////////////开启USART////////////
//	/* 使能串口*/
//	USART_Cmd(USART1, ENABLE);

	/* 若总线空闲，产生中断 */
	//总线空闲中断是在检测到在接收数据后，数据总线上一个字节的时间内，没有再接到数据后发生。
	//也就是RXNE位被置位之后，才开始检测，只被置位一次，除非再次检测到RXNE位被置位，然后才开始检测下一次的总线空闲
	USART_ITConfig(USARTx,USART_IT_IDLE, ENABLE);

	/* 打开串口接收中断*/
//	USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);	//使用空闲中断，则此功能不用

	/* 开启DMA方式接收*/
	USART_DMACmd(USARTx,USART_DMAReq_Rx,ENABLE);
	/* 开启DMA方式发送*/
	USART_DMACmd(USARTx,USART_DMAReq_Tx,ENABLE);

	/* 使能串口*/
	USART_Cmd(USARTx, ENABLE);
}

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
}



