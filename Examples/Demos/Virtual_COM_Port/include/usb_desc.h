/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_desc.h
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Descriptor Header for Virtual COM Port Device
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DESC_H
#define __USB_DESC_H

#include "usb_type.h"			//主要是用 typedef为 stm32支持的数据类型取一些新的名称/
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/

//**************描述符类型 bDescriptorType
#define USB_DEVICE_DESCRIPTOR_TYPE              		0x01		//USB设备描述符类型
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       		0x02		//USB配置描述符类型
#define USB_STRING_DESCRIPTOR_TYPE              		0x03		//USB字符串描述符类型
#define USB_INTERFACE_DESCRIPTOR_TYPE           		0x04		//USB接口描述符类型
#define USB_ENDPOINT_DESCRIPTOR_TYPE            		0x05		//USB端点描述符类型
#define USB_INTERFACE_ASSOCIATION_DESCRIPTOR_TYPE 	0x0B		//USB_IAD描述符类型
//**************设备类/接口代码 bDeviceClass/bInterfaceClass
#define	USB_Interface_Descriptors_DeviceClass				0x00		//设备类型在接口描述符中说明 
#define	USB_Audio_InterfaceClass										0x01		//音频接口类
#define	USB_Communications_DeviceClass							0x02		//通讯设备类
#define	USB_CDC_InterfaceClass											0x02		//通讯接口类
#define	USB_HID_InterfaceClass											0x03		//人机接口类
#define	USB_Physical_InterfaceClass									0x05		//物理接口类 
#define	USB_Image_InterfaceClass										0x06		//图像接口类 
#define	USB_Printer_InterfaceClass 									0x07		//打印接口类
#define	USB_MSC_InterfaceClass											0x08		//大数据存储接口类
#define	USB_HUB_DeviceClass													0x09		//集线器设备类
#define	CDC_DATA_INTERFACE_CLASS 										0x0A		//CDC数据接口类
#define	USB_Smart_Card_InterfaceClass								0x0B		//智能卡接口类 
#define	USB_Content_Security_InterfaceClass					0x0D		//安全接口类
#define	USB_Video_InterfaceClass										0x0E		//视频接口类
#define	USB_Diagnostic_Device_DeviceClass						0xDC		//诊断设备类
#define	USB_Diagnostic_Device_InterfaceClass 				0xDC		//诊断接口类
#define	USB_Wireless_Controller_InterfaceClass			0xE0		//无线控制器接口类 
#define	USB_Miscellaneous_DeviceClass 							0xEF		//混合设备类
#define	USB_Miscellaneous_InterfaceClass 						0xEF		//混合设备接口类
#define	USB_Application_Specific_InterfaceClass			0xFE		//特定应用接口类（包括红外的桥接器等） 
#define	USB_Vendor_Specific_DeviceClass							0xFF		//厂商定义的设备类
#define	USB_Vendor_Specific_InterfaceClass					0xFF		//厂商定义的设备接口类

/* Communication device class specification version 1.10 */
#define CDC_V1_10                               		0x0110
//**************CDC相关参数
#define	CDC_ABSTRACT_CONTROL_MODEL									0x02
#define	CDC_CS_INTERFACE														0x24	//描述符类型为类特殊接口（CS_INTERFACE）编号为0x24。
#define	CDC_HEADER																	0x00	//描述符子类为Header Functional Descriptor 编号为0x00 
#define	CDC_CALL_MANAGEMENT													0x01	//描述符子类为Call Management 编号为0x01。
#define	CDC_ABSTRACT_CONTROL_MANAGEMENT							0x02	//描述符子类为Abstract Control Management Functional Descriptor 编号为0x02。
#define	CDC_UNION																		0x06	//描述符子类为Union functional descriptor，编号为0x06。

//**************USB端点传输类型
#define USB_ENDPOINT_TYPE_CONTROL              			0x00	//控制传输
#define USB_ENDPOINT_TYPE_ISOCHRONOUS          			0x01	//同步传输
#define USB_ENDPOINT_TYPE_BULK                 			0x02	//块传输
#define USB_ENDPOINT_TYPE_INTERRUPT            			0x03	//中断传输


#define VIRTUAL_COM_PORT_DATA_SIZE              64
#define VIRTUAL_COM_PORT_INT_SIZE               8

#define VIRTUAL_COM_PORT_SIZ_DEVICE_DESC        18			//设备描述符总长度
//#define VIRTUAL_COM_PORT_SIZ_CONFIG_DESC        207			// 67/83/97配置描述符总长度
#define VIRTUAL_COM_PORT_SIZ_STRING_LANGID      4				//字符串LANGID描述符总长度
#define VIRTUAL_COM_PORT_SIZ_STRING_VENDOR      38			//字符串VENDOR描述符总长度
#define VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT     50			//字符串PRODUCT描述符总长度
#define VIRTUAL_COM_PORT_SIZ_STRING_SERIAL      26			//字符串SERIAL描述符总长度

//#define STANDARD_ENDPOINT_DESC_SIZE             0x09
#define USB_CONFIG_BUS_POWERED                 0x80
#define USB_CONFIG_SELF_POWERED                0xC0
#define USB_CONFIG_POWER_MA(mA)                ((mA)/2)
/* bEndpointAddress in Endpoint Descriptor */
#define USB_ENDPOINT_DIRECTION_MASK            0x80
#define USB_ENDPOINT_OUT(addr)                 ((addr) | 0x00)
#define USB_ENDPOINT_IN(addr)                  ((addr) | 0x80)


#define VIRTUAL_COM_PORT_SIZ_CONFIG_DESC        (USB_CONFIGURATION_DESC_SIZE	+	(USB_NUM_INTERFACES/2) * IAD_CDC_IF_DESC_SET_SIZE)			// 67/83/97配置描述符总长度---test	
//#define VIRTUAL_COM_PORT_SIZ_CONFIG_DESC        (u16)(1500)			// 67/83/97配置描述符总长度---test	

