/******************************** User_library *********************************
* 文件名 	: STM32_SDCard.C
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

#include "STM32_SDCard.H"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//static u32 CardType =  SDIO_STD_CAPACITY_SD_CARD_V1_1;
//static u32 CSD_Tab[4], CID_Tab[4], RCA = 0;
//static u32 DeviceMode = SD_DMA_MODE;
//static u32 TotalNumberOfBytes = 0, StopCondition = 0;
//u32 *SrcBuffer, *DestBuffer;
//volatile SD_Error TransferError = SD_OK;
//vu32 TransferEnd = 0;
//vu32 NumberOfBytes = 0;
//SDIO_InitTypeDef SDIO_InitStructure;
//SDIO_CmdInitTypeDef SDIO_CmdInitStructure;
//SDIO_DataInitTypeDef SDIO_DataInitStructure;

/* Private function prototypes -----------------------------------------------*/
//static SD_Error CmdError(void);
//static SD_Error CmdResp1Error(u8 cmd);
//static SD_Error CmdResp7Error(void);
//static SD_Error CmdResp3Error(void);
//static SD_Error CmdResp2Error(void);
//static SD_Error CmdResp6Error(u8 cmd, u16 *prca);
//static SD_Error SDEnWideBus(FunctionalState NewState);
//static SD_Error IsCardProgramming(u8 *pstatus);
//static SD_Error FindSCR(u16 rca, u32 *pscr);
//static u8 convert_from_bytes_to_power_of_two(u16 NumberOfBytes);
//static void GPIO_Configuration(void);
//static void DMA_TxConfiguration(u32 *BufferSRC, u32 BufferSize);
//static void DMA_RxConfiguration(u32 *BufferDST, u32 BufferSize);

/* Private functions ---------------------------------------------------------*/	
unsigned char SD_TX_Addr[SDCard_BufferSize]={0xFF};
unsigned char SD_RX_Addr[SDCard_BufferSize]={0xFF};
unsigned char	SDCard_Type=0;					//SD卡的类型
SDCard_Status_TypeDef	SDCard_Status=SDCard_Ready;
SDCard_Step_TypeDef	SDCard_Step=SDCard_DeInit;
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->函数名			:	SDCard_Configuration
//->功能描述		:	SD卡项目配置
//->输入			: 无
//->输出			:	Errorstatus	配置状态
//->返回值			:	无
//->例程			:
//->调用函数		:
//->被调用函数	:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
SDCard_ErrorStatus	SDCard_Configuration(SDCard_CardInfo_TypeDef *SDCard_CardInfo)
{
	//____________定义变量
	SDCard_ErrorStatus Errorstatus = SDCard_SUCCESS;				//SDCard_SUCCESS 无错误，SDCard_ERROR 错误
	//____________函数调用
	SDCard_GPIO_Configuration();							//SPI管脚配置
	SDCard_SPI_Configuration(SD_CLK_SpeedL);	//SPI配置--低速
#ifdef	SDCard_DMA
	SDCard_DMA_Configuration((u32*)SD_TX_Addr,(u32*)SD_RX_Addr,SDCard_BufferSize);
#endif
	Errorstatus=SDCard_Initialize(SDCard_CardInfo);														//SDCard初始化
	if(Errorstatus==SDCard_SUCCESS)
	{
		if(SDCard_CardInfo->SDCard_TYPE!=SD_TYPE_ERR)
		{
			SDCard_SPI_Configuration(SD_CLK_SpeedH);	//SPI配置--高速
			return SDCard_SUCCESS;
		}
		else
		{
			return SDCard_ERROR;
		}
	}
	return	Errorstatus;
}
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->函数名			:	SDCard_Server
//->功能描述		:	SD卡项目执行(服务)程序
//->输入			: 
//->输出			:	Errorstatus	状态
//->返回值			:	无
//->例程			:
//->调用函数		:
//->被调用函数	:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
SDCard_ErrorStatus	SDCard_Server(void)
{
	//____________定义变量
	SDCard_ErrorStatus Errorstatus = SDCard_SUCCESS;				//SDCard_SUCCESS 无错误，SDCard_ERROR 错误
	#ifdef	SDCard_DMA
	if(DMA_GetFlagStatus(DMA1_IT_GL2)== SET)
	{			
		//____________清除标志位
		DMA_ClearFlag(DMA1_FLAG_GL2); 
		//____________关闭DMA发送 
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
		DMA_Cmd(DMA1_Channel3,DISABLE);											//DMA发送关闭3
		DMA_Cmd(DMA1_Channel2,DISABLE);											//DMA发送关闭2
		SDCard_Status=SDCard_Ready;
		//____________禁止片选	
		SDCard_DisSelect();
		return	SDCard_SUCCESS;
	}
	else
	{
		return	SDCard_ERROR;
	}
#endif

	return	Errorstatus;
}
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->函数名			:	SDCard_GPIO_Configuration
//->功能描述		:	SPI管脚配置
//->输入			: 
//->输出			:	无
//->返回值			:	无
//->例程			:
//->调用函数		:
//->被调用函数	:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
void SDCard_GPIO_Configuration(void)
{
	//1)____________定义
	GPIO_InitTypeDef		GPIO_InitStructure;
	//2)____________开启时钟
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,	ENABLE);		//开启AFIO
	RCC_APB2PeriphClockCmd(SDCard_CS_Periph,		ENABLE);		//开启CS时钟
	RCC_APB2PeriphClockCmd(SDCard_CLK_Periph,		ENABLE);		//开启CLK时钟
	RCC_APB2PeriphClockCmd(SDCard_MISO_Periph,	ENABLE);		//开启MISO时钟
	RCC_APB2PeriphClockCmd(SDCard_MOSI_Periph,	ENABLE);		//开启MOSI时钟	
	//3)____________管脚配置
	//3.1）CS配置
	GPIO_InitStructure.GPIO_Pin			=	SDCard_CS_PIN;
	GPIO_InitStructure.GPIO_Mode		=	GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed		= GPIO_Speed_50MHz;
	GPIO_Init(SDCard_CS_PORT, &GPIO_InitStructure);
	//3.2）CLK配置
	GPIO_InitStructure.GPIO_Pin			=	SDCard_CLK_PIN;
	GPIO_InitStructure.GPIO_Mode		=	GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed		= GPIO_Speed_50MHz;
	GPIO_Init(SDCard_CLK_PORT, &GPIO_InitStructure);
	//3.3）MISO配置
	GPIO_InitStructure.GPIO_Pin			=	SDCard_MISO_PIN;
	GPIO_InitStructure.GPIO_Mode		=	GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed		= GPIO_Speed_50MHz;
	GPIO_Init(SDCard_MISO_PORT, &GPIO_InitStructure);
	//3.4）MOSI配置
	GPIO_InitStructure.GPIO_Pin			=	SDCard_MOSI_PIN;
	GPIO_InitStructure.GPIO_Mode		=	GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed		= GPIO_Speed_50MHz;
	GPIO_Init(SDCard_MOSI_PORT, &GPIO_InitStructure);
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	
//	
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//	//____________SD-CS
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;			
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOC,&GPIO_InitStructure);
//	
//	//____________TOUCH-CS
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;			
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->函数名			:	SDCard_SPI_Configuration
//->功能描述		:	SPI配置
//->输入			: Speed SPI速度选择（低速/高速)
//->输出			:	无
//->返回值			:	无
//->例程			:
//->调用函数		:
//->被调用函数	:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
void SDCard_SPI_Configuration(char Speed)
{
	//1)____________定义
	SPI_InitTypeDef			SPI_InitStructure;
	RCC_APB2PeriphClockCmd(SDCard_SPI_Periph,		ENABLE);		//开启SPI时钟
	//2)____________SPI初始化
	if(Speed==SD_CLK_SpeedH)		//高速端口
	{
		SPI_InitStructure.SPI_Direction					=	SPI_Direction_2Lines_FullDuplex;	//设置方向				（2线全双工、2线只接收、一线发送、一线接收）
		SPI_InitStructure.SPI_Mode							=	SPI_Mode_Master;									//模式         	（从或主设备）
		SPI_InitStructure.SPI_DataSize					=	SPI_DataSize_8b;									//宽度         	（8或16位）
		SPI_InitStructure.SPI_CPOL							=	SPI_CPOL_High;										//时钟极性     	（低或高）
		SPI_InitStructure.SPI_CPHA							=	SPI_CPHA_2Edge;										//时钟相位     	（第一个或第二个跳变沿）
		SPI_InitStructure.SPI_NSS								=	SPI_NSS_Soft;											//片选方式     	（硬件或软件方式）
	//	SPI_InitStructure.SPI_NSS=SPI_NSS_Hard;																		//片选方式     	（硬件或软件方式）
		SPI_InitStructure.SPI_BaudRatePrescaler	=	SPI_BaudRatePrescaler_128;					//波特率预分频 	（从2---256分频）
		SPI_InitStructure.SPI_FirstBit					=	SPI_FirstBit_MSB;									//最先发送的位 	（最低位，还是最高位在先）
		SPI_InitStructure.SPI_CRCPolynomial			=	0X07;															//设置crc多项式	（其复位值为0x0007，根据应用可以设置其他数值。）
		SPI_Init(SDCard_SPI_PORT,&SPI_InitStructure);													//SPI1初始化
	}
	else											//低速端口
	{
		SPI_InitStructure.SPI_Direction					=	SPI_Direction_2Lines_FullDuplex;	//设置方向				（2线全双工、2线只接收、一线发送、一线接收）
		SPI_InitStructure.SPI_Mode							=	SPI_Mode_Master;									//模式         	（从或主设备）
		SPI_InitStructure.SPI_DataSize					=	SPI_DataSize_8b;									//宽度         	（8或16位）
		SPI_InitStructure.SPI_CPOL							=	SPI_CPOL_High;										//时钟极性     	（低或高）
		SPI_InitStructure.SPI_CPHA							=	SPI_CPHA_2Edge;										//时钟相位     	（第一个或第二个跳变沿）
		SPI_InitStructure.SPI_NSS								=	SPI_NSS_Soft;											//片选方式     	（硬件或软件方式）
	//	SPI_InitStructure.SPI_NSS=SPI_NSS_Hard;																		//片选方式     	（硬件或软件方式）
		SPI_InitStructure.SPI_BaudRatePrescaler	=	SPI_BaudRatePrescaler_256;				//波特率预分频 	（从2---256分频）
		SPI_InitStructure.SPI_FirstBit					=	SPI_FirstBit_MSB;									//最先发送的位 	（最低位，还是最高位在先）
		SPI_InitStructure.SPI_CRCPolynomial			=	0X07;															//设置crc多项式	（其复位值为0x0007，根据应用可以设置其他数值。）
		SPI_Init(SDCard_SPI_PORT,&SPI_InitStructure);													//SPI1初始化
	}
	SDCard_CS_OFF;			//取消片选
	SPI_Cmd(SDCard_SPI_PORT, ENABLE);				//使能SPI
}
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->函数名			:	SDCard_DMA_Configuration
//->功能描述		:	DMA配置
//->输入			: SD_TX_Addr	SPI发送地址
//							SD_RX_Addr	SPI接收地址
//							BufferSize	DMA缓存大小
//->输出			:	无
//->返回值			:	无
//->例程			:
//->调用函数		:
//->被调用函数	:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
void	SDCard_DMA_Configuration(u32 *SD_TX_Addr,u32 *SD_RX_Addr,u32 BufferSize)
{
	//1)____________定义
	DMA_InitTypeDef			DMA_Initstructure;
	NVIC_InitTypeDef		NVIC_Initstructure;
	
	//2)____________DMA发送初始化，外设作为DMA的目的端
	DMA_Initstructure.DMA_PeripheralBaseAddr	=	(u32)(&SPI1->DR);								//DMA外设源地址
	DMA_Initstructure.DMA_MemoryBaseAddr			=	(u32)SD_TX_Addr;								//DMA数据内存地址
	DMA_Initstructure.DMA_DIR									= DMA_DIR_PeripheralDST;					//DMA_DIR_PeripheralDST（外设作为DMA的目的端），DMA_DIR_PeripheralSRC（外设作为数据传输的来源）
	DMA_Initstructure.DMA_BufferSize					= BufferSize; 										//指定DMA通道的DMA缓存的大小
	DMA_Initstructure.DMA_PeripheralInc				= DMA_PeripheralInc_Disable;			//DMA_PeripheralInc_Enable（外设地址寄存器递增），DMA_PeripheralInc_Disable（外设地址寄存器不变），
	DMA_Initstructure.DMA_MemoryInc						=	DMA_MemoryInc_Enable;						//DMA_MemoryInc_Enable（内存地址寄存器递增），DMA_MemoryInc_Disable（内存地址寄存器不变）
	DMA_Initstructure.DMA_PeripheralDataSize	= DMA_PeripheralDataSize_Byte;		//外设数据宽度--DMA_PeripheralDataSize_Byte（数据宽度为8位），DMA_PeripheralDataSize_HalfWord（数据宽度为16位），DMA_PeripheralDataSize_Word（数据宽度为32位）
	DMA_Initstructure.DMA_MemoryDataSize			= DMA_MemoryDataSize_Byte;				//内存数据宽度--DMA_MemoryDataSize_Byte（数据宽度为8位），DMA_MemoryDataSize_HalfWord（数据宽度为16位），DMA_MemoryDataSize_Word（数据宽度为32位）
	DMA_Initstructure.DMA_Mode								= DMA_Mode_Normal;								//DMA工作模式--DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular（不停地传送）
	DMA_Initstructure.DMA_Priority						= DMA_Priority_High; 							//DMA通道的转输优先级--DMA_Priority_VeryHigh（非常高）DMA_Priority_High（高)，DMA_Priority_Medium（中），DMA_Priority_Low（低）
	DMA_Initstructure.DMA_M2M									= DMA_M2M_Disable;								//DMA通道的内存到内存传输--DMA_M2M_Enable(设置为内存到内存传输)，DMA_M2M_Disable（非内存到内存传输）
	DMA_Init(DMA1_Channel3,&DMA_Initstructure);																	//初始化DMA

	//3)____________DMA接收初始化，外设作为DMA的源端
	DMA_Initstructure.DMA_PeripheralBaseAddr	=	(u32)(&SPI1->DR);								//DMA外设源地址
	DMA_Initstructure.DMA_MemoryBaseAddr     	=	(u32)SD_RX_Addr;								//DMA数据内存地址
	DMA_Initstructure.DMA_DIR									=	DMA_DIR_PeripheralSRC;					//DMA_DIR_PeripheralDST（外设作为DMA的目的端），DMA_DIR_PeripheralSRC（外设作为数据传输的来源）
	DMA_Initstructure.DMA_BufferSize					= BufferSize; 										//指定DMA通道的DMA缓存的大小
	DMA_Initstructure.DMA_PeripheralInc				= DMA_PeripheralInc_Disable;			//DMA_PeripheralInc_Enable（外设地址寄存器递增），DMA_PeripheralInc_Disable（外设地址寄存器不变），
	DMA_Initstructure.DMA_MemoryInc						=	DMA_MemoryInc_Enable;						//DMA_MemoryInc_Enable（内存地址寄存器递增），DMA_MemoryInc_Disable（内存地址寄存器不变）
	DMA_Initstructure.DMA_PeripheralDataSize	= DMA_PeripheralDataSize_Byte;		//外设数据宽度--DMA_PeripheralDataSize_Byte（数据宽度为8位），DMA_PeripheralDataSize_HalfWord（数据宽度为16位），DMA_PeripheralDataSize_Word（数据宽度为32位）
	DMA_Initstructure.DMA_MemoryDataSize			= DMA_MemoryDataSize_Byte;				//内存数据宽度--DMA_MemoryDataSize_Byte（数据宽度为8位），DMA_MemoryDataSize_HalfWord（数据宽度为16位），DMA_MemoryDataSize_Word（数据宽度为32位）
	DMA_Initstructure.DMA_Mode								= DMA_Mode_Normal;								//DMA工作模式--DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular（不停地传送）
	DMA_Initstructure.DMA_Priority						= DMA_Priority_High; 							//DMA通道的转输优先级--DMA_Priority_VeryHigh（非常高）DMA_Priority_High（高)，DMA_Priority_Medium（中），DMA_Priority_Low（低）
	DMA_Initstructure.DMA_M2M									= DMA_M2M_Disable;								//DMA通道的内存到内存传输--DMA_M2M_Enable(设置为内存到内存传输)，DMA_M2M_Disable（非内存到内存传输）
	DMA_Init(DMA1_Channel2,&DMA_Initstructure);																	//初始化DMA
	
	//4)____________DMA通道中断初始化---此为DMA发送中断----DMA发送完成中断
	NVIC_Initstructure.NVIC_IRQChannel = DMA1_Channel2_IRQChannel;       				//设置中断源
	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_Initstructure.NVIC_IRQChannelSubPriority = 1; 													//中断响应优先级0	
	NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;        											//打开中断
	NVIC_Init(&NVIC_Initstructure);

	//5)____________设置触发中断事件
	//		DMA_IT_TC		传输完成中断
	//		DMA_IT_HT		传输过半中断
	//		DMA_IT_TE		传输错误中断
	DMA_ITConfig(DMA1_Channel2,DMA_IT_TC,ENABLE);						//传输完成中断屏蔽
	//6)____________使能DMA传输通道	
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);				//使能DMA发送
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);				//使能DMA接收

	//6)____________禁止DMA---需要传输时再打开
	DMA_Cmd(DMA1_Channel2,DISABLE);	
	DMA_Cmd(DMA1_Channel3,DISABLE);
}

