/******************************** User_library *********************************
* 文件名 	: STM32_GT32L32M0180.C
* 作者   	: wegam@sina.com
* 版本   	: V
* 日期   	: 2017/01/15
* 说明   	: 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


#include "GT32L32M0180.H"

#define STM32_GT32L32M0180_Code_Edit

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


u8 BAR_PIC_ADDR[64]={0XFF};
u8 GBCode[64]={0XFF};

u8 UseSPI1_flg=0;		//如果使用纯硬件SPI1（含CS脚），UseSPI1_flg=1，否则UseSPI1_flg=0；
u8 UseSPI2_flg=0;		//如果使用纯硬件SPI2（含CS脚），UseSPI2_flg=1，否则UseSPI2_flg=0；
u8 UseSPI3_flg=0;		//如果使用纯硬件SPI3（含CS脚），UseSPI3_flg=1，否则UseSPI3_flg=0；


void GT32L32_CS_DisSelect(GT32L32_Info_TypeDef *GT32L32_Info);
void GT32L32_CS_Select(GT32L32_Info_TypeDef *GT32L32_Info);
void GT32L32_CLK_L	(GT32L32_Info_TypeDef *GT32L32_Info);
void GT32L32_CLK_H	(GT32L32_Info_TypeDef *GT32L32_Info);
void GT32L32_SI_L		(GT32L32_Info_TypeDef *GT32L32_Info);
void GT32L32_SI_H		(GT32L32_Info_TypeDef *GT32L32_Info);
u8 GT32L32_SO				(GT32L32_Info_TypeDef *GT32L32_Info);


//---------私有函数
u8  GT32L32_GetDig_Info(u8 Sequence,GT32L32_Info_TypeDef *GT32L32_Info);			//点阵数字符号字符
u8  GT32L32_GetASCII_Info(u8 ASCIICode,GT32L32_Info_TypeDef *GT32L32_Info);		//ASCII字符信息
u8	GT32L32_GetGB18030_Info(u16 word,GT32L32_Info_TypeDef *GT32L32_Info);			//GB18030汉字&字符信息
u8 	GT32L32_Get_Info(u16 word,GT32L32_Info_TypeDef *GT32L32_Info);						//获取字符地址和点阵长度

u32 GT32L32_GetAddress(u8 font, u8 c1, u8 c2, u8 c3, u8 c4);//获取地址
u32 GT32L32_GetBufferLen(u8 font, u8 c1, u8 c2, u8 c3, u8 c4);//获取长度
u16  GT32L32_ReadBuffer(GT32L32_Info_TypeDef *GT32L32_Info,u8 font,u16 word,unsigned char *Buffer);		//从字库中读数据函数

u32 GT32L32_GetGB18030_12(u8 c1, u8 c2, u8 c3, u8 c4);		//12x12点阵GB18030汉字&字符地址计算
u32 GT32L32_GetGB18030_16(u8 c1, u8 c2, u8 c3, u8 c4);		//16x16点阵GB18030汉字&字符地址计算
u32 GT32L32_GetGB18030_24(u8 c1, u8 c2, u8 c3, u8 c4);		//24x24点阵GB18030汉字&字符地址计算
u32 GT32L32_GetGB18030_32(u8 c1, u8 c2, u8 c3, u8 c4);		//32x32点阵GB18030汉字&字符地址计算

u32 GT32L32_GetBarCode_13(u8 * BAR_NUM);															//EAN13条形码调用程序
u32 GT32L32_GetBarCode_128(u8	*BAR_NUM,u8	flag);											//GB/T 18347-2001(CODE128)条形码调用程序
u32 GT32L32_GetAntennaCode_12X12(u8	NUM,u8 *GetBuffer);								//天线图标调用程序
u32 GT32L32_GetBatteryCode_12X12(u8	NUM,u8 *GetBuffer);								//电池图标调用程序

u16	GT32L32_U2G_13(u16 Unicode);																			//UNICODE转GBK码表映射算法,仅1&3字符区
u32 GT32L32_UNICODE_To_GBK(u16 unicode,u8 *GetBuffer);								//UNICODE转GBK码表映射算法（不含符号区,转换全部双字节区） 
u32 GT32L32_BIG5_To_GBK(u16 BIG5_Code,u8 *GetBuffer);									//BIG5转GBK转换算法

/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void GT32L32_ConfigurationNR(GT32L32_Info_TypeDef *GT32L32_Info)
{
		//1)**********定义相关结构体
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//2)**********相关GPIO配置
	if(GT32L32_Info->GT32L32_Port.sSPIx==SPI1)
	{
		//PA4-NSS;PA5-SCK;PA6-MISO;PA7-MOSI;
		//2.1)**********打开SPI时钟	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);			//开启SPI时钟	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA, ENABLE);	
		
		if((GT32L32_Info->GT32L32_Port.sGT32L32_CS_PORT==GPIOA)&&(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PIN==GPIO_Pin_4))			//如果SPI_NSS为SPI_NSS_Soft（软件控制方式）
		{
//			SPI_SSOutputCmd(Pinfo->sSPIx, ENABLE);			//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
//			u8 UseSPI1_flg=1;		//如果使用纯硬件SPI1（含CS脚），UseSPI1_flg=1，否则UseSPI1_flg=0；
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		}
		else
		{
//			SPI_SSOutputCmd(Pinfo->sSPIx, DISABLE);			//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
//			u8 UseSPI1_flg=0;		//如果使用纯硬件SPI1（含CS脚），UseSPI1_flg=1，否则UseSPI1_flg=0；
			//开CS-GPIO时钟
			if(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PORT==GPIOA)
			{
				if((GT32L32_Info->GT32L32_Port.sGT32L32_CS_PIN==GPIO_Pin_13)||(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PIN==GPIO_Pin_14)||(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PIN==GPIO_Pin_15))
				{
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
					//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);			//关闭SW功能
					GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		//关闭JTAG,SW功能开启
				}
			}
			else if(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PORT==GPIOB)
			{
				if((GT32L32_Info->GT32L32_Port.sGT32L32_CS_PIN==GPIO_Pin_3)||(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PIN==GPIO_Pin_4))
				{
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
					GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);				//关闭JTAG
				}
				else
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
			}
			else if(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PORT==GPIOC)
			{
				if((GT32L32_Info->GT32L32_Port.sGT32L32_CS_PIN==GPIO_Pin_14)||(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PIN==GPIO_Pin_15))
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_AFIO, ENABLE);
				else
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
			}
			else if(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PORT==GPIOD)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
			}
			else if(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PORT==GPIOE)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
			}
			else if(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PORT==GPIOF)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
			}
			else if(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PORT==GPIOG)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
			}
			//SCK,MISO,MOSI配置
			GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			//CS配置
			GPIO_InitStructure.GPIO_Pin 	= GT32L32_Info->GT32L32_Port.sGT32L32_CS_PIN;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PORT, &GPIO_InitStructure);
		}
	}
	else if(GT32L32_Info->GT32L32_Port.sSPIx==SPI2)
	{
		//PB12-NSS;PB13-SCK;PB14-MISO;PB15-MOSI;
		//2.2)**********打开SPI时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 ,ENABLE);				//开启SPI时钟			
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);
		
		if((GT32L32_Info->GT32L32_Port.sGT32L32_CS_PORT==GPIOB)&&(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PIN==GPIO_Pin_12))			//如果SPI_NSS为SPI_NSS_Soft（软件控制方式）
		{
//			u8 UseSPI2_flg=1;		//如果使用纯硬件SPI2（含CS脚），UseSPI2_flg=1，否则UseSPI2_flg=0；
			SPI_SSOutputCmd(GT32L32_Info->GT32L32_Port.sSPIx, ENABLE);			//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
			GPIO_Init(GPIOB, &GPIO_InitStructure);	
		}
		else
		{
			
//			SPI_SSOutputCmd(Pinfo->sSPIx, DISABLE);			//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
//			u8 UseSPI2_flg=0;		//如果使用纯硬件SPI2（含CS脚），UseSPI2_flg=1，否则UseSPI2_flg=0；
			//开CS-GPIO时钟
			if(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PORT==GPIOA)
			{
				if((GT32L32_Info->GT32L32_Port.sGT32L32_CS_PIN==GPIO_Pin_13)||(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PIN==GPIO_Pin_14)||(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PIN==GPIO_Pin_15))
				{
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
					//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);			//关闭SW功能
					GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		//关闭JTAG,SW功能开启
				}
				else
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			}
			else if(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PORT==GPIOB)
			{
				if((GT32L32_Info->GT32L32_Port.sGT32L32_CS_PIN==GPIO_Pin_3)||(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PIN==GPIO_Pin_4))
				{
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
					GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);				//关闭JTAG
				}
				else
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
			}
			else if(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PORT==GPIOC)
			{
				if((GT32L32_Info->GT32L32_Port.sGT32L32_CS_PIN==GPIO_Pin_14)||(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PIN==GPIO_Pin_15))
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_AFIO, ENABLE);
				else
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
			}
			else if(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PORT==GPIOD)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
			}
			else if(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PORT==GPIOE)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
			}
			else if(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PORT==GPIOF)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
			}
			else if(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PORT==GPIOG)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
			}
			//SCK,MISO,MOSI配置
			GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			//CS配置
			GPIO_InitStructure.GPIO_Pin 	= GT32L32_Info->GT32L32_Port.sGT32L32_CS_PIN;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PORT, &GPIO_InitStructure);
		}
	}
	else if(GT32L32_Info->GT32L32_Port.sSPIx==SPI3)
	{
		//PA15-NSS;PB3-SCK;PB4-MISO;PB5-MOSI;
		//2.2)**********打开SPI时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3 ,ENABLE);			
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
		
		if((GT32L32_Info->GT32L32_Port.sGT32L32_CS_PORT==GPIOA)&&(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PIN==GPIO_Pin_15))			//如果SPI_NSS为SPI_NSS_Soft（软件控制方式）
		{
			UseSPI3_flg=1;		//如果使用纯硬件SPI3（含CS脚），UseSPI3_flg=1，否则UseSPI3_flg=0；
			
			GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;  		//复用推挽输出
			GPIO_Init(GPIOB, &GPIO_InitStructure);
			
			//2.2)**********SPI_NSS配置		
			GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_15;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;  		//复用推挽输出
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		}
		else																										//如果SPI_NSS为SPI_NSS_Soft（软件控制方式）
		{
			UseSPI3_flg=0;		//如果使用纯硬件SPI3（含CS脚），UseSPI3_flg=1，否则UseSPI3_flg=0；
			//开CS-GPIO时钟
			if(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PORT==GPIOA)
			{
				if((GT32L32_Info->GT32L32_Port.sGT32L32_CS_PIN==GPIO_Pin_13)||(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PIN==GPIO_Pin_14)||(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PIN==GPIO_Pin_15))
				{
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
					//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);			//关闭SW功能
					GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		//关闭JTAG,SW功能开启
				}
				else
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			}
			else if(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PORT==GPIOB)
			{
				if((GT32L32_Info->GT32L32_Port.sGT32L32_CS_PIN==GPIO_Pin_3)||(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PIN==GPIO_Pin_4))
				{
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
					GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);				//关闭JTAG
				}
				else
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
			}
			else if(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PORT==GPIOC)
			{
				if((GT32L32_Info->GT32L32_Port.sGT32L32_CS_PIN==GPIO_Pin_14)||(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PIN==GPIO_Pin_15))
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_AFIO, ENABLE);
				else
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
			}
			else if(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PORT==GPIOD)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
			}
			else if(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PORT==GPIOE)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
			}
			else if(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PORT==GPIOF)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
			}
			else if(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PORT==GPIOG)
			{
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
			}
			//SCK,MISO,MOSI配置
			GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			//CS配置
			GPIO_InitStructure.GPIO_Pin 	= GT32L32_Info->GT32L32_Port.sGT32L32_CS_PIN;
			GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PORT, &GPIO_InitStructure);
		}			
	}
	//3)**********SPI配置选项
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;				//设置方向				（2线全双工、2线只接收、一线发送、一线接收）
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;															//模式         	（从或主设备）
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;													//宽度         	（8或16位）
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;																//时钟极性     	（低或高）
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;															//时钟相位     	（第一个或第二个跳变沿）
	if(UseSPI1_flg==1||UseSPI2_flg==1||UseSPI3_flg==1)																			//如果使用纯硬件SPI1（含CS脚），UseSPI1_flg=1，否则UseSPI1_flg=0；
	{
		SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;																//片选方式     	（硬件或软件方式）
	}
	else
	{
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;															//片选方式     	（硬件或软件方式）
	}	
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;				//波特率预分频 	（从2---256分频）
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;												//最先发送的位 	（最低位，还是最高位在先）
	SPI_InitStructure.SPI_CRCPolynomial = 7;																	//设置crc多项式	（数字）如7
	SPI_Init(GT32L32_Info->GT32L32_Port.sSPIx,&SPI_InitStructure);

	SPI_Cmd(GT32L32_Info->GT32L32_Port.sSPIx, ENABLE);				//使能SPI	
	
	//3)**********使能SPIx_NESS为主输出模式
	if((GT32L32_Info->GT32L32_Port.sSPIx->CR1&0X0200)!=SPI_NSS_Soft)						//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
	{
		SPI_SSOutputCmd(GT32L32_Info->GT32L32_Port.sSPIx, ENABLE);			//如果在主机模式下的片选方式为硬件（SPI_NSS_Hard）方式，此处必须打开，否则NSS无信号
	}
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
void	GT32L32_Server(void)
{

}

/*****************************************************************************
点阵数字符号字符
说明：																					  --------------------
此部分内容为0 1 2 3 4 5 6 7 8 9 , . ￥ $ ￡
Squence：表示 字符顺序，从0开始计数。
BaseAdd：说明该套字库在芯片中的起始地址。
Address：对应字符点阵在芯片中的字节地址。
******************************************************************************/
u8  GT32L32_GetDig_Info(u8 Sequence,GT32L32_Info_TypeDef *GT32L32_Info)
{
	//____________定义变量
	u8 len=0;	//数据长度变量
	u32 address=0;				//地址变量
	u32 BaseAddr=GT32L32_Info->GT32L32_Data.GT32L32_BaseAddr;	
	//____________配置数据
	switch(BaseAddr)
	{
	case 0x112400:	address=Sequence * 56+ BaseAddr;			//(u32)0x112400,			//14x28数字符号字符
									len=56;	//GT32L32_ReadBuffer(Sequence * 56+ BaseAdd,56,DZ_Data); 			//14X28
				break ;
	case 0x112748: 	address=Sequence * 120+ BaseAddr;			//(u32)0x112748,			//20x40数字符号字符
									len=120;//GT32L32_ReadBuffer(Sequence * 120+ BaseAdd,120,DZ_Data); 		//20X40
				break ;
	case 0x112CE8: 	address=Sequence * 114+ BaseAddr+2;		//(u32)0x112CE8,			//28点阵不等宽数字符号字符
									len=112;//GT32L32_ReadBuffer(Sequence * 114+ BaseAdd+2,112,DZ_Data); 	//28X28
				break ;
	case 0x113396: 	address=Sequence * 202+ BaseAddr+2;		//(u32)0x113396,			//40点阵不等宽数字符号字符
									len=200;//GT32L32_ReadBuffer(Sequence * 202+ BaseAdd+2,200,DZ_Data); 	//40X40
				break ;  
	default:       break ;
	}
	//____________更新数据
	GT32L32_Info->GT32L32_Data.GT32L32_Address=address;			//字符地址
	GT32L32_Info->GT32L32_Data.GT32L32_BufferSize=len;			//点阵数据长度
	return 0;	  
}