// Interface numbers
enum {
    USB_CDC_CIF_NUM0,			//CDC通讯接口0
    USB_CDC_DIF_NUM0,			//CDC数据接口0
//    USB_CDC_CIF_NUM1,			//CDC通讯接口1
//    USB_CDC_DIF_NUM1,			//CDC数据接口1
//    USB_CDC_CIF_NUM2,			//CDC通讯接口2
//    USB_CDC_DIF_NUM2,			//CDC数据接口2
//		USB_CDC_CIF_NUM3,			//CDC通讯接口3
//    USB_CDC_DIF_NUM3,			//CDC数据接口3	
//		USB_CDC_CIF_NUM4,			//CDC通讯接口4
//    USB_CDC_DIF_NUM4,			//CDC数据接口4
//	
//		USB_CDC_CIF_NUM5,			//CDC通讯接口5
//		USB_CDC_DIF_NUM5,			//CDC数据接口5
//		USB_CDC_CIF_NUM6,			//CDC通讯接口6
//		USB_CDC_DIF_NUM6,			//CDC数据接口6
//		USB_CDC_CIF_NUM7,			//CDC通讯接口7
//		USB_CDC_DIF_NUM7,			//CDC数据接口7
//		USB_CDC_CIF_NUM8,			//CDC通讯接口8
//		USB_CDC_DIF_NUM8,			//CDC数据接口8

//		USB_CDC_CIF_NUM9,			//CDC通讯接口9
//    USB_CDC_DIF_NUM9,			//CDC数据接口9
//		USB_CDC_CIF_NUM10,		//CDC通讯接口10
//    USB_CDC_DIF_NUM10,		//CDC数据接口10
//		USB_CDC_CIF_NUM11,		//CDC通讯接口11
//    USB_CDC_DIF_NUM11,		//CDC数据接口11
//		
//		USB_CDC_CIF_NUM12,		//CDC通讯接口12
//    USB_CDC_DIF_NUM12,		//CDC数据接口12
//		
//		USB_CDC_CIF_NUM13,		//CDC通讯接口13
//    USB_CDC_DIF_NUM13,		//CDC数据接口13
//		USB_CDC_CIF_NUM14,		//CDC通讯接口14
//    USB_CDC_DIF_NUM14,		//CDC数据接口14
//		USB_CDC_CIF_NUM15,		//CDC通讯接口15
//    USB_CDC_DIF_NUM15,		//CDC数据接口15
//		USB_CDC_CIF_NUM16,		//CDC通讯接口16
//    USB_CDC_DIF_NUM16,		//CDC数据接口16
    
    USB_NUM_INTERFACES   	//接口数量
};

/* Exported functions ------------------------------------------------------- */
extern const u8 Virtual_Com_Port_DeviceDescriptor[VIRTUAL_COM_PORT_SIZ_DEVICE_DESC];
//extern const u8 Virtual_Com_Port_ConfigDescriptor[VIRTUAL_COM_PORT_SIZ_CONFIG_DESC];
extern const u8 Virtual_Com_Port_ConfigDescriptor[];			//---数组不设置长度--长度不确定
extern const u8 USBD_DeviceQualifier[10];

extern const u8 Virtual_Com_Port_StringLangID[VIRTUAL_COM_PORT_SIZ_STRING_LANGID];
extern const u8 Virtual_Com_Port_StringVendor[VIRTUAL_COM_PORT_SIZ_STRING_VENDOR];
extern const u8 Virtual_Com_Port_StringProduct[VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT];
extern u8 Virtual_Com_Port_StringSerial[VIRTUAL_COM_PORT_SIZ_STRING_SERIAL];


//*******************************************************相关描述符长度计算*******************************************************//
#define USB_DEVICE_DESC_SIZE     									(sizeof(USB_DEVICE_DESCRIPTOR))											//获取设备描述符长度	0X12
#define USB_DEVICE_QUALIFIER_DESC_SIZE     				(sizeof(USB_DEVICE_QUALIFIER_DESCRIPTOR))						//获取USB 2.0设备限定描述符长度
#define USB_CONFIGURATION_DESC_SIZE     					(sizeof(USB_CONFIGURATION_DESCRIPTOR))							//获取配置描述符长度	0X09
#define USB_INTERFACE_DESC_SIZE     							(sizeof(USB_INTERFACE_DESCRIPTOR))									//获取接口描述符长度	0X09
#define USB_ENDPOINT_DESC_SIZE     								(sizeof(USB_ENDPOINT_DESCRIPTOR))										//获取端点描述符长度	0X07
#define USB_COMMON_DESC_SIZE     									(sizeof(USB_COMMON_DESCRIPTOR))											//获取字符串描述符长度
#define USB_Header_Functional_DESC_SIZE     			(sizeof(USB_Header_Functional_Descriptor))					//获取公共描述符长度	0X05
#define USB_Call_Managment_Functional_DESC_SIZE 	(sizeof(USB_Call_Managment_Functional_Descriptor))	//获取调用管理功能描述符	0X05
#define USB_ACM_Functional_DESC_SIZE     					(sizeof(USB_ACM_Functional_Descriptor))							//获取抽象控制管理功能描述符长度	0X04
#define USB_Union_Functional_DESC_SIZE     				(sizeof(Union_Functional_Descriptor))								//获取抽象控制管理功能描述符长度	0X05
#define USB_INTERFACE_ASSOC_DESC_SIZE     				(sizeof(USB_INTERFACE_ASSOCIATION_DESCRIPTOR))			//获取IAD描述符长度		0x08
	


#define WBVAL(x) (x & 0xFF),((x >> 8) & 0xFF)			//双字节拆分为两个单字节


