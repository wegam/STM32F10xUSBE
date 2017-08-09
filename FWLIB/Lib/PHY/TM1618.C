#include "TM1618.H"
#include "STM32_GPIO.H"
//#include "STM32F10x_BitBand.H"

#define	TM1618_DelayTime	1

unsigned char TM1618_Daddr[]={0XC0,0XC2,0XC4,0XC6,0XC8,0XCA,0XCC,0XCE};			//显示地址
unsigned char TM1618_SegCode[]={0x3F,0X06,0X5B,0X4F,0x66,0X6D,0X7D,0x07,0x7f,0x6f,0x00,0xff};
unsigned char TM1618_CovSeg[8]={0};			//共阳显示时转换后显示代码
//unsigned char d[8]={0};


void TM1618_Delay(u16 time);
void TM1618_WriteByte(TM1618_Pindef *Pinfo,unsigned char byted);
void TM1618_WriteCommand(TM1618_Pindef *Pinfo,unsigned char command);
void TM1618_WriteCommandN(TM1618_Pindef *Pinfo,unsigned char command);
void TM1618_DataCov(u16 number);			//共阳显示数据转换

/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void TM1618_Delay(u16 time)
{
	while(time--);
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void TM1618_PinConf(TM1618_Pindef *Pinfo)
{
	GPIO_Configuration_OPP50	(Pinfo->TM1618_DIO_PORT,		Pinfo->TM1618_DIO_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->TM1618_CLK_PORT,		Pinfo->TM1618_CLK_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->TM1618_STB_PORT,		Pinfo->TM1618_STB_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void TM1618_WriteByte(TM1618_Pindef *Pinfo,unsigned char byted)
{
	unsigned char i=0;
	for(i=0;i<8;i++)
	{
		GPIO_ResetBits(Pinfo->TM1618_CLK_PORT, Pinfo->TM1618_CLK_Pin);
		TM1618_Delay(TM1618_DelayTime);
		if((byted&0x01)==0x01)						 //确认最低位数，从低位开始写入
		{
			GPIO_SetBits(Pinfo->TM1618_DIO_PORT, Pinfo->TM1618_DIO_Pin);
		}
		else
		{
			GPIO_ResetBits(Pinfo->TM1618_DIO_PORT, Pinfo->TM1618_DIO_Pin);
		}
		TM1618_Delay(TM1618_DelayTime);
		GPIO_SetBits(Pinfo->TM1618_CLK_PORT, Pinfo->TM1618_CLK_Pin);
		TM1618_Delay(TM1618_DelayTime);
		byted=byted>>1;							 //右移一位		
	}
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void TM1618_WriteCommand(TM1618_Pindef *Pinfo,unsigned char command)
{
//	stb=1;
	GPIO_ResetBits(Pinfo->TM1618_STB_PORT, Pinfo->TM1618_STB_Pin);			//stb=0;
	TM1618_Delay(TM1618_DelayTime);
	TM1618_WriteByte(Pinfo,0x44);	//固定地址模式
	TM1618_Delay(TM1618_DelayTime);
	GPIO_SetBits(Pinfo->TM1618_STB_PORT, Pinfo->TM1618_STB_Pin);		//stb=1;
	TM1618_Delay(TM1618_DelayTime);
	
	GPIO_ResetBits(Pinfo->TM1618_STB_PORT, Pinfo->TM1618_STB_Pin);			//stb=0;
	TM1618_Delay(TM1618_DelayTime);
	TM1618_WriteByte(Pinfo,command);				//脉冲宽度设置 0x80（关）<0x88<0x89<0x8a<0x8b<0x8c<0x8d<0x8e<0x8f
	TM1618_Delay(TM1618_DelayTime);
	GPIO_SetBits(Pinfo->TM1618_STB_PORT, Pinfo->TM1618_STB_Pin);		//stb=1;
	TM1618_Delay(TM1618_DelayTime);
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void TM1618_WriteCommandN(TM1618_Pindef *Pinfo,unsigned char command)
{
//	stb=1;
	GPIO_SetBits(Pinfo->TM1618_STB_PORT, Pinfo->TM1618_STB_Pin);			//stb=0;
	TM1618_Delay(TM1618_DelayTime);
	TM1618_WriteByte(Pinfo,0x44);																			//固定地址模式
	TM1618_Delay(TM1618_DelayTime);
	GPIO_ResetBits(Pinfo->TM1618_STB_PORT, Pinfo->TM1618_STB_Pin);		//stb=1;
	TM1618_Delay(TM1618_DelayTime);
	
	GPIO_SetBits(Pinfo->TM1618_STB_PORT, Pinfo->TM1618_STB_Pin);			//stb=0;
	TM1618_Delay(TM1618_DelayTime);
	TM1618_WriteByte(Pinfo,command);				//脉冲宽度设置(亮度) 0x80（关）<0x88<0x89<0x8a<0x8b<0x8c<0x8d<0x8e<0x8f
	TM1618_Delay(TM1618_DelayTime);
	GPIO_ResetBits(Pinfo->TM1618_STB_PORT, Pinfo->TM1618_STB_Pin);		//stb=1;
	TM1618_Delay(TM1618_DelayTime);
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void TM1618_WriteData(TM1618_Pindef *Pinfo,u16 Data)
{
	unsigned char i=0;
	TM1618_WriteCommand(Pinfo,0X88);			//亮度
	TM1618_Delay(TM1618_DelayTime);
	for(i=0;i<8;i++)
	{
		GPIO_ResetBits(Pinfo->TM1618_STB_PORT, Pinfo->TM1618_STB_Pin);	//stb=0;
		TM1618_Delay(TM1618_DelayTime);
		TM1618_WriteByte(Pinfo,Data);				//写入地址
		TM1618_WriteByte(Pinfo,Data);				//写入数据
		TM1618_Delay(TM1618_DelayTime);
		GPIO_SetBits(Pinfo->TM1618_STB_PORT, Pinfo->TM1618_STB_Pin);		//stb=1;
	}
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void TM1618_WriteDataN(TM1618_Pindef *Pinfo,u16 Data)
{
	unsigned char i=0;
	TM1618_DataCov(Data);
	TM1618_WriteCommandN(Pinfo,0X8A);			//亮度
	TM1618_Delay(TM1618_DelayTime);
	for(i=0;i<8;i++)
	{
		GPIO_SetBits(Pinfo->TM1618_STB_PORT, Pinfo->TM1618_STB_Pin);			//stb=0;
		TM1618_WriteByte(Pinfo,TM1618_Daddr[i]);				//写入地址
		TM1618_Delay(TM1618_DelayTime);
		TM1618_WriteByte(Pinfo,TM1618_CovSeg[i]);				//写入数据
		TM1618_Delay(TM1618_DelayTime);
		GPIO_ResetBits(Pinfo->TM1618_STB_PORT, Pinfo->TM1618_STB_Pin);		//stb=1;
	}
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	共阳显示数据转换
*输入				: 
*返回值			:	无
*******************************************************************************/
void TM1618_DataCov(u16 number)
{
	unsigned char s[4];
	unsigned char	i,j;
	s[0]=TM1618_SegCode[number/1000];
	s[1]=TM1618_SegCode[number%1000/100];
	s[2]=TM1618_SegCode[number%100/10];
	s[3]=TM1618_SegCode[number%10];
	
//	if(number<10)
//	{
//		s[0]=TM1618_SegCode[10];
//		s[1]=TM1618_SegCode[10];
//		s[2]=TM1618_SegCode[10];
//	}
//	else if(100>number&&number>9)
//	{
//		s[0]=TM1618_SegCode[10];
//		s[1]=TM1618_SegCode[10];
//	}
//	else if(1000>number&&number>99)
//		s[0]=TM1618_SegCode[10];
	
	
	
	for(i=0;i<8;i++)
	{
		for(j=0;j<4;j++)
		{
			TM1618_CovSeg[i]<<=1;
			if(s[j]&0x01)
			TM1618_CovSeg[i]|=0x01;
			s[j]>>=1;
			
		}
	}

}



