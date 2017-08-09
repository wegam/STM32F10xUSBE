/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_core.h
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Standard protocol processing functions prototypes
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_CORE_H
#define __USB_CORE_H
#include "usb_type.h"			//主要是用 typedef为 stm32支持的数据类型取一些新的名称/
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum _CONTROL_STATE
{
  WAIT_SETUP,       /* 0 */		//等待SETUP
  SETTING_UP,       /* 1 */		//正在SETUP
  IN_DATA,          /* 2 */
  OUT_DATA,         /* 3 */
  LAST_IN_DATA,     /* 4 */		//最后的数据
  LAST_OUT_DATA,    /* 5 */		//最后的数据
  WAIT_STATUS_IN,   /* 7 */		//等待状态
  WAIT_STATUS_OUT,  /* 8 */		//等待状态
  STALLED,          /* 9 */		//终止发送和接受
  PAUSE             /* 10 */
} CONTROL_STATE;    /* The state machine states of a control pipe */

typedef struct OneDescriptor
{
  u8 *Descriptor;
  u16 Descriptor_Size;
}
ONE_DESCRIPTOR, *PONE_DESCRIPTOR;
/* All the request process routines return a value of this type
   If the return value is not SUCCESS or NOT_READY,
   the software will STALL the correspond endpoint */
typedef enum _RESULT
{
  USB_SUCCESS = 0,    /* Process sucessfully */			//请求成功进行
  USB_ERROR,																				//错误
  USB_UNSUPPORT,																		//请求不支持
  USB_NOT_READY       /* The process has not been finished, endpoint will be
                         NAK to further rquest */		//请求过程还没有完成,
} RESULT;


/*-*-*-*-*-*-*-*-*-*-* Definitions for endpoint level -*-*-*-*-*-*-*-*-*-*-*-*/
typedef struct _ENDPOINT_INFO
{
  /* When send data out of the device,
   CopyData() is used to get data buffer 'Length' bytes data
   if Length is 0,
    CopyData() returns the total length of the data
    if the request is not supported, returns 0
    (NEW Feature )
     if CopyData() returns -1, the calling routine should not proceed
     further and will resume the SETUP process by the class device
   if Length is not 0,
    CopyData() returns a pointer to indicate the data location
   Usb_wLength is the data remain to be sent,
   Usb_wOffset is the Offset of original data
  When receive data from the host,
   CopyData() is used to get user data buffer which is capable
   of Length bytes data to copy data from the endpoint buffer.
   if Length is 0,
    CopyData() returns the available data length,
   if Length is not 0,
    CopyData() returns user buffer address
   Usb_rLength is the data remain to be received,
   Usb_rPointer is the Offset of data buffer
  */
  u16  Usb_wLength;
  u16  Usb_wOffset;
  u16  PacketSize;
  u8   *(*CopyData)(u16 Length);	//这是一个函数指针定义。
}ENDPOINT_INFO;

/*-*-*-*-*-*-*-*-*-*-*-* Definitions for device level -*-*-*-*-*-*-*-*-*-*-*-*/
//内核将设备级信息保存在设备表结构中，设备表结构是DEVICE 类型，类型定义如下：
typedef struct _DEVICE		//设备表结构--内核将设备级信息保存在设备表结构中
{
  u8 Total_Endpoint;     /* Number of endpoints that are used */	//代表USB 应用使用的端点数量
  u8 Total_Configuration;/* Number of configuration available */	//代表USB 应用使用的配置数量
}
DEVICE;

typedef union		//为了简化对DEVICE_INFO 结构中的某些字段的访问（以u16 或u8 格式）, 定义了一个共用体u16_u8。
{
  u16 w;
  struct BW
  {
    u8 bb1;
    u8 bb0;
  }
  bw;
} u16_u8;

