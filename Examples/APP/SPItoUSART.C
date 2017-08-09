#ifdef SPItoUSART

#include "SPItoUSART.H"
#include "stm32f10x_exti.h"



//#define	DMA1_BufferSize 256		//DMA1缓冲大小

#define SPI_BufferSize 64

u8 m_SPI_Buffer=0;
u16 m_DMA_num=0;
u8 fla=0;

//u8 Usart_Test_Buffer[DMA1_BufferSize]={0};

u8 SPI_Tx_Buffer[SPI_BufferSize]={0xAA,0xA9,0xA8,0xA7,0xA6,0xA5,0xA4,0xA3,0xA2,0xA1,0xFF};
u8 SPI_Rx_Buffer[SPI_BufferSize]={0x00};

u8 f_SPI_Rx=0;
u16 SPI1_NSS=0;
u16 SPI2_NSS=0;


NVIC_InitTypeDef NVIC_Initstructure;
u16 time=0;
u16 time_LEDM=0;
u16 time_LEDN=0;
u8 num=0;
u8 ch[120]="USART_BASIC_Configuration(USART_TypeDef* USARTx,u32 USART_BaudRate,u8 NVICPreemptionPriority,u8 NVIC_SubPriority)\n";

void LED_SET(u16 dsptime);

//u8 itf=0;
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void SPItoUSART_Configuration(void)
{
	PWM_Configuration(TIM2,7200,200,20);
	
	GPIO_Configuration(GPIOC,GPIO_Pin_9|GPIO_Pin_12,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);			//GPIO配置
	GPIO_Configuration(GPIOD,GPIO_Pin_2,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);			//GPIO配置
	GPIO_Configuration(GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);			//GPIO配置
//	GPIO_Configuration(GPIOB,GPIO_Pin_4,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);			//GPIO配置
	
	
//	PWM_Configuration(TIM2,7200,10000,50);

//	USART_DMA_Configuration(USART1,115200,1,1,(u32*)Usart_Test_Buffer,(u32*)Usart_Test_Buffer,DMA1_BufferSize);	//USART_DMA配置

	SPI2_Conf_TEST((u32*)SPI_Tx_Buffer,(u32*)SPI_Rx_Buffer,SPI_BufferSize);
//	SPI_DMA_Configuration(SPI2,&SPI_InitStructure,(u32*)SPI_Buffer,(u32*)SPI_Buffer,SPI_BUFFERSIZE);		//SPI_DMA配置
	memset(SPI_Rx_Buffer,0,SPI_BufferSize);							//初始化缓冲
	
	LED_SET(time);
//	PWM_Configuration(TIM2,7200,200,20);

}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void SPItoUSART_Server(void)
{
	if(time_LEDM++>200)
	{
		time_LEDM=0;
		if(time_LEDN++>8)
			time_LEDN=0;
	}
	LED_SET(time_LEDN);
	
	//NSS上升沿中断，表示传输完成，进入中断
	if(EXTI_GetITStatus(EXTI_Line12)==SET)
	{
		u16 tcut=0;
		EXTI_ClearITPendingBit(EXTI_Line12);
		
//		while(tcut++<72);
		DMA_Cmd(DMA1_Channel4,DISABLE);			
		DMA1_Channel4->CNDTR	=	SPI_BufferSize;       			//重新设置接收数据个数		
		DMA_Cmd(DMA1_Channel4,ENABLE);
		
//		SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, DISABLE);
		DMA_Cmd(DMA1_Channel5,DISABLE);
		DMA1_Channel5->CNDTR	=	11;       									//重新设置接收数据个数	
		DMA1_Channel5->CMAR		=	(u32)SPI_Tx_Buffer;       	//重新设置接收数据个数	
//		SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
		DMA_Cmd(DMA1_Channel5,ENABLE);
		
		DMA_ClearFlag(DMA1_FLAG_GL5);
		DMA_ClearFlag(DMA1_FLAG_GL4);
		
		time=0;
	}
	else
	{
		time++;
		if(time>=1000)
		{
			time=0;
			memset(SPI_Rx_Buffer,0,SPI_BufferSize);							//初始化缓冲
		}
	}
	
	
	
	
	
//	if(SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_RXNE)==SET)
//	{
//		SPI_I2S_ClearITPendingBit(SPI2, SPI_I2S_IT_RXNE);
//		DMA_Cmd(DMA1_Channel4,DISABLE);
//		m_DMA_nu0=DMA_GetCurrDataCounter(DMA1_Channel4);
//		DMA1_Channel4->CNDTR	=	SPI_BufferSize;       	//重新设置接收数据个数
//		DMA_Cmd(DMA1_Channel4,ENABLE);
//		time=0;
//		
//	}
//	else
//	{
//		time++;
//		if(time>=1000)
//		{
//			time=0;
//			memset(SPI_Rx_Buffer,0,SPI_BufferSize);							//初始化缓冲
//		}
//	}


	
	
////	SPI DMA接收
//	if(SPI_Rx_Buffer[8]!=0)
//	{
//		time++;
//		if(time>=1000)
//		{
//			memset(SPI_Rx_Buffer,0,SPI_BufferSize);							//初始化缓冲
//			DMA_ClearFlag(DMA1_FLAG_GL4);
//			DMA_ClearITPendingBit(DMA1_IT_GL4);

//			DMA_Cmd(DMA1_Channel4,DISABLE);
//			DMA1_Channel4->CNDTR	=	SPI_BufferSize;       	//重新设置接收数据个数
//			DMA_Cmd(DMA1_Channel4,ENABLE);
//			
//		}
//	}
//	else
//		time=0;
	
	
	
	
//	ErrorStatus status = ERROR;
//	u32 num=0;
//	time++;
//	num=USART_RX_FlagClear(USART1);						//清除串口DMA发送全局中断标志
//	USART_RX_FlagClear(USART1);				//清除串口接收中断标志
//	if(num!=0)
//	{
//		time=0;
//	}
//	else if(!USART_TX_DMAFlagClear(USART1))
//	{
//		time=0;
//		free(USART_BUFFER);
//		memset(Usart_Test_Buffer,0,DMA1_BufferSize);			//初始化缓冲
//	}

//	if(SPI_I2S_GetITStatus(SPI2,SPI_I2S_IT_RXNE)==SET)
//	{
////		SPI_Buffer[m_SPI_Buffer++] = SPI2->DR; 
////		SPI_I2S_ClearITPendingBit(SPI2,SPI_I2S_IT_RXNE);
//		while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_IT_RXNE)==SET)
//		{
//			SPI_Buffer[m_SPI_Buffer++] = SPI_I2S_ReceiveData(SPI2);
//		}
////		SPI_I2S_ClearITPendingBit(SPI2,SPI_I2S_IT_RXNE);
//	}
//	if(m_SPI_Buffer>=10)
//	{
//		m_SPI_Buffer=0;
//		memset(SPI_Buffer,0,10);			//初始化缓冲
//	}
	
//	if(SPI_Rx_Buffer[0] == 0x55)
//		{
//			GPIO_ResetBits(GPIOx_led, GPIO_Pin_x_led);
//			for(i=0; i<0x3fffff; i++);
//			GPIO_SetBits(GPIOx_led, GPIO_Pin_x_led);
//			for(i=0; i<0x3fffff; i++);
//					
//			j++;

//			if(j == 10)
//			{
//				j = 0;	

//			}
//		}

}

