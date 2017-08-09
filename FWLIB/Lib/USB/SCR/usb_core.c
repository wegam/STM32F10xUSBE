/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_core.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Standard protocol processing (USB v2.0)
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
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ValBit(VAR,Place)    (VAR & (1 << Place))
#define SetBit(VAR,Place)    (VAR |= (1 << Place))
#define ClrBit(VAR,Place)    (VAR &= ((1 << Place) ^ 255))

#define Send0LengthData() { _SetEPTxCount(ENDP0, 0); \
    vSetEPTxStatus(EP_TX_VALID); \
  }

#define vSetEPRxStatus(st) (SaveRState = st)
#define vSetEPTxStatus(st) (SaveTState = st)

#define USB_StatusIn() Send0LengthData()
#define USB_StatusOut() vSetEPRxStatus(EP_RX_VALID)

#define StatusInfo0 StatusInfo.bw.bb1 /* Reverse bb0 & bb1 */
#define StatusInfo1 StatusInfo.bw.bb0

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u16_u8 StatusInfo;
bool Data_Mul_MaxPacketSize = FALSE;
/* Private function prototypes -----------------------------------------------*/
static void DataStageOut(void);
static void DataStageIn(void);
static void NoData_Setup0(void);
static void Data_Setup0(void);
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Standard_GetConfiguration.
* Description    : Return the current configuration variable address.	//返回当前配置变量的地址
* Input          : Length - How many bytes are needed.	//需要的字节
* Output         : None.
* Return         : Return 1 , if the request is invalid when "Length" is 0.	//如果'Length'值为0时，请求无效，返回1。长度不为0时，返回配置变量的地址
*                  Return "Buffer" if the "Length" is not 0.								//当配置值与摸个配置描述符中的配置编号一致时，表示选中该配置
*******************************************************************************/
u8 *Standard_GetConfiguration(u16 Length)
{
  if (Length == 0)	//如果'Length'值为0时，请求无效，返回0	//长度不为0时，返回配置变量的地址
  {
    pInformation->Ctrl_Info.Usb_wLength =
      sizeof(pInformation->Current_Configuration);
    return 0;
  }
  pUser_Standard_Requests->User_GetConfiguration();
  return (u8 *)&pInformation->Current_Configuration;
}

/*******************************************************************************
* Function Name  : Standard_SetConfiguration.
* Description    : This routine is called to set the configuration value
*                  Then each class should configure device themself.	//该函数用于设置配置值，主机发送该请求后，usb设备要配置自己的设备值
* Input          : None.
* Output         : None.
* Return         : Return USB_SUCCESS, if the request is performed.		//当该请求被执行了，返回USB_SUCCESS
*                  Return USB_UNSUPPORT, if the request is invalid.		//当请求无效时，然会USB_UNSUPPORT
*******************************************************************************/
RESULT Standard_SetConfiguration(void)	//该函数用于设置配置值，主机发送该请求后，usb设备要配置自己的设备值
{

  if ((pInformation->USBwValue0 <=
      Device_Table.Total_Configuration) && (pInformation->USBwValue1 == 0)
      && (pInformation->USBwIndex == 0)) /*call Back usb spec 2.0*/
  {
    pInformation->Current_Configuration = pInformation->USBwValue0;
    pUser_Standard_Requests->User_SetConfiguration();
    return USB_SUCCESS;
  }
  else
  {
    return USB_UNSUPPORT;
  }
}

/*******************************************************************************
* Function Name  : Standard_GetInterface.
* Description    : Return the Alternate Setting of the current interface.	//返回当前接口的备用接口
* Input          : Length - How many bytes are needed.		//需要的字节数
* Output         : None.
* Return         : Return 0, if the request is invalid when "Length" is 0.	//当'Length'的值为0，请求无效，返回0，当长度不为0，返回备用接口的地址
*                  Return "Buffer" if the "Length" is not 0.
*******************************************************************************/
u8 *Standard_GetInterface(u16 Length)
{
  if (Length == 0)	//当'Length'的值为0，请求无效，返回0，当长度不为0，返回备用接口的地址
  {
    pInformation->Ctrl_Info.Usb_wLength =
      sizeof(pInformation->Current_AlternateSetting);
    return 0;
  }
  pUser_Standard_Requests->User_GetInterface();
  return (u8 *)&pInformation->Current_AlternateSetting;
}

