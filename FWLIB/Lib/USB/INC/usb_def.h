/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_def.h
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Definitions related to USB Core
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DEF_H
#define __USB_DEF_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum _RECIPIENT_TYPE
{
  DEVICE_RECIPIENT,     //DEVICE_RECIPIENT==00	接收设备/* Recipient device */
  INTERFACE_RECIPIENT,  //INTERFACE_RECIPIENT==01	/* Recipient interface */
  ENDPOINT_RECIPIENT,   //ENDPOINT_RECIPIENT==02	/* Recipient endpoint */
  OTHER_RECIPIENT				//OTHER_RECIPIENT==03
} RECIPIENT_TYPE;


typedef enum _STANDARD_REQUESTS
{
  GET_STATUS = 0,			//GET_STATUS==00用来返回特定接收者的状态
  CLEAR_FEATURE,			//CLEAR_FEATURE==01用来清除或禁止接收者的某些特性
  RESERVED1,							
  SET_FEATURE,				//SET_FEATURE==03用来启用或激活命令接收者的某些特性				
  RESERVED2,
  SET_ADDRESS,				//SET_ADDRESS==05用来给设备分配地址
  GET_DESCRIPTOR,			//GET_DESCRIPTOR==06用于主机获取设备的特定描述符
  SET_DESCRIPTOR,			//SET_DESCRIPTOR==07修改设备中有关的描述符，或者增加新的描述符
  GET_CONFIGURATION,	//GET_CONFIGURATION==08用于主机获取设备当前设备的配置值（注同上面的不同） 
  SET_CONFIGURATION,	//SET_CONFIGURATION==09用于主机指示设备采用的要求的配置
  GET_INTERFACE,			//GET_INTERFACE==0A用于获取当前某个接口描述符编号
  SET_INTERFACE,			//SET_INTERFACE==0B用于主机要求设备用某个描述符来描述接口
  TOTAL_sREQUEST,  /* Total number of Standard request */
  SYNCH_FRAME = 12		//SYNCH_FRAME==0C用于设备设置和报告一个端点的同步帧
} STANDARD_REQUESTS;

/* Definition of "USBwValue" */
typedef enum _DESCRIPTOR_TYPE
{
  DEVICE_DESCRIPTOR = 1,		//DEVICE_DESCRIPTOR==01	设备描述符
  CONFIG_DESCRIPTOR,				//CONFIG_DESCRIPTOR==02	配置描述符
  STRING_DESCRIPTOR,				//STRING_DESCRIPTOR==03	字符串描述符
  INTERFACE_DESCRIPTOR,			//INTERFACE_DESCRIPTOR==04	接口描述符
  ENDPOINT_DESCRIPTOR				//ENDPOINT_DESCRIPTOR==05	端点描述符
} DESCRIPTOR_TYPE;

/* Feature selector of a SET_FEATURE or CLEAR_FEATURE */
typedef enum _FEATURE_SELECTOR
{
  ENDPOINT_STALL,
  DEVICE_REMOTE_WAKEUP
} FEATURE_SELECTOR;

/* Exported constants --------------------------------------------------------*/
/* Definition of "USBbmRequestType" */
#define REQUEST_TYPE      0x60  /* Mask to get request type */
#define STANDARD_REQUEST  0x00 	// STANDARD_REQUEST == 00 标准请求 /* Standard request */		// STANDARD_REQUEST == 00 标准请求
#define CLASS_REQUEST     0x20  /* Class request */
#define VENDOR_REQUEST    0x40  /* Vendor request */

#define RECIPIENT         0x1F  /* Mask to get recipient */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __USB_DEF_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