//CDC接口描述符长度
#define CDC_IF_DESC_SET_SIZE	(		USB_INTERFACE_DESC_SIZE									/*接口描述符长度	0X09*/							\
																+ USB_Header_Functional_DESC_SIZE					/*头功能描述符长度	0X05*/						\
																+ USB_Call_Managment_Functional_DESC_SIZE	/*调用管理功能描述符长度	0X05*/			\
																+	USB_ACM_Functional_DESC_SIZE						/*抽象控制管理功能描述符长度	0X04*/	\
																+ USB_Union_Functional_DESC_SIZE					/*联合功能描述符长度	0X05	*/					\
																+ USB_ENDPOINT_DESC_SIZE									/*端点描述符长度	0X07*/							\
																+ USB_INTERFACE_DESC_SIZE									/*接口描述符长度	0X09*/							\
																+ 2 * USB_ENDPOINT_DESC_SIZE 							/*端点描述符长度	0X0E	*/	)

//组合CDC接口描述符长度		
#define IAD_CDC_IF_DESC_SET_SIZE    ( USB_INTERFACE_ASSOC_DESC_SIZE			/*IAD描述符长度	0x08	*/ 	\
																		+ CDC_IF_DESC_SET_SIZE							/*CDC接口描述符长度	0x3A*/)


//CDC接口描述符设置
#define CDC_IF_DESC_SET( 					comIfNum,		/*通讯接口序号*/				\
																	datIfNum, 	/*数据接口序号*/				\
																	comInEp, 		/*通讯（IN）端点*/			\
																	datOutEp, 	/*数据（OUT）端点*/		\
																	datInEp			/*数据（IN）端点*/	)		\
/* CDC Communication Interface Descriptor */                            \
    USB_INTERFACE_DESC_SIZE,                /* bLength */               \
    USB_INTERFACE_DESCRIPTOR_TYPE,          /* bDescriptorType */       \
    comIfNum,                               /* bInterfaceNumber */      \
    0x00,                                   /* bAlternateSetting */     \
    0x01,                                   /* bNumEndpoints */         \
    USB_CDC_InterfaceClass,      						/* bInterfaceClass */       \
    CDC_ABSTRACT_CONTROL_MODEL,             /* bInterfaceSubClass */    \
    0x01,                                   /* bInterfaceProtocol */    \
    0x00,                                   /* iInterface */            \
/* Header Functional Descriptor */                                      \
    0x05,                                   /* bLength */               \
    CDC_CS_INTERFACE,                       /* bDescriptorType */       \
    CDC_HEADER,                             /* bDescriptorSubtype */    \
    WBVAL(CDC_V1_10), /* 1.10 */            /* bcdCDC */                \
/* Call Management Functional Descriptor */                             \
    0x05,                                   /* bFunctionLength */       \
    CDC_CS_INTERFACE,                       /* bDescriptorType */       \
    CDC_CALL_MANAGEMENT,                    /* bDescriptorSubtype */    \
    0x03,                                   /* bmCapabilities */        \
    datIfNum,                               /* bDataInterface */        \
/* Abstract Control Management Functional Descriptor */                 \
    0x04,                                   /* bFunctionLength */       \
    CDC_CS_INTERFACE,                       /* bDescriptorType */       \
    CDC_ABSTRACT_CONTROL_MANAGEMENT,        /* bDescriptorSubtype */    \
    0x02,                                   /* bmCapabilities */        \
/* Union Functional Descriptor */                                       \
    0x05,                                   /* bFunctionLength */       \
    CDC_CS_INTERFACE,                       /* bDescriptorType */       \
    CDC_UNION,                              /* bDescriptorSubtype */    \
    comIfNum,                               /* bMasterInterface */      \
    datIfNum,                               /* bSlaveInterface0 */      \
/* Endpoint, Interrupt IN */                /* event notification */    \
    USB_ENDPOINT_DESC_SIZE,                 /* bLength */               \
    USB_ENDPOINT_DESCRIPTOR_TYPE,           /* bDescriptorType */       \
    comInEp,                                /* bEndpointAddress */      \
    USB_ENDPOINT_TYPE_INTERRUPT,            /* bmAttributes */          \
    WBVAL(0x000A),                          /* wMaxPacketSize */        \
    0x01,                                   /* bInterval */             \
                                                                        \
/* CDC Data Interface Descriptor */                                     \
    USB_INTERFACE_DESC_SIZE,                /* bLength */               \
    USB_INTERFACE_DESCRIPTOR_TYPE,          /* bDescriptorType */       \
    datIfNum,                               /* bInterfaceNumber */      \
    0x00,                                   /* bAlternateSetting */     \
    0x02,                                   /* bNumEndpoints */         \
    CDC_DATA_INTERFACE_CLASS,               /* bInterfaceClass */       \
    0x00,                                   /* bInterfaceSubClass */    \
    0x00,                                   /* bInterfaceProtocol */    \
    0x00,                                   /* iInterface */            \
/* Endpoint, Bulk OUT */                                                \
    USB_ENDPOINT_DESC_SIZE,                 /* bLength */               \
    USB_ENDPOINT_DESCRIPTOR_TYPE,           /* bDescriptorType */       \
    datOutEp,                               /* bEndpointAddress */      \
    USB_ENDPOINT_TYPE_BULK,                 /* bmAttributes */          \
    WBVAL(VIRTUAL_COM_PORT_DATA_SIZE),      /* wMaxPacketSize */        \
    0x00,                                   /* bInterval */             \
/* Endpoint, Bulk IN */                                                 \
    USB_ENDPOINT_DESC_SIZE,                 /* bLength */               \
    USB_ENDPOINT_DESCRIPTOR_TYPE,           /* bDescriptorType */       \
    datInEp,                                /* bEndpointAddress */      \
    USB_ENDPOINT_TYPE_BULK,                 /* bmAttributes */          \
    WBVAL(VIRTUAL_COM_PORT_DATA_SIZE),      /* wMaxPacketSize */        \
    0x00																		/* bInterval */
		