/*******************************************************************************
* Function Name  : Standard_SetInterface.
* Description    : This routine is called to set the interface.
*                  Then each class should configure the interface them self.
* Input          : None.
* Output         : None.
* Return         : - Return USB_SUCCESS, if the request is performed.
*                  - Return USB_UNSUPPORT, if the request is invalid.
*******************************************************************************/
RESULT Standard_SetInterface(void)
{
  RESULT Re;
  /*Test if the specified Interface and Alternate Setting are supported by
    the application Firmware*/
  Re = (*pProperty->Class_Get_Interface_Setting)(pInformation->USBwIndex0, pInformation->USBwValue0);

  if (pInformation->Current_Configuration != 0)
  {
    if ((Re != USB_SUCCESS) || (pInformation->USBwIndex1 != 0)
        || (pInformation->USBwValue1 != 0))
    {
      return  USB_UNSUPPORT;
    }
    else if (Re == USB_SUCCESS)
    {
      pUser_Standard_Requests->User_SetInterface();
      pInformation->Current_Interface = pInformation->USBwIndex0;
      pInformation->Current_AlternateSetting = pInformation->USBwValue0;
      return USB_SUCCESS;
    }

  }

  return USB_UNSUPPORT;
}

/*******************************************************************************
* Function Name  : Standard_GetStatus.
* Description    : Copy the device request data to "StatusInfo buffer".	//拷贝设备请求数据到"StatusInfo buffer".
* Input          : - Length - How many bytes are needed.								//需要的字节数
* Output         : None.
* Return         : Return 0, if the request is at end of data block,		//如果请求在数据块的末端，或则当'Length'为0是，请求无效，则返回0
*                  or is invalid when "Length" is 0.
*******************************************************************************/
u8 *Standard_GetStatus(u16 Length)
{
  if (Length == 0)		//如果请求在数据块的末端，或则当'Length'为0是，请求无效，则返回0
  {
    pInformation->Ctrl_Info.Usb_wLength = 2;
    return 0;
  }
	//拷贝设备请求数据到"StatusInfo buffer".
  StatusInfo.w = 0;
  /* Reset Status Information */

  if (Type_Recipient == (STANDARD_REQUEST | DEVICE_RECIPIENT))
  {
    /*Get Device Status */
    u8 Feature = pInformation->Current_Feature;

    /* Remote Wakeup enabled */
    if (ValBit(Feature, 5))
    {
      SetBit(StatusInfo0, 1);
    }

    /* Bus-powered */
    if (ValBit(Feature, 6))
    {
      ClrBit(StatusInfo0, 0);
    }
    else /* Self-powered */
    {
      SetBit(StatusInfo0, 0);
    }
  }
  /*Interface Status*/
  else if (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))
  {
    return (u8 *)&StatusInfo;
  }
  /*Get EndPoint Status*/
  else if (Type_Recipient == (STANDARD_REQUEST | ENDPOINT_RECIPIENT))
  {
    u8 Related_Endpoint;
    u8 wIndex0 = pInformation->USBwIndex0;

    Related_Endpoint = (wIndex0 & 0x0f);
    if (ValBit(wIndex0, 7))
    {
      /* IN endpoint */
      if (_GetTxStallStatus(Related_Endpoint))
      {
        SetBit(StatusInfo0, 0); /* IN Endpoint stalled */
      }
    }
    else
    {
      /* OUT endpoint */
      if (_GetRxStallStatus(Related_Endpoint))
      {
        SetBit(StatusInfo0, 0); /* OUT Endpoint stalled */
      }
    }

  }
  else
  {
    return NULL;
  }
  pUser_Standard_Requests->User_GetStatus();
  return (u8 *)&StatusInfo;
}

