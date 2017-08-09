/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_desc.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Descriptors for Virtual Com Port Demo
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
* 功能：具体设备的相关描述符定义和处理
* 
* 
* 
* 
* 
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"


/*=================================================================================*/
/*=================================================================================*/
/* USB标准设备描述符*/
//const u8 Virtual_Com_Port_DeviceDescriptor[] =								//设备描述符--数组
//  {
//    0x12,   /* bLength */																			//整个描述符长度--0x12/18个字节
//    USB_DEVICE_DESCRIPTOR_TYPE,     /* bDescriptorType */			//类别--0X01设备描述符
//    0x00,
//    0x02,   /* bcdUSB = 2.00 */																//此设备与描述表兼容的USB设备说明版本号（BCD 码）
//    0x02,   /* bDeviceClass: CDC */														//设备类码：0X02 CDC控制类
//    0x00,   /* bDeviceSubClass */															//子类挖码
//    0x00,   /* bDeviceProtocol */															//协议码
//    0x40,   /* bMaxPacketSize0 */															//端点0的最大包大小（仅8,16,32,64 为合法值）
//    0x83,
//    0x04,   /* idVendor = 0x0483 */														//厂商标志（由USB-IF组织赋值）
//    0x40,
//    0x57,   /* idProduct = 0x7540 */													//产品标志（由厂商赋值）
//    0x00,
//    0x02,   /* bcdDevice = 2.00 */														//设备的版本号	
//    1,              /* Index of string descriptor describing manufacturer */										//描述厂商信息的字符串描述符的索引值。
//    2,              /* Index of string descriptor describing product */													//描述产品信息的字串描述符的索引值。
//    3,              /* Index of string descriptor describing the device's serial number */			//描述设备序列号信息的字串描述符的索引值。
//    0x01    /* bNumConfigurations */													//可能的配置描述符数目																						
//  };	
const u8 Virtual_Com_Port_DeviceDescriptor[] =								//设备描述符--数组
  {
    0x12,   /* bLength */																			//整个描述符长度--0x12/18个字节
    USB_DEVICE_DESCRIPTOR_TYPE,     /* bDescriptorType */			//类别--0X01设备描述符
    0x00,
    0x02,   /* bcdUSB = 2.00 */																//此设备与描述表兼容的USB设备说明版本号（BCD 码）
		#if (USB_MULTI_IF)
			0xEF,   /* bDeviceClass:  */															//设备类码：0XEF 混杂设备
			0x02,   /* bDeviceSubClass */															//子类挖码
			0x01,   /* bDeviceProtocol */															//协议码
			0x40,   /* bMaxPacketSize0 */															//端点0的最大包大小（仅8,16,32,64 为合法值）
			0x83,
			0x04,   /* idVendor = 0x0483 */														//厂商标志（由USB-IF组织赋值）
			0x40,
			0x50,   /* idProduct = 0x5740 */													//产品标志（由厂商赋值）
		#else
			0x02,   /* bDeviceClass: CDC */														//设备类码：0X02 CDC控制类
			0x02,   /* bDeviceSubClass */															//子类挖码
			0x01,   /* bDeviceProtocol */															//协议码
			0x40,   /* bMaxPacketSize0 */															//端点0的最大包大小（仅8,16,32,64 为合法值）
			0x83,
			0x04,   /* idVendor = 0x0483 */														//厂商标志（由USB-IF组织赋值）
			0x40,
			0x57,   /* idProduct = 0x5740 */													//产品标志（由厂商赋值）
		#endif
    0x00,
    0x02,   /* bcdDevice = 2.00 */														//设备的版本号	
    1,              /* Index of string descriptor describing manufacturer */										//描述厂商信息的字符串描述符的索引值。
    2,              /* Index of string descriptor describing product */													//描述产品信息的字串描述符的索引值。
    3,              /* Index of string descriptor describing the device's serial number */			//描述设备序列号信息的字串描述符的索引值。
    0x01    /* bNumConfigurations */													//可能的配置描述符数目
  };
const u8 USBD_DeviceQualifier[] =								//设备描述符--数组
  {
    0x0A,   /* bLength */																			//整个描述符长度--0x12/18个字节
    0X06,     /* bDescriptorType */			//类别--0X00设备描述符
    0x00,
    0x02,   /* bcdUSB = 2.00 */																//此设备与描述表兼容的USB设备说明版本号（BCD 码）
    0x00,   /* bDeviceClass: CDC */														//设备类码：0X02 CDC控制类
    0x00,   /* bDeviceSubClass */															//子类挖码
    0x00,   /* bDeviceProtocol */															//协议码
    0x40,   /* bMaxPacketSize0 */															//端点0的最大包大小（仅8,16,32,64 为合法值）
    0x01,
    0x00,   /* idVendor = 0x0483 */														//厂商标志（由USB-IF组织赋值）													
  };
	
