/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : stm32f10x_can.h
* Author             : MCD Application Team
* Version            : V2.0.1
* Date               : 06/13/2008
* Description        : This file contains all the functions prototypes for the
*                      CAN firmware library.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/*—————————————————《控制器局域网（CAN）》——————————————————/
*	本外设作为CAN网络的界面，支持CAN 协议2.0A和2.0B。它的设计目标是，以最小的CPU 负荷来高效处理
*	大量收到的报文。它也支持报文发送的优先级要求（优先级特性可软件配置）。
——————————————————————————————————————————*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_CAN_H
#define __STM32F10x_CAN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_map.h"

/* Exported types ------------------------------------------------------------*/
/* CAN init structure definition */
typedef struct
{
  FunctionalState CAN_TTCM;						//CAN_TTCM用来使能或者失能时间触发通讯模式，可以设置这个参数的值为ENABLE或者DISABLE。
  FunctionalState CAN_ABOM;						//CAN_ABOM用来使能或者失能自动离线管理，可以设置这个参数的值为ENABLE或者DISABLE。
  FunctionalState CAN_AWUM;						//CAN_AWUM用来使能或者失能自动唤醒模式，可以设置这个参数的值为ENABLE或者DISABLE。
  FunctionalState CAN_NART;						//CAN_NARM用来使能或者失能非自动重传输模式，可以设置这个参数的值为ENABLE或者DISABLE。
  FunctionalState CAN_RFLM;						//CAN_RFLM用来使能或者失能接收FIFO锁定模式，可以设置这个参数的值为ENABLE或者DISABLE。
  FunctionalState CAN_TXFP;						//CAN_TXFP用来使能或者失能发送FIFO优先级，可以设置这个参数的值为ENABLE或者DISABLE。
  u8 CAN_Mode;												//CAN_Mode设置了CAN的工作模式，<CAN operating mode>.给出了该参数可取的值
  u8 CAN_SJW;													//CAN_SJW定义了重新同步跳跃宽度(SJW)，即在每位中可以延长或缩短多少个时间单位的上限，<CAN synchronisation jump width>给出了该参数可取的值
  u8 CAN_BS1;													//CAN_BS1设定了时间段1的时间单位数目，<time quantum in bit segment 1>给出了该参数可取的值
  u8 CAN_BS2;													//CAN_BS2设定了时间段1的时间单位数目，<time quantum in bit segment 2>给出了该参数可取的值
  u16 CAN_Prescaler;									//CAN_Prescaler设定了一个时间单位的长度，它的范围是1到1024。
} CAN_InitTypeDef;											

/* CAN filter init structure definition */
typedef struct
{
  u8 CAN_FilterNumber;									//CAN_FilterNumber指定了待初始化的过滤器，它的范围是0到13。
  u8 CAN_FilterMode;										//CAN_FilterMode指定了过滤器将被初始化到的模式。<CAN_FilterMode>给出了该参数可取的值
  u8 CAN_FilterScale;										//CAN_FilterScale给出了过滤器位宽，<CAN_FilterScale>给出了该参数可取的值
  u16 CAN_FilterIdHigh;									//CAN_FilterIdHigh用来设定过滤器标识符（32位位宽时为其高段位，16位位宽时为第一个）。它的范围是0x0000到0xFFFF。
  u16 CAN_FilterIdLow;									//CAN_FilterIdHigh用来设定过滤器标识符（32位位宽时为其低段位，16位位宽时为第二个）。它的范围是0x0000到0xFFFF。
  u16 CAN_FilterMaskIdHigh;							//CAN_FilterMaskIdHigh用来设定过滤器屏蔽标识符或者过滤器标识符（32位位宽时为其高段位，16位位宽时为第一个）。它的范围是0x0000到0xFFFF。
  u16 CAN_FilterMaskIdLow;							//CAN_FilterMaskIdLow用来设定过滤器屏蔽标识符或者过滤器标识符（32位位宽时为其低段位，16位位宽时为第二个）。它的范围是0x0000到0xFFFF。
  u16 CAN_FilterFIFOAssignment;					//CAN_FilterFIFO设定了指向过滤器的FIFO（0或1），<CAN_FilterFIFO>给出了该参数可取的值
  FunctionalState CAN_FilterActivation;	//CAN_FilterActivation使能或者失能过滤器。该参数可取的值为ENABLE或者DISABLE。
} CAN_FilterInitTypeDef;

