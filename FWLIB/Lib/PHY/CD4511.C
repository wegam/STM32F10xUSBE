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



#include "CD4511.H"
#include "STM32_GPIO.H"
//#include "STM32F10x_BitBand.H"


/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void CD4511_Delay(u32 time)
{
	while(time--);
}
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void CD4511_PinConf(CD4511_Pindef *Pinfo)
{
	GPIO_Configuration_OPP50	(Pinfo->CD4511_A0_PORT,		Pinfo->CD4511_A0_Pin);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->CD4511_A1_PORT,		Pinfo->CD4511_A1_Pin);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->CD4511_A2_PORT,		Pinfo->CD4511_A2_Pin);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->CD4511_A3_PORT,		Pinfo->CD4511_A3_Pin);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->CD4511_EN_PORT,		Pinfo->CD4511_EN_Pin);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
}

/*******************************************************************************
*������			:	function
*��������		:	д�����ݲ���ʾ
*����				: 
*����ֵ			:	��
*******************************************************************************/
void CD4511_WriteData(CD4511_Pindef *Pinfo,u8 num)			//д�����ݲ���ʾ
{
	//�ر����---��Ӱ
	GPIO_ResetBits(Pinfo->CD4511_EN_PORT, Pinfo->CD4511_EN_Pin);
//	CD4511_Delay(50);
	GPIO_ResetBits(Pinfo->CD4511_A0_PORT, Pinfo->CD4511_A0_Pin);
	GPIO_ResetBits(Pinfo->CD4511_A1_PORT, Pinfo->CD4511_A1_Pin);
	GPIO_ResetBits(Pinfo->CD4511_A2_PORT, Pinfo->CD4511_A2_Pin);
	GPIO_ResetBits(Pinfo->CD4511_A3_PORT, Pinfo->CD4511_A3_Pin);
	CD4511_Delay(50);
	//A0
	if((num&0x01)!=0)
	{
		GPIO_SetBits(Pinfo->CD4511_A0_PORT, Pinfo->CD4511_A0_Pin);
	}
	else
	{
		GPIO_ResetBits(Pinfo->CD4511_A0_PORT, Pinfo->CD4511_A0_Pin);
	}
	//A1
	if((num&0x02)!=0)
	{
		GPIO_SetBits(Pinfo->CD4511_A1_PORT, Pinfo->CD4511_A1_Pin);
	}
	else
	{
		GPIO_ResetBits(Pinfo->CD4511_A1_PORT, Pinfo->CD4511_A1_Pin);
	}
	//A2
	if((num&0x04)!=0)
	{
		GPIO_SetBits(Pinfo->CD4511_A2_PORT, Pinfo->CD4511_A2_Pin);
	}
	else
	{
		GPIO_ResetBits(Pinfo->CD4511_A2_PORT, Pinfo->CD4511_A2_Pin);
	}
	//A3
	if((num&0x08)!=0)
	{
		GPIO_SetBits(Pinfo->CD4511_A3_PORT, Pinfo->CD4511_A3_Pin);
	}
	else
	{
		GPIO_ResetBits(Pinfo->CD4511_A3_PORT, Pinfo->CD4511_A3_Pin);
	}
//	CD4511_Delay(10000);
	GPIO_SetBits(Pinfo->CD4511_EN_PORT, Pinfo->CD4511_EN_Pin);
}
/*******************************************************************************
* ������			:	CD4511_DisplayOFF
* ��������		:	�ر���ʾ---��NPN������
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void CD4511_DisplayOFF(CD4511_Pindef *Pinfo)			//�ر���ʾ---��NPN������
{
	GPIO_ResetBits(Pinfo->CD4511_EN_PORT, Pinfo->CD4511_EN_Pin);
//	CD4511_Delay(1000);
}