//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->函数名			:	SDCard_Initialize
//->功能描述		:	SDCard初始化
//->输入			: 无
//->输出			:	Errorstatus	初始化状态	SD_OK/SD_ERROR
//->返回值			:	无
//->例程			:
//->调用函数		:
//->被调用函数	:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
SDCard_ErrorStatus	SDCard_Initialize(SDCard_CardInfo_TypeDef *SDCard_CardInfo)
{
//	//____________定义变量
//	SDCard_ErrorStatus Errorstatus = SDCard_SUCCESS;				//SDCard_SUCCESS 无错误，SDCard_ERROR 错误
////	unsigned char r1;     						// 存放SD卡的返回值
////	unsigned short retry;  						// 用来进行超时计数
////	unsigned char buf[4];  
////	unsigned short i; 
//#ifdef		SDCard_DMA
//	//1)____________等待SD卡准备
//	if(SDCard_Step==SDCard_DeInit)
//	{
//		if(SDCard_Status==SDCard_Ready)
//			SDCard_EnterReady();
//		else
//		{
//			SDCard_Status=SDCard_Ready;
//			SDCard_Step=SDCard_StepReady;	//SD卡准备完成
//		}
//	}
//	//2)____________使SD卡进入IDLE状态，发送CMD0命令，成功后返回0x01
//	if(SDCard_Step==SDCard_Ready)
//	{
//		if(SDCard_Status==SDCard_Ready)
//			SDCard_EnterIDLE(SDCard_CardInfo);		//进入IDLE状态
//		else
//		{
//			if(memchr(SD_RX_Addr,0x01,8)!=NULL)		//未收到复位成功标志，重试
//			{
//				memset(SD_RX_Addr,0XFF,514);				//初始化缓冲
//				memset(SD_TX_Addr,0XFF,514);				//初始化缓冲
//				
//				SDCard_Status=SDCard_Ready;
//				SDCard_Initialize(SDCard_CardInfo);	//重新调用程序
//			}
//			else
//			{
//				SDCard_Status=SDCard_Ready;
//				SDCard_Step=SDCard_StepIDLE;				//SD卡准备完成
//			}
//		}
//	}
//	//3)____________SD卡识别
//	if(SDCard_Step==SDCard_StepIDLE)
//	{
//		if(SDCard_Status==SDCard_Ready)
//		{
//			SDCard_Identification(SDCard_CardInfo);
//		}
//	}

//	
//	//3)____________SD卡识别
//	//3.1)__先发送V2.0版才具有的CMD8命令，然后检测返回值，
//	//3.2.1)返回值若是0X01，则表示此卡为V2.0卡，
//	//3.2.2)然后再发送循环命令CMD55+ACMD41，直到返回0x00，确定SD2.0卡初始化成功；
//	//3.2.3)然后再发送CMD58命令，接收返回的OCR寄存器的数据，其中第31位用于判断V2.0的卡是否为SDHC类型。
//	//3.3.1)返回值若不为0X01，则进一步判断是V1.0卡还是MMC卡
//	//3.3.2)先发送循环命令CMD55+ACMD41进行复位，如果复位不成功则考虑是MMC卡，如果复位成功，则为V1.0卡。
//	//3.3.3)在复位不成功的情况下，再使用CMD1进行复位，如果复位成功，则表明是MMC卡，如果复位不成功，则表示是无法识别的卡。
//	Errorstatus=SDCard_Identification(SDCard_CardInfo);
//	if(Errorstatus!=SDCard_SUCCESS)											//复位失败
//	{
//		SDCard_DisSelect();									//取消片选,释放SPI总线--取消上次片选
//		return SDCard_ERROR;															//SDCard_SUCCESS 无错误，SDCard_ERROR 错误
//	}
//	SDCard_GetCSD(SDCard_CardInfo);											//获取SD卡的CSD信息，包括容量和速度信息
//	SDCard_GetCID(SDCard_CardInfo);											//获取SD卡的CID信息，包括制造商信息

//	
//	SDCard_DisSelect();									//取消片选,释放SPI总线--取消上次片选
//	return	SDCard_SUCCESS;
//#endif	
	
		//____________定义变量
	SDCard_ErrorStatus Errorstatus = SDCard_SUCCESS;				//SDCard_SUCCESS 无错误，SDCard_ERROR 错误
//	unsigned char r1;     						// 存放SD卡的返回值
//	unsigned short retry;  						// 用来进行超时计数
//	unsigned char buf[4];  
//	unsigned short i; 
	SDCard_Select();									//取消片选,释放SPI总线--取消上次片选
	//1)____________等待SD卡准备
	SDCard_EnterReady();
	//2)____________使SD卡进入IDLE状态
	if(SDCard_Step==SDCard_DeInit)
	{
	}
	//__发送CMD0命令，成功后返回0x01
		Errorstatus=SDCard_EnterIDLE(SDCard_CardInfo);	//进入IDLE状态
	//____________SD复位状态
	if(Errorstatus!=SDCard_SUCCESS)										//复位失败
		return SDCard_ERROR;					//SDCard_SUCCESS 无错误，SDCard_ERROR 错误
	
	//3)____________SD卡识别
	//3.1)__先发送V2.0版才具有的CMD8命令，然后检测返回值，
	//3.2.1)返回值若是0X01，则表示此卡为V2.0卡，
	//3.2.2)然后再发送循环命令CMD55+ACMD41，直到返回0x00，确定SD2.0卡初始化成功；
	//3.2.3)然后再发送CMD58命令，接收返回的OCR寄存器的数据，其中第31位用于判断V2.0的卡是否为SDHC类型。
	//3.3.1)返回值若不为0X01，则进一步判断是V1.0卡还是MMC卡
	//3.3.2)先发送循环命令CMD55+ACMD41进行复位，如果复位不成功则考虑是MMC卡，如果复位成功，则为V1.0卡。
	//3.3.3)在复位不成功的情况下，再使用CMD1进行复位，如果复位成功，则表明是MMC卡，如果复位不成功，则表示是无法识别的卡。
	Errorstatus=SDCard_Identification(SDCard_CardInfo);
	if(Errorstatus!=SDCard_SUCCESS)											//复位失败
	{
		SDCard_DisSelect();									//取消片选,释放SPI总线--取消上次片选
		return SDCard_ERROR;															//SDCard_SUCCESS 无错误，SDCard_ERROR 错误
	}
	SDCard_GetCSD(SDCard_CardInfo);											//获取SD卡的CSD信息，包括容量和速度信息
	SDCard_GetCID(SDCard_CardInfo);											//获取SD卡的CID信息，包括制造商信息
	
	
#ifdef Gon	
	//____________SD卡判断
	SDCard_Type=0;//默认无卡
	if(r1==0X01)
	{
		if(SDCard_SendCmd(CMD8,0x1AA,0x87)==1)//SD V2.0
		{
			for(i=0;i<4;i++)
				buf[i]=SDCard_ReadWriteByte(0XFF);	//Get trailing return value of R7 resp
			if(buf[2]==0X01&&buf[3]==0XAA)//卡是否支持2.7~3.6V
			{
				retry=0XFFFE;
				do
				{
					SDCard_SendCmd(CMD55,0,0X01);		//发送CMD55
					r1=SDCard_SendCmd(CMD41,0x40000000,0X01);//发送CMD41
				}while(r1&&retry--);

				if(retry&&SDCard_SendCmd(CMD58,0,0X01)==0)//鉴别SD2.0卡版本开始
				{
					for(i=0;i<4;i++)
						buf[i]=SDCard_ReadWriteByte(0XFF);	//得到OCR值
					if(buf[0]&0x40)
						SDCard_Type=SD_TYPE_V2HC;    						//检查CCS
					else
						SDCard_Type=SD_TYPE_V2;   
				}
			}
		}
		else//SD V1.x/ MMC	V3
		{
			SDCard_SendCmd(CMD55,0,0X01);		//发送CMD55
			r1=SDCard_SendCmd(CMD41,0,0X01);	//发送CMD41
			
			if(r1<=1)
			{		
				SDCard_Type=SD_TYPE_V1;
				retry=0XFFFE;
				do //等待退出IDLE模式
				{
					SDCard_SendCmd(CMD55,0,0X01);		//发送CMD55
					r1=SDCard_SendCmd(CMD41,0,0X01);	//发送CMD41
				}while(r1&&retry--);
				
			}
			else//MMC卡不支持CMD55+CMD41识别
			{
				SDCard_Type=SD_TYPE_MMC;//MMC V3
				retry=0XFFFE;
				do //等待退出IDLE模式
				{	
					r1=SDCard_SendCmd(CMD1,0,0X01);//发送CMD1
				}while(r1&&retry--);  
			}
			if(retry==0||SDCard_SendCmd(CMD16,512,0X01)!=0)
				SDCard_Type=SD_TYPE_ERR;//错误的卡
		}
	}
	
#endif	
	SDCard_DisSelect();									//取消片选,释放SPI总线--取消上次片选
	return	SDCard_SUCCESS;
}
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->函数名			:	SDCard_DisSelect
//->功能描述		:	取消片选,释放SPI总线
//->输入			: 
//->输出			:	无
//->返回值			:	无
//->例程			:
//->调用函数		:
//->被调用函数	:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
void	SDCard_DisSelect(void)
{
	SDCard_CS_OFF;								//GPIO_SetBits(SDCard_CS_PORT,	SDCard_CS_PIN)			//取消片选
//	GPIO_SetBits(SDCard_CS_PORT,	SDCard_CS_PIN);			//取消片选
	SDCard_ReadWriteByte(0xff);		//提供额外的8个时钟
}
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->函数名			:	SDCard_Select
//->功能描述		:	选择sd卡,并且等待卡准备OK
//->输入			: 
//->输出			:	无
//->返回值			:	无
//->例程			:
//->调用函数		:
//->被调用函数	:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
void	SDCard_Select(void)
{
	SDCard_CS_ON;												//GPIO_ResetBits(SDCard_CS_PORT,	SDCard_CS_PIN)
}
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->函数名		:	SDCard_DMASend
//->功能描述	:	DMA方式向SD写入数据，开启DMA发送和接收 
//->输入		:	BufferSize-字节数
//->输出		:
//->返回 		:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
void SDCard_DMASend(unsigned int BufferSize)
{
	DMA1_Channel3->CNDTR =BufferSize;										//设定待发送缓冲区大小
	DMA1_Channel3->CMAR =(u32)SD_TX_Addr;					//发送缓冲区	
	
	DMA1_Channel2->CNDTR =BufferSize;										//设定接收缓冲区大小
	DMA1_Channel2->CMAR =(u32)SD_RX_Addr;					//接收缓冲区
	
	DMA_Cmd(DMA1_Channel3,ENABLE);											//DMA发送开启3
	DMA_Cmd(DMA1_Channel2,ENABLE);											//DMA发送开启2
}
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->函数名			:	SDCard_WaitReady
//->功能描述		:	等待卡准备好
//->输入			: 
//->输出			:	无
//->返回值			:	无
//->例程			:
//->调用函数		:
//->被调用函数	:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
//SDCard_ErrorStatus	SDCard_WaitReady(void)
//{
//	//____________定义变量
//	SDCard_ErrorStatus Errorstatus = SDCard_SUCCESS;				//SDCard_SUCCESS 无错误，SDCard_ERROR 错误
//	u32 retry=0;													//用来进行超时计数
//	//____________等待
//	do
//	{
//		if(SDCard_ReadWriteByte(0XFF)==0XFF)
//			return SDCard_SUCCESS;																	//SDCard_SUCCESS 无错误，SDCard_ERROR 错误
//		retry++;		  	
//	}while(retry<0XFFFFFF);//等待
//	//____________失败	
//	Errorstatus = SDCard_ERROR;																	//SDCard_SUCCESS 无错误，SDCard_ERROR 错误
//	//____________返回状态
//	return(Errorstatus);	
//}
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->函数名			:	SDCard_GetResponse
//->功能描述		:	等待SD卡回应
//->输入			: Response 需要得到的回应值
//->输出			:	无
//->返回值			:	SD_OK 无错误
//							SD_ERROR 错误
//->例程			:
//->调用函数		:
//->被调用函数	:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
SDCard_ErrorStatus SDCard_GetResponse(unsigned char Response)
{
	//____________定义变量
	SDCard_ErrorStatus Errorstatus = SDCard_SUCCESS;				//SDCard_SUCCESS 无错误，SDCard_ERROR 错误
	
	unsigned int Count=0xFFFF;		//等待次数
//	//____________禁止片选	
//	SDCard_DisSelect();	
	//____________使能片选
//	SDCard_Select();		//使能sd卡,并且等待卡准备OK		
//	while ((SDCard_ReadWriteByte(0XFF))&&Count)	//等待得到准确的回应
//		Count--; 
//	Count=1; 
	while ((SDCard_ReadWriteByte(0XFF)!=Response)&&Count--);	//等待得到准确的回应

	if (Count==0)
	{
		Errorstatus = SDCard_ERROR;		//得到回应失败  
	}		
	else
	{
		Errorstatus = SDCard_SUCCESS;	//SDCard_SUCCESS 无错误，SDCard_ERROR 错误
	}
//	//____________禁止片选	
//	SDCard_DisSelect();	
	//____________返回状态
	return(Errorstatus);
}
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->函数名			:	SDCard_RecvData
//->功能描述		:	从sd卡读取一个数据包的内容
//->输入			: buf 接收缓冲区
//							len 需要读取的数据长度
//->输出			:	无
//->返回值			:	SD_OK 无错误
//							SD_ERROR 错误
//->例程			:
//->调用函数		:
//->被调用函数	:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
SDCard_ErrorStatus SDCard_RecvData(unsigned char	*Rxbuf,unsigned int len)
{
	//____________定义变量
	SDCard_ErrorStatus Errorstatus = SDCard_SUCCESS;		//SDCard_SUCCESS 无错误，SDCard_ERROR 错误
	
	//____________使能片选
	SDCard_Select();																		//使能sd卡,并且等待卡准备OK
	
	//____________//等待SD卡发回数据起始令牌0xFE
	if(SDCard_GetResponse(0xFE)==SDCard_ERROR)
	{
		//____________禁止片选	
		SDCard_DisSelect();	
		return SDCard_ERROR;//等待SD卡发回数据起始令牌0xFE
	}

	while(len--)//开始接收数据
	{
		*Rxbuf=SDCard_ReadWriteByte(0xFF);
		Rxbuf++;
	}
	//____________接收CRC，下面是2个伪CRC（dummy CRC）
	SDCard_ReadWriteByte(0xFF);
	SDCard_ReadWriteByte(0xFF);
	
	//____________禁止片选	
	SDCard_DisSelect();
	
	return SDCard_SUCCESS;		//读取成功				

	//____________返回状态
	return(Errorstatus);
}
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->函数名			:	SDCard_EnterReady
//->功能描述		:	SD卡准备
//->输入			: 
//->输出			:	无
//->返回值			:	无
//->例程			:
//->调用函数		:
//->被调用函数	:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
void	SDCard_EnterReady(void)
{
	//____________定义变量
	u8 retry=0;													//用来进行超时计数
	//____________使能片选
//	SDCard_Select();										//使能sd卡,并且等待卡准备OK
	//____________发送数据
	//	SD卡要求复位前至少发送74个clock，这里我发了很多，足够多
	for(retry=0;retry<=20;retry++)
	{
		SDCard_ReadWriteByte(0XFF);
	}
	//____________禁止片选	
//	SDCard_DisSelect();									//取消片选,释放SPI总线
}
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->函数名			:	SDCard_EnterReady
//->功能描述		:	SD卡准备
//->输入			: 
//->输出			:	无
//->返回值			:	无
//->例程			:
//->调用函数		:
//->被调用函数	:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
SDCard_ErrorStatus	SDCard_EnterIDLE(SDCard_CardInfo_TypeDef *SDCard_CardInfo)
{
	//____________定义变量
//	SDCard_ErrorStatus Errorstatus = SDCard_SUCCESS;				//SDCard_SUCCESS 无错误，SDCard_ERROR 错误
	unsigned char retry=20;							//用来进行超时计数
	unsigned char r1=0xFF;							//接收返回字
//	SDCard_DisSelect();								//取消片选,释放SPI总线--取消上次片选
//	SDCard_Select();									//使能sd卡,并且等待卡准备OK
	//____________发送CMD0命令
	//__发送CMD0命令，成功后返回0x01
	//____________判断回应
#ifdef	SDCard_DMA
	SDCard_SendCmd(CMD0,0x00,0x95);
#else
	retry=20;
	r1=0xFF;
	do
	{
//		SDCard_Select();										//使能sd卡,并且等待卡准备OK
		r1	=	SDCard_SendCmd(CMD0,0x00,0x95);
//		SDCard_DisSelect();									//取消片选,释放SPI总线--取消上次片选
	}while((r1!=0x01)&&retry--);
	if(r1!=0x01)
	{
//		SDCard_CardInfo->SD_Error=
		return	SDCard_ERROR;																//SDCard_SUCCESS 无错误，SDCard_ERROR 错误
	}
#endif
	//____________返回状态
	return	SDCard_SUCCESS;	
}
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->函数名			:	SDCard_Identification
//->功能描述		:	SD卡识别
//->输入			: SD卡识别结构体
//->输出			:	无
//->返回值			:	无
//->例程			:
//->调用函数		:
//->被调用函数	:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
SDCard_ErrorStatus	SDCard_Identification(SDCard_CardInfo_TypeDef *SDCard_CardInfo)
{	
	//2)__先发送V2.0版才具有的CMD8命令，然后检测返回值，
	//2.1.1)返回值若是0X01，则表示此卡为V2.0卡，
	//2.1.2)然后再发送循环命令CMD55+ACMD41，直到返回0x00，确定SD2.0卡初始化成功；
	//2.1.3)然后再发送CMD58命令，接收返回的OCR寄存器的数据，其中第31位用于判断V2.0的卡是否为SDHC类型。
	//2.2.1)返回值若不为0X01，则进一步判断是V1.0卡还是MMC卡
	//2.2.2)先发送循环命令CMD55+ACMD41进行复位，如果复位不成功则考虑是MMC卡，如果复位成功，则为V1.0卡。
	//2.2.3)在复位不成功的情况下，再使用CMD1进行复位，如果复位成功，则表明是MMC卡，如果复位不成功，则表示是无法识别的卡。
	
	//1)____________定义变量
	SDCard_ErrorStatus Errorstatus = SDCard_SUCCESS;				//SDCard_SUCCESS 无错误，SDCard_ERROR 错误
	unsigned char retry=0;							//用来进行超时计数
	unsigned char r1=0xFF;							//接收返回字
	
//	SDCard_DisSelect();									//取消片选,释放SPI总线--取消上次片选
//	SDCard_Select();										//使能sd卡,并且等待卡准备OK
	
	//2)____________发送CMD8命令,接收返回值
	retry=20;
	r1=0xFF;
	do
	{
//		SDCard_Select();										//使能sd卡,并且等待卡准备OK
		r1	=	SDCard_SendCmd(CMD8,0x01AA,0x87);
//		SDCard_DisSelect();									//取消片选,释放SPI总线--取消上次片选
	}while((r1!=0x01)&&retry--);
	//2)____________传送完CMD8数据
	if(r1==0x01)
	{
//		SDCard_Select();										//使能sd卡,并且等待卡准备OK
		SDCard_ReadWriteByte(0xFF);
		SDCard_ReadWriteByte(0xFF);
		SDCard_ReadWriteByte(0xFF);
		SDCard_ReadWriteByte(0xFF);
//		SDCard_DisSelect();									//取消片选,释放SPI总线--取消上次片选
	}
	else
	{
		SDCard_CardInfo->SD_Error	=	SD_ILLEGAL_CMD;					//小容量V2.0SD卡
		return	SDCard_ERROR;																//SDCard_SUCCESS 无错误，SDCard_ERROR 错误
	}
	//____________判断SD类型
	//2.1.1)返回值若是0X01，则表示此卡为V2.0卡，
	if(r1==0x01)				//V2.0卡
	{	
		//2.1.2)然后再发送循环命令CMD55+ACMD41，直到返回0x00，确定SD2.0卡初始化成功；		
		retry=100;
		r1=0xFF;
		do
		{
			SDCard_SendCmd(CMD55,0x00,0x01);
			r1	=	SDCard_SendCmd(CMD41,0X40000000,0x01);
		}while((r1!=0x00)&&retry--);		
		if(r1==0x00)		//V2.0SD卡复位成功，获取SDCard OCR信息，包括卡容量类型
		{
			//2.1.3)然后再发送CMD58命令，接收返回的OCR寄存器的数据，其中第31位用于判断V2.0的卡是否为SDHC类型。
			Errorstatus=SDCard_GetOCR(SDCard_CardInfo);
//			SDCard_DisSelect();									//取消片选,释放SPI总线--取消上次片选
			if(Errorstatus==SDCard_SUCCESS)
			return SDCard_SUCCESS;																	//SDCard_SUCCESS 无错误，SDCard_ERROR 错误
		}
		else		//复位失败
		{
			SDCard_CardInfo->SDCard_TYPE	=	SD_TYPE_V2;			//小容量V2.0SD卡
			SDCard_DisSelect();									//取消片选,释放SPI总线--取消上次片选
			return	SDCard_ERROR;																//SDCard_SUCCESS 无错误，SDCard_ERROR 错误
		}
	}	
	//2.2.1)返回值若不为0X01，则进一步判断是V1.0卡还是MMC卡
	else
	{
		//2.2.2)先发送循环命令CMD55+ACMD41进行复位，如果复位不成功则考虑是MMC卡，如果复位成功，则为V1.0卡。
		retry=100;
		r1=0xFF;
		do
		{
			SDCard_SendCmd(CMD55,0x00,0x01);
			r1	=	SDCard_SendCmd(CMD41,0X40000000,0x01);
		}while((r1!=0x00)&&retry--);
		if(r1==0x00)		//V2.0SD卡复位成功，获取SDCard OCR信息，包括卡容量类型
		{
			SDCard_GetOCR(SDCard_CardInfo);
		}
		//2.2.3)在复位不成功的情况下，再使用CMD1进行复位，如果复位成功，则表明是MMC卡，如果复位不成功，则表示是无法识别的卡。
		else
		{
			retry=100;
			r1=0xFF;
			do
			{
				SDCard_SendCmd(CMD1,0x00,0x00);
			}while((r1!=0x00)&&retry--);
			if(r1==0x00)		//卡复位成功，表示此卡为MMC卡
			{
				SDCard_CardInfo->SDCard_TYPE	=	SD_TYPE_MMC;			//MMC卡
			}
			else						//复位不成功，表示为无法识别的卡
			{
				SDCard_CardInfo->SDCard_TYPE	=	SD_TYPE_ERR;			//MMC卡
//				SDCard_DisSelect();									//取消片选,释放SPI总线--取消上次片选
				return	SDCard_ERROR;															//SDCard_SUCCESS 无错误，SDCard_ERROR 错误
			}
		}
	}
	//____________返回SD卡信息
	return SDCard_SUCCESS;																	//SDCard_SUCCESS 无错误，SDCard_ERROR 错误
}