//IAD_CDC描述符设置
#define IAD_CDC_IF_DESC_SET( 			comIfNum,		/*通讯接口序号*/				\
																	datIfNum, 	/*数据接口序号*/				\
																	comInEp, 		/*通讯（IN）端点*/			\
																	datOutEp, 	/*数据（OUT）端点*/		\
																	datInEp			/*数据（IN）端点*/	)		\
		/* IAD描述符配置 */                                  							\
    0x08,                                   /* bLength */           	\
    0x0B,                                   /* bDescriptorType */    	\
    comIfNum,                               /* bFirstInterface */  		\
    0x02,                                   /* bInterfaceCount */   	\
    USB_Communications_DeviceClass,      		/* bFunctionClass */     	\
    CDC_ABSTRACT_CONTROL_MODEL,             /* bFunctionSubClass */  	\
    0x01,                                   /* bFunctionProcotol */ 	\
    0x00,                                   /* iInterface */       		\
		/* CDC接口描述符配置 */                                     				\
    CDC_IF_DESC_SET(comIfNum,	datIfNum,	comInEp,	datOutEp,	datInEp)


	






//********************************************************相关描述符结构体********************************************************//
//************设备描述符结构体*************//
/* USB Standard Device Descriptor */
typedef __packed struct _USB_DEVICE_DESCRIPTOR
{
  u8  bLength;						//描述设备描述符的总字节数
  u8  bDescriptorType;		//描述符的类型（为0X01，这里是设备描述符）
  u16 bcdUSB;							//这个设备兼容的USB设备版本号
  u8  bDeviceClass;				//设备类码：是由USB 协会规定的，描述的是接口所能实现的功能。当此域为0时下面的子类也必须为0，当为0XFF表示的是厂商自定义设备类
  u8  bDeviceSubClass;		//子类代码码：这个码值的意思是根据设备类码来看。如设备类码为零，这字段也要零，如设备类码为0XFF，此域的所有值保留。	
  u8  bDeviceProtocol;		//协议码：这些码的值视设备码和子类代码的值而定。当该字段为0是，表示设备不使用类所定义的协议， 当该字段的值为0XFF时，表示使用设备厂商自定义的协议	
  u8  bMaxPacketSize0;		//端点0的能缓冲的最大数据包大小
  u16 idVendor;						//生产设备厂家的标志（由USB 相关组织给的）
  u16 idProduct;					//产品标志（由生产的厂家自己做编号）
  u16 bcdDevice;					//设备的版本号
  u8  iManufacturer;			//描述生产设备厂家的信息的字符串描述符的索引值。
  u8  iProduct;						//描述所使用设备产品的信息的字串描述符的索引值。
  u8  iSerialNumber;			//描述设备产品的序列号信息的字串描述符的索引值。
  u8  bNumConfigurations;	//设备有多少种配置
} USB_DEVICE_DESCRIPTOR;

//************USB 2.0设备限定描述符结构体*************//
/* USB 2.0 Device Qualifier Descriptor */
typedef __packed struct _USB_DEVICE_QUALIFIER_DESCRIPTOR
{
  u8  bLength;						//长度
  u8  bDescriptorType;		//类型（0x06)
  u16 bcdUSB;							//USB版本
  u8  bDeviceClass;				//设备类代码
  u8  bDeviceSubClass;		//子设备类
  u8  bDeviceProtocol;		//设备协议
  u8  bMaxPacketSize0;		//最大数据包
  u8  bNumConfigurations;	//配置数量
  u8  bReserved;					//保留
} USB_DEVICE_QUALIFIER_DESCRIPTOR;

//************配置描述符结构体*************//
/* USB Standard Configuration Descriptor */
typedef __packed struct _USB_CONFIGURATION_DESCRIPTOR {
  u8  bLength;									//整个描述符长度--0x09/9个字节
  u8  bDescriptorType;					//类别--0X02设备描述符
  u16 wTotalLength;							//此配置信息的总长（包括配置，接口，端点和设备类及厂商定义的描述符）
  u8  bNumInterfaces;						//此配置所支持的接口个数
  u8  bConfigurationValue;			//在SetConfiguration（）请求中用作参数来选定此配置。
  u8  iConfiguration;						//描述此配置的字串描述表索引
  u8  bmAttributes;							//配置特性：D7： 保留（设为一），D6： 自给电源，D5： 远程唤醒，D4..0：保留（设为一）
  u8  bMaxPower;								//在此配置下的总线电源耗费量。以2mA 为一个单位。
} USB_CONFIGURATION_DESCRIPTOR;

//************接口描述符结构体*************//
/* USB Standard Interface Descriptor */
typedef __packed struct _USB_INTERFACE_DESCRIPTOR
{
  u8  bLength;							/* bLength */							//接口描述表长度--0x09/9个字节
  u8  bDescriptorType;			/* bDescriptorType */			//接口描述表类（此处应为0x04）接口描述符
  u8  bInterfaceNumber;			/* bInterfaceNumber */		//接口号，当前配置支持的接口数组索引（从零开始）。该接口的编号，第一个接口，编号为0。
  u8  bAlternateSetting;		/* bAlternateSetting */		//备用的接口描述符编号，可选设置的索引值
  u8  bNumEndpoints;				/* bNumEndpoints */				//该接口使用的端点数，不包括端点0，如果是0则说明此接口只用缺省控制管道。非0端点的数目。CDC接口只使用一个中断输入端点。
  u8  bInterfaceClass;			/* bInterfaceClass */			//接口所属的类值：0值为将来的标准保留。 如果此域的值设为FFH，则此接口类由厂商说明。 所有其它的值由USB说明保留。,（例0x02 CDC类）
  u8  bInterfaceSubClass;		/* bInterfaceSubClass */	//接口子类型
  u8  bInterfaceProtocol;		/* bInterfaceProtocol */	//接口遵循的协议
  u8  iInterface;						/* iInterface */					//描述此接口的字串描述表的索引值。该接口的字符串索引值。这里没有，为0。
} USB_INTERFACE_DESCRIPTOR;

