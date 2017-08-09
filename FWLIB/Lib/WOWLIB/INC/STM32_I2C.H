/******************************** User_library *********************************
* �ļ��� 	: STM32_SDCard.H
* ����   	: wegam@sina.com
* �汾   	: V
* ����   	: 2016/01/01
* ˵��   	: 
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
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//�������̿�����
//IIC ��������	
//����ǿ
//�޸�����:2014/2/24 
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	
#define delay_usDelay_us
			
//IO��������
//#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=8<<28;}
//#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=3<<28;}

//IO��������	
//#define IIC_SCL    PB11 //SCL
//#define IIC_SDA    PB10 //SDA	
//#define READ_SDA   PB10in  //����SDA 
/* GPIO Init structure definition */
typedef struct
{
	GPIO_TypeDef* GPIO_SCL;
	GPIO_TypeDef* GPIO_SDA;
	u16 IIC_SCL;
	u16 IIC_SDA;
	void (*IIC_INII)(GPIO_TypeDef*,u16,GPIO_TypeDef*,u16); 	//I2C�ܽų�ʼ������ָ��
  void (*IIC_Configuration)(); 	//I2C�ܽų�ʼ������ָ��
}IIC_InitTypeDef;


//IIC���в�������
//void IIC_INII(GPIO_TypeDef*,u16,GPIO_TypeDef*,u16);
void IIC_Delay(u16 j);		//��ʱ����
void IIC_Configuration(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin);		//I2C�ܽų�ʼ��
void IIC_IN(GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin);
void IIC_OUT(GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin);


void IIC_Start(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin);						//����IIC��ʼ�ź�
void IIC_Stop(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin);				//����IICֹͣ�ź�

//void IIC_Send_Byte(u8);			//IIC����һ���ֽ�
void IIC_Send_Byte(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin,u8 txd);//IIC��ȡһ���ֽ�
u8 IIC_Send_Data(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin,unsigned char *Buffer_Addr);

u8 IIC_Wait_Ack(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin);					//IIC����ACK�ź�
void IIC_NAck(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin);				//IIC������ACK�ź�

u8 IIC_Read_Byte(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin,unsigned char ack);	//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
void IIC_Read_Data(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin,unsigned char *Buffer_Addr);

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	




#endif

