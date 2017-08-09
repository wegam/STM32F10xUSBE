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


#ifndef __STM32_I2C_H
#define __STM32_I2C_H

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
//////////////////////////////////////////////////////////////////////////////////	
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//称重托盘控制器
//IIC 驱动函数	
//唐永强
//修改日期:2014/2/24 
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	
#define delay_usDelay_us
			
//IO方向设置
//#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=8<<28;}
//#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=3<<28;}

//IO操作函数	
//#define IIC_SCL    PB11 //SCL
//#define IIC_SDA    PB10 //SDA	
//#define READ_SDA   PB10in  //输入SDA 
/* GPIO Init structure definition */
typedef struct
{
	GPIO_TypeDef* GPIO_SCL;
	GPIO_TypeDef* GPIO_SDA;
	u16 IIC_SCL;
	u16 IIC_SDA;
	void (*IIC_INII)(GPIO_TypeDef*,u16,GPIO_TypeDef*,u16); 	//I2C管脚初始化函数指针
  void (*IIC_Configuration)(); 	//I2C管脚初始化函数指针
}IIC_InitTypeDef;


//IIC所有操作函数
//void IIC_INII(GPIO_TypeDef*,u16,GPIO_TypeDef*,u16);
void IIC_Delay(u16 j);		//延时函数
void IIC_Configuration(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin);		//I2C管脚初始化
void IIC_IN(GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin);
void IIC_OUT(GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin);


void IIC_Start(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin);						//发送IIC开始信号
void IIC_Stop(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin);				//发送IIC停止信号

//void IIC_Send_Byte(u8);			//IIC发送一个字节
void IIC_Send_Byte(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin,u8 txd);//IIC读取一个字节
u8 IIC_Send_Data(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin,unsigned char *Buffer_Addr);

u8 IIC_Wait_Ack(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin); 				//IIC等待ACK信号
void IIC_Ack(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin);					//IIC发送ACK信号
void IIC_NAck(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin);				//IIC不发送ACK信号

u8 IIC_Read_Byte(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin,unsigned char ack);	//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
void IIC_Read_Data(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin,unsigned char *Buffer_Addr);

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	




#endif


