#include "CS5530.H"

#include "STM32_GPIO.H"

#include <stdint.h>


//#define CS5530_CS_LOW()			((pInfo->pGPIOxCS)->BRR = pInfo->pinCS)
//#define CS5530_CS_HIGH()		((pInfo->pGPIOxCS)->BSRR = pInfo->pinCS)
//#define CS5530_SDI_LOW()		((g_CS5530PinInfo->pGPIOxSDI)->BRR = g_CS5530PinInfo->pinSDI)
//#define CS5530_SDI_HIGH()		((g_CS5530PinInfo->pGPIOxSDI)->BSRR = g_CS5530PinInfo->pinSDI)
//#define CS5530_SDO_STATE()	((g_CS5530PinInfo->pGPIOxSDO)->IDR & g_CS5530PinInfo->pinSDO)
//#define CS5530_SCLK_LOW()		((g_CS5530PinInfo->pGPIOxSCLK)->BRR = g_CS5530PinInfo->pinSCLK)
//#define CS5530_SCLK_HIGH()	((g_CS5530PinInfo->pGPIOxSCLK)->BSRR = g_CS5530PinInfo->pinSCLK)
unsigned long CS5530_Status=0x00;



void CS5530_Delay(u32 time);
void CS5530_CS_LOW(CS5530_Pindef *Pinfo);
void CS5530_CS_HIGH(CS5530_Pindef *Pinfo);
void CS5530_SDI_LOW(CS5530_Pindef *Pinfo);
void CS5530_SDI_HIGH(CS5530_Pindef *Pinfo);
void CS5530_SCLK_LOW(CS5530_Pindef *Pinfo);
void CS5530_SCLK_HIGH(CS5530_Pindef *Pinfo);
u8 CS5530_SDO_STATE(CS5530_Pindef *Pinfo);

void CS5530_WriteOneByte(CS5530_Pindef *Pinfo, u8 dat);
void CS5530_WriteRegister(CS5530_Pindef *Pinfo, u8 command,u32 setData);
//void CS5530_WriteCommand(CS5530_Pindef *Pinfo,u8 command);

unsigned char CS5530_ReadOneByte(CS5530_Pindef *Pinfo);
u32 CS5530_GetADData(CS5530_Pindef *Pinfo);
u32 CS5530_ReadRegister(CS5530_Pindef *Pinfo, u8 command);
void CS5530_ClearBuf(CS5530_Pindef *Pinfo);


void CS5530_PowerUp(CS5530_Pindef *Pinfo);		//CS5530上电及初始化



