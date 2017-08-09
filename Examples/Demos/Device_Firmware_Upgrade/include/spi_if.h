/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : dfu_mal.h
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Header for dfu_mal.c file.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_IF_MAL_H
#define __SPI_IF_MAL_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

u16 SPI_If_Init(void);
u16 SPI_If_Erase (u32 SectorAddress);
u16 SPI_If_Write (u32 SectorAddress, u32 DataLength);
u8 *SPI_If_Read (u32 SectorAddress, u32 DataLength);

#endif /* __SPI_IF_MAL_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