//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->函数名			:	SDCard_ReadWriteByte
//->功能描述		:	读写一个字节
//->输入			: 写入一个字节
//->输出			:	读出的字节
//->返回值			:	无
//->例程			:
//->调用函数		:
//->被调用函数	:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
unsigned char	SDCard_ReadWriteByte(u8 TxData)
{
	//____________定义变量
	unsigned short retry=0;													//用来进行超时计数
	//____________等待发送缓冲区为空
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);	
//	while(SPI_I2S_GetFlagStatus(SDCard_SPI_PORT, SPI_I2S_FLAG_TXE) == RESET); 		//检查指令SPI发送标志是否为空
//	{
//		retry++;
//		if(retry>2000)
//			return 0;
//	}	
	//____________发送数据
	SPI_I2S_SendData(SDCard_SPI_PORT, TxData);				//发送数据
	//____________等待接收数据
//	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	retry=0;	
	while(SPI_I2S_GetFlagStatus(SDCard_SPI_PORT, SPI_I2S_FLAG_RXNE) == RESET)		//检查指令SPI接收完成标志设置与否
	{
		retry++;
		if(retry>1000)
			return 0;
	}
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	//____________返回接收到的数据
	return SPI_I2S_ReceiveData(SDCard_SPI_PORT); 			//返回接收到的数据	
}

//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->函数名			:	SDCard_SendCmd
//->功能描述		:	向SD卡发送一个命令
//->输入			: cmd	命令
//							arg 参数
//							crc	crc7
//->输出			:	无
//->返回值			:	无
//->例程			:
//->调用函数		:
//->被调用函数	:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
unsigned char SDCard_SendCmd(unsigned char cmd, unsigned int arg,unsigned char crc)
{
	//____________定义变量
	unsigned char r1;				//状态值
	unsigned char retry=0;	//用来进行超时计数
	//____________使能片选
//	SDCard_DisSelect();									//取消片选,释放SPI总线--取消上次片选
//	SDCard_Select();										//使能sd卡,并且等待卡准备OK
#ifndef	SDCard_DMA
	memset(SD_TX_Addr,0XFF,8);			//初始化缓冲
	memset(SD_TX_Addr,0XFF,8);			//初始化缓冲
	
	SD_TX_Addr[0]=cmd | 0x40;
	SD_TX_Addr[1]=arg >> 24;
	SD_TX_Addr[2]=arg >> 16;
	SD_TX_Addr[3]=arg >> 8;
	SD_TX_Addr[4]=arg;
	SD_TX_Addr[5]=crc;
	SD_TX_Addr[6]=0xFF;
	SD_TX_Addr[7]=0xFF;
	SDCard_DMASend(8);
#else
	//____________写入数据
	SDCard_ReadWriteByte(cmd | 0x40);	//分别写入命令
	SDCard_ReadWriteByte(arg >> 24);	//参数
	SDCard_ReadWriteByte(arg >> 16);	//
	SDCard_ReadWriteByte(arg >> 8);		
	SDCard_ReadWriteByte(arg);	  
	SDCard_ReadWriteByte(crc);				//CRC7
	
	if(cmd==CMD12)
		SDCard_ReadWriteByte(0xff);//Skip a stuff byte when stop reading
	//____________等待响应
	r1=0xFF;
	for(retry=0;retry<0x1F&&(r1&0x80);retry++)
	{
		r1=SDCard_ReadWriteByte(0xFF);
	}
#endif
//	Retry=0X1F;
//	do
//	{
//		r1=SDCard_ReadWriteByte(0xFF);
//	}while((r1&0X80) && Retry--);	

	//____________禁止片选	
//	SDCard_CS_OFF;											//GPIO_SetBits(SDCard_CS_PORT,	SDCard_CS_PIN)			//取消片选
//	SDCard_DisSelect();									//取消片选,释放SPI总线--取消上次片选
	//____________返回状态值
	return r1;	
}

//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->函数名			:	SDCard_GetCID
//->功能描述		:	获取SD卡的CID信息，包括制造商信息
//->输入			: CID_Buffer(存放CID的内存，至少16Byte）
//->输出			:	无
//->返回值			:	SD_OK 无错误
//							SD_ERROR 错误
//->例程			:
//->调用函数		:
//->被调用函数	:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
SDCard_ErrorStatus SDCard_GetCID(SDCard_CardInfo_TypeDef *SDCard_CardInfo)
{
	//____________定义变量
	SDCard_ErrorStatus Errorstatus = SDCard_SUCCESS;				//SD_OK 无错误，SD_ERROR 错误
	SDCard_CID_TypeDef	SDCard_CID;
	unsigned char R1;																		//状态值
	unsigned char R2;																		//状态值
	unsigned char	CID[20]={0xFF};
	unsigned char i;																		//
//	//____________使能片选
//	SDCard_Select();										//使能sd卡,并且等待卡准备OK	
	//____________发CMD10命令，读CID
	R1=SDCard_SendCmd(CMD10,0,0x01);
	//____________读CID
	if(R1==0x00)
	{
//		//____________使能片选
//		SDCard_Select();										//使能sd卡,并且等待卡准备OK	
		//____________返回R2,
		R2=SDCard_ReadWriteByte(0xFF);		
		//____________读取CSD
		for(i=0;i<19;i++)
		{
			CID[i]=SDCard_ReadWriteByte(0xFF);
		}
//		//____________禁止片选	
//		SDCard_DisSelect();									//取消片选,释放SPI总线
		//____________解析CID内容
		SDCard_CID.ManufacturerID=CID[0];
		SDCard_CID.OEM_AppliID=(((CID[1]&0X00FF)<<8)|(CID[2]&0X00FF));
//		SDCard_CID.ProdName1=((CID[3]<<24)|(CID[4]<<16)|(CID[5]<<8)|(CID[6]));
//		SDCard_CID.ProdName2=CID[7];
		SDCard_CID.ProdRev=CID[8];
		SDCard_CID.ProdSN=((CID[9]&0X000000FF<<24)|(CID[10]&0X000000FF<<16)|(CID[11]&0X000000FF<<8)|(CID[12]&0X000000FF));
		SDCard_CID.ManufactDate=((CID[13]&0X000F<<8)|(CID[14]&0X00FF));
		SDCard_CID.CID_CRC=(CID[15]&0xFE)>>1;
		//产品名称数组
		SDCard_CID.ProdName[0]=CID[3];
		SDCard_CID.ProdName[1]=CID[4];
		SDCard_CID.ProdName[2]=CID[5];
		SDCard_CID.ProdName[3]=CID[6];
		SDCard_CID.ProdName[4]=CID[7];
		
		//____________更新SDCard_CardInfo结构体参数
		SDCard_CardInfo->SDCard_CID=SDCard_CID;
		
		return	SDCard_SUCCESS;
	}
	else
	{
		return	SDCard_ERROR;
	}	
	//____________返回状态
	return(Errorstatus);
}
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->函数名			:	SDCard_GetCSD
//->功能描述		:	获取SD卡的CSD信息，包括容量和速度信息
//->输入			: CSD_Buffer (存放CSD的内存，至少16Byte）
//->输出			:	无
//->返回值			:	无
//->例程			:
//->调用函数		:
//->被调用函数	:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
SDCard_ErrorStatus SDCard_GetCSD(SDCard_CardInfo_TypeDef *SDCard_CardInfo)
{
	//____________定义变量
	SDCard_ErrorStatus	Errorstatus = SDCard_SUCCESS;		//SDCard_SUCCESS 无错误，SDCard_ERROR 错误
	SDCard_CSD_TypeDef	SDCard_CSD;
	unsigned char R1;																		//状态值
	unsigned char R2;																		//状态值
	unsigned char	CSD[20]={0xFF};
	unsigned char i;																		//
	//____________发CMD9命令，读CSD,	返回类型为R2
	R1=SDCard_SendCmd(CMD9,0,0x01);
	//____________读CSD
	if(R1==0x00)
	{
//		//____________使能片选
//		SDCard_Select();										//使能sd卡,并且等待卡准备OK	
		//____________返回R2,
		R2=SDCard_ReadWriteByte(0xFF);		
		//____________读取CSD
		for(i=0;i<19;i++)
		{
			CSD[i]=SDCard_ReadWriteByte(0xFF);
		}		
//		//____________禁止片选	
//		SDCard_DisSelect();									//取消片选,释放SPI总线
		//____________解析CSD内容	V2.0版本
		SDCard_CSD.CSD_STRUCTURE			=	(CSD[0]>>6)&0x03;			
		SDCard_CSD.TAAC								=	CSD[1];														
		SDCard_CSD.NSAC								=	CSD[2];													
		SDCard_CSD.TRAN_SPEED					=	CSD[3];														
		SDCard_CSD.CCC								=	((0X0FFF&CSD[4])<<4)|((CSD[5]>>4)&0X0F);		
		SDCard_CSD.READ_BL_LEN				=	CSD[5]&0x0F;				
		SDCard_CSD.READ_BL_PARTIAL		=	CSD[6]>>7;				
		SDCard_CSD.WRITE_BLK_MISALIGN	=	(CSD[6]&0x40>>6);	
		SDCard_CSD.READ_BLK_MISALIGN	=	(CSD[6]&0x20>>5);	
		SDCard_CSD.DSR_IMP						=	(CSD[6]&0x10>>4);		
		SDCard_CSD.C_SIZE							=	((CSD[7]&0x003F)<<16)|((CSD[8]&0x00FF)<<8)|((CSD[9]&0x00FF));
		SDCard_CSD.ERASE_BLK_EN				=	CSD[10]&40>>6;
		SDCard_CSD.SECTOR_SIZE				=	(CSD[10]&0x3F)<<1|(CSD[11]&0x80)>>7;
		SDCard_CSD.WP_GRP_SIZE				=	CSD[11]&0x7F;
		SDCard_CSD.WP_GRP_ENABLE			=	(CSD[12]&0x80)>>7;
		SDCard_CSD.R2W_FACTOR					=	(CSD[12]&0x1C)>>2;
		SDCard_CSD.WRITE_BL_LEN				=	(CSD[12]&0x03)<<2|(CSD[13]&0xC0)>>6;
		SDCard_CSD.WRITE_BL_PARTIAL		=	(CSD[13]&0x20)>>5;
		SDCard_CSD.FILE_FORMAT_GRP		=	(CSD[14]&0x80)>>7;
		SDCard_CSD.COPY								=	(CSD[14]&0x40)>>6;
		SDCard_CSD.PERM_WRITE_PROTECT	=	(CSD[14]&0x20)>>5;
		SDCard_CSD.TMP_WRITE_PROTECT	=	(CSD[14]&0x10)>>4;
		SDCard_CSD.FILE_FORMAT				=	(CSD[14]&0x0C)>>2;
		SDCard_CSD.CSD_CRC						=	(CSD[15]&0xFE)>>1;	

		//____________更新SDCard_CardInfo结构体参数
		SDCard_CardInfo->SDCard_CSD=SDCard_CSD;
		//____________SD卡容量计算
		//memory capacity = (C_SIZE+1) * 512K byte 单位Kb
		SDCard_CardInfo->CardCapacity=((SDCard_CSD.C_SIZE+1)*512)/1024;		//单位MB
		
		return	SDCard_SUCCESS;
	}

	else
	{
		return	SDCard_ERROR;
	}
	//____________返回状态
	return(Errorstatus);
}
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->函数名			:	SDCard_GetOCR
//->功能描述		:	获取SD卡的OSR信息，包括容量和速度信息和操作电压R3返回类型 
//							在SD卡上电后发送CMD58(SPI Mode Only)
//->输入			: 
//->输出			:	无
//->返回值			:	无
//->例程			:
//->调用函数		:
//->被调用函数	:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
SDCard_ErrorStatus	SDCard_GetOCR(SDCard_CardInfo_TypeDef *SDCard_CardInfo)
{
	unsigned char r3[4]={0xFF};
	//2.1.3)然后再发送CMD58命令，接收返回的OCR寄存器的数据，其中第31位用于判断V2.0的卡是否为SDHC类型。
	SDCard_SendCmd(CMD58,0x00,0x00);
	
//	SDCard_Select();										//使能sd卡,并且等待卡准备OK
	r3[0]	=	SDCard_ReadWriteByte(0xFF);
	r3[1]	=	SDCard_ReadWriteByte(0xFF);
	r3[2]	=	SDCard_ReadWriteByte(0xFF);
	r3[3]	=	SDCard_ReadWriteByte(0xFF);
//	SDCard_DisSelect();									//取消片选,释放SPI总线--取消上次片选
	
	if((r3[0]&0x40)==0x40)	//大容量卡
		SDCard_CardInfo->SDCard_TYPE=SD_TYPE_V2HC;					//大容量V2.0SD卡
	else
		SDCard_CardInfo->SDCard_TYPE=SD_TYPE_V2MC;					//小容量V2.0SD卡
	return	SDCard_SUCCESS;						
}
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->函数名			:	SDCard_GetOCR
//->功能描述		:	获取SD卡的OSR信息，包括容量和速度信息和操作电压R3返回类型 
//							在SD卡上电后发送CMD58(SPI Mode Only)
//->输入			: 
//->输出			:	无
//->返回值			:	无
//->例程			:
//->调用函数		:
//->被调用函数	:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
SDCard_ErrorStatus SDCard_ReadSingleBlock(u32 addr,unsigned char *Rxbuf, u16 BlockSize)
{
	//____________定义变量
	SDCard_ErrorStatus	Errorstatus = SDCard_SUCCESS;		//SDCard_SUCCESS 无错误，SDCard_ERROR 错误

	unsigned char retry=0;
	unsigned char R1;																		//状态值
	unsigned char R2;																		//状态值
	
#ifdef	SDCard_DMA
//	//____________更新SD卡读写状态
//		SDCard_Status=SDCard_DMA_DIS;
	if(SDCard_Status!=SDCard_DMA_EN)								//DMA关闭状态，可以使用
	{
		//____________使能片选
			SDCard_Select();																		//使能sd卡,并且等待卡准备OK
		//____________发CMD17命令，读sector 如果有错误，重试20次
		retry=100;
		do
		{
			R1=SDCard_SendCmd(CMD17,addr,0x01);
		}while(R1!=0x00&&retry--);
		//____________返回错误	
		if(retry==0x00)			//返回错误
		{
			
			return	SDCard_ERROR;	
		}
		//____________返回正常，开始读数据	
		else
		{
			
			
			//____________//等待SD卡发回数据起始令牌0xFE
			if(SDCard_GetResponse(0xFE)==SDCard_ERROR)
			{
				//____________禁止片选	
				SDCard_DisSelect();	
				return SDCard_ERROR;//等待SD卡发回数据起始令牌0xFE
			}
				memset(SD_TX_Addr,0XFF,BlockSize);						//初始化发送缓冲区
				memset(SD_RX_Addr,0XFF,BlockSize);						//初始化缓冲
				
				Rxbuf=SD_RX_Addr;		
				
				DMA1_Channel3->CNDTR =BlockSize+5;						//设定待发送缓冲区大小
				DMA1_Channel3->CMAR =(u32)SD_TX_Addr;		//发送缓冲区	
				
				DMA1_Channel2->CNDTR =BlockSize+5;						//设定接收缓冲区大小
				DMA1_Channel2->CMAR =(u32)SD_RX_Addr;		//接收缓冲区
				
				DMA_Cmd(DMA1_Channel3,ENABLE);					//DMA发送开启3
				DMA_Cmd(DMA1_Channel2,ENABLE);					//DMA发送开启2
				
				//____________更新SD卡读写状态
				SDCard_Status=SDCard_DMA_EN;
				
				return SDCard_SUCCESS;									//读取成功	
			}
	}
	else
	{		
		return	SDCard_ERROR;
	}
#else
		//____________发CMD17命令，读sector 如果有错误，重试20次
	retry=100;
	do
	{
		R1=SDCard_SendCmd(CMD17,addr,0x01);
	}while(R1!=0x00&&retry--);
	//____________返回错误	
	if(retry==0x00)			//返回错误
	{
		
		return	SDCard_ERROR;	
	}
	//____________返回正常，开始读数据	
	else
	{
		SDCard_RecvData((unsigned char*)Rxbuf,BlockSize);											//从sd卡读取一个数据包的内容
	}
#endif
	
	return	SDCard_SUCCESS;						
}