/*******************************************************************************
* Function Name  : Standard_ClearFeature.															//清除或禁止某个指定的特性
* Description    : Clear or disable a specific feature.
* Input          : None.
* Output         : None.
* Return         : - Return USB_SUCCESS, if the request is performed.
*                  - Return USB_UNSUPPORT, if the request is invalid.	//当请求被执行，返回Return USB_SUCCESS,如果请求无效，返回USB_UNSUPPORT
*******************************************************************************/
RESULT Standard_ClearFeature(void)
{
  u32     Type_Rec = Type_Recipient;
  u32     Status;

	
  if (Type_Rec == (STANDARD_REQUEST | DEVICE_RECIPIENT))
  {/*Device Clear Feature*/
    ClrBit(pInformation->Current_Feature, 5);
    return USB_SUCCESS;			//当请求被执行，返回Return USB_SUCCESS,如果请求无效，返回USB_UNSUPPORT
  }
  else if (Type_Rec == (STANDARD_REQUEST | ENDPOINT_RECIPIENT))
  {/*EndPoint Clear Feature*/
    DEVICE* pDev;
    u32 Related_Endpoint;
    u32 wIndex0;
    u32 rEP;

    if ((pInformation->USBwValue != ENDPOINT_STALL)
        || (pInformation->USBwIndex1 != 0))
    {
      return USB_UNSUPPORT;
    }

    pDev = &Device_Table;
    wIndex0 = pInformation->USBwIndex0;
    rEP = wIndex0 & ~0x80;
    Related_Endpoint = ENDP0 + rEP;

    if (ValBit(pInformation->USBwIndex0, 7))
    {
      /*Get Status of endpoint & stall the request if the related_ENdpoint
      is Disabled*/
      Status = _GetEPTxStatus(Related_Endpoint);
    }
    else
    {
      Status = _GetEPRxStatus(Related_Endpoint);
    }

    if ((rEP >= pDev->Total_Endpoint) || (Status == 0)
        || (pInformation->Current_Configuration == 0))
    {
      return USB_UNSUPPORT;
    }


    if (wIndex0 & 0x80)
    {
      /* IN endpoint */
      if (_GetTxStallStatus(Related_Endpoint ))
      {
        ClearDTOG_TX(Related_Endpoint);
        SetEPTxStatus(Related_Endpoint, EP_TX_VALID);
      }
    }
    else
    {
      /* OUT endpoint */
      if (_GetRxStallStatus(Related_Endpoint))
      {
        if (Related_Endpoint == ENDP0)
        {
          /* After clear the STALL, enable the default endpoint receiver */
          SetEPRxCount(Related_Endpoint, Device_Property.MaxPacketSize);
          _SetEPRxStatus(Related_Endpoint, EP_RX_VALID);
        }
        else
        {
          ClearDTOG_RX(Related_Endpoint);
          _SetEPRxStatus(Related_Endpoint, EP_RX_VALID);
        }
      }
    }
    pUser_Standard_Requests->User_ClearFeature();
    return USB_SUCCESS;
  }

  return USB_UNSUPPORT;
}

/*******************************************************************************
* Function Name  : Standard_SetEndPointFeature												//设置或使能端点的指定的特性
* Description    : Set or enable a specific feature of EndPoint
* Input          : None.
* Output         : None.
* Return         : - Return USB_SUCCESS, if the request is performed.
*                  - Return USB_UNSUPPORT, if the request is invalid.	//如果请求被执行，返回USB_SUCCESS,如果请求无效，则返回USB_UNSUPPORT
*******************************************************************************/
RESULT Standard_SetEndPointFeature(void)
{
  u32    wIndex0;
  u32    Related_Endpoint;
  u32    rEP;
  u32   Status;

  wIndex0 = pInformation->USBwIndex0;
  rEP = wIndex0 & ~0x80;
  Related_Endpoint = ENDP0 + rEP;

  if (ValBit(pInformation->USBwIndex0, 7))
  {
    /* get Status of endpoint & stall the request if the related_ENdpoint
    is Disabled*/
    Status = _GetEPTxStatus(Related_Endpoint);
  }
  else
  {
    Status = _GetEPRxStatus(Related_Endpoint);
  }

  if (Related_Endpoint >= Device_Table.Total_Endpoint
      || pInformation->USBwValue != 0 || Status == 0
      || pInformation->Current_Configuration == 0)
  {
    return USB_UNSUPPORT;		//如果请求被执行，返回USB_SUCCESS,如果请求无效，则返回USB_UNSUPPORT
  }
  else
  {
    if (wIndex0 & 0x80)
    {
      /* IN endpoint */
      _SetEPTxStatus(Related_Endpoint, EP_TX_STALL);
    }

    else
    {
      /* OUT endpoint */
      _SetEPRxStatus(Related_Endpoint, EP_RX_STALL);
    }
  }
  pUser_Standard_Requests->User_SetEndPointFeature();
  return USB_SUCCESS;			//如果请求被执行，返回USB_SUCCESS,如果请求无效，则返回USB_UNSUPPORT
}

