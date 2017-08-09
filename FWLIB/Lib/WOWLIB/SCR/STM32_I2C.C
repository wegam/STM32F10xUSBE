/******************************** User_library *********************************
* 文件名 	: STM32_SDCard.H
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


#include "STM32_I2C.H"
//#include "STM32F10x_BitBand.H"

//////////////////////////////////////////////////////////////////////////////////	

////////////////////////////////////////////////////////////////////////////////// 	
//#define IIC_SCL	GPIO_Pin_10
//#define IIC_SDA	GPIO_Pin_11
//#define GPIO_SCL	GPIOB
//#define GPIO_SDA	GPIOB
//GPIO_ResetBits(GPIO_TypeDef* GPIOx, u16 GPIO_Pin);
//GPIO_SetBits(GPIO_TypeDef* GPIOx, u16 GPIO_Pin);
//GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, u16 GPIO_Pin);
//GPIO_WriteBit(GPIO_TypeDef* GPIOx, u16 GPIO_Pin, BitActionBitVal);
//#define	(IIC_SCL=1) 0
//u16 IIC_SCL=0;
//GPIO_TypeDef* GPIO_SCL=0;

//GPIO_TypeDef* GPIO_SDA=0;
//u16 IIC_SDA=0;

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
	j*=5;
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
//void IIC_INII(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin)
//{
//	GPIO_SCL=GPIOx_SCL;
//	IIC_SCL=IIC_SCLPin;
//	
//	GPIO_SDA=GPIOx_SDA;
//	IIC_SDA=IIC_SDAPin;
//}
/*******************************************************************************
*函数名		:IIC_Configuration
*功能描述	:IIC管脚初始化
*输入			:无
*输出			:无
*返回值		:无
*例程			：
*******************************************************************************/
void IIC_Configuration(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin)
{					
	GPIO_InitTypeDef GPIO_InitStructure;
	//RCC->APB2ENR|=1<<4;//先使能外设IO PORTC时钟
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOC, ENABLE );	
	//SCL管脚配置
	GPIO_InitStructure.GPIO_Pin = IIC_SCLPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx_SCL, &GPIO_InitStructure);
	//SDA管脚配置
	GPIO_InitStructure.GPIO_Pin = IIC_SDAPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx_SDA, &GPIO_InitStructure);

}
/*******************************************************************************
*函数名		:IIC_IN
*功能描述	:IIC读数据管脚配置
*输入			:无
*输出			:无
*返回值		:无
*例程			：
*******************************************************************************/
void IIC_IN(GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin)
{					
	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC->APB2ENR|=1<<4;//先使能外设IO PORTC时钟
//	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
	
	GPIO_InitStructure.GPIO_Pin = IIC_SDAPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;   //上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx_SDA, &GPIO_InitStructure);

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
void IIC_OUT(GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin)
{					
	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC->APB2ENR|=1<<4;//先使能外设IO PORTC时钟
//	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
	
	GPIO_InitStructure.GPIO_Pin = IIC_SDAPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx_SDA, &GPIO_InitStructure);

}
/*******************************************************************************
*函数名		:IIC_Start
*功能描述	:IIC启动,在SCL线是高电平时SDA线从高电平向低电平切换这个情况表示起始条件
*输入			:无
*输出			:无
*返回值		:无
*例程			：
*******************************************************************************/
void IIC_Start(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin)
{
	//将总线设置为输出模式
	IIC_OUT(GPIOx_SDA,IIC_SDAPin);     //sda线输出
	GPIO_SetBits(GPIOx_SDA,IIC_SDAPin);
//	IIC_SDA=1;
	GPIO_SetBits(GPIOx_SCL,IIC_SCLPin);
//	IIC_SCL=1;
	IIC_Delay(5);
	GPIO_ResetBits(GPIOx_SDA,IIC_SDAPin);
// 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	IIC_Delay(5);
	GPIO_ResetBits(GPIOx_SCL,IIC_SCLPin);
//	IIC_SCL=0;//钳住IIC总线，准备发送或接收数据
}
/*******************************************************************************
*函数名		:IIC_Stop
*功能描述	:IIC停止,当SCL是高电平时SDA线由低电平向高电平切换表示停止条件
*输入			:无
*输出			:无
*返回值		:无
*例程			：
*******************************************************************************/
void IIC_Stop(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin)
{
	IIC_OUT(GPIOx_SDA,IIC_SDAPin);//sda线输出
	GPIO_ResetBits(GPIOx_SCL,IIC_SCLPin);
	GPIO_ResetBits(GPIOx_SDA,IIC_SDAPin);
//	IIC_SCL=0;
//	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
	IIC_Delay(5);
	GPIO_SetBits(GPIOx_SCL,IIC_SCLPin);
	GPIO_SetBits(GPIOx_SDA,IIC_SDAPin);
//	IIC_SCL=1; 
//	IIC_SDA=1;//发送IIC总线结束信号
	IIC_Delay(5);
	GPIO_ResetBits(GPIOx_SCL,IIC_SCLPin);
	GPIO_ResetBits(GPIOx_SDA,IIC_SDAPin);	
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
u8 IIC_Wait_Ack(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin)
{
	u8 ucErrTime=0;
	IIC_IN(GPIOx_SDA,IIC_SDAPin);//SDA设置为输入
	IIC_Delay(5);
	GPIO_SetBits(GPIOx_SDA,IIC_SDAPin);  
//	IIC_SDA=1;
	IIC_Delay(5);
	GPIO_SetBits(GPIOx_SCL,IIC_SCLPin);	
//	IIC_SCL=1;
	IIC_Delay(5);	
	while(GPIO_ReadInputDataBit(GPIOx_SDA,IIC_SDAPin))
	{
		ucErrTime++;
		IIC_Delay(5);
		if(ucErrTime>250)
		{
			IIC_Stop(GPIOx_SCL,IIC_SCLPin,GPIOx_SDA,IIC_SDAPin);
			return 0;
		}
	}
	GPIO_ResetBits(GPIOx_SCL,IIC_SCLPin);
//	IIC_SCL=0;//时钟输出0 	
	return 1;  
}
/*******************************************************************************
*函数名		:IIC_Ack
*功能描述	:产生应答信号
*输入			:无
*输出			:无
*返回值		:无
*例程			：
*******************************************************************************/
void IIC_Ack(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin)
{
	GPIO_ResetBits(GPIOx_SCL,IIC_SCLPin);
//	IIC_SCL=0;
	IIC_Delay(5);
	
	IIC_OUT(GPIOx_SDA,IIC_SDAPin);
	GPIO_ResetBits(GPIOx_SDA,IIC_SDAPin);
//	IIC_SDA=0;
	IIC_Delay(5);
	GPIO_SetBits(GPIOx_SCL,IIC_SCLPin);
//	IIC_SCL=1;
	IIC_Delay(5);
	GPIO_ResetBits(GPIOx_SCL,IIC_SCLPin);
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
void IIC_NAck(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin)
{
	GPIO_ResetBits(GPIOx_SCL,IIC_SCLPin);
//	IIC_SCL=0;
	IIC_OUT(GPIOx_SDA,IIC_SDAPin);
	GPIO_SetBits(GPIOx_SDA,IIC_SDAPin); 
//	IIC_SDA=1;
	IIC_Delay(5);
	GPIO_SetBits(GPIOx_SCL,IIC_SCLPin);
//	IIC_SCL=1;
	IIC_Delay(5);
	GPIO_ResetBits(GPIOx_SCL,IIC_SCLPin);
//	IIC_SCL=0;
}
/*******************************************************************************
*函数名		:IIC_Send_Byte
*功能描述	:IIC发送一个字节
*输入			:无
*输出			:无
*返回值		:无
*例程			:
*******************************************************************************/
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			
void IIC_Send_Byte(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin,u8 txd)
{                        
	u8 t;   
	IIC_OUT(GPIOx_SDA,IIC_SDAPin); 
	GPIO_ResetBits(GPIOx_SCL,IIC_SCLPin);
//	IIC_SCL=0;//拉低时钟开始数据传输
	IIC_Delay(5);
	for(t=0;t<8;t++)
	{     
		if((txd&0x80) == 0x00)
			GPIO_ResetBits(GPIOx_SDA,IIC_SDAPin);
		else
			GPIO_SetBits(GPIOx_SDA,IIC_SDAPin);
//        IIC_SDA=(txd&0x80)>>7;
		txd<<=1; 	
		IIC_Delay(5);   //对TEA5767这三个延时都是必须的
		GPIO_SetBits(GPIOx_SCL,IIC_SCLPin);
//		IIC_SCL=1;
		IIC_Delay(5);
		GPIO_ResetBits(GPIOx_SCL,IIC_SCLPin);
//		IIC_SCL=0;	
		IIC_Delay(5);
	}	
}
/*******************************************************************************
* 函数名		:IIC_Send_Data	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
u8 IIC_Send_Data(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin,unsigned char *Buffer_Addr)
{
	u8 ack_flag=0;
	IIC_Start(GPIOx_SCL,IIC_SCLPin,GPIOx_SDA,IIC_SDAPin);
	IIC_Send_Byte(GPIOx_SCL,IIC_SCLPin,GPIOx_SDA,IIC_SDAPin,0x82);//IIC读取一个字节
	ack_flag=IIC_Wait_Ack(GPIOx_SCL,IIC_SCLPin,GPIOx_SDA,IIC_SDAPin); 				//IIC等待ACK信号
	if(ack_flag==0)
		return 1;
	else
		return 0;

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
u8 IIC_Read_Byte(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin,unsigned char ack)
{
	unsigned char i,receive=0;
	IIC_IN(GPIOx_SDA,IIC_SDAPin);//SDA设置为输入
	for(i=0;i<8;i++ )
	{
		GPIO_ResetBits(GPIOx_SCL,IIC_SCLPin);
//	IIC_SCL=0; 
		IIC_Delay(5);
		GPIO_SetBits(GPIOx_SCL,IIC_SCLPin);
//	IIC_SCL=1;
		IIC_Delay(5);
		receive<<=1;
		if(GPIO_ReadInputDataBit(GPIOx_SDA,IIC_SDAPin))
			receive++;   
		IIC_Delay(5); 
	}					
	if (!ack)
		IIC_NAck(GPIOx_SCL,IIC_SCLPin,GPIOx_SDA,IIC_SDAPin);//发送nACK
	else
		IIC_Ack(GPIOx_SCL,IIC_SCLPin,GPIOx_SDA,IIC_SDAPin); //发送ACK   
	return receive;
}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void IIC_Read_Data(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin,unsigned char *Buffer_Addr)
{

}