/* CAN Tx message structure definition */
typedef struct
{
  u32 StdId;														//StdId用来设定标准标识符。它的取值范围为0到0x7FF
  u32 ExtId;														//ExtId用来设定扩展标识符。它的取值范围为0到0x3FFFF
  u8 IDE;																//IDE用来设定消息标识符的类型，<CAN identifier type>给出了该参数可取的值
  u8 RTR;																//RTR用来设定待传输消息的帧类型。它可以设置为数据帧或者远程帧<CAN remote transmission request>。
  u8 DLC;																//DLC用来设定待传输消息的帧长度。它的取值范围是0到0x8。
  u8 Data[8];														//Data[8]包含了待传输数据，它的取值范围为0到0xFF。
} CanTxMsg;

/* CAN Rx message structure definition */
typedef struct
{
  u32 StdId;														//StdId用来设定标准标识符。它的取值范围为0到0x7FF
  u32 ExtId;														//ExtId用来设定扩展标识符。它的取值范围为0到0x3FFFF
  u8 IDE;																//IDE用来设定消息标识符的类型，<CAN identifier type>给出了该参数可取的值
  u8 RTR;																//RTR用来设定待传输消息的帧类型。它可以设置为数据帧或者远程帧<CAN remote transmission request>。
  u8 DLC;																//DLC用来设定待传输消息的帧长度。它的取值范围是0到0x8。
  u8 Data[8];														//Data[8]包含了待传输数据，它的取值范围为0到0xFF。
  u8 FMI;																//FMI设定为消息将要通过的过滤器索引，这些消息存储于邮箱中。该参数取值范围0到0xFF。
} CanRxMsg;

/* Exported constants --------------------------------------------------------*/

/* CAN sleep constants */
#define CANINITFAILED              ((u8)0x00) /* CAN initialization failed */
#define CANINITOK                  ((u8)0x01) /* CAN initialization failed */

/* CAN operating mode */
#define CAN_Mode_Normal             ((u8)0x00)  /* normal mode */														//CAN硬件工作在正常模式
#define CAN_Mode_LoopBack           ((u8)0x01)  /* loopback mode */													//CAN硬件工作在环回模式
#define CAN_Mode_Silent             ((u8)0x02)  /* silent mode */														//CAN硬件工作在静默模式
#define CAN_Mode_Silent_LoopBack    ((u8)0x03)  /* loopback combined with silent mode */		//CAN硬件工作在静默环回模式

#define IS_CAN_MODE(MODE) (((MODE) == CAN_Mode_Normal) || ((MODE) == CAN_Mode_LoopBack)|| \
                           ((MODE) == CAN_Mode_Silent) || ((MODE) == CAN_Mode_Silent_LoopBack))

/* CAN synchronisation jump width */	//重新同步跳跃宽度(SJW)：定义了在每位中可以延长或缩短多少个时间单元的上限。其值可以编程为1到4个时间单元。
#define CAN_SJW_1tq                 ((u8)0x00)  /* 1 time quantum */		//重新同步跳跃宽度1个时间单位
#define CAN_SJW_2tq                 ((u8)0x01)  /* 2 time quantum */		//重新同步跳跃宽度2个时间单位
#define CAN_SJW_3tq                 ((u8)0x02)  /* 3 time quantum */		//重新同步跳跃宽度3个时间单位
#define CAN_SJW_4tq                 ((u8)0x03)  /* 4 time quantum */		//重新同步跳跃宽度4个时间单位

#define IS_CAN_SJW(SJW) (((SJW) == CAN_SJW_1tq) || ((SJW) == CAN_SJW_2tq)|| \
                         ((SJW) == CAN_SJW_3tq) || ((SJW) == CAN_SJW_4tq))

/* time quantum in bit segment 1 */	//时间段1(BS1)：定义采样点的位置。它包含CAN标准里的PROP_SEG和PHASE_SEG1。
																		//其值可以编程为1到16个时间单元，但也可以被自动延长，以补偿因为网络中不同节点的频率差异所造成的相位的正向漂移。
