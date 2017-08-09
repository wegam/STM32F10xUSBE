/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_init.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Initialization routines & global variables
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
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/*  The number of current endpoint, it will be used to specify an endpoint */
 u8	EPindex;
/*  The number of current device, it is an index to the Device_Table */
/* u8	Device_no; */
/*  Points to the DEVICE_INFO structure of current device */
/*  The purpose of this register is to speed up the execution */
DEVICE_INFO *pInformation;		//设备状态
/*  Points to the DEVICE_PROP structure of current device */
/*  The purpose of this register is to speed up the execution */
DEVICE_PROP *pProperty;
/*  Temporary save the state of Rx & Tx status. */
/*  Whenever the Rx or Tx state is changed, its value is saved */
/*  in this variable first and will be set to the EPRB or EPRA */
/*  at the end of interrupt process */
u16	SaveState ;
u16  wInterrupt_Mask;
DEVICE_INFO	Device_Info;
USER_STANDARD_REQUESTS  *pUser_Standard_Requests;

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : USB_Init
* Description    : USB system initialization
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_Init(void)
{
  pInformation = &Device_Info;			//这个是设备资料部分,里面的全部变量，静态的	//pInformation 是一个指向Device_Info 的指针
  pInformation->ControlState = 2;
  pProperty = &Device_Property;			//pProperty 是一个指针，指向DEVICE_PROP 这个是设备属性部分，在usb_prop.c->Line 60
																		//设备属性部分包含一些方法，即函数。也包括两个参数，一个是接收区的缓冲区地址，一个
																		//是最大的包的长度。都是用字节表示的。
																		
  pUser_Standard_Requests = &User_Standard_Requests;		//标准请求
  /* Initialize devices one by one */
  pProperty->Init();			//初始化--初始化完成后，后面的事件处理由中断处理函数进行,usb_istr.c->USB_Istr->CTR_LP()		
													//usb_prop.c->Device_Property->Virtual_Com_Port_init;	Virtual_Com_Port_init
													//这个属性是一个通用的属性，它指以相当于实例化套到USB 这个头上去。故我们在这里看
													//上去运行一个pProperty->Init();实际上运行的是DEVICE_PROP 中的
													//Virtual_Com_Port_init（）这个函数。
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