typedef struct _DEVICE_INFO		//设备信息结构--USB 内核将主机发送过来的用于实现USB 设备的设置包保存在设备信息结构中，该结构类型是
{
  u8 USBbmRequestType;       /* bmRequestType */		//设置包中的bmRequestType 的副本
  u8 USBbRequest;            /* bRequest */					//设置包中的bRequest 的副本
  u16_u8 USBwValues;         /* wValue */						//设置包中的wValue 的副本
  u16_u8 USBwIndexs;         /* wIndex */						//设置包中的wIndex 的副本
  u16_u8 USBwLengths;        /* wLength */

  u8 ControlState;           /* of type CONTROL_STATE */			//内核的状态
  u8 Current_Feature;																					//反映了当前设备的特性
  u8 Current_Configuration;   /* Selected configuration */		//指工作中设备的设置
  u8 Current_Interface;       /* Selected interface of current configuration */		//是选中的接口。
  u8 Current_AlternateSetting;/* Selected Alternate Setting of current						//是为当前工作设置和接口的选中的备用设置
                                     interface*/

  ENDPOINT_INFO Ctrl_Info;
}DEVICE_INFO;

typedef struct _DEVICE_PROP			//设备属性结构--USB 内核在必要时将控制权交给用户程序。用户处理过程以Device_Property 数组给出，该结构属于DEVICE_PROP 类型，类型定义如下：
{
  void (*Init)(void);        /* Initialize the device */		 			/*USB IP 的初始化过程. 在应用开始时调用一次，用于管理初始化进程。*/
  void (*Reset)(void);       /* Reset routine of this device */		/*USB IP 复位过程. 当宏单元收到RESET 信号时调用，用户程序应该在此过程中设置端点（设置默认的控制端点并使能接收）。*/
	
  /* Device dependent process after the status stage */	/*在控制传输中分三个过程：1.建立过程，2、可选的数据过程，3、状态过程*/
  void (*Process_Status_IN)(void);			/*状态过程中，处理IN令牌包*/
  void (*Process_Status_OUT)(void);			/*状态过程中，处理OUT令牌包*/

  /* Procedure of process on setup stage of a class specified request with data stage */	/*在建立阶段的过程中，会有很多特殊类请求的数据阶段stage */
  /*所有在数据阶段的特殊类请求都在Class_Data_Setup()函数中处理
		Class_Data_Setup() 会响应去检查所有的特殊类请求，同时根据请求填充ENDPOINT_INFO结构信息
		如果IN令牌包是期望的令牌包，则wLength和wOffset两个域会分别被填充成要发送的总字节数和要开始传输的位置
    如果OUT令牌包是期盼的令牌包，则rLength和rOffser将会分别被填充成要接收的总字节数和要接收数据的缓冲区起始地址
    如果请求有效，Class_Data_Setup返回SUCCESS，否则返回UNSUPPORT
   注意：
    因为GET_CONFIGURATION和GET_INTERFACE两个请求跟个别的类联系密切，所以他们会在该函数中检查和处理*/
  RESULT (*Class_Data_Setup)(u8 RequestNo);

  /* Procedure of process on setup stage of a class specified request without data stage */	/*在建立过程中，会有很多特殊类请求的无数据阶段*/
  /*所有的没有数据阶段的特殊请求都在Class_NoData_Setup这个函数中处理
   Class_NoData_Setup() 
   会响应去检查所有特殊类请求，并且执行请求

   注意：
    因为SET_CONFIGURATION和SET_INTERFACE这两个请求跟个别的类联系密切，他们都会在该函数中被检查和处理*/

  RESULT (*Class_NoData_Setup)(u8 RequestNo);

  /*Class_Get_Interface_Setting()
   这个函数时在usb_core.c文件中被调用来测试应用程序是否支持被选中的接口和备用接口
   这个函数时由用户写的。如果应用程序支持接口和备用接口，则必须返回"SUCCESS"，否则，返回"UNSUPPORT"*/

  RESULT  (*Class_Get_Interface_Setting)(u8 Interface, u8 AlternateSetting);

  u8* (*GetDeviceDescriptor)(u16 Length);			//内核得到设备描述符
  u8* (*GetConfigDescriptor)(u16 Length);			//内核得到配置描述符
  u8* (*GetStringDescriptor)(u16 Length);			//内核得到字符串描述符
	
	/* 这个字段不是用于当前库版本。它是只保持兼容以前的版本*/
  u8* RxEP_buffer;
  u8 MaxPacketSize;			//最大的包长度为64字节

}DEVICE_PROP;

