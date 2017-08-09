/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_prop.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : All processing related to Virtual Com Port Demo
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


/******************************** 说明20160912**********************************
********************************************************************************
* 功能：用于上层协议处理，比如 HID协议，大容量存储设备协议
* 
* 
* 
* 
* 
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "hw_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 Request = 0;

LINE_CODING linecoding =
  {
    115200, /* baud rate*/
    0x00,   /* stop bits-1*/
    0x00,   /* parity - none*/
    0x08    /* no. of bits 8*/
  };

/* -------------------------------------------------------------------------- */
/*  Structures initializations */
/* -------------------------------------------------------------------------- */

DEVICE Device_Table =
  {
    EP_NUM,				//被使用的端点数
    1							//可以使用的端点数
  };

/*设备属性*/
DEVICE_PROP Device_Property =				//
  {
    Virtual_Com_Port_init,									//Virtual_Com_Port的初始化函数
    Virtual_Com_Port_Reset,									//Virtual_Com_Port的复位函数
    Virtual_Com_Port_Status_In,							//CustomVirtual_Com_PortHID状态输入函数
    Virtual_Com_Port_Status_Out,						//CustomHID状态输出函数
    Virtual_Com_Port_Data_Setup,						//CustomHID的处理有数据阶段的特殊类请求函数
    Virtual_Com_Port_NoData_Setup,					//CustomHID的处理没有数据阶段的特殊类请求函数
    Virtual_Com_Port_Get_Interface_Setting,	//CustomHID获取接口及备用接口设置（是否可用）
    Virtual_Com_Port_GetDeviceDescriptor,		//CustomHID获取设备描述符
    Virtual_Com_Port_GetConfigDescriptor,		//CustomHID获取配置描述符
    Virtual_Com_Port_GetStringDescriptor,		//CustomHID获取字符串描述符
    0,																			//当前库未使用
    0x40 /*MAX PACKET SIZE*/								//最大的包长度为64字节
  };

/*注册USB标准请求的实现函数*/
USER_STANDARD_REQUESTS User_Standard_Requests =		
  {
    Virtual_Com_Port_GetConfiguration,			//获取配置请求
    Virtual_Com_Port_SetConfiguration,			//设置配置请求
    Virtual_Com_Port_GetInterface,					//获取接口请求
    Virtual_Com_Port_SetInterface,					//设置接口请求
    Virtual_Com_Port_GetStatus,							//获取状态请求
    Virtual_Com_Port_ClearFeature,					//清除属性请求
    Virtual_Com_Port_SetEndPointFeature,		//设置端点属性请求
    Virtual_Com_Port_SetDeviceFeature,			//设置设备属性请求
    Virtual_Com_Port_SetDeviceAddress				//设置设备地址请求
  };

/*注册设备描述符信息*/
ONE_DESCRIPTOR Device_Descriptor =
  {
    (u8*)Virtual_Com_Port_DeviceDescriptor,	//注册设备描述符数组
    VIRTUAL_COM_PORT_SIZ_DEVICE_DESC				//设备描述符的长度
  };
	
/*注册设备描述符信息*/
ONE_DESCRIPTOR Qualifier_Descriptor =
  {
    (u8*)USBD_DeviceQualifier,	//注册设备描述符数组
    10				//设备描述符的长度
  };

/*注册配置描述符信息*/
ONE_DESCRIPTOR Config_Descriptor =
  {
    (u8*)Virtual_Com_Port_ConfigDescriptor,										//注册配置描述符数组
    VIRTUAL_COM_PORT_SIZ_CONFIG_DESC												//配置描述符的长度
  };

