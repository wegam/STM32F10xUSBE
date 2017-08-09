/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : hw_config.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Hardware Configuration & Setup
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
* 功能：用于 USB硬件配置
* 
* 
* 
* 
* 
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "usb_lib.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "hw_config.h"
#include "platform_config.h"
#include "usb_pwr.h"

#include	"string.h"			//memcpy

#include "STM32_USART.H"
#include "stm32f10x_nvic.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;
ErrorStatus HSEStartUpStatus;

/* Extern variables ----------------------------------------------------------*/
u8 buffer_in[VIRTUAL_COM_PORT_DATA_SIZE];

u8 buffer_rx[VIRTUAL_COM_PORT_DATA_SIZE];
u8 buffer_tx[VIRTUAL_COM_PORT_DATA_SIZE];

u32 USART_Rx_ptr_in = 0;
u32 USART_Rx_ptr_out = 0;
u32 USART_Rx_length  = 0;

u8 Usart_tx_flg=0;

extern u32 count_in;
extern LINE_CODING linecoding;

//extern u8 buffer_out[VIRTUAL_COM_PORT_DATA_SIZE];
extern u32 count_out;

u8  USB_Tx_State = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_System(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);

    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* ADCCLK = PCLK2/6 */
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while (RCC_GetSYSCLKSource() != 0x08)
    {}
  }

  /* Enable GPIOA, GPIOD and USART1 clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO
                         | RCC_APB2Periph_USART1, ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		//关闭JTAG接口，使用PA15为GPIO

  /* Enable USB_DISCONNECT GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_DISCONNECT, ENABLE);

  /* Configure USB pull-up pin */
  GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);

  /* Configure USART1 Rx (PA.10) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USART1 Tx (PA.09) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz)
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_USBClock(void)
{
  /* USBCLK = PLLCLK / 1.5 */
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
  /* Enable USB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

/*******************************************************************************
* Function Name  : Enter_LowPowerMode
* Description    : Power-off system clocks and power while entering suspend mode
* 描述			    	: 挂起模式
* Input          : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{
  /* Set the device state to suspend */
  bDeviceState = SUSPENDED;
}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode
* Description    : Restores system clocks and power while exiting suspend mode
* 描述			    	: 退出挂起模式
* Input          : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  /* Set the device state to the correct state */
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else
  {
    bDeviceState = ATTACHED;
  }
}

/*******************************************************************************
* Function Name  : USB_Interrupts_Config
* Description    : Configures the USB interrupts
* Input          : None.
* Return         : None.
*******************************************************************************/
void USB_Interrupts_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

#ifdef  VECT_TAB_RAM
  /* Set the Vector Table base location at 0x20000000 */
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN_RX0_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

//  /* Enable USART1 Interrupt */
//  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : USB_Cable_Config
* Description    : Software Connection/Disconnection of USB Cable
* Input          : None.
* Return         : Status
*******************************************************************************/
void USB_Cable_Config (FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
		GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
  else
  {
    GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
		GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
}

/*******************************************************************************
* Function Name  :  UART0_Config_Default.
* Description    :  configure the UART 0 with default values.	串口的默认配置值
* Input          :  None.
* Return         :  None.
*******************************************************************************/
void USART_Config_Default(void)
{
//--------------原程序
  /* USART1 default configuration */
  /* USART1 configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - Parity Odd
        - Hardware flow control desabled
        - Receive and transmit enabled
  */
//  USART_InitStructure.USART_BaudRate = 9600;
//  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//  USART_InitStructure.USART_StopBits = USART_StopBits_1;
//  USART_InitStructure.USART_Parity = USART_Parity_Odd;
//  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

//  /* Configure the USART1 */
//  USART_Init(USART1, &USART_InitStructure);

//  /* Enable the USART Receive interrupt */
//  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	
//-----------------新--DMA空闲中断	
	USART_DMA_Configuration(USART1,115200,0,0,(u32*)buffer_rx,(u32*)buffer_tx,VIRTUAL_COM_PORT_DATA_SIZE);
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC, DISABLE);
	USART_ITConfig(USART1,USART_IT_IDLE, DISABLE);					//使用空闲中断，DMA自动接收，检测到总线空闲表示发送端已经发送完成，数据保存在DMA缓冲器中
}

