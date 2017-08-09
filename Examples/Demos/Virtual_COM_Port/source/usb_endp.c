/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_endp.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Endpoint routines
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_mem.h"
#include "hw_config.h"
#include "usb_istr.h"
#include "usb_pwr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 buffer_out[VIRTUAL_COM_PORT_DATA_SIZE];

u32 count_out = 0;
u32 count_in = 0;

#define VCOMPORT_IN_FRAME_INTERVAL             5

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  :	EP3_IN_Callback
* Description    :	端点3输出回调	---设备接收
* Input          : 	None.
* Output         : 	None.
* Return         : 	None.
*******************************************************************************/
void EP3_OUT_Callback(void)
{
  count_out = GetEPRxCount(ENDP3);														//获取USB接收到的数据
  PMAToUserBufferCopy(buffer_out, ENDP3_RXADDR, count_out);		//USB接口到的数据从串口发送
	SetEPRxValid(ENDP3);																				//使能端点3
	
	//----添加程序---USB发送给串口
	USB_To_USART_Send_Data(buffer_out,count_out);
}

/*******************************************************************************
* Function Name  : 	EP1_IN_Callback
* Description    :	端点1输入的回调函数	--设备发送
* Input          : 	None.
* Output         : 	None.
* Return         : 	None.
*******************************************************************************/
void EP1_IN_Callback(void)
{
//	count_in = 0;
//	Handle_USBAsynchXfer();
	USART_To_USB_Send_Data();
//	USART_TX_DMAFlagClear(USART1);
}
/*******************************************************************************
* Function Name  : SOF_Callback / INTR_SOFINTR_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SOF_Callback(void)
{
//  static u32 FrameCount = 0;
//  
//  if(bDeviceState == CONFIGURED)
//  {
//    if (FrameCount++ == VCOMPORT_IN_FRAME_INTERVAL)
//    {
//      /* Reset the frame counter */
//      FrameCount = 0;
//      
//      /* Check the data to be sent through IN pipe */
//      Handle_USBAsynchXfer();
//    }
//  }
	Handle_USBAsynchXfer();
//	USART_To_USB_Send_Data(); 
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