typedef struct _USER_STANDARD_REQUESTS			//用户标准请求结构--用户标准需求结构是用户代码和标准请求管理之间的接口，
{
  void (*User_GetConfiguration)(void);       /* Get Configuration */		/*获取配置*/	//在收到 Get Configuration（获得配置）标准请求后调用。
  void (*User_SetConfiguration)(void);       /* Set Configuration */		/*设置配置*/	//在收到 Set Configuration（设置配置）标准请求后调用。
  void (*User_GetInterface)(void);           /* Get Interface */				/*获取接口*/	//在收到 Get interface（获取接口）标准请求后调用。
  void (*User_SetInterface)(void);           /* Set Interface */				/*设置接口*/	//在收到 Set interface（设置接口）标准请求后调用。
  void (*User_GetStatus)(void);              /* Get Status */						/*获取状态*/	//在收到 Get interface（获得状态）标准请求后调用。
  void (*User_ClearFeature)(void);           /* Clear Feature */				/*清除特性*/	//在收到 Clear Feature（清除） 标准请求后调用。
  void (*User_SetEndPointFeature)(void);     /* Set Endpoint Feature */	/*设置端点特性*/	//在收到 Set Feature（设置） 标准请求后调用。
  void (*User_SetDeviceFeature)(void);       /* Set Device Feature */		/*设置设备特性*/	//在收到 Set Feature（设置） 标准请求后调用。
  void (*User_SetDeviceAddress)(void);       /* Set Device Address */		/*设置设备地址*/	//在收到set Address（设置地址） 标准请求后调用。
}
USER_STANDARD_REQUESTS;

/* Exported constants --------------------------------------------------------*/
#define Type_Recipient (pInformation->USBbmRequestType & (REQUEST_TYPE | RECIPIENT))

#define Usb_rLength Usb_wLength
#define Usb_rOffset Usb_wOffset

#define USBwValue		USBwValues.w
#define USBwValue0	USBwValues.bw.bb0
#define USBwValue1	USBwValues.bw.bb1
#define USBwIndex		USBwIndexs.w
#define USBwIndex0	USBwIndexs.bw.bb0
#define USBwIndex1	USBwIndexs.bw.bb1
#define USBwLength	USBwLengths.w
#define USBwLength0	USBwLengths.bw.bb0
#define USBwLength1	USBwLengths.bw.bb1

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
u8 Setup0_Process(void);
u8 Post0_Process(void);
u8 Out0_Process(void);
u8 In0_Process(void);

RESULT Standard_SetEndPointFeature(void);
RESULT Standard_SetDeviceFeature(void);

u8 *Standard_GetConfiguration(u16 Length);	//如果'Length'值为0时，请求无效，返回0	//长度不为0时，返回配置变量的地址
RESULT Standard_SetConfiguration(void);			//该函数用于设置配置值，主机发送该请求后，usb设备要配置自己的设备值
u8 *Standard_GetInterface(u16 Length);			//当'Length'的值为0，请求无效，返回0，当长度不为0，返回备用接口的地址
RESULT Standard_SetInterface(void);
u8 *Standard_GetDescriptorData(u16 Length, PONE_DESCRIPTOR pDesc);

u8 *Standard_GetStatus(u16 Length);					//拷贝设备请求数据到"StatusInfo buffer".//如果请求在数据块的末端，或则当'Length'为0是，请求无效，则返回0
RESULT Standard_ClearFeature(void);					//清除或禁止某个指定的特性	//当请求被执行，返回Return USB_SUCCESS,如果请求无效，返回USB_UNSUPPORT
RESULT Standard_SetEndPointFeature(void);		//设置或使能端点的指定的特性	//如果请求被执行，返回USB_SUCCESS,如果请求无效，则返回USB_UNSUPPORT
RESULT Standard_SetDeviceFeature(void);			//设置或使能设备指定的特性		//如果请求被执行，返回USB_SUCCESS,如果请求无效，则返回USB_UNSUPPORT
void SetDeviceAddress(u8);
void NOP_Process(void);

extern DEVICE_PROP Device_Property;
extern  USER_STANDARD_REQUESTS User_Standard_Requests;
extern  DEVICE  Device_Table;
extern DEVICE_INFO Device_Info;

/* cells saving status during interrupt servicing */
extern u16 SaveRState;
extern u16 SaveTState;

#endif /* __USB_CORE_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
