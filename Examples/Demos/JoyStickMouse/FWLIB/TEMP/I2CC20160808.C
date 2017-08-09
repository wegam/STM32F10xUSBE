
#include "I2CH20160808.H"
//#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	

////////////////////////////////////////////////////////////////////////////////// 	
//#define IIC_SCL	GPIO_Pin_10
//#define IIC_SDA	GPIO_Pin_11
//#define GPIO_SCL	GPIOB
//#define GPIO_SDA	GPIOB
//GPIO_ResetBits(GPIO_TypeDef* GPIOx, u16 GPIO_Pin);
//GPIO_SetBits(GPIO_TypeDef* GPIOx, u16 GPIO_Pin);
//GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, u16 GPIO_Pin);
//GPIO_WriteBit(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, BitAction BitVal);
//#define	(IIC_SCL=1) 0
u16 IIC_SCL=0;
GPIO_TypeDef* GPIO_SCL=0;

GPIO_TypeDef* GPIO_SDA=0;
u16 IIC_SDA=0;

/*******************************************************************************
*函数名		:IIC_Delay
*功能描述	:595延时函数
*输入			:j-延时时间					
*输出			:无
*返回值		:无
*例程			：
*******************************************************************************/
void IIC_Delay(u16 j)
{
	for(;j>0;j--);
}

/*******************************************************************************
*函数名		:IIC_INII
*功能描述	:IIC管脚初始化
*输入			:无
*输出			:无
*返回值		:无
*例程			：
*******************************************************************************/
void IIC_PinSet(GPIO_TypeDef* GPIOP_SCL,u16 IIC_SCLP,GPIO_TypeDef* GPIOP_SDA,u16 IIC_SDAP)
{
	GPIO_SCL=GPIOP_SCL;
	IIC_SCL=IIC_SCLP;
	
	GPIO_SDA=GPIOP_SDA;
	IIC_SDA=IIC_SDAP;
}
/*******************************************************************************
*函数名		:IIC_Configuration
*功能描述	:IIC管脚初始化
*输入			:无
*输出			:无
*返回值		:无
*例程			：
*******************************************************************************/
void IIC_Configuration(void)
{					
	GPIO_InitTypeDef GPIO_InitStructure;
	//RCC->APB2ENR|=1<<4;//先使能外设IO PORTC时钟
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
	//SCL管脚配置
	GPIO_InitStructure.GPIO_Pin = IIC_SCL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_SCL, &GPIO_InitStructure);
	//SDA管脚配置
	GPIO_InitStructure.GPIO_Pin = IIC_SDA;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_SDA, &GPIO_InitStructure);

}
/*******************************************************************************
*函数名		:IIC_IN
*功能描述	:IIC读数据管脚配置
*输入			:无
*输出			:无
*返回值		:无
*例程			：
*******************************************************************************/
void IIC_IN(void)
{					
	GPIO_InitTypeDef GPIO_InitStructure;
	//RCC->APB2ENR|=1<<4;//先使能外设IO PORTC时钟
//	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
	
	GPIO_InitStructure.GPIO_Pin = IIC_SDA;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_SDA, &GPIO_InitStructure);

//	IIC_SCL=1;
//	IIC_SDA=1;
}
/*******************************************************************************
*函数名		:IIC_OUT
*功能描述	:IIC写数据管脚配置
*输入			:无
*输出			:无
*返回值		:无
*例程			：
*******************************************************************************/
void IIC_OUT(void)
{					
	GPIO_InitTypeDef GPIO_InitStructure;
	//RCC->APB2ENR|=1<<4;//先使能外设IO PORTC时钟
//	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
	
	GPIO_InitStructure.GPIO_Pin = IIC_SDA;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_SDA, &GPIO_InitStructure);

}
/*******************************************************************************
*函数名		:IIC_Start
*功能描述	:IIC启动
*输入			:无
*输出			:无
*返回值		:无
*例程			：
*******************************************************************************/
void IIC_Start(void)
{
	//将总线设置为输出模式
	IIC_OUT();     //sda线输出
	GPIO_WriteBit(GPIO_SDA,IIC_SDA,Bit_SET);
//	IIC_SDA=1;
	GPIO_WriteBit(GPIO_SCL,IIC_SCL,Bit_SET);
//	IIC_SCL=1;
	IIC_Delay(5);
	GPIO_WriteBit(GPIO_SDA,IIC_SDA,Bit_RESET);
// 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	IIC_Delay(5);
	GPIO_WriteBit(GPIO_SCL,IIC_SCL,Bit_RESET);
//	IIC_SCL=0;//钳住IIC总线，准备发送或接收数据
}
/*******************************************************************************
*函数名		:IIC_Stop
*功能描述	:IIC停止
*输入			:无
*输出			:无
*返回值		:无
*例程			：
*******************************************************************************/
void IIC_Stop(void)
{
	IIC_OUT();//sda线输出
	GPIO_WriteBit(GPIO_SCL,IIC_SCL,Bit_RESET);
	GPIO_WriteBit(GPIO_SDA,IIC_SDA,Bit_RESET);
//	IIC_SCL=0;
//	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
	IIC_Delay(5);
	GPIO_WriteBit(GPIO_SCL,IIC_SCL,Bit_SET);
	GPIO_WriteBit(GPIO_SDA,IIC_SDA,Bit_SET);
//	IIC_SCL=1; 
//	IIC_SDA=1;//发送IIC总线结束信号
	IIC_Delay(5);								
}
/*******************************************************************************
*函数名		:IIC_Wait_Ack
*功能描述	:检测IIC应答
*输入			:无
*输出			:无
*返回值		:1，接收应答失败
					 0，接收应答成功
*例程			：
*******************************************************************************/
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	IIC_IN();//SDA设置为输入
	GPIO_WriteBit(GPIO_SDA,IIC_SDA,Bit_SET);  
