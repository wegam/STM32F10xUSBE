/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : nor_if.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : specific media access Layer for NOR flash
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#include "platform_config.h"

#ifdef USE_STM3210E_EVAL

/* Includes ------------------------------------------------------------------*/
#include "fsmc_nor.h"
#include "nor_if.h"
#include "dfu_mal.h"
#include "stm32f10x_fsmc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern NOR_IDTypeDef NOR_ID;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : NOR_If_Init
* Description    : Initializes the Media on the STM32
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u16 NOR_If_Init(void)
{
  /* Configure FSMC Bank1 NOR/SRAM2 */
  FSMC_NOR_Init();

  /* Enable FSMC Bank1 NOR/SRAM2 */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM2, ENABLE);

  return MAL_OK;
}

/*******************************************************************************
* Function Name  : NOR_If_Erase
* Description    : Erase sector
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u16 NOR_If_Erase(u32 Address)
{
  /* select the alternate descriptor following NOR ID */
  if ((NOR_ID.Manufacturer_Code == 0x01)&&(NOR_ID.Device_Code2 == NOR_S29GL128))
  {
    FSMC_NOR_EraseBlock((Address&0x00FFFFFF)>>1);
  }
  else
  {
    FSMC_NOR_EraseBlock(Address&0x00FFFFFF);    
  }

  return MAL_OK;
}

/*******************************************************************************
* Function Name  : NOR_If_Write
* Description    : Write sectors
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u16 NOR_If_Write(u32 Address, u32 DataLength)
{
  if ((DataLength & 1) == 1)
  {
    DataLength += 1;
  }
 
  FSMC_NOR_WriteBuffer((u16 *)MAL_Buffer, (Address&0x00FFFFFF), DataLength >> 1);  
  
  return MAL_OK;
}

/*******************************************************************************
* Function Name  : NOR_If_Read
* Description    : Read sectors
* Input          : None
* Output         : None
* Return         : buffer address pointer
*******************************************************************************/
u8 *NOR_If_Read(u32 Address, u32 DataLength)
{
  return  (u8*)(Address);
}

#endif

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
