
#include "DMAH20160808.H"

//u32 *DMA_Buffer;
//u8	DMA_BufferSize=0;
/*******************************************************************************
*函数名		:DMA_Configuration
*功能描述	:串口配置
*输入			:MemoryAddr--内存地址,
					 BufferSize--缓存大小
*输出			:无
*返回值		:无
*例程			：
*******************************************************************************/
void DMA_Configuration(DMA_Channel_TypeDef* DMAy_Channelx,u32 *MemoryAddr,u32 BufferSize)	//USART1_DMA配置
{
	DMA_InitTypeDef    DMA_Initstructure;
	NVIC_InitTypeDef   NVIC_Initstructure;
	
	/*开启DMA时钟*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);	
	
	/* Enable the DMA1 Interrupt */
	NVIC_Initstructure.NVIC_IRQChannel = DMA1_Channel4_IRQChannel;       		//通道设置为串口1中断
	NVIC_Initstructure.NVIC_IRQChannelSubPriority = 0;     									//中断响应优先级0
	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;        									//打开中断
	NVIC_Init(&NVIC_Initstructure);

	/*DMA发送配置*/
	DMA_Initstructure.DMA_PeripheralBaseAddr =  (u32)(&USART1->DR);					//DMA外设源地址
	DMA_Initstructure.DMA_MemoryBaseAddr     = (u32)MemoryAddr;							//DMA数据内存地址
	DMA_Initstructure.DMA_DIR = DMA_DIR_PeripheralDST;											//DMA_DIR_PeripheralDST（外设作为DMA的目的端），DMA_DIR_PeripheralSRC（外设作为数据传输的来源）
	DMA_Initstructure.DMA_BufferSize = BufferSize; 													//指定DMA通道的DMA缓存的大小
//	DMA_Initstructure.DMA_BufferSize = sizeof(MemoryAddr[0]); 													//指定DMA通道的DMA缓存的大小
	DMA_Initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//DMA_PeripheralInc_Enable（外设地址寄存器递增），DMA_PeripheralInc_Disable（外设地址寄存器不变），
	DMA_Initstructure.DMA_MemoryInc =DMA_MemoryInc_Enable;									//DMA_MemoryInc_Enable（内存地址寄存器递增），DMA_MemoryInc_Disable（内存地址寄存器不变）
	DMA_Initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//外设数据宽度--DMA_PeripheralDataSize_Byte（数据宽度为8位），DMA_PeripheralDataSize_HalfWord（数据宽度为16位），DMA_PeripheralDataSize_Word（数据宽度为32位）
	DMA_Initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//内存数据宽度--DMA_MemoryDataSize_Byte（数据宽度为8位），DMA_MemoryDataSize_HalfWord（数据宽度为16位），DMA_MemoryDataSize_Word （数据宽度为32位）
	DMA_Initstructure.DMA_Mode = DMA_Mode_Normal;														//DMA工作模式--DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular （不停地传送）
	DMA_Initstructure.DMA_Priority = DMA_Priority_High; 										//DMA通道的转输优先级--DMA_Priority_VeryHigh（非常高）DMA_Priority_High（高)，DMA_Priority_Medium（中），DMA_Priority_Low（低）
	DMA_Initstructure.DMA_M2M = DMA_M2M_Disable;														//DMA通道的内存到内存传输--DMA_M2M_Enable(设置为内存到内存传输)，DMA_M2M_Disable（非内存到内存传输）
	DMA_Init(DMAy_Channelx,&DMA_Initstructure);															//初始化DMA

	/* 清除DMA1通道4所有标志*/
	DMA_ClearFlag(DMA1_FLAG_GL4);                                 					// 清除DMA所有标志
	/* 关闭DMA1通道5*/
	DMA_Cmd(DMAy_Channelx,DISABLE);
	/* 使能DMA1通道4传输完成中断 */
	DMA_ITConfig(DMAy_Channelx,DMA_IT_TC, ENABLE);


	/*DMA接收配置*/
	DMA_Initstructure.DMA_PeripheralBaseAddr =  (u32)(&USART1->DR);					//DMA外设源地址
	DMA_Initstructure.DMA_MemoryBaseAddr     = 	(u32)MemoryAddr;						//DMA数据内存地址
	DMA_Initstructure.DMA_DIR = DMA_DIR_PeripheralSRC;											//DMA_DIR_PeripheralDST（外设作为DMA的目的端），DMA_DIR_PeripheralSRC（外设作为数据传输的来源）
	DMA_Initstructure.DMA_BufferSize = BufferSize; 												//指定DMA通道的DMA缓存的大小
//	DMA_Initstructure.DMA_BufferSize = sizeof(MemoryAddr); 									//指定DMA通道的DMA缓存的大小
	DMA_Initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//DMA_PeripheralInc_Enable（外设地址寄存器递增），DMA_PeripheralInc_Disable（外设地址寄存器不变），
	DMA_Initstructure.DMA_MemoryInc =DMA_MemoryInc_Enable;									//DMA_MemoryInc_Enable（内存地址寄存器递增），DMA_MemoryInc_Disable（内存地址寄存器不变）
	DMA_Initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//外设数据宽度--DMA_PeripheralDataSize_Byte（数据宽度为8位），DMA_PeripheralDataSize_HalfWord（数据宽度为16位），DMA_PeripheralDataSize_Word（数据宽度为32位）
	DMA_Initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//内存数据宽度--DMA_MemoryDataSize_Byte（数据宽度为8位），DMA_MemoryDataSize_HalfWord（数据宽度为16位），DMA_MemoryDataSize_Word （数据宽度为32位）
	DMA_Initstructure.DMA_Mode = DMA_Mode_Normal;														//DMA工作模式--DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular （不停地传送）
	DMA_Initstructure.DMA_Priority = DMA_Priority_High; 										//DMA通道的转输优先级--DMA_Priority_VeryHigh（非常高）DMA_Priority_High（高)，DMA_Priority_Medium（中），DMA_Priority_Low（低）
	DMA_Initstructure.DMA_M2M = DMA_M2M_Disable;														//DMA通道的内存到内存传输--DMA_M2M_Enable(设置为内存到内存传输)，DMA_M2M_Disable（非内存到内存传输）
	DMA_Init(DMA1_Channel5,&DMA_Initstructure);															//初始化DMA
	
	/* 清除DMA1通道5所有标志*/
	DMA_ClearFlag(DMA1_FLAG_GL5);                                 					// 清除DMA所有标志
	/* 启动DMA1通道5*/
	DMA_Cmd(DMA1_Channel5,ENABLE);
}
/*******************************************************************************
*函数名		:USART_DMA_Configuration
*功能描述	:串口配置
*输入			:MemoryAddr--内存地址,
					 BufferSize--缓存大小
*输出			:无
*返回值		:无
*例程			：
*******************************************************************************/
void USART_DMA_Configuration(USART_TypeDef* USARTx,u32 *MemoryAddr,u32 BufferSize)	//USART_DMA配置
{
	DMA_InitTypeDef    DMA_Initstructure;
	NVIC_InitTypeDef   NVIC_Initstructure;
	
	DMA_Channel_TypeDef* DMAx_Channeltx=0;
	DMA_Channel_TypeDef* DMAx_Channelrx=0;
	u8 DMAx_Channelx_IRQChannel=0;
	u32 DMAx_FLAG_GLtx=0;
	u32 DMAx_FLAG_GLrx=0;
	
	/*开启DMA时钟*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);	
	
	/*串口*/
	if(USARTx==USART1)
	{
		DMAx_Channeltx=DMA1_Channel4;
		DMAx_Channelrx=DMA1_Channel5;
		DMAx_Channelx_IRQChannel=DMA1_Channel4_IRQChannel;
		DMAx_FLAG_GLtx=DMA1_FLAG_GL4;
		DMAx_FLAG_GLrx=DMA1_FLAG_GL5;
	}
	else if(USARTx==USART2)
	{
		DMAx_Channeltx=DMA1_Channel7;
		DMAx_Channelrx=DMA1_Channel6;
		DMAx_Channelx_IRQChannel=DMA1_Channel7_IRQChannel;
		DMAx_FLAG_GLtx=DMA1_FLAG_GL7;
		DMAx_FLAG_GLrx=DMA1_FLAG_GL6;
	}
	else if(USARTx==USART3)
	{
		DMAx_Channeltx=DMA1_Channel2;
		DMAx_Channelrx=DMA1_Channel3;
		DMAx_Channelx_IRQChannel=DMA1_Channel2_IRQChannel;
		DMAx_FLAG_GLtx=DMA1_FLAG_GL2;
		DMAx_FLAG_GLrx=DMA1_FLAG_GL3;
	}
	else if(USARTx==UART4)
	{
		DMAx_Channeltx=DMA2_Channel5;
		DMAx_Channelrx=DMA2_Channel3;
		DMAx_Channelx_IRQChannel=DMA2_Channel3_IRQChannel;
		DMAx_FLAG_GLtx=DMA2_FLAG_GL5;
		DMAx_FLAG_GLrx=DMA2_FLAG_GL3;
	}
	else if(USARTx==UART5)
	{
	}
	
	/* Enable the DMA1 Interrupt */
	NVIC_Initstructure.NVIC_IRQChannel = DMAx_Channelx_IRQChannel;       		//通道设置为串口1中断
	NVIC_Initstructure.NVIC_IRQChannelSubPriority = 1;     									//中断响应优先级0
	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;        									//打开中断
	NVIC_Init(&NVIC_Initstructure);

	/*DMA发送配置*/
	DMA_Initstructure.DMA_PeripheralBaseAddr =  (u32)(&USARTx->DR);					//DMA外设源地址
	DMA_Initstructure.DMA_MemoryBaseAddr     = (u32)MemoryAddr;							//DMA数据内存地址
	DMA_Initstructure.DMA_DIR = DMA_DIR_PeripheralDST;											//DMA_DIR_PeripheralDST（外设作为DMA的目的端），DMA_DIR_PeripheralSRC（外设作为数据传输的来源）
	DMA_Initstructure.DMA_BufferSize = BufferSize; 													//指定DMA通道的DMA缓存的大小
//	DMA_Initstructure.DMA_BufferSize = sizeof(MemoryAddr[0]); 													//指定DMA通道的DMA缓存的大小
	DMA_Initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//DMA_PeripheralInc_Enable（外设地址寄存器递增），DMA_PeripheralInc_Disable（外设地址寄存器不变），
	DMA_Initstructure.DMA_MemoryInc =DMA_MemoryInc_Enable;									//DMA_MemoryInc_Enable（内存地址寄存器递增），DMA_MemoryInc_Disable（内存地址寄存器不变）
	DMA_Initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//外设数据宽度--DMA_PeripheralDataSize_Byte（数据宽度为8位），DMA_PeripheralDataSize_HalfWord（数据宽度为16位），DMA_PeripheralDataSize_Word（数据宽度为32位）
	DMA_Initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//内存数据宽度--DMA_MemoryDataSize_Byte（数据宽度为8位），DMA_MemoryDataSize_HalfWord（数据宽度为16位），DMA_MemoryDataSize_Word （数据宽度为32位）
	DMA_Initstructure.DMA_Mode = DMA_Mode_Normal;														//DMA工作模式--DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular （不停地传送）
	DMA_Initstructure.DMA_Priority = DMA_Priority_High; 										//DMA通道的转输优先级--DMA_Priority_VeryHigh（非常高）DMA_Priority_High（高)，DMA_Priority_Medium（中），DMA_Priority_Low（低）
	DMA_Initstructure.DMA_M2M = DMA_M2M_Disable;														//DMA通道的内存到内存传输--DMA_M2M_Enable(设置为内存到内存传输)，DMA_M2M_Disable（非内存到内存传输）
	DMA_Init(DMAx_Channeltx,&DMA_Initstructure);															//初始化DMA

	/* 清除DMA1通道4所有标志*/
	DMA_ClearFlag(DMAx_FLAG_GLtx);                                 					// 清除DMA所有标志
	/* 关闭DMA1通道5*/
	DMA_Cmd(DMAx_Channeltx,DISABLE);
	/* 使能DMA1通道4传输完成中断 */
	DMA_ITConfig(DMAx_Channeltx,DMA_IT_TC, ENABLE);


	/*DMA接收配置*/
	DMA_Initstructure.DMA_PeripheralBaseAddr =  (u32)(&USARTx->DR);					//DMA外设源地址
	DMA_Initstructure.DMA_MemoryBaseAddr     = 	(u32)MemoryAddr;						//DMA数据内存地址
	DMA_Initstructure.DMA_DIR = DMA_DIR_PeripheralSRC;											//DMA_DIR_PeripheralDST（外设作为DMA的目的端），DMA_DIR_PeripheralSRC（外设作为数据传输的来源）
	DMA_Initstructure.DMA_BufferSize = BufferSize; 												//指定DMA通道的DMA缓存的大小
//	DMA_Initstructure.DMA_BufferSize = sizeof(MemoryAddr); 									//指定DMA通道的DMA缓存的大小
	DMA_Initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//DMA_PeripheralInc_Enable（外设地址寄存器递增），DMA_PeripheralInc_Disable（外设地址寄存器不变），
	DMA_Initstructure.DMA_MemoryInc =DMA_MemoryInc_Enable;									//DMA_MemoryInc_Enable（内存地址寄存器递增），DMA_MemoryInc_Disable（内存地址寄存器不变）
	DMA_Initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//外设数据宽度--DMA_PeripheralDataSize_Byte（数据宽度为8位），DMA_PeripheralDataSize_HalfWord（数据宽度为16位），DMA_PeripheralDataSize_Word（数据宽度为32位）
	DMA_Initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//内存数据宽度--DMA_MemoryDataSize_Byte（数据宽度为8位），DMA_MemoryDataSize_HalfWord（数据宽度为16位），DMA_MemoryDataSize_Word （数据宽度为32位）
	DMA_Initstructure.DMA_Mode = DMA_Mode_Normal;														//DMA工作模式--DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular （不停地传送）
	DMA_Initstructure.DMA_Priority = DMA_Priority_High; 										//DMA通道的转输优先级--DMA_Priority_VeryHigh（非常高）DMA_Priority_High（高)，DMA_Priority_Medium（中），DMA_Priority_Low（低）
	DMA_Initstructure.DMA_M2M = DMA_M2M_Disable;														//DMA通道的内存到内存传输--DMA_M2M_Enable(设置为内存到内存传输)，DMA_M2M_Disable（非内存到内存传输）
	DMA_Init(DMAx_Channelrx,&DMA_Initstructure);															//初始化DMA
	
	/* 清除DMA1通道5所有标志*/
	DMA_ClearFlag(DMAx_FLAG_GLrx);                                 					// 清除DMA所有标志
	/* 启动DMA1通道5*/
	DMA_Cmd(DMAx_Channelrx,ENABLE);	
}
/*******************************************************************************
*函数名		:DMA_Server
*功能描述	:DMA中断服务函数
*输入			: 
*输出			:无
*返回值		:无
*例程			：
*******************************************************************************/
void DMA_Server(void)
{ 
#ifdef	RS485_TO_USART_PRJ
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);	
#endif
#ifdef	PD002T20_PRJ
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);	
#endif
	/***USART1-TXD***/
	if(DMA_GetITStatus(DMA1_IT_TC4)== SET)
	{
		/*清除标志位*/
		DMA_ClearFlag(DMA1_FLAG_TC4); 
		/*关闭DMA发送*/ 
		DMA_Cmd(DMA1_Channel4,DISABLE);
	}
	/***USART2-TXD***/
	else if(DMA_GetITStatus(DMA1_IT_TC7)== SET)
	{
			/*清除标志位*/
		DMA_ClearFlag(DMA1_FLAG_TC7); 
		/*关闭DMA发送*/ 
		DMA_Cmd(DMA1_Channel7,DISABLE); 
	}
	/***USART3-TXD***/
	else if(DMA_GetITStatus(DMA1_IT_GL2)== SET)
	{
			/*清除标志位*/
		DMA_ClearFlag(DMA1_FLAG_TC2); 
		/*关闭DMA发送*/ 
		DMA_Cmd(DMA1_Channel2,DISABLE); 
	}
	/***USART4-TXD***/
	else if(DMA_GetITStatus(DMA2_IT_GL5)== SET)
	{
			/*清除标志位*/
		DMA_ClearFlag(DMA2_FLAG_TC5); 
		/*关闭DMA发送*/ 
		DMA_Cmd(DMA2_Channel5,DISABLE); 
	}
	/***ADC***/
	else if(DMA_GetITStatus(DMA1_IT_GL1)== SET)
	{
		DMA_Cmd(DMA1_Channel1,DISABLE);
		USER_Server();
			/*清除标志位*/
		DMA_ClearFlag(DMA1_FLAG_TC1); 
		/*关闭DMA发送*/ 
		DMA_Cmd(DMA1_Channel1,ENABLE);
			
	}
}