const u8 Virtual_Com_Port_ConfigDescriptor[] =
{
/* Configuration 1 */
  USB_CONFIGURATION_DESC_SIZE,       	/* bLength */
  USB_CONFIGURATION_DESCRIPTOR_TYPE, 	/* bDescriptorType */
  WBVAL((USB_CONFIGURATION_DESC_SIZE	+	(USB_NUM_INTERFACES/2) * IAD_CDC_IF_DESC_SET_SIZE)),/* wTotalLength */
  USB_NUM_INTERFACES,                	/* bNumInterfaces */
  0x01,                              	/* bConfigurationValue: 0x01 is used to select this configuration */
  0x00,                              	/* iConfiguration: no string to describe this configuration */
  USB_CONFIG_BUS_POWERED,							/* bmAttributes USB_CONFIG_REMOTE_WAKEUP*/
  USB_CONFIG_POWER_MA(100),          	/* bMaxPower, device power consumption is 100 mA */
	
  IAD_CDC_IF_DESC_SET(	USB_CDC_CIF_NUM0,		USB_CDC_DIF_NUM0,		USB_ENDPOINT_IN(1),		USB_ENDPOINT_OUT(3),		USB_ENDPOINT_IN(1)	),		//CDC接口1
//  IAD_CDC_IF_DESC_SET(	USB_CDC_CIF_NUM1, 	USB_CDC_DIF_NUM1, 	USB_ENDPOINT_IN(1), 	USB_ENDPOINT_OUT(3), 		USB_ENDPOINT_IN(1)	),		//CDC接口2
//  IAD_CDC_IF_DESC_SET(	USB_CDC_CIF_NUM2, 	USB_CDC_DIF_NUM2,		USB_ENDPOINT_IN(1),		USB_ENDPOINT_OUT(3), 		USB_ENDPOINT_IN(1)	),		//CDC接口3
//	IAD_CDC_IF_DESC_SET(	USB_CDC_CIF_NUM3, 	USB_CDC_DIF_NUM3,		USB_ENDPOINT_IN(1),		USB_ENDPOINT_OUT(3), 		USB_ENDPOINT_IN(1)	),		//CDC接口4
//	
//	IAD_CDC_IF_DESC_SET(	USB_CDC_CIF_NUM4, 	USB_CDC_DIF_NUM4,		USB_ENDPOINT_IN(1),		USB_ENDPOINT_OUT(3), 		USB_ENDPOINT_IN(1)	),		//CDC接口5
//	IAD_CDC_IF_DESC_SET(	USB_CDC_CIF_NUM5, 	USB_CDC_DIF_NUM5,		USB_ENDPOINT_IN(11),	USB_ENDPOINT_OUT(12), 	USB_ENDPOINT_IN(12)	),		//CDC接口6
//	IAD_CDC_IF_DESC_SET(	USB_CDC_CIF_NUM6, 	USB_CDC_DIF_NUM6,		USB_ENDPOINT_IN(11),	USB_ENDPOINT_OUT(12), 	USB_ENDPOINT_IN(12)	),		//CDC接口7
//	IAD_CDC_IF_DESC_SET(	USB_CDC_CIF_NUM7, 	USB_CDC_DIF_NUM7,		USB_ENDPOINT_IN(13),	USB_ENDPOINT_OUT(14), 	USB_ENDPOINT_IN(14)	),		//CDC接口8
//	IAD_CDC_IF_DESC_SET(	USB_CDC_CIF_NUM8, 	USB_CDC_DIF_NUM8,		USB_ENDPOINT_IN(15),	USB_ENDPOINT_OUT(16), 	USB_ENDPOINT_IN(16)	),		//CDC接口9
//	
//	IAD_CDC_IF_DESC_SET(	USB_CDC_CIF_NUM9, 	USB_CDC_DIF_NUM9,		USB_ENDPOINT_IN(15),	USB_ENDPOINT_OUT(16), 	USB_ENDPOINT_IN(16)	),		//CDC接口10
//	IAD_CDC_IF_DESC_SET(	USB_CDC_CIF_NUM10, 	USB_CDC_DIF_NUM10,	USB_ENDPOINT_IN(15),	USB_ENDPOINT_OUT(16), 	USB_ENDPOINT_IN(16)	),		//CDC接口11
//	IAD_CDC_IF_DESC_SET(	USB_CDC_CIF_NUM11, 	USB_CDC_DIF_NUM11,	USB_ENDPOINT_IN(15),	USB_ENDPOINT_OUT(16), 	USB_ENDPOINT_IN(16)	),		//CDC接口12
//	IAD_CDC_IF_DESC_SET(	USB_CDC_CIF_NUM12, 	USB_CDC_DIF_NUM12,	USB_ENDPOINT_IN(15),	USB_ENDPOINT_OUT(16), 	USB_ENDPOINT_IN(16)	),		//CDC接口13
//	
//	IAD_CDC_IF_DESC_SET(	USB_CDC_CIF_NUM13, 	USB_CDC_DIF_NUM13,	USB_ENDPOINT_IN(15),	USB_ENDPOINT_OUT(16), 	USB_ENDPOINT_IN(16)	),		//CDC接口14
//	IAD_CDC_IF_DESC_SET(	USB_CDC_CIF_NUM14, 	USB_CDC_DIF_NUM14,	USB_ENDPOINT_IN(15),	USB_ENDPOINT_OUT(16), 	USB_ENDPOINT_IN(16)	),		//CDC接口15
//	IAD_CDC_IF_DESC_SET(	USB_CDC_CIF_NUM15, 	USB_CDC_DIF_NUM15,	USB_ENDPOINT_IN(15),	USB_ENDPOINT_OUT(16), 	USB_ENDPOINT_IN(16)	),		//CDC接口16
//	IAD_CDC_IF_DESC_SET(	USB_CDC_CIF_NUM16, 	USB_CDC_DIF_NUM16,	USB_ENDPOINT_IN(15),	USB_ENDPOINT_OUT(16), 	USB_ENDPOINT_IN(16)	)			//CDC接口17
};
	