/*******************************************************************************
* Function Name  : Standard_SetDeviceFeature.										
* Description    : Set or enable a specific feature of Device.	//设置或使能设备指定的特性
* Input          : None.
* Output         : None.
* Return         : - Return USB_SUCCESS, if the request is performed.
*                  - Return USB_UNSUPPORT, if the request is invalid.		//如果请求被执行，返回USB_SUCCESS,如果请求无效，则返回USB_UNSUPPORT
*******************************************************************************/
RESULT Standard_SetDeviceFeature(void)
{
  SetBit(pInformation->Current_Feature, 5);
  pUser_Standard_Requests->User_SetDeviceFeature();
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Standard_GetDescriptorData.
* Description    : Standard_GetDescriptorData is used for descriptors transfer.
*                : This routine is used for the descriptors resident in Flash
*                  or RAM
*                  pDesc can be in either Flash or RAM
*                  The purpose of this routine is to have a versatile way to
*                  response descriptors request. It allows user to generate
*                  certain descriptors with software or read descriptors from
*                  external storage part by part.
* Input          : - Length - Length of the data in this transfer.
*                  - pDesc - A pointer points to descriptor struct.
*                  The structure gives the initial address of the descriptor and
*                  its original size.
* Output         : None.
* Return         : Address of a part of the descriptor pointed by the Usb_
*                  wOffset The buffer pointed by this address contains at least
*                  Length bytes.
*******************************************************************************/
u8 *Standard_GetDescriptorData(u16 Length, ONE_DESCRIPTOR *pDesc)
{
  u32  wOffset;

  wOffset = pInformation->Ctrl_Info.Usb_wOffset;
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = pDesc->Descriptor_Size - wOffset;
    return 0;
  }

  return pDesc->Descriptor + wOffset;
}

/*******************************************************************************
* Function Name  : DataStageOut.
* Description    : Data stage of a Control Write Transfer.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void DataStageOut(void)
{
  ENDPOINT_INFO *pEPinfo = &pInformation->Ctrl_Info;
  u32 save_rLength;

  save_rLength = pEPinfo->Usb_rLength;

  if (pEPinfo->CopyData && save_rLength)
  {
    u8 *Buffer;
    u32 Length;

    Length = pEPinfo->PacketSize;		//获取数据长度
    if (Length > save_rLength)
    {
      Length = save_rLength;
    }

    Buffer = (*pEPinfo->CopyData)(Length);
    pEPinfo->Usb_rLength -= Length;
    pEPinfo->Usb_rOffset += Length;

    PMAToUserBufferCopy(Buffer, GetEPRxAddr(ENDP0), Length);	//复制数据
  }

  if (pEPinfo->Usb_rLength != 0)
  {
    vSetEPRxStatus(EP_RX_VALID);/* re-enable for next data reception */
    SetEPTxCount(ENDP0, 0);			//复位缓冲区准备接收下一个数据
    vSetEPTxStatus(EP_TX_VALID);/* Expect the host to abort the data OUT stage */
  }
  /* Set the next State*/
  if (pEPinfo->Usb_rLength >= pEPinfo->PacketSize)
  {
    pInformation->ControlState = OUT_DATA;
  }
  else
  {
    if (pEPinfo->Usb_rLength > 0)
    {
      pInformation->ControlState = LAST_OUT_DATA;
    }
    else if (pEPinfo->Usb_rLength == 0)
    {
      pInformation->ControlState = WAIT_STATUS_IN;
      USB_StatusIn();
    }
  }
}

/*******************************************************************************
* Function Name  : DataStageIn.
* Description    : Data stage of a Control Read Transfer.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void DataStageIn(void)
{
  ENDPOINT_INFO *pEPinfo = &pInformation->Ctrl_Info;
  u32 save_wLength = pEPinfo->Usb_wLength;
  u32 ControlState = pInformation->ControlState;

  u8 *DataBuffer;
  u32 Length;

  if ((save_wLength == 0) && (ControlState == LAST_IN_DATA))
  {
    if(Data_Mul_MaxPacketSize == TRUE)
    {
      /* No more data to send and empty packet */
      Send0LengthData();
      ControlState = LAST_IN_DATA;
      Data_Mul_MaxPacketSize = FALSE;
    }
    else 
    {
      /* No more data to send so STALL the TX Status*/
      ControlState = WAIT_STATUS_OUT;
      vSetEPTxStatus(EP_TX_STALL);
    }
    
    goto Expect_Status_Out;
  }

  Length = pEPinfo->PacketSize;			//*************得到数据包的长度
  ControlState = (save_wLength <= Length) ? LAST_IN_DATA : IN_DATA;		//比较大小得到是LAST_IN_DATA还是IN_DATA    18字节<64字节  ControlState = LAST_IN_DATA

  if (Length > save_wLength)
  {
    Length = save_wLength;
  }

  DataBuffer = (*pEPinfo->CopyData)(Length);				//复制数据到DataBuffer

  UserToPMABufferCopy(DataBuffer, GetEPTxAddr(ENDP0), Length);	//将数据发送到PMA缓冲区--发送数据

  SetEPTxCount(ENDP0, Length);		//设置发送数据长度

  pEPinfo->Usb_wLength -= Length;
  pEPinfo->Usb_wOffset += Length;
  vSetEPTxStatus(EP_TX_VALID);		//使能发送

  USB_StatusOut();/* Expect the host to abort the data IN stage */	//