/*******************************************************************************
* Function Name  :  UART0_Config.
* Description    :  Configure the UART 1 according to the linecoding structure.
										根据line coding 结构体配置串口.
* Input          :  None.
* Return         :  Configuration status
                    TRUE : configuration done with success
                    FALSE : configuration aborted.
*******************************************************************************/
bool USART_Config(void)
{

  /* set the Stop bit*/
	/**************设置停止位**************/
  switch (linecoding.format)
  {
    case 0:
      USART_InitStructure.USART_StopBits = USART_StopBits_1;		//1位停止位
      break;
    case 1:
      USART_InitStructure.USART_StopBits = USART_StopBits_1_5;	//1.5为停止位
      break;
    case 2:
      USART_InitStructure.USART_StopBits = USART_StopBits_2;		//2位停止位
      break;
    default :
    {
      USART_Config_Default();																		//默认配置
      return (FALSE);
    }
  }

  /* set the parity bit*/
	/**************设置校验位**************/
  switch (linecoding.paritytype)
  {
    case 0:
      USART_InitStructure.USART_Parity = USART_Parity_No;			//没有校验
      break;
    case 1:
      USART_InitStructure.USART_Parity = USART_Parity_Even;		//偶校验
      break;	
    case 2:
      USART_InitStructure.USART_Parity = USART_Parity_Odd;		//奇校验
      break;
    default :
    {
      USART_Config_Default();																	//默认配置
      return (FALSE);
    }
  }

  /*set the data type : only 8bits and 9bits is supported */
	/**************设置数据位: 8位或9位**************/
  switch (linecoding.datatype)
  {
    case 0x07:
      USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//8为数据位，这个选项就校验位必须设置(奇校验/偶校验)
      break;
    case 0x08:
      USART_InitStructure.USART_WordLength = USART_WordLength_9b;	//9位数据位
      break;
    default :
    {
      USART_Config_Default();																//默认配置
      return (FALSE);
    }
  }

  USART_InitStructure.USART_BaudRate = linecoding.bitrate;													//设置波特率
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//设置没有硬件数据流控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;										//使能接收、发送
  USART_Init(USART1, &USART_InitStructure);																					//初始化串口
  USART_Cmd(USART1, ENABLE);																												//使能串口
  return (TRUE);
}
/*******************************************************************************
* Function Name  : USB_To_UART_Send_Data.
* Description    : send the received data from USB to the UART 0.将USB接收到的数据从串口发送数据
* Input          : data_buffer: data address.
                   Nb_bytes: number of bytes to send.
* Return         : none.
*******************************************************************************/
void USB_To_USART_Send_Data(u8* data_buffer, u8 Nb_bytes)
{
	Usart_tx_flg=1;
	USART_DMASend(USART1,(u32*)data_buffer,(u16)Nb_bytes);		//自定义printf串口DMA发送程序
}
/*******************************************************************************
* Function Name  : USB_To_UART_Send_Data.
* Description    : send the received data from USB to the UART 0.将USB接收到的数据从串口发送数据
* Input          : data_buffer: data address.
                   Nb_bytes: number of bytes to send.
* Return         : none.
*******************************************************************************/
void USB_To_USART_Send_DataBAC1(u8* data_buffer, u8 Nb_bytes)
{
	//---原程序
//  u32 i;
//  for (i = 0; i < Nb_bytes; i++)									//串口发送数据
//  {
//    USART_SendData(USART1, *(data_buffer + i));
//		while((USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET));
//  }
	
	//----修改程序
	u32 i;
	if ((count_out != 0) && (bDeviceState == CONFIGURED))
	{
		for (i = 0; i < Nb_bytes; i++)									//串口发送数据
		{
			USART_SendData(USART1, *(data_buffer + i));
			while((USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET));
		}
		count_out = 0;
	}
}
/*******************************************************************************
* Function Name  : UART_To_USB_Send_Data.
* Description    : send the received data from UART 0 to USB.	发送串口接收到的数据到USB
* Input          : None.
* Return         : none.
*******************************************************************************/
void USART_To_USB_Send_Data(void)
{	
	if(USART_GetFlagStatus(USART1,USART_FLAG_IDLE))
	{
		USART_ClearFlag(USART1,USART_FLAG_IDLE);
		DMA_Cmd(DMA1_Channel5,DISABLE);    																//关闭接收DMA
		count_in = USART1->DR; 																								//读出数据以完成清除标志			
		count_in = VIRTUAL_COM_PORT_DATA_SIZE -  DMA_GetCurrDataCounter(DMA1_Channel5);	//得到真正接收数据个数

		memcpy(buffer_in, buffer_rx, count_in);
		memset(buffer_rx, 0x00, VIRTUAL_COM_PORT_DATA_SIZE);

		DMA1_Channel5->CMAR=(u32)buffer_rx;																//重新设置DMA接收地址
		DMA1_Channel5->CNDTR=VIRTUAL_COM_PORT_DATA_SIZE;														//重新设置接收数据个数			
		DMA_Cmd(DMA1_Channel5,ENABLE);  																	//开启接收DMA
		UserToPMABufferCopy(buffer_in, ENDP1_TXADDR, count_in);
		SetEPTxCount(ENDP1, count_in);
		SetEPTxValid(ENDP1);
	}
}
/*******************************************************************************
* Function Name  : UART_To_USB_Send_Data.
* Description    : send the received data from UART 0 to USB.	发送串口接收到的数据到USB
* Input          : None.
* Return         : none.
*******************************************************************************/
void USART_To_USB_Send_Databac1(void)
{
	//------原程序
//  if (linecoding.datatype == 7)
//  {
//    buffer_in[count_in] = USART_ReceiveData(USART1) & 0x7F;
//  }
//  else if (linecoding.datatype == 8)
//  {
//    buffer_in[count_in] = USART_ReceiveData(USART1);
//  }
//  count_in++;
//  UserToPMABufferCopy(buffer_in, ENDP1_TXADDR, count_in);
//  SetEPTxCount(ENDP1, count_in);
//  SetEPTxValid(ENDP1);
//	u32 num=0;
//	num=USART_RX_FlagClearADDR(USART1,(u32*)buffer_rx);				//清除串口接收中断标志
	
//	USART_ClearITPendingBit(USART1,USART_IT_IDLE); 										//清除空闲串口标志位
	DMA_Cmd(DMA1_Channel5,DISABLE);    																//关闭接收DMA
	count_in = USART1->DR; 																								//读出数据以完成清除标志			
	count_in = VIRTUAL_COM_PORT_DATA_SIZE -  DMA_GetCurrDataCounter(DMA1_Channel5);	//得到真正接收数据个数
	
	memcpy(buffer_in, buffer_rx, count_in);
	memset(buffer_rx, 0x00, VIRTUAL_COM_PORT_DATA_SIZE);
	
	DMA1_Channel5->CMAR=(u32)buffer_rx;																//重新设置DMA接收地址
	DMA1_Channel5->CNDTR=VIRTUAL_COM_PORT_DATA_SIZE;														//重新设置接收数据个数			
	DMA_Cmd(DMA1_Channel5,ENABLE);  																	//开启接收DMA	
	
	
//	count_in++;
  UserToPMABufferCopy(buffer_in, ENDP1_TXADDR, count_in);
  SetEPTxCount(ENDP1, count_in);
  SetEPTxValid(ENDP1);
}
/*******************************************************************************
* Function Name  : Handle_USBAsynchXfer.
* Description    : send data to USB.
* Input          : None.
* Return         : none.
*******************************************************************************/
void Handle_USBAsynchXfer (void)
{
	if(DMA_GetFlagStatus(DMA1_FLAG_GL4))
	{
		if(Usart_tx_flg)
		{
			if(USART_GetFlagStatus(USART1,USART_FLAG_TXE))
			{
				USART_ClearFlag(USART1,USART_FLAG_TXE);
				DMA_ClearFlag(DMA1_FLAG_GL4);								//清除标志
				DMA_Cmd(DMA1_Channel4,DISABLE);							//DMA发送关闭
			}
		}
	}
	if(USART_GetFlagStatus(USART1,USART_FLAG_IDLE))
	{
		USART_ClearFlag(USART1,USART_FLAG_IDLE);
		DMA_Cmd(DMA1_Channel5,DISABLE);    																//关闭接收DMA
		count_in = USART1->DR; 																								//读出数据以完成清除标志			
		count_in = VIRTUAL_COM_PORT_DATA_SIZE -  DMA_GetCurrDataCounter(DMA1_Channel5);	//得到真正接收数据个数

		memcpy(buffer_in, buffer_rx, count_in);
		memset(buffer_rx, 0x00, VIRTUAL_COM_PORT_DATA_SIZE);

		DMA1_Channel5->CMAR=(u32)buffer_rx;																//重新设置DMA接收地址
		DMA1_Channel5->CNDTR=VIRTUAL_COM_PORT_DATA_SIZE;														//重新设置接收数据个数			
		DMA_Cmd(DMA1_Channel5,ENABLE);  																	//开启接收DMA
		
		Usart_tx_flg=1;
		UserToPMABufferCopy(buffer_in, ENDP1_TXADDR, count_in);
		SetEPTxCount(ENDP1, count_in);
		SetEPTxValid(ENDP1);
	}
		

}
/*******************************************************************************
* Function Name  : Handle_USBAsynchXfer.
* Description    : send data to USB.
* Input          : None.
* Return         : none.
*******************************************************************************/
void Handle_USBAsynchXferBAC1 (void)
{
  
  u16 USB_Tx_ptr;
  u16 USB_Tx_length;
  
  if(USB_Tx_State != 1)
  {
    if (USART_Rx_ptr_out == VIRTUAL_COM_PORT_DATA_SIZE)
    {
      USART_Rx_ptr_out = 0;
    }
    
    if(USART_Rx_ptr_out == USART_Rx_ptr_in) 
    {
      USB_Tx_State = 0; 
      return;
    }
    
    if(USART_Rx_ptr_out > USART_Rx_ptr_in) /* rollback */
    { 
      USART_Rx_length = VIRTUAL_COM_PORT_DATA_SIZE - USART_Rx_ptr_out;
    }
    else 
    {
      USART_Rx_length = USART_Rx_ptr_in - USART_Rx_ptr_out;
    }
    
    if (USART_Rx_length > VIRTUAL_COM_PORT_DATA_SIZE)
    {
      USB_Tx_ptr = USART_Rx_ptr_out;
      USB_Tx_length = VIRTUAL_COM_PORT_DATA_SIZE;
      
      USART_Rx_ptr_out += VIRTUAL_COM_PORT_DATA_SIZE;	
      USART_Rx_length -= VIRTUAL_COM_PORT_DATA_SIZE;	
    }
    else
    {
      USB_Tx_ptr = USART_Rx_ptr_out;
      USB_Tx_length = USART_Rx_length;
      
      USART_Rx_ptr_out += USART_Rx_length;
      USART_Rx_length = 0;
    }
    USB_Tx_State = 1; 
    UserToPMABufferCopy(&buffer_in[USB_Tx_ptr], ENDP1_TXADDR, USB_Tx_length);
    SetEPTxCount(ENDP1, USB_Tx_length);
    SetEPTxValid(ENDP1); 
  }  
  
}
/*******************************************************************************
* Function Name  : Get_SerialNum.
* Description    : Create the serial number string descriptor.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Get_SerialNum(void)
{
  u32 Device_Serial0, Device_Serial1, Device_Serial2;

  Device_Serial0 = *(vu32*)(0x1FFFF7E8);
  Device_Serial1 = *(vu32*)(0x1FFFF7EC);
  Device_Serial2 = *(vu32*)(0x1FFFF7F0);

  if (Device_Serial0 != 0)
  {
    Virtual_Com_Port_StringSerial[2] = (u8)(Device_Serial0 & 0x000000FF);
    Virtual_Com_Port_StringSerial[4] = (u8)((Device_Serial0 & 0x0000FF00) >> 8);
    Virtual_Com_Port_StringSerial[6] = (u8)((Device_Serial0 & 0x00FF0000) >> 16);
    Virtual_Com_Port_StringSerial[8] = (u8)((Device_Serial0 & 0xFF000000) >> 24);

    Virtual_Com_Port_StringSerial[10] = (u8)(Device_Serial1 & 0x000000FF);
    Virtual_Com_Port_StringSerial[12] = (u8)((Device_Serial1 & 0x0000FF00) >> 8);
    Virtual_Com_Port_StringSerial[14] = (u8)((Device_Serial1 & 0x00FF0000) >> 16);
    Virtual_Com_Port_StringSerial[16] = (u8)((Device_Serial1 & 0xFF000000) >> 24);

    Virtual_Com_Port_StringSerial[18] = (u8)(Device_Serial2 & 0x000000FF);
    Virtual_Com_Port_StringSerial[20] = (u8)((Device_Serial2 & 0x0000FF00) >> 8);
    Virtual_Com_Port_StringSerial[22] = (u8)((Device_Serial2 & 0x00FF0000) >> 16);
    Virtual_Com_Port_StringSerial[24] = (u8)((Device_Serial2 & 0xFF000000) >> 24);
  }
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