/* USB配置描述符集合(配置、接口、端点、类、厂商)(Configuration, Interface, Endpoint, Class, Vendor */
const u8 Virtual_Com_Port_ConfigDescriptor0[] =			//配置描述符
  {
		//*******************以下为配置描述符*********************/
    /*Configuation Descriptor*/		
    0x09,   /* bLength: Configuation Descriptor size */																		//整个描述符长度--0x09/9个字节
    USB_CONFIGURATION_DESCRIPTOR_TYPE,      /* bDescriptorType: Configuration */					//类别--0X02设备描述符
//    VIRTUAL_COM_PORT_SIZ_CONFIG_DESC,       /* wTotalLength:no of returned bytes */				//此配置信息的总长（包括配置，接口，端点和设备类及厂商定义的描述符）
//    0x00,
		WBVAL((USB_CONFIGURATION_DESC_SIZE	+	6 * IAD_CDC_IF_DESC_SET_SIZE)),/* wTotalLength */
    0x06,   /* bNumInterfaces: 2 interface */																							//此配置所支持的接口个数
    0x01,   /* bConfigurationValue: Configuration value */																//在SetConfiguration（）请求中用作参数来选定此配置。
    0x00,   /* iConfiguration: Index of string descriptor describing the configuration */	//描述此配置的字串描述表索引
    0xC0,   /* bmAttributes: self powered */																							//配置特性：D7： 保留（设为一），D6： 自给电源，D5： 远程唤醒，D4..0：保留（设为一）
    0x32,   /* MaxPower 0 mA */																														//在此配置下的总线电源耗费量。以2mA 为一个单位。
    /*Interface Descriptor*/																							/*接口描述符*/
		
		
		//*******************以下为IAD描述符*********************/
//		#if (USB_MULTI_IF)
//			CDC_ACM_DESC_IAD(1,1),
//		#endif
		
		/* Interface Association Descriptor */
		0x08,               				/* bLength: Interface Descriptor size */
		USB_INTERFACE_ASSOCIATION_DESCRIPTOR_TYPE,   		/* bDescriptorType: IAD */
		0x00,               				/* bFirstInterface */	//起始接口
		0x02,               				/* bInterfaceCount */	//接口数
		0x02,               				/* bFunctionClass */
		0x02,               				/* bFunctionSubClass */
		0x01,               				/* bFunctionProtocol */
		0x02,              					/* iFunction */

			
		
		//*******************以下为CDC类接口描述符*********************/
    0x09,   /* bLength: Interface Descriptor size */											//整个描述符长度--0x09/9个字节
		USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: Interface */			//接口描述表类（此处应为0x04）接口描述符
    /* Interface descriptor type */																				//接口类型描述
    0x00,   /* bInterfaceNumber: Number of Interface */										//接口号，当前配置支持的接口数组索引（从零开始）。该接口的编号，第一个接口，编号为0。
    0x00,   /* bAlternateSetting: Alternate setting */										//可选设置的索引值。该接口的备用编号，为0。
    0x01,   /* bNumEndpoints: One endpoints used */												//该接口使用的端点数，不包括端点0，如果是0则说明此接口只用缺省控制管道。非0端点的数目。CDC接口只使用一个中断输入端点。
    0x02,   /* bInterfaceClass: Communication Interface Class */					//接口所属的类值：0值为将来的标准保留。 如果此域的值设为FFH，则此接口类由厂商说明。 所有其它的值由USB说明保留。,0x02 CDC类
    0x02,   /* bInterfaceSubClass: Abstract Control Model */							//子类码:这些值的定义视bInterfaceClass域而定。 如果bInterfaceClass域的值为零则此域的值必须为零。 bInterfaceClass域不为FFH则所有值由USB 所保留。
    0x01,   /* bInterfaceProtocol: Common AT commands */									//协议码：bInterfaceClass 和bInterfaceSubClass 域的值而定.如果一个接口支持设备类相关的请求此域的值指出了设备类说明中所定义的协议.使用Common AT Commands（通用AT命令）
    0x00,   /* iInterface: */																							//描述此接口的字串描述表的索引值。该接口的字符串索引值。这里没有，为0。
		
		
		
		//**********************以下为功能描述符*****************************//
    /*Header Functional Descriptor*/													/*头功能描述*/
		/****************头功能描述符***************/
    0x05,   /* bLength: Endpoint Descriptor size */												//描述符长度--0x05/5个字节
    0x24,   /* bDescriptorType: CS_INTERFACE */														//描述符类型为类特殊接口（CS_INTERFACE）编号为0x24。
    0x00,   /* bDescriptorSubtype: Header Func Desc */										//描述符子类为Header Functional Descriptor  
    0x10,   /* bcdCDC: spec release number */															//bcdCDC字段。CDC版本号，为0x0110（低字节在先）
    0x01,
		
		
    /*Call Managment Functional Descriptor*/
		/*************调用管理功能描述符*************/
    0x05,   /* bFunctionLength */																					//描述符长度
    0x24,   /* bDescriptorType: CS_INTERFACE */														//描述符类型为类特殊接口（CS_INTERFACE）编号为0x24。
    0x01,   /* bDescriptorSubtype: Call Management Func Desc */						//描述符子类为Call Management 编号为0x01。
    0x00,   /* bmCapabilities: D0+D1 */																		//设备自己不管理call management  
    0x01,   /* bDataInterface: 1 */																				//有一个数据类接口用作call management  
		
		
		
    /*ACM（Abstract Control Management ） Functional Descriptor*/
		/**************抽象控制管理功能描述符 ****************/
    0x04,   /* bFunctionLength */																					//描述符长度
    0x24,   /* bDescriptorType: CS_INTERFACE */														//描述符类型为类特殊接口（CS_INTERFACE）编号为0x24。
    0x02,   /* bDescriptorSubtype: Abstract Control Management desc */		//描述符子类为Abstract Control Management Functional Descriptor 编号为0x02。
    0x02,   /* bmCapabilities */																					//支持Set_Line_Coding、Set_Control_Line_State、Get_Line_Coding请求和Serial_State通知 
		
		
    /*Union Functional Descriptor*/
		/***************联合功能描述符**************/
    0x05,   /* bFunctionLength */																					//描述符长度
    0x24,   /* bDescriptorType: CS_INTERFACE */														//描述符类型为类特殊接口（CS_INTERFACE）编号为0x24。
    0x06,   /* bDescriptorSubtype: Union func desc */											//描述符子类为Union functional descriptor，编号为0x06。
    0x00,   /* bMasterInterface: Communication class interface */					//MasterInterface字段。这里为前面编号为0的CDC接口。
    0x01,   /* bSlaveInterface0: Data Class Interface */									//SlaveInterface字段，这里为接下来编号为1的数据类接口。
		
		
    /*Endpoint 2 Descriptor*/																	
		/********************输入（主机）端点2描述符******************/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */				//端点描述类型 0x04
    0x82,   /* bEndpointAddress: (IN2) */																	//端点：此描述表所描述的端点的地址、方向：
																																					//		Bit 3..0 : 端点号，Bit 6..4 : 保留,为零，Bit 7: 方向,如果控制端点则略。0：输出端点（主机到设备），1：输入端点（设备到主机）
   
		0x03,   /* bmAttributes: Interrupt */																	//传输类型：Bit 1..0 :传送类型；00=控制传送，01=同步传送，10=批传送，11=中断传送
    VIRTUAL_COM_PORT_INT_SIZE,      /* wMaxPacketSize: */									//端点能够接收或发送的最大数据包的大小。对于实进传输，此值用于为每帧的数据净负荷预留时间。在实际运行时，管道可能不完全需要预留的带宽，实际带宽可由设备通过一种非USB定义的机制汇报给主机。对于中断传输，批量传输和控制传输，端点可能发送比之短的数据包
    0x00,
    0xFF,   /* bInterval: */																							//周期数据传输端点的时间间隙。此域的值对于批传送的端点及控制传送的端点无意义。对于同步传送的端点此域必需为1，表示周期为1ms。对于中断传送的端点此域值的范围为1ms到255ms。
    
		
		
		/*Data class interface descriptor*/
		/********************数据类接口描述符********************/
    0x09,   /* bLength: Endpoint Descriptor size */												//
    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: */								//USB接口描述符 0X04
    0x01,   /* bInterfaceNumber: Number of Interface */										//接口号，当前配置支持的接口数组索引（从零开始）。
    0x00,   /* bAlternateSetting: Alternate setting */										//可选设置的索引值。
    0x02,   /* bNumEndpoints: Two endpoints used */												//此接口用的端点数量，如果是零则说明此接口只用缺省控制管道。
    0x0A,   /* bInterfaceClass: CDC */																		//接口所属的类值： 0值为将来的标准保留。 如果此域的值设为FFH，则此接口类由厂商说明。 所有其它的值由USB 说明保留。0x0A CDC数据类
    0x00,   /* bInterfaceSubClass: */																			//子类码
    0x00,   /* bInterfaceProtocol: */																			//协议码：bInterfaceClass 和bInterfaceSubClass 域的值而定.如果一个接口支持设备类相关的请求此域的值指出了设备类说明中所定义的协议.
    0x00,   /* iInterface: */																							//描述此接口的字串描述表的索引值。   
	 
		
		/*Endpoint 3 Descriptor*/																
		/********************输出（主机）端点3描述符******************/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */				//端点描述类型 0x04
    0x03,   /* bEndpointAddress: (OUT3) */																//端点：此描述表所描述的端点的地址、方向：
																																					//		Bit 3..0 : 端点号，Bit 6..4 : 保留,为零，Bit 7: 方向,如果控制端点则略。0：输出端点（主机到设备），1：输入端点（设备到主机）
    0x02,   /* bmAttributes: Bulk */																			//传输类型：Bit 1..0 :传送类型；00=控制传送，01=同步传送，10=批传送，11=中断传送
    VIRTUAL_COM_PORT_DATA_SIZE,             /* wMaxPacketSize: */					//端点能够接收或发送的最大数据包的大小。对于实进传输，此值用于为每帧的数据净负荷预留时间。在实际运行时，管道可能不完全需要预留的带宽，实际带宽可由设备通过一种非USB定义的机制汇报给主机。对于中断传输，批量传输和控制传输，端点可能发送比之短的数据包
    0x00,
    0x00,   /* bInterval: ignore for Bulk transfer */											//周期数据传输端点的时间间隙。此域的值对于批传送的端点及控制传送的端点无意义。对于同步传送的端点此域必需为1，表示周期为1ms。对于中断传送的端点此域值的范围为1ms到255ms。
    
		
		/*Endpoint 1 Descriptor*/
		/********************输入端点1描述符********************/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */				//端点描述类型 0x04
    0x81,   /* bEndpointAddress: (IN1) */																	//端点：此描述表所描述的端点的地址、方向：
																																					//		Bit 3..0 : 端点号，Bit 6..4 : 保留,为零，Bit 7: 方向,如果控制端点则略。0：输出端点（主机到设备），1：输入端点（设备到主机）
    0x02,   /* bmAttributes: Bulk */																			//传输类型：Bit 1..0 :传送类型；00=控制传送，01=同步传送，10=批传送，11=中断传送
    VIRTUAL_COM_PORT_DATA_SIZE,             /* wMaxPacketSize: */					//端点能够接收或发送的最大数据包的大小。对于实进传输，此值用于为每帧的数据净负荷预留时间。在实际运行时，管道可能不完全需要预留的带宽，实际带宽可由设备通过一种非USB定义的机制汇报给主机。对于中断传输，批量传输和控制传输，端点可能发送比之短的数据包
    0x00,
    0x00,    /* bInterval */																								//周期数据传输端点的时间间隙。此域的值对于批传送的端点及控制传送的端点无意义。对于同步传送的端点此域必需为1，表示周期为1ms。对于中断传送的端点此域值的范围为1ms到255ms。
		
		
		
//**************************************************************************************************************************************		
		
		/* Interface Association Descriptor */
		0x08,               				/* bLength: Interface Descriptor size */
		USB_INTERFACE_ASSOCIATION_DESCRIPTOR_TYPE,   		/* bDescriptorType: IAD */
		0x02,               				/* bFirstInterface */
		0x02,               				/* bInterfaceCount */
		0x02,               				/* bFunctionClass */
		0x02,               				/* bFunctionSubClass */
		0x01,               				/* bFunctionProtocol */
		0x02,              					/* iFunction */

			
		
		//*******************以下为CDC类接口描述符*********************/
    0x09,   /* bLength: Interface Descriptor size */											//整个描述符长度--0x09/9个字节
		USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: Interface */			//接口描述表类（此处应为0x04）接口描述符
    /* Interface descriptor type */																				//接口类型描述
    0x02,   /* bInterfaceNumber: Number of Interface */										//接口号，当前配置支持的接口数组索引（从零开始）。该接口的编号，第一个接口，编号为0。
    0x00,   /* bAlternateSetting: Alternate setting */										//可选设置的索引值。该接口的备用编号，为0。
    0x01,   /* bNumEndpoints: One endpoints used */												//该接口使用的端点数，不包括端点0，如果是0则说明此接口只用缺省控制管道。非0端点的数目。CDC接口只使用一个中断输入端点。
    0x02,   /* bInterfaceClass: Communication Interface Class */					//接口所属的类值：0值为将来的标准保留。 如果此域的值设为FFH，则此接口类由厂商说明。 所有其它的值由USB说明保留。,0x02 CDC类
    0x02,   /* bInterfaceSubClass: Abstract Control Model */							//子类码:这些值的定义视bInterfaceClass域而定。 如果bInterfaceClass域的值为零则此域的值必须为零。 bInterfaceClass域不为FFH则所有值由USB 所保留。
    0x01,   /* bInterfaceProtocol: Common AT commands */									//协议码：bInterfaceClass 和bInterfaceSubClass 域的值而定.如果一个接口支持设备类相关的请求此域的值指出了设备类说明中所定义的协议.使用Common AT Commands（通用AT命令）
    0x00,   /* iInterface: */																							//描述此接口的字串描述表的索引值。该接口的字符串索引值。这里没有，为0。
		
		
		
		//**********************以下为功能描述符*****************************//
    /*Header Functional Descriptor*/													/*头功能描述*/
		/****************头功能描述符***************/
    0x05,   /* bLength: Endpoint Descriptor size */												//描述符长度--0x05/5个字节
    0x24,   /* bDescriptorType: CS_INTERFACE */														//描述符类型为类特殊接口（CS_INTERFACE）编号为0x24。
    0x00,   /* bDescriptorSubtype: Header Func Desc */										//描述符子类为Header Functional Descriptor  
    0x10,   /* bcdCDC: spec release number */															//bcdCDC字段。CDC版本号，为0x0110（低字节在先）
    0x01,
		
		
    /*Call Managment Functional Descriptor*/
		/*************调用管理功能描述符*************/
    0x05,   /* bFunctionLength */																					//描述符长度
    0x24,   /* bDescriptorType: CS_INTERFACE */														//描述符类型为类特殊接口（CS_INTERFACE）编号为0x24。
    0x01,   /* bDescriptorSubtype: Call Management Func Desc */						//描述符子类为Call Management 编号为0x01。
    0x00,   /* bmCapabilities: D0+D1 */																		//设备自己不管理call management  
    0x03,   /* bDataInterface: 1 */																				//有一个数据类接口用作call management  
		
		
		
    /*ACM（Abstract Control Management ） Functional Descriptor*/
		/**************抽象控制管理功能描述符 ****************/
    0x04,   /* bFunctionLength */																					//描述符长度
    0x24,   /* bDescriptorType: CS_INTERFACE */														//描述符类型为类特殊接口（CS_INTERFACE）编号为0x24。
    0x02,   /* bDescriptorSubtype: Abstract Control Management desc */		//描述符子类为Abstract Control Management Functional Descriptor 编号为0x02。
    0x02,   /* bmCapabilities */																					//支持Set_Line_Coding、Set_Control_Line_State、Get_Line_Coding请求和Serial_State通知 
		
		
    /*Union Functional Descriptor*/
		/***************联合功能描述符**************/
    0x05,   /* bFunctionLength */																					//描述符长度
    0x24,   /* bDescriptorType: CS_INTERFACE */														//描述符类型为类特殊接口（CS_INTERFACE）编号为0x24。
    0x06,   /* bDescriptorSubtype: Union func desc */											//描述符子类为Union functional descriptor，编号为0x06。
    0x02,   /* bMasterInterface: Communication class interface */					//MasterInterface字段。这里为前面编号为0的CDC接口。
    0x03,   /* bSlaveInterface0: Data Class Interface */									//SlaveInterface字段，这里为接下来编号为1的数据类接口。
		
		
    /*Endpoint 2 Descriptor*/																	
		/********************输入（主机）端点2描述符******************/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */				//端点描述类型 0x04
    0x84,   /* bEndpointAddress: (IN2) */																	//端点：此描述表所描述的端点的地址、方向：
																																					//		Bit 3..0 : 端点号，Bit 6..4 : 保留,为零，Bit 7: 方向,如果控制端点则略。0：输出端点（主机到设备），1：输入端点（设备到主机）
   
		0x03,   /* bmAttributes: Interrupt */																	//传输类型：Bit 1..0 :传送类型；00=控制传送，01=同步传送，10=批传送，11=中断传送
    VIRTUAL_COM_PORT_INT_SIZE,      /* wMaxPacketSize: */									//端点能够接收或发送的最大数据包的大小。对于实进传输，此值用于为每帧的数据净负荷预留时间。在实际运行时，管道可能不完全需要预留的带宽，实际带宽可由设备通过一种非USB定义的机制汇报给主机。对于中断传输，批量传输和控制传输，端点可能发送比之短的数据包
    0x00,
    0xFF,   /* bInterval: */																							//周期数据传输端点的时间间隙。此域的值对于批传送的端点及控制传送的端点无意义。对于同步传送的端点此域必需为1，表示周期为1ms。对于中断传送的端点此域值的范围为1ms到255ms。
    
		
		
		/*Data class interface descriptor*/
		/********************数据类接口描述符********************/
    0x09,   /* bLength: Endpoint Descriptor size */												//
    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: */								//USB接口描述符 0X04
    0x03,   /* bInterfaceNumber: Number of Interface */										//接口号，当前配置支持的接口数组索引（从零开始）。
    0x00,   /* bAlternateSetting: Alternate setting */										//可选设置的索引值。
    0x02,   /* bNumEndpoints: Two endpoints used */												//此接口用的端点数量，如果是零则说明此接口只用缺省控制管道。
    0x0A,   /* bInterfaceClass: CDC */																		//接口所属的类值： 0值为将来的标准保留。 如果此域的值设为FFH，则此接口类由厂商说明。 所有其它的值由USB 说明保留。0x0A CDC数据类
    0x00,   /* bInterfaceSubClass: */																			//子类码
    0x00,   /* bInterfaceProtocol: */																			//协议码：bInterfaceClass 和bInterfaceSubClass 域的值而定.如果一个接口支持设备类相关的请求此域的值指出了设备类说明中所定义的协议.
    0x00,   /* iInterface: */																							//描述此接口的字串描述表的索引值。   
	 
		
		/*Endpoint 3 Descriptor*/																
		/********************输出（主机）端点3描述符******************/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */				//端点描述类型 0x04
    0x06,   /* bEndpointAddress: (OUT3) */																//端点：此描述表所描述的端点的地址、方向：
																																					//		Bit 3..0 : 端点号，Bit 6..4 : 保留,为零，Bit 7: 方向,如果控制端点则略。0：输出端点（主机到设备），1：输入端点（设备到主机）
    0x02,   /* bmAttributes: Bulk */																			//传输类型：Bit 1..0 :传送类型；00=控制传送，01=同步传送，10=批传送，11=中断传送
    VIRTUAL_COM_PORT_DATA_SIZE,             /* wMaxPacketSize: */					//端点能够接收或发送的最大数据包的大小。对于实进传输，此值用于为每帧的数据净负荷预留时间。在实际运行时，管道可能不完全需要预留的带宽，实际带宽可由设备通过一种非USB定义的机制汇报给主机。对于中断传输，批量传输和控制传输，端点可能发送比之短的数据包
    0x00,
    0x00,   /* bInterval: ignore for Bulk transfer */											//周期数据传输端点的时间间隙。此域的值对于批传送的端点及控制传送的端点无意义。对于同步传送的端点此域必需为1，表示周期为1ms。对于中断传送的端点此域值的范围为1ms到255ms。
    
		
		/*Endpoint 1 Descriptor*/
		/********************输入端点1描述符********************/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */				//端点描述类型 0x04
    0x85,   /* bEndpointAddress: (IN1) */																	//端点：此描述表所描述的端点的地址、方向：
																																					//		Bit 3..0 : 端点号，Bit 6..4 : 保留,为零，Bit 7: 方向,如果控制端点则略。0：输出端点（主机到设备），1：输入端点（设备到主机）
    0x02,   /* bmAttributes: Bulk */																			//传输类型：Bit 1..0 :传送类型；00=控制传送，01=同步传送，10=批传送，11=中断传送
    VIRTUAL_COM_PORT_DATA_SIZE,             /* wMaxPacketSize: */					//端点能够接收或发送的最大数据包的大小。对于实进传输，此值用于为每帧的数据净负荷预留时间。在实际运行时，管道可能不完全需要预留的带宽，实际带宽可由设备通过一种非USB定义的机制汇报给主机。对于中断传输，批量传输和控制传输，端点可能发送比之短的数据包
    0x00,
    0x00,    /* bInterval */																								//周期数据传输端点的时间间隙。此域的值对于批传送的端点及控制传送的端点无意义。对于同步传送的端点此域必需为1，表示周期为1ms。对于中断传送的端点此域值的范围为1ms到255ms。
		
//**************************************************************************************************************************************		
		
		/* Interface Association Descriptor */
		0x08,               				/* bLength: Interface Descriptor size */
		USB_INTERFACE_ASSOCIATION_DESCRIPTOR_TYPE,   		/* bDescriptorType: IAD */
		0x04,               				/* bFirstInterface */
		0x02,               				/* bInterfaceCount */
		0x02,               				/* bFunctionClass */
		0x02,               				/* bFunctionSubClass */
		0x01,               				/* bFunctionProtocol */
		0x02,              					/* iFunction */

			
		
		//*******************以下为CDC类接口描述符*********************/
    0x09,   /* bLength: Interface Descriptor size */											//整个描述符长度--0x09/9个字节
		USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: Interface */			//接口描述表类（此处应为0x04）接口描述符
    /* Interface descriptor type */																				//接口类型描述
    0x04,   /* bInterfaceNumber: Number of Interface */										//接口号，当前配置支持的接口数组索引（从零开始）。该接口的编号，第一个接口，编号为0。
    0x00,   /* bAlternateSetting: Alternate setting */										//可选设置的索引值。该接口的备用编号，为0。
    0x01,   /* bNumEndpoints: One endpoints used */												//该接口使用的端点数，不包括端点0，如果是0则说明此接口只用缺省控制管道。非0端点的数目。CDC接口只使用一个中断输入端点。
    0x02,   /* bInterfaceClass: Communication Interface Class */					//接口所属的类值：0值为将来的标准保留。 如果此域的值设为FFH，则此接口类由厂商说明。 所有其它的值由USB说明保留。,0x02 CDC类
    0x02,   /* bInterfaceSubClass: Abstract Control Model */							//子类码:这些值的定义视bInterfaceClass域而定。 如果bInterfaceClass域的值为零则此域的值必须为零。 bInterfaceClass域不为FFH则所有值由USB 所保留。
    0x01,   /* bInterfaceProtocol: Common AT commands */									//协议码：bInterfaceClass 和bInterfaceSubClass 域的值而定.如果一个接口支持设备类相关的请求此域的值指出了设备类说明中所定义的协议.使用Common AT Commands（通用AT命令）
    0x00,   /* iInterface: */																							//描述此接口的字串描述表的索引值。该接口的字符串索引值。这里没有，为0。
		
		
		
		//**********************以下为功能描述符*****************************//
    /*Header Functional Descriptor*/													/*头功能描述*/
		/****************头功能描述符***************/
    0x05,   /* bLength: Endpoint Descriptor size */												//描述符长度--0x05/5个字节
    0x24,   /* bDescriptorType: CS_INTERFACE */														//描述符类型为类特殊接口（CS_INTERFACE）编号为0x24。
    0x00,   /* bDescriptorSubtype: Header Func Desc */										//描述符子类为Header Functional Descriptor 编号为0x00 
    0x10,   /* bcdCDC: spec release number */															//bcdCDC字段。CDC版本号，为0x0110（低字节在先）
    0x01,
		
		
    /*Call Managment Functional Descriptor*/
		/*************调用管理功能描述符*************/
    0x05,   /* bFunctionLength */																					//描述符长度
    0x24,   /* bDescriptorType: CS_INTERFACE */														//描述符类型为类特殊接口（CS_INTERFACE）编号为0x24。
    0x01,   /* bDescriptorSubtype: Call Management Func Desc */						//描述符子类为Call Management 编号为0x01。
    0x00,   /* bmCapabilities: D0+D1 */																		//设备自己不管理call management  
    0x03,   /* bDataInterface: 1 */																				//有一个数据类接口用作call management  
		
		
		
    /*ACM（Abstract Control Management ） Functional Descriptor*/
		/**************抽象控制管理功能描述符 ****************/
    0x04,   /* bFunctionLength */																					//描述符长度
    0x24,   /* bDescriptorType: CS_INTERFACE */														//描述符类型为类特殊接口（CS_INTERFACE）编号为0x24。
    0x02,   /* bDescriptorSubtype: Abstract Control Management desc */		//描述符子类为Abstract Control Management Functional Descriptor 编号为0x02。
    0x02,   /* bmCapabilities */																					//支持Set_Line_Coding、Set_Control_Line_State、Get_Line_Coding请求和Serial_State通知 
		
		
    /*Union Functional Descriptor*/
		/***************联合功能描述符**************/
    0x05,   /* bFunctionLength */																					//描述符长度
    0x24,   /* bDescriptorType: CS_INTERFACE */														//描述符类型为类特殊接口（CS_INTERFACE）编号为0x24。
    0x06,   /* bDescriptorSubtype: Union func desc */											//描述符子类为Union functional descriptor，编号为0x06。
    0x04,   /* bMasterInterface: Communication class interface */					//MasterInterface字段。这里为前面编号为0的CDC接口。
    0x05,   /* bSlaveInterface0: Data Class Interface */									//SlaveInterface字段，这里为接下来编号为1的数据类接口。
		
		
    /*Endpoint 2 Descriptor*/																	
		/********************输入（主机）端点2描述符******************/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */				//端点描述类型 0x04
    0x87,   /* bEndpointAddress: (IN2) */																	//端点：此描述表所描述的端点的地址、方向：
																																					//		Bit 3..0 : 端点号，Bit 6..4 : 保留,为零，Bit 7: 方向,如果控制端点则略。0：输出端点（主机到设备），1：输入端点（设备到主机）
   
		0x03,   /* bmAttributes: Interrupt */																	//传输类型：Bit 1..0 :传送类型；00=控制传送，01=同步传送，10=批传送，11=中断传送
    VIRTUAL_COM_PORT_INT_SIZE,      /* wMaxPacketSize: */									//端点能够接收或发送的最大数据包的大小。对于实进传输，此值用于为每帧的数据净负荷预留时间。在实际运行时，管道可能不完全需要预留的带宽，实际带宽可由设备通过一种非USB定义的机制汇报给主机。对于中断传输，批量传输和控制传输，端点可能发送比之短的数据包
    0x00,
    0xFF,   /* bInterval: */																							//周期数据传输端点的时间间隙。此域的值对于批传送的端点及控制传送的端点无意义。对于同步传送的端点此域必需为1，表示周期为1ms。对于中断传送的端点此域值的范围为1ms到255ms。
    
		
		
		/*Data class interface descriptor*/
		/********************数据类接口描述符********************/
    0x09,   /* bLength: Endpoint Descriptor size */												//
    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: */								//USB接口描述符 0X04
    0x05,   /* bInterfaceNumber: Number of Interface */										//接口号，当前配置支持的接口数组索引（从零开始）。
    0x00,   /* bAlternateSetting: Alternate setting */										//可选设置的索引值。
    0x02,   /* bNumEndpoints: Two endpoints used */												//此接口用的端点数量，如果是零则说明此接口只用缺省控制管道。
    0x0A,   /* bInterfaceClass: CDC */																		//接口所属的类值： 0值为将来的标准保留。 如果此域的值设为FFH，则此接口类由厂商说明。 所有其它的值由USB 说明保留。0x0A CDC数据类
    0x00,   /* bInterfaceSubClass: */																			//子类码
    0x00,   /* bInterfaceProtocol: */																			//协议码：bInterfaceClass 和bInterfaceSubClass 域的值而定.如果一个接口支持设备类相关的请求此域的值指出了设备类说明中所定义的协议.
    0x00,   /* iInterface: */																							//描述此接口的字串描述表的索引值。   
	 
		
		/*Endpoint 3 Descriptor*/																
		/********************输出（主机）端点3描述符******************/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */				//端点描述类型 0x04
    0x08,   /* bEndpointAddress: (OUT3) */																//端点：此描述表所描述的端点的地址、方向：
																																					//		Bit 3..0 : 端点号，Bit 6..4 : 保留,为零，Bit 7: 方向,如果控制端点则略。0：输出端点（主机到设备），1：输入端点（设备到主机）
    0x02,   /* bmAttributes: Bulk */																			//传输类型：Bit 1..0 :传送类型；00=控制传送，01=同步传送，10=批传送，11=中断传送
    VIRTUAL_COM_PORT_DATA_SIZE,             /* wMaxPacketSize: */					//端点能够接收或发送的最大数据包的大小。对于实进传输，此值用于为每帧的数据净负荷预留时间。在实际运行时，管道可能不完全需要预留的带宽，实际带宽可由设备通过一种非USB定义的机制汇报给主机。对于中断传输，批量传输和控制传输，端点可能发送比之短的数据包
    0x00,
    0x00,   /* bInterval: ignore for Bulk transfer */											//周期数据传输端点的时间间隙。此域的值对于批传送的端点及控制传送的端点无意义。对于同步传送的端点此域必需为1，表示周期为1ms。对于中断传送的端点此域值的范围为1ms到255ms。
    
		
		/*Endpoint 1 Descriptor*/
		/********************输入端点1描述符********************/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */				//端点描述类型 0x04
    0x85,   /* bEndpointAddress: (IN1) */																	//端点：此描述表所描述的端点的地址、方向：
																																					//		Bit 3..0 : 端点号，Bit 6..4 : 保留,为零，Bit 7: 方向,如果控制端点则略。0：输出端点（主机到设备），1：输入端点（设备到主机）
    0x02,   /* bmAttributes: Bulk */																			//传输类型：Bit 1..0 :传送类型；00=控制传送，01=同步传送，10=批传送，11=中断传送
    VIRTUAL_COM_PORT_DATA_SIZE,             /* wMaxPacketSize: */					//端点能够接收或发送的最大数据包的大小。对于实进传输，此值用于为每帧的数据净负荷预留时间。在实际运行时，管道可能不完全需要预留的带宽，实际带宽可由设备通过一种非USB定义的机制汇报给主机。对于中断传输，批量传输和控制传输，端点可能发送比之短的数据包
    0x00,
    0x00    /* bInterval */																								//周期数据传输端点的时间间隙。此域的值对于批传送的端点及控制传送的端点无意义。对于同步传送的端点此域必需为1，表示周期为1ms。对于中断传送的端点此域值的范围为1ms到255ms。
	
		
		
//		/*Data class interface descriptor*/
//		/********************数据类接口描述符********************/
//    0x09,   /* bLength: Endpoint Descriptor size */												//
//    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: */								//USB接口描述符 0X04
//    0x02,   /* bInterfaceNumber: Number of Interface */										//接口号，当前配置支持的接口数组索引（从零开始）。
//    0x00,   /* bAlternateSetting: Alternate setting */										//可选设置的索引值。
//    0x02,   /* bNumEndpoints: Two endpoints used */												//此接口用的端点数量，如果是零则说明此接口只用缺省控制管道。
//    0x0A,   /* bInterfaceClass: CDC */																		//接口所属的类值： 0值为将来的标准保留。 如果此域的值设为FFH，则此接口类由厂商说明。 所有其它的值由USB 说明保留。0x0A CDC数据类
//    0x00,   /* bInterfaceSubClass: */																			//子类码
//    0x00,   /* bInterfaceProtocol: */																			//协议码：bInterfaceClass 和bInterfaceSubClass 域的值而定.如果一个接口支持设备类相关的请求此域的值指出了设备类说明中所定义的协议.
//    0x00,   /* iInterface: */																							//描述此接口的字串描述表的索引值。   
//	 
//		
//		/*Endpoint 3 Descriptor*/																
//		/********************输出（主机）端点3描述符******************/
//    0x07,   /* bLength: Endpoint Descriptor size */
//    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */				//端点描述类型 0x04
//    0x05,   /* bEndpointAddress: (OUT3) */																//端点：此描述表所描述的端点的地址、方向：
//																																					//		Bit 3..0 : 端点号，Bit 6..4 : 保留,为零，Bit 7: 方向,如果控制端点则略。0：输出端点（主机到设备），1：输入端点（设备到主机）
//    0x02,   /* bmAttributes: Bulk */																			//传输类型：Bit 1..0 :传送类型；00=控制传送，01=同步传送，10=批传送，11=中断传送
//    VIRTUAL_COM_PORT_DATA_SIZE,             /* wMaxPacketSize: */					//端点能够接收或发送的最大数据包的大小。对于实进传输，此值用于为每帧的数据净负荷预留时间。在实际运行时，管道可能不完全需要预留的带宽，实际带宽可由设备通过一种非USB定义的机制汇报给主机。对于中断传输，批量传输和控制传输，端点可能发送比之短的数据包
//    0x00,
//    0x00,   /* bInterval: ignore for Bulk transfer */											//周期数据传输端点的时间间隙。此域的值对于批传送的端点及控制传送的端点无意义。对于同步传送的端点此域必需为1，表示周期为1ms。对于中断传送的端点此域值的范围为1ms到255ms。
//    
//		
//		/*Endpoint 1 Descriptor*/
//		/********************输入端点1描述符********************/
//    0x07,   /* bLength: Endpoint Descriptor size */
//    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */				//端点描述类型 0x04
//    0x86,   /* bEndpointAddress: (IN1) */																	//端点：此描述表所描述的端点的地址、方向：
//																																					//		Bit 3..0 : 端点号，Bit 6..4 : 保留,为零，Bit 7: 方向,如果控制端点则略。0：输出端点（主机到设备），1：输入端点（设备到主机）
//    0x02,   /* bmAttributes: Bulk */																			//传输类型：Bit 1..0 :传送类型；00=控制传送，01=同步传送，10=批传送，11=中断传送
//    VIRTUAL_COM_PORT_DATA_SIZE,             /* wMaxPacketSize: */					//端点能够接收或发送的最大数据包的大小。对于实进传输，此值用于为每帧的数据净负荷预留时间。在实际运行时，管道可能不完全需要预留的带宽，实际带宽可由设备通过一种非USB定义的机制汇报给主机。对于中断传输，批量传输和控制传输，端点可能发送比之短的数据包
//    0x00,
//    0x00    /* bInterval */																								//周期数据传输端点的时间间隙。此域的值对于批传送的端点及控制传送的端点无意义。对于同步传送的端点此域必需为1，表示周期为1ms。对于中断传送的端点此域值的范围为1ms到255ms。
		
		
};
///* USB配置描述符集合(配置、接口、端点、类、厂商)(Configuration, Interface, Endpoint, Class, Vendor */
//const u8 Virtual_Com_Port_ConfigDescriptor[] =			//配置描述符
//  {
//    /*Configuation Descriptor*/
//		
//    0x09,   /* bLength: Configuation Descriptor size */																		//整个描述符长度--0x09/9个字节
//    USB_CONFIGURATION_DESCRIPTOR_TYPE,      /* bDescriptorType: Configuration */					//类别--0X02设备描述符
//    VIRTUAL_COM_PORT_SIZ_CONFIG_DESC,       /* wTotalLength:no of returned bytes */				//此配置信息的总长（包括配置，接口，端点和设备类及厂商定义的描述符）
//    0x00,
//    0x02,   /* bNumInterfaces: 2 interface */																							//此配置所支持的接口个数
//    0x01,   /* bConfigurationValue: Configuration value */																//在SetConfiguration（）请求中用作参数来选定此配置。
//    0x00,   /* iConfiguration: Index of string descriptor describing the configuration */	//描述此配置的字串描述表索引
//    0xC0,   /* bmAttributes: self powered */																							//配置特性：D7： 保留（设为一），D6： 自给电源，D5： 远程唤醒，D4..0：保留（设为一）
//    0x32,   /* MaxPower 0 mA */																														//在此配置下的总线电源耗费量。以2mA 为一个单位。
//    /*Interface Descriptor*/																							/*接口描述符*/
//		
//		
//		/**************  接口描述符****************/
//    0x09,   /* bLength: Interface Descriptor size */											//整个描述符长度--0x09/9个字节
//    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: Interface */			//接口描述表类（此处应为0x04）
//    /* Interface descriptor type */														/*接口类型描述*/
//    0x00,   /* bInterfaceNumber: Number of Interface */										//接口号，当前配置支持的接口数组索引（从零开始）。
//    0x00,   /* bAlternateSetting: Alternate setting */										//可选设置的索引值。
//    0x01,   /* bNumEndpoints: One endpoints used */												//此接口用的端点数量，如果是0则说明此接口只用缺省控制管道。
//    0x02,   /* bInterfaceClass: Communication Interface Class */					//接口所属的类值：0值为将来的标准保留。 如果此域的值设为FFH，则此接口类由厂商说明。 所有其它的值由USB说明保留。,0x02 CDC控制类
//    0x02,   /* bInterfaceSubClass: Abstract Control Model */							//子类码:这些值的定义视bInterfaceClass域而定。 如果bInterfaceClass域的值为零则此域的值必须为零。 bInterfaceClass域不为FFH则所有值由USB 所保留。
//    0x01,   /* bInterfaceProtocol: Common AT commands */									//协议码：bInterfaceClass 和bInterfaceSubClass 域的值而定.如果一个接口支持设备类相关的请求此域的值指出了设备类说明中所定义的协议.
//    0x00,   /* iInterface: */																							//描述此接口的字串描述表的索引值。
//		
//		
//    /*Header Functional Descriptor*/													/*头功能描述*/
//		/****************头功能描述符***************/
//    0x05,   /* bLength: Endpoint Descriptor size */												//描述符长度--0x05/5个字节
//    0x24,   /* bDescriptorType: CS_INTERFACE */														//配置描述符类型
//    0x00,   /* bDescriptorSubtype: Header Func Desc */
//    0x10,   /* bcdCDC: spec release number */
//    0x01,
//		
//		
//    /*Call Managment Functional Descriptor*/
//		/*************调用管理功能描述符*************/
//    0x05,   /* bFunctionLength */																					//描述符长度
//    0x24,   /* bDescriptorType: CS_INTERFACE */
//    0x01,   /* bDescriptorSubtype: Call Management Func Desc */
//    0x00,   /* bmCapabilities: D0+D1 */
//    0x01,   /* bDataInterface: 1 */
//		
//		
//		
//    /*ACM Functional Descriptor*/
//		/**************ACM 功能描述符****************/
//    0x04,   /* bFunctionLength */																					//描述符长度
//    0x24,   /* bDescriptorType: CS_INTERFACE */
//    0x02,   /* bDescriptorSubtype: Abstract Control Management desc */
//    0x02,   /* bmCapabilities */
//		
//		
//    /*Union Functional Descriptor*/
//		/***************联合功能描述符**************/
//    0x05,   /* bFunctionLength */																					//描述符长度
//    0x24,   /* bDescriptorType: CS_INTERFACE */
//    0x06,   /* bDescriptorSubtype: Union func desc */
//    0x00,   /* bMasterInterface: Communication class interface */
//    0x01,   /* bSlaveInterface0: Data Class Interface */
//		
//		
//		
//    /*Endpoint 2 Descriptor*/																	/*端点2描述*/
//		/********************输入端点2描述符******************/
//    0x07,   /* bLength: Endpoint Descriptor size */
//    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */				//端点描述类型 0x04
//    0x82,   /* bEndpointAddress: (IN2) */																	//端点：此描述表所描述的端点的地址、方向：
//																																					//		Bit 3..0 : 端点号，Bit 6..4 : 保留,为零，Bit 7: 方向,如果控制端点则略。0：输出端点（主机到设备），1：输入端点（设备到主机）
//   
//		0x03,   /* bmAttributes: Interrupt */																	//传输类型：Bit 1..0 :传送类型；00=控制传送，01=同步传送，10=批传送，11=中断传送
//    VIRTUAL_COM_PORT_INT_SIZE,      /* wMaxPacketSize: */									//端点能够接收或发送的最大数据包的大小。对于实进传输，此值用于为每帧的数据净负荷预留时间。在实际运行时，管道可能不完全需要预留的带宽，实际带宽可由设备通过一种非USB定义的机制汇报给主机。对于中断传输，批量传输和控制传输，端点可能发送比之短的数据包
//    0x00,
//    0xFF,   /* bInterval: */																							//周期数据传输端点的时间间隙。此域的值对于批传送的端点及控制传送的端点无意义。对于同步传送的端点此域必需为1，表示周期为1ms。对于中断传送的端点此域值的范围为1ms到255ms。
//    
//		
//		/*Data class interface descriptor*/
//		/********************数据类接口描述符********************/
//    0x09,   /* bLength: Endpoint Descriptor size */												//
//    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: */								//USB接口描述符 0X04
//    0x01,   /* bInterfaceNumber: Number of Interface */										//接口号，当前配置支持的接口数组索引（从零开始）。
//    0x00,   /* bAlternateSetting: Alternate setting */										//可选设置的索引值。
//    0x02,   /* bNumEndpoints: Two endpoints used */												//此接口用的端点数量，如果是零则说明此接口只用缺省控制管道。
//    0x0A,   /* bInterfaceClass: CDC */																		//接口所属的类值： 0值为将来的标准保留。 如果此域的值设为FFH，则此接口类由厂商说明。 所有其它的值由USB 说明保留。0x0A CDC数据类
//    0x00,   /* bInterfaceSubClass: */																			//子类码
//    0x00,   /* bInterfaceProtocol: */																			//协议码：bInterfaceClass 和bInterfaceSubClass 域的值而定.如果一个接口支持设备类相关的请求此域的值指出了设备类说明中所定义的协议.
//    0x00,   /* iInterface: */																							//描述此接口的字串描述表的索引值。   
//	 
//		
//		/*Endpoint 3 Descriptor*/																/*端点3描述*/
//		/********************输出端点3描述符******************/
//    0x07,   /* bLength: Endpoint Descriptor size */
//    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */				//端点描述类型 0x04
//    0x03,   /* bEndpointAddress: (OUT3) */																//端点：此描述表所描述的端点的地址、方向：
//																																					//		Bit 3..0 : 端点号，Bit 6..4 : 保留,为零，Bit 7: 方向,如果控制端点则略。0：输出端点（主机到设备），1：输入端点（设备到主机）
//    0x02,   /* bmAttributes: Bulk */																			//传输类型：Bit 1..0 :传送类型；00=控制传送，01=同步传送，10=批传送，11=中断传送
//    VIRTUAL_COM_PORT_DATA_SIZE,             /* wMaxPacketSize: */					//端点能够接收或发送的最大数据包的大小。对于实进传输，此值用于为每帧的数据净负荷预留时间。在实际运行时，管道可能不完全需要预留的带宽，实际带宽可由设备通过一种非USB定义的机制汇报给主机。对于中断传输，批量传输和控制传输，端点可能发送比之短的数据包
//    0x00,
//    0x00,   /* bInterval: ignore for Bulk transfer */											//周期数据传输端点的时间间隙。此域的值对于批传送的端点及控制传送的端点无意义。对于同步传送的端点此域必需为1，表示周期为1ms。对于中断传送的端点此域值的范围为1ms到255ms。
//    
//		
//		/*Endpoint 1 Descriptor*/
//		/********************输入端点1描述符********************/
//    0x07,   /* bLength: Endpoint Descriptor size */
//    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */				//端点描述类型 0x04
//    0x81,   /* bEndpointAddress: (IN1) */																	//端点：此描述表所描述的端点的地址、方向：
//																																					//		Bit 3..0 : 端点号，Bit 6..4 : 保留,为零，Bit 7: 方向,如果控制端点则略。0：输出端点（主机到设备），1：输入端点（设备到主机）
//    0x02,   /* bmAttributes: Bulk */																			//传输类型：Bit 1..0 :传送类型；00=控制传送，01=同步传送，10=批传送，11=中断传送
//    VIRTUAL_COM_PORT_DATA_SIZE,             /* wMaxPacketSize: */					//端点能够接收或发送的最大数据包的大小。对于实进传输，此值用于为每帧的数据净负荷预留时间。在实际运行时，管道可能不完全需要预留的带宽，实际带宽可由设备通过一种非USB定义的机制汇报给主机。对于中断传输，批量传输和控制传输，端点可能发送比之短的数据包
//    0x00,
//    0x00,    /* bInterval */																								//周期数据传输端点的时间间隙。此域的值对于批传送的端点及控制传送的端点无意义。对于同步传送的端点此域必需为1，表示周期为1ms。对于中断传送的端点此域值的范围为1ms到255ms。
//			
//	/* Interface, Alternate Setting 0, HID Class */                                                           \
//  0x09,              /* bLength */                                                       \
//  0x04,        /* bDescriptorType */                                               \
//  0x02,                      /* bInterfaceNumber */                                              \
//  0x00,                                 /* bAlternateSetting */                                             \
//  0x02,       /* bNumEndpoints */                                                 \
//  0x03,     /* bInterfaceClass */                                               \
//  0x00,                    /* bInterfaceSubClass */                                            \
//  0x00,                    /* bInterfaceProtocol */                                            \
//  0x03,                  /* iInterface */                                                    \
//                                                                                                            \
///* HID Class Descriptor */                                                                                  \
//  0x07,                    /* bLength */                                                       \
//  0x21,              /* bDescriptorType */                                               \
//  0x01, /* 1.00 */             /* bcdHID */                                                        \
//  0x00,                                 /* bCountryCode */                                                  \
//  0x01,                                 /* bNumDescriptors */                                               \
//  0x22,           /* bDescriptorType */                                               \
//  0x22,      /* wDescriptorLength */
//	
//	
//	/* Endpoint, HID Interrupt In */                                                                            \
//  0x07,               /* bLength */                                                       \
//  0x05,         /* bDescriptorType */                                               \
//  0x86,   /* bEndpointAddress */                                              \
//  0x03,          /* bmAttributes */                                                  \
//	0x40,
//  0x00,       /* wMaxPacketSize */                                                \
//  0x01,                   /* bInterval */                                                     \
//                                                                                                            \
///* Endpoint, HID Interrupt Out */                                                                           \
//  0x07,               /* bLength */                                                       \
//  0x05,         /* bDescriptorType */                                               \
//  0x07, /* bEndpointAddress */                                              \
//  0x03,          /* bmAttributes */                                                  \
//	0x40,
//  0x00,       /* wMaxPacketSize */                                                \
//  0x01                   /* bInterval */	
//};