#define CAN_BS1_1tq                 ((u8)0x00)  /* 1 time quantum */		//时间段1为1个时间单位
#define CAN_BS1_2tq                 ((u8)0x01)  /* 2 time quantum */
#define CAN_BS1_3tq                 ((u8)0x02)  /* 3 time quantum */
#define CAN_BS1_4tq                 ((u8)0x03)  /* 4 time quantum */
#define CAN_BS1_5tq                 ((u8)0x04)  /* 5 time quantum */
#define CAN_BS1_6tq                 ((u8)0x05)  /* 6 time quantum */
#define CAN_BS1_7tq                 ((u8)0x06)  /* 7 time quantum */
#define CAN_BS1_8tq                 ((u8)0x07)  /* 8 time quantum */
#define CAN_BS1_9tq                 ((u8)0x08)  /* 9 time quantum */
#define CAN_BS1_10tq                ((u8)0x09)  /* 10 time quantum */
#define CAN_BS1_11tq                ((u8)0x0A)  /* 11 time quantum */
#define CAN_BS1_12tq                ((u8)0x0B)  /* 12 time quantum */
#define CAN_BS1_13tq                ((u8)0x0C)  /* 13 time quantum */
#define CAN_BS1_14tq                ((u8)0x0D)  /* 14 time quantum */
#define CAN_BS1_15tq                ((u8)0x0E)  /* 15 time quantum */
#define CAN_BS1_16tq                ((u8)0x0F)  /* 16 time quantum */		//时间段1为16个时间单位

#define IS_CAN_BS1(BS1) ((BS1) <= CAN_BS1_16tq)

/* time quantum in bit segment 2 */	//时间段2(BS2)：定义发送点的位置。它代表CAN标准里的PHASE_SEG2。
																		//其值可以编程为1到8个时间单元，但也可以被自动缩短以补偿相位的负向漂移。
#define CAN_BS2_1tq                 ((u8)0x00)  /* 1 time quantum */		//时间段2为1个时间单位
#define CAN_BS2_2tq                 ((u8)0x01)  /* 2 time quantum */
#define CAN_BS2_3tq                 ((u8)0x02)  /* 3 time quantum */
#define CAN_BS2_4tq                 ((u8)0x03)  /* 4 time quantum */
#define CAN_BS2_5tq                 ((u8)0x04)  /* 5 time quantum */
#define CAN_BS2_6tq                 ((u8)0x05)  /* 6 time quantum */
#define CAN_BS2_7tq                 ((u8)0x06)  /* 7 time quantum */
#define CAN_BS2_8tq                 ((u8)0x07)  /* 8 time quantum */		//时间段2为8个时间单位

#define IS_CAN_BS2(BS2) ((BS2) <= CAN_BS2_8tq)

/* CAN clock prescaler */
#define IS_CAN_PRESCALER(PRESCALER) (((PRESCALER) >= 1) && ((PRESCALER) <= 1024))

/* CAN filter number */
#define IS_CAN_FILTER_NUMBER(NUMBER) ((NUMBER) <= 13)

/* CAN filter mode */
#define CAN_FilterMode_IdMask       ((u8)0x00)  /* id/mask mode */					//标识符屏蔽位模式
#define CAN_FilterMode_IdList       ((u8)0x01)  /* identifier list mode */	//标识符列表模式---为了过滤出一个标识符，应该设置过滤器组工作在标识符列表模式。

#define IS_CAN_FILTER_MODE(MODE) (((MODE) == CAN_FilterMode_IdMask) || \
                                  ((MODE) == CAN_FilterMode_IdList))

/* CAN filter scale */
#define CAN_FilterScale_16bit       ((u8)0x00) /* 16-bit filter scale */		//2个16位过滤器
#define CAN_FilterScale_32bit       ((u8)0x01) /* 2-bit filter scale */			//1个32位过滤器

#define IS_CAN_FILTER_SCALE(SCALE) (((SCALE) == CAN_FilterScale_16bit) || \
                                    ((SCALE) == CAN_FilterScale_32bit))

/* CAN filter FIFO assignation */
#define CAN_FilterFIFO0             ((u8)0x00)  /* Filter FIFO 0 assignment for filter x */		//过滤器FIFO0指向过滤器0
#define CAN_FilterFIFO1             ((u8)0x01)  /* Filter FIFO 1 assignment for filter x */		//过滤器FIFO0指向过滤器1

#define IS_CAN_FILTER_FIFO(FIFO) (((FIFO) == CAN_FilterFIFO0) || \
                                  ((FIFO) == CAN_FilterFIFO1))