//	IIC_SDA=1;
	IIC_Delay(5);
	GPIO_WriteBit(GPIO_SCL,IIC_SCL,Bit_SET);	
//	IIC_SCL=1;
	IIC_Delay(5);	
	while(GPIO_ReadInputDataBit(GPIO_SDA,IIC_SDA))
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	GPIO_WriteBit(GPIO_SCL,IIC_SCL,Bit_RESET);
//	IIC_SCL=0;//时钟输出0 	
	return 0;  
}
/*******************************************************************************
*函数名		:IIC_Ack
*功能描述	:产生应答信号
*输入			:无
*输出			:无
*返回值		:无
*例程			：
*******************************************************************************/
void IIC_Ack(void)
{
	GPIO_WriteBit(GPIO_SCL,IIC_SCL,Bit_RESET);
//	IIC_SCL=0;
	IIC_OUT();
	GPIO_WriteBit(GPIO_SDA,IIC_SDA,Bit_RESET);
//	IIC_SDA=0;
	IIC_Delay(5);
	GPIO_WriteBit(GPIO_SCL,IIC_SCL,Bit_SET);
//	IIC_SCL=1;
	IIC_Delay(5);
	GPIO_WriteBit(GPIO_SCL,IIC_SCL,Bit_RESET);
//	IIC_SCL=0;
}
/*******************************************************************************
*函数名		:IIC_NAck
*功能描述	:不应答
*输入			:无
*输出			:无
*返回值		:无
*例程			：
*******************************************************************************/
//不产生ACK应答		
void IIC_NAck(void)
{
	GPIO_WriteBit(GPIO_SCL,IIC_SCL,Bit_RESET);
//	IIC_SCL=0;
	IIC_OUT();
	GPIO_WriteBit(GPIO_SDA,IIC_SDA,Bit_SET); 
//	IIC_SDA=1;
	IIC_Delay(5);
	GPIO_WriteBit(GPIO_SCL,IIC_SCL,Bit_SET);
//	IIC_SCL=1;
	IIC_Delay(5);
	GPIO_WriteBit(GPIO_SCL,IIC_SCL,Bit_RESET);
//	IIC_SCL=0;
}
/*******************************************************************************
*函数名		:IIC_Send_Byte
*功能描述	:IIC发送一个字节
*输入			:无
*输出			:无
*返回值		:无
*例程			：
*******************************************************************************/
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	IIC_OUT(); 
	GPIO_WriteBit(GPIO_SCL,IIC_SCL,Bit_RESET);
//	IIC_SCL=0;//拉低时钟开始数据传输
	IIC_Delay(5);
	for(t=0;t<8;t++)
	{     
		if((txd&0x80) == 0x00)
			GPIO_WriteBit(GPIO_SDA,IIC_SDA,Bit_RESET);
		else
			GPIO_WriteBit(GPIO_SDA,IIC_SDA,Bit_SET);
//        IIC_SDA=(txd&0x80)>>7;
		txd<<=1; 	
		IIC_Delay(5);   //对TEA5767这三个延时都是必须的
		GPIO_WriteBit(GPIO_SCL,IIC_SCL,Bit_SET);
//		IIC_SCL=1;
		IIC_Delay(5);
		GPIO_WriteBit(GPIO_SCL,IIC_SCL,Bit_RESET);
//		IIC_SCL=0;	
		IIC_Delay(5);
	}	
}
/*******************************************************************************
*函数名		:
*功能描述	:
*输入			:无
*输出			:无
*返回值		:无
*例程			：
*******************************************************************************/
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC_IN();//SDA设置为输入
	for(i=0;i<8;i++ )
	{
		GPIO_WriteBit(GPIO_SCL,IIC_SCL,Bit_RESET);
//	IIC_SCL=0; 
		IIC_Delay(5);
		GPIO_WriteBit(GPIO_SCL,IIC_SCL,Bit_SET);
//	IIC_SCL=1;
		IIC_Delay(5);
		receive<<=1;
		if(GPIO_ReadInputDataBit(GPIO_SDA,IIC_SDA))
			receive++;   
		IIC_Delay(5); 
    }					
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}




