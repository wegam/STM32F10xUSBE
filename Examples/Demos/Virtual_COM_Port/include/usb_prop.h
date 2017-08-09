/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_prop.h
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : All processing related to Virtual COM Port Demo (Endpoint 0)
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usb_prop_H
#define __usb_prop_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct
{
  u32 bitrate;			//波特率
  u8 format;				//停止位
  u8 paritytype;		//检验位
  u8 datatype;			//数据位
}LINE_CODING;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/

#define Virtual_Com_Port_GetConfiguration          NOP_Process
//#define Virtual_Com_Port_SetConfiguration          NOP_Process
#define Virtual_Com_Port_GetInterface              NOP_Process
#define Virtual_Com_Port_SetInterface              NOP_Process
#define Virtual_Com_Port_GetStatus                 NOP_Process
#define Virtual_Com_Port_ClearFeature              NOP_Process
#define Virtual_Com_Port_SetEndPointFeature        NOP_Process
#define Virtual_Com_Port_SetDeviceFeature          NOP_Process
//#define Virtual_Com_Port_SetDeviceAddress          NOP_Process

#define SEND_ENCAPSULATED_COMMAND   0x00			//
#define GET_ENCAPSULATED_RESPONSE   0x01			//
#define SET_COMM_FEATURE            0x02			//设置串口特性请求
#define GET_COMM_FEATURE            0x03			//获取串口特性请求
#define CLEAR_COMM_FEATURE          0x04			//清除串口特性请求
#define SET_LINE_CODING             0x20			//设置串口通讯信息请求
#define GET_LINE_CODING             0x21			//获取串口通信信息请求
#define SET_CONTROL_LINE_STATE      0x22			//设置控制信息状态
#define SEND_BREAK                  0x23			//

/* Exported functions ------------------------------------------------------- */
void Virtual_Com_Port_init(void);
void Virtual_Com_Port_Reset(void);
void Virtual_Com_Port_SetConfiguration(void);
void Virtual_Com_Port_SetDeviceAddress (void);
void Virtual_Com_Port_Status_In (void);
void Virtual_Com_Port_Status_Out (void);
RESULT Virtual_Com_Port_Data_Setup(u8);
RESULT Virtual_Com_Port_NoData_Setup(u8);
RESULT Virtual_Com_Port_Get_Interface_Setting(u8 Interface, u8 AlternateSetting);
u8 *Virtual_Com_Port_GetDeviceDescriptor(u16 );
u8 *Virtual_Com_Port_GetConfigDescriptor(u16);
u8 *Virtual_Com_Port_GetStringDescriptor(u16);

u8 *Virtual_Com_Port_GetLineCoding(u16 Length);	//获取串口通信信息
u8 *Virtual_Com_Port_SetLineCoding(u16 Length);	//设置串口通讯信息

#endif /* __usb_prop_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

