#ifdef MX25L4006E

#include "MX25L4006E.H"




/*##############################################################################
################################################################################

********************************************************************************

################################################################################
##############################################################################*/


/******************************MX25L4006E命令定义*******************************/
#define	MX25L4006E_WREN		(unsigned char)0x06				//写使能
#define	MX25L4006E_WRDI		(unsigned char)0x04				//写失能
#define	MX25L4006E_WRSR		(unsigned char)0x01				//写状态寄存器
#define	MX25L4006E_RDID		(unsigned char)0x9F				//读设备信息，3byte；第一字节为制造商代码，第二第三字节为芯片代码
#define	MX25L4006E_RDSR		(unsigned char)0x05				//读状态寄存器
#define	MX25L4006E_READ		(unsigned char)0x06				//读数据
#define	MX25L4006E_FREAD	(unsigned char)0x06				//快速读取数据
#define	MX25L4006E_RDSFDP	(unsigned char)0x5A				//
#define	MX25L4006E_RES		(unsigned char)0xAB				//读器件地址
#define	MX25L4006E_REMS		(unsigned char)0x90				//读制造商代码和产品代码
#define	MX25L4006E_DREAD	(unsigned char)0x3B				//
#define	MX25L4006E_SE			(unsigned char)0x20				//扇区擦除
#define	MX25L4006E_BE			(unsigned char)0x52				//块擦除		也可以用0XD8
#define	MX25L4006E_CE			(unsigned char)0x60				//芯片擦除; 整片擦除; 也可以用0XC7
#define	MX25L4006E_PP			(unsigned char)0x02				//写页面
#define	MX25L4006E_DP			(unsigned char)0xB9				//深度省电状态
#define	MX25L4006E_RDP		(unsigned char)0xAB				//退出深度掉电

		



//SPI_ConfTypeDef *SPI4;

//#define	SPI4_NSSP GPIO_Pin_4


#define	SPI_BUFFERSIZE	10

char SPI_TX_Buffer[SPI_BUFFERSIZE]={0X90,0X0F,0X0A,0X00};
char SPI_RX_Buffer[SPI_BUFFERSIZE]={0};

SPI_InitTypeDef  SPI_InitStructure;


vu32 FLASH_ID = 0;

u32 Temp = 0,Temp0=0,Temp1=0,Temp2=0;		//测试

/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void MX25L4006E_Configuration(void)
{
	SPI_StructConf(&SPI_InitStructure);										//配置参数
	
//	SPI_BASIC_Configuration(SPI1,&SPI_InitStructure);			//SPI常规配置
	
	SPI_DMA_Configuration(SPI1,&SPI_InitStructure,(u32*)SPI_TX_Buffer,(u32*)SPI_RX_Buffer,SPI_BUFFERSIZE);				//SPI_DMA配置
	
	PWM_OUT(TIM2,PWM_OUTChannel2,1,500);	//PWM设定-20161127版本
	
//	SPIT_Configuration(SPI1);							//SPI常规使用方式配置
//	SPI_Cmd(SPI1, ENABLE);
//	SPI_I2S_ReceiveData(SPI1);


}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void MX25L4006E_Server(void)
{
//	if(Temp==0)
//		MX25L4006E_ReadID();
//	SPI_DMA_BufferRead((u32*)SPI_RX_Buffer);
//	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
//	SPI_DMASend(SPI1,(u32*)SPI_TX_Buffer,(u32*)SPI_RX_Buffer,SPI_BUFFERSIZE);	//SPI_DMA发送函数----后边的省略号就是可变参数
//	GPIO_SetBits(GPIOA,GPIO_Pin_4);	
//	SPI_Cmd(SPI1, ENABLE);
	SPI_DMA_ReceiveSendByte(SPI1,8);		//DMA收发数据
//	SPI_Cmd(SPI1, DISABLE);
}
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
	SPI_InitStruct->SPI_DataSize=SPI_DataSize_8b;												//宽度         	（8或16位）
	SPI_InitStruct->SPI_CPOL=SPI_CPOL_High;															//时钟极性     	（低或高）
	SPI_InitStruct->SPI_CPHA=SPI_CPHA_2Edge;														//时钟相位     	（第一个或第二个跳变沿）