/***************************************
ASCII 调用
ASCIICode：表示ASCII 码（8bits）
BaseAdd：说明该套字库在芯片中的起始地址。
GT32L32_ReadBuffer： 是读点阵数据函数。
GetBuffer：是保存读出的点阵数据的数组。
****************************************/
u8  GT32L32_GetASCII_Info(u8 ASCIICode,GT32L32_Info_TypeDef *GT32L32_Info)
{
	//____________定义变量
	u8 len=0;	//数据长度变量
	u32 address=0;				//地址变量
	u32 BaseAddr=GT32L32_Info->GT32L32_Data.GT32L32_BaseAddr;
	
	//____________配置数据	
	if  ((ASCIICode >= 0x20)&&(ASCIICode<=0x7e))
	{	
	  switch(BaseAddr)
	  {
	  case 0x100000:	address=(ASCIICode-0x20)*8+BaseAddr;
										len=8;//GT32L32_ReadBuffer((ASCIICode-0x20)*8+BaseAdd,8,DZ_Data); //5X7
					break ;
	  case 0x100300:	address=(ASCIICode-0x20)*8+BaseAddr;
										len=8;//GT32L32_ReadBuffer((ASCIICode-0x20)*8+BaseAdd,8,DZ_Data); //7X8
					break ;
	  case 0x100600:	address=(ASCIICode-0x20)*8+BaseAddr;
										len=8;//GT32L32_ReadBuffer((ASCIICode-0x20)*8+BaseAdd,8,DZ_Data); //7X8 Fat
					break ;
	  case 0x100900:	address=(ASCIICode-0x20)*12+BaseAddr;
										len=12;//GT32L32_ReadBuffer((ASCIICode-0x20)*12+BaseAdd,12,DZ_Data); //6X12 
					break ;
	  case 0x100D80:	address=(ASCIICode-0x20)*16+BaseAddr;
										len=16;//GT32L32_ReadBuffer((ASCIICode-0x20)*16+BaseAdd,16,DZ_Data); //8X16
					break ;  
	  case 0x101580:	address=(ASCIICode-0x20)*16+BaseAddr;
										len=16;//GT32L32_ReadBuffer((ASCIICode-0x20)*16+BaseAdd,16,DZ_Data); // 8X16 Fat 
					break ;
	  case 0x101B80:	address=(ASCIICode-0x20)*48+BaseAddr;
										len=48;//GT32L32_ReadBuffer((ASCIICode-0x20)*48+BaseAdd,48,DZ_Data); //12X24
					break ;  
	  case 0x102D80: 	address=(ASCIICode-0x20)*64+BaseAddr;
										len=64;//GT32L32_ReadBuffer((ASCIICode-0x20)*64+BaseAdd,64,DZ_Data); //16X32
					break ;
	  case 0x104580: 	address=(ASCIICode-0x20)*64+BaseAddr;
										len=64;//GT32L32_ReadBuffer((ASCIICode-0x20)*64+BaseAdd,64,DZ_Data); //16X32 Fat
					break ;
	  case 0x105D80: 	address=(ASCIICode-0x20)*26+BaseAddr+2;
										len=24;//GT32L32_ReadBuffer((ASCIICode-0x20)*26+BaseAdd+2,24,DZ_Data); //12X12 A 
					break ;
	  case 0x106740: 	address=(ASCIICode-0x20)*34+BaseAddr+2;
										len=32;//GT32L32_ReadBuffer((ASCIICode-0x20)*34+BaseAdd+2,32,DZ_Data); //16X16 Arial
					break ; 
	  case 0x107400: 	address=(ASCIICode-0x20)*74+BaseAddr+2;
										len=72;//GT32L32_ReadBuffer((ASCIICode-0x20)*74+BaseAdd+2,72,DZ_Data); //24X24 
					break ;
	  case 0x108FC0: 	address=(ASCIICode-0x20)*130+BaseAddr+2;
										len=128;//GT32L32_ReadBuffer((ASCIICode-0x20)*130+BaseAdd+2,128,DZ_Data); //32X32 
					break ;
	  case 0x10C080: 	address=(ASCIICode-0x20)*26+BaseAddr+2;
										len=24;//GT32L32_ReadBuffer((ASCIICode-0x20)*26+BaseAdd+2,24,DZ_Data); //12X12 T 
					break ;
	  case 0x10CA50: 	address=(ASCIICode-0x20)*34+BaseAddr+2;
										len=32;//GT32L32_ReadBuffer((ASCIICode-0x20)*34+BaseAdd+2,32,DZ_Data); //16X16 T
					break ;
	  case 0x10D740: 	address=(ASCIICode-0x20)*74+BaseAddr+2;
										len=72;//GT32L32_ReadBuffer((ASCIICode-0x20)*74+BaseAdd+2,72,DZ_Data); //24X24 T 
					break ;
	  case 0x10F340: 	address=(ASCIICode-0x20)*130+BaseAddr+2;
										len=128;//GT32L32_ReadBuffer((ASCIICode-0x20)*130+BaseAdd+2,128,DZ_Data); //32X32 T 
					break ;  
	  default: 
					break ;
	  }
		//____________更新数据
		GT32L32_Info->GT32L32_Data.GT32L32_Address=address;			//字符地址
		GT32L32_Info->GT32L32_Data.GT32L32_BufferSize=len;			//点阵数据长度	
	 return 1;
	}
	else
	{
		GT32L32_Info->GT32L32_Data.GT32L32_Address=address;			//字符地址
		GT32L32_Info->GT32L32_Data.GT32L32_BufferSize=len;			//点阵数据长度	
		GT32L32_Info->GT32L32_Status=GT32L32_Error_ASCIICode;		//ASCII码错误
		return 0; 
	}
}
/*************************************************************************************************** 
16x16点阵GB18030汉字&字符
函数：u8 GB18030_16_GetData(u8 c1, u8 c2, u8 c3, u8 c4) 
功能：计算汉字点阵在芯片中的地址,读取点阵数据到指定数组。 
参数：c1,c2,c3,c4：
4字节汉字内码通过参数c1,c2,c3,c4传入，双字节内码通过参数c1,c2 传入，c3=0,c4=0 
返回：汉字点阵的字节地址(byte address)。
如果用户是按 word mode 读取点阵数据，则其地址(word address)为字节地址除以2，
即：word address = byte address / 2 . 例如：
BaseAdd: 说明汉字点阵数据在字库芯片中的起始地址，即BaseAdd＝0x114FDE;
 “啊”字的内码为0xb0a1,则byte address = GB18030_16_GetData(0xb0,0xa1,0x00,0x00) *32+BaseAdd; 
word address = byte address / 2 
“.”字的内码为0x8139ee39,则byte address = GB18030_16_GetData(0x81,0x39,0xee,0x39) *32+ BaseAdd
word address = byte address / 2 
****************************************************************************************************/
u8 GT32L32_GetGB18030_Info(u16 word,GT32L32_Info_TypeDef *GT32L32_Info) 
{
	//____________定义变量
	u8	len=0;	//数据长度变量
	u32 address=0;				//地址变量
	u32 BaseAddr=GT32L32_Info->GT32L32_Data.GT32L32_BaseAddr;
	
	u8	c1=word>>8;
	u8	c2=word;
	u8	c3=0;
	u8	c4=0;
	u8	l1=0;
	//____________字体判断
	switch(BaseAddr)
	{
		case	GB18030_BaseAddr_zf12x12	:	l1=0;len=24;
					break;
		case	GB18030_BaseAddr_zf16x16	:	l1=0;len=32;
					break;
		case	GB18030_BaseAddr_zf24x24	:	l1=0;len=72;
					break;
		case	GB18030_BaseAddr_zf32x32	:	l1=192;len=128;
					break;
		default: 
					break ;
	}
	//____________数据检查
	if(c2==0x7F) 	//空
	{
		GT32L32_Info->GT32L32_Status=GT32L32_Error_HzEmp;	//汉字内码低8位为0x7F,为空内容
		return 0;
	}
	//0）____________配置数据		
	if(c1>=0xA1 && c1 <= 0xa9 && c2>=0xa1) 					//Section 1 
		address= (c1 - 0xA1) * 94 + (c2 - 0xA1); 
	else if(c1>=0xa8 && c1 <= 0xa9 && c2<0xa1) 			//Section 5 
	{ 
		if(c2>0x7f)
			c2--; 
		address=(c1-0xa8)*96 + (c2-0x40)+846; 
	} 
	if(c1>=0xb0 && c1 <= 0xf7 && c2>=0xa1) 					//Section 2 
		address= (c1 - 0xB0) * 94 + (c2 - 0xA1)+1038-l1; 
	else if(c1<0xa1 && c1>=0x81 && c2>=0x40 ) 			//Section 3 
	{ 
		if(c2>0x7f) 
			c2--;
		address=(c1-0x81)*190 + (c2-0x40) + 1038 +6768-l1;
	} 
	else if(c1>=0xaa && c2<0xa1) 										//Section 4 
	{ 
		if(c2>0x7f) 
			c2--; 
		address=(c1-0xaa)*96 + (c2-0x40) + 1038 +12848-l1; 
	}
	else if(c1==0x81 && c2>=0x39) 									//四字节区1 
	{ 
		address =1038 + 21008+(c3-0xEE)*10+c4-0x39-l1; 
	} 
	else if(c1==0x82)																//四字节区2 
	{ 
		address =1038 + 21008+161+(c2-0x30)*1260+(c3-0x81)*10+c4-0x30-l1; 
	}
	//____________更新数据
	address=address*len+BaseAddr;
	GT32L32_Info->GT32L32_Data.GT32L32_Address=address;			//字符地址
	GT32L32_Info->GT32L32_Data.GT32L32_BufferSize=len;			//点阵数据长度	
	
	return	1; 
}