//************端点描述符结构体*************//
/* USB Standard Endpoint Descriptor */
typedef __packed struct _USB_ENDPOINT_DESCRIPTOR
{
  u8  bLength;							//端点描述表长度--0x07/7个字节
  u8  bDescriptorType;			//端点描述类型 0x04
  u8  bEndpointAddress;			//端点：此描述表所描述的端点的地址、方向：Bit 3..0 : 端点号，Bit 6..4 : 保留,为零，Bit 7: 方向,如果控制端点则略。0：输出端点（主机到设备），1：输入端点（设备到主机）
  u8  bmAttributes;					//传输类型：Bit 1..0 :传送类型；00=控制传送，01=同步传送，10=批传送，11=中断传送
  u16 wMaxPacketSize;				//端点能够接收或发送的最大数据包的大小。对于实进传输，此值用于为每帧的数据净负荷预留时间。在实际运行时，管道可能不完全需要预留的带宽，实际带宽可由设备通过一种非USB定义的机制汇报给主机。对于中断传输，批量传输和控制传输，端点可能发送比之短的数据包
  u8  bInterval;						//周期数据传输端点的时间间隙。此域的值对于批传送的端点及控制传送的端点无意义。对于同步传送的端点此域必需为1，表示周期为1ms。对于中断传送的端点此域值的范围为1ms到255ms。
} USB_ENDPOINT_DESCRIPTOR;

//************字符串描述符结构体*************//
/* USB String Descriptor */
typedef __packed struct _USB_STRING_DESCRIPTOR
{
  u8  bLength;						//此描述表的字节数
  u8  bDescriptorType;		//字串描述表类型（此处应为0x03）
  u16 bString/*[]*/;			//字符串数组
} USB_STRING_DESCRIPTOR;

//************公共描述符结构体*************//
/* USB Common Descriptor */
typedef __packed struct _USB_COMMON_DESCRIPTOR
{
  u8  bLength;
  u8  bDescriptorType;
} USB_COMMON_DESCRIPTOR;
//************头功能描述符*************//
/* Header Functional Descriptor */
typedef __packed struct _USB_Header_Functional_Descriptor
{
	u8 	bLength;   							//描述符长度--0x05/5个字节
	u8	bDescriptorType;   			//描述符类型为类特殊接口（CS_INTERFACE）编号为0x24。
	u8	bDescriptorSubtype;   	//描述符子类为Header Functional Descriptor  
	u16	bcdCDC;	   							//bcdCDC字段。CDC版本号，为0x0110（低字节在先）
} USB_Header_Functional_Descriptor;
//************调用功能描述符*************//
/* Call Managment Functional Descriptor */
typedef __packed struct _USB_Call_Managment_Functional_Descriptor
{
	u8 	bLength;   					//描述符长度--0x05/5个字节
	u8	bDescriptorType;   	//描述符类型为类特殊接口（CS_INTERFACE）编号为0x24。
	u8	bDescriptorSubtype;	//描述符子类为Call Management 编号为0x01。
	u8	bmCapabilities;  		//设备自己不管理call management  
	u8	bDataInterface;   	//数据类接口数量用作call management
} USB_Call_Managment_Functional_Descriptor;
//************抽象控制管理功能描述符*************//
/* ACM（Abstract Control Management ） Functional Descriptor */
typedef __packed struct _USB_ACM_Functional_Descriptor
{
	u8 	bLength;  					//描述符长度--0x04/4个字节
	u8 	bDescriptorType;  	//描述符类型为类特殊接口（CS_INTERFACE）编号为0x24。
	u8 	bDescriptorSubtype;	//描述符子类为Abstract Control Management Functional Descriptor 编号为0x02。
	u8 	bmCapabilities;   	//支持Set_Line_Coding、Set_Control_Line_State、Get_Line_Coding请求和Serial_State通知
} USB_ACM_Functional_Descriptor;
//************联合功能描述符*************//
/* Union Functional Descriptor */
typedef __packed struct _USB_Union_Functional_Descriptor
{
	u8 	bLength;   					//描述符长度--0x05/5个字节
	u8 	bDescriptorType;   	//描述符类型为类特殊接口（CS_INTERFACE）编号为0x24。
	u8 	bDescriptorSubtype;	//描述符子类为Union functional descriptor，编号为0x06。
	u8 	bMasterInterface;  	//MasterInterface字段。这里为前面编号为0的CDC接口。
	u8 	bSlaveInterface;   	//SlaveInterface字段，这里为接下来编号为1的数据类接口。
} Union_Functional_Descriptor;
//************组合设备描述符结构体*************//
/* USB Interface Association Descriptor */	//USB复合设备一般用Interface Association Descriptor（IAD）实现，就是在要合并的接口前加上IAD描述符。
typedef __packed struct _USB_INTERFACE_ASSOCIATION_DESCRIPTOR
{
  u8  bLength;							/* bLength */						//描述符大小--0x08/8个字节
  u8  bDescriptorType;			/* bDescriptorType */		//IAD描述符类型
  u8  bFirstInterface;			/* bFirstInterface */		//起始接口	
  u8  bInterfaceCount;			/* bInterfaceCount */		//接口数
  u8  bFunctionClass;				/* bFunctionClass */		//类型代码
  u8  bFunctionSubclass;		/* bFunctionSubclass */	//子类型代码
  u8  bFunctionProtocol;		/* bFunctionProtocol */	//协议代码
  u8  iFunction;						/* iFunction */					//描述字符串索引
} USB_INTERFACE_ASSOCIATION_DESCRIPTOR;