/* USB String Descriptors */
const u8 Virtual_Com_Port_StringLangID[VIRTUAL_COM_PORT_SIZ_STRING_LANGID] =		//字符串描述
  {
    VIRTUAL_COM_PORT_SIZ_STRING_LANGID,						//此描述表的字节数
    USB_STRING_DESCRIPTOR_TYPE,										//字串描述表类型（此处应为0x03）
    0x09,																					//语言标识（LANGID） 码0
    0x04 /* LangID = 0x0409: U.S. English */			//语言标识（LANGID） 码X
  };

	
const u8 Virtual_Com_Port_StringVendor[VIRTUAL_COM_PORT_SIZ_STRING_VENDOR] =		//
  {
    VIRTUAL_COM_PORT_SIZ_STRING_VENDOR,     /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,             /* bDescriptorType	*/
    /* Manufacturer: "STMicroelectronics" */
    'S', 0, 'T', 0, 'M', 0, 'i', 0, 'c', 0, 'r', 0, 'o', 0, 'e', 0,
    'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0, 'i', 0,
    'c', 0, 's', 0
  };

const u8 Virtual_Com_Port_StringProduct[VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT] =
  {
    VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT,          /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    /* Product name: "STM32 Virtual COM Port" */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, ' ', 0, 'V', 0, 'i', 0,
    'r', 0, 't', 0, 'u', 0, 'a', 0, 'l', 0, ' ', 0, 'C', 0, 'O', 0,
    'M', 0, ' ', 0, 'P', 0, 'o', 0, 'r', 0, 't', 0, ' ', 0, ' ', 0
  };

u8 Virtual_Com_Port_StringSerial[VIRTUAL_COM_PORT_SIZ_STRING_SERIAL] =
  {
    VIRTUAL_COM_PORT_SIZ_STRING_SERIAL,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, '1', 0, '0', 0
  }
  ;/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