/* CAN Tx */
#define IS_CAN_TRANSMITMAILBOX(TRANSMITMAILBOX) ((TRANSMITMAILBOX) <= ((u8)0x02))
#define IS_CAN_STDID(STDID)   ((STDID) <= ((u32)0x7FF))
#define IS_CAN_EXTID(EXTID)   ((EXTID) <= ((u32)0x1FFFFFFF))
#define IS_CAN_DLC(DLC)       ((DLC) <= ((u8)0x08))

/* CAN identifier type */
#define CAN_ID_STD                 ((u32)0x00000000)  /* Standard Id */		//使用标准帧标识符
#define CAN_ID_EXT                 ((u32)0x00000004)  /* Extended Id */		//使用扩展帧标识符

#define IS_CAN_IDTYPE(IDTYPE) (((IDTYPE) == CAN_ID_STD) || ((IDTYPE) == CAN_ID_EXT))

/* CAN remote transmission request */
#define CAN_RTR_DATA                ((u32)0x00000000)  /* Data frame */		//数据帧
#define CAN_RTR_REMOTE              ((u32)0x00000002)  /* Remote frame */	//远程帧

#define IS_CAN_RTR(RTR) (((RTR) == CAN_RTR_DATA) || ((RTR) == CAN_RTR_REMOTE))

/* CAN transmit constants */
#define CANTXFAILED                 ((u8)0x00) /* CAN transmission failed */
#define CANTXOK                     ((u8)0x01) /* CAN transmission succeeded */
#define CANTXPENDING                ((u8)0x02) /* CAN transmission pending */
#define CAN_NO_MB                   ((u8)0x04) /* CAN cell did not provide an empty mailbox */

/* CAN receive FIFO number constants */
#define CAN_FIFO0                 ((u8)0x00) /* CAN FIFO0 used to receive */
#define CAN_FIFO1                 ((u8)0x01) /* CAN FIFO1 used to receive */

#define IS_CAN_FIFO(FIFO) (((FIFO) == CAN_FIFO0) || ((FIFO) == CAN_FIFO1))

/* CAN sleep constants */
#define CANSLEEPFAILED              ((u8)0x00) /* CAN did not enter the sleep mode */
#define CANSLEEPOK                  ((u8)0x01) /* CAN entered the sleep mode */

/* CAN wake up constants */
#define CANWAKEUPFAILED             ((u8)0x00) /* CAN did not leave the sleep mode */
#define CANWAKEUPOK                 ((u8)0x01) /* CAN leaved the sleep mode */

/* CAN flags */
#define CAN_FLAG_EWG                ((u32)0x00000001) /* Error Warning Flag */
#define CAN_FLAG_EPV                ((u32)0x00000002) /* Error Passive Flag */
#define CAN_FLAG_BOF                ((u32)0x00000004) /* Bus-Off Flag */

#define IS_CAN_FLAG(FLAG) (((FLAG) == CAN_FLAG_EWG) || ((FLAG) == CAN_FLAG_EPV) ||\
                           ((FLAG) == CAN_FLAG_BOF))

/* CAN interrupts */					//CAN中断源
#define CAN_IT_RQCP0                ((u32)0x00000005) /* Request completed mailbox 0 */			//邮箱1请求完成
#define CAN_IT_RQCP1                ((u32)0x00000006) /* Request completed mailbox 1 */			//邮箱2请求完成
#define CAN_IT_RQCP2                ((u32)0x00000007) /* Request completed mailbox 2 */			//邮箱3请求完成			
#define CAN_IT_TME                  ((u32)0x00000001) /* Transmit mailbox empty */					//发送邮箱空中断屏蔽
#define CAN_IT_FMP0                 ((u32)0x00000002) /* FIFO 0 message pending */					//FIFO0消息挂号中断屏蔽
#define CAN_IT_FF0                  ((u32)0x00000004) /* FIFO 0 full */											//FIFO0满中断屏蔽
#define CAN_IT_FOV0                 ((u32)0x00000008) /* FIFO 0 overrun */									//FIFO0溢出中断屏蔽
#define CAN_IT_FMP1                 ((u32)0x00000010) /* FIFO 1 message pending */					//FIFO1消息挂号中断屏蔽
#define CAN_IT_FF1                  ((u32)0x00000020) /* FIFO 1 full */											//FIFO1满中断屏蔽
#define CAN_IT_FOV1                 ((u32)0x00000040) /* FIFO 1 overrun */									//FIFO1溢出中断屏蔽
#define CAN_IT_EWG                  ((u32)0x00000100) /* Error warning */										//错误警告中断屏蔽
#define CAN_IT_EPV                  ((u32)0x00000200) /* Error passive */										//错误被动中断屏蔽
#define CAN_IT_BOF                  ((u32)0x00000400) /* Bus-off */													//离线中断屏蔽
#define CAN_IT_LEC                  ((u32)0x00000800) /* Last error code */									//上次错误号中断屏蔽
#define CAN_IT_ERR                  ((u32)0x00008000) /* Error */														//错误中断屏蔽
#define CAN_IT_WKU                  ((u32)0x00010000) /* Wake-up */													//唤醒中断屏蔽
#define CAN_IT_SLK                  ((u32)0x00020000) /* Sleep */														//睡眠标志位中断屏蔽