/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void SPI_StructConfU(SPI_InitTypeDef* SPI_InitStruct)
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
//	SPI_InitStruct->SPI_Direction=SPI_Direction_1Line_Rx;			//设置方向				（只读）
//	SPI_InitStruct->SPI_Mode=SPI_Mode_Master;														//模式         	（从或主设备）
	SPI_InitStruct->SPI_Mode=SPI_Mode_Slave;													//模式         	（从或主设备）
	SPI_InitStruct->SPI_DataSize=SPI_DataSize_8b;												//宽度         	（8或16位）
	SPI_InitStruct->SPI_CPOL=SPI_CPOL_High;															//时钟极性     	（低或高）
//	SPI_InitStruct->SPI_CPOL=SPI_CPOL_Low;															//时钟极性     	（低或高）
	SPI_InitStruct->SPI_CPHA=SPI_CPHA_2Edge;														//时钟相位     	（第一个或第二个跳变沿）
//	SPI_InitStruct->SPI_CPHA=SPI_CPHA_1Edge;														//时钟相位     	（第一个或第二个跳变沿）
//	SPI_InitStruct->SPI_NSS=SPI_NSS_Soft;															//片选方式     	（硬件或软件方式）
	SPI_InitStruct->SPI_NSS=SPI_NSS_Hard;																//片选方式     	（硬件或软件方式）
	SPI_InitStruct->SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_4;		//波特率预分频 	（从2---256分频）
	SPI_InitStruct->SPI_FirstBit=SPI_FirstBit_MSB;											//最先发送的位 	（最低位，还是最高位在先）
	SPI_InitStruct->SPI_CRCPolynomial=0X07;															//设置crc多项式	（其复位值为0x0007，根据应用可以设置其他数值。）
	
//	SPI_Init(SPI2, SPI_InitStruct);
//	SPI_Cmd(SPI2, ENABLE);
}

void LED_SET(u16 dsptime)
{
	
//	GPIO_InitTypeDef GPIO_InitStructure;						//GPIO结构体
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;				//GPIO引脚号			
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode;				//GPIO模式
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed;			//GPIO输出速度，输入时无效
//	GPIO_Init(GPIOx,&GPIO_InitStructure);						//GPIO初始化
	
//	GPIO_Configuration(GPIOC,GPIO_Pin_12,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);			//GPIO配置
//	GPIO_Configuration(GPIOD,GPIO_Pin_2,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);			//GPIO配置
//	GPIO_Configuration(GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);			//GPIO配置
	
	GPIO_SetBits(GPIOC,GPIO_Pin_9|GPIO_Pin_12);
	GPIO_SetBits(GPIOD,GPIO_Pin_2);
	GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);
	if(dsptime%10==0)
	GPIO_ResetBits(GPIOC,GPIO_Pin_12);
	else if(dsptime%10==1)
	GPIO_ResetBits(GPIOD,GPIO_Pin_2);
	else if(dsptime%10==2)
	GPIO_ResetBits(GPIOB,GPIO_Pin_3);
	else if(dsptime%10==3)
	GPIO_ResetBits(GPIOB,GPIO_Pin_4);
	else if(dsptime%10==4)
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	else if(dsptime%10==5)
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	else if(dsptime%10==6)
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);
	else if(dsptime%10==7)
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	else if(dsptime%10==8)
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);
	else if(dsptime%10==9)
	GPIO_ResetBits(GPIOC,GPIO_Pin_9);
}

#endif

