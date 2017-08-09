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



#ifndef __STM32_CAN_H
#define __STM32_CAN_H

#include "stm32f10x_can.h"
//#include "stm32f10x_rcc.h"
//#include "stm32f10x_gpio.h"
//#include "stm32f10x_nvic.h"

#include "stm32f10x_type.h"

//#include "CAN_TEST.H"
//#include "STM32_WOW.H"

/* CAN�����ض��� ----------------------------------------*/
typedef enum
{ 
  CANRemapNone = 0,
  CANRemap1, 
  CANRemap2
}CANRemap_TypeDef;

#define IS_CANRemap_Status(CANRemap_Status) (((CANRemap_Status) == CANRemapNone) || ((CANRemap_Status) == CANRemap1) || \
                              ((CANRemap_Status) == CANRemap2))

//*****************CAN��ʼ��
void CAN_Configuration_NR(u32 CAN_BaudRate);				//CAN����---��־λ��ѯ��ʽ�������ж�
void CAN_Configuration_IT(u32 CAN_BaudRate);				//CAN����---�жϷ�ʽ

//*****************CAN�˲�������
void CAN_FilterInitConfiguration_StdData(u8 CAN_GROUP,u16 CAN_ID,u16 MaskId);	//CAN�˲�������---��׼����֡ģʽ
void CAN_FilterInitConfiguration_ExtData(u8 CAN_GROUP,u32 CAN_ID,u32 MaskId);	//CAN�˲�������---��չ����֡ģʽ


ErrorStatus CAN_PinSet(CANRemap_TypeDef CANRemap_Status);		//CAN�������ú���

//*****************CAN��������
void CAN_StdTX_DATA(u16 CAN_ID,u8 length,u8 *Data);			//CANʹ�ñ�׼֡��������
void CAN_ExtTX_DATA(u32 CAN_ID,u8 length,u8 *Data);			//CANʹ����չ֡��������

void CAN_Server(void);

void CAN_RX_DATA(CanRxMsg *RxMessage);
#endif