#endif /* __USB_DESC_H */
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/







//说明文档
/* USB Standard Device Descriptor */
/*=================================================================================*/
/*=================================================================================*/
//在USB中，USB HOST是通过各种描述符来识别设备的，有设备描述符，配置描述符，接口描述符，端点描述符，字符串描述符，报告描述符等等。
//USB报告描述符(Report Descriptor)是HID设备中的一个描述符，它是比较复杂的一个描述符。
/********************************1,USB标准设备描述结构：*******************************
*************************************************************************************
设备描述符给出了USB设备的一般信息，包括对设备及在设备配置中起全程作用的信息，
包括制造商标识号ID、产品序列号、所属设备类号、默认端点的最大包长度和配置描述
符的个数等。一个USB设备必须有且仅有一个设备描述符。设备描述符是设备连接到总
线上时USB主机所读取的第一个描述符，它包含了14个字段，结构如下：
*-----------------------------------------------------------------------------------
*	|序号	|			域						|大小	|	值	|	描述
*-----------------------------------------------------------------------------------
*	|	0		|bLength						|	1		|数字	|	描述设备描述符的总字节数		
*-----------------------------------------------------------------------------------
*	|	1		|bDecriptorType			|	1		|常量	|	描述符的类型（为0X01，这里是设备描述符）	
*-----------------------------------------------------------------------------------
*	|	2		|bcdUSB							|	2		|BCD码|	这个设备兼容的USB设备版本号	
*-----------------------------------------------------------------------------------
*	|	4		|bDeviceClass				|	1		|类		|	设备类码：是由USB 协会规定的，描述的是接口所能实现的功能。当此域为0时下面的子类也必须为0，当为0XFF表示的是厂商自定义设备类	
*-----------------------------------------------------------------------------------
*	|	5		|bDeviceSubClass		|	1		|子类	|	子类代码码：这个码值的意思是根据设备类码来看。如设备类码为零，这字段也要零，如设备类码为0XFF，此域的所有值保留。	
*-----------------------------------------------------------------------------------
*	|	6		|bDevicePortocol		|	1		|协议	|	协议码：这些码的值视设备码和子类代码的值而定。当该字段为0是，表示设备不使用类所定义的协议， 当该字段的值为0XFF时，表示使用设备厂商自定义的协议	
*-----------------------------------------------------------------------------------
*	|	7		|bMaxPacketSize0		|	1		|数字	|	端点0的能缓冲的最大数据包大小	
*-----------------------------------------------------------------------------------
*	|	8		|idVendor						|	2		|ID		|	生产设备厂家的标志（由USB 相关组织给的）	
*-----------------------------------------------------------------------------------
*	|	10	|idProduct					|	2		|ID		|	产品标志（由生产的厂家自己做编号）	
*-----------------------------------------------------------------------------------
*	|	12	|bcdDevice					|	2		|BCD码|	设备的版本号	
*-----------------------------------------------------------------------------------
*	|	14	|iManufacturer			|	1		|索引	|	描述生产设备厂家的信息的字符串描述符的索引值。
*-----------------------------------------------------------------------------------
*	|	15	|iProduct						|	1		|索引	|	描述所使用设备产品的信息的字串描述符的索引值。	
*-----------------------------------------------------------------------------------
*	|	16	|iSerialNumber			|	1		|索引	|	描述设备产品的序列号信息的字串描述符的索引值。	
*-----------------------------------------------------------------------------------
*	|	17	|bNumConfigurations	|	1		|数字	|	设备有多少种配置	
*-----------------------------------------------------------------------------------

*	其中bDescriptorType为描述符的类型，其含义可查下表
*	(此表也适用于标准命令Get_Descriptor中wValue域高字节的取值含义)：
*-----------------------------------------------------------------------------------
*	|	类型			|	值	|	描述符	
*-----------------------------------------------------------------------------------
*	|						|0x01	|	设备描述符（Device Descriptor)
*	|						|---------------------------------------------------------------------
*	|						|0x02	|	配置描述符（Configuration Descriptor）
*	|						|---------------------------------------------------------------------
*	|标准描述符	|0x03	|	字符串描述符（String Descriptor）
*	|						|---------------------------------------------------------------------
*	|						|0x04	|	接口描述符（Interface Descriptor）
*	|						|---------------------------------------------------------------------
*	|						|0x05	|	端点描述符（EndPont Descriptor）
*-----------------------------------------------------------------------------------
*	|						|0x29	|	集线器类描述符（Hub Descriptor）
*	|类描述符		|---------------------------------------------------------------------
*	|						|0x21	|	人机接口类描述符（HID）
*-----------------------------------------------------------------------------------
*	|厂商定义		|0xFF	|
*-----------------------------------------------------------------------------------


*	设备类代码bDeviceClass可查下表：
*-----------------------------------------------------------------------------------
*	|	值(十进制)	|	值	|	说明	
*-----------------------------------------------------------------------------------
*	|			0			|0x00	|	接口描述符中提供类的值
*-----------------------------------------------------------------------------------
*	|			2			|0x02	|	通信类
*-----------------------------------------------------------------------------------
*	|			9			|0x09	|	集线器类
*-----------------------------------------------------------------------------------
*	|			220		|0xDC	|	用于诊断用途的设备类
*-----------------------------------------------------------------------------------
*	|			224		|0xE0	|	无线通信设备类
*-----------------------------------------------------------------------------------
*	|			255		|0xFF	|	厂商定义的设备类
*-----------------------------------------------------------------------------------
************************************************************************************/