/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : SD_Init
//->Description    : Initializes the SD Card and put it into StandBy State (Ready 
//->                 for data transfer).
//->Input          : None
//->Output         : None
//->Return         : SD_Error: SD Card Error code.
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=

SD_Error SD_Init(void)
{

  SD_Error errorstatus = SD_OK;

  {
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_ResetBits(GPIOB, GPIO_Pin_5) ;
  }
  
  //____________Configure SDIO interface GPIO 
//  GPIO_Configuration();

  //____________Enable the SDIO AHB Clock 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SDIO, ENABLE);

  //____________Enable the DMA2 Clock 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

  SDIO_DeInit();

  errorstatus = SD_PowerON();

  if (errorstatus != SD_OK)
  {
    //____________CMD Response TimeOut (wait for CMDSENT flag)
    return(errorstatus);
  }

  errorstatus = SD_InitializeCards();

  if (errorstatus != SD_OK)
  {
    //____________CMD Response TimeOut (wait for CMDSENT flag)
    return(errorstatus);
  }

  //____________Configure the SDIO peripheral
  //____________HCLK = 72 MHz, SDIOCLK = 72 MHz, SDIO_CK = HCLK/(2 + 1) = 24 MHz   
  SDIO_InitStructure.SDIO_ClockDiv = SDIO_TRANSFER_CLK_DIV; 
  SDIO_InitStructure.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
  SDIO_InitStructure.SDIO_ClockBypass = SDIO_ClockBypass_Disable;
  SDIO_InitStructure.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
  SDIO_InitStructure.SDIO_BusWide = SDIO_BusWide_1b;
  SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;
  SDIO_Init(&SDIO_InitStructure);

  return(errorstatus);
}
*/

/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : SD_PowerON
//->Description    : Enquires cards about their operating voltage and configures 
//->                 clock controls.
//->Input          : None
//->Output         : None
//->Return         : SD_Error: SD Card Error code.
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
SD_Error SD_PowerON(void)
{
  SD_Error errorstatus = SD_OK;
  u32 response = 0, count = 0;
  bool validvoltage = FALSE;
  u32 SDType = SD_STD_CAPACITY;

  //____________Power ON Sequence
  //____________Configure the SDIO peripheral
  SDIO_InitStructure.SDIO_ClockDiv = SDIO_INIT_CLK_DIV; 											//HCLK = 72MHz, SDIOCLK = 72MHz, SDIO_CK = HCLK/(178 + 2) = 400 KHz
  SDIO_InitStructure.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
  SDIO_InitStructure.SDIO_ClockBypass = SDIO_ClockBypass_Disable;
  SDIO_InitStructure.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
  SDIO_InitStructure.SDIO_BusWide = SDIO_BusWide_1b;
  SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;
  SDIO_Init(&SDIO_InitStructure);

  //____________Set Power State to ON
  SDIO_SetPowerState(SDIO_PowerState_ON);

  //____________Enable SDIO Clock
  SDIO_ClockCmd(ENABLE);

  //____________CMD0: GO_IDLE_STATE
  //____________No CMD response required
  SDIO_CmdInitStructure.SDIO_Argument = 0x0;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_GO_IDLE_STATE;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_No;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdError();

  if (errorstatus != SD_OK)
  {
    //____________CMD Response TimeOut (wait for CMDSENT flag)
    return(errorstatus);
  }

  //____________CMD8: SEND_IF_COND
  //____________Send CMD8 to verify SD card interface operating condition
  //____________Argument: - [31:12]: Reserved (shall be set to '0')
  //            - [11:8]: Supply Voltage (VHS) 0x1 (Range: 2.7-3.6 V)
  //            - [7:0]: Check Pattern (recommended 0xAA)
  //____________CMD Response: R7
  SDIO_CmdInitStructure.SDIO_Argument = SD_CHECK_PATTERN;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_SEND_IF_COND;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdResp7Error();

  if (errorstatus == SD_OK)
  {
    CardType = SDIO_STD_CAPACITY_SD_CARD_V2_0; 			// SD Card 2.0
    SDType = SD_HIGH_CAPACITY;
  }
  else
  {
    //____________CMD55
    SDIO_CmdInitStructure.SDIO_Argument = 0x00;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_APP_CMD;
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);
    errorstatus = CmdResp1Error(SDIO_APP_CMD);
  }
  //____________CMD55
  SDIO_CmdInitStructure.SDIO_Argument = 0x00;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_APP_CMD;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);
  errorstatus = CmdResp1Error(SDIO_APP_CMD);

  //____________If errorstatus is Command TimeOut, it is a MMC card
  //____________If errorstatus is SD_OK it is a SD card: SD card 2.0 (voltage range mismatch)
  //						or SD card 1.x
  if (errorstatus == SD_OK)
  {
    //____________SD CARD
    //____________Send ACMD41 SD_APP_OP_COND with Argument 0x80100000
    while ((!validvoltage) && (count < SD_MAX_VOLT_TRIAL))
    {

      //____________SEND CMD55 APP_CMD with RCA as 0
      SDIO_CmdInitStructure.SDIO_Argument = 0x00;
      SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_APP_CMD;
      SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStructure);

      errorstatus = CmdResp1Error(SDIO_APP_CMD);

      if (errorstatus != SD_OK)
      {
        return(errorstatus);
      }
      SDIO_CmdInitStructure.SDIO_Argument = SD_VOLTAGE_WINDOW_SD | SDType;
      SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_SD_APP_OP_COND;
      SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStructure);

      errorstatus = CmdResp3Error();
      if (errorstatus != SD_OK)
      {
        return(errorstatus);
      }

      response = SDIO_GetResponse(SDIO_RESP1);
      validvoltage = (bool) (((response >> 31) == 1) ? 1 : 0);
      count++;
    }
    if (count >= SD_MAX_VOLT_TRIAL)
    {
      errorstatus = SD_INVALID_VOLTRANGE;
      return(errorstatus);
    }

    if (response &= SD_HIGH_CAPACITY)
    {
      CardType = SDIO_HIGH_CAPACITY_SD_CARD;
    }

  }//____________else MMC Card

  return(errorstatus);
}
*/

