/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : nor_if.h
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Header for nor_if.c file.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NOR_IF_MAL_H
#define __NOR_IF_MAL_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

u16 NOR_If_Init(void);
u16 NOR_If_Erase(u32 Address);
u16 NOR_If_Write(u32 Address, u32 DataLength);
u8 *NOR_If_Read(u32 Address, u32 DataLength);

#endif /* __NOR_IF_MAL_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