/*=================================================================================*/
/*=================================================================================*/
/********************************2,USB标准配置描述结构：*******************************
*************************************************************************************
配置描述符中包括了描述符的长度（属于此描述符的所有接口描述符和端点描述符的长度的和）、
供电方式（自供电/总线供电）、最大耗电量等。主果主机发出USB标准命令Get_Descriptor要
求得到设备的某个配置描述符，那么除了此配置描述符以外，此配置包含的所有接口描述符与端
点描述符都将提供给USB主机。
*-----------------------------------------------------------------------------------
*	|序号	|			域							|大小	|	值	|	描述
*-----------------------------------------------------------------------------------
*	|	0		|bLength							|	1		|数字	|	此描述表的字节数长度。		
*-----------------------------------------------------------------------------------
*	|	1		|bDecriptorType				|	1		|常量	|	配置描述表类型（此处为0x02）	
*-----------------------------------------------------------------------------------
*	|	2		|wTotalLength					|	2		|数字	|	此配置信息的总长（包括配置，接口，端点和设备类及厂商定义的描述符）	
*-----------------------------------------------------------------------------------
*	|	4		|bNumInterfaces				|	1		|数字	|	此配置所支持的接口个数
*-----------------------------------------------------------------------------------
*	|	5		|bCongfigurationValue	|	1		|数字	|	描述此配置的字串描述表索引
*-----------------------------------------------------------------------------------
*	|	6		|iConfiguration				|	1		|索引	|	协议码：这些码的值视设备码和子类代码的值而定。当该字段为0是，表示设备不使用类所定义的协议， 当该字段的值为0XFF时，表示使用设备厂商自定义的协议	
*-----------------------------------------------------------------------------------
*	|	7		|bmAttributes					|	1		|位图	|	配置特性：
*	|			|											|			|			|	D7： 保留（设为一）
*	|			|											|			|			|	D6： 自给电源
*	|			|											|			|			|	D5： 远程唤醒
*	|			|											|			|			|	D4..D0：保留（设为一）
*	|			|											|			|			|	一个既用总线电源又有自给电源的设备会在MaxPower域
*	|			|											|			|			|	指出需要从总线取的电量。并设置D6为一。运行时期的
*	|			|											|			|			|	实际电源模式可由GetStatus(DEVICE) 请求得到。
*-----------------------------------------------------------------------------------
*	|	8		|MaxPower							|	1		|mA		|	在此配置下的总线电源耗费量。以 2mA 为一个单位。	
*-----------------------------------------------------------------------------------
************************************************************************************/

/*=================================================================================*/
/*=================================================================================*/

/**********************************3,接口描述符：*************************************
*************************************************************************************
配置描述符中包含了一个或多个接口描述符，这里的“接口”并不是指物理存在的接口，在这里把它称之
为“功能”更易理解些，例如一个设备既有录音的功能又有扬声器的功能，则这个设备至少就有两个“接口”。
如果一个配置描述符不止支持一个接口描述符，并且每个接口描述符都有一个或多个端点描述符，那么在
响应USB主机的配置描述符命令时，USB设备的端点描述符总是紧跟着相关的接口描述符后面，作为配置
描述符的一部分被返回。接口描述符不可直接用Set_Descriptor和Get_Descriptor来存取。
如果一个接口仅使用端点0，则接口描述符以后就不再返回端点描述符，并且此接口表现的是一个控制
接口的特性，它使用与端点0相关联的默认管道进行数据传输。在这种情况下bNumberEndpoints域应被
设置成0。接口描述符在说明端点个数并不把端点0计算在内。
*-----------------------------------------------------------------------------------
*	|序号	|			域						|大小	|	值	|	描述
*-----------------------------------------------------------------------------------
*	|	0		|bLength						|	1		|数字	|	此表的字节数		
*-----------------------------------------------------------------------------------
*	|	1		|bDecriptorType			|	1		|常量	|	接口描述表类（此处应为0x04）	
*-----------------------------------------------------------------------------------
*	|	2		|bInterfaceNumber		|	1		|数字	|	接口号，当前配置支持的接口数组索引（从零开始）。
*-----------------------------------------------------------------------------------
*	|	3		|bAlternateSetting	|	1		|数字	|	可选设置的索引值。
*-----------------------------------------------------------------------------------
*	|	4		|bNumEndpoints			|	1		|数字	|	此接口用的端点数量，如果是零则说明此接口只用缺省控制管道。	
*-----------------------------------------------------------------------------------
*	|	5		|bInterfaceClass		|	1		|类		|	接口所属的类值： 零值为将来的标准保留。 如果此域的值设为FFH，则此接口类由厂商说明。 所有其它的值由USB说明保留。
*-----------------------------------------------------------------------------------
*	|	6		|bInterfaceSubClass	|	1		|子类	|	子类码 这些值的定义视bInterfaceClass域而定。 如果bInterfaceClass域的值为零则此域的值必须为零。 
*	|			|										|			|			|	bInterfaceClass域不为FFH则所有值由USB 所保留。
*-----------------------------------------------------------------------------------
*	|	7		|bInterfaceProtocol	|	1		|协议	|	协议码：bInterfaceClass 和bInterfaceSubClass 域的值而定.
*	|			|										|			|			|	如果一个接口支持设备类相关的请求此域的值指出了设备类说明中所定义的协议.	
*-----------------------------------------------------------------------------------
*	|	8		|iInterface					|	1		|索引	|	描述此接口的字串描述表的索引值。	
*-----------------------------------------------------------------------------------


*	接口类代码bInterfaceClass可查下表：
*	对于bInterfaceClass字段，表示接口所属的类别，USB协议根据功能将不同的接口划分成不的类，
*	其具体含义如下表所示：
*-----------------------------------------------------------------------------------
*	|	值（十六进制）	|	类别	
*-----------------------------------------------------------------------------------
*	|	0x01					|	音频类
*-----------------------------------------------------------------------------------
*	|	0x02					|	CDC控制类
*-----------------------------------------------------------------------------------
*	|	0x03					|	人机接口类（HID）
*-----------------------------------------------------------------------------------
*	|	0x05					|	物理类
*-----------------------------------------------------------------------------------
*	|	0x06					|	图像类
*-----------------------------------------------------------------------------------
*	|	0x07					|	打印机类
*-----------------------------------------------------------------------------------
*	|	0x08					|	大数据存储类
*-----------------------------------------------------------------------------------
*	|	0x09					|	集线器类
*-----------------------------------------------------------------------------------
*	|	0x0A					|	CDC数据类
*-----------------------------------------------------------------------------------
*	|	0x0B					|	智能卡类
*-----------------------------------------------------------------------------------
*	|	0x0D					|	安全类
*-----------------------------------------------------------------------------------
*	|	0xDC					|	诊断设备类
*-----------------------------------------------------------------------------------
*	|	0xE0					|	无线控制器类
*-----------------------------------------------------------------------------------
*	|	0xFE					|	特定应用类（包括红外的桥接器等）
*-----------------------------------------------------------------------------------
*	|	0xFF					|	厂商定义的设备
*-----------------------------------------------------------------------------------


************************************************************************************/
/*=================================================================================*/
/*=================================================================================*/