/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : SD_PowerOFF
//->Description    : Turns the SDIO output signals off.
//->Input          : None
//->Output         : None
//->Return         : SD_Error: SD Card Error code.
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
SD_Error SD_PowerOFF(void)
{
  SD_Error errorstatus = SD_OK;

  //____________Set Power State to OFF
  SDIO_SetPowerState(SDIO_PowerState_OFF);

  return(errorstatus);
}
*/
/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : SD_InitializeCards
//->Description    : Intialises all cards or single card as the case may be. 
//->                 Card(s) come into standby state.
//->Input          : None
//->Output         : None
//->Return         : SD_Error: SD Card Error code.
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
SD_Error SD_InitializeCards(void)
{
  SD_Error errorstatus = SD_OK;
  u16 rca = 0x01;

  if (SDIO_GetPowerState() == SDIO_PowerState_OFF)
  {
    errorstatus = SD_REQUEST_NOT_APPLICABLE;
    return(errorstatus);
  }

  if (SDIO_SECURE_DIGITAL_IO_CARD != CardType)
  {
    //____________Send CMD2 ALL_SEND_CID
    SDIO_CmdInitStructure.SDIO_Argument = 0x0;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_ALL_SEND_CID;
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Long;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);

    errorstatus = CmdResp2Error();

    if (SD_OK != errorstatus)
    {
      return(errorstatus);
    }

    CID_Tab[0] = SDIO_GetResponse(SDIO_RESP1);
    CID_Tab[1] = SDIO_GetResponse(SDIO_RESP2);
    CID_Tab[2] = SDIO_GetResponse(SDIO_RESP3);
    CID_Tab[3] = SDIO_GetResponse(SDIO_RESP4);
  }
  if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) ||  (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) ||  (SDIO_SECURE_DIGITAL_IO_COMBO_CARD == CardType)
      ||  (SDIO_HIGH_CAPACITY_SD_CARD == CardType))
  {
    //____________Send CMD3 SET_REL_ADDR with argument 0
    //____________SD Card publishes its RCA.
    SDIO_CmdInitStructure.SDIO_Argument = 0x00;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_SET_REL_ADDR;
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);

    errorstatus = CmdResp6Error(SDIO_SET_REL_ADDR, &rca);

    if (SD_OK != errorstatus)
    {
      return(errorstatus);
    }
  }

  if (SDIO_SECURE_DIGITAL_IO_CARD != CardType)
  {
    RCA = rca;

    //____________Send CMD9 SEND_CSD with argument as card's RCA
    SDIO_CmdInitStructure.SDIO_Argument = (u32)(rca << 16);
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_SEND_CSD;
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Long;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);

    errorstatus = CmdResp2Error();

    if (SD_OK != errorstatus)
    {
      return(errorstatus);
    }

    CSD_Tab[0] = SDIO_GetResponse(SDIO_RESP1);
    CSD_Tab[1] = SDIO_GetResponse(SDIO_RESP2);
    CSD_Tab[2] = SDIO_GetResponse(SDIO_RESP3);
    CSD_Tab[3] = SDIO_GetResponse(SDIO_RESP4);
  }

  errorstatus = SD_OK; 		//All cards get intialized

  return(errorstatus);
}
*/
/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : SD_GetCardInfo
//->Description    : Returns information about specific card.
//->Input          : cardinfo : pointer to a SD_CardInfo structure 
//->                 that contains all SD card information.
//->Output         : None
//->Return         : SD_Error: SD Card Error code.
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
SD_Error SD_GetCardInfo(SD_CardInfo *cardinfo)
{
  SD_Error errorstatus = SD_OK;
  u8 tmp = 0;

  cardinfo->CardType = (u8)CardType;
  cardinfo->RCA = (u16)RCA;

  //____________Byte 0
  tmp = (u8)((CSD_Tab[0] & 0xFF000000) >> 24);
  cardinfo->SD_csd.CSDStruct = (tmp & 0xC0) >> 6;
  cardinfo->SD_csd.SysSpecVersion = (tmp & 0x3C) >> 2;
  cardinfo->SD_csd.Reserved1 = tmp & 0x03;

  //____________Byte 1
  tmp = (u8)((CSD_Tab[0] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.TAAC = tmp;

  //____________Byte 2
  tmp = (u8)((CSD_Tab[0] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.NSAC = tmp;

  //____________Byte 3
  tmp = (u8)(CSD_Tab[0] & 0x000000FF);
  cardinfo->SD_csd.MaxBusClkFrec = tmp;

  //____________Byte 4
  tmp = (u8)((CSD_Tab[1] & 0xFF000000) >> 24);
  cardinfo->SD_csd.CardComdClasses = tmp << 4;

  //____________Byte 5
  tmp = (u8)((CSD_Tab[1] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.CardComdClasses |= (tmp & 0xF0) >> 4;
  cardinfo->SD_csd.RdBlockLen = tmp & 0x0F;

  //____________Byte 6
  tmp = (u8)((CSD_Tab[1] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.PartBlockRead = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.WrBlockMisalign = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.RdBlockMisalign = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.DSRImpl = (tmp & 0x10) >> 4;
  cardinfo->SD_csd.Reserved2 = 0; 			//Reserved

  if ((CardType == SDIO_STD_CAPACITY_SD_CARD_V1_1) || (CardType == SDIO_STD_CAPACITY_SD_CARD_V2_0))
  {
    cardinfo->SD_csd.DeviceSize = (tmp & 0x03) << 10;

    //____________Byte 7
    tmp = (u8)(CSD_Tab[1] & 0x000000FF);
    cardinfo->SD_csd.DeviceSize |= (tmp) << 2;

    //____________Byte 8
    tmp = (u8)((CSD_Tab[2] & 0xFF000000) >> 24);
    cardinfo->SD_csd.DeviceSize |= (tmp & 0xC0) >> 6;

    cardinfo->SD_csd.MaxRdCurrentVDDMin = (tmp & 0x38) >> 3;
    cardinfo->SD_csd.MaxRdCurrentVDDMax = (tmp & 0x07);

    //____________Byte 9
    tmp = (u8)((CSD_Tab[2] & 0x00FF0000) >> 16);
    cardinfo->SD_csd.MaxWrCurrentVDDMin = (tmp & 0xE0) >> 5;
    cardinfo->SD_csd.MaxWrCurrentVDDMax = (tmp & 0x1C) >> 2;
    cardinfo->SD_csd.DeviceSizeMul = (tmp & 0x03) << 1;
    //____________Byte 10
    tmp = (u8)((CSD_Tab[2] & 0x0000FF00) >> 8);
    cardinfo->SD_csd.DeviceSizeMul |= (tmp & 0x80) >> 7;
    
    cardinfo->CardCapacity = (cardinfo->SD_csd.DeviceSize + 1) ;
    cardinfo->CardCapacity *= (1 << (cardinfo->SD_csd.DeviceSizeMul + 2));
    cardinfo->CardBlockSize = 1 << (cardinfo->SD_csd.RdBlockLen);
    cardinfo->CardCapacity *= cardinfo->CardBlockSize;
  }
  else if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
  {
    //____________Byte 7
    tmp = (u8)(CSD_Tab[1] & 0x000000FF);
    cardinfo->SD_csd.DeviceSize = (tmp & 0x3F) << 16;

    //____________Byte 8
    tmp = (u8)((CSD_Tab[2] & 0xFF000000) >> 24);

    cardinfo->SD_csd.DeviceSize |= (tmp << 8);

    //____________Byte 9
    tmp = (u8)((CSD_Tab[2] & 0x00FF0000) >> 16);

    cardinfo->SD_csd.DeviceSize |= (tmp);

    //____________Byte 10
    tmp = (u8)((CSD_Tab[2] & 0x0000FF00) >> 8);
    
    cardinfo->CardCapacity = (cardinfo->SD_csd.DeviceSize + 1) * 512 * 1024;
    cardinfo->CardBlockSize = 512;    
  }


  cardinfo->SD_csd.EraseGrSize = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.EraseGrMul = (tmp & 0x3F) << 1;

  //____________Byte 11
  tmp = (u8)(CSD_Tab[2] & 0x000000FF);
  cardinfo->SD_csd.EraseGrMul |= (tmp & 0x80) >> 7;
  cardinfo->SD_csd.WrProtectGrSize = (tmp & 0x7F);

  //____________Byte 12
  tmp = (u8)((CSD_Tab[3] & 0xFF000000) >> 24);
  cardinfo->SD_csd.WrProtectGrEnable = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.ManDeflECC = (tmp & 0x60) >> 5;
  cardinfo->SD_csd.WrSpeedFact = (tmp & 0x1C) >> 2;
  cardinfo->SD_csd.MaxWrBlockLen = (tmp & 0x03) << 2;

  //____________Byte 13
  tmp = (u8)((CSD_Tab[3] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.MaxWrBlockLen |= (tmp & 0xC0) >> 6;
  cardinfo->SD_csd.WriteBlockPaPartial = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.Reserved3 = 0;
  cardinfo->SD_csd.ContentProtectAppli = (tmp & 0x01);

  //____________Byte 14
  tmp = (u8)((CSD_Tab[3] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.FileFormatGrouop = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.CopyFlag = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.PermWrProtect = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.TempWrProtect = (tmp & 0x10) >> 4;
  cardinfo->SD_csd.FileFormat = (tmp & 0x0C) >> 2;
  cardinfo->SD_csd.ECC = (tmp & 0x03);

  //____________Byte 15
  tmp = (u8)(CSD_Tab[3] & 0x000000FF);
  cardinfo->SD_csd.CSD_CRC = (tmp & 0xFE) >> 1;
  cardinfo->SD_csd.Reserved4 = 1;


  //____________Byte 0
  tmp = (u8)((CID_Tab[0] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ManufacturerID = tmp;

  //____________Byte 1
  tmp = (u8)((CID_Tab[0] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.OEM_AppliID = tmp << 8;

  //____________Byte 2
  tmp = (u8)((CID_Tab[0] & 0x000000FF00) >> 8);
  cardinfo->SD_cid.OEM_AppliID |= tmp;

  //____________Byte 3
  tmp = (u8)(CID_Tab[0] & 0x000000FF);
  cardinfo->SD_cid.ProdName1 = tmp << 24;

  //____________Byte 4
  tmp = (u8)((CID_Tab[1] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdName1 |= tmp << 16;

  //____________Byte 5
  tmp = (u8)((CID_Tab[1] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.ProdName1 |= tmp << 8;

  //____________Byte 6
  tmp = (u8)((CID_Tab[1] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ProdName1 |= tmp;

  //____________Byte 7
  tmp = (u8)(CID_Tab[1] & 0x000000FF);
  cardinfo->SD_cid.ProdName2 = tmp;

  //____________Byte 8
  tmp = (u8)((CID_Tab[2] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdRev = tmp;

  //____________Byte 9
  tmp = (u8)((CID_Tab[2] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.ProdSN = tmp << 24;

  //____________Byte 10
  tmp = (u8)((CID_Tab[2] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ProdSN |= tmp << 16;

  //____________Byte 11
  tmp = (u8)(CID_Tab[2] & 0x000000FF);
  cardinfo->SD_cid.ProdSN |= tmp << 8;

  //____________Byte 12
  tmp = (u8)((CID_Tab[3] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdSN |= tmp;

  //____________Byte 13
  tmp = (u8)((CID_Tab[3] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.Reserved1 |= (tmp & 0xF0) >> 4;
  cardinfo->SD_cid.ManufactDate = (tmp & 0x0F) << 8;

  //____________Byte 14
  tmp = (u8)((CID_Tab[3] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ManufactDate |= tmp;

  //____________Byte 15
  tmp = (u8)(CID_Tab[3] & 0x000000FF);
  cardinfo->SD_cid.CID_CRC = (tmp & 0xFE) >> 1;
  cardinfo->SD_cid.Reserved2 = 1;
  
  return(errorstatus);
}
*/

/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : SD_EnableWideBusOperation
//->Description    : Enables wide bus opeartion for the requeseted card if 
//->                 supported by card.
//->Input          : WideMode: Specifies the SD card wide bus mode. 
//->                    This parameter can be one of the following values:
//->                      - SDIO_BusWide_8b: 8-bit data transfer (Only for MMC)
//->                      - SDIO_BusWide_4b: 4-bit data transfer
//->                      - SDIO_BusWide_1b: 1-bit data transfer
//->Output         : None
//->Return         : SD_Error: SD Card Error code.
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
SD_Error SD_EnableWideBusOperation(u32 WideMode)
{
  SD_Error errorstatus = SD_OK;

  //____________MMC Card doesn't support this feature
  if (SDIO_MULTIMEDIA_CARD == CardType)
  {
    errorstatus = SD_UNSUPPORTED_FEATURE;
    return(errorstatus);
  }
  else if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType))
  {
    if (SDIO_BusWide_8b == WideMode)
    {
      errorstatus = SD_UNSUPPORTED_FEATURE;
      return(errorstatus);
    }
    else if (SDIO_BusWide_4b == WideMode)
    {
      errorstatus = SDEnWideBus(ENABLE);

      if (SD_OK == errorstatus)
      {
        //____________Configure the SDIO peripheral
        SDIO_InitStructure.SDIO_ClockDiv = SDIO_TRANSFER_CLK_DIV; 
        SDIO_InitStructure.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
        SDIO_InitStructure.SDIO_ClockBypass = SDIO_ClockBypass_Disable;
        SDIO_InitStructure.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
        SDIO_InitStructure.SDIO_BusWide = SDIO_BusWide_4b;
        SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;
        SDIO_Init(&SDIO_InitStructure);
      }
    }
    else
    {
      errorstatus = SDEnWideBus(DISABLE);

      if (SD_OK == errorstatus)
      {
        //____________Configure the SDIO peripheral
        SDIO_InitStructure.SDIO_ClockDiv = SDIO_TRANSFER_CLK_DIV; 
        SDIO_InitStructure.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
        SDIO_InitStructure.SDIO_ClockBypass = SDIO_ClockBypass_Disable;
        SDIO_InitStructure.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
        SDIO_InitStructure.SDIO_BusWide = SDIO_BusWide_1b;
        SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;
        SDIO_Init(&SDIO_InitStructure);
      }
    }
  }

  return(errorstatus);
}
*/

/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : SD_SetDeviceMode
//->Description    : Sets device mode whether to operate in Polling, Interrupt or
//->                 DMA mode.
//->Input          : Mode: Specifies the Data Transfer mode.
//->                    This parameter can be one of the following values:
//->                      - SD_DMA_MODE: Data transfer using DMA.
//->                      - SD_INTERRUPT_MODE: Data transfer using interrupts.
//->                      - SD_POLLING_MODE: Data transfer using flags.
//->Output         : None
//->Return         : SD_Error: SD Card Error code.
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
SD_Error SD_SetDeviceMode(u32 Mode)
{
  SD_Error errorstatus = SD_OK;

  if ((Mode == SD_DMA_MODE) || (Mode == SD_INTERRUPT_MODE) || (Mode == SD_POLLING_MODE))
  {
    DeviceMode = Mode;
  }
  else
  {
    errorstatus = SD_INVALID_PARAMETER;
  }
  return(errorstatus);

}
*/
/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : SD_SelectDeselect
//->Description    : Selects od Deselects the corresponding card.
//->Input          : addr: Address of the Card to be selected.
//->Output         : None
//->Return         : SD_Error: SD Card Error code.
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
SD_Error SD_SelectDeselect(u32 addr)
{
  SD_Error errorstatus = SD_OK;

  //____________Send CMD7 SDIO_SEL_DESEL_CARD
  SDIO_CmdInitStructure.SDIO_Argument =  addr;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_SEL_DESEL_CARD;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SDIO_SEL_DESEL_CARD);

  return(errorstatus);
}
*/
/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : SD_ReadBlock
//->Description    : Allows to read one block from a specified address in a card.
//->Input          : - addr: Address from where data are to be read.
//->                 - readbuff: pointer to the buffer that will contain the 
//->                   received data
//->                 - blocksize: the SD card Data block size.
//->Output         : None
//->Return         : SD_Error: SD Card Error code.
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
SD_Error SD_ReadBlock(u32 addr, u32 *readbuff, u16 BlockSize)
{
  SD_Error errorstatus = SD_OK;
  u32 count = 0, *tempbuff = readbuff;
  u8 power = 0;

  if (NULL == readbuff)
  {
    errorstatus = SD_INVALID_PARAMETER;
    return(errorstatus);
  }

  TransferError = SD_OK;
  TransferEnd = 0;
  TotalNumberOfBytes = 0;

  //____________Clear all DPSM configuration
  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DataLength = 0;
  SDIO_DataInitStructure.SDIO_DataBlockSize = SDIO_DataBlockSize_1b;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Disable;
  SDIO_DataConfig(&SDIO_DataInitStructure);
  SDIO_DMACmd(DISABLE);

  if (SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED)
  {
    errorstatus = SD_LOCK_UNLOCK_FAILED;
    return(errorstatus);
  }
  
  if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
  {
    BlockSize = 512;
    addr /= 512;
  }
  if ((BlockSize > 0) && (BlockSize <= 2048) && ((BlockSize & (BlockSize - 1)) == 0))
  {
    power = convert_from_bytes_to_power_of_two(BlockSize);

    //____________Set Block Size for Card
    SDIO_CmdInitStructure.SDIO_Argument = (u32) BlockSize;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_SET_BLOCKLEN;
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SDIO_SET_BLOCKLEN);

    if (SD_OK != errorstatus)
    {
      return(errorstatus);
    }
  }
  else
  {
    errorstatus = SD_INVALID_PARAMETER;
    return(errorstatus);
  }

  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DataLength = BlockSize;
  SDIO_DataInitStructure.SDIO_DataBlockSize = (u32) power << 4;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataConfig(&SDIO_DataInitStructure);

  TotalNumberOfBytes = BlockSize;
  StopCondition = 0;
  DestBuffer = readbuff;

  //____________Send CMD17 READ_SINGLE_BLOCK
  SDIO_CmdInitStructure.SDIO_Argument = (u32)addr;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_READ_SINGLE_BLOCK;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SDIO_READ_SINGLE_BLOCK);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }
  //____________In case of single block transfer, no need of stop transfer at all.
  if (DeviceMode == SD_POLLING_MODE)
  {
    //____________Polling mode
    while (!(SDIO->STA &(SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND | SDIO_FLAG_STBITERR)))
    {
      if (SDIO_GetFlagStatus(SDIO_FLAG_RXFIFOHF) != RESET)
      {
        for (count = 0; count < 8; count++)
        {
          *(tempbuff + count) = SDIO_ReadData();
        }
        tempbuff += 8;
      }
    }

    if (SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET)
    {
      SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT);
      errorstatus = SD_DATA_TIMEOUT;
      return(errorstatus);
    }
    else if (SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET)
    {
      SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);
      errorstatus = SD_DATA_CRC_FAIL;
      return(errorstatus);
    }
    else if (SDIO_GetFlagStatus(SDIO_FLAG_RXOVERR) != RESET)
    {
      SDIO_ClearFlag(SDIO_FLAG_RXOVERR);
      errorstatus = SD_RX_OVERRUN;
      return(errorstatus);
    }
    else if (SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET)
    {
      SDIO_ClearFlag(SDIO_FLAG_STBITERR);
      errorstatus = SD_START_BIT_ERR;
      return(errorstatus);
    }
    while (SDIO_GetFlagStatus(SDIO_FLAG_RXDAVL) != RESET)
    {
      *tempbuff = SDIO_ReadData();
      tempbuff++;
    }

    //____________Clear all the static flags
    SDIO_ClearFlag(SDIO_STATIC_FLAGS);
  }
  else if (DeviceMode == SD_INTERRUPT_MODE)
  {
    SDIO_ITConfig(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND | SDIO_IT_RXOVERR | SDIO_IT_RXFIFOHF | SDIO_IT_STBITERR, ENABLE);
    while ((TransferEnd == 0) && (TransferError == SD_OK))
    {}
    if (TransferError != SD_OK)
    {
      return(TransferError);
    }
  }
  else if (DeviceMode == SD_DMA_MODE)
  {
    SDIO_ITConfig(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND | SDIO_IT_RXOVERR | SDIO_IT_STBITERR, ENABLE);
    SDIO_DMACmd(ENABLE);
    DMA_RxConfiguration(readbuff, BlockSize);
    while (DMA_GetFlagStatus(DMA2_FLAG_TC4) == RESET)
    {}
  }
  return(errorstatus);
}
*/
/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : SD_ReadMultiBlocks
//->Description    : Allows to read blocks from a specified address  in a card.
//->Input          : - addr: Address from where data are to be read.
//->                 - readbuff: pointer to the buffer that will contain the 
//->                   received data.
//->                 - BlockSize: the SD card Data block size.
//->                 - NumberOfBlocks: number of blocks to be read.
//->Output         : None
//->Return         : SD_Error: SD Card Error code.
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
SD_Error SD_ReadMultiBlocks(u32 addr, u32 *readbuff, u16 BlockSize, u32 NumberOfBlocks)
{
  SD_Error errorstatus = SD_OK;
  u32 count = 0, *tempbuff = readbuff;
  u8 power = 0;

  if (NULL == readbuff)
  {
    errorstatus = SD_INVALID_PARAMETER;
    return(errorstatus);
  }

  TransferError = SD_OK;
  TransferEnd = 0;
  TotalNumberOfBytes = 0;

  //____________Clear all DPSM configuration
  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DataLength = 0;
  SDIO_DataInitStructure.SDIO_DataBlockSize = SDIO_DataBlockSize_1b;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Disable;
  SDIO_DataConfig(&SDIO_DataInitStructure);
  SDIO_DMACmd(DISABLE);

  if (SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED)
  {
    errorstatus = SD_LOCK_UNLOCK_FAILED;
    return(errorstatus);
  }

  if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
  {
    BlockSize = 512;
    addr /= 512;
  }
  
  if ((BlockSize > 0) && (BlockSize <= 2048) && (0 == (BlockSize & (BlockSize - 1))))
  {
    power = convert_from_bytes_to_power_of_two(BlockSize);

    //____________Set Block Size for Card
    SDIO_CmdInitStructure.SDIO_Argument = (u32) BlockSize;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_SET_BLOCKLEN;
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SDIO_SET_BLOCKLEN);

    if (SD_OK != errorstatus)
    {
      return(errorstatus);
    }
  }
  else
  {
    errorstatus = SD_INVALID_PARAMETER;
    return(errorstatus);
  }

  if (NumberOfBlocks > 1)
  {
    //____________Common to all modes
    if (NumberOfBlocks * BlockSize > SD_MAX_DATA_LENGTH)
    {
      errorstatus = SD_INVALID_PARAMETER;
      return(errorstatus);
    }

    TotalNumberOfBytes = NumberOfBlocks * BlockSize;
    StopCondition = 1;
    DestBuffer = readbuff;

    SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
    SDIO_DataInitStructure.SDIO_DataLength = NumberOfBlocks * BlockSize;
    SDIO_DataInitStructure.SDIO_DataBlockSize = (u32) power << 4;
    SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
    SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
    SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
    SDIO_DataConfig(&SDIO_DataInitStructure);

    //____________Send CMD18 READ_MULT_BLOCK with argument data address
    SDIO_CmdInitStructure.SDIO_Argument = (u32)addr;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_READ_MULT_BLOCK;
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SDIO_READ_MULT_BLOCK);

    if (errorstatus != SD_OK)
    {
      return(errorstatus);
    }

    if (DeviceMode == SD_POLLING_MODE)
    {
      //____________Polling mode
      while (!(SDIO->STA &(SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DATAEND | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_STBITERR)))
      {
        if (SDIO_GetFlagStatus(SDIO_FLAG_RXFIFOHF) != RESET)
        {
          for (count = 0; count < SD_HALFFIFO; count++)
          {
            *(tempbuff + count) = SDIO_ReadData();
          }
          tempbuff += SD_HALFFIFO;
        }
      }

      if (SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET)
      {
        SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT);
        errorstatus = SD_DATA_TIMEOUT;
        return(errorstatus);
      }
      else if (SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET)
      {
        SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);
        errorstatus = SD_DATA_CRC_FAIL;
        return(errorstatus);
      }
      else if (SDIO_GetFlagStatus(SDIO_FLAG_RXOVERR) != RESET)
      {
        SDIO_ClearFlag(SDIO_FLAG_RXOVERR);
        errorstatus = SD_RX_OVERRUN;
        return(errorstatus);
      }
      else if (SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET)
      {
        SDIO_ClearFlag(SDIO_FLAG_STBITERR);
        errorstatus = SD_START_BIT_ERR;
        return(errorstatus);
      }
      while (SDIO_GetFlagStatus(SDIO_FLAG_RXDAVL) != RESET)
      {
        *tempbuff = SDIO_ReadData();
        tempbuff++;
      }

      if (SDIO_GetFlagStatus(SDIO_FLAG_DATAEND) != RESET)
      {
        //____________In Case Of SD-CARD Send Command STOP_TRANSMISSION
        if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType))
        {
          //____________Send CMD12 STOP_TRANSMISSION
          SDIO_CmdInitStructure.SDIO_Argument = 0x0;
          SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_STOP_TRANSMISSION;
          SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
          SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
          SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
          SDIO_SendCommand(&SDIO_CmdInitStructure);

          errorstatus = CmdResp1Error(SDIO_STOP_TRANSMISSION);

          if (errorstatus != SD_OK)
          {
            return(errorstatus);
          }
        }
      }
      //____________Clear all the static flags
      SDIO_ClearFlag(SDIO_STATIC_FLAGS);
    }
    else if (DeviceMode == SD_INTERRUPT_MODE)
    {
      SDIO_ITConfig(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND | SDIO_IT_RXOVERR | SDIO_IT_RXFIFOHF | SDIO_IT_STBITERR, ENABLE);
      while ((TransferEnd == 0) && (TransferError == SD_OK))
      {}
      if (TransferError != SD_OK)
      {
        return(TransferError);
      }
    }
    else if (DeviceMode == SD_DMA_MODE)
    {
      SDIO_ITConfig(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND | SDIO_IT_RXOVERR | SDIO_IT_STBITERR, ENABLE);
      SDIO_DMACmd(ENABLE);
      DMA_RxConfiguration(readbuff, (NumberOfBlocks * BlockSize));
      while (DMA_GetFlagStatus(DMA2_FLAG_TC4) == RESET)
      {}
      while ((TransferEnd == 0) && (TransferError == SD_OK))
      {}
      if (TransferError != SD_OK)
      {
        return(TransferError);
      }
    }
  }
  return(errorstatus);
}
*/
/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : SD_WriteBlock
//->Description    : Allows to write one block starting from a specified address 
//->                 in a card.
//->Input          : - addr: Address from where data are to be read.
//->                 - writebuff: pointer to the buffer that contain the data to be
//->                  transferred.
//->                - BlockSize: the SD card Data block size.
//->Output         : None
//->Return         : SD_Error: SD Card Error code.
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
SD_Error SD_WriteBlock(u32 addr, u32 *writebuff, u16 BlockSize)
{
  SD_Error errorstatus = SD_OK;
  u8  power = 0, cardstate = 0;
  u32 timeout = 0, bytestransferred = 0;
  u32 cardstatus = 0, count = 0, restwords = 0;
  u32 *tempbuff = writebuff;

  if (writebuff == NULL)
  {
    errorstatus = SD_INVALID_PARAMETER;
    return(errorstatus);
  }

  TransferError = SD_OK;
  TransferEnd = 0;
  TotalNumberOfBytes = 0;

  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DataLength = 0;
  SDIO_DataInitStructure.SDIO_DataBlockSize = SDIO_DataBlockSize_1b;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Disable;
  SDIO_DataConfig(&SDIO_DataInitStructure);
  SDIO_DMACmd(DISABLE);

  if (SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED)
  {
    errorstatus = SD_LOCK_UNLOCK_FAILED;
    return(errorstatus);
  }

  if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
  {
    BlockSize = 512;
    addr /= 512;
  }
  
  //____________Set the block size, both on controller and card
  if ((BlockSize > 0) && (BlockSize <= 2048) && ((BlockSize & (BlockSize - 1)) == 0))
  {
    power = convert_from_bytes_to_power_of_two(BlockSize);

    SDIO_CmdInitStructure.SDIO_Argument = (u32) BlockSize;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_SET_BLOCKLEN;
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SDIO_SET_BLOCKLEN);

    if (errorstatus != SD_OK)
    {
      return(errorstatus);
    }
  }
  else
  {
    errorstatus = SD_INVALID_PARAMETER;
    return(errorstatus);
  }

  //____________Wait till card is ready for data Added
  SDIO_CmdInitStructure.SDIO_Argument = (u32) (RCA << 16);
  SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_SEND_STATUS;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SDIO_SEND_STATUS);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }

  cardstatus = SDIO_GetResponse(SDIO_RESP1);

  timeout = SD_DATATIMEOUT;

  while (((cardstatus & 0x00000100) == 0) && (timeout > 0))
  {
    timeout--;
    SDIO_CmdInitStructure.SDIO_Argument = (u32) (RCA << 16);
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_SEND_STATUS;
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SDIO_SEND_STATUS);

    if (errorstatus != SD_OK)
    {
      return(errorstatus);
    }
    cardstatus = SDIO_GetResponse(SDIO_RESP1);
  }

  if (timeout == 0)
  {
    return(SD_ERROR);
  }

  //____________Send CMD24 WRITE_SINGLE_BLOCK
  SDIO_CmdInitStructure.SDIO_Argument = addr;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_WRITE_SINGLE_BLOCK;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SDIO_WRITE_SINGLE_BLOCK);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }

  TotalNumberOfBytes = BlockSize;
  StopCondition = 0;
  SrcBuffer = writebuff;

  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DataLength = BlockSize;
  SDIO_DataInitStructure.SDIO_DataBlockSize = (u32) power << 4;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataConfig(&SDIO_DataInitStructure);

  //____________In case of single data block transfer no need of stop command at all
  if (DeviceMode == SD_POLLING_MODE)
  {
    while (!(SDIO->STA & (SDIO_FLAG_DBCKEND | SDIO_FLAG_TXUNDERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_STBITERR)))
    {
      if (SDIO_GetFlagStatus(SDIO_FLAG_TXFIFOHE) != RESET)
      {
        if ((TotalNumberOfBytes - bytestransferred) < 32)
        {
          restwords = ((TotalNumberOfBytes - bytestransferred) % 4 == 0) ? ((TotalNumberOfBytes - bytestransferred) / 4) : (( TotalNumberOfBytes -  bytestransferred) / 4 + 1);

          for (count = 0; count < restwords; count++, tempbuff++, bytestransferred += 4)
          {
            SDIO_WriteData(*tempbuff);
          }
        }
        else
        {
          for (count = 0; count < 8; count++)
          {
            SDIO_WriteData(*(tempbuff + count));
          }
          tempbuff += 8;
          bytestransferred += 32;
        }
      }
    }
    if (SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET)
    {
      SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT);
      errorstatus = SD_DATA_TIMEOUT;
      return(errorstatus);
    }
    else if (SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET)
    {
      SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);
      errorstatus = SD_DATA_CRC_FAIL;
      return(errorstatus);
    }
    else if (SDIO_GetFlagStatus(SDIO_FLAG_TXUNDERR) != RESET)
    {
      SDIO_ClearFlag(SDIO_FLAG_TXUNDERR);
      errorstatus = SD_TX_UNDERRUN;
      return(errorstatus);
    }
    else if (SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET)
    {
      SDIO_ClearFlag(SDIO_FLAG_STBITERR);
      errorstatus = SD_START_BIT_ERR;
      return(errorstatus);
    }
  }
  else if (DeviceMode == SD_INTERRUPT_MODE)
  {
    SDIO_ITConfig(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND | SDIO_FLAG_TXFIFOHE | SDIO_IT_TXUNDERR | SDIO_IT_STBITERR, ENABLE);
    while ((TransferEnd == 0) && (TransferError == SD_OK))
    {}
    if (TransferError != SD_OK)
    {
      return(TransferError);
    }
  }
  else if (DeviceMode == SD_DMA_MODE)
  {
    SDIO_ITConfig(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND | SDIO_IT_TXUNDERR | SDIO_IT_STBITERR, ENABLE);
    DMA_TxConfiguration(writebuff, BlockSize);
    SDIO_DMACmd(ENABLE);
    while (DMA_GetFlagStatus(DMA2_FLAG_TC4) == RESET)
    {}
    while ((TransferEnd == 0) && (TransferError == SD_OK))
    {}
    if (TransferError != SD_OK)
    {
      return(TransferError);
    }
  }

  //____________Clear all the static flags
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  //____________Wait till the card is in programming state
  errorstatus = IsCardProgramming(&cardstate);

  while ((errorstatus == SD_OK) && ((cardstate == SD_CARD_PROGRAMMING) || (cardstate == SD_CARD_RECEIVING)))
  {
    errorstatus = IsCardProgramming(&cardstate);
  }

  return(errorstatus);
}
*/
/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : SD_WriteMultiBlocks
//->Description    : Allows to write blocks starting from a specified address in 
//->                 a card.
//->Input          : - addr: Address from where data are to be read.
//->                 - writebuff: pointer to the buffer that contain the data to be
//->                   transferred.
//->                 - BlockSize: the SD card Data block size.
//->                 - NumberOfBlocks: number of blocks to be written.
//->Output         : None
//->Return         : SD_Error: SD Card Error code.
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
SD_Error SD_WriteMultiBlocks(u32 addr, u32 *writebuff, u16 BlockSize, u32 NumberOfBlocks)
{
  SD_Error errorstatus = SD_OK;
  u8  power = 0, cardstate = 0;
  u32 bytestransferred = 0;
  u32 count = 0, restwords = 0;
  u32 *tempbuff = writebuff;

  if (writebuff == NULL)
  {
    errorstatus = SD_INVALID_PARAMETER;
    return(errorstatus);
  }

  TransferError = SD_OK;
  TransferEnd = 0;
  TotalNumberOfBytes = 0;

  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DataLength = 0;
  SDIO_DataInitStructure.SDIO_DataBlockSize = SDIO_DataBlockSize_1b;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Disable;
  SDIO_DataConfig(&SDIO_DataInitStructure);
  SDIO_DMACmd(DISABLE);

  if (SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED)
  {
    errorstatus = SD_LOCK_UNLOCK_FAILED;
    return(errorstatus);
  }

  if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
  {
    BlockSize = 512;
    addr /= 512;
  }
  
  //____________Set the block size, both on controller and card
  if ((BlockSize > 0) && (BlockSize <= 2048) && ((BlockSize & (BlockSize - 1)) == 0))
  {
    power = convert_from_bytes_to_power_of_two(BlockSize);

    SDIO_CmdInitStructure.SDIO_Argument = (u32) BlockSize;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_SET_BLOCKLEN;
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SDIO_SET_BLOCKLEN);

    if (errorstatus != SD_OK)
    {
      return(errorstatus);
    }
  }
  else
  {
    errorstatus = SD_INVALID_PARAMETER;
    return(errorstatus);
  }

  //____________Wait till card is ready for data Added
  SDIO_CmdInitStructure.SDIO_Argument = (u32) (RCA << 16);
  SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_SEND_STATUS;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SDIO_SEND_STATUS);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }

  if (NumberOfBlocks > 1)
  {
    //____________ Common to all modes
    if (NumberOfBlocks * BlockSize > SD_MAX_DATA_LENGTH)
    {
      errorstatus = SD_INVALID_PARAMETER;
      return(errorstatus);
    }

    if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType))
    {
      //____________To improve performance
      SDIO_CmdInitStructure.SDIO_Argument = (u32) (RCA << 16);
      SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_APP_CMD;
      SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStructure);


      errorstatus = CmdResp1Error(SDIO_APP_CMD);

      if (errorstatus != SD_OK)
      {
        return(errorstatus);
      }
      //____________To improve performance
      SDIO_CmdInitStructure.SDIO_Argument = (u32)NumberOfBlocks;
      SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_SET_BLOCK_COUNT;
      SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStructure);

      errorstatus = CmdResp1Error(SDIO_SET_BLOCK_COUNT);

      if (errorstatus != SD_OK)
      {
        return(errorstatus);
      }
    }

    //____________Send CMD25 WRITE_MULT_BLOCK with argument data address
    SDIO_CmdInitStructure.SDIO_Argument = (u32)addr;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_WRITE_MULT_BLOCK;
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SDIO_WRITE_MULT_BLOCK);

    if (SD_OK != errorstatus)
    {
      return(errorstatus);
    }

    TotalNumberOfBytes = NumberOfBlocks * BlockSize;
    StopCondition = 1;
    SrcBuffer = writebuff;

    SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
    SDIO_DataInitStructure.SDIO_DataLength = NumberOfBlocks * BlockSize;
    SDIO_DataInitStructure.SDIO_DataBlockSize = (u32) power << 4;
    SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
    SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
    SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
    SDIO_DataConfig(&SDIO_DataInitStructure);

    if (DeviceMode == SD_POLLING_MODE)
    {
      while (!(SDIO->STA & (SDIO_FLAG_TXUNDERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DATAEND | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_STBITERR)))
      {
        if (SDIO_GetFlagStatus(SDIO_FLAG_TXFIFOHE) != RESET)
        {
          if (!((TotalNumberOfBytes - bytestransferred) < SD_HALFFIFOBYTES))
          {
            for (count = 0; count < SD_HALFFIFO; count++)
            {
              SDIO_WriteData(*(tempbuff + count));
            }
            tempbuff += SD_HALFFIFO;
            bytestransferred += SD_HALFFIFOBYTES;
          }
          else
          {
            restwords = ((TotalNumberOfBytes - bytestransferred) % 4 == 0) ? ((TotalNumberOfBytes - bytestransferred) / 4) :
                        ((TotalNumberOfBytes - bytestransferred) / 4 + 1);

            for (count = 0; count < restwords; count++, tempbuff++, bytestransferred += 4)
            {
              SDIO_WriteData(*tempbuff);
            }
          }
        }
      }

      if (SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET)
      {
        SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT);
        errorstatus = SD_DATA_TIMEOUT;
        return(errorstatus);
      }
      else if (SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET)
      {
        SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);
        errorstatus = SD_DATA_CRC_FAIL;
        return(errorstatus);
      }
      else if (SDIO_GetFlagStatus(SDIO_FLAG_TXUNDERR) != RESET)
      {
        SDIO_ClearFlag(SDIO_FLAG_TXUNDERR);
        errorstatus = SD_TX_UNDERRUN;
        return(errorstatus);
      }
      else if (SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET)
      {
        SDIO_ClearFlag(SDIO_FLAG_STBITERR);
        errorstatus = SD_START_BIT_ERR;
        return(errorstatus);
      }

      if (SDIO_GetFlagStatus(SDIO_FLAG_DATAEND) != RESET)
      {
       if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType))
        {
          //____________Send CMD12 STOP_TRANSMISSION
          SDIO_CmdInitStructure.SDIO_Argument = 0x0;
          SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_STOP_TRANSMISSION;
          SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
          SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
          SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
          SDIO_SendCommand(&SDIO_CmdInitStructure);


          errorstatus = CmdResp1Error(SDIO_STOP_TRANSMISSION);

          if (errorstatus != SD_OK)
          {
            return(errorstatus);
          }
        }
      }
    }
    else if (DeviceMode == SD_INTERRUPT_MODE)
    {
      SDIO_ITConfig(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND | SDIO_IT_TXFIFOHE | SDIO_IT_TXUNDERR | SDIO_IT_STBITERR, ENABLE);
      while ((TransferEnd == 0) && (TransferError == SD_OK))
      {}
      if (TransferError != SD_OK)
      {
        return(TransferError);
      }
    }
    else if (DeviceMode == SD_DMA_MODE)
    {
      SDIO_ITConfig(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND | SDIO_IT_TXUNDERR | SDIO_IT_STBITERR, ENABLE);
      SDIO_DMACmd(ENABLE);
      DMA_TxConfiguration(writebuff, (NumberOfBlocks * BlockSize));
      while (DMA_GetFlagStatus(DMA2_FLAG_TC4) == RESET)
      {}
      while ((TransferEnd == 0) && (TransferError == SD_OK))
      {}
      if (TransferError != SD_OK)
      {
        return(TransferError);
      }
    }
  }
  //____________Clear all the static flags
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  //____________Wait till the card is in programming state
  errorstatus = IsCardProgramming(&cardstate);

  while ((errorstatus == SD_OK) && ((cardstate == SD_CARD_PROGRAMMING) || (cardstate == SD_CARD_RECEIVING)))
  {
    errorstatus = IsCardProgramming(&cardstate);
  }

  return(errorstatus);
}
*/
/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : SD_GetTransferState
//->Description    : Gets the cuurent data transfer state.
//->Input          : None
//->Output         : None
//->Return         : SDTransferState: Data Transfer state.
//->                 This value can be: 
//->                  - SD_NO_TRANSFER: No data transfer is acting
//->                  - SD_TRANSFER_IN_PROGRESS: Data transfer is acting
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
SDTransferState SD_GetTransferState(void)
{
  if (SDIO->STA & (SDIO_FLAG_TXACT | SDIO_FLAG_RXACT))
  {
    return(SD_TRANSFER_IN_PROGRESS);
  }
  else
  {
    return(SD_NO_TRANSFER);
  }
}
*/
/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : SD_StopTransfer
//->Description    : Aborts an ongoing data transfer.
//->Input          : None
//->Output         : None
//->Return         : SD_Error: SD Card Error code.
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
SD_Error SD_StopTransfer(void)
{
  SD_Error errorstatus = SD_OK;

  //____________Send CMD12 STOP_TRANSMISSION
  SDIO_CmdInitStructure.SDIO_Argument = 0x0;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_STOP_TRANSMISSION;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SDIO_STOP_TRANSMISSION);

  return(errorstatus);
}
*/
/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : SD_Erase
//->Description    : Allows to erase memory area specified for the given card.
//->Input          : - startaddr: the start address.
//->                 - endaddr: the end address.
//->Output         : None
//->Return         : SD_Error: SD Card Error code.
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
SD_Error SD_Erase(u32 startaddr, u32 endaddr)
{
  SD_Error errorstatus = SD_OK;
  u32 delay = 0;
  vu32 maxdelay = 0;
  u8 cardstate = 0;

  //____________Check if the card coomnd class supports erase command
  if (((CSD_Tab[1] >> 20) & SD_CCCC_ERASE) == 0)
  {
    errorstatus = SD_REQUEST_NOT_APPLICABLE;
    return(errorstatus);
  }

  maxdelay = 72000 / ((SDIO->CLKCR & 0xFF) + 2);

  if (SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED)
  {
    errorstatus = SD_LOCK_UNLOCK_FAILED;
    return(errorstatus);
  }

  if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
  {
    startaddr /= 512;
    endaddr /= 512;
  }
  
  //____________According to sd-card spec 1.0 ERASE_GROUP_START (CMD32) and erase_group_end(CMD33)
  if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType))
  {
		//____________Send CMD32 SD_ERASE_GRP_START with argument as addr
    SDIO_CmdInitStructure.SDIO_Argument = startaddr;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_SD_ERASE_GRP_START;
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SDIO_SD_ERASE_GRP_START);
    if (errorstatus != SD_OK)
    {
      return(errorstatus);
    }

    //____________Send CMD33 SD_ERASE_GRP_END with argument as addr
    SDIO_CmdInitStructure.SDIO_Argument = endaddr;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_SD_ERASE_GRP_END;
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SDIO_SD_ERASE_GRP_END);
    if (errorstatus != SD_OK)
    {
      return(errorstatus);
    }
  }

  //____________Send CMD38 ERASE
  SDIO_CmdInitStructure.SDIO_Argument = 0;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_ERASE;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SDIO_ERASE);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }

  for (delay = 0; delay < maxdelay; delay++)
  {}

  //____________Wait till the card is in programming state
  errorstatus = IsCardProgramming(&cardstate);

  while ((errorstatus == SD_OK) && ((SD_CARD_PROGRAMMING == cardstate) || (SD_CARD_RECEIVING == cardstate)))
  {
    errorstatus = IsCardProgramming(&cardstate);
  }

  return(errorstatus);
}
*/
/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : SD_SendStatus
//->Description    : Returns the current card's status.
//->Input          : pcardstatus: pointer to the buffer that will contain the SD 
//->                 card status (Card Status register).
//->Output         : None
//->Return         : SD_Error: SD Card Error code.
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
SD_Error SD_SendStatus(u32 *pcardstatus)
{
  SD_Error errorstatus = SD_OK;

  if (pcardstatus == NULL)
  {
    errorstatus = SD_INVALID_PARAMETER;
    return(errorstatus);
  }

  SDIO_CmdInitStructure.SDIO_Argument = (u32) RCA << 16;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_SEND_STATUS;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);


  errorstatus = CmdResp1Error(SDIO_SEND_STATUS);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }

  *pcardstatus = SDIO_GetResponse(SDIO_RESP1);

  return(errorstatus);
}
*/
/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : SD_SendSDStatus
//->Description    : Returns the current SD card's status.
//->Input          : psdstatus: pointer to the buffer that will contain the SD 
//->                 card status (SD Status register).
//->Output         : None
//->Return         : SD_Error: SD Card Error code.
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
SD_Error SD_SendSDStatus(u32 *psdstatus)
{
  SD_Error errorstatus = SD_OK;
  u32 count = 0;

  if (SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED)
  {
    errorstatus = SD_LOCK_UNLOCK_FAILED;
    return(errorstatus);
  }

  //____________Set block size for card if it is not equal to current block size for card.
  SDIO_CmdInitStructure.SDIO_Argument = 64;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_SET_BLOCKLEN;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SDIO_SET_BLOCKLEN);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }

  //____________CMD55
  SDIO_CmdInitStructure.SDIO_Argument = (u32) RCA << 16;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_APP_CMD;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);
  errorstatus = CmdResp1Error(SDIO_APP_CMD);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }

  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DataLength = 64;
  SDIO_DataInitStructure.SDIO_DataBlockSize = SDIO_DataBlockSize_64b;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataConfig(&SDIO_DataInitStructure);


  //____________Send ACMD13 SD_APP_STAUS  with argument as card's RCA.
  SDIO_CmdInitStructure.SDIO_Argument = 0;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_SD_APP_STAUS;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);
  errorstatus = CmdResp1Error(SDIO_SD_APP_STAUS);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }

  while (!(SDIO->STA &(SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND | SDIO_FLAG_STBITERR)))
  {
    if (SDIO_GetFlagStatus(SDIO_FLAG_RXFIFOHF) != RESET)
    {
      for (count = 0; count < 8; count++)
      {
        *(psdstatus + count) = SDIO_ReadData();
      }
      psdstatus += 8;
    }
  }

  if (SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT);
    errorstatus = SD_DATA_TIMEOUT;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);
    errorstatus = SD_DATA_CRC_FAIL;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_RXOVERR) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_RXOVERR);
    errorstatus = SD_RX_OVERRUN;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_STBITERR);
    errorstatus = SD_START_BIT_ERR;
    return(errorstatus);
  }

  while (SDIO_GetFlagStatus(SDIO_FLAG_RXDAVL) != RESET)
  {
    *psdstatus = SDIO_ReadData();
    psdstatus++;
  }

  //____________Clear all the static status flags
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);
  psdstatus -= 16;
  for (count = 0; count < 16; count++)
  {
    psdstatus[count] = ((psdstatus[count] & SD_0TO7BITS) << 24) |((psdstatus[count] & SD_8TO15BITS) << 8) |
                       ((psdstatus[count] & SD_16TO23BITS) >> 8) |((psdstatus[count] & SD_24TO31BITS) >> 24);
  }
  return(errorstatus);
}
*/
/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : SD_ProcessIRQSrc
//->Description    : Allows to process all the interrupts that are high.
//->Input          : None
//->Output         : None
//->Return         : SD_Error: SD Card Error code.
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
SD_Error SD_ProcessIRQSrc(void)
{
  u32 count = 0, restwords = 0;

  if (DeviceMode == SD_INTERRUPT_MODE)
  {
    if (SDIO_GetITStatus(SDIO_IT_RXFIFOHF) != RESET)
    {
      for (count = 0; count < SD_HALFFIFO; count++)
      {
        *(DestBuffer + count) = SDIO_ReadData();
      }
      DestBuffer += SD_HALFFIFO;
      NumberOfBytes += SD_HALFFIFOBYTES;
    }
    else if (SDIO_GetITStatus(SDIO_IT_TXFIFOHE) != RESET)
    {
      if ((TotalNumberOfBytes - NumberOfBytes) < SD_HALFFIFOBYTES)
      {
        restwords = ((TotalNumberOfBytes - NumberOfBytes) %  4 == 0) ?
                    ((TotalNumberOfBytes - NumberOfBytes) / 4) :
                    ((TotalNumberOfBytes - NumberOfBytes) / 4 + 1);

        for (count = 0; count < restwords;  count++, SrcBuffer++, NumberOfBytes += 4)
        {
          SDIO_WriteData(*SrcBuffer);
        }
      }
      else
      {
        for (count = 0; count < SD_HALFFIFO; count++)
        {
          SDIO_WriteData(*(SrcBuffer + count));
        }

        SrcBuffer += SD_HALFFIFO;
        NumberOfBytes += SD_HALFFIFOBYTES;
      }
    }
  }

  if (SDIO_GetITStatus(SDIO_IT_DATAEND) != RESET)
  {
    if (DeviceMode != SD_DMA_MODE)
    {
      while ((SDIO_GetFlagStatus(SDIO_FLAG_RXDAVL) != RESET)  &&  (NumberOfBytes < TotalNumberOfBytes))
      {
        *DestBuffer = SDIO_ReadData();
        DestBuffer++;
        NumberOfBytes += 4;
      }
    }

    if (StopCondition == 1)
    {
      TransferError = SD_StopTransfer();
    }
    else
    {
      TransferError = SD_OK;
    }
    SDIO_ClearITPendingBit(SDIO_IT_DATAEND);
    SDIO_ITConfig(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND |
                  SDIO_IT_TXFIFOHE | SDIO_IT_RXFIFOHF | SDIO_IT_TXUNDERR |
                  SDIO_IT_RXOVERR | SDIO_IT_STBITERR, DISABLE);
    TransferEnd = 1;
    NumberOfBytes = 0;
    return(TransferError);
  }

  if (SDIO_GetITStatus(SDIO_IT_DCRCFAIL) != RESET)
  {
    SDIO_ClearITPendingBit(SDIO_IT_DCRCFAIL);
    SDIO_ITConfig(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND |
                  SDIO_IT_TXFIFOHE | SDIO_IT_RXFIFOHF | SDIO_IT_TXUNDERR |
                  SDIO_IT_RXOVERR | SDIO_IT_STBITERR, DISABLE);
    NumberOfBytes = 0;
    TransferError = SD_DATA_CRC_FAIL;
    return(SD_DATA_CRC_FAIL);
  }

  if (SDIO_GetITStatus(SDIO_IT_DTIMEOUT) != RESET)
  {
    SDIO_ClearITPendingBit(SDIO_IT_DTIMEOUT);
    SDIO_ITConfig(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND |
                  SDIO_IT_TXFIFOHE | SDIO_IT_RXFIFOHF | SDIO_IT_TXUNDERR |
                  SDIO_IT_RXOVERR | SDIO_IT_STBITERR, DISABLE);
    NumberOfBytes = 0;
    TransferError = SD_DATA_TIMEOUT;
    return(SD_DATA_TIMEOUT);
  }

  if (SDIO_GetITStatus(SDIO_IT_RXOVERR) != RESET)
  {
    SDIO_ClearITPendingBit(SDIO_IT_RXOVERR);
    SDIO_ITConfig(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND |
                  SDIO_IT_TXFIFOHE | SDIO_IT_RXFIFOHF | SDIO_IT_TXUNDERR |
                  SDIO_IT_RXOVERR | SDIO_IT_STBITERR, DISABLE);
    NumberOfBytes = 0;
    TransferError = SD_RX_OVERRUN;
    return(SD_RX_OVERRUN);
  }

  if (SDIO_GetITStatus(SDIO_IT_TXUNDERR) != RESET)
  {
    SDIO_ClearITPendingBit(SDIO_IT_TXUNDERR);
    SDIO_ITConfig(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND |
                  SDIO_IT_TXFIFOHE | SDIO_IT_RXFIFOHF | SDIO_IT_TXUNDERR |
                  SDIO_IT_RXOVERR | SDIO_IT_STBITERR, DISABLE);
    NumberOfBytes = 0;
    TransferError = SD_TX_UNDERRUN;
    return(SD_TX_UNDERRUN);
  }

  if (SDIO_GetITStatus(SDIO_IT_STBITERR) != RESET)
  {
    SDIO_ClearITPendingBit(SDIO_IT_STBITERR);
    SDIO_ITConfig(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND |
                  SDIO_IT_TXFIFOHE | SDIO_IT_RXFIFOHF | SDIO_IT_TXUNDERR |
                  SDIO_IT_RXOVERR | SDIO_IT_STBITERR, DISABLE);
    NumberOfBytes = 0;
    TransferError = SD_START_BIT_ERR;
    return(SD_START_BIT_ERR);
  }

  return(SD_OK);
}
*/
/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : CmdError
//->Description    : Checks for error conditions for CMD0.
//->Input          : None
//->Output         : None
//->Return         : SD_Error: SD Card Error code.
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
static SD_Error CmdError(void)
{
  SD_Error errorstatus = SD_OK;
  u32 timeout;

  timeout = SDIO_CMD0TIMEOUT; 		//10000

  while ((timeout > 0) && (SDIO_GetFlagStatus(SDIO_FLAG_CMDSENT) == RESET))
  {
    timeout--;
  }

  if (timeout == 0)
  {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    return(errorstatus);
  }

  //____________Clear all the static flags
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  return(errorstatus);
}
*/
/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : CmdResp7Error
//->Description    : Checks for error conditions for R7.
//->                 response.
//->Input          : None
//->Output         : None
//->Return         : SD_Error: SD Card Error code.
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
static SD_Error CmdResp7Error(void)
{
  SD_Error errorstatus = SD_OK;
  u32 status;
  u32 timeout = SDIO_CMD0TIMEOUT;

  status = SDIO->STA;

  while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)) && (timeout > 0))
  {
    timeout--;
    status = SDIO->STA;
  }

  if ((timeout == 0) || (status & SDIO_FLAG_CTIMEOUT))
  {
    //____________ Card is not V2.0 complient or card does not support the set voltage range
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
    return(errorstatus);
  }

  if (status & SDIO_FLAG_CMDREND)
  {
    //____________Card is SD V2.0 compliant
    errorstatus = SD_OK;
    SDIO_ClearFlag(SDIO_FLAG_CMDREND);
    return(errorstatus);
  }
  return(errorstatus);
}
*/
/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : CmdResp1Error
//->Description    : Checks for error conditions for R1.
//->                 response
//->Input          : cmd: The sent command index.
//->Output         : None
//->Return         : SD_Error: SD Card Error code.
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
static SD_Error CmdResp1Error(u8 cmd)
{
  SD_Error errorstatus = SD_OK;
  u32 status;
  u32 response_r1;

  status = SDIO->STA;

  while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)))
  {
    status = SDIO->STA;
  }

  if (status & SDIO_FLAG_CTIMEOUT)
  {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
    return(errorstatus);
  }
  else if (status & SDIO_FLAG_CCRCFAIL)
  {
    errorstatus = SD_CMD_CRC_FAIL;
    SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
    return(errorstatus);
  }

  //____________Check response received is of desired command
  if (SDIO_GetCommandResponse() != cmd)
  {
    errorstatus = SD_ILLEGAL_CMD;
    return(errorstatus);
  }

  //____________Clear all the static flags
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  //____________We have received response, retrieve it for analysis
  response_r1 = SDIO_GetResponse(SDIO_RESP1);

  if ((response_r1 & SD_OCR_ERRORBITS) == SD_ALLZERO)
  {
    return(errorstatus);
  }

  if (response_r1 & SD_OCR_ADDR_OUT_OF_RANGE)
  {
    return(SD_ADDR_OUT_OF_RANGE);
  }

  if (response_r1 & SD_OCR_ADDR_MISALIGNED)
  {
    return(SD_ADDR_MISALIGNED);
  }

  if (response_r1 & SD_OCR_BLOCK_LEN_ERR)
  {
    return(SD_BLOCK_LEN_ERR);
  }

  if (response_r1 & SD_OCR_ERASE_SEQ_ERR)
  {
    return(SD_ERASE_SEQ_ERR);
  }

  if (response_r1 & SD_OCR_BAD_ERASE_PARAM)
  {
    return(SD_BAD_ERASE_PARAM);
  }

  if (response_r1 & SD_OCR_WRITE_PROT_VIOLATION)
  {
    return(SD_WRITE_PROT_VIOLATION);
  }

  if (response_r1 & SD_OCR_LOCK_UNLOCK_FAILED)
  {
    return(SD_LOCK_UNLOCK_FAILED);
  }

  if (response_r1 & SD_OCR_COM_CRC_FAILED)
  {
    return(SD_COM_CRC_FAILED);
  }

  if (response_r1 & SD_OCR_ILLEGAL_CMD)
  {
    return(SD_ILLEGAL_CMD);
  }

  if (response_r1 & SD_OCR_CARD_ECC_FAILED)
  {
    return(SD_CARD_ECC_FAILED);
  }

  if (response_r1 & SD_OCR_CC_ERROR)
  {
    return(SD_CC_ERROR);
  }

  if (response_r1 & SD_OCR_GENERAL_UNKNOWN_ERROR)
  {
    return(SD_GENERAL_UNKNOWN_ERROR);
  }

  if (response_r1 & SD_OCR_STREAM_READ_UNDERRUN)
  {
    return(SD_STREAM_READ_UNDERRUN);
  }

  if (response_r1 & SD_OCR_STREAM_WRITE_OVERRUN)
  {
    return(SD_STREAM_WRITE_OVERRUN);
  }

  if (response_r1 & SD_OCR_CID_CSD_OVERWRIETE)
  {
    return(SD_CID_CSD_OVERWRITE);
  }

  if (response_r1 & SD_OCR_WP_ERASE_SKIP)
  {
    return(SD_WP_ERASE_SKIP);
  }

  if (response_r1 & SD_OCR_CARD_ECC_DISABLED)
  {
    return(SD_CARD_ECC_DISABLED);
  }

  if (response_r1 & SD_OCR_ERASE_RESET)
  {
    return(SD_ERASE_RESET);
  }

  if (response_r1 & SD_OCR_AKE_SEQ_ERROR)
  {
    return(SD_AKE_SEQ_ERROR);
  }
  return(errorstatus);
}
*/
/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : CmdResp3Error
//->Description    : Checks for error conditions for R3 (OCR).
//->                 response.
//->Input          : None
//->Output         : None
//->Return         : SD_Error: SD Card Error code.
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
static SD_Error CmdResp3Error(void)
{
  SD_Error errorstatus = SD_OK;
  u32 status;

  status = SDIO->STA;

  while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)))
  {
    status = SDIO->STA;
  }

  if (status & SDIO_FLAG_CTIMEOUT)
  {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
    return(errorstatus);
  }
  //____________Clear all the static flags
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);
  return(errorstatus);
}
*/
/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : CmdResp2Error
//->Description    : Checks for error conditions for R2 (CID or CSD).
//->                 response.
//->Input          : None
//->Output         : None
//->Return         : SD_Error: SD Card Error code.
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
static SD_Error CmdResp2Error(void)
{
  SD_Error errorstatus = SD_OK;
  u32 status;

  status = SDIO->STA;

  while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CTIMEOUT | SDIO_FLAG_CMDREND)))
  {
    status = SDIO->STA;
  }

  if (status & SDIO_FLAG_CTIMEOUT)
  {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
    return(errorstatus);
  }
  else if (status & SDIO_FLAG_CCRCFAIL)
  {
    errorstatus = SD_CMD_CRC_FAIL;
    SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
    return(errorstatus);
  }

  //____________Clear all the static flags
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  return(errorstatus);
}
*/
/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : CmdResp6Error
//->Description    : Checks for error conditions for R6 (RCA).
//->                 response.
//->Input          : - cmd: The sent command index.
//->                 - prca: pointer to the variable that will contain the SD
//->                   card relative address RCA. 
//->Output         : None
//->Return         : SD_Error: SD Card Error code.
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
static SD_Error CmdResp6Error(u8 cmd, u16 *prca)
{
  SD_Error errorstatus = SD_OK;
  u32 status;
  u32 response_r1;

  status = SDIO->STA;

  while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CTIMEOUT | SDIO_FLAG_CMDREND)))
  {
    status = SDIO->STA;
  }

  if (status & SDIO_FLAG_CTIMEOUT)
  {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
    return(errorstatus);
  }
  else if (status & SDIO_FLAG_CCRCFAIL)
  {
    errorstatus = SD_CMD_CRC_FAIL;
    SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
    return(errorstatus);
  }

  //____________Check response received is of desired command
  if (SDIO_GetCommandResponse() != cmd)
  {
    errorstatus = SD_ILLEGAL_CMD;
    return(errorstatus);
  }

  //____________Clear all the static flags
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  //____________We have received response, retrieve it. 
  response_r1 = SDIO_GetResponse(SDIO_RESP1);

  if (SD_ALLZERO == (response_r1 & (SD_R6_GENERAL_UNKNOWN_ERROR | SD_R6_ILLEGAL_CMD | SD_R6_COM_CRC_FAILED)))
  {
    *prca = (u16) (response_r1 >> 16);
    return(errorstatus);
  }

  if (response_r1 & SD_R6_GENERAL_UNKNOWN_ERROR)
  {
    return(SD_GENERAL_UNKNOWN_ERROR);
  }

  if (response_r1 & SD_R6_ILLEGAL_CMD)
  {
    return(SD_ILLEGAL_CMD);
  }

  if (response_r1 & SD_R6_COM_CRC_FAILED)
  {
    return(SD_COM_CRC_FAILED);
  }

  return(errorstatus);
}
*/
/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : SDEnWideBus
//->Description    : Enables or disables the SDIO wide bus mode.
//->Input          : NewState: new state of the SDIO wide bus mode.
//->                 This parameter can be: ENABLE or DISABLE.
//->Output         : None
//->Return         : SD_Error: SD Card Error code.
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
static SD_Error SDEnWideBus(FunctionalState NewState)
{
  SD_Error errorstatus = SD_OK;

  u32 scr[2] = {0, 0};

  if (SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED)
  {
    errorstatus = SD_LOCK_UNLOCK_FAILED;
    return(errorstatus);
  }

  //____________Get SCR Register
  errorstatus = FindSCR(RCA, scr);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }

  //____________If wide bus operation to be enabled
  if (NewState == ENABLE)
  {
    //____________If requested card supports wide bus operation
    if ((scr[1] & SD_WIDE_BUS_SUPPORT) != SD_ALLZERO)
    {
      //____________Send CMD55 APP_CMD with argument as card's RCA.
      SDIO_CmdInitStructure.SDIO_Argument = (u32) RCA << 16;
      SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_APP_CMD;
      SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStructure);

      errorstatus = CmdResp1Error(SDIO_APP_CMD);

      if (errorstatus != SD_OK)
      {
        return(errorstatus);
      }

      //____________Send ACMD6 APP_CMD with argument as 2 for wide bus mode
      SDIO_CmdInitStructure.SDIO_Argument = 0x2;
      SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_APP_SD_SET_BUSWIDTH;
      SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStructure);

      errorstatus = CmdResp1Error(SDIO_APP_SD_SET_BUSWIDTH);

      if (errorstatus != SD_OK)
      {
        return(errorstatus);
      }
      return(errorstatus);
    }
    else
    {
      errorstatus = SD_REQUEST_NOT_APPLICABLE;
      return(errorstatus);
    }
  }   //If wide bus operation to be disabled
  else
  {
    //____________If requested card supports 1 bit mode operation
    if ((scr[1] & SD_SINGLE_BUS_SUPPORT) != SD_ALLZERO)
    {
      //____________Send CMD55 APP_CMD with argument as card's RCA.
      SDIO_CmdInitStructure.SDIO_Argument = (u32) RCA << 16;
      SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_APP_CMD;
      SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStructure);


      errorstatus = CmdResp1Error(SDIO_APP_CMD);

      if (errorstatus != SD_OK)
      {
        return(errorstatus);
      }

      //____________Send ACMD6 APP_CMD with argument as 2 for wide bus mode
      SDIO_CmdInitStructure.SDIO_Argument = 0x00;
      SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_APP_SD_SET_BUSWIDTH;
      SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStructure);

      errorstatus = CmdResp1Error(SDIO_APP_SD_SET_BUSWIDTH);

      if (errorstatus != SD_OK)
      {
        return(errorstatus);
      }

      return(errorstatus);
    }
    else
    {
      errorstatus = SD_REQUEST_NOT_APPLICABLE;
      return(errorstatus);
    }
  }
}
*/
/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : IsCardProgramming
//->Description    : Checks if the SD card is in programming state.
//->Input          : pstatus: pointer to the variable that will contain the SD
//->                 card state.
//->Output         : None
//->Return         : SD_Error: SD Card Error code.
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
static SD_Error IsCardProgramming(u8 *pstatus)
{
  SD_Error errorstatus = SD_OK;
  vu32 respR1 = 0, status = 0;

  SDIO_CmdInitStructure.SDIO_Argument = (u32) RCA << 16;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_SEND_STATUS;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  status = SDIO->STA;
  while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)))
  {
    status = SDIO->STA;
  }

  if (status & SDIO_FLAG_CTIMEOUT)
  {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
    return(errorstatus);
  }
  else if (status & SDIO_FLAG_CCRCFAIL)
  {
    errorstatus = SD_CMD_CRC_FAIL;
    SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
    return(errorstatus);
  }

  status = (u32)SDIO_GetCommandResponse();

  //____________Check response received is of desired command
  if (status != SDIO_SEND_STATUS)
  {
    errorstatus = SD_ILLEGAL_CMD;
    return(errorstatus);
  }

  //____________Clear all the static flags
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);


  //____________We have received response, retrieve it for analysis
  respR1 = SDIO_GetResponse(SDIO_RESP1);

  //____________Find out card status
  *pstatus = (u8) ((respR1 >> 9) & 0x0000000F);

  if ((respR1 & SD_OCR_ERRORBITS) == SD_ALLZERO)
  {
    return(errorstatus);
  }

  if (respR1 & SD_OCR_ADDR_OUT_OF_RANGE)
  {
    return(SD_ADDR_OUT_OF_RANGE);
  }

  if (respR1 & SD_OCR_ADDR_MISALIGNED)
  {
    return(SD_ADDR_MISALIGNED);
  }

  if (respR1 & SD_OCR_BLOCK_LEN_ERR)
  {
    return(SD_BLOCK_LEN_ERR);
  }

  if (respR1 & SD_OCR_ERASE_SEQ_ERR)
  {
    return(SD_ERASE_SEQ_ERR);
  }

  if (respR1 & SD_OCR_BAD_ERASE_PARAM)
  {
    return(SD_BAD_ERASE_PARAM);
  }

  if (respR1 & SD_OCR_WRITE_PROT_VIOLATION)
  {
    return(SD_WRITE_PROT_VIOLATION);
  }

  if (respR1 & SD_OCR_LOCK_UNLOCK_FAILED)
  {
    return(SD_LOCK_UNLOCK_FAILED);
  }

  if (respR1 & SD_OCR_COM_CRC_FAILED)
  {
    return(SD_COM_CRC_FAILED);
  }

  if (respR1 & SD_OCR_ILLEGAL_CMD)
  {
    return(SD_ILLEGAL_CMD);
  }

  if (respR1 & SD_OCR_CARD_ECC_FAILED)
  {
    return(SD_CARD_ECC_FAILED);
  }

  if (respR1 & SD_OCR_CC_ERROR)
  {
    return(SD_CC_ERROR);
  }

  if (respR1 & SD_OCR_GENERAL_UNKNOWN_ERROR)
  {
    return(SD_GENERAL_UNKNOWN_ERROR);
  }

  if (respR1 & SD_OCR_STREAM_READ_UNDERRUN)
  {
    return(SD_STREAM_READ_UNDERRUN);
  }

  if (respR1 & SD_OCR_STREAM_WRITE_OVERRUN)
  {
    return(SD_STREAM_WRITE_OVERRUN);
  }

  if (respR1 & SD_OCR_CID_CSD_OVERWRIETE)
  {
    return(SD_CID_CSD_OVERWRITE);
  }

  if (respR1 & SD_OCR_WP_ERASE_SKIP)
  {
    return(SD_WP_ERASE_SKIP);
  }

  if (respR1 & SD_OCR_CARD_ECC_DISABLED)
  {
    return(SD_CARD_ECC_DISABLED);
  }

  if (respR1 & SD_OCR_ERASE_RESET)
  {
    return(SD_ERASE_RESET);
  }

  if (respR1 & SD_OCR_AKE_SEQ_ERROR)
  {
    return(SD_AKE_SEQ_ERROR);
  }

  return(errorstatus);
}
*/
/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : FindSCR
//->Description    : Find the SD card SCR register value.
//->Input          : - rca: selected card address.
//->                 - pscr: pointer to the buffer that will contain the SCR value.
//->Output         : None
//->Return         : SD_Error: SD Card Error code.
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
static SD_Error FindSCR(u16 rca, u32 *pscr)
{
  u32 index = 0;
  SD_Error errorstatus = SD_OK;
  u32 tempscr[2] = {0, 0};

  //____________Set Block Size To 8 Bytes
  //____________Send CMD55 APP_CMD with argument as card's RCA
  SDIO_CmdInitStructure.SDIO_Argument = (u32)8;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_SET_BLOCKLEN;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SDIO_SET_BLOCKLEN);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }

  //____________Send CMD55 APP_CMD with argument as card's RCA
  SDIO_CmdInitStructure.SDIO_Argument = (u32) RCA << 16;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_APP_CMD;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SDIO_APP_CMD);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }
  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DataLength = 8;
  SDIO_DataInitStructure.SDIO_DataBlockSize = SDIO_DataBlockSize_8b;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataConfig(&SDIO_DataInitStructure);


  //____________Send ACMD51 SD_APP_SEND_SCR with argument as 0
  SDIO_CmdInitStructure.SDIO_Argument = 0x0;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_SD_APP_SEND_SCR;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SDIO_SD_APP_SEND_SCR);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }

  while (!(SDIO->STA & (SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND | SDIO_FLAG_STBITERR)))
  {
    if (SDIO_GetFlagStatus(SDIO_FLAG_RXDAVL) != RESET)
    {
      *(tempscr + index) = SDIO_ReadData();
      index++;
    }
  }

  if (SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT);
    errorstatus = SD_DATA_TIMEOUT;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);
    errorstatus = SD_DATA_CRC_FAIL;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_RXOVERR) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_RXOVERR);
    errorstatus = SD_RX_OVERRUN;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_STBITERR);
    errorstatus = SD_START_BIT_ERR;
    return(errorstatus);
  }

  //____________Clear all the static flags
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  *(pscr + 1) = ((tempscr[0] & SD_0TO7BITS) << 24) | ((tempscr[0] & SD_8TO15BITS) << 8) | ((tempscr[0] & SD_16TO23BITS) >> 8) | ((tempscr[0] & SD_24TO31BITS) >> 24);

  *(pscr) = ((tempscr[1] & SD_0TO7BITS) << 24) | ((tempscr[1] & SD_8TO15BITS) << 8) | ((tempscr[1] & SD_16TO23BITS) >> 8) | ((tempscr[1] & SD_24TO31BITS) >> 24);

  return(errorstatus);
}
*/
/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : convert_from_bytes_to_power_of_two
//->Description    : Converts the number of bytes in power of two and returns the
//->                 power.
//->Input          : NumberOfBytes: number of bytes.
//->Output         : None
//->Return         : None
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
static u8 convert_from_bytes_to_power_of_two(u16 NumberOfBytes)
{
  u8 count = 0;

  while (NumberOfBytes != 1)
  {
    NumberOfBytes >>= 1;
    count++;
  }
  return(count);
}
*/
/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : GPIO_Configuration
//->Description    : Configures the SDIO Corresponding GPIO Ports
//->Input          : None
//->Output         : None
//->Return         : None
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
static void GPIO_Configuration(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  //____________GPIOC and GPIOD Periph clock enable
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);

  //____________Configure PC.08, PC.09, PC.10, PC.11, PC.12 pin: D0, D1, D2, D3, CLK pin
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  //____________Configure PD.02 CMD line
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}
*/
/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : DMA_TxConfiguration
//->Description    : Configures the DMA2 Channel4 for SDIO Tx request.
//->Input          : - BufferSRC: pointer to the source buffer
//->                 - BufferSize: buffer size
//->Output         : None
//-> Return         : None
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
static void DMA_TxConfiguration(u32 *BufferSRC, u32 BufferSize)
{
  DMA_InitTypeDef DMA_InitStructure;

  DMA_ClearFlag(DMA2_FLAG_TC4 | DMA2_FLAG_TE4 | DMA2_FLAG_HT4 | DMA2_FLAG_GL4);

  //____________DMA2 Channel4 disable
  DMA_Cmd(DMA2_Channel4, DISABLE);

  //____________DMA2 Channel4 Config
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)SDIO_FIFO_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)BufferSRC;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = BufferSize / 4;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA2_Channel4, &DMA_InitStructure);

  //____________DMA2 Channel4 enable
  DMA_Cmd(DMA2_Channel4, ENABLE);
}
*/
/*
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->Function Name  : DMA_RxConfiguration
//->Description    : Configures the DMA2 Channel4 for SDIO Rx request.
//->Input          : - BufferDST: pointer to the destination buffer
//->                  - BufferSize: buffer size
//->Output         : None
//->Return         : None
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
static void DMA_RxConfiguration(u32 *BufferDST, u32 BufferSize)
{
  DMA_InitTypeDef DMA_InitStructure;

  DMA_ClearFlag(DMA2_FLAG_TC4 | DMA2_FLAG_TE4 | DMA2_FLAG_HT4 | DMA2_FLAG_GL4);

  //____________DMA2 Channel4 disable
  DMA_Cmd(DMA2_Channel4, DISABLE);

  //____________DMA2 Channel4 Config
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)SDIO_FIFO_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)BufferDST;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = BufferSize / 4;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA2_Channel4, &DMA_InitStructure);

  //____________DMA2 Channel4 enable
  DMA_Cmd(DMA2_Channel4, ENABLE);
}
*/
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/









































