Expect_Status_Out:
  pInformation->ControlState = ControlState;
}

/*******************************************************************************
* Function Name  : NoData_Setup0.
* Description    : Proceed the processing of setup request without data stage.//不带数据返回请求的配置处理
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void NoData_Setup0(void)
{
  RESULT Result = USB_UNSUPPORT;
  u32 RequestNo = pInformation->USBbRequest;
  u32 ControlState;

  if (Type_Recipient == (STANDARD_REQUEST | DEVICE_RECIPIENT))	// STANDARD_REQUEST == 00 标准请求		//DEVICE_RECIPIENT==00	接收设备
  {
    /* Device Request*/
    /* SET_CONFIGURATION*/
    if (RequestNo == SET_CONFIGURATION)			//SET_CONFIGURATION==09用于主机指示设备采用的要求的配置
    {
      Result = Standard_SetConfiguration();
    }

    /*SET ADDRESS*/
    else if (RequestNo == SET_ADDRESS)			//SET_ADDRESS==05用来给设备分配地址
    {
      if ((pInformation->USBwValue0 > 127) || (pInformation->USBwValue1 != 0)
          || (pInformation->USBwIndex != 0)
          || (pInformation->Current_Configuration != 0))
        /* Device Address should be 127 or less*/
      {
        ControlState = STALLED;
        goto exit_NoData_Setup0;
      }
      else
      {
        Result = USB_SUCCESS;
      }
    }
    /*SET FEATURE for Device*/
    else if (RequestNo == SET_FEATURE)	//SET_FEATURE==03用来启用或激活命令接收者的某些特性	
    {
      if ((pInformation->USBwValue0 == DEVICE_REMOTE_WAKEUP)
          && (pInformation->USBwIndex == 0)
          && (ValBit(pInformation->Current_Feature, 5)))
      {
        Result = Standard_SetDeviceFeature();
      }
      else
      {
        Result = USB_UNSUPPORT;
      }
    }
    /*Clear FEATURE for Device */
    else if (RequestNo == CLEAR_FEATURE)	//CLEAR_FEATURE==01用来清除或禁止接收者的某些特性
    {
      if (pInformation->USBwValue0 == DEVICE_REMOTE_WAKEUP
          && pInformation->USBwIndex == 0
          && ValBit(pInformation->Current_Feature, 5))
      {
        Result = Standard_ClearFeature();
      }
      else
      {
        Result = USB_UNSUPPORT;
      }
    }

  }

  /* Interface Request*/
  else if (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))	// STANDARD_REQUEST == 00 标准请求	//INTERFACE_RECIPIENT==01
  {
    /*SET INTERFACE*/
    if (RequestNo == SET_INTERFACE)			//SET_INTERFACE==0B用于主机要求设备用某个描述符来描述接口
    {
      Result = Standard_SetInterface();
    }
  }

  /* EndPoint Request*/
  else if (Type_Recipient == (STANDARD_REQUEST | ENDPOINT_RECIPIENT))		// STANDARD_REQUEST == 00 标准请求	//ENDPOINT_RECIPIENT==02
  {
    /*CLEAR FEATURE for EndPoint*/
    if (RequestNo == CLEAR_FEATURE)		//CLEAR_FEATURE==01用来清除或禁止接收者的某些特性
    {
      Result = Standard_ClearFeature();
    }
    /* SET FEATURE for EndPoint*/
    else if (RequestNo == SET_FEATURE)	//SET_FEATURE==03用来启用或激活命令接收者的某些特性
    {
      Result = Standard_SetEndPointFeature();
    }
  }
  else
  {
    Result = USB_UNSUPPORT;
  }


  if (Result != USB_SUCCESS)
  {
    Result = (*pProperty->Class_NoData_Setup)(RequestNo);
    if (Result == USB_NOT_READY)
    {
      ControlState = PAUSE;
      goto exit_NoData_Setup0;
    }
  }

  if (Result != USB_SUCCESS)
  {
    ControlState = STALLED;
    goto exit_NoData_Setup0;
  }

  ControlState = WAIT_STATUS_IN;/* After no data stage SETUP */

  USB_StatusIn();

