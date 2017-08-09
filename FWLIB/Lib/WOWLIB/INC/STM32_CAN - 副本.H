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



#ifndef __STM32_CAN_H
#define __STM32_CAN_H

#include "stm32f10x_can.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_nvic.h"

#include "CAN_TEST.H"
#include "STM32_WOW.H"

/* CAN引脚重定义 ----------------------------------------*/
typedef enum
{ 
  CANRemapNone = 0,
  CANRemap1, 
  CANRemap2
}CANRemap_TypeDef;

#define IS_CANRemap_Status(CANRemap_Status) (((CANRemap_Status) == CANRemapNone) || ((CANRemap_Status) == CANRemap1) || \
                              ((CANRemap_Status) == CANRemap2))


void CAN_Configuration(void);	//CAN配置
void CAN_Server(void);	

ErrorStatus CAN_PinSet(CANRemap_TypeDef CANRemap_Status);		//CAN引脚配置函数
void CAN_INIT(void);
void CAN_FilterInitConfiguration(u8 CAN_GROUP,u16 CAN_ID,u16 MaskId);	//CAN滤波器配置
void CAN_TX_DATA(u16 CAN_ID,u16 data4);
void CAN_RX_DATA(CanRxMsg *RxMessage);
#endif