/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void CS5530_Delay(u32 time)
{
	while(time--);
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	称重板
* 输入			: void
* 返回值			: void
*******************************************************************************/
void CS5530_CS_LOW(CS5530_Pindef *Pinfo)
{	
	GPIO_ResetBits(Pinfo->CS5530_CS_PORT, Pinfo->CS5530_CS_Pin);
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	称重板
* 输入			: void
* 返回值			: void
*******************************************************************************/
void CS5530_CS_HIGH(CS5530_Pindef *Pinfo)
{
	GPIO_SetBits(Pinfo->CS5530_CS_PORT, Pinfo->CS5530_CS_Pin);
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	称重板
* 输入			: void
* 返回值			: void
*******************************************************************************/
void CS5530_SDI_LOW(CS5530_Pindef *Pinfo)
{
	GPIO_ResetBits(Pinfo->CS5530_SDI_PORT, Pinfo->CS5530_SDI_Pin);
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	称重板
* 输入			: void
* 返回值			: void
*******************************************************************************/
void CS5530_SDI_HIGH(CS5530_Pindef *Pinfo)
{
	GPIO_SetBits(Pinfo->CS5530_SDI_PORT, Pinfo->CS5530_SDI_Pin);
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	称重板
* 输入			: void
* 返回值			: void
*******************************************************************************/
void CS5530_SCLK_LOW(CS5530_Pindef *Pinfo)
{
	GPIO_ResetBits(Pinfo->CS5530_SCLK_PORT, Pinfo->CS5530_SCLK_Pin);
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	称重板
* 输入			: void
* 返回值			: void
*******************************************************************************/
void CS5530_SCLK_HIGH(CS5530_Pindef *Pinfo)
{
	GPIO_SetBits(Pinfo->CS5530_SCLK_PORT, Pinfo->CS5530_SCLK_Pin);
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	称重板
* 输入			: void
* 返回值			: void
*******************************************************************************/
u8 CS5530_SDO_STATE(CS5530_Pindef *Pinfo)
{
	return(GPIO_ReadInputDataBit(Pinfo->CS5530_SDO_PORT, Pinfo->CS5530_SDO_Pin));
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void CS5530_PinConf(CS5530_Pindef *Pinfo)
{
	GPIO_Configuration_OPP50	(Pinfo->CS5530_CS_PORT,		Pinfo->CS5530_CS_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->CS5530_SDI_PORT,	Pinfo->CS5530_SDI_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->CS5530_SCLK_PORT,	Pinfo->CS5530_SCLK_Pin);		//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	GPIO_Configuration_IPU	(Pinfo->CS5530_SDO_PORT,	Pinfo->CS5530_SDO_Pin);			//将GPIO相应管脚配置为上拉输入模式----V20170605
	
	CS5530_PowerUp(Pinfo);		//CS5530上电及初始化
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	称重板
* 输入			: void
* 返回值			: void
*******************************************************************************/
void CS5530_WriteOneByte(CS5530_Pindef *Pinfo, uint8_t dat)
{
	uint8_t i;

	for(i=0; i<8; i++)
	{
		if((dat&0x80) != 0)
			CS5530_SDI_HIGH(Pinfo);
		else
			CS5530_SDI_LOW(Pinfo);
		__nop();
		__nop();
		__nop();
		__nop();
//		CS5530_Delay(1);
		CS5530_SCLK_HIGH(Pinfo);
		__nop();
		__nop();
		__nop();
		__nop();
//		CS5530_Delay(1);
		CS5530_SCLK_LOW(Pinfo);
		__nop();
		__nop();
//		__nop();
//		__nop();
//		CS5530_Delay(1);
		dat <<= 1;
	}
}

/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void CS5530_WriteCommand(CS5530_Pindef *Pinfo,u8 command)
{
	CS5530_WriteOneByte(Pinfo,command);
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void CS5530_WriteRegister(CS5530_Pindef *Pinfo, u8 command,u32 setData)
{
//	unsigned long temp=0xFFFFFFFF;
	CS5530_WriteOneByte(Pinfo, command);
	CS5530_WriteOneByte(Pinfo, (uint8_t)(setData>>24));
	CS5530_WriteOneByte(Pinfo, (uint8_t)(setData>>16));
	CS5530_WriteOneByte(Pinfo, (uint8_t)(setData>>8));
	CS5530_WriteOneByte(Pinfo, (uint8_t)(setData>>0));
}

/*******************************************************************************
* 函数名			:	function
* 功能描述		:	称重板
* 输入			: void
* 返回值			: void
*******************************************************************************/
unsigned char CS5530_ReadOneByte(CS5530_Pindef *Pinfo)
{
	uint8_t i;
	uint8_t reValue=0;

	CS5530_SDI_LOW(Pinfo);
	for(i=0; i<8; i++)
	{
		__nop();
		__nop();
		//Delay_us(100);
		CS5530_SCLK_HIGH(Pinfo);
		__nop();
		__nop();
		__nop();
		__nop();
		//Delay_us(100);
		reValue <<= 1;
		if(CS5530_SDO_STATE(Pinfo))
			reValue++;
		CS5530_SCLK_LOW(Pinfo);
		__nop();
		__nop();
	}

	return reValue;
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
u32 CS5530_GetADData(CS5530_Pindef *Pinfo)
{
	u32 reValue=0;
	
	CS5530_CS_LOW(Pinfo);
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	//Delay_us(100);
	if(CS5530_SDO_STATE(Pinfo) == 0)
	{
		CS5530_WriteOneByte(Pinfo, CS5530_CONTINUOUS_ON);		///*继续连续转换模式*/
		reValue = ((u32)CS5530_ReadOneByte(Pinfo)<<24)
				+((uint32_t)CS5530_ReadOneByte(Pinfo)<<16)
				+((uint32_t)CS5530_ReadOneByte(Pinfo)<<8)
				+(uint32_t)CS5530_ReadOneByte(Pinfo);
	}
	else
		reValue = 0xFFFFFFFF;
	__nop();
	__nop();
	__nop();
	__nop();
	//Delay_us(100);
	CS5530_CS_HIGH(Pinfo);

	return reValue;
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
u32 CS5530_ReadRegister(CS5530_Pindef *Pinfo, u8 command)
{
	volatile unsigned long temp=0x00;
	
	CS5530_WriteOneByte(Pinfo, command);
	temp = (CS5530_ReadOneByte(Pinfo)<<24)
			+(CS5530_ReadOneByte(Pinfo)<<16)
			+(CS5530_ReadOneByte(Pinfo)<<8)
			+CS5530_ReadOneByte(Pinfo);
	
	return temp;
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void CS5530_ClearBuf(CS5530_Pindef *Pinfo)
{
//	uint8_t i;

//	__disable_irq();
//	pInfo->continuousOFCount = 0;
//	for(i=0; i<pInfo->dataBufSize; i++)
//		pInfo->pDataBuf[i] = 0xFFFFFFFF;
//	pInfo->dataPtr = 0;
//	__enable_irq();
}
/*******************************************************************************
* 函数名			:	CS5530_PowerUp
* 功能描述		:	CS5530上电及初始化
* 输入			: void
* 返回值			: void
*******************************************************************************/
void CS5530_PowerUp(CS5530_Pindef *Pinfo)
{
	u8 num=0;
	CS5530_CS_LOW(Pinfo);
	CS5530_Delay(50000);CS5530_Delay(50000);CS5530_Delay(50000);CS5530_Delay(50000);CS5530_Delay(50000);CS5530_Delay(50000);
	//1)向系统所有ADC发送复位序列
	//1.1、************写入SYNC1
	for(num=0;num<15;num++)
	CS5530_WriteOneByte(Pinfo, CS5530_SYNC1);
	CS5530_Delay(50);
	//1.2、************写入SYNC0
	CS5530_WriteOneByte(Pinfo, CS5530_SYNC0);
	CS5530_Delay(50);
	//2)写配置寄存器 写入CS5530复位命令 RS为1
	CS5530_WriteRegister(Pinfo,CS5530_WRITE_CONFIG,CS5530_CONF_SYSTEM_RESET);
	CS5530_Delay(500);
	CS5530_WriteOneByte(Pinfo, CS5530_NULL_BYTE);
	CS5530_Delay(50);
	CS5530_WriteRegister(Pinfo,CS5530_WRITE_CONFIG,CS5530_CONF_NORMAL_MODE);
	CS5530_Delay(50);
	
	//检测RV是否为1(复位成 功后为1),如果不为1再继续读取配置寄存器		
		CS5530_Delay(10);
		do
		{ 
			CS5530_Status = CS5530_ReadRegister(Pinfo, CS5530_READ_CONFIG);
		}
		while((CS5530_Status & CS5530_CONF_RESET_STATUS) != 0);
	
//	CS5530_Delay(50);		
//	CS5530_WriteRegister(Pinfo,CS5530_WRITE_CONFIG,CS5530_CONF_NORMAL_MODE);
//	CS5530_Delay(50);
	
//	while(CS5530_ReadRegister(Pinfo,CS5530_READ_CONFIG)==0x00000000);
//	while(CS5530_ReadRegister(Pinfo,CS5530_READ_OFFSET)==0x00000000);
//	while(CS5530_ReadRegister(Pinfo,CS5530_READ_GAIN)==0x01000000);
		
//	CS5530_Status=0xFFFFFFFF;
	CS5530_Status=CS5530_ReadRegister(Pinfo,CS5530_READ_CONFIG);			//配置寄存器
//	CS5530_Status=0xFFFFFFFF;
	CS5530_Status=CS5530_ReadRegister(Pinfo,CS5530_READ_OFFSET);			//偏移寄存器
//	CS5530_Status=0xFFFFFFFF;
	CS5530_Status=CS5530_ReadRegister(Pinfo,CS5530_READ_GAIN);				//增益寄存器
		
		
//	CS5530_Status=0xFFFFFFFF;
		
	CS5530_Status=CS5530_Status|CS5530_CONF_SHORT_INPUTS;								//设置为输入短路

	CS5530_Status=CS5530_ReadRegister(Pinfo,CS5530_READ_CONFIG);				//增益寄存器

	CS5530_Status=CS5530_Status|0x2000;																	//7.5sps
	
//	CS5530_Status=CS5530_Status|0x1800;																	//15sps
	
//	CS5530_Status=CS5530_Status|0x0800;																	//60sps
	
////	CS5530_Status=CS5530_Status|0x4000;																	//3840sps
	
//	CS5530_Status=CS5530_Status|0x6000;																	//240sps
	
	CS5530_WriteCommand(Pinfo,CS5530_SYSTEM_OFFSET_CAL);								//执行偏移校准
	
	CS5530_WriteCommand(Pinfo,CS5530_SYSTEM_GAIN_CAL);									//执行系统增益校准	
	
	CS5530_Status=CS5530_Status|CS5530_CONF_UNIPOLAR_MODE;							//极性选择单极性模式
	
	CS5530_WriteRegister(Pinfo,CS5530_WRITE_CONFIG,CS5530_Status);			//配置寄存器
	
	CS5530_Status=CS5530_ReadRegister(Pinfo,CS5530_READ_CONFIG);				//配置寄存器
	
	CS5530_Status=CS5530_Status&(~CS5530_CONF_SHORT_INPUTS);						//正常输入,通道输入不短接
	
	CS5530_WriteRegister(Pinfo,CS5530_WRITE_CONFIG,CS5530_Status);			//写配置寄存器
	
	CS5530_WriteRegister(Pinfo,CS5530_WRITE_GAIN,0x00FFFFFF);						//写增益寄存器
	
//	CS5530_WriteRegister(Pinfo,CS5530_WRITE_GAIN,0x00100000);					//写增益寄存器
	
	CS5530_WriteCommand(Pinfo,CS5530_START_CONTINUOUS);									//执行连续转换
	
	CS5530_Delay(500);
	CS5530_CS_HIGH(Pinfo);
}
/*******************************************************************************
*函数名			:	CS5530_ReadData
*功能描述		:	读取AD值，如果返回0xFFFFFFFF,则未读取到24位AD值
*输入				: 
*返回值			:	无
*******************************************************************************/
u32	CS5530_ReadData(CS5530_Pindef *Pinfo)
{
	u32 ADC_Value=0xFFFFFFFF;
	
//	CS5530_CS_HIGH(Pinfo);
	CS5530_CS_LOW(Pinfo);		
	if(CS5530_SDO_STATE(Pinfo) == 0)
	{			
		ADC_Value=CS5530_GetADData(Pinfo)>>8;
	}
	return ADC_Value;

}







/************************************************************************************/