/*注册字符串描述符，包括语言ID、厂商、产品、序列号描述符*/
ONE_DESCRIPTOR String_Descriptor[4] =
  {
    {(u8*)Virtual_Com_Port_StringLangID, VIRTUAL_COM_PORT_SIZ_STRING_LANGID},		//注册语言字符串描述符数组
    {(u8*)Virtual_Com_Port_StringVendor, VIRTUAL_COM_PORT_SIZ_STRING_VENDOR},		//注册厂商字符串描述符数组
    {(u8*)Virtual_Com_Port_StringProduct, VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT},	//注册产品字符串描述符数组
    {(u8*)Virtual_Com_Port_StringSerial, VIRTUAL_COM_PORT_SIZ_STRING_SERIAL}		//注册序列号字符串描述符数组
  };

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : Virtual_Com_Port_init.
* Description    : Virtual COM Port Mouse init routine.	初始化
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_init(void)
{

  /* Update the serial number string descriptor with the data from the unique
  ID*/
  Get_SerialNum();	//**************************************获取序列号,设置芯片序列号，将描述符中的例如STM 等字符串修改没太大意义。

  pInformation->Current_Configuration = 0;	//配置状态

  /* Connect the device */
  PowerOn();				//*******上电usb_pwr.c->Line59:开启USB上拉，强制USB复位，开启相关中断使能标志；CNTR_RESETM | CNTR_SUSPM | CNTR_WKUPM;		//允许以下中断
  /* USB interrupts initialization */
  /* clear pending interrupts */
  _SetISTR(0);			//**************************************执行基本的初始化操作，比如说设备IP和端点0的初始化
  wInterrupt_Mask = IMR_MSK;		//#define IMR_MSK (CNTR_CTRM  | CNTR_SOFM  | CNTR_RESETM )
																//CNTR_CTRM：正确传输(CTR)中断屏蔽位 (Correct transfer interrupt mask)
																//CNTR_SOFM：帧首中断屏蔽位 (Start of frame interrupt mask)
																//CNTR_RESETM：USB复位中断屏蔽位 (USB reset interrupt mask)
  /* set interrupts mask */
  _SetCNTR(wInterrupt_Mask);		////使能相应中断

  /* configure the USART 1 to the default settings */
  USART_Config_Default();		//配置串口至缺省状态---在这里波特率被设为9600，并且允许了接收中断。发送中断没有允许。

  bDeviceState = UNCONNECTED;		//usb_pwr.h->DEVICE_STATE**************************将当前的状态定义为未连接状态
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_Reset
* Description    : Virtual_Com_Port Mouse reset routine	复位
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_Reset(void)
{
  /* Set Virtual_Com_Port DEVICE as not configured */
  pInformation->Current_Configuration = 0;				//设置当前的配置为0，表示没有配置过

  /* Current Feature initialization */
  pInformation->Current_Feature = Virtual_Com_Port_ConfigDescriptor[7];	//当前的属性，bmAttributes:设备的一些特性，0xc0表示自供电，不支持远程唤醒

  /* Set Virtual_Com_Port DEVICE with the default Interface*/
  pInformation->Current_Interface = 0;
  SetBTABLE(BTABLE_ADDRESS);

  /* Initialize Endpoint 0 */
	/* *****初始化端点0***** */
  SetEPType(ENDP0, EP_CONTROL);													//设置端点0为控制端点
  SetEPTxStatus(ENDP0, EP_TX_STALL);										//设置端点0发送延时
  SetEPRxAddr(ENDP0, ENDP0_RXADDR);											//设置端点0的接收缓冲区地址
  SetEPTxAddr(ENDP0, ENDP0_TXADDR);											//设置端点0的发送缓冲区地址
  Clear_Status_Out(ENDP0);															//清除端点0的状态
  SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);		//设置端点0的接收最大包
  SetEPRxValid(ENDP0);																	//使能接收状态

  /* Initialize Endpoint 1 */
	/* *****初始化端点1***** */
  SetEPType(ENDP1, EP_BULK);														//设置端点1为进批量传输
  SetEPTxAddr(ENDP1, ENDP1_TXADDR);											//设置端点发送地址
  SetEPTxStatus(ENDP1, EP_TX_NAK);											//设置端点1的发送不响应
  SetEPRxStatus(ENDP1, EP_RX_DIS);											//设置端点1不接收

  /* Initialize Endpoint 2 */
	/* *****初始化端点2***** */
  SetEPType(ENDP2, EP_INTERRUPT);												//设置端点2为中断传输
  SetEPTxAddr(ENDP2, ENDP2_TXADDR);											//设置端点2发送地址
  SetEPRxStatus(ENDP2, EP_RX_DIS);											//设置端点2不接收状态
  SetEPTxStatus(ENDP2, EP_TX_NAK);											//设置端点2端点2为接收不响应

  /* Initialize Endpoint 3 */
	/* *****初始化端点3***** */
  SetEPType(ENDP3, EP_BULK);														//设置端点3为仅批量传输
  SetEPRxAddr(ENDP3, ENDP3_RXADDR);											//设置端点3接收地址
  SetEPRxCount(ENDP3, VIRTUAL_COM_PORT_DATA_SIZE);			//设置端点3的计数值
  SetEPRxStatus(ENDP3, EP_RX_VALID);										//设置端点3接收有效
  SetEPTxStatus(ENDP3, EP_TX_DIS);											//设置端点3不发送

  /* Set this device to response on default address */
  SetDeviceAddress(0);																	//设置设备为默认地址为0

  bDeviceState = ATTACHED;
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_SetConfiguration.
* Description    : Udpade the device state to configured.更新设备配置状态
* 描述			    	: 配置
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_SetConfiguration(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_SetConfiguration.
* Description    : Udpade the device state to addressed.更新设备的编址状态
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_SetDeviceAddress (void)
{
  bDeviceState = ADDRESSED;
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_Status_In.
* Description    : Virtual COM Port Status In Routine.状态输入函数
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_Status_In(void)
{
  if (Request == SET_LINE_CODING)
  {
    USART_Config();
    Request = 0;
  }
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_Status_Out
* Description    : Virtual COM Port Status OUT Routine.状态输出函数
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_Status_Out(void)
{}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_Data_Setup
* Description    : handle the data class specific requests 处理有数据阶段的特殊类请求
* Input          : Request Nb.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
RESULT Virtual_Com_Port_Data_Setup(u8 RequestNo)
{
  u8    *(*CopyRoutine)(u16);

  CopyRoutine = NULL;
	//请求的字段
  if (RequestNo == GET_LINE_CODING)		//****************************获取串口通信信息请求														
  {
    if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))	//类请求，请求的接收者是接口
    {
      CopyRoutine = Virtual_Com_Port_GetLineCoding;	//**************指针函数指向Virtual_Com_Port_GetLineCoding函数
    }
  }
  else if (RequestNo == SET_LINE_CODING)	//************************设置串口通讯信息请求
  {
    if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))	//类请求，请求的接收者是接口
    {
      CopyRoutine = Virtual_Com_Port_SetLineCoding;	//**************指针函数指向Virtual_Com_Port_SetLineCoding函数
    }
    Request = SET_LINE_CODING;
  }

  if (CopyRoutine == NULL)
  {
    return USB_UNSUPPORT;
  }

  pInformation->Ctrl_Info.CopyData = CopyRoutine;		//**************注册指针指向的函数
  pInformation->Ctrl_Info.Usb_wOffset = 0;
  (*CopyRoutine)(0);					//************************************调用该函数
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_NoData_Setup.
* Description    : handle the no data class specific requests.处理没有数据阶段特殊类请求
* Input          : Request Nb.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
RESULT Virtual_Com_Port_NoData_Setup(u8 RequestNo)
{

  if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))	//***类请求，请求的接收者是接口
  {
    if (RequestNo == SET_COMM_FEATURE)		//*************************设置虚拟串口特性
    {
      return USB_SUCCESS;
    }
    else if (RequestNo == SET_CONTROL_LINE_STATE)		//***************设置控制信息状态
    {
      return USB_SUCCESS;
    }
  }

  return USB_UNSUPPORT;
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_GetDeviceDescriptor.
* Description    : Gets the device descriptor. 	数据类请求--获取设备描述符
* Input          : Length.
* Output         : None.
* Return         : The address of the device descriptor.
*******************************************************************************/
u8 *Virtual_Com_Port_GetDeviceDescriptor(u16 Length)
{
  return Standard_GetDescriptorData(Length, &Device_Descriptor);
}
/*******************************************************************************
* Function Name  : Virtual_Com_Port_GetDeviceDescriptor.
* Description    : Gets the device descriptor. 	数据类请求--获取设备描述符
* Input          : Length.
* Output         : None.
* Return         : The address of the device descriptor.
*******************************************************************************/
u8 *Qualifier_GetDeviceDescriptor(u16 Length)
{
  return Standard_GetDescriptorData(10,&Qualifier_Descriptor);
}
/*******************************************************************************
* Function Name  : Virtual_Com_Port_GetConfigDescriptor.
* Description    : get the configuration descriptor.数据类请求--获取配置描述符
* Input          : Length.
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
u8 *Virtual_Com_Port_GetConfigDescriptor(u16 Length)
{
  return Standard_GetDescriptorData(Length, &Config_Descriptor);
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_GetStringDescriptor
* Description    : Gets the string descriptors according to the needed index	数据类请求--根据索引获取字符描述符
* Input          : Length.
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
u8 *Virtual_Com_Port_GetStringDescriptor(u16 Length)
{
  u8 wValue0 = pInformation->USBwValue0;
  if (wValue0 > 4)
  {
    return NULL;
  }
  else
  {
    return Standard_GetDescriptorData(Length, &String_Descriptor[wValue0]);
  }
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_Get_Interface_Setting.
* Description    : test the interface and the alternate setting according to the
*                  supported one.		测试接口及其备用接口是否可用
* Input1         : u8: Interface : interface number.
* Input2         : u8: AlternateSetting : Alternate Setting number.
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
RESULT Virtual_Com_Port_Get_Interface_Setting(u8 Interface, u8 AlternateSetting)
{
  if (AlternateSetting > 0)		//备用的编号大于0
  {
    return USB_UNSUPPORT;
  }
  else if (Interface > 1)			//接口的编号大于1
  {
    return USB_UNSUPPORT;
  }
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_GetLineCoding.
* Description    : send the linecoding structure to the PC host.	发送linecoding结构体到PC机
* Input          : Length.
* Output         : None.
* Return         : Inecoding structure base address.  Linecoding结构体的基地址
*******************************************************************************/
u8 *Virtual_Com_Port_GetLineCoding(u16 Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = sizeof(linecoding);
    return NULL;
  }
  return(u8 *)&linecoding;			//串口通信信息
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_SetLineCoding.
* Description    : Set the linecoding structure fields.	设置linecoding结构体
* Input          : Length.
* Output         : None.
* Return         : Linecoding structure base address.		Linecoding结构体的基地址.
*******************************************************************************/
u8 *Virtual_Com_Port_SetLineCoding(u16 Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = sizeof(linecoding);
    return NULL;
  }
  return(u8 *)&linecoding;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

