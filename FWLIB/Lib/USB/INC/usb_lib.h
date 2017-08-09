/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_lib.h
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : USB library include files
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_LIB_H
#define __USB_LIB_H

/* Includes ------------------------------------------------------------------*/
#include "usb_type.h"			//主要是用 typedef为 stm32支持的数据类型取一些新的名称/
#include "usb_regs.h"			//用于寄存器操作
#include "usb_def.h"			//主要是定义一些相关的结构体
#include "usb_core.h"			//USB总线数据处理的核心文件
#include "usb_init.h"			//用于端点数据输入输入中断处理
#include "usb_mem.h"			//用于缓冲区操作
#include "usb_int.h"			//用于端点数据输入输入中断处

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* External variables --------------------------------------------------------*/

#endif /* __USB_LIB_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