exit_NoData_Setup0:
  pInformation->ControlState = ControlState;
  return;
}

/*******************************************************************************
* Function Name  : Data_Setup0.
* Description    : Proceed the processing of setup request with data stage.//带数据返回请求的配置处理
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Data_Setup0(void)
{
  u8 *(*CopyRoutine)(u16);										//这是一个函数指针定义。
  RESULT Result;
  u32 Request_No = pInformation->USBbRequest;

  u32 Related_Endpoint, Reserved;
  u32 wOffset, Status;



  CopyRoutine = NULL;			//这是一个函数指针，由用户提供。
  wOffset = 0;

  if (Request_No == GET_DESCRIPTOR)			//GET_DESCRIPTOR==06用于主机获取设备的特定描述符
  {
    if (Type_Recipient == (STANDARD_REQUEST | DEVICE_RECIPIENT))		//标准请求|设备接收
    {
      u8 wValue1 = pInformation->USBwValue1;
      if (wValue1 == DEVICE_DESCRIPTOR)								//DEVICE_DESCRIPTOR==01	设备描述符
      {
        CopyRoutine = pProperty->GetDeviceDescriptor;	//设备描述符	usb_prop.c->Device_Property->Virtual_Com_Port_GetDeviceDescriptor->Device_Descriptor->Virtual_Com_Port_DeviceDescriptor
      }
      else if (wValue1 == CONFIG_DESCRIPTOR)					//CONFIG_DESCRIPTOR==02	配置描述符
      {
        CopyRoutine = pProperty->GetConfigDescriptor;	//配置描述符	usb_prop.c->Device_Property->Virtual_Com_Port_GetConfigDescriptor->Config_Descriptor->Virtual_Com_Port_ConfigDescriptor
      }
      else if (wValue1 == STRING_DESCRIPTOR)					//STRING_DESCRIPTOR==03	字符串描述符
      {
        CopyRoutine = pProperty->GetStringDescriptor;	//字符串描述符	usb_prop.c->Device_Property->Virtual_Com_Port_GetStringDescriptor->String_Descriptor
      }
			else if (wValue1 == 06)					//CONFIG_DESCRIPTOR==06	配置描述符
      {
//        CopyRoutine = Qualifier_GetDeviceDescriptor;	//配置描述符	usb_prop.c->Device_Property->Virtual_Com_Port_GetConfigDescriptor->Config_Descriptor->Virtual_Com_Port_ConfigDescriptor
				CopyRoutine = pProperty->GetDeviceDescriptor;		//配置描述符	usb_prop.c->Device_Property->Virtual_Com_Port_GetConfigDescriptor->Config_Descriptor->Virtual_Com_Port_ConfigDescriptor
			}
      			/* End of GET_DESCRIPTOR */
    }
  }

  /*GET STATUS*/		//GET_STATUS==00用来返回特定接收者的状态
  else if ((Request_No == GET_STATUS) && (pInformation->USBwValue == 0)
           && (pInformation->USBwLength == 0x0002)
           && (pInformation->USBwIndex1 == 0))
  {
    /* GET STATUS for Device*/
    if ((Type_Recipient == (STANDARD_REQUEST | DEVICE_RECIPIENT))						//获取设备状态
        && (pInformation->USBwIndex == 0))
    {
      CopyRoutine = Standard_GetStatus;		//Standard_GetStatus为函数指针：usb_core.c->Line158:u8 *Standard_GetStatus(u16 Length)
    }

    /* GET STATUS for Interface*/
    else if (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))		//获取接口状态
    {
      if (((*pProperty->Class_Get_Interface_Setting)(pInformation->USBwIndex0, 0) == USB_SUCCESS)
          && (pInformation->Current_Configuration != 0))
      {
        CopyRoutine = Standard_GetStatus;		//Standard_GetStatus为函数指针：usb_core.c->Line158:u8 *Standard_GetStatus(u16 Length)
      }
    }

    /* GET STATUS for EndPoint*/
    else if (Type_Recipient == (STANDARD_REQUEST | ENDPOINT_RECIPIENT))			//获取端点状态
    {
      Related_Endpoint = (pInformation->USBwIndex0 & 0x0f);			//获取端点号
      Reserved = pInformation->USBwIndex0 & 0x70;

      if (ValBit(pInformation->USBwIndex0, 7))
      {
        /*Get Status of endpoint & stall the request if the related_ENdpoint
        is Disabled*/
        Status = _GetEPTxStatus(Related_Endpoint);
      }
      else
      {
        Status = _GetEPRxStatus(Related_Endpoint);
      }

      if ((Related_Endpoint < Device_Table.Total_Endpoint) && (Reserved == 0)
          && (Status != 0))
      {
        CopyRoutine = Standard_GetStatus;
      }
    }

  }

  /*GET CONFIGURATION*/		//GET_CONFIGURATION==08用于主机获取设备当前设备的配置值（注同上面的不同）
  else if (Request_No == GET_CONFIGURATION)
  {
    if (Type_Recipient == (STANDARD_REQUEST | DEVICE_RECIPIENT))
    {
      CopyRoutine = Standard_GetConfiguration;
    }
  }
  /*GET INTERFACE*/		//GET_INTERFACE==0A用于获取当前某个接口描述符编号
  else if (Request_No == GET_INTERFACE)
  {
    if ((Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))
        && (pInformation->Current_Configuration != 0) && (pInformation->USBwValue == 0)
        && (pInformation->USBwIndex1 == 0) && (pInformation->USBwLength == 0x0001)
        && ((*pProperty->Class_Get_Interface_Setting)(pInformation->USBwIndex0, 0) == USB_SUCCESS))
    {
      CopyRoutine = Standard_GetInterface;
    }

  }
  
  if (CopyRoutine)
  {
    pInformation->Ctrl_Info.Usb_wOffset = wOffset;
    pInformation->Ctrl_Info.CopyData = CopyRoutine;
    /* sb in the original the cast to word was directly */
    /* now the cast is made step by step */
    (*CopyRoutine)(0);
    Result = USB_SUCCESS;
  }
  else
  {
    Result = (*pProperty->Class_Data_Setup)(pInformation->USBbRequest);
    if (Result == USB_NOT_READY)
    {
      pInformation->ControlState = PAUSE;
      return;
    }
  }

  if (pInformation->Ctrl_Info.Usb_wLength == 0xFFFF)
  {
    /* Data is not ready, wait it */
    pInformation->ControlState = PAUSE;
    return;
  }
  if ((Result == USB_UNSUPPORT) || (pInformation->Ctrl_Info.Usb_wLength == 0))
  {
    /* Unsupported request */
    pInformation->ControlState = STALLED;
    return;
  }


  if (ValBit(pInformation->USBbmRequestType, 7))
  {
    /* Device ==> Host */		//上面这个语句主要是判断传输方向。如果为 1，则是设备到主机
    vu32 wLength = pInformation->USBwLength;		// 这个一般是 64
     
    /* Restrict the data length to be the one host asks */
    if (pInformation->Ctrl_Info.Usb_wLength > wLength)	//设备描述符长度 18
    {
      pInformation->Ctrl_Info.Usb_wLength = wLength;
    }
    
    else if (pInformation->Ctrl_Info.Usb_wLength < pInformation->USBwLength)
    {
      if (pInformation->Ctrl_Info.Usb_wLength < pProperty->MaxPacketSize)
      {
        Data_Mul_MaxPacketSize = FALSE;
      }
      else if ((pInformation->Ctrl_Info.Usb_wLength % pProperty->MaxPacketSize) == 0)
      {
        Data_Mul_MaxPacketSize = TRUE;
      }
    }   

    pInformation->Ctrl_Info.PacketSize = pProperty->MaxPacketSize;
    DataStageIn();
  }
  else
  {
    pInformation->ControlState = OUT_DATA;
    vSetEPRxStatus(EP_RX_VALID); /* enable for next data reception */
  }

  return;
}