/*************************************************************************************************** 
16x16点阵GB18030汉字&字符
函数：u8 GB18030_16_GetData(u8 c1, u8 c2, u8 c3, u8 c4) 
功能：计算汉字点阵在芯片中的地址,读取点阵数据到指定数组。 
参数：c1,c2,c3,c4：
4字节汉字内码通过参数c1,c2,c3,c4传入，双字节内码通过参数c1,c2 传入，c3=0,c4=0 
返回：汉字点阵的字节地址(byte address)。
如果用户是按 word mode 读取点阵数据，则其地址(word address)为字节地址除以2，
即：word address = byte address / 2 . 例如：
BaseAdd: 说明汉字点阵数据在字库芯片中的起始地址，即BaseAdd＝0x114FDE;
 “啊”字的内码为0xb0a1,则byte address = GB18030_16_GetData(0xb0,0xa1,0x00,0x00) *32+BaseAdd; 
word address = byte address / 2 
“.”字的内码为0x8139ee39,则byte address = GB18030_16_GetData(0x81,0x39,0xee,0x39) *32+ BaseAdd
word address = byte address / 2 
****************************************************************************************************/
u8 GT32L32_Get_Info(u16 word,GT32L32_Info_TypeDef *GT32L32_Info) 
{ 
	//____________定义变量
//	u8 len=0;	//数据长度变量
//	u32 address=0;				//地址变量
	u32 GT32L32_BaseAddress=GT32L32_Info->GT32L32_Data.GT32L32_BaseAddr;
	//0）____________基地址判断
	if((GT32L32_BaseAddress<ASCII_BaseAddr_zf5x7)||(GT32L32_BaseAddress>=Reserved_BaseAddr_LSB))
	{
		GT32L32_Info->GT32L32_Status=GT32L32_Error_Addr;
		return 0;
	}
	//1）____________获取数字地址&点阵字符长度信息
	else if(GT32L32_BaseAddress==Digital_BaseAddr_sz14x28||GT32L32_BaseAddress==Digital_BaseAddr_sz20x40||GT32L32_BaseAddress==Digital_BaseAddr_sz28||GT32L32_BaseAddress==Digital_BaseAddr_sz40)
	{
		GT32L32_GetDig_Info(word&0xFF,GT32L32_Info);
		return 1;
	}
	//2）____________获取ASCII字符地址&点阵字符长度信息
	else if(GT32L32_BaseAddress<=ASCII_BaseAddr_TNR32)
	{
		GT32L32_GetASCII_Info(word&0xFF,GT32L32_Info);
		return 1;
	}
	//3）____________获取字符地址&点阵字符长度信息
	else if(GT32L32_BaseAddress==GB18030_BaseAddr_hz12x12||GT32L32_BaseAddress==GB18030_BaseAddr_hz16x16||GT32L32_BaseAddress==GB18030_BaseAddr_hz24x24||GT32L32_BaseAddress==GB18030_BaseAddr_hz32x32)
	{
		GT32L32_GetGB18030_Info(word,GT32L32_Info);
	}
	//4）获取条码地址
	//5）获取天线符号地址
	//6）获取电池符号地址
//	u32 BaseAdd=0x194FDE; 
	
//	return(Address*32+BaseAddr);
		return 0;
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 4字节汉字内码通过参数c1,c2,c3,c4传入，双字节内码通过参数c1,c2 传入，c3=0,c4=0 
*返回值			:	汉字点阵的字节地址(Address)。
*******************************************************************************/
u32 GT32L32_GetAddress(u8 font, u8 c1, u8 c2, u8 c3, u8 c4)
{
	u32 len=0;			//汉字点阵的数据长度
	u32 Address=0;	//汉字点阵的字节地址(Address)
	u32 BaseAddr=0;
	//判断单字节与双字节字符（汉字通常为双字节）
	if(c1>0x80)				//双字节，ASCII码表为（0x00~0x7F)
	{
		//字体大小判断
		if(font==12)				//(u32)0x113D0E,			//12x12点阵GB18030汉字/
		{
			Address=GT32L32_GetGB18030_12(c1,	c2,	c3,	c4);
		}
		else if(font==16)		//(u32)0x194FDE,			//16x16点阵GB18030汉字
		{
			Address=GT32L32_GetGB18030_16(c1,	c2,	c3,	c4);
		}
		else if(font==24)		//(u32)0x2743DE,			//24x24点阵GB18030汉字
		{
			Address=GT32L32_GetGB18030_24(c1,	c2,	c3,	c4);
		}
		else if(font==32)		//(u32)0x47AE10,			//32x32点阵GB18030汉字
		{
			Address=GT32L32_GetGB18030_32(c1,	c2,	c3,	c4);
		}
	}
	else							//单字节，ASCII码表为（0x00~0x7F)
	{
		if  ((c1 >0x20)&&(c1<=0x7e))		//ASCIICode ASCII码表为（0x00~0x7F)	（0x00~0x20为控制符)
		{	
			switch(font)
			{
				case 0x100000:	Address=(c1-0x20)*8+BaseAddr;		//(u32)0x100000,			//5x7点阵ASCII标准字符
												len=8;//GT32L32_ReadBuffer((ASCIICode-0x20)*8+BaseAdd,8,DZ_Data); //5X7
							break ;
				case 0x100300:	Address=(c1-0x20)*8+BaseAddr;		//(u32)0x100300,			//7x8点阵ASCII标准字符
												len=8;//GT32L32_ReadBuffer((ASCIICode-0x20)*8+BaseAdd,8,DZ_Data); //7X8
							break ;
				case 0x100600:	Address=(c1-0x20)*8+BaseAddr;		//(u32)0x100600,			//7x8点阵ASCII粗体字符
												len=8;//GT32L32_ReadBuffer((ASCIICode-0x20)*8+BaseAdd,8,DZ_Data); //7X8 Fat
							break ;
				case 0x100900:	Address=(c1-0x20)*12+BaseAddr;		//(u32)0x100900,			//6x12点阵ASCII字符
												len=12;//GT32L32_ReadBuffer((ASCIICode-0x20)*12+BaseAdd,12,DZ_Data); //6X12 
							break ;
				case 0x100D80:	Address=(c1-0x20)*16+BaseAddr;		//(u32)0x100D80,			//8x16点阵ASCII标准字符
												len=16;//GT32L32_ReadBuffer((ASCIICode-0x20)*16+BaseAdd,16,DZ_Data); //8X16
							break ;  
				case 0x101580:	Address=(c1-0x20)*16+BaseAddr;		//(u32)0x101580,			//8x16点阵ASCII粗体字符
												len=16;//GT32L32_ReadBuffer((ASCIICode-0x20)*16+BaseAdd,16,DZ_Data); // 8X16 Fat 
							break ;
				case 0x101B80:	Address=(c1-0x20)*48+BaseAddr;		//(u32)0x101B80,			//12x24点阵ASCII标准字符
												len=48;//GT32L32_ReadBuffer((ASCIICode-0x20)*48+BaseAdd,48,DZ_Data); //12X24
							break ;  
				case 35: 				Address=(c1-0x20)*64+0x102D80;		//(u32)0x102D80,			//16x32点阵ASCII标准字符
												len=64;//GT32L32_ReadBuffer((ASCIICode-0x20)*64+BaseAdd,64,DZ_Data); //16X32
							break ;
				case 0x104580: 	Address=(c1-0x20)*64+BaseAddr;		//(u32)0x104580,			//16x32点阵ASCII粗体字符
												len=64;//GT32L32_ReadBuffer((ASCIICode-0x20)*64+BaseAdd,64,DZ_Data); //16X32 Fat
							break ;
				case 12: 	Address=(c1-0x20)*26+0x105D80+2;		//(u32)0x105D80,			//12点阵不等宽ASCII方头（Arial）字符
												len=24;//GT32L32_ReadBuffer((ASCIICode-0x20)*26+BaseAdd+2,24,DZ_Data); //12X12 A 
							break ;
				case 0x106740: 	Address=(c1-0x20)*34+BaseAddr+2;		//(u32)0x106740,			//16点阵不等宽ASCII方头（Arial）字符
												len=32;//GT32L32_ReadBuffer((ASCIICode-0x20)*34+BaseAdd+2,32,DZ_Data); //16X16 Arial
							break ; 
				case 0x107400: 	Address=(c1-0x20)*74+BaseAddr+2;		//(u32)0x107400,			//24点阵不等宽ASCII方头（Arial）字符
												len=72;//GT32L32_ReadBuffer((ASCIICode-0x20)*74+BaseAdd+2,72,DZ_Data); //24X24 
							break ;
				case 0x108FC0: 	Address=(c1-0x20)*130+BaseAddr+2;		//(u32)0x108FC0,			//32点阵不等宽ASCII方头（Arial）字符
												len=128;//GT32L32_ReadBuffer((ASCIICode-0x20)*130+BaseAdd+2,128,DZ_Data); //32X32 
							break ;
				case 121: 	Address=(c1-0x20)*26+0x10C080+2;		//(u32)0x10C080,			//12点阵不等宽ASCII白正（Times New Roman）字符
												len=24;//GT32L32_ReadBuffer((ASCIICode-0x20)*26+BaseAdd+2,24,DZ_Data); //12X12 T 
							break ;
				case 16: 	Address=(c1-0x20)*34+0x10CA50+2;		//(u32)0x10CA50,			//16点阵不等宽ASCII白正（Times New Roman）字符
												len=32;//GT32L32_ReadBuffer((ASCIICode-0x20)*34+BaseAdd+2,32,DZ_Data); //16X16 T
							break ;
				case 24: 	Address=(c1-0x20)*74+0x10D740+2;		//(u32)0x10D740,			//24点阵不等宽ASCII白正（Times New Roman）字符
												len=72;//GT32L32_ReadBuffer((ASCIICode-0x20)*74+BaseAdd+2,72,DZ_Data); //24X24 T 
							break ;
				case 32: 	Address=(c1-0x20)*130+0x10F340+2;		//(u32)0x10F340,			//32点阵不等宽ASCII白正（Times New Roman）字符
												len=128;//GT32L32_ReadBuffer((ASCIICode-0x20)*130+BaseAdd+2,128,DZ_Data); //32X32 T 
							break ;  
				default: 
							break ;
			}
		}	
	}	
	return(Address);
}

/*************************************************************************************************** 
16x16点阵GB18030汉字&字符
函数：u8 GB18030_16_GetData(u8 c1, u8 c2, u8 c3, u8 c4) 
功能：计算汉字点阵在芯片中的地址,读取点阵数据到指定数组。 
参数：c1,c2,c3,c4：
4字节汉字内码通过参数c1,c2,c3,c4传入，双字节内码通过参数c1,c2 传入，c3=0,c4=0 
返回：汉字点阵的字节地址(byte address)。
如果用户是按 word mode 读取点阵数据，则其地址(word address)为字节地址除以2，
即：word address = byte address / 2 . 例如：
BaseAdd: 说明汉字点阵数据在字库芯片中的起始地址，即BaseAdd＝0x114FDE;
 “啊”字的内码为0xb0a1,则byte address = GB18030_16_GetData(0xb0,0xa1,0x00,0x00) *32+BaseAdd; 
word address = byte address / 2 
“.”字的内码为0x8139ee39,则byte address = GB18030_16_GetData(0x81,0x39,0xee,0x39) *32+ BaseAdd
word address = byte address / 2 
****************************************************************************************************/
u32 GT32L32_GetBufferLen(u8 font, u8 c1, u8 c2, u8 c3, u8 c4) 
{	
	u32 lengh=0;
	//判断单字节与双字节字符（汉字通常为双字节）
	if(c1>0x80)				//双字节，ASCII码表为（0x00~0x7F)
	{	
		//字体大小判断
		if(font==12)
		{
			lengh=24;		//2(列）x12（行）
		}
		else if(font==16)
		{
			lengh=32;		//2(列）x16（行）
		}
		else if(font==24)
		{
			lengh=72;		//3(列）x24（行）
		}
		else if(font==32)
		{
			lengh=128;	//4(列）x32（行）
		}
	}
	else
	{
		//字体大小判断
		if(font==12)
		{
			lengh=24;		//2(列）x12（行）
		}
		else if(font==16)
		{
			lengh=32;		//2(列）x16（行）
		}
		else if(font==24)
		{
			lengh=72;		//3(列）x24（行）
		}
		else if(font==32)
		{
			lengh=128;	//4(列）x32（行）
		}
	}
	return lengh;	 
}
/*************************************************************************************************** 
12x12点阵GB18030汉字&字符
函数：u32 GB18030_12_GetData(u8 c1, u8 c2, u8 c3, u8 c4) 
功能：计算汉字点阵在芯片中的地址，读取点阵数据到指定数组。 
参数：汉字内码通过参数c1,c2传入，c3=0;c4=0. 
注：12x12为GBK字符集，无四字节区。 
返回：汉字点阵的字节地址(byte address)。
如果用户是按 word mode 读取点阵数据，则其地址(word address)为字节地址除以2，
即：word address = byte address / 2 . 例如：
BaseAdd: 说明汉字点阵数据在字库芯片中的起始地址，即12x12点阵字库起始地址：BaseAdd＝0x113D0E
“啊”字的内码为0xb0a1,则byte address = GB18030_12_GetData(0xb0,0xa1,0x00,0x00) *24+BaseAdd ,
word address = byte address / 2	.
****************************************************************************************************/
u32 GT32L32_GetGB18030_12(u8 c1, u8 c2, u8 c3, u8 c4) 
{
	u32 Address=0;						//Address：对应字符点阵在芯片中的字节地址。
	u32 BaseAdd=0x113D0E; 		//12x12点阵字库起始地址：BaseAdd＝0x113D0E
	if(c2==0x7f) 
	{
		return (BaseAdd);
	} 
	if(c1>=0xA1 && c1 <= 0xa9 && c2>=0xa1) 			//Section 1 
		Address= (c1 - 0xA1) * 94 + (c2 - 0xA1); 
	else if(c1>=0xa8 && c1 <= 0xa9 && c2<0xa1) 	//Section 5 
	{ 
		if(c2>0x7f)
			c2--; 
		Address=(c1-0xa8)*96 + (c2-0x40)+846-192; 
	} 
	if(c1>=0xb0 && c1 <= 0xf7 && c2>=0xa1) //Section 2 
			Address= (c1 - 0xB0) * 94 + (c2 - 0xA1)+1038; 
	else if(c1<0xa1 && c1>=0x81 && c2>=0x40 ) //Section 3 
	{ 
		if(c2>0x7f) 
			c2--;
		Address=(c1-0x81)*190 + (c2-0x40) + 1038 +6768-192;
	} 
	else if(c1>=0xaa && c2<0xa1) //Section 4 
	{ 
		if(c2>0x7f) 
			c2--; 
		Address=(c1-0xaa)*96 + (c2-0x40) + 1038 +12848-192; 
	} 
	return(Address*24+BaseAdd); 
}
/*************************************************************************************************** 
16x16点阵GB18030汉字&字符
函数：u8 GB18030_16_GetData(u8 c1, u8 c2, u8 c3, u8 c4) 
功能：计算汉字点阵在芯片中的地址,读取点阵数据到指定数组。 
参数：c1,c2,c3,c4：
4字节汉字内码通过参数c1,c2,c3,c4传入，双字节内码通过参数c1,c2 传入，c3=0,c4=0 
返回：汉字点阵的字节地址(byte address)。
如果用户是按 word mode 读取点阵数据，则其地址(word address)为字节地址除以2，
即：word address = byte address / 2 . 例如：
BaseAdd: 说明汉字点阵数据在字库芯片中的起始地址，即BaseAdd＝0x114FDE;
 “啊”字的内码为0xb0a1,则byte address = GB18030_16_GetData(0xb0,0xa1,0x00,0x00) *32+BaseAdd; 
word address = byte address / 2 
“.”字的内码为0x8139ee39,则byte address = GB18030_16_GetData(0x81,0x39,0xee,0x39) *32+ BaseAdd
word address = byte address / 2 
****************************************************************************************************/
u32 GT32L32_GetGB18030_16(u8 c1, u8 c2, u8 c3, u8 c4) 
{ 
	u32 Address=0;						//Address：对应字符点阵在芯片中的字节地址。
	u32 BaseAdd=0x194FDE; 		//16x16点阵字库起始地址：BaseAdd＝0x194FDE，
	if(c2==0x7f) 
	{
		return (BaseAdd);
	} 
	if(c1>=0xA1 && c1 <= 0xa9 && c2>=0xa1) //Section 1 
		Address= (c1 - 0xA1) * 94 + (c2 - 0xA1); 
	else if(c1>=0xa8 && c1 <= 0xa9 && c2<0xa1) //Section 5 
	{ 
		if(c2>0x7f)
			c2--; 
		Address=(c1-0xa8)*96 + (c2-0x40)+846; 
	} 
	if(c1>=0xb0 && c1 <= 0xf7 && c2>=0xa1) //Section 2 
		Address= (c1 - 0xB0) * 94 + (c2 - 0xA1)+1038; 
	else if(c1<0xa1 && c1>=0x81 && c2>=0x40 ) //Section 3 
	{ 
		if(c2>0x7f) 
			c2--;
		Address=(c1-0x81)*190 + (c2-0x40) + 1038 +6768;
	} 
	else if(c1>=0xaa && c2<0xa1) //Section 4 
	{ 
		if(c2>0x7f) 
			c2--; 
		Address=(c1-0xaa)*96 + (c2-0x40) + 1038 +12848-192; 
	}
	else if(c1==0x81 && c2>=0x39) //四字节区1 
	{ 
		Address =1038 + 21008+(c3-0xEE)*10+c4-0x39; 
	} 
	else if(c1==0x82)//四字节区2 
	{ 
		Address =1038 + 21008+161+(c2-0x30)*1260+(c3-0x81)*10+c4-0x30; 
	} 
	return(Address*32+BaseAdd); 
}

/*************************************************************************************************** 
24x24点阵GB18030汉字&字符
函数：u8 GB18030_24_GetData(u8 c1, u8 c2, u8 c3, u8 c4) 
功能：计算汉字点阵在芯片中的地址,读取点阵数据到指定数组。 
参数：c1,c2,c3,c4：
4字节汉字内码通过参数c1,c2,c3,c4传入，双字节内码通过参数c1,c2 传入，c3=0,c4=0 
返回：汉字点阵的字节地址(byte address)。
如果用户是按 word mode 读取点阵数据，则其地址(word address)为字节地址除以2，
即：word address = byte address / 2 . 例如：
BaseAdd: 说明汉字点阵数据在字库芯片中的起始地址，即BaseAdd＝0x1F43DE;
 “啊”字的内码为0xb0a1,则byte address = GB18030_24_GetData(0xb0,0xa1,0x00,0x00) *72+BaseAdd; 
word address = byte address / 2 
“.”字的内码为0x8139ee39,则byte address = GB18030_24_GetData(0x81,0x39,0xee,0x39) *72+ BaseAdd
word address = byte address / 2 
****************************************************************************************************/
u32 GT32L32_GetGB18030_24(u8 c1, u8 c2, u8 c3, u8 c4) 
{ 
	u32 Address=0;						//Address：对应字符点阵在芯片中的字节地址。
	u32 BaseAdd=0x2743DE; 		//24x24点阵字库起始地址：BaseAdd＝0x2743DE，
	if(c2==0x7f) 
	{
		return (BaseAdd);
	} 
	if(c1>=0xA1 && c1 <= 0xa9 && c2>=0xa1) 			//Section 1
		Address= (c1 - 0xA1) * 94 + (c2 - 0xA1); 
	else if(c1>=0xa8 && c1 <= 0xa9 && c2<0xa1) 	//Section 5
	{ 
		if(c2>0x7f)
			c2--; 
		Address=(c1-0xa8)*96 + (c2-0x40)+846; 
	} 
	if(c1>=0xb0 && c1 <= 0xf7 && c2>=0xa1) 			//Section 2
		Address= (c1 - 0xB0) * 94 + (c2 - 0xA1)+1038; 
	else if(c1<0xa1 && c1>=0x81 && c2>=0x40 ) 	//Section 3
	{ 
		if(c2>0x7f) 
			c2--;
		Address=(c1-0x81)*190 + (c2-0x40) + 1038 +6768;
	} 
	else if(c1>=0xaa && c2<0xa1) 			//Section 4
	{ 
		if(c2>0x7f) 
			c2--; 
		Address=(c1-0xaa)*96 + (c2-0x40) + 1038 +12848; 
	}
	else if(c1==0x81 && c2>=0x39) 		//四字节区1 
	{ 
		Address =1038 + 21008+(c3-0xEE)*10+c4-0x39; 
	} 
	else if(c1==0x82)									//四字节区2 
	{ 
		Address =1038 + 21008+161+(c2-0x30)*1260+(c3-0x81)*10+c4-0x30; 
	} 
	return(Address*72+BaseAdd);
}

/*************************************************************************************************** 
32x32点阵GB18030汉字&字符
函数：u32 GB18030_32_GetData (u8 c1, u8 c2, u8 c3, u8 c4)  
功能：计算汉字点阵在芯片中的地址 
参数：c1,c2,c3,c4：4字节汉字内码通过参数c1,c2,c3,c4传入，双字节内码通过参数c1,c2传 入，c3=0,c4=0 
返回：汉字点阵的字节地址(byte address)。如果用户是按 word mode 读取点阵数据，
则其地 址(word address)为字节地址除以2，即：word address = byte address / 2 . 
例如：BaseAdd: 说明汉字点阵数据在字库芯片中的起始地址，即BaseAdd＝0x47AE10; “啊”字的内码为0xb0a1,
则byte address = gt(0xb0,0xa1,0x00,0x00) *128+BaseAdd word address = byte address / 2 
“.”字的内码为0x8139ee39,则byte address = gt(0x81, 0x39,0xee,0x39) *128+ BaseAdd word address = byte address / 2 
****************************************************************************************************/
u32 GT32L32_GetGB18030_32(u8 c1, u8 c2, u8 c3, u8 c4) 
{ 
	u32 Address=0;						//Address：对应字符点阵在芯片中的字节地址。
	u32  BaseAdd=0x47AE10;		//32x32点阵字库起始地址：BaseAdd＝0x47AE10，
	if(c2==0x7f) 
	{
		return (BaseAdd);
	} 
	if(c1>=0xA1 && c1 <= 0xa9 && c2>=0xa1) 			//Section 1
		Address= (c1 - 0xA1) * 94 + (c2 - 0xA1); 
	else if(c1>=0xa8 && c1 <= 0xa9 && c2<0xa1) 	//Section 5
	{ 
		if(c2>0x7f)
			c2--; 
		Address=(c1-0xa8)*96 + (c2-0x40)+846-192; 
	} 
	if(c1>=0xb0 && c1 <= 0xf7 && c2>=0xa1) 			//Section 2
		Address= (c1 - 0xB0) * 94 + (c2 - 0xA1)	+	1038-192; 
//		Address= (c1 - 0xB0) * 94 + (c2 - 0xA1);
	else if(c1<0xa1 && c1>=0x81 && c2>=0x40 ) 	//Section 3
	{ 
		if(c2>0x7f) 
			c2--;
		Address=(c1-0x81)*190 + (c2-0x40) + 1038 +	6768;
	} 
	else if(c1>=0xaa && c2<0xa1) 			//Section 4
	{ 
		if(c2>0x7f) 
			c2--; 
		Address=(c1-0xaa)*96 + (c2-0x40) + 1038 +	12848; 
	}
	else if(c1==0x81 && c2>=0x39) 		//四字节区1 
	{ 
		Address =1038 + 21008+(c3-0xEE)*10+c4-0x39; 
	} 
	else if(c1==0x82)									//四字节区2 
	{ 
		Address =1038 + 21008+161+(c2-0x30)*1260+(c3-0x81)*10+c4-0x30; 
	} 
//	return (BaseAdd);
	return(Address*128+BaseAdd);  
} 

/**********************************************************************
EAN13条形码调用程序
函数：DWORD* BAR_CODE(int* BAR_NUM)
功能：将数组条形码转为对应条形码图形地址。
参数：int* BAR_NUM 条形码数字数组指针，BAR_NUM[13]数组包含13个数字。
返回：定义DWORD BAR_PIC_ADDR[13];用于存放对应地址，返回此数组指针。
***********************************************************************/

u32 GT32L32_GetBarCode_13(u8 * BAR_NUM)
{
	u32 i,BaseAddr=0x478FD2;
	BAR_PIC_ADDR[0]=BAR_NUM[0]*54+540*0+ BaseAddr;
	BAR_PIC_ADDR[1]=BAR_NUM[1]*54+540*1+ BaseAddr;
	switch(BAR_NUM[0])
	{
		case 0:
			for(i=2;i<=6;i++)
			{
				BAR_PIC_ADDR[i]=BAR_NUM[i]*54+540*1+ BaseAddr;
			}
		break;
		case 1:
			BAR_PIC_ADDR[2]=BAR_NUM[2]*54+540*1+ BaseAddr;
			BAR_PIC_ADDR[3]=BAR_NUM[3]*54+540*2+ BaseAddr;
			BAR_PIC_ADDR[4]=BAR_NUM[4]*54+540*1+ BaseAddr;
			BAR_PIC_ADDR[5]=BAR_NUM[5]*54+540*2+ BaseAddr;
			BAR_PIC_ADDR[6]=BAR_NUM[6]*54+540*2+ BaseAddr;
		break;
		case 2:
			BAR_PIC_ADDR[2]=BAR_NUM[2]*54+540*1+ BaseAddr;
			BAR_PIC_ADDR[3]=BAR_NUM[3]*54+540*2+ BaseAddr;
			BAR_PIC_ADDR[4]=BAR_NUM[4]*54+540*2+ BaseAddr;
			BAR_PIC_ADDR[5]=BAR_NUM[5]*54+540*1+ BaseAddr;
			BAR_PIC_ADDR[6]=BAR_NUM[6]*54+540*2+ BaseAddr;
		break;
		case 3:
			BAR_PIC_ADDR[2]=BAR_NUM[2]*54+540*1+ BaseAddr;
			BAR_PIC_ADDR[3]=BAR_NUM[3]*54+540*2+ BaseAddr;
			BAR_PIC_ADDR[4]=BAR_NUM[4]*54+540*2+ BaseAddr;
			BAR_PIC_ADDR[5]=BAR_NUM[5]*54+540*2+ BaseAddr;
			BAR_PIC_ADDR[6]=BAR_NUM[6]*54+540*1+ BaseAddr;
		break;
		case 4:
			BAR_PIC_ADDR[2]=BAR_NUM[2]*54+540*2+ BaseAddr;
			BAR_PIC_ADDR[3]=BAR_NUM[3]*54+540*1+ BaseAddr;
			BAR_PIC_ADDR[4]=BAR_NUM[4]*54+540*1+ BaseAddr;
			BAR_PIC_ADDR[5]=BAR_NUM[5]*54+540*2+ BaseAddr;
			BAR_PIC_ADDR[6]=BAR_NUM[6]*54+540*2+ BaseAddr;
		break;
		case 5:
			BAR_PIC_ADDR[2]=BAR_NUM[2]*54+540*2+ BaseAddr;
			BAR_PIC_ADDR[3]=BAR_NUM[3]*54+540*2+ BaseAddr;
			BAR_PIC_ADDR[4]=BAR_NUM[4]*54+540*1+ BaseAddr;
			BAR_PIC_ADDR[5]=BAR_NUM[5]*54+540*1+ BaseAddr;
			BAR_PIC_ADDR[6]=BAR_NUM[6]*54+540*2+ BaseAddr;
		break;
		case 6:
			BAR_PIC_ADDR[2]=BAR_NUM[2]*54+540*2+ BaseAddr;
			BAR_PIC_ADDR[3]=BAR_NUM[3]*54+540*2+ BaseAddr;
			BAR_PIC_ADDR[4]=BAR_NUM[4]*54+540*2+ BaseAddr;
			BAR_PIC_ADDR[5]=BAR_NUM[5]*54+540*1+ BaseAddr;
			BAR_PIC_ADDR[6]=BAR_NUM[6]*54+540*1+ BaseAddr;
		break;
		case 7:
			BAR_PIC_ADDR[2]=BAR_NUM[2]*54+540*2+ BaseAddr;
			BAR_PIC_ADDR[3]=BAR_NUM[3]*54+540*1+ BaseAddr;
			BAR_PIC_ADDR[4]=BAR_NUM[4]*54+540*2+ BaseAddr;
			BAR_PIC_ADDR[5]=BAR_NUM[5]*54+540*1+ BaseAddr;
			BAR_PIC_ADDR[6]=BAR_NUM[6]*54+540*2+ BaseAddr;
		break;
		case 8:
			BAR_PIC_ADDR[2]=BAR_NUM[2]*54+540*2+ BaseAddr;
			BAR_PIC_ADDR[3]=BAR_NUM[3]*54+540*1+ BaseAddr;
			BAR_PIC_ADDR[4]=BAR_NUM[4]*54+540*2+ BaseAddr;
			BAR_PIC_ADDR[5]=BAR_NUM[5]*54+540*2+ BaseAddr;
			BAR_PIC_ADDR[6]=BAR_NUM[6]*54+540*1+ BaseAddr;
		break;
		case 9:
			BAR_PIC_ADDR[2]=BAR_NUM[2]*54+540*2+ BaseAddr;
			BAR_PIC_ADDR[3]=BAR_NUM[3]*54+540*2+ BaseAddr;
			BAR_PIC_ADDR[4]=BAR_NUM[4]*54+540*1+ BaseAddr;
			BAR_PIC_ADDR[5]=BAR_NUM[5]*54+540*2+ BaseAddr;
			BAR_PIC_ADDR[6]=BAR_NUM[6]*54+540*1+ BaseAddr;
		break;
	}
	BAR_PIC_ADDR[7]=BAR_NUM[7]*54+540*3+ BaseAddr;
	for(i=8;i<=11;i++)
	{
		BAR_PIC_ADDR[i]=BAR_NUM[i]*54+540*4+ BaseAddr;
	}
	BAR_PIC_ADDR[12]=BAR_NUM[12]*54+540*5+ BaseAddr;
	
	memcpy(BAR_NUM,BAR_PIC_ADDR,140);	
	return BAR_PIC_ADDR[0];
}

/***********************************************************************************************
GB/T 18347-2001(CODE128)条形码调用程序
函数：u32 BAR_CODE128(u8 *BAR_NUM)
功能：将数组条形码转为对应条形码图形地址
参数：u8 *BAR_NUM 条形码数字数组指针,BAR_NUM[4]数组包含4个条形码ASCII符(数组取值为0~94)。
返回：定义u32 BAR_PIC_ADDR[7] ；用于存放对应地址，返回数组指针。
设基地址：BaseAddr=0x3F2ED3;
起始符有3种模式
当flag=1时为Code-128-A;
当flag=2时为Code-128-B;
当flag=3时为Code-128-C;
***********************************************************************************************/
u32 GT32L32_GetBarCode_128(u8	*BAR_NUM,u8	flag)
{
  u32 BaseAddr=0x479C7A;
  u8 i;
  for(i=0;i<7;i++) 
	{
     switch(flag)
		{  
		case 1 :
			 if(i==0)
			 {
				 BAR_PIC_ADDR[i]=103*40+BaseAddr;
			 }
			 else if(i==1||i==2||i==3||i==4)           
			 {
				 BAR_PIC_ADDR[i]=BAR_NUM[i-1]*40+BaseAddr;
			 }
			 else if(i==5)
			 {
				 BAR_PIC_ADDR[i]=95*40+BaseAddr;
			 }
			 else if(i==6)
			 {
				 BAR_PIC_ADDR[i]=106*40+BaseAddr;
			 }
			 break;
     
		case 2 :
			if(i==0)
			{
				BAR_PIC_ADDR[i]=104*40+BaseAddr;
			}
			else if(i==1||i==2||i==3||i==4)           
			{
				BAR_PIC_ADDR[i]=BAR_NUM[i-1]*40+BaseAddr;
			}
			else if(i==5)
			{
				BAR_PIC_ADDR[i]=95*40+BaseAddr;
			}
			else if(i==6)
			{
				BAR_PIC_ADDR[i]=106*40+BaseAddr;
			}
			break;
   	       
		case 3 :
			if(i==0)
			{
				BAR_PIC_ADDR[i]=105*40+BaseAddr;
			}
			else if(i==1||i==2||i==3||i==4)           
			{
				BAR_PIC_ADDR[i]=BAR_NUM[i-1]*40+BaseAddr;
			}
			else if(i==5)
			{
				BAR_PIC_ADDR[i]=95*40+BaseAddr;
			}
			else if(i==6)
			{
				BAR_PIC_ADDR[i]=106*40+BaseAddr;
			}
			break;
   
     default:
     break;
		}
	}
	return BAR_PIC_ADDR[0];
}

/***********************************
天线调用程序
函数：u32 Antenna_CODE_12X12_GetData(u8 NUM)
功能：获取12X12天线调用地址,取出点阵数据。
参数：NUM 0123带表天线信号强度。
返回：数据地址
***********************************/
u32 GT32L32_GetAntennaCode_12X12(u8	NUM,u8 *GetBuffer)
{
	u32 Address,BaseAdd=0x47AD32;
	Address=NUM*24+BaseAdd;
	return Address;
}

/**********************************
电池调用程序
函数：u32 Battery_CODE_12X12_GetData(u8 NUM)
功能：获取12X12电池调用地址,取出点阵数据。
参数：NUM 0123带表电池电量。
返回：数据地址
**********************************/
u32 GT32L32_GetBatteryCode_12X12(u8	NUM,u8 *GetBuffer)
{
	u32 Address, BaseAdd=0x47ADAA;
	Address=BaseAdd+NUM*24;
	return Address;
}
/**********************************************************
UNICODE转GBK码表映射算法,仅1&3字符区 
函数：WORD U2G(WORD Unicode) 
功能：将UNICODE码转换为GB码,
注：仅应用于1区&3区。 
参数：WORD Unicode,表示输入的UNICODE码。 
返回：对应的GB码在字库中存放的地址。
（注:读取对应地址2字节数据即为unicode对应的GB码）。
***********************************************************/ 

u16 GT32L32_U2G_13(u16 Unicode) 
{
	u16 GB_Code; 
	u32 i; 
	u16 UG[]={	0x3000,0x3001,0x3002,0x00b7,0x02c9,0x02c7,0x00a8,
							0x3003,0x3005,0x2014, 0xff5e,0x2016,0x2026,0x2018,0x2019,0x201c,
							0x201d,0x3014,0x3015,0x3008, 0x3009,0x300a,0x300b,0x300c,0x300d,
							0x300e,0x300f,0x3016,0x3017,0x3010, 0x3011,0x00b1,0x00d7,0x00f7,
							0x2236,0x2227,0x2228,0x2211,0x220f,0x222a, 0x2229,0x2208,0x2237,
							0x221a,0x22a5,0x2225,0x2220,0x2312,0x2299,0x222b, 0x222e,0x2261,
							0x224c,0x2248,0x223d,0x221d,0x2260,0x226e,0x226f,0x2264, 0x2265,
							0x221e,0x2235,0x2234,0x2642,0x2640,0x00b0,0x2032,0x2033,0x2103,
							0xff04,0x00a4,0xffe0,0xffe1,0x2030,0x00a7,0x2116,0x2606,0x2605,
							0x25cb, 0x25cf,0x25ce,0x25c7,0x25c6,0x25a1,0x25a0,0x25b3,0x25b2,
							0x203b,0x2192, 0x2190,0x2191,0x2193,0x3013
						}; 

	if(Unicode<=0xffe5&&Unicode>=0xff01)//section 3 
	{ 
		if(Unicode==0xff04) 
			GB_Code=0xa1e7;
		if(Unicode==0xff5e) 
			GB_Code=0xa1ab; 
		if(Unicode==0xffe0) 
			GB_Code=0xa1e9; 
		if(Unicode==0xffe1) 
			GB_Code=0xa1ea; 
		if(Unicode==0xffe3) 
			GB_Code=0xa3fe; 
		if(Unicode==0xffe5) 
			GB_Code=0xa3a4; 
		else
			GB_Code=Unicode-0xff01+0xa3a1; 
	} 
	if(Unicode<=0x3013) 
	{ 
		for(i=0;i<94;i++) 
		{ 
			if(Unicode==UG[i]) 
			GB_Code=0xa1a1+i; 
		} 
	}
	GBCode[0]=GB_Code>>8;
	GBCode[1]=GB_Code&0xff;
	return GB_Code;
}

/***************************************************************************
UNICODE转GBK码表映射算法（不含符号区,转换全部双字节区）
GB_Address转换之后的GB码的存放地址， 
函数：u32 U2G(u16 Unicode) 
功能：将UNICODE码转换为GB码, 
参数：u16 Unicode,表示输入的UNICODE码。 
返回：对应的GB码在字库中存放的地址。 
（注:读取对应地址2字节数据即为unicode对应的GB码）。
转码表起始地址： BaseAdd＝0x3EA90E;
***************************************************************************/
u32 GT32L32_UNICODE_To_GBK(u16 unicode,u8 *GetBuffer)
{
	u32 BaseAdd=0x46A90E;
	u32 Address,U_Start_Addr;
	if(unicode<=0x0451&&unicode>=0x00a0) 
	{
		U_Start_Addr	=	0;
		Address= U_Start_Addr +(unicode-0x00a0)*2; 
	} 
	else if(unicode<=0x2642&&unicode>=0x2010) 
	{ 
		U_Start_Addr =1892; 
		Address= U_Start_Addr +(unicode-0x2010)*2; 
	} 
	else if(unicode<=0x33d5&&unicode>=0x3000) 
	{ 
		U_Start_Addr =5066; 
		Address= U_Start_Addr +(unicode-0x3000)*2; 
	} 
	else if(unicode<=0x9fa5&&unicode>=0x4e00) 
	{ 
		U_Start_Addr =7030; 
		Address= U_Start_Addr +(unicode-0x4e00)*2; 
	} 
	else if(unicode<=0xfe6b&&unicode>=0xfe30) 
	{ 
		U_Start_Addr =48834; 
		Address= U_Start_Addr +(unicode-0xfe30)*2; 
	} 
	else if(unicode<=0xff5e&&unicode>=0xff01) 
	{ 
		U_Start_Addr =48954; 
		Address= U_Start_Addr +(unicode-0xff01)*2; 
	} 
	else if(unicode<=0xffe5&&unicode>=0xffe0) 
	{ 
		U_Start_Addr =49142; 
		Address= U_Start_Addr +(unicode-0xffe0)*2; 
	} 
	else if (unicode<=0xFA29&&unicode>=0xF92C) 
	{ 
		U_Start_Addr =49312; 
		Address= U_Start_Addr +(unicode-0xF92C)*2; 
	} 
	else if(unicode<=0xE864&&unicode>=0xE816) 
	{ 
		U_Start_Addr =49820; 
		Address= U_Start_Addr +(unicode-0xE816)*2; 
	}
	else if(unicode<=0x2ECA&&unicode>=0x2E81) 
	{ 
		U_Start_Addr =49978; 
		Address= U_Start_Addr +(unicode-0x2E81)*2; 
	} 
	else if(unicode<=0x49B7&&unicode>=0x4947) 
	{ 
		U_Start_Addr =50126; 
		Address= U_Start_Addr +(unicode-0x4947)*2; 
	} 
	else if(unicode<=0x4DAE&&unicode>=0x4C77) 
	{ 
		U_Start_Addr =50352; 
		Address= U_Start_Addr +(unicode-0x4C77)*2; 
	} 
	else if(unicode<=0x3CE0&&unicode>=0x3447) 
	{ 
		U_Start_Addr =50976; 
		Address= U_Start_Addr +(unicode-0x3447)*2; 
	} 
	else if(unicode<=0x478D&&unicode>=0x4056) 
	{ 
		U_Start_Addr =55380; 
		Address= U_Start_Addr +(unicode-0x4056)*2; 
	} 
	Address+=BaseAdd;
	return Address; 
}



/***********************************************************
BIG5转GBK转换算法
函数：u32 GT32L32_BIG5_To_GBK(u16 BIG5_Code) 
功能：计算BIG5码对应GB码存放的起始地址 
参数：BIG5_Code 输入BIG5编码双字节 
返回：对应GB码存放地址，对应GB码高位在前，低位在后。
************************************************************/
u32 GT32L32_BIG5_To_GBK(u16 BIG5_Code,u8 *GetBuffer) 
{
	u32 Address,part_addr,BaseAddr=0x7F1E10;//转码表基地址; 
	u8 TMP,BIG5_MSB,BIG5_LSB; 
	BIG5_MSB= BIG5_Code >>8; 
	BIG5_LSB= BIG5_Code &0x00ff; 

	if(BIG5_MSB>=0xa1&&BIG5_MSB<=0xa3)
	{ 
		part_addr=0; 
		TMP=0xa1; 
	} 
	else if(BIG5_MSB>=0xa4&&BIG5_MSB<=0xc6) 
	{ 
		part_addr=816; 
		TMP=0xa4; 
	} 
	else if(BIG5_MSB>=0xc9&&BIG5_MSB<=0xf9) 
	{ 
		part_addr=11618; TMP=0xc9; 
	}
	if(BIG5_LSB<=0x7e&&BIG5_LSB>=0x40) 
	{ 
		Address=part_addr+((BIG5_MSB-TMP)*157+BIG5_LSB-0x40)*2+BaseAddr; 
	}
	else if(BIG5_LSB<=0xfe&&BIG5_LSB>=0xa1) 
	{ 
		Address=part_addr+((BIG5_MSB-TMP)*157+BIG5_LSB-0xa1+63)*2+BaseAddr; 
	}
	return Address; 
}





//	//____________SPI_CS控制定义
//		#define GT32L32_CS_DisSelect				GPIO_SetBits(GT32L32_CS_PORT,				GT32L32_CS_PIN)			//取消片选
//		#define GT32L32_CS_Select						GPIO_ResetBits(GT32L32_CS_PORT,			GT32L32_CS_PIN)		//使能片选
//	//____________SPI_CLK/SCLK控制定义
//		#define GT32L32_CLK_L								GPIO_ResetBits(GT32L32_CLK_PORT,	GT32L32_CLK_PIN)		//低电平
//		#define GT32L32_CLK_H								GPIO_SetBits(GT32L32_CLK_PORT,		GT32L32_CLK_PIN)			//高电平
//	//____________SPI_MISO/SO控制定义
//		#define GT32L32_SO									GPIO_ReadOutputDataBit(GT32L32_MISO_PORT,GT32L32_MISO_PIN)
//	//____________SPI_MOSI/SI控制定义
//		#define GT32L32_SI_L								GPIO_ResetBits(GT32L32_MOSI_PORT,	GT32L32_MOSI_PIN)		//低电平
//		#define GT32L32_SI_H								GPIO_SetBits(GT32L32_MOSI_PORT,		GT32L32_MOSI_PIN)			//高电平

//------------------------------------------SPI协议类函数
/*******************************************************************************
*函数名			:	function
*功能描述		:	取消片选
*输入				: 
*返回值			:	无
*******************************************************************************/
void GT32L32_CS_DisSelect(GT32L32_Info_TypeDef *GT32L32_Info)
{
	GPIO_SetBits(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PORT,	GT32L32_Info->GT32L32_Port.sGT32L32_CS_PIN);			//取消片选
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	使能片选
*输入				: 
*返回值			:	无
*******************************************************************************/
void GT32L32_CS_Select(GT32L32_Info_TypeDef *GT32L32_Info)
{
	GPIO_ResetBits(GT32L32_Info->GT32L32_Port.sGT32L32_CS_PORT,	GT32L32_Info->GT32L32_Port.sGT32L32_CS_PIN);			//使能片选
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void GT32L32_CLK_L(GT32L32_Info_TypeDef *GT32L32_Info)
{
	GPIO_ResetBits(GT32L32_Info->GT32L32_Port.sGT32L32_CLK_PORT,	GT32L32_Info->GT32L32_Port.sGT32L32_CLK_PIN);			//取消片选
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	使能片选
*输入				: 
*返回值			:	无
*******************************************************************************/
void GT32L32_CLK_H(GT32L32_Info_TypeDef *GT32L32_Info)
{
	GPIO_SetBits(GT32L32_Info->GT32L32_Port.sGT32L32_CLK_PORT,	GT32L32_Info->GT32L32_Port.sGT32L32_CLK_PIN);			//使能片选
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void GT32L32_SI_L(GT32L32_Info_TypeDef *GT32L32_Info)
{
	GPIO_ResetBits(GT32L32_Info->GT32L32_Port.sGT32L32_MOSI_PORT,	GT32L32_Info->GT32L32_Port.sGT32L32_MOSI_PIN);			//取消片选
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	使能片选
*输入				: 
*返回值			:	无
*******************************************************************************/
void GT32L32_SI_H(GT32L32_Info_TypeDef *GT32L32_Info)
{
	GPIO_SetBits(GT32L32_Info->GT32L32_Port.sGT32L32_MOSI_PORT,	GT32L32_Info->GT32L32_Port.sGT32L32_MOSI_PIN);			//使能片选
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	使能片选
*输入				: 
*返回值			:	无
*******************************************************************************/
u8 GT32L32_SO(GT32L32_Info_TypeDef *GT32L32_Info)
{
	return GPIO_ReadOutputDataBit(GT32L32_Info->GT32L32_Port.sGT32L32_MOSI_PORT,GT32L32_Info->GT32L32_Port.sGT32L32_MOSI_PIN);			//读取MISO数据
}
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->函数名			:	GT32L32_ReadStatus
//->功能描述		:	读取芯片状态
//->输入			: Address  	：	表示字符点阵在芯片中的字节地址。
//->						len				：	是读点阵数据字节数。
//->						*Buffer		：	是保存读出的点阵数据的数组。
//->输出			:	读出的字节
//->返回值			:	无
//->例程			:
//->调用函数		:
//->被调用函数	:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
u8 GT32L32_ReadStatus(GT32L32_Info_TypeDef *GT32L32_Info)
{
#ifdef	GT32L32_SPIPORT_EN
	//____________定义变量
	u8	ChipStatus=0;
	u8	Address=0x05;	//0X60 OR 0XC7
	//____________使能片选
	GPIO_ResetBits(GPIOC,GPIO_Pin_6);
	GT32L32_CS_Select(GT32L32_Info);
	//____________发送地址数据	
	GT32L32_ReadWriteByte(Address);						//发送地址
	//____________接收数据	
	
	ChipStatus=GT32L32_ReadWriteByte(0XFF);// 从字库读出点阵数据到数组中。

	//____________取消片选	
	GPIO_SetBits(GPIOC,GPIO_Pin_6);
	GT32L32_CS_DisSelect(GT32L32_Info);

#else
	//____________定义变量
	u8	ChipStatus=0;
	u8	Address=0x05;	//0X60 OR 0XC7
	//____________使能片选
	GT32L32_CS_Select(GT32L32_Info);
	//____________发送地址数据	
	GT32L32_SendByte(Address);
	//____________接收数据	
	ChipStatus=GT32L32_ReadByte();	// 从字库读出点阵数据到数组中。
	//____________取消片选
	GT32L32_CS_DisSelect(GT32L32_Info);
#endif
	return ChipStatus;
}
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->函数名			:	r_dat_bat
//->功能描述		:	从字库中读数据函数
//->输入			: Address  	：	表示字符点阵在芯片中的字节地址。
//->						len				：	是读点阵数据字节数。
//->						*Buffer		：	是保存读出的点阵数据的数组。
//->输出			:	读出的字节
//->返回值			:	无
//->例程			:
//->调用函数		:
//->被调用函数	:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
void GT32L32_ChipErase(GT32L32_Info_TypeDef *GT32L32_Info)
{
#ifdef	GT32L32_SPIPORT_EN
	//____________定义变量
	u32 i=0;
	u8	len=255;
	u8	Address=0x60;	//0X60 OR 0XC7
	//____________使能片选
	GPIO_ResetBits(GPIOC,GPIO_Pin_6);
	GT32L32_CS_Select(GT32L32_Info);

	GT32L32_ReadWriteByte(Address);						//发送地址

	for(i=0;i<len;i++)
	{
	 GT32L32_ReadWriteByte(0XFF);// 从字库读出点阵数据到数组中。
	}
	//____________取消片选	
	GPIO_SetBits(GPIOC,GPIO_Pin_6);
	GT32L32_CS_DisSelect(GT32L32_Info);

#else
	//____________定义变量
	u32	j=0;
	u8	len=255;
	u8	Address=0x60;	//0X60 OR 0XC7
	//____________使能片选
	GT32L32_CS_Select;
	//____________发送地址数据	
	GT32L32_SendByte(Address);
	//____________接收数据	
	for(j=0;j<len;j++)
	{
	 GT32L32_ReadByte();	// 从字库读出点阵数据到数组中。
	}
	//____________取消片选
	GT32L32_CS_DisSelect;

#endif
}
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->函数名			:	r_dat_bat
//->功能描述		:	从字库中读数据函数
//->输入			: Address  	：	表示字符点阵在芯片中的字节地址。
//->						len				：	是读点阵数据字节数。
//->						*Buffer		：	是保存读出的点阵数据的数组。
//->输出			:	读出的字节
//->返回值			:	无
//->例程			:
//->调用函数		:
//->被调用函数	:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
u8 GT32L32_ReadBufferbac(GT32L32_Info_TypeDef *GT32L32_Info,u32 Address,u16 len,u8 *Buffer)
{
	//____________定义变量
	u32 i=0;
//	u8	Status=0XFF;
	//____________使能片选
	GT32L32_CS_Select(GT32L32_Info);
	Address=Address|0x03000000;		//0x03指令字+地址。
//	Address=Address|0x0B000000;//0x0B指令字+地址。--快速
	GT32L32_ReadWriteByte(Address>>24);				//发送地址高8位
	GT32L32_ReadWriteByte(Address>>16);				//发送地址
	GT32L32_ReadWriteByte(Address>>8);				//发送地址
	GT32L32_ReadWriteByte(Address);						//发送地址
	if((Address&0x0B000000)==0x0B000000)
		GT32L32_ReadWriteByte(0XFF);						//从字库读出点阵数据到数组中。
//	while((Status=GT32L32_ReadStatus()&0x01)!=0x01);
	i=0xFFFF;
	while(i--);
	for(i=0;i<len;i++)
	{
		Buffer[i]=GT32L32_ReadWriteByte(0XFF);	//从字库读出点阵数据到数组中。
	}
	//____________取消片选	
	GPIO_SetBits(GPIOC,GPIO_Pin_6);
	GT32L32_CS_DisSelect(GT32L32_Info);
	return Buffer[0];	
}
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->函数名			:	r_dat_bat
//->功能描述		:	从字库中读数据函数
//->输入			: Address  	：	表示字符点阵在芯片中的字节地址。
//->						len				：	是读点阵数据字节数。
//->						*Buffer		：	是保存读出的点阵数据的数组。
//->输出			:	读出的字节
//->返回值			:	无
//->例程			:
//->调用函数		:
//->被调用函数	:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
u16 GT32L32_ReadBuffer(GT32L32_Info_TypeDef *GT32L32_Info,u8 font,u16 word,unsigned char *Buffer)
{
	//____________定义变量
	u32 i=0;
	u32 Address=0;
	u32 lengh=0;
	
	//区分单双字节
	if(word>>8>=0x80)
	{
		//获取ASCII点阵地址
		Address	=	GT32L32_GetAddress(font, (u8)(word>>8), (u8)word, 0, 0);		//获取地址
		//获取ASCII点阵数据长度
		lengh= GT32L32_GetBufferLen(font, (u8)(word>>8), (u8)word, 0, 0);			//获取长度
	}
	else
	{
		//获取ASCII点阵地址
		Address	=	GT32L32_GetAddress(font, (u8)word, 0, 0, 0);		//获取地址
		//获取ASCII点阵数据长度
		lengh= GT32L32_GetBufferLen(font, (u8)word, 0, 0, 0);			//获取长度
	}	
	
	
	//____________使能片选
	GT32L32_CS_Select(GT32L32_Info);
	Address=Address|0x03000000;		//0x03指令字+地址。
//	Address=Address|0x0B000000;//0x0B指令字+地址。--快速
	GT32L32_ReadWriteByte(Address>>24);				//发送地址高8位
	GT32L32_ReadWriteByte(Address>>16);				//发送地址
	GT32L32_ReadWriteByte(Address>>8);				//发送地址
	GT32L32_ReadWriteByte(Address);						//发送地址
	if((Address&0x0B000000)==0x0B000000)
		GT32L32_ReadWriteByte(0XFF);						//从字库读出点阵数据到数组中。
//	while((Status=GT32L32_ReadStatus()&0x01)!=0x01);
	i=0xFFFF;
	while(i--);
	for(i=0;i<lengh;i++)
	{
		Buffer[i]=GT32L32_ReadWriteByte(0XFF);	//从字库读出点阵数据到数组中。
	}
	//____________取消片选	
	GT32L32_CS_DisSelect(GT32L32_Info);
	
	return lengh;	
}

//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->函数名			:	GT32_ReadWriteByte
//->功能描述		:	读写一个字节
//->输入			: 写入一个字节
//->输出			:	读出的字节
//->返回值			:	无
//->例程			:
//->调用函数		:
//->被调用函数	:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
u8	GT32L32_ReadWriteByte(u8 Data)
{
#ifdef	GT32L32_SPIPORT_EN		//使用SPI接口
	//____________定义变量
	u16 retry=0XFFFF;													//用来进行超时计数
	//____________等待发送缓冲区为空
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);	
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); 		//检查指令SPI发送标志是否为空
	{
		retry++;
		if(retry>2000)
			return 0;
	}	
	//____________发送数据
	SPI_I2S_SendData(SPI1, Data);				//发送数据
	//____________等待接收数据
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	retry=0;	
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)		//检查指令SPI接收完成标志设置与否
	{
		retry++;
		if(retry>8000)
			return 0;
	}
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	//____________返回接收到的数据
	return SPI_I2S_ReceiveData(SPI1); 			//返回接收到的数据	
#else					//使用自定义协议
	u8	dat=0;
	dat=GT32L32_ReadByte();	
	return dat;
#endif
}

//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->函数名			:	GT32L32_SendByte
//->功能描述		:	发送一个字节数据,先发送高位
//->输入			: 
//->输出			:	
//->返回值			:	
//->例程			:
//->调用函数		:
//->被调用函数	:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
void GT32L32_SendByte(GT32L32_Info_TypeDef *GT32L32_Info,u8 ByteData)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		GT32L32_CLK_L(GT32L32_Info);
		if(ByteData&0x80)
			GT32L32_SI_H(GT32L32_Info);
		else 
			GT32L32_SI_L(GT32L32_Info);
		GT32L32_CLK_H(GT32L32_Info);
		ByteData=ByteData<<1;
	}					
}
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->函数名			:	ReadByte
//->功能描述		:	读一个字节,先读高位
//->输入			:
//->输出			:
//->返回值			:
//->例程			:
//->调用函数		:
//->被调用函数	:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
u8 GT32L32_ReadByte(GT32L32_Info_TypeDef *GT32L32_Info)
{
	u8 i;
	u8 dat=0;
	GT32L32_CLK_H(GT32L32_Info);
	for(i=0;i<8;i++)
	{
		GT32L32_CLK_L(GT32L32_Info);
		dat=dat<<1;
		if(GT32L32_SO(GT32L32_Info))
			dat=dat|0x01;
		else 
			dat&=0xfe;
		GT32L32_CLK_H(GT32L32_Info);		
	}	
	return dat;
}




/*******************	wegam@sina.com	*******************/
/*********************	2017/01/21	*********************/
/**********************	END OF FILE	*********************/






































































































////=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
////->函数名			:	SDCard_Configuration
////->功能描述		:	SD卡项目配置
////->输入			: 无
////->输出			:	Errorstatus	配置状态
////->返回值			:	无
////->例程			:
////->调用函数		:
////->被调用函数	:
////<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
//void	GT32L32_Configuration(void)
//{
//	//____________函数调用
//	GT32L32_GPIO_Configuration();										//SPI管脚配置
//	GT32L32_SPI_Configuration(GT32L32_CLK_SpeedH);	//SPI配置--低速
//#ifdef	GT32L32_DMA
//	GT32L32_DMA_Configuration((u32*)SD_TX_Addr,(u32*)SD_RX_Addr,SDCard_BufferSize);
//#endif

//}
////=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
////->函数名			:	SDCard_Server
////->功能描述		:	SD卡项目执行(服务)程序
////->输入			: 
////->输出			:	Errorstatus	状态
////->返回值			:	无
////->例程			:
////->调用函数		:
////->被调用函数	:
////<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
//void	GT32L32_Server(void)
//{

//}
////=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
////->函数名			:	SDCard_GPIO_Configuration
////->功能描述		:	SPI管脚配置
////->输入			: 
////->输出			:	无
////->返回值			:	无
////->例程			:
////->调用函数		:
////->被调用函数	:
////<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
//void GT32L32_GPIO_Configuration(void)
//{
//	//1)____________定义
//	GPIO_InitTypeDef		GPIO_InitStructure;
//	//2)____________开启时钟
//		
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,	ENABLE);		//开启AFIO
//	RCC_APB2PeriphClockCmd(GT32L32_CS_Periph,		ENABLE);		//开启CS时钟
//	RCC_APB2PeriphClockCmd(GT32L32_CLK_Periph,	ENABLE);		//开启CLK时钟
//	RCC_APB2PeriphClockCmd(GT32L32_MISO_Periph,	ENABLE);		//开启MISO时钟
//	RCC_APB2PeriphClockCmd(GT32L32_MOSI_Periph,	ENABLE);		//开启MOSI时钟	
//	//3)____________管脚配置
//	//3.1）CS配置
//	GPIO_InitStructure.GPIO_Pin			=	GT32L32_CS_PIN;
//	GPIO_InitStructure.GPIO_Mode		=	GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed		= GPIO_Speed_50MHz;
//	GPIO_Init(GT32L32_CS_PORT, &GPIO_InitStructure);
//	//3.2）CLK配置
//	GPIO_InitStructure.GPIO_Pin			=	GT32L32_CLK_PIN;
//	GPIO_InitStructure.GPIO_Mode		=	GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed		= GPIO_Speed_50MHz;
//	GPIO_Init(GT32L32_CLK_PORT, &GPIO_InitStructure);
//	//3.3）MISO配置
//	GPIO_InitStructure.GPIO_Pin			=	GT32L32_MISO_PIN;
//	GPIO_InitStructure.GPIO_Mode		=	GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed		= GPIO_Speed_50MHz;
//	GPIO_Init(GT32L32_MISO_PORT, &GPIO_InitStructure);
//	//3.4）MOSI配置
//	GPIO_InitStructure.GPIO_Pin			=	GT32L32_MOSI_PIN;
//	GPIO_InitStructure.GPIO_Mode		=	GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed		= GPIO_Speed_50MHz;
//	GPIO_Init(GT32L32_MOSI_PORT, &GPIO_InitStructure);
//	
//}
////=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
////->函数名			:	SDCard_SPI_Configuration
////->功能描述		:	SPI配置
////->输入			: Speed SPI速度选择（低速/高速)
////->输出			:	无
////->返回值			:	无
////->例程			:
////->调用函数		:
////->被调用函数	:
////<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
//void GT32L32_SPI_Configuration(char Speed)
//{
//	//1)____________定义
//	SPI_InitTypeDef			SPI_InitStructure;
//	RCC_APB2PeriphClockCmd(GT32L32_SPI_Periph,		ENABLE);		//开启SPI时钟
//	//2)____________SPI初始化
//	if(Speed==GT32L32_CLK_SpeedH)		//高速端口
//	{
//		SPI_InitStructure.SPI_Direction					=	SPI_Direction_2Lines_FullDuplex;	//设置方向				（2线全双工、2线只接收、一线发送、一线接收）
//		SPI_InitStructure.SPI_Mode							=	SPI_Mode_Master;									//模式         	（从或主设备）
//		SPI_InitStructure.SPI_DataSize					=	SPI_DataSize_8b;									//宽度         	（8或16位）
//		SPI_InitStructure.SPI_CPOL							=	SPI_CPOL_High;										//时钟极性     	（低或高）
//		SPI_InitStructure.SPI_CPHA							=	SPI_CPHA_2Edge;										//时钟相位     	（第一个或第二个跳变沿）
//		SPI_InitStructure.SPI_NSS								=	SPI_NSS_Soft;											//片选方式     	（硬件或软件方式）
//	//	SPI_InitStructure.SPI_NSS=SPI_NSS_Hard;																		//片选方式     	（硬件或软件方式）
//		SPI_InitStructure.SPI_BaudRatePrescaler	=	SPI_BaudRatePrescaler_128;					//波特率预分频 	（从2---256分频）
//		SPI_InitStructure.SPI_FirstBit					=	SPI_FirstBit_MSB;									//最先发送的位 	（最低位，还是最高位在先）
//		SPI_InitStructure.SPI_CRCPolynomial			=	0X07;															//设置crc多项式	（其复位值为0x0007，根据应用可以设置其他数值。）
//		SPI_Init(GT32L32_SPI_PORT,&SPI_InitStructure);													//SPI1初始化
//	}
//	else											//低速端口
//	{
//		SPI_InitStructure.SPI_Direction					=	SPI_Direction_2Lines_FullDuplex;	//设置方向				（2线全双工、2线只接收、一线发送、一线接收）
//		SPI_InitStructure.SPI_Mode							=	SPI_Mode_Master;									//模式         	（从或主设备）
//		SPI_InitStructure.SPI_DataSize					=	SPI_DataSize_8b;									//宽度         	（8或16位）
//		SPI_InitStructure.SPI_CPOL							=	SPI_CPOL_High;										//时钟极性     	（低或高）
//		SPI_InitStructure.SPI_CPHA							=	SPI_CPHA_2Edge;										//时钟相位     	（第一个或第二个跳变沿）
//		SPI_InitStructure.SPI_NSS								=	SPI_NSS_Soft;											//片选方式     	（硬件或软件方式）
//	//	SPI_InitStructure.SPI_NSS=SPI_NSS_Hard;																		//片选方式     	（硬件或软件方式）
//		SPI_InitStructure.SPI_BaudRatePrescaler	=	SPI_BaudRatePrescaler_256;				//波特率预分频 	（从2---256分频）
//		SPI_InitStructure.SPI_FirstBit					=	SPI_FirstBit_MSB;									//最先发送的位 	（最低位，还是最高位在先）
//		SPI_InitStructure.SPI_CRCPolynomial			=	0X07;															//设置crc多项式	（其复位值为0x0007，根据应用可以设置其他数值。）
//		SPI_Init(GT32L32_SPI_PORT,&SPI_InitStructure);													//SPI1初始化
//	}
//	GT32L32_CS_DisSelect;			//取消片选
//	SPI_Cmd(GT32L32_SPI_PORT, ENABLE);				//使能SPI
//}
////=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
////->函数名			:	SDCard_DMA_Configuration
////->功能描述		:	DMA配置
////->输入			: SD_TX_Addr	SPI发送地址
////							SD_RX_Addr	SPI接收地址
////							BufferSize	DMA缓存大小
////->输出			:	无
////->返回值			:	无
////->例程			:
////->调用函数		:
////->被调用函数	:
////<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
//void	GT32_DMA_Configuration(u32 *GT32L32_TX_Addr,u32 *GT32L32_RX_Addr,u32 BufferSize)
//{
//	//1)____________定义
//	DMA_InitTypeDef			DMA_Initstructure;
//	NVIC_InitTypeDef		NVIC_Initstructure;
//	
//	//2)____________DMA发送初始化，外设作为DMA的目的端
//	DMA_Initstructure.DMA_PeripheralBaseAddr	=	(u32)(&SPI1->DR);								//DMA外设源地址
//	DMA_Initstructure.DMA_MemoryBaseAddr			=	(u32)GT32L32_TX_Addr;								//DMA数据内存地址
//	DMA_Initstructure.DMA_DIR									= DMA_DIR_PeripheralDST;					//DMA_DIR_PeripheralDST（外设作为DMA的目的端），DMA_DIR_PeripheralSRC（外设作为数据传输的来源）
//	DMA_Initstructure.DMA_BufferSize					= BufferSize; 										//指定DMA通道的DMA缓存的大小
//	DMA_Initstructure.DMA_PeripheralInc				= DMA_PeripheralInc_Disable;			//DMA_PeripheralInc_Enable（外设地址寄存器递增），DMA_PeripheralInc_Disable（外设地址寄存器不变），
//	DMA_Initstructure.DMA_MemoryInc						=	DMA_MemoryInc_Enable;						//DMA_MemoryInc_Enable（内存地址寄存器递增），DMA_MemoryInc_Disable（内存地址寄存器不变）
//	DMA_Initstructure.DMA_PeripheralDataSize	= DMA_PeripheralDataSize_Byte;		//外设数据宽度--DMA_PeripheralDataSize_Byte（数据宽度为8位），DMA_PeripheralDataSize_HalfWord（数据宽度为16位），DMA_PeripheralDataSize_Word（数据宽度为32位）
//	DMA_Initstructure.DMA_MemoryDataSize			= DMA_MemoryDataSize_Byte;				//内存数据宽度--DMA_MemoryDataSize_Byte（数据宽度为8位），DMA_MemoryDataSize_HalfWord（数据宽度为16位），DMA_MemoryDataSize_Word（数据宽度为32位）
//	DMA_Initstructure.DMA_Mode								= DMA_Mode_Normal;								//DMA工作模式--DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular（不停地传送）
//	DMA_Initstructure.DMA_Priority						= DMA_Priority_High; 							//DMA通道的转输优先级--DMA_Priority_VeryHigh（非常高）DMA_Priority_High（高)，DMA_Priority_Medium（中），DMA_Priority_Low（低）
//	DMA_Initstructure.DMA_M2M									= DMA_M2M_Disable;								//DMA通道的内存到内存传输--DMA_M2M_Enable(设置为内存到内存传输)，DMA_M2M_Disable（非内存到内存传输）
//	DMA_Init(DMA1_Channel3,&DMA_Initstructure);																	//初始化DMA

//	//3)____________DMA接收初始化，外设作为DMA的源端
//	DMA_Initstructure.DMA_PeripheralBaseAddr	=	(u32)(&SPI1->DR);								//DMA外设源地址
//	DMA_Initstructure.DMA_MemoryBaseAddr     	=	(u32)GT32L32_RX_Addr;								//DMA数据内存地址
//	DMA_Initstructure.DMA_DIR									=	DMA_DIR_PeripheralSRC;					//DMA_DIR_PeripheralDST（外设作为DMA的目的端），DMA_DIR_PeripheralSRC（外设作为数据传输的来源）
//	DMA_Initstructure.DMA_BufferSize					= BufferSize; 										//指定DMA通道的DMA缓存的大小
//	DMA_Initstructure.DMA_PeripheralInc				= DMA_PeripheralInc_Disable;			//DMA_PeripheralInc_Enable（外设地址寄存器递增），DMA_PeripheralInc_Disable（外设地址寄存器不变），
//	DMA_Initstructure.DMA_MemoryInc						=	DMA_MemoryInc_Enable;						//DMA_MemoryInc_Enable（内存地址寄存器递增），DMA_MemoryInc_Disable（内存地址寄存器不变）
//	DMA_Initstructure.DMA_PeripheralDataSize	= DMA_PeripheralDataSize_Byte;		//外设数据宽度--DMA_PeripheralDataSize_Byte（数据宽度为8位），DMA_PeripheralDataSize_HalfWord（数据宽度为16位），DMA_PeripheralDataSize_Word（数据宽度为32位）
//	DMA_Initstructure.DMA_MemoryDataSize			= DMA_MemoryDataSize_Byte;				//内存数据宽度--DMA_MemoryDataSize_Byte（数据宽度为8位），DMA_MemoryDataSize_HalfWord（数据宽度为16位），DMA_MemoryDataSize_Word（数据宽度为32位）
//	DMA_Initstructure.DMA_Mode								= DMA_Mode_Normal;								//DMA工作模式--DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular（不停地传送）
//	DMA_Initstructure.DMA_Priority						= DMA_Priority_High; 							//DMA通道的转输优先级--DMA_Priority_VeryHigh（非常高）DMA_Priority_High（高)，DMA_Priority_Medium（中），DMA_Priority_Low（低）
//	DMA_Initstructure.DMA_M2M									= DMA_M2M_Disable;								//DMA通道的内存到内存传输--DMA_M2M_Enable(设置为内存到内存传输)，DMA_M2M_Disable（非内存到内存传输）
//	DMA_Init(DMA1_Channel2,&DMA_Initstructure);																	//初始化DMA
//	
//	//4)____________DMA通道中断初始化---此为DMA发送中断----DMA发送完成中断
//	NVIC_Initstructure.NVIC_IRQChannel = DMA1_Channel2_IRQChannel;       				//设置中断源
//	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority=1;
//	NVIC_Initstructure.NVIC_IRQChannelSubPriority = 1; 													//中断响应优先级0	
//	NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;        											//打开中断
//	NVIC_Init(&NVIC_Initstructure);

//	//5)____________设置触发中断事件
//	//		DMA_IT_TC		传输完成中断
//	//		DMA_IT_HT		传输过半中断
//	//		DMA_IT_TE		传输错误中断
//	DMA_ITConfig(DMA1_Channel2,DMA_IT_TC,ENABLE);						//传输完成中断屏蔽
//	//6)____________使能DMA传输通道	
//	SPI_I2S_DMACmd(GT32L32_SPI_PORT, SPI_I2S_DMAReq_Tx, ENABLE);				//使能DMA发送
//	SPI_I2S_DMACmd(GT32L32_SPI_PORT, SPI_I2S_DMAReq_Rx, ENABLE);				//使能DMA接收

//	//6)____________禁止DMA---需要传输时再打开
//	DMA_Cmd(DMA1_Channel2,DISABLE);	
//	DMA_Cmd(DMA1_Channel3,DISABLE);
//}

///*****************************************************************************
//点阵数字符号字符
//说明：																					  --------------------
//此部分内容为0 1 2 3 4 5 6 7 8 9 , . ￥ $ ￡
//Squence：表示 字符顺序，从0开始计数。
//BaseAdd：说明该套字库在芯片中的起始地址。
//Address：对应字符点阵在芯片中的字节地址。
//******************************************************************************/
//u8  GT32L32_GetDig_Info(u8 Sequence,GT32L32_Info_TypeDef *GT32L32_Info)
//{
//	//____________定义变量
//	u8 len=0;	//数据长度变量
//	u32 address=0;				//地址变量
//	u32 BaseAddr=GT32L32_Info->GT32L32_BaseAddr;	
//	//____________配置数据
//	switch(BaseAddr)
//	{
//	case 0x112400:	address=Sequence * 56+ BaseAddr;
//									len=56;	//GT32L32_ReadBuffer(Sequence * 56+ BaseAdd,56,DZ_Data); //14X28
//				break ;
//	case 0x112748: 	address=Sequence * 120+ BaseAddr;
//									len=120;//GT32L32_ReadBuffer(Sequence * 120+ BaseAdd,120,DZ_Data); //20X40
//				break ;
//	case 0x112CE8: 	address=Sequence * 114+ BaseAddr+2;
//									len=112;//GT32L32_ReadBuffer(Sequence * 114+ BaseAdd+2,112,DZ_Data); //28X28
//				break ;
//	case 0x113396: 	address=Sequence * 202+ BaseAddr+2;
//									len=200;//GT32L32_ReadBuffer(Sequence * 202+ BaseAdd+2,200,DZ_Data); //40X40
//				break ;  
//	default:       break ;
//	}
//	//____________更新数据
//	GT32L32_Info->GT32L32_Address=address;
//	GT32L32_Info->GT32L32_BufferSize=len;
//	return 0;	  
//}

///***************************************
//ASCII 调用
//ASCIICode：表示ASCII 码（8bits）
//BaseAdd：说明该套字库在芯片中的起始地址。
//GT32L32_ReadBuffer： 是读点阵数据函数。
//GetBuffer：是保存读出的点阵数据的数组。
//****************************************/
//u8  GT32L32_GetASCII_Info(u8 ASCIICode,GT32L32_Info_TypeDef *GT32L32_Info)
//{
//	//____________定义变量
//	u8 len=0;	//数据长度变量
//	u32 address=0;				//地址变量
//	u32 BaseAddr=GT32L32_Info->GT32L32_BaseAddr;
//	
//	//____________配置数据	
//	if  ((ASCIICode >= 0x20)&&(ASCIICode<=0x7e))
//	{	
//	  switch(BaseAddr)
//	  {
//	  case 0x100000:	address=(ASCIICode-0x20)*8+BaseAddr;
//										len=8;//GT32L32_ReadBuffer((ASCIICode-0x20)*8+BaseAdd,8,DZ_Data); //5X7
//					break ;
//	  case 0x100300:	address=(ASCIICode-0x20)*8+BaseAddr;
//										len=8;//GT32L32_ReadBuffer((ASCIICode-0x20)*8+BaseAdd,8,DZ_Data); //7X8
//					break ;
//	  case 0x100600:	address=(ASCIICode-0x20)*8+BaseAddr;
//										len=8;//GT32L32_ReadBuffer((ASCIICode-0x20)*8+BaseAdd,8,DZ_Data); //7X8 Fat
//					break ;
//	  case 0x100900:	address=(ASCIICode-0x20)*12+BaseAddr;
//										len=12;//GT32L32_ReadBuffer((ASCIICode-0x20)*12+BaseAdd,12,DZ_Data); //6X12 
//					break ;
//	  case 0x100D80:	address=(ASCIICode-0x20)*16+BaseAddr;
//										len=16;//GT32L32_ReadBuffer((ASCIICode-0x20)*16+BaseAdd,16,DZ_Data); //8X16
//					break ;  
//	  case 0x101580:	address=(ASCIICode-0x20)*16+BaseAddr;
//										len=16;//GT32L32_ReadBuffer((ASCIICode-0x20)*16+BaseAdd,16,DZ_Data); // 8X16 Fat 
//					break ;
//	  case 0x101B80:	address=(ASCIICode-0x20)*48+BaseAddr;
//										len=48;//GT32L32_ReadBuffer((ASCIICode-0x20)*48+BaseAdd,48,DZ_Data); //12X24
//					break ;  
//	  case 0x102D80: 	address=(ASCIICode-0x20)*64+BaseAddr;
//										len=64;//GT32L32_ReadBuffer((ASCIICode-0x20)*64+BaseAdd,64,DZ_Data); //16X32
//					break ;
//	  case 0x104580: 	address=(ASCIICode-0x20)*64+BaseAddr;
//										len=64;//GT32L32_ReadBuffer((ASCIICode-0x20)*64+BaseAdd,64,DZ_Data); //16X32 Fat
//					break ;
//	  case 0x105D80: 	address=(ASCIICode-0x20)*26+BaseAddr+2;
//										len=24;//GT32L32_ReadBuffer((ASCIICode-0x20)*26+BaseAdd+2,24,DZ_Data); //12X12 A 
//					break ;
//	  case 0x106740: 	address=(ASCIICode-0x20)*34+BaseAddr+2;
//										len=32;//GT32L32_ReadBuffer((ASCIICode-0x20)*34+BaseAdd+2,32,DZ_Data); //16X16 Arial
//					break ; 
//	  case 0x107400: 	address=(ASCIICode-0x20)*74+BaseAddr+2;
//										len=72;//GT32L32_ReadBuffer((ASCIICode-0x20)*74+BaseAdd+2,72,DZ_Data); //24X24 
//					break ;
//	  case 0x108FC0: 	address=(ASCIICode-0x20)*130+BaseAddr+2;
//										len=128;//GT32L32_ReadBuffer((ASCIICode-0x20)*130+BaseAdd+2,128,DZ_Data); //32X32 
//					break ;
//	  case 0x10C080: 	address=(ASCIICode-0x20)*26+BaseAddr+2;
//										len=24;//GT32L32_ReadBuffer((ASCIICode-0x20)*26+BaseAdd+2,24,DZ_Data); //12X12 T 
//					break ;
//	  case 0x10CA50: 	address=(ASCIICode-0x20)*34+BaseAddr+2;
//										len=32;//GT32L32_ReadBuffer((ASCIICode-0x20)*34+BaseAdd+2,32,DZ_Data); //16X16 T
//					break ;
//	  case 0x10D740: 	address=(ASCIICode-0x20)*74+BaseAddr+2;
//										len=72;//GT32L32_ReadBuffer((ASCIICode-0x20)*74+BaseAdd+2,72,DZ_Data); //24X24 T 
//					break ;
//	  case 0x10F340: 	address=(ASCIICode-0x20)*130+BaseAddr+2;
//										len=128;//GT32L32_ReadBuffer((ASCIICode-0x20)*130+BaseAdd+2,128,DZ_Data); //32X32 T 
//					break ;  
//	  default: 
//					break ;
//	  }
//		//____________更新数据
//		GT32L32_Info->GT32L32_Address=address;			//字符地址
//		GT32L32_Info->GT32L32_BufferSize=len;				//点阵数据长度
//	 return 1;
//	}
//	else
//	{
//		GT32L32_Info->GT32L32_Address=address;								//字符地址
//		GT32L32_Info->GT32L32_BufferSize=len;									//点阵数据长度
//		GT32L32_Info->GT32L32_Error=GT32L32_Error_ASCIICode;	//ASCII码错误
//		return 0; 
//	}
//}
///*************************************************************************************************** 
//16x16点阵GB18030汉字&字符
//函数：u8 GB18030_16_GetData(u8 c1, u8 c2, u8 c3, u8 c4) 
//功能：计算汉字点阵在芯片中的地址,读取点阵数据到指定数组。 
//参数：c1,c2,c3,c4：
//4字节汉字内码通过参数c1,c2,c3,c4传入，双字节内码通过参数c1,c2 传入，c3=0,c4=0 
//返回：汉字点阵的字节地址(byte address)。
//如果用户是按 word mode 读取点阵数据，则其地址(word address)为字节地址除以2，
//即：word address = byte address / 2 . 例如：
//BaseAdd: 说明汉字点阵数据在字库芯片中的起始地址，即BaseAdd＝0x114FDE;
// “啊”字的内码为0xb0a1,则byte address = GB18030_16_GetData(0xb0,0xa1,0x00,0x00) *32+BaseAdd; 
//word address = byte address / 2 
//“.”字的内码为0x8139ee39,则byte address = GB18030_16_GetData(0x81,0x39,0xee,0x39) *32+ BaseAdd
//word address = byte address / 2 
//****************************************************************************************************/
//u8 GT32L32_GetGB18030_Info(u16 word,GT32L32_Info_TypeDef *GT32L32_Info) 
//{
//	//____________定义变量
//	u8	len=0;	//数据长度变量
//	u32 address=0;				//地址变量
//	u32 BaseAddr=GT32L32_Info->GT32L32_BaseAddr;
//	
//	u8	c1=word>>8;
//	u8	c2=word;
//	u8	c3=0;
//	u8	c4=0;
//	u8	l1=0;
//	//____________字体判断
//	switch(BaseAddr)
//	{
//		case	GB18030_BaseAddr_zf12x12	:	l1=0;len=24;
//					break;
//		case	GB18030_BaseAddr_zf16x16	:	l1=0;len=32;
//					break;
//		case	GB18030_BaseAddr_zf24x24	:	l1=0;len=72;
//					break;
//		case	GB18030_BaseAddr_zf32x32	:	l1=192;len=128;
//					break;
//		default: 
//					break ;
//	}
//	//____________数据检查
//	if(c2==0x7F) 	//空
//	{
//		GT32L32_Info->GT32L32_Error=GT32L32_Error_HzEmp;	//汉字内码低8位为0x7F,为空内容
//		return 0;
//	}
//	//0）____________配置数据		
//	if(c1>=0xA1 && c1 <= 0xa9 && c2>=0xa1) //Section 1 
//		address= (c1 - 0xA1) * 94 + (c2 - 0xA1); 
//	else if(c1>=0xa8 && c1 <= 0xa9 && c2<0xa1) //Section 5 
//	{ 
//		if(c2>0x7f)
//			c2--; 
//		address=(c1-0xa8)*96 + (c2-0x40)+846; 
//	} 
//	if(c1>=0xb0 && c1 <= 0xf7 && c2>=0xa1) //Section 2 
//		address= (c1 - 0xB0) * 94 + (c2 - 0xA1)+1038-l1; 
//	else if(c1<0xa1 && c1>=0x81 && c2>=0x40 ) //Section 3 
//	{ 
//		if(c2>0x7f) 
//			c2--;
//		address=(c1-0x81)*190 + (c2-0x40) + 1038 +6768-l1;
//	} 
//	else if(c1>=0xaa && c2<0xa1) //Section 4 
//	{ 
//		if(c2>0x7f) 
//			c2--; 
//		address=(c1-0xaa)*96 + (c2-0x40) + 1038 +12848-l1; 
//	}
//	else if(c1==0x81 && c2>=0x39) //四字节区1 
//	{ 
//		address =1038 + 21008+(c3-0xEE)*10+c4-0x39-l1; 
//	} 
//	else if(c1==0x82)//四字节区2 
//	{ 
//		address =1038 + 21008+161+(c2-0x30)*1260+(c3-0x81)*10+c4-0x30-l1; 
//	}
//	//____________更新数据
//	address=address*len+BaseAddr;
//	GT32L32_Info->GT32L32_Address=address;			//字符地址
//	GT32L32_Info->GT32L32_BufferSize=len;				//点阵数据长度	
//	
//	return	1; 
//}

///*************************************************************************************************** 
//16x16点阵GB18030汉字&字符
//函数：u8 GB18030_16_GetData(u8 c1, u8 c2, u8 c3, u8 c4) 
//功能：计算汉字点阵在芯片中的地址,读取点阵数据到指定数组。 
//参数：c1,c2,c3,c4：
//4字节汉字内码通过参数c1,c2,c3,c4传入，双字节内码通过参数c1,c2 传入，c3=0,c4=0 
//返回：汉字点阵的字节地址(byte address)。
//如果用户是按 word mode 读取点阵数据，则其地址(word address)为字节地址除以2，
//即：word address = byte address / 2 . 例如：
//BaseAdd: 说明汉字点阵数据在字库芯片中的起始地址，即BaseAdd＝0x114FDE;
// “啊”字的内码为0xb0a1,则byte address = GB18030_16_GetData(0xb0,0xa1,0x00,0x00) *32+BaseAdd; 
//word address = byte address / 2 
//“.”字的内码为0x8139ee39,则byte address = GB18030_16_GetData(0x81,0x39,0xee,0x39) *32+ BaseAdd
//word address = byte address / 2 
//****************************************************************************************************/
//u8 GT32L32_Get_Info(u16 word,GT32L32_Info_TypeDef *GT32L32_Info) 
//{ 
//	//____________定义变量
//	u8 len=0;	//数据长度变量
//	u32 address=0;				//地址变量
//	u32 GT32L32_BaseAddress=GT32L32_Info->GT32L32_BaseAddr;
//	//0）____________基地址判断
//	if((GT32L32_BaseAddress<ASCII_BaseAddr_zf5x7)||(GT32L32_BaseAddress>=Reserved_BaseAddr_LSB))
//	{
//		GT32L32_Info->GT32L32_Error=GT32L32_Error_Addr;
//		return 0;
//	}
//	//1）____________获取数字地址&点阵字符长度信息
//	else if(GT32L32_BaseAddress==Digital_BaseAddr_sz14x28||GT32L32_BaseAddress==Digital_BaseAddr_sz20x40||GT32L32_BaseAddress==Digital_BaseAddr_sz28||GT32L32_BaseAddress==Digital_BaseAddr_sz40)
//	{
//		GT32L32_GetDig_Info(word&0xFF,GT32L32_Info);
//		return 1;
//	}
//	//2）____________获取ASCII字符地址&点阵字符长度信息
//	else if(GT32L32_BaseAddress<=ASCII_BaseAddr_TNR32)
//	{
//		GT32L32_GetASCII_Info(word&0xFF,GT32L32_Info);
//		return 1;
//	}
//	//3）____________获取字符地址&点阵字符长度信息
//	else if(GT32L32_BaseAddress==GB18030_BaseAddr_hz12x12||GT32L32_BaseAddress==GB18030_BaseAddr_hz16x16||GT32L32_BaseAddress==GB18030_BaseAddr_hz24x24||GT32L32_BaseAddress==GB18030_BaseAddr_hz32x32)
//	{
//		GT32L32_GetGB18030_Info(word,GT32L32_Info);
//	}
//	//4）获取条码地址
//	//5）获取天线符号地址
//	//6）获取电池符号地址
////	u32 BaseAdd=0x194FDE; 
//	
////	return(Address*32+BaseAddr);
//		return 0;
//}
///*************************************************************************************************** 
//16x16点阵GB18030汉字&字符
//函数：u8 GB18030_16_GetData(u8 c1, u8 c2, u8 c3, u8 c4) 
//功能：计算汉字点阵在芯片中的地址,读取点阵数据到指定数组。 
//参数：c1,c2,c3,c4：
//4字节汉字内码通过参数c1,c2,c3,c4传入，双字节内码通过参数c1,c2 传入，c3=0,c4=0 
//返回：汉字点阵的字节地址(byte address)。
//如果用户是按 word mode 读取点阵数据，则其地址(word address)为字节地址除以2，
//即：word address = byte address / 2 . 例如：
//BaseAdd: 说明汉字点阵数据在字库芯片中的起始地址，即BaseAdd＝0x114FDE;
// “啊”字的内码为0xb0a1,则byte address = GB18030_16_GetData(0xb0,0xa1,0x00,0x00) *32+BaseAdd; 
//word address = byte address / 2 
//“.”字的内码为0x8139ee39,则byte address = GB18030_16_GetData(0x81,0x39,0xee,0x39) *32+ BaseAdd
//word address = byte address / 2 
//****************************************************************************************************/
//u32 GT32L32_GetBufferLen(u16 word,GT32L32_BaseAddr_TypeDef *BaseAddr,u8 c1, u8 c2, u8 c3, u8 c4) 
//{ 
//	u32 len=0;
//	u32 Address=0;
////	u32 BaseAdd=0x194FDE; 
//	if(c2==0x7f) 
//	{
//		return (0);
//	} 
//	if(c1>=0xA1 && c1 <= 0xa9 && c2>=0xa1) //Section 1 
//		Address= (c1 - 0xA1) * 94 + (c2 - 0xA1); 
//	else if(c1>=0xa8 && c1 <= 0xa9 && c2<0xa1) //Section 5 
//	{ 
//		if(c2>0x7f)
//			c2--; 
//		Address=(c1-0xa8)*96 + (c2-0x40)+846; 
//	} 
//	if(c1>=0xb0 && c1 <= 0xf7 && c2>=0xa1) //Section 2 
//		Address= (c1 - 0xB0) * 94 + (c2 - 0xA1)+1038; 
//	else if(c1<0xa1 && c1>=0x81 && c2>=0x40 ) //Section 3 
//	{ 
//		if(c2>0x7f) 
//			c2--;
//		Address=(c1-0x81)*190 + (c2-0x40) + 1038 +6768;
//	} 
//	else if(c1>=0xaa && c2<0xa1) //Section 4 
//	{ 
//		if(c2>0x7f) 
//			c2--; 
//		Address=(c1-0xaa)*96 + (c2-0x40) + 1038 +12848; 
//	}
//	else if(c1==0x81 && c2>=0x39) //四字节区1 
//	{ 
//		Address =1038 + 21008+(c3-0xEE)*10+c4-0x39; 
//	} 
//	else if(c1==0x82)//四字节区2 
//	{ 
//		Address =1038 + 21008+161+(c2-0x30)*1260+(c3-0x81)*10+c4-0x30; 
//	} 
////	GT32L32_ReadBuffer(Address*32+BaseAddr,32,GetBuffer);
//	return(Address*32+*BaseAddr); 
//}
///*************************************************************************************************** 
//12x12点阵GB18030汉字&字符
//函数：u32 GB18030_12_GetData(u8 c1, u8 c2, u8 c3, u8 c4) 
//功能：计算汉字点阵在芯片中的地址，读取点阵数据到指定数组。 
//参数：汉字内码通过参数c1,c2传入，c3=0;c4=0. 
//注：12x12为GBK字符集，无四字节区。 
//返回：汉字点阵的字节地址(byte address)。
//如果用户是按 word mode 读取点阵数据，则其地址(word address)为字节地址除以2，
//即：word address = byte address / 2 . 例如：
//BaseAdd: 说明汉字点阵数据在字库芯片中的起始地址，即BaseAdd＝0x093D0E，
//“啊”字的内码为0xb0a1,则byte address = GB18030_12_GetData(0xb0,0xa1,0x00,0x00) *24+BaseAdd ,
//word address = byte address / 2	.
//****************************************************************************************************/
//#ifdef	STM32_GT32L32M0180_Code_Edit
//u32 GT32L32_GetGB18030_12(u8 c1, u8 c2, u8 c3, u8 c4,u8 *GetBuffer) 
//{
//	u32 h=0;
//	u32 BaseAdd=0x113D0E; 
//	
//	return 0;
//}

//#else

//u32 GT32L32_GetGB18030_12(u8 c1, u8 c2, u8 c3, u8 c4,u8 *GetBuffer) 
//{
//	u32 h=0;
//	u32 BaseAdd=0x113D0E; 
//	if(c2==0x7f) 
//	{
//		GT32L32_ReadBuffer(BaseAdd,24,GetBuffer);
//		return (BaseAdd);
//	} 
//	if(c1>=0xA1 && c1 <= 0xa9 && c2>=0xa1) //Section 1 
//		h= (c1 - 0xA1) * 94 + (c2 - 0xA1); 
//	else if(c1>=0xa8 && c1 <= 0xa9 && c2<0xa1) //Section 5 
//	{ 
//		if(c2>0x7f)
//			c2--; 
//		h=(c1-0xa8)*96 + (c2-0x40)+846; 
//	} 
//	if(c1>=0xb0 && c1 <= 0xf7 && c2>=0xa1) //Section 2 
//			h= (c1 - 0xB0) * 94 + (c2 - 0xA1)+1038; 
//	else if(c1<0xa1 && c1>=0x81 && c2>=0x40 ) //Section 3 
//	{ 
//		if(c2>0x7f) 
//			c2--;
//		h=(c1-0x81)*190 + (c2-0x40) + 1038 +6768;
//	} 
//	else if(c1>=0xaa && c2<0xa1) //Section 4 
//	{ 
//		if(c2>0x7f) 
//			c2--; 
//		h=(c1-0xaa)*96 + (c2-0x40) + 1038 +12848; 
//	} 
//	GT32L32_ReadBuffer(h*24+BaseAdd,24,GetBuffer);
//	return(h*24+BaseAdd); 
//}
//#endif
///*************************************************************************************************** 
//16x16点阵GB18030汉字&字符
//函数：u8 GB18030_16_GetData(u8 c1, u8 c2, u8 c3, u8 c4) 
//功能：计算汉字点阵在芯片中的地址,读取点阵数据到指定数组。 
//参数：c1,c2,c3,c4：
//4字节汉字内码通过参数c1,c2,c3,c4传入，双字节内码通过参数c1,c2 传入，c3=0,c4=0 
//返回：汉字点阵的字节地址(byte address)。
//如果用户是按 word mode 读取点阵数据，则其地址(word address)为字节地址除以2，
//即：word address = byte address / 2 . 例如：
//BaseAdd: 说明汉字点阵数据在字库芯片中的起始地址，即BaseAdd＝0x114FDE;
// “啊”字的内码为0xb0a1,则byte address = GB18030_16_GetData(0xb0,0xa1,0x00,0x00) *32+BaseAdd; 
//word address = byte address / 2 
//“.”字的内码为0x8139ee39,则byte address = GB18030_16_GetData(0x81,0x39,0xee,0x39) *32+ BaseAdd
//word address = byte address / 2 
//****************************************************************************************************/
//u32 GT32L32_GetGB18030_16(u8 c1, u8 c2, u8 c3, u8 c4,u8 *GetBuffer) 
//{ 
//	u32 h=0;
//	u32 BaseAdd=0x194FDE; 
//	if(c2==0x7f) 
//	{
//		GT32L32_ReadBuffer(BaseAdd,32,GetBuffer);
//		return (BaseAdd);
//	} 
//	if(c1>=0xA1 && c1 <= 0xa9 && c2>=0xa1) //Section 1 
//		h= (c1 - 0xA1) * 94 + (c2 - 0xA1); 
//	else if(c1>=0xa8 && c1 <= 0xa9 && c2<0xa1) //Section 5 
//	{ 
//		if(c2>0x7f)
//			c2--; 
//		h=(c1-0xa8)*96 + (c2-0x40)+846; 
//	} 
//	if(c1>=0xb0 && c1 <= 0xf7 && c2>=0xa1) //Section 2 
//		h= (c1 - 0xB0) * 94 + (c2 - 0xA1)+1038; 
//	else if(c1<0xa1 && c1>=0x81 && c2>=0x40 ) //Section 3 
//	{ 
//		if(c2>0x7f) 
//			c2--;
//		h=(c1-0x81)*190 + (c2-0x40) + 1038 +6768;
//	} 
//	else if(c1>=0xaa && c2<0xa1) //Section 4 
//	{ 
//		if(c2>0x7f) 
//			c2--; 
//		h=(c1-0xaa)*96 + (c2-0x40) + 1038 +12848; 
//	}
//	else if(c1==0x81 && c2>=0x39) //四字节区1 
//	{ 
//		h =1038 + 21008+(c3-0xEE)*10+c4-0x39; 
//	} 
//	else if(c1==0x82)//四字节区2 
//	{ 
//		h =1038 + 21008+161+(c2-0x30)*1260+(c3-0x81)*10+c4-0x30; 
//	} 
//	GT32L32_ReadBuffer(h*32+BaseAdd,32,GetBuffer);
//	return(h*32+BaseAdd); 
//}

///*************************************************************************************************** 
//24x24点阵GB18030汉字&字符
//函数：u8 GB18030_24_GetData(u8 c1, u8 c2, u8 c3, u8 c4) 
//功能：计算汉字点阵在芯片中的地址,读取点阵数据到指定数组。 
//参数：c1,c2,c3,c4：
//4字节汉字内码通过参数c1,c2,c3,c4传入，双字节内码通过参数c1,c2 传入，c3=0,c4=0 
//返回：汉字点阵的字节地址(byte address)。
//如果用户是按 word mode 读取点阵数据，则其地址(word address)为字节地址除以2，
//即：word address = byte address / 2 . 例如：
//BaseAdd: 说明汉字点阵数据在字库芯片中的起始地址，即BaseAdd＝0x1F43DE;
// “啊”字的内码为0xb0a1,则byte address = GB18030_24_GetData(0xb0,0xa1,0x00,0x00) *72+BaseAdd; 
//word address = byte address / 2 
//“.”字的内码为0x8139ee39,则byte address = GB18030_24_GetData(0x81,0x39,0xee,0x39) *72+ BaseAdd
//word address = byte address / 2 
//****************************************************************************************************/
//u32 GT32L32_GetGB18030_24(u8 c1, u8 c2, u8 c3, u8 c4,u8 *GetBuffer) 
//{ 
//	u32 h=0;
//	u32 BaseAdd=0x2743DE; 
//	if(c2==0x7f) 
//	{
//		GT32L32_ReadBuffer(BaseAdd,72,GetBuffer);
//		return (BaseAdd);
//	} 
//	if(c1>=0xA1 && c1 <= 0xa9 && c2>=0xa1) //Section 1 
//		h= (c1 - 0xA1) * 94 + (c2 - 0xA1); 
//	else if(c1>=0xa8 && c1 <= 0xa9 && c2<0xa1) //Section 5 
//	{ 
//		if(c2>0x7f) c2--; 
//		h=(c1-0xa8)*96 + (c2-0x40)+846; 
//	} 
//	if(c1>=0xb0 && c1 <= 0xf7 && c2>=0xa1) //Section 2 
//		h= (c1 - 0xB0) * 94 + (c2 - 0xA1)+1038; 
//	else if(c1<0xa1 && c1>=0x81 && c2>=0x40 ) //Section 3 
//	{ 
//		if(c2>0x7f) 
//			c2--;
//		h=(c1-0x81)*190 + (c2-0x40) + 1038 +6768;
//	} 
//	else if(c1>=0xaa && c2<0xa1) //Section 4 
//	{ 
//		if(c2>0x7f) 
//			c2--; 
//		h=(c1-0xaa)*96 + (c2-0x40) + 1038 +12848; 
//	}
//	else if(c1==0x81 && c2>=0x39) //四字节区1 
//	{ 
//		h =1038 + 21008+(c3-0xEE)*10+c4-0x39; 
//	} 
//	else if(c1==0x82)//四字节区2 
//	{ 
//		h =1038 + 21008+161+(c2-0x30)*1260+(c3-0x81)*10+c4-0x30; 
//	} 
//	GT32L32_ReadBuffer(h*72+BaseAdd,72,GetBuffer);
//	return(h*72+BaseAdd); 
//}

///*************************************************************************************************** 
//32x32点阵GB18030汉字&字符
//函数：u32 GB18030_32_GetData (u8 c1, u8 c2, u8 c3, u8 c4)  
//功能：计算汉字点阵在芯片中的地址 
//参数：c1,c2,c3,c4：4字节汉字内码通过参数c1,c2,c3,c4传入，双字节内码通过参数c1,c2传 入，c3=0,c4=0 
//返回：汉字点阵的字节地址(byte address)。如果用户是按 word mode 读取点阵数据，
//则其地 址(word address)为字节地址除以2，即：word address = byte address / 2 . 
//例如：BaseAdd: 说明汉字点阵数据在字库芯片中的起始地址，即BaseAdd＝0x47AE10; “啊”字的内码为0xb0a1,
//则byte address = gt(0xb0,0xa1,0x00,0x00) *128+BaseAdd word address = byte address / 2 
//“.”字的内码为0x8139ee39,则byte address = gt(0x81, 0x39,0xee,0x39) *128+ BaseAdd word address = byte address / 2 
//****************************************************************************************************/
//u32 GT32L32_GetGB18030_32(u8 c1, u8 c2, u8 c3, u8 c4,u8 *GetBuffer) 
//{ 
//	u32 h=0;
//	u32  BaseAdd=0x47AE10;
//	if(c2==0x7f) 
//	{
//		GT32L32_ReadBuffer(BaseAdd,128,GetBuffer);
//		return (BaseAdd); 
//	}
//	if(c1>=0xA1 && c1 <= 0xAB && c2>=0xa1) //Section 1 
//		h= (c1 - 0xA1) * 94 + (c2 - 0xA1); 
//	else if(c1>=0xa8 && c1 <= 0xa9 && c2<0xa1) //Section 5 
//	{ 
//		if(c2>0x7f) 
//			c2--; 
//		h=(c1-0xa8)*96 + (c2-0x40)+846; 
//	} 
//	if(c1>=0xb0 && c1 <= 0xf7 && c2>=0xa1) //Section 2 
//		h= (c1 - 0xB0) * 94 + (c2 - 0xA1)+1038-192; 
//	else if(c1<0xa1 && c1>=0x81 && c2>=0x40) //Section 3 
//	{ 
//		if(c2>0x7f) 
//			c2--; 
//		h=(c1-0x81)*190 + (c2-0x40) + 1038 +6768-192; 
//	} 
//	else if(c1>=0xaa && c2<0xa1) //Section 4 
//	{ 
//		if(c2>0x7f)
//			c2--;
//		h=(c1-0xaa)*96 + (c2-0x40) + 1038 +12848-192; 
//	} 
//	else if(c1==0x81 && c2>=0x39) //四字节区1 
//	{ 
//		h =1038 + 21008+(c3-0xEE)*10+c4-0x39-192; 
//	} 
//	else if(c1==0x82)//四字节区2 
//	{ 
//		h =1038 + 21008+161+(c2-0x30)*1260+(c3-0x81)*10+c4-0x30-192; 
//	} 
//	GT32L32_ReadBuffer(h*128+BaseAdd,128,GetBuffer);
//	return(h*128+BaseAdd);  
//} 

///**********************************************************************
//EAN13条形码调用程序
//函数：DWORD* BAR_CODE(int* BAR_NUM)
//功能：将数组条形码转为对应条形码图形地址。
//参数：int* BAR_NUM 条形码数字数组指针，BAR_NUM[13]数组包含13个数字。
//返回：定义DWORD BAR_PIC_ADDR[13];用于存放对应地址，返回此数组指针。
//***********************************************************************/

//u32 GT32L32_GetBarCode_13(u8 * BAR_NUM)
//{
//	u32 i,BaseAddr=0x478FD2;
//	BAR_PIC_ADDR[0]=BAR_NUM[0]*54+540*0+ BaseAddr;
//	BAR_PIC_ADDR[1]=BAR_NUM[1]*54+540*1+ BaseAddr;
//	switch(BAR_NUM[0])
//	{
//		case 0:
//			for(i=2;i<=6;i++)
//			{
//				BAR_PIC_ADDR[i]=BAR_NUM[i]*54+540*1+ BaseAddr;
//			}
//		break;
//		case 1:
//			BAR_PIC_ADDR[2]=BAR_NUM[2]*54+540*1+ BaseAddr;
//			BAR_PIC_ADDR[3]=BAR_NUM[3]*54+540*2+ BaseAddr;
//			BAR_PIC_ADDR[4]=BAR_NUM[4]*54+540*1+ BaseAddr;
//			BAR_PIC_ADDR[5]=BAR_NUM[5]*54+540*2+ BaseAddr;
//			BAR_PIC_ADDR[6]=BAR_NUM[6]*54+540*2+ BaseAddr;
//		break;
//		case 2:
//			BAR_PIC_ADDR[2]=BAR_NUM[2]*54+540*1+ BaseAddr;
//			BAR_PIC_ADDR[3]=BAR_NUM[3]*54+540*2+ BaseAddr;
//			BAR_PIC_ADDR[4]=BAR_NUM[4]*54+540*2+ BaseAddr;
//			BAR_PIC_ADDR[5]=BAR_NUM[5]*54+540*1+ BaseAddr;
//			BAR_PIC_ADDR[6]=BAR_NUM[6]*54+540*2+ BaseAddr;
//		break;
//		case 3:
//			BAR_PIC_ADDR[2]=BAR_NUM[2]*54+540*1+ BaseAddr;
//			BAR_PIC_ADDR[3]=BAR_NUM[3]*54+540*2+ BaseAddr;
//			BAR_PIC_ADDR[4]=BAR_NUM[4]*54+540*2+ BaseAddr;
//			BAR_PIC_ADDR[5]=BAR_NUM[5]*54+540*2+ BaseAddr;
//			BAR_PIC_ADDR[6]=BAR_NUM[6]*54+540*1+ BaseAddr;
//		break;
//		case 4:
//			BAR_PIC_ADDR[2]=BAR_NUM[2]*54+540*2+ BaseAddr;
//			BAR_PIC_ADDR[3]=BAR_NUM[3]*54+540*1+ BaseAddr;
//			BAR_PIC_ADDR[4]=BAR_NUM[4]*54+540*1+ BaseAddr;
//			BAR_PIC_ADDR[5]=BAR_NUM[5]*54+540*2+ BaseAddr;
//			BAR_PIC_ADDR[6]=BAR_NUM[6]*54+540*2+ BaseAddr;
//		break;
//		case 5:
//			BAR_PIC_ADDR[2]=BAR_NUM[2]*54+540*2+ BaseAddr;
//			BAR_PIC_ADDR[3]=BAR_NUM[3]*54+540*2+ BaseAddr;
//			BAR_PIC_ADDR[4]=BAR_NUM[4]*54+540*1+ BaseAddr;
//			BAR_PIC_ADDR[5]=BAR_NUM[5]*54+540*1+ BaseAddr;
//			BAR_PIC_ADDR[6]=BAR_NUM[6]*54+540*2+ BaseAddr;
//		break;
//		case 6:
//			BAR_PIC_ADDR[2]=BAR_NUM[2]*54+540*2+ BaseAddr;
//			BAR_PIC_ADDR[3]=BAR_NUM[3]*54+540*2+ BaseAddr;
//			BAR_PIC_ADDR[4]=BAR_NUM[4]*54+540*2+ BaseAddr;
//			BAR_PIC_ADDR[5]=BAR_NUM[5]*54+540*1+ BaseAddr;
//			BAR_PIC_ADDR[6]=BAR_NUM[6]*54+540*1+ BaseAddr;
//		break;
//		case 7:
//			BAR_PIC_ADDR[2]=BAR_NUM[2]*54+540*2+ BaseAddr;
//			BAR_PIC_ADDR[3]=BAR_NUM[3]*54+540*1+ BaseAddr;
//			BAR_PIC_ADDR[4]=BAR_NUM[4]*54+540*2+ BaseAddr;
//			BAR_PIC_ADDR[5]=BAR_NUM[5]*54+540*1+ BaseAddr;
//			BAR_PIC_ADDR[6]=BAR_NUM[6]*54+540*2+ BaseAddr;
//		break;
//		case 8:
//			BAR_PIC_ADDR[2]=BAR_NUM[2]*54+540*2+ BaseAddr;
//			BAR_PIC_ADDR[3]=BAR_NUM[3]*54+540*1+ BaseAddr;
//			BAR_PIC_ADDR[4]=BAR_NUM[4]*54+540*2+ BaseAddr;
//			BAR_PIC_ADDR[5]=BAR_NUM[5]*54+540*2+ BaseAddr;
//			BAR_PIC_ADDR[6]=BAR_NUM[6]*54+540*1+ BaseAddr;
//		break;
//		case 9:
//			BAR_PIC_ADDR[2]=BAR_NUM[2]*54+540*2+ BaseAddr;
//			BAR_PIC_ADDR[3]=BAR_NUM[3]*54+540*2+ BaseAddr;
//			BAR_PIC_ADDR[4]=BAR_NUM[4]*54+540*1+ BaseAddr;
//			BAR_PIC_ADDR[5]=BAR_NUM[5]*54+540*2+ BaseAddr;
//			BAR_PIC_ADDR[6]=BAR_NUM[6]*54+540*1+ BaseAddr;
//		break;
//	}
//	BAR_PIC_ADDR[7]=BAR_NUM[7]*54+540*3+ BaseAddr;
//	for(i=8;i<=11;i++)
//	{
//		BAR_PIC_ADDR[i]=BAR_NUM[i]*54+540*4+ BaseAddr;
//	}
//	BAR_PIC_ADDR[12]=BAR_NUM[12]*54+540*5+ BaseAddr;
//	
//	memcpy(BAR_NUM,BAR_PIC_ADDR,140);	
//	return BAR_PIC_ADDR[0];
//}

///***********************************************************************************************
//GB/T 18347-2001(CODE128)条形码调用程序
//函数：u32 BAR_CODE128(u8 *BAR_NUM)
//功能：将数组条形码转为对应条形码图形地址
//参数：u8 *BAR_NUM 条形码数字数组指针,BAR_NUM[4]数组包含4个条形码ASCII符(数组取值为0~94)。
//返回：定义u32 BAR_PIC_ADDR[7] ；用于存放对应地址，返回数组指针。
//设基地址：BaseAddr=0x3F2ED3;
//起始符有3种模式
//当flag=1时为Code-128-A;
//当flag=2时为Code-128-B;
//当flag=3时为Code-128-C;
//***********************************************************************************************/
//u32 GT32L32_GetBarCode_128(u8	*BAR_NUM,u8	flag)
//{
//  u32 BaseAddr=0x479C7A;
//  u8 i;
//  for(i=0;i<7;i++) 
//	{
//     switch(flag)
//		{  
//		case 1 :
//			 if(i==0)
//			 {
//				 BAR_PIC_ADDR[i]=103*40+BaseAddr;
//			 }
//			 else if(i==1||i==2||i==3||i==4)           
//			 {
//				 BAR_PIC_ADDR[i]=BAR_NUM[i-1]*40+BaseAddr;
//			 }
//			 else if(i==5)
//			 {
//				 BAR_PIC_ADDR[i]=95*40+BaseAddr;
//			 }
//			 else if(i==6)
//			 {
//				 BAR_PIC_ADDR[i]=106*40+BaseAddr;
//			 }
//			 break;
//     
//		case 2 :
//			if(i==0)
//			{
//				BAR_PIC_ADDR[i]=104*40+BaseAddr;
//			}
//			else if(i==1||i==2||i==3||i==4)           
//			{
//				BAR_PIC_ADDR[i]=BAR_NUM[i-1]*40+BaseAddr;
//			}
//			else if(i==5)
//			{
//				BAR_PIC_ADDR[i]=95*40+BaseAddr;
//			}
//			else if(i==6)
//			{
//				BAR_PIC_ADDR[i]=106*40+BaseAddr;
//			}
//			break;
//   	       
//		case 3 :
//			if(i==0)
//			{
//				BAR_PIC_ADDR[i]=105*40+BaseAddr;
//			}
//			else if(i==1||i==2||i==3||i==4)           
//			{
//				BAR_PIC_ADDR[i]=BAR_NUM[i-1]*40+BaseAddr;
//			}
//			else if(i==5)
//			{
//				BAR_PIC_ADDR[i]=95*40+BaseAddr;
//			}
//			else if(i==6)
//			{
//				BAR_PIC_ADDR[i]=106*40+BaseAddr;
//			}
//			break;
//   
//     default:
//     break;
//		}
//	}
//	return BAR_PIC_ADDR[0];
//}

///***********************************
//天线调用程序
//函数：u32 Antenna_CODE_12X12_GetData(u8 NUM)
//功能：获取12X12天线调用地址,取出点阵数据。
//参数：NUM 0123带表天线信号强度。
//返回：数据地址
//***********************************/
//u32 GT32L32_GetAntennaCode_12X12(u8	NUM,u8 *GetBuffer)
//{
//	u32 Address,BaseAdd=0x47AD32;
//	Address=NUM*24+BaseAdd;
//	GT32L32_ReadBuffer(Address,24,GetBuffer);
//	return Address;
//}

///**********************************
//电池调用程序
//函数：u32 Battery_CODE_12X12_GetData(u8 NUM)
//功能：获取12X12电池调用地址,取出点阵数据。
//参数：NUM 0123带表电池电量。
//返回：数据地址
//**********************************/
//u32 GT32L32_GetBatteryCode_12X12(u8	NUM,u8 *GetBuffer)
//{
//	u32 Address, BaseAdd=0x47ADAA;
//	Address=BaseAdd+NUM*24;
//	GT32L32_ReadBuffer(Address,24,GetBuffer);
//	return Address;
//}
///**********************************************************
//UNICODE转GBK码表映射算法,仅1&3字符区 
//函数：WORD U2G(WORD Unicode) 
//功能：将UNICODE码转换为GB码,
//注：仅应用于1区&3区。 
//参数：WORD Unicode,表示输入的UNICODE码。 
//返回：对应的GB码在字库中存放的地址。
//（注:读取对应地址2字节数据即为unicode对应的GB码）。
//***********************************************************/ 

//u16 GT32L32_U2G_13(u16 Unicode) 
//{
//	u16 GB_Code; 
//	u32 i; 
//	u16 UG[]={	0x3000,0x3001,0x3002,0x00b7,0x02c9,0x02c7,0x00a8,
//							0x3003,0x3005,0x2014, 0xff5e,0x2016,0x2026,0x2018,0x2019,0x201c,
//							0x201d,0x3014,0x3015,0x3008, 0x3009,0x300a,0x300b,0x300c,0x300d,
//							0x300e,0x300f,0x3016,0x3017,0x3010, 0x3011,0x00b1,0x00d7,0x00f7,
//							0x2236,0x2227,0x2228,0x2211,0x220f,0x222a, 0x2229,0x2208,0x2237,
//							0x221a,0x22a5,0x2225,0x2220,0x2312,0x2299,0x222b, 0x222e,0x2261,
//							0x224c,0x2248,0x223d,0x221d,0x2260,0x226e,0x226f,0x2264, 0x2265,
//							0x221e,0x2235,0x2234,0x2642,0x2640,0x00b0,0x2032,0x2033,0x2103,
//							0xff04,0x00a4,0xffe0,0xffe1,0x2030,0x00a7,0x2116,0x2606,0x2605,
//							0x25cb, 0x25cf,0x25ce,0x25c7,0x25c6,0x25a1,0x25a0,0x25b3,0x25b2,
//							0x203b,0x2192, 0x2190,0x2191,0x2193,0x3013
//						}; 

//	if(Unicode<=0xffe5&&Unicode>=0xff01)//section 3 
//	{ 
//		if(Unicode==0xff04) 
//			GB_Code=0xa1e7;
//		if(Unicode==0xff5e) 
//			GB_Code=0xa1ab; 
//		if(Unicode==0xffe0) 
//			GB_Code=0xa1e9; 
//		if(Unicode==0xffe1) 
//			GB_Code=0xa1ea; 
//		if(Unicode==0xffe3) 
//			GB_Code=0xa3fe; 
//		if(Unicode==0xffe5) 
//			GB_Code=0xa3a4; 
//		else
//			GB_Code=Unicode-0xff01+0xa3a1; 
//	} 
//	if(Unicode<=0x3013) 
//	{ 
//		for(i=0;i<94;i++) 
//		{ 
//			if(Unicode==UG[i]) 
//			GB_Code=0xa1a1+i; 
//		} 
//	}
//	GBCode[0]=GB_Code>>8;
//	GBCode[1]=GB_Code&0xff;
//	return GB_Code;
//}

///***************************************************************************
//UNICODE转GBK码表映射算法（不含符号区,转换全部双字节区）
//GB_Address转换之后的GB码的存放地址， 
//函数：u32 U2G(u16 Unicode) 
//功能：将UNICODE码转换为GB码, 
//参数：u16 Unicode,表示输入的UNICODE码。 
//返回：对应的GB码在字库中存放的地址。 
//（注:读取对应地址2字节数据即为unicode对应的GB码）。
//转码表起始地址： BaseAdd＝0x3EA90E;
//***************************************************************************/
//u32 GT32L32_UNICODE_To_GBK(u16 unicode,u8 *GetBuffer)
//{
//	u32 BaseAdd=0x46A90E;
//	u32 Address,U_Start_Addr;
//	if(unicode<=0x0451&&unicode>=0x00a0) 
//	{
//		U_Start_Addr	=	0;
//		Address= U_Start_Addr +(unicode-0x00a0)*2; 
//	} 
//	else if(unicode<=0x2642&&unicode>=0x2010) 
//	{ 
//		U_Start_Addr =1892; 
//		Address= U_Start_Addr +(unicode-0x2010)*2; 
//	} 
//	else if(unicode<=0x33d5&&unicode>=0x3000) 
//	{ 
//		U_Start_Addr =5066; 
//		Address= U_Start_Addr +(unicode-0x3000)*2; 
//	} 
//	else if(unicode<=0x9fa5&&unicode>=0x4e00) 
//	{ 
//		U_Start_Addr =7030; 
//		Address= U_Start_Addr +(unicode-0x4e00)*2; 
//	} 
//	else if(unicode<=0xfe6b&&unicode>=0xfe30) 
//	{ 
//		U_Start_Addr =48834; 
//		Address= U_Start_Addr +(unicode-0xfe30)*2; 
//	} 
//	else if(unicode<=0xff5e&&unicode>=0xff01) 
//	{ 
//		U_Start_Addr =48954; 
//		Address= U_Start_Addr +(unicode-0xff01)*2; 
//	} 
//	else if(unicode<=0xffe5&&unicode>=0xffe0) 
//	{ 
//		U_Start_Addr =49142; 
//		Address= U_Start_Addr +(unicode-0xffe0)*2; 
//	} 
//	else if (unicode<=0xFA29&&unicode>=0xF92C) 
//	{ 
//		U_Start_Addr =49312; 
//		Address= U_Start_Addr +(unicode-0xF92C)*2; 
//	} 
//	else if(unicode<=0xE864&&unicode>=0xE816) 
//	{ 
//		U_Start_Addr =49820; 
//		Address= U_Start_Addr +(unicode-0xE816)*2; 
//	}
//	else if(unicode<=0x2ECA&&unicode>=0x2E81) 
//	{ 
//		U_Start_Addr =49978; 
//		Address= U_Start_Addr +(unicode-0x2E81)*2; 
//	} 
//	else if(unicode<=0x49B7&&unicode>=0x4947) 
//	{ 
//		U_Start_Addr =50126; 
//		Address= U_Start_Addr +(unicode-0x4947)*2; 
//	} 
//	else if(unicode<=0x4DAE&&unicode>=0x4C77) 
//	{ 
//		U_Start_Addr =50352; 
//		Address= U_Start_Addr +(unicode-0x4C77)*2; 
//	} 
//	else if(unicode<=0x3CE0&&unicode>=0x3447) 
//	{ 
//		U_Start_Addr =50976; 
//		Address= U_Start_Addr +(unicode-0x3447)*2; 
//	} 
//	else if(unicode<=0x478D&&unicode>=0x4056) 
//	{ 
//		U_Start_Addr =55380; 
//		Address= U_Start_Addr +(unicode-0x4056)*2; 
//	} 
//	Address+=BaseAdd;
//	GT32L32_ReadBuffer(Address,2,GetBuffer);
//	return Address; 
//}



///***********************************************************
//BIG5转GBK转换算法
//函数：u32 GT32L32_BIG5_To_GBK(u16 BIG5_Code) 
//功能：计算BIG5码对应GB码存放的起始地址 
//参数：BIG5_Code 输入BIG5编码双字节 
//返回：对应GB码存放地址，对应GB码高位在前，低位在后。
//************************************************************/
//u32 GT32L32_BIG5_To_GBK(u16 BIG5_Code,u8 *GetBuffer) 
//{
//	u32 Address,part_addr,BaseAddr=0x7F1E10;//转码表基地址; 
//	u8 TMP,BIG5_MSB,BIG5_LSB; 
//	BIG5_MSB= BIG5_Code >>8; 
//	BIG5_LSB= BIG5_Code &0x00ff; 

//	if(BIG5_MSB>=0xa1&&BIG5_MSB<=0xa3)
//	{ 
//		part_addr=0; 
//		TMP=0xa1; 
//	} 
//	else if(BIG5_MSB>=0xa4&&BIG5_MSB<=0xc6) 
//	{ 
//		part_addr=816; 
//		TMP=0xa4; 
//	} 
//	else if(BIG5_MSB>=0xc9&&BIG5_MSB<=0xf9) 
//	{ 
//		part_addr=11618; TMP=0xc9; 
//	}
//	if(BIG5_LSB<=0x7e&&BIG5_LSB>=0x40) 
//	{ 
//		Address=part_addr+((BIG5_MSB-TMP)*157+BIG5_LSB-0x40)*2+BaseAddr; 
//	}
//	else if(BIG5_LSB<=0xfe&&BIG5_LSB>=0xa1) 
//	{ 
//		Address=part_addr+((BIG5_MSB-TMP)*157+BIG5_LSB-0xa1+63)*2+BaseAddr; 
//	}
//	GT32L32_ReadBuffer(Address,2,GetBuffer);
//	
//	return Address; 
//}
////=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
////->函数名			:	GT32L32_ReadStatus
////->功能描述		:	读取芯片状态
////->输入			: Address  	：	表示字符点阵在芯片中的字节地址。
////->						len				：	是读点阵数据字节数。
////->						*Buffer		：	是保存读出的点阵数据的数组。
////->输出			:	读出的字节
////->返回值			:	无
////->例程			:
////->调用函数		:
////->被调用函数	:
////<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
//u8 GT32L32_ReadStatus(void)
//{
//#ifdef	GT32L32_SPIPORT_EN
//	//____________定义变量
//	u8	ChipStatus=0;
//	u8	Address=0x05;	//0X60 OR 0XC7
//	//____________使能片选
//	GPIO_ResetBits(GPIOC,GPIO_Pin_6);
//	GT32L32_CS_Select;
//	//____________发送地址数据	
//	GT32L32_ReadWriteByte(Address);						//发送地址
//	//____________接收数据	
//	
//	ChipStatus=GT32L32_ReadWriteByte(0XFF);// 从字库读出点阵数据到数组中。

//	//____________取消片选	
//	GPIO_SetBits(GPIOC,GPIO_Pin_6);
//	GT32L32_CS_DisSelect;

//#else
//	//____________定义变量
//	u8	ChipStatus=0;
//	u8	Address=0x05;	//0X60 OR 0XC7
//	//____________使能片选
//	GT32L32_CS_Select;
//	//____________发送地址数据	
//	GT32L32_SendByte(Address);
//	//____________接收数据	
//	ChipStatus=GT32L32_ReadByte();	// 从字库读出点阵数据到数组中。
//	//____________取消片选
//	GT32L32_CS_DisSelect;
//#endif
//	return ChipStatus;
//}
////=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
////->函数名			:	r_dat_bat
////->功能描述		:	从字库中读数据函数
////->输入			: Address  	：	表示字符点阵在芯片中的字节地址。
////->						len				：	是读点阵数据字节数。
////->						*Buffer		：	是保存读出的点阵数据的数组。
////->输出			:	读出的字节
////->返回值			:	无
////->例程			:
////->调用函数		:
////->被调用函数	:
////<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
//void GT32L32_ChipErase(void)
//{
//#ifdef	GT32L32_SPIPORT_EN
//	//____________定义变量
//	u32 i=0;
//	u8	len=255;
//	u8	Address=0x60;	//0X60 OR 0XC7
//	//____________使能片选
//	GPIO_ResetBits(GPIOC,GPIO_Pin_6);
//	GT32L32_CS_Select;

//	GT32L32_ReadWriteByte(Address);						//发送地址

//	for(i=0;i<len;i++)
//	{
//	 GT32L32_ReadWriteByte(0XFF);// 从字库读出点阵数据到数组中。
//	}
//	//____________取消片选	
//	GPIO_SetBits(GPIOC,GPIO_Pin_6);
//	GT32L32_CS_DisSelect;

//#else
//	//____________定义变量
//	u32	j=0;
//	u8	len=255;
//	u8	Address=0x60;	//0X60 OR 0XC7
//	//____________使能片选
//	GT32L32_CS_Select;
//	//____________发送地址数据	
//	GT32L32_SendByte(Address);
//	//____________接收数据	
//	for(j=0;j<len;j++)
//	{
//	 GT32L32_ReadByte();	// 从字库读出点阵数据到数组中。
//	}
//	//____________取消片选
//	GT32L32_CS_DisSelect;

//#endif
//}
////=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
////->函数名			:	r_dat_bat
////->功能描述		:	从字库中读数据函数
////->输入			: Address  	：	表示字符点阵在芯片中的字节地址。
////->						len				：	是读点阵数据字节数。
////->						*Buffer		：	是保存读出的点阵数据的数组。
////->输出			:	读出的字节
////->返回值			:	无
////->例程			:
////->调用函数		:
////->被调用函数	:
////<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
//u8 GT32L32_ReadBuffer(u32 Address,u16 len,u8 *Buffer)
//{
//#ifdef	GT32L32_SPIPORT_EN
//	//____________定义变量
//	u32 i=0;
////	u8	Status=0XFF;
//	//____________使能片选
//	GPIO_ResetBits(GPIOC,GPIO_Pin_6);
//	GT32L32_CS_Select;
//	Address=Address|0x03000000;//0x03指令字+地址。
////	Address=Address|0x0B000000;//0x0B指令字+地址。--快速
//	GT32L32_ReadWriteByte(Address>>24);				//发送地址高8位
//	GT32L32_ReadWriteByte(Address>>16);				//发送地址
//	GT32L32_ReadWriteByte(Address>>8);				//发送地址
//	GT32L32_ReadWriteByte(Address);						//发送地址
//	if((Address&0x0B000000)==0x0B000000)
//		GT32L32_ReadWriteByte(0XFF);							//从字库读出点阵数据到数组中。
////	while((Status=GT32L32_ReadStatus()&0x01)!=0x01);
//	i=0x0F;
//	while(i--);
//	for(i=0;i<len;i++)
//	{
//	 Buffer[i]=GT32L32_ReadWriteByte(0XFF);// 从字库读出点阵数据到数组中。
//	}
//	//____________取消片选	
//	GPIO_SetBits(GPIOC,GPIO_Pin_6);
//	GT32L32_CS_DisSelect;
//	return Buffer[0];	
//#else
//	//____________定义变量
//	u32	j=0;
//	//____________使能片选
//	GT32L32_CS_Select;
//	Address=Address|0x03000000;//0x03指令字+地址。
////	Address=Address|0x0B000000;//0x0B指令字+地址。--快速
//	//____________发送地址数据	
//	GT32L32_SendByte((Address>>24)	&	0xFF);
//	GT32L32_SendByte((Address>>16)	&	0xFF);
//	GT32L32_SendByte((Address>>8)		&	0xFF);
//	GT32L32_SendByte(Address				&	0xFF);
//	if((Address&0x0B000000)==0x0B000000)
//		GT32L32_SendByte(0XFF);							//从字库读出点阵数据到数组中。
//	//____________接收数据	
//	for(j=0;j<len;j++)
//	{
//	 Buffer[j]=GT32L32_ReadByte();	// 从字库读出点阵数据到数组中。
//	}
//	//____________取消片选
//	GT32L32_CS_DisSelect;
//	return Buffer[0];
//#endif
//}

////=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
////->函数名			:	GT32_ReadWriteByte
////->功能描述		:	读写一个字节
////->输入			: 写入一个字节
////->输出			:	读出的字节
////->返回值			:	无
////->例程			:
////->调用函数		:
////->被调用函数	:
////<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
//u8	GT32L32_ReadWriteByte(u8 Data)
//{
//#ifdef	GT32L32_SPIPORT_EN		//使用SPI接口
//	//____________定义变量
//	u16 retry=0XFFFF;													//用来进行超时计数
//	//____________等待发送缓冲区为空
//	while (SPI_I2S_GetFlagStatus(GT32L32_SPI_PORT, SPI_I2S_FLAG_BSY) == SET);	
////	while(SPI_I2S_GetFlagStatus(GT32L32_SPI_PORT, SPI_I2S_FLAG_TXE) == RESET); 		//检查指令SPI发送标志是否为空
////	{
////		retry++;
////		if(retry>2000)
////			return 0;
////	}	
//	//____________发送数据
//	SPI_I2S_SendData(GT32L32_SPI_PORT, Data);				//发送数据
//	//____________等待接收数据
////	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
//	retry=0;	
////	while(SPI_I2S_GetFlagStatus(GT32L32_SPI_PORT, SPI_I2S_FLAG_RXNE) == RESET)		//检查指令SPI接收完成标志设置与否
////	{
////		retry++;
////		if(retry>8000)
////			return 0;
////	}
//	while (SPI_I2S_GetFlagStatus(GT32L32_SPI_PORT, SPI_I2S_FLAG_BSY) == SET);
//	//____________返回接收到的数据
//	return SPI_I2S_ReceiveData(GT32L32_SPI_PORT); 			//返回接收到的数据	
//#else					//使用自定义协议
//	u8	dat=0;
//	dat=GT32L32_ReadByte();	
//	return dat;
//#endif
//}

////=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
////->函数名			:	GT32L32_SendByte
////->功能描述		:	发送一个字节数据,先发送高位
////->输入			: 
////->输出			:	
////->返回值			:	
////->例程			:
////->调用函数		:
////->被调用函数	:
////<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
//void GT32L32_SendByte(u8 ByteData)
//{
//	u8 i;
//	for(i=0;i<8;i++)
//	{
//		GT32L32_CLK_L;
//		if(ByteData&0x80)
//			GT32L32_SI_H;
//		else 
//			GT32L32_SI_L;
//		GT32L32_CLK_H;
//		ByteData=ByteData<<1;
//	}					
//}
////=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
////->函数名			:	ReadByte
////->功能描述		:	读一个字节,先读高位
////->输入			: 	
////->输出			:		
////->返回值			:		
////->例程			:
////->调用函数		:
////->被调用函数	:
////<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
//u8 GT32L32_ReadByte(void)
//{
//	u8 i;
//	u8 dat=0;
//	GT32L32_CLK_H;
//	for(i=0;i<8;i++)
//	{
//		GT32L32_CLK_L;
//		dat=dat<<1;
//		if(GT32L32_SO)
//			dat=dat|0x01;
//		else 
//			dat&=0xfe;
//		GT32L32_CLK_H	;		
//	}	
//	return dat;
//}


///*******************	wegam@sina.com	*******************/
///*********************	2017/01/21	*********************/
///**********************	END OF FILE	*********************/