/**********************************4,端点描述符：*************************************
*************************************************************************************
端点是设备与主机之间进行数据传输的逻辑接口，除配置使用的端点0（控制端点，一般一个设备只有一
个控制端点）为双向端口外，其它均为单向。端点描述符描述了数据的传输类型、传输方向、数据包大小
和端点号（也可称为端点地址）等。
除了描述符中描述的端点外，每个设备必须要有一个默认的控制型端点，地址为0，它的数据传输为双向，
而且没有专门的描述符，只是在设备描述符中定义了它的最大包长度。主机通过此端点向设备发送命令，
获得设备的各种描述符的信息，并通过它来配置设备。

*-----------------------------------------------------------------------------------
*	|序号	|			域						|大小	|	值	|	描述
*-----------------------------------------------------------------------------------
*	|	0		|bLength						|	1		|数字	|	此表的字节数		
*-----------------------------------------------------------------------------------
*	|	1		|bDecriptorType			|	1		|常量	|	端点描述表类（此处应为0x05）	
*-----------------------------------------------------------------------------------
*	|	2		|bEndpointAddress		|	1		|端点	|	此描述表所描述的端点的地址、方向：
*	|			|										|			|			|	Bit 3..0 : 端点号.
*	|			|										|			|			|	Bit 6..4 : 保留,为零
*	|			|										|			|			|	Bit 7: 方向,如果控制端点则略。
*	|			|										|			|			|	0：输出端点（主机到设备）
*	|			|										|			|			|	1：输入端点（设备到主机）
*-----------------------------------------------------------------------------------
*	|	3		|bmAttributes				|	1		|位图	|	此域的值描述的是在bConfigurationValue域所指的配置下端点的特性。
*	|			|										|			|			|	Bit 1..0 :传送类型
*	|			|										|			|			|		00=控制传送
*	|			|										|			|			|		01=同步传送
*	|			|										|			|			|		10=批传送
*	|			|										|			|			|		11=中断传送
*	|			|										|			|			|	所有其它的位都保留。	
*-----------------------------------------------------------------------------------
*	|	4		|wMaxPacketSize			|	2		|数字	|	当前配置下此端点能够接收或发送的最大数据包的大小。 
*	|			|										|			|			|	对于实进传输，此值用于为每帧的数据净负荷预留时间。
*	|			|										|			|			|	在实际运行时，管道可能不完全需要预留的带宽，实际
*	|			|										|			|			|	带宽可由设备通过一种非USB定义的机制汇报给主机。
*	|			|										|			|			|	对于中断传输，批量传输和控制传输，端点可能发送比之短的数据包	
*-----------------------------------------------------------------------------------
*	|	6		|bInterval					|	1		|数字	|	周期数据传输端点的时间间隙。 此域的值对于批传送的
*	|			|										|			|			|	端点及控制传送的端点无意义。对于同步传送的端点此
*	|			|										|			|			|	域必需为1，表示周期为1ms。对于中断传送的端点此域
*	|			|										|			|			|	值的范围为1ms到255ms。
*-----------------------------------------------------------------------------------

************************************************************************************/

/**********************************5,字符串描述符：***********************************
*************************************************************************************
字符串描述符是一种可选的USB标准描述符，描述了如制商、设备名称或序列号等信息。如果一个设备无
字符串描述符，则其它描述符中与字符串有关的索引值都必须为0。字符串使用的是Unicode编码。

主机请示得到某个字符串描述符时一般分成两步：首先主机向设备发出USB标准命令Get_Descriptor，
其中所使用的字符串的索引值为0，设备返回一个字符串描述符，此描述符的结构如下：
*-----------------------------------------------------------------------------------
*	|序号	|			域						|大小	|	值	|	描述
*-----------------------------------------------------------------------------------
*	|	0		|bLength						|	1		|数字	|	此表的字节数		
*-----------------------------------------------------------------------------------
*	|	1		|bDecriptorType			|	1		|常量	|	端点描述表类（此处应为0x05）	
*-----------------------------------------------------------------------------------
*	|	2		|wLANGID[0]					|	2		|数字	|	语言标识（LANGID） 码0
*-----------------------------------------------------------------------------------
*	|	…	|…									|	…	|…		|	…
*-----------------------------------------------------------------------------------
*	|	N		|wLANGID[x]					|	2		|数字	|	语言标识（LANGID） 码X
*-----------------------------------------------------------------------------------
*	该字符串描述符双字节的语言ID的数组，wLANGID[0]~wLANGID[x]指明了设备支持的语言，具体含义可查看USB_LANGIDs.pdf。
************************************************************************************/