/*******************************************************************************
* Function Name  : Setup0_Process
* Description    : Get the device request data and dispatch to individual process.
* Input          : None.
* Output         : None.
* Return         : Post0_Process.
*******************************************************************************/
u8 Setup0_Process(void)
{

  union
  {
    u8* b;
    u16* w;
  } pBuf;
	/*PMAAddr是包缓冲区起始地址，_GetEPRxAddr(ENDP0)获得端点 0描述符表里的接收缓冲区地址，为什么要乘以2 呢？大概因为描述符表里地址项为16 位，
	使用的是相对偏移。*/
  pBuf.b = PMAAddr + (u8 *)(_GetEPRxAddr(ENDP0) * 2); /* *2 for 32 bits addr */	////这是取得端点0 接收缓冲区的起始地址。
	//更新请状态信息
  if (pInformation->ControlState != PAUSE)
  {
    pInformation->USBbmRequestType = *pBuf.b++; /* bmRequestType */		//请求类型，表明方向和接收对象（设备、接口还是端点）此时为 80，表明设备到主机
    pInformation->USBbRequest = *pBuf.b++; /* bRequest */							//请求代码，第一次时应该是 6，表明主机要获取设备描述符。
    pBuf.w++;  /* word not accessed because of 32 bits addressing */
    pInformation->USBwValue = ByteSwap(*pBuf.w++); /* wValue */
    pBuf.w++;  /* word not accessed because of 32 bits addressing */
    pInformation->USBwIndex  = ByteSwap(*pBuf.w++); /* wIndex */
    pBuf.w++;  /* word not accessed because of 32 bits addressing */
    pInformation->USBwLength = *pBuf.w; /* wLength */
  }

  pInformation->ControlState = SETTING_UP;
  if (pInformation->USBwLength == 0)
  {
    /* Setup with no data stage */
    NoData_Setup0();		//建立空数据包		//无数据传输
  }
  else
  {
    /* Setup with data stage */
    Data_Setup0();		//usb_core.c->Line666://建立数据包	////这次是有数据传输的，所以有进入该该函数。
  }
  return Post0_Process();
}

