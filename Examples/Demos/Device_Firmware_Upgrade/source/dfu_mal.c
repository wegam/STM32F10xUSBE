/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : dfu_mal.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Generic media access Layer
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "dfu_mal.h"
#include "spi_if.h"
#include "flash_if.h"
#include "nor_if.h"
#include "fsmc_nor.h"
#include "usb_type.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "platform_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u16 (*pMAL_Init) (void);
u16 (*pMAL_Erase) (u32 SectorAddress);
u16 (*pMAL_Write) (u32 SectorAddress, u32 DataLength);
u8  *(*pMAL_Read)  (u32 SectorAddress, u32 DataLength);
u8  MAL_Buffer[wTransferSize]; /* RAM Buffer for Downloaded Data */
NOR_IDTypeDef NOR_ID;
extern ONE_DESCRIPTOR DFU_String_Descriptor[7];


static const u16  TimingTable[5][2] =
  {
    { 3000 ,  20 }, /* SPI Flash */
    { 1000 ,  25 }, /* NOR Flash M29W128F */
    {  100 , 104 }, /* Internal Flash */
    { 1000 ,  25 }, /* NOR Flash M29W128G */
    { 1000 ,  45 }  /* NOR Flash S29GL128 */
  };

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : MAL_Init
* Description    : Initializes the Media on the STM32
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u16 MAL_Init(void)
{

  FLASH_If_Init(); /* Internal Flash */
  SPI_If_Init();   /* SPI Flash */
#ifdef USE_STM3210E_EVAL  /* USE_STM3210B_EVAL */
  NOR_If_Init();  /* NOR Flash */
  FSMC_NOR_ReadID(&NOR_ID);
    
  FSMC_NOR_ReturnToReadMode();

  /* select the alternate descriptor following NOR ID */
  if ((NOR_ID.Manufacturer_Code == 0x01)&&(NOR_ID.Device_Code2 == NOR_S29GL128))
  {
    DFU_String_Descriptor[6].Descriptor = DFU_StringInterface2_3;
  } 
  
  /* select the alternate descriptor following NOR ID */
  if  ((NOR_ID.Manufacturer_Code == 0x20)&&(NOR_ID.Device_Code2 == NOR_M29W128G))
  {
    DFU_String_Descriptor[6].Descriptor = DFU_StringInterface2_2;
  }
#endif
  return MAL_OK;
}

/*******************************************************************************
* Function Name  : MAL_Erase
* Description    : Erase sector
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u16 MAL_Erase(u32 SectorAddress)
{

  switch (SectorAddress & MAL_MASK)
  {
    case SPI_FLASH_BASE:
      pMAL_Erase = SPI_If_Erase;
      break;

    case INTERNAL_FLASH_BASE:
      pMAL_Erase = FLASH_If_Erase;
      break;
#ifdef USE_STM3210E_EVAL  /* USE_STM3210B_EVAL */
    case NOR_FLASH_BASE:
      pMAL_Erase = NOR_If_Erase;
      break;
#endif
    default:
      return MAL_FAIL;
  }
  return pMAL_Erase(SectorAddress);
}

/*******************************************************************************
* Function Name  : MAL_Write
* Description    : Write sectors
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u16 MAL_Write (u32 SectorAddress, u32 DataLength)
{

  switch (SectorAddress & MAL_MASK)
  {
    case SPI_FLASH_BASE:
      pMAL_Write = SPI_If_Write;
      break;

    case INTERNAL_FLASH_BASE:
      pMAL_Write = FLASH_If_Write;
      break;
#ifdef USE_STM3210E_EVAL
    case NOR_FLASH_BASE:
      pMAL_Write = NOR_If_Write;
      break;
#endif
    default:
      return MAL_FAIL;
  }
  return pMAL_Write(SectorAddress, DataLength);
}

/*******************************************************************************
* Function Name  : MAL_Read
* Description    : Read sectors
* Input          : None
* Output         : None
* Return         : Buffer pointer
*******************************************************************************/
u8 *MAL_Read (u32 SectorAddress, u32 DataLength)
{

  switch (SectorAddress & MAL_MASK)
  {
    case SPI_FLASH_BASE:
      pMAL_Read = SPI_If_Read;
      break;

    case INTERNAL_FLASH_BASE:
      pMAL_Read = FLASH_If_Read;
      break;
#ifdef USE_STM3210E_EVAL
    case NOR_FLASH_BASE:
      pMAL_Read = NOR_If_Read;
      break;
#endif
    default:
      return 0;
  }
  return pMAL_Read (SectorAddress, DataLength);
}

/*******************************************************************************
* Function Name  : MAL_GetStatus
* Description    : Get status
* Input          : None
* Output         : None
* Return         : Buffer pointer
*******************************************************************************/
u16 MAL_GetStatus(u32 SectorAddress , u8 Cmd, u8 *buffer)
{
  u8 x = (SectorAddress  >> 26) & 0x03 ; /* 0x000000000 --> 0 */
  /* 0x640000000 --> 1 */
  /* 0x080000000 --> 2 */

  u8 y = Cmd & 0x01;

  if ((x == 1) && (NOR_ID.Device_Code2 == NOR_M29W128G)&& (NOR_ID.Manufacturer_Code == 0x20))
  {
    x = 3 ;
  }
  else if((x == 1) && (NOR_ID.Device_Code2 == NOR_S29GL128) && (NOR_ID.Manufacturer_Code == 0x01))
  {
    x = 4 ;
  }  
    
  SET_POLLING_TIMING(TimingTable[x][y]);  /* x: Erase/Write Timing */
  /* y: Media              */
  return MAL_OK;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
