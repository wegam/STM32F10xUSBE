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



#include "74HC595.H"
#include "STM32_GPIO.H"

#include <stdint.h>
//#include "STM32F10x_BitBand.H"

#define HC595Time	20

volatile unsigned char HC595_SegCode[10]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};


void HC595_Delay(u32 time);

void HC595_SDA_LOW(HC595_Pindef *Pinfo);
void HC595_SDA_HIGH(HC595_Pindef *Pinfo);
void HC595_SCK_LOW(HC595_Pindef *Pinfo);
void HC595_SCK_HIGH(HC595_Pindef *Pinfo);
void HC595_RCK_LOW(HC595_Pindef *Pinfo);
void HC595_RCK_HIGH(HC595_Pindef *Pinfo);
void HC595_WriteByte(HC595_Pindef *Pinfo,u8 num);
void HC595_WriteByteN(HC595_Pindef *Pinfo,u8 num);	//�������


/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void HC595_Delay(u32 time)
{
	while(time--);
}
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void HC595_PinConf(HC595_Pindef *Pinfo)
{
	GPIO_Configuration_OPP50	(Pinfo->HC595_SDA_PORT,		Pinfo->HC595_SDA_Pin);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->HC595_SCK_PORT,		Pinfo->HC595_SCK_Pin);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->HC595_RCK_PORT,		Pinfo->HC595_RCK_Pin);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	
//	GPIO_Configuration_OPP50	(Pinfo->HC595_CLR_PORT,		Pinfo->HC595_CLR_Pin);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
//	GPIO_Configuration_OPP50	(Pinfo->HC595_OE_PORT,		Pinfo->HC595_OE_Pin);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
}
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void HC595_SDA_LOW(HC595_Pindef *Pinfo)
{
	GPIO_ResetBits(Pinfo->HC595_SDA_PORT, Pinfo->HC595_SDA_Pin);
}
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void HC595_SDA_HIGH(HC595_Pindef *Pinfo)
{
	GPIO_SetBits(Pinfo->HC595_SDA_PORT, Pinfo->HC595_SDA_Pin);
}
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void HC595_SCK_LOW(HC595_Pindef *Pinfo)
{
	GPIO_ResetBits(Pinfo->HC595_SCK_PORT, Pinfo->HC595_SCK_Pin);
}
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void HC595_SCK_HIGH(HC595_Pindef *Pinfo)
{
	GPIO_SetBits(Pinfo->HC595_SCK_PORT, Pinfo->HC595_SCK_Pin);
}
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void HC595_RCK_LOW(HC595_Pindef *Pinfo)
{
	GPIO_ResetBits(Pinfo->HC595_RCK_PORT, Pinfo->HC595_RCK_Pin);
}
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void HC595_RCK_HIGH(HC595_Pindef *Pinfo)
{
	GPIO_SetBits(Pinfo->HC595_RCK_PORT, Pinfo->HC595_RCK_Pin);
}
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void HC595_WriteByte(HC595_Pindef *Pinfo,u8 Seg)
{
	unsigned char i=0;
	for(i=0;i<8;i++)
	{
		HC595_SCK_LOW(Pinfo);
		HC595_Delay(HC595Time);
		if((Seg&0x80)==0x80)
		{
			HC595_SDA_HIGH(Pinfo);
		}
		else
		{
			HC595_SDA_LOW(Pinfo);
		}
		Seg=Seg<<1;
		HC595_Delay(HC595Time);
		HC595_SCK_HIGH(Pinfo);
		HC595_Delay(HC595Time);		
	}	
}
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void HC595_WriteByteN(HC595_Pindef *Pinfo,u8 Seg)
{
	unsigned char i=0;
	for(i=0;i<8;i++)
	{
		HC595_SCK_HIGH(Pinfo);
		HC595_Delay(HC595Time);
		if((Seg&0x80)==0x80)
		{			
			HC595_SDA_LOW(Pinfo);
		}
		else
		{
			HC595_SDA_HIGH(Pinfo);
		}
		Seg=Seg<<1;
		HC595_Delay(HC595Time);		
		HC595_SCK_LOW(Pinfo);	
		HC595_Delay(HC595Time);		
	}	
}
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void HC595_WriteNumber(HC595_Pindef *Pinfo,u8 num)
{
	HC595_WriteByte(Pinfo,HC595_SegCode[num]);	
}
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void HC595_WriteNumberN(HC595_Pindef *Pinfo,u8 num)
{
	HC595_WriteByteN(Pinfo,HC595_SegCode[num]);	
}