/*******************************************************************************
* Function Name  : In0_Process
* Description    : Process the IN token on all default endpoint.
* Input          : None.
* Output         : None.
* Return         : Post0_Process.
*******************************************************************************/
u8 In0_Process(void)
{
  u32 ControlState = pInformation->ControlState;		//复制ControlState
	//判断ControlState---主机请求输入数据
  if ((ControlState == IN_DATA) || (ControlState == LAST_IN_DATA))	//判断USB工作状态
  {
    DataStageIn();				//数据写入发送寄存器
    /* ControlState may be changed outside the function */
    ControlState = pInformation->ControlState;
  }

  else if (ControlState == WAIT_STATUS_IN)		//设置地址
  {
    if ((pInformation->USBbRequest == SET_ADDRESS) &&
        (Type_Recipient == (STANDARD_REQUEST | DEVICE_RECIPIENT)))
    {
      SetDeviceAddress(pInformation->USBwValue0);
      pUser_Standard_Requests->User_SetDeviceAddress();
    }
    (*pProperty->Process_Status_IN)();
    ControlState = STALLED;
  }

  else
  {
    ControlState = STALLED;
  }

  pInformation->ControlState = ControlState;

  return Post0_Process();			//发送
}

/*******************************************************************************
* Function Name  : Out0_Process
* Description    : Process the OUT token on all default endpoint.
* Input          : None.
* Output         : None.
* Return         : Post0_Process.
*******************************************************************************/
u8 Out0_Process(void)
{
  u32 ControlState = pInformation->ControlState;

  if ((ControlState == OUT_DATA) || (ControlState == LAST_OUT_DATA))
  {
    DataStageOut();			//从PMA拷贝数据
    ControlState = pInformation->ControlState; /* may be changed outside the function */
  }

  else if (ControlState == WAIT_STATUS_OUT)
  {
    (*pProperty->Process_Status_OUT)();
    ControlState = STALLED;
  }

  else if ((ControlState == IN_DATA) || (ControlState == LAST_IN_DATA))
  {
    /* host aborts the transfer before finish */
    ControlState = STALLED;
  }

  /* Unexpect state, STALL the endpoint */
  else
  {
    ControlState = STALLED;
  }

  pInformation->ControlState = ControlState;

  return Post0_Process();
}

/*******************************************************************************
* Function Name  : Post0_Process
* Description    : Stall the Endpoint 0 in case of error.
* Input          : None.
* Output         : None.
* Return         : - 0 if the control State is in PAUSE
*                  - 1 if not.
*******************************************************************************/
u8 Post0_Process(void)
{
  SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);

  if (pInformation->ControlState == STALLED)
  {
    vSetEPRxStatus(EP_RX_STALL);
    vSetEPTxStatus(EP_TX_STALL);
  }

  return (pInformation->ControlState == PAUSE);
}

/*******************************************************************************
* Function Name  : SetDeviceAddress.
* Description    : Set the device and all the used Endpoints addresses.
* Input          : - Val: device adress.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetDeviceAddress(u8 Val)
{
  u32 i;
  u32 nEP = Device_Table.Total_Endpoint;

  /* set address in every used endpoint */
  for (i = 0; i < nEP; i++)
  {
    _SetEPAddress((u8)i, (u8)i);
  } /* for */
  _SetDADDR(Val | DADDR_EF); /* set device address and enable function */
}

/*******************************************************************************
* Function Name  : NOP_Process
* Description    : No operation function.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void NOP_Process(void)
{
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