//	SPI_InitStruct->SPI_NSS=SPI_NSS_Soft;																//片选方式     	（硬件或软件方式）
	SPI_InitStruct->SPI_NSS=SPI_NSS_Hard;																//片选方式     	（硬件或软件方式）
	SPI_InitStruct->SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_32;			//波特率预分频 	（从2---256分频）
	SPI_InitStruct->SPI_FirstBit=SPI_FirstBit_MSB;											//最先发送的位 	（最低位，还是最高位在先）
	SPI_InitStruct->SPI_CRCPolynomial=0X07;															//设置crc多项式	（其复位值为0x0007，根据应用可以设置其他数值。）
}
/*******************************************************************************
*函数名			:	MX25L4006E_WriteEnable
*功能描述		:	写使能
*输入				: 
*输出				:	无
*返回值			:	无
*例程				:
*调用函数		:
*被调用函数	:
*******************************************************************************/
void MX25L4006E_WriteEnable(void)		//MX25L4006E写使能命令
{
	SPI_Cmd(SPI1, ENABLE);
	SPI_I2S_SendData(SPI1,MX25L4006E_WREN);
	SPI_Cmd(SPI1, DISABLE);
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	等待命令执行结束
							执行写使能会值位状态寄存器中的BUSY位，只有BUSY位变为0时说明写使能命令执行完成
*输入				: 
*输出				:	无
*返回值			:	无
*例程				:
*调用函数		:
*被调用函数	:
*******************************************************************************/
void MX25L4006E_WaitForWriteEnd(void)		//等待命令执行结束
{
	u8  FLASH_Status = 0;
  //1)**********使能芯片
	SPI_Cmd(SPI1, ENABLE);
  //2)**********发送读取状态寄存器命令
	SPI_SendByte(0X05);
  //3)**********读取状态寄存器直到完成
	do
	{
    //3.0)**********发送任意（虚拟）数据，读取状态
		FLASH_Status = SPI_SendByte(0x00);
	}
  while ((FLASH_Status & 0X01) == SET); 
	SPI_Cmd(SPI1, DISABLE);
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	扇区擦除//在写入数据前先要对芯片进行擦除。
*输入				: 
*输出				:	无
*返回值			:	无
*例程				:
*调用函数		:
*被调用函数	:
*******************************************************************************/
void MX25L4006E_SectorErase(void)		//扇区擦除
{
	//1)**********发送写使能命令
	MX25L4006E_WriteEnable();				//MX25L4006E写使能命令
	//2)**********使能写命令是一个写状态寄存器的命令，所以要等待该命令执行完，才能写其他命令
	MX25L4006E_WaitForWriteEnd();		//等待命令执行结束
	//3)**********擦除扇区
	//3.1)**********发送写使能命令
	SPI_Cmd(SPI1, ENABLE);
	//3.2)**********发送擦除扇区命令
	SPI_SendByte(0X20);
	//3.3)**********发送要擦除的起始地址

}

/*******************************************************************************
*函数名		:	function
*功能描述	:	函数功能说明
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void MX25L4006E_COMMAND(unsigned char Command)
{

}

/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
u32 MX25L4006E_ReadID(void)
{
//	u32 Temp = 0,Temp0=0,Temp1=0,Temp2=0;

  /* Select the FLASH: Chip Select low */
//  SPI_FLASH_CS_LOW();
//	GPIO_ResetBits(GPIOA,GPIO_Pin_4);

	SPI_Cmd(SPI1, ENABLE);
  /* Send "RDID " instruction */
  SPI_SendByte(0x9F);
//	SPI_SendByte(0X00);
//	SPI_SendByte(0X00);
//	SPI_SendByte(0X00);
  /* Read a byte from the FLASH */
  Temp0 = SPI_SendByte(0X01);

  /* Read a byte from the FLASH */
  Temp1 = SPI_SendByte(0X01);

  /* Read a byte from the FLASH */
  Temp2 = SPI_SendByte(0X01);

  /* Deselect the FLASH: Chip Select high */
//  SPI_FLASH_CS_HIGH();
	GPIO_SetBits(GPIOA,GPIO_Pin_4);

  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
	SPI_Cmd(SPI1, DISABLE);
  return Temp;	
}

#endif


