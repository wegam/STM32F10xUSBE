///******************************** User_library *********************************
//* 文件名 	: STM32_SDCard.H
//* 作者   	: wegam@sina.com
//* 版本   	: V
//* 日期   	: 2016/01/01
//* 说明   	: 
//********************************************************************************
//* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
//* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
//* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
//* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
//* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
//* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
//*******************************************************************************/


//#include "STM32_SPI_Struct.H"
////#include "STM32F10x_BitBand.H"

////char *SPI_BUFFER;
////u32	SPI_BUFFERSIZE;




///*******************************************************************************
//*函数名		:	SPI_Configuration
//*功能描述	:	串口配置
//*输入			:	MemoryAddr--内存地址,
//						BufferSize--缓存大小
//*输出			:	无
//*返回值		:	无
//*例程			:
//*******************************************************************************/
//void SPIT_Configuration(SPI_TypeDef* SPIx)							//SPI常规使用方式配置
//{	

//}

///*******************************************************************************
//*函数名		:	SPI_PinSet
//*功能描述	:	SPI管脚配置初始化
//*输入			: 
//*输出			:	无
//*返回值		:	无
//*例程			:
//*******************************************************************************/
//void SPIT_PinSet(SPI_TypeDef* SPIx)													//SPI管脚配置及初始化
//{
//	//1)**********定义相关结构体
//	SPI_InitTypeDef  SPI_InitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;

//	//2)**********相关GPIO配置
//	if(SPIx==SPI1)
//	{
//		//2.1)**********打开SPI时钟	
//			
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA, ENABLE);
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);
//		
////		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
////		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
////		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////		GPIO_Init(GPIOA, &GPIO_InitStructure);
//		
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_Init(GPIOA, &GPIO_InitStructure);
//		
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_Init(GPIOA, &GPIO_InitStructure);
//	}
//	else if(SPIx==SPI2)
//	{
//		//2.2)**********打开SPI时钟
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 ,ENABLE);
//			
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);
//		
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14| GPIO_Pin_15;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
//		GPIO_Init(GPIOB, &GPIO_InitStructure);	
//		
////		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 |GPIO_Pin_14| GPIO_Pin_15;
////		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
////		GPIO_Init(GPIOB, &GPIO_InitStructure);	
////		
////		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
////		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
////		GPIO_Init(GPIOB, &GPIO_InitStructure);	
//	}
//		else if(SPIx==SPI3)
//	{
//		//2.2)**********打开SPI时钟
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3 ,ENABLE);
//			
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
//		
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 |GPIO_Pin_4| GPIO_Pin_5;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
//		GPIO_Init(GPIOB, &GPIO_InitStructure);	
//		
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
//		GPIO_Init(GPIOA, &GPIO_InitStructure);	
//		
////		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 |GPIO_Pin_4| GPIO_Pin_5;
////		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
////		GPIO_Init(GPIOB, &GPIO_InitStructure);	
////		
////		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
////		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
////		GPIO_Init(GPIOA, &GPIO_InitStructure);	
//	}
//	//3)**********SPI配置选项
//	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;				//设置方向				（2线全双工、2线只接收、一线发送、一线接收）
//	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;															//模式         	（从或主设备）
//	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;													//宽度         	（8或16位）
//	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;																//时钟极性     	（低或高）
//	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;															//时钟相位     	（第一个或第二个跳变沿）
////	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;																	//片选方式     	（硬件或软件方式）
//	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;																	//片选方式     	（硬件或软件方式）
//	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;				//波特率预分频 	（从2---256分频）
//	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;												//最先发送的位 	（最低位，还是最高位在先）
//	SPI_InitStructure.SPI_CRCPolynomial = 7;																	//设置crc多项式	（数字）如7
//	SPI_Init(SPIx, &SPI_InitStructure);

//}

///*******************************************************************************
//*函数名		:	
//*功能描述	:	
//*输入			: 
//*输出			:	无
//*返回值		:	无
//*例程			:
//*******************************************************************************/
//u8 SPIT_SendByte(u8 byte)
//{
//  /* 等待DR寄存器空 */
//  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

//  /* Send byte through the SPI1 peripheral */
//  SPI_I2S_SendData(SPI1, byte);

//  /* Wait to receive a byte */
//  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

//  /* Return the byte read from the SPI bus */
//  return SPI_I2S_ReceiveData(SPI1);
//}