#define IS_CAN_ITConfig(IT) (((IT) == CAN_IT_TME)   || ((IT) == CAN_IT_FMP0)  ||\
                             ((IT) == CAN_IT_FF0)   || ((IT) == CAN_IT_FOV0)  ||\
                             ((IT) == CAN_IT_FMP1)  || ((IT) == CAN_IT_FF1)   ||\
                             ((IT) == CAN_IT_FOV1)  || ((IT) == CAN_IT_EWG)   ||\
                             ((IT) == CAN_IT_EPV)   || ((IT) == CAN_IT_BOF)   ||\
                             ((IT) == CAN_IT_LEC)   || ((IT) == CAN_IT_ERR)   ||\
                             ((IT) == CAN_IT_WKU)   || ((IT) == CAN_IT_SLK))

#define IS_CAN_ITStatus(IT) (((IT) == CAN_IT_RQCP0)  || ((IT) == CAN_IT_RQCP1)  ||\
                             ((IT) == CAN_IT_RQCP2)  || ((IT) == CAN_IT_FF0)    ||\
                             ((IT) == CAN_IT_FOV0)   || ((IT) == CAN_IT_FF1)    ||\
                             ((IT) == CAN_IT_FOV1)   || ((IT) == CAN_IT_EWG)    ||\
                             ((IT) == CAN_IT_EPV)    || ((IT) == CAN_IT_BOF)    ||\
                             ((IT) == CAN_IT_WKU)    || ((IT) == CAN_IT_SLK))

/* Exported macro ------------------------------------------------------------*/
/* Exported function protypes ----------------------------------------------- */
void CAN_DeInit(void);																						//将外设CAN的全部寄存器重设为缺省值
u8 CAN_Init(CAN_InitTypeDef* CAN_InitStruct);											//根据CAN_InitStruct中指定的参数初始化外设CAN的寄存器
void CAN_FilterInit(CAN_FilterInitTypeDef* CAN_FilterInitStruct);	//根据CAN_FilterInitStruct中指定的参数初始化外设CAN的寄存器
void CAN_StructInit(CAN_InitTypeDef* CAN_InitStruct);
void CAN_ITConfig(u32 CAN_IT, FunctionalState NewState);					//使能或者失能指定的CAN中断,输入参数CAN_IT为待使能或者失能的CAN中断。
u8 CAN_Transmit(CanTxMsg* TxMessage);															//开始一个消息的传输
u8 CAN_TransmitStatus(u8 TransmitMailbox);												//检查消息传输的状态
void CAN_CancelTransmit(u8 Mailbox);															//取消一个传输请求
void CAN_FIFORelease(u8 FIFONumber);															//释放一个FIFO
u8 CAN_MessagePending(u8 FIFONumber);															//返回挂号的信息数量
void CAN_Receive(u8 FIFONumber, CanRxMsg* RxMessage);							//接收一个消息
u8 CAN_Sleep(void);																								//使CAN进入低功耗模式
u8 CAN_WakeUp(void);																							//将CAN唤醒
FlagStatus CAN_GetFlagStatus(u32 CAN_FLAG);												//检查指定的CAN标志位被设置与否
void CAN_ClearFlag(u32 CAN_FLAG);																	//清除CAN的待处理标志位
ITStatus CAN_GetITStatus(u32 CAN_IT);															//检查指定的CAN中断发生与否
void CAN_ClearITPendingBit(u32 CAN_IT);														//清除CAN中断待处理标志位

#endif /* __STM32F10x_CAN_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