/*
unsigned char SD_CMD[100]={0};
//=============================================================================
//->函数名		:DAC_Configuration
//->功能描述	:DAC配置
//->输入			:
//->输出			:无
//->返回值		:无
//->说明			:利用DAC输出指定的电压，软件触发DAC转换。
					 STM32 DAC管脚为DAC1-PA4,DAC2-PA5 12Bit 
//=============================================================================
void SD_Configuration(void)
{

}
//=============================================================================
//->函数名		:DAC_Configuration
//->功能描述	:DAC配置
//->输入			:
//->输出			:无
//->返回值		:无
//->说明			:利用DAC输出指定的电压，软件触发DAC转换。
					 STM32 DAC管脚为DAC1-PA4,DAC2-PA5 12Bit 
//=============================================================================
void User_SPIInit(void)
 {
  GPIO_InitTypeDef GPIO_InitStructure;
//	 GPIO_InitTypeDef GPIO_InitStructure;+
  
  SPI_InitTypeDef SPI_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1,ENABLE);  //使能时钟
 
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
  
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  
  GPIO_Init(GPIOA,&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
  
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
  
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  
  GPIO_Init(GPIOA,&GPIO_InitStructure);
  
  SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;    //双线全双工
 
  SPI_InitStructure.SPI_Mode=SPI_Mode_Master;  //主模式
 
  SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;  //8位数据
 
  SPI_InitStructure.SPI_CPOL=SPI_CPOL_High;   //这里要注意，一定要配置为上升沿数据有效，因为SD卡为上升沿数据有效
 
  SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;  
  
  SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;
  
  SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_256;
  
  SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;
  
  SPI_InitStructure.SPI_CRCPolynomial=7;
  
  SPI_Init(SPI1,&SPI_InitStructure);
  
  SPI_Cmd(SPI1,ENABLE); 
 }

 

//=============================================================================
//->函数名		:DAC_Configuration
//->功能描述	:DAC配置
//->输入			:
//->输出			:无
//->返回值		:无
//->说明			:利用DAC输出指定的电压，软件触发DAC转换。
					 STM32 DAC管脚为DAC1-PA4,DAC2-PA5 12Bit 
//=============================================================================

//SPI初始化以后就可以写SPI读写函数了，以下两个函数参照了网上的资料，出处找不到了，但是这两个函数帮了我大忙，再次感谢提供资料的无名者

 

void SD_WriteByte(unsigned char data)
 {
  u16 temp;
  temp=data;
  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE )==RESET);
    SPI_I2S_SendData(SPI1,temp);
  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE )==RESET);
    SPI_I2S_ReceiveData(SPI1);
 }

//=============================================================================
//->函数名		:DAC_Configuration
//->功能描述	:DAC配置
//->输入			:
//->输出			:无
//->返回值		:无
//->说明			:利用DAC输出指定的电压，软件触发DAC转换。
					 STM32 DAC管脚为DAC1-PA4,DAC2-PA5 12Bit 
//=============================================================================
unsigned char SD_ReadByte(void)
 {
  unsigned char temp;
  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE )==RESET);
    SPI_I2S_SendData(SPI1,0xFF);
  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE )==RESET);
    temp=SPI_I2S_ReceiveData(SPI1);
  return temp;
 }


//有了上面两个函数，问题就好解决了，下面实现发送SD命令函数

//=============================================================================
//->函数名		:DAC_Configuration
//->功能描述	:DAC配置
//->输入			:
//->输出			:无
//->返回值		:无
//->说明			:利用DAC输出指定的电压，软件触发DAC转换。
					 STM32 DAC管脚为DAC1-PA4,DAC2-PA5 12Bit 
//============================================================================= 

unsigned char SD_SendCmd(unsigned char *SD_CMD)  //Cmd为unsigned char Cmd[6]数组，存放SD固定6字节命令
{
  unsigned char i,temp;
  
  temp=0xFF;   //赋予一个初值
 for(i=0;i<6;i++)
     SD_WriteByte(SD_CMD[i]);   //发送6字节命令
 do
  {
    temp=SD_ReadByte();  //一直读SD的应答字节，其实应答字节数量不定，这里简化只收取第一个应答字节，赋temp为0xFF主要因为发现所有应答字节序列的第一个字节不  

                                         //会是0xFF
  }while(temp==0xFF);

  return temp;  
 }

//=============================================================================
//->函数名		:DAC_Configuration
//->功能描述	:DAC配置
//->输入			:
//->输出			:无
//->返回值		:无
//->说明			:利用DAC输出指定的电压，软件触发DAC转换。
					 STM32 DAC管脚为DAC1-PA4,DAC2-PA5 12Bit 
//=============================================================================

//发送命令函数完成后下面就该是SD_Init()函数了

 

unsigned char SD_Init(void)
 {
  unsigned char i,temp;
  for(i=0;i<200;i++)          //SD卡要求复位前至少发送74个clock，这里我发了很多，足够多
   SD_WriteByte(0xFF);

 temp=0x00;
  SD_CMD[0]=0x40;   //发送复位命令CMD0，CMD[1]-CMD[4]初始化为0，这里不用再写了，因为CMD0不需要参数
 SD_CMD[5]=0x95;
  do
  {
   temp=SD_SendCmd(SD_CMD);   
   
  }while(temp!=0x01);     //不断发送CMD0，直到返回0x01，即SD卡的Idle状态（我设置的无论何时SD卡CSS始终为低电平）
 

 temp=0x03;  //发送指令CMD55和指令ACMD41
  SD_CMD[5]=0xFF;
  do
  {
    SD_CMD[0]=0x77;  //CMD55
    temp=SD_SendCmd(SD_CMD);
    SD_CMD[0]=0x69;  //ACMD41
    temp=SD_SendCmd(SD_CMD);
  }while(temp!=0x00);     //循环发送CMD55和ACMD41，直到SD卡返回0x00，即初始化完成且进入到SPI模式，注意在整个所有的过程中，SD卡的CSS时钟为低电平
 
  return temp;  //当然，返回0x00则SD卡初始化成功
}

//=============================================================================
//->函数名		:DAC_Configuration
//->功能描述	:DAC配置
//->输入			:
//->输出			:无
//->返回值		:无
//->说明			:利用DAC输出指定的电压，软件触发DAC转换。
					 STM32 DAC管脚为DAC1-PA4,DAC2-PA5 12Bit 
//=============================================================================

//既然SD卡初始化成功，下面就好说了，下面实现读取一个512字节的块和写入512字节的块

 

void  SD_Read_SigleBlock(unsigned long addr,unsigned char *ptr)   //addr为4字节地址，这里必须为512的整数倍，ptr为大于512字节的接受缓冲区指针，必须为byte
 {
  unsigned char temp;
  unsigned int i=0;
  temp=0xFF;
  SD_CMD[0]=0x51;
  SD_CMD[4]=addr;
  addr=addr>>8;
  SD_CMD[3]=addr;
  addr=addr>>8;
  SD_CMD[2]=addr;
  addr=addr>>8;
  SD_CMD[1]=addr;
  SD_CMD[5]=0xFF;
  do
  {
    temp=SD_SendCmd(SD_CMD);
  }while(temp!=0x00);         //直到返回读取单块命令的正确应答字节，即返回0x00，说明命令发送成功，发送成功后就要读取SD发送的数据了
 do
  {
    temp=SD_ReadByte();
  }while(temp!=0xFE&&temp!=0xFC);    //读取SD卡发送的数据，不断的读取，直到读到SD发送的数据开始信号，即0xFE或0xFC，再往下就是512字节的正式数据
  for(i=0;i<512;i++)
     ptr[i]=SD_ReadByte();       //读取512字节的正式数据
 temp=SD_ReadByte();        //下面还要读取两个字节的CRC校验数据，SD的SPI模式下除了CMD0的CRC有效外，其他CRC校验都无效
 temp=SD_ReadByte();
 }

//=============================================================================
//->函数名		:DAC_Configuration
//->功能描述	:DAC配置
//->输入			:
//->输出			:无
//->返回值		:无
//->说明			:利用DAC输出指定的电压，软件触发DAC转换。
					 STM32 DAC管脚为DAC1-PA4,DAC2-PA5 12Bit 
//=============================================================================

void SD_Write_SigleBlock(unsigned long addr,unsigned char *ptr)  //参数addr为写入数据的地址，必须为512整数倍；ptr为512字节的发送缓冲区指针，必须为byte
 {
   unsigned char temp;
  unsigned int i=0;
  temp=0xFF;
  SD_CMD[0]=0x58;
  SD_CMD[4]=addr;
  addr=addr>>8;
  SD_CMD[3]=addr;
  addr=addr>>8;
  SD_CMD[2]=addr;
  addr=addr>>8;
  SD_CMD[1]=addr;
  SD_CMD[5]=0xFF;
  do
  {
    temp=SD_SendCmd(SD_CMD);
  }while(temp!=0x00);   //循环发送写单块命令，直到返回正确应答信号0x00
  
  
   SD_WriteByte(0xFE);   //给SD卡发送正式数据的开始字节信号0xFE或0xFC，这里我选取0xFE
  for(i=0;i<512;i++)
    SD_WriteByte(ptr[i]);   //给SD卡发送要写的512字节的正式数据
 SD_WriteByte(0xFF);   //发送两字节的CRC校验数据，虽说没有用，但形式上还是要发送的
 SD_WriteByte(0xFF);
 }

//=============================================================================
//->函数名		:DAC_Configuration
//->功能描述	:DAC配置
//->输入			:
//->输出			:无
//->返回值		:无
//->说明			:利用DAC输出指定的电压，软件触发DAC转换。
					 STM32 DAC管脚为DAC1-PA4,DAC2-PA5 12Bit 
//=============================================================================

//以上都完成后，我们就可以操作SD卡了

//unsigned char data[512];

//unsigned int i=0;

//void mian()

//{

//  for(i=0;i<512;i++)

//    data[i]=0xFF;

//  SD_Write_SigleBlock(0x00000000,data);

//  for(i=0;i<512;i++)

//    data[i]=0;

//  SD_Read_SigleBlock(0x00000000,data);

//   //在这里检查data里的内容是否都为0xFF即可，如果为0xFF，说明一切成功，否则，要检查了

//    while(1);

//}

*/


/*******************	wegam@sina.com	*******************/
/*********************	2017/01/15	*********************/
/**********************	END OF FILE	*********************/


