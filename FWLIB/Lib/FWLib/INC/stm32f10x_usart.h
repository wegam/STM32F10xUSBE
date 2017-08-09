/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : stm32f10x_usart.h
* Author             : MCD Application Team
* Version            : V2.0.1
* Date               : 06/13/2008
* Description        : This file contains all the functions prototypes for the
*                      USART firmware library.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_USART_H
#define __STM32F10x_USART_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_map.h"

/* Exported types ------------------------------------------------------------*/
/* USART Init Structure definition */
typedef struct
{
  u32 USART_BaudRate;			//USART传输的波特率，波特率可以由以下公式计算：IntegerDivider = ((APBClock) / (16 * (USART_InitStruct->USART_BaudRate)))；FractionalDivider = ((IntegerDivider - ((u32) IntegerDivider)) * 16) + 0.5
  u16 USART_WordLength;		//在一个帧中传输或者接收到的数据位数		：Line62:USART Word Length
  u16 USART_StopBits;			//发送的停止位数目
  u16 USART_Parity;				//奇偶校验模式
  u16 USART_Mode;					//使能或者失能发送和接收模式
  u16 USART_HardwareFlowControl; 	//硬件流控制模式 
} USART_InitTypeDef;

/* USART Clock Init Structure definition */
typedef struct
{
  u16 USART_Clock;
  u16 USART_CPOL;
  u16 USART_CPHA;
  u16 USART_LastBit;
} USART_ClockInitTypeDef;

/* Exported constants --------------------------------------------------------*/
#define IS_USART_ALL_PERIPH(PERIPH) (((*(u32*)&(PERIPH)) == USART1_BASE) || \
                                     ((*(u32*)&(PERIPH)) == USART2_BASE) || \
                                     ((*(u32*)&(PERIPH)) == USART3_BASE) || \
                                     ((*(u32*)&(PERIPH)) == UART4_BASE) || \
                                     ((*(u32*)&(PERIPH)) == UART5_BASE))

#define IS_USART_123_PERIPH(PERIPH) (((*(u32*)&(PERIPH)) == USART1_BASE) || \
                                     ((*(u32*)&(PERIPH)) == USART2_BASE) || \
                                     ((*(u32*)&(PERIPH)) == USART3_BASE))

#define IS_USART_1234_PERIPH(PERIPH) (((*(u32*)&(PERIPH)) == USART1_BASE) || \
                                      ((*(u32*)&(PERIPH)) == USART2_BASE) || \
                                      ((*(u32*)&(PERIPH)) == USART3_BASE) || \
                                      ((*(u32*)&(PERIPH)) == UART4_BASE))

/* USART Word Length ---------------------------------------------------------*/
#define USART_WordLength_8b                  ((u16)0x0000)		//数据位-8位
#define USART_WordLength_9b                  ((u16)0x1000)		//数据位-9位
                                    
#define IS_USART_WORD_LENGTH(LENGTH) (((LENGTH) == USART_WordLength_8b) || \
                                      ((LENGTH) == USART_WordLength_9b))

/* USART Stop Bits -----------------------------------------------------------*/
#define USART_StopBits_1                     ((u16)0x0000)	//停止位：在帧结尾传输1个停止位
#define USART_StopBits_0_5                   ((u16)0x1000)	//停止位：在帧结尾传输0.5个停止位
#define USART_StopBits_2                     ((u16)0x2000)	//停止位：在帧结尾传输2个停止位
#define USART_StopBits_1_5                   ((u16)0x3000)	//停止位：在帧结尾传输1.5个停止位

#define IS_USART_STOPBITS(STOPBITS) (((STOPBITS) == USART_StopBits_1) || \
                                     ((STOPBITS) == USART_StopBits_0_5) || \
                                     ((STOPBITS) == USART_StopBits_2) || \
                                     ((STOPBITS) == USART_StopBits_1_5))
/* USART Parity --------------------------------------------------------------*/
#define USART_Parity_No                      ((u16)0x0000)	//奇偶校验-关闭
#define USART_Parity_Even                    ((u16)0x0400)	//奇偶校验-偶校验
#define USART_Parity_Odd                     ((u16)0x0600) 	//奇偶校验-奇校验

#define IS_USART_PARITY(PARITY) (((PARITY) == USART_Parity_No) || \
                                 ((PARITY) == USART_Parity_Even) || \
                                 ((PARITY) == USART_Parity_Odd))

/* USART Mode ----------------------------------------------------------------*/
#define USART_Mode_Rx                        ((u16)0x0004)	//接收使能
#define USART_Mode_Tx                        ((u16)0x0008)	//发送使能

#define IS_USART_MODE(MODE) ((((MODE) & (u16)0xFFF3) == 0x00) && ((MODE) != (u16)0x00))

/* USART Hardware Flow Control -----------------------------------------------*/
#define USART_HardwareFlowControl_None       ((u16)0x0000)	//硬件流控制失能
#define USART_HardwareFlowControl_RTS        ((u16)0x0100)	//发送请求RTS使能
#define USART_HardwareFlowControl_CTS        ((u16)0x0200)	//清除发送CTS使能
#define USART_HardwareFlowControl_RTS_CTS    ((u16)0x0300)	//RTS和CTS使能

#define IS_USART_HARDWARE_FLOW_CONTROL(CONTROL)\
                              (((CONTROL) == USART_HardwareFlowControl_None) || \
                               ((CONTROL) == USART_HardwareFlowControl_RTS) || \
                               ((CONTROL) == USART_HardwareFlowControl_CTS) || \
                               ((CONTROL) == USART_HardwareFlowControl_RTS_CTS))

#define IS_USART_PERIPH_HFC(PERIPH, HFC) ((((*(u32*)&(PERIPH)) != UART4_BASE) && \
                                          ((*(u32*)&(PERIPH)) != UART5_BASE)) \
                                          || ((HFC) == USART_HardwareFlowControl_None))                                

/* USART Clock ---------------------------------------------------------------*/
#define USART_Clock_Disable                  ((u16)0x0000)
#define USART_Clock_Enable                   ((u16)0x0800)

#define IS_USART_CLOCK(CLOCK) (((CLOCK) == USART_Clock_Disable) || \
                               ((CLOCK) == USART_Clock_Enable))

/* USART Clock Polarity ------------------------------------------------------*/
#define USART_CPOL_Low                       ((u16)0x0000)
#define USART_CPOL_High                      ((u16)0x0400)

#define IS_USART_CPOL(CPOL) (((CPOL) == USART_CPOL_Low) || ((CPOL) == USART_CPOL_High))
                               
/* USART Clock Phase ---------------------------------------------------------*/
#define USART_CPHA_1Edge                     ((u16)0x0000)
#define USART_CPHA_2Edge                     ((u16)0x0200)
#define IS_USART_CPHA(CPHA) (((CPHA) == USART_CPHA_1Edge) || ((CPHA) == USART_CPHA_2Edge))

/* USART Last Bit ------------------------------------------------------------*/
#define USART_LastBit_Disable                ((u16)0x0000)
#define USART_LastBit_Enable                 ((u16)0x0100)

#define IS_USART_LASTBIT(LASTBIT) (((LASTBIT) == USART_LastBit_Disable) || \
                                   ((LASTBIT) == USART_LastBit_Enable))

/* USART Interrupt definition ------------------------------------------------*/
#define USART_IT_PE                          ((u16)0x0028)					//奇偶错误中断
#define USART_IT_TXE                         ((u16)0x0727)					//发送中断
#define USART_IT_TC                          ((u16)0x0626)					//传输完成中断
#define USART_IT_RXNE                        ((u16)0x0525)					//接收中断
#define USART_IT_IDLE                        ((u16)0x0424)					//USART空闲中断->当检测到总线空闲时，该位被硬件置位。如果USART_CR1中的IDLEIE为’1’，则产生中断。由软件序列清除该位(先读USART_SR，然后读USART_DR)。
#define USART_IT_LBD                         ((u16)0x0846)					//LIN中断检测中断
#define USART_IT_CTS                         ((u16)0x096A)					//CTS中断
#define USART_IT_ERR                         ((u16)0x0060)					//错误中断
#define USART_IT_ORE                         ((u16)0x0360)					//过载错误 (Overrun error)
#define USART_IT_NE                          ((u16)0x0260)					//噪声错误标志 (Noise error flag)
#define USART_IT_FE                          ((u16)0x0160)					//帧错误 (Framing error)

#define IS_USART_CONFIG_IT(IT) (((IT) == USART_IT_PE) || ((IT) == USART_IT_TXE) || \
                               ((IT) == USART_IT_TC) || ((IT) == USART_IT_RXNE) || \
                               ((IT) == USART_IT_IDLE) || ((IT) == USART_IT_LBD) || \
                               ((IT) == USART_IT_CTS) || ((IT) == USART_IT_ERR))

#define IS_USART_IT(IT) (((IT) == USART_IT_PE) || ((IT) == USART_IT_TXE) || \
                         ((IT) == USART_IT_TC) || ((IT) == USART_IT_RXNE) || \
                         ((IT) == USART_IT_IDLE) || ((IT) == USART_IT_LBD) || \
                         ((IT) == USART_IT_CTS) || ((IT) == USART_IT_ORE) || \
                         ((IT) == USART_IT_NE) || ((IT) == USART_IT_FE))

#define IS_USART_CLEAR_IT(IT) (((IT) == USART_IT_PE) || ((IT) == USART_IT_TC) || \
                               ((IT) == USART_IT_RXNE) || ((IT) == USART_IT_IDLE) || \
                               ((IT) == USART_IT_LBD) || ((IT) == USART_IT_CTS) ||  \
                               ((IT) == USART_IT_ORE) || ((IT) == USART_IT_NE) || \
                               ((IT) == USART_IT_FE))

#define IS_USART_PERIPH_IT(PERIPH, USART_IT) ((((*(u32*)&(PERIPH)) != UART4_BASE) && \
                                              ((*(u32*)&(PERIPH)) != UART5_BASE)) \
                                              || ((USART_IT) != USART_IT_CTS))                                                                           

/* USART DMA Requests --------------------------------------------------------*/
#define USART_DMAReq_Tx                      ((u16)0x0080)
#define USART_DMAReq_Rx                      ((u16)0x0040)

#define IS_USART_DMAREQ(DMAREQ) ((((DMAREQ) & (u16)0xFF3F) == 0x00) && ((DMAREQ) != (u16)0x00))

/* USART WakeUp methods ------------------------------------------------------*/
#define USART_WakeUp_IdleLine                ((u16)0x0000)
#define USART_WakeUp_AddressMark             ((u16)0x0800)

#define IS_USART_WAKEUP(WAKEUP) (((WAKEUP) == USART_WakeUp_IdleLine) || \
                                 ((WAKEUP) == USART_WakeUp_AddressMark))

/* USART LIN Break Detection Length ------------------------------------------*/
#define USART_LINBreakDetectLength_10b      ((u16)0x0000)
#define USART_LINBreakDetectLength_11b      ((u16)0x0020)

#define IS_USART_LIN_BREAK_DETECT_LENGTH(LENGTH) \
                               (((LENGTH) == USART_LINBreakDetectLength_10b) || \
                                ((LENGTH) == USART_LINBreakDetectLength_11b))

/* USART IrDA Low Power ------------------------------------------------------*/
#define USART_IrDAMode_LowPower              ((u16)0x0004)
#define USART_IrDAMode_Normal                ((u16)0x0000)

#define IS_USART_IRDA_MODE(MODE) (((MODE) == USART_IrDAMode_LowPower) || \
                                  ((MODE) == USART_IrDAMode_Normal))

/* USART Flags ---------------------------------------------------------------*/
#define USART_FLAG_CTS                       ((u16)0x0200)		//CTS 标志 (CTS flag)	
																															//如果设置了CTSE位，当nCTS输入变化状态时，该位被硬件置高。由软件将其清零。如果USART_CR3中的CTSIE为’1’，则产生中断。
																															//0：nCTS状态线上没有变化；
																															//1：nCTS状态线上发生变化。
																															//注：UART4和UART5上不存在这一位。
#define USART_FLAG_LBD                       ((u16)0x0100)		//LIN断开检测标志 (LIN break detection flag)
																															//当探测到LIN断开时，该位由硬件置’1’，由软件清’0’(向该位写0)。如果USART_CR3中的LBDIE = 1，则产生中断。
																															//0：没有检测到LIN断开；
																															//1：检测到LIN断开。
																															//注意：若LBDIE=1，当LBD为’1’时要产生中断。
#define USART_FLAG_TXE                       ((u16)0x0080)		//TXE:发送数据寄存器空 (Transmit data register empty)
																															//当TDR寄存器中的数据被硬件转移到移位寄存器的时候，该位被硬件置位。如果USART_CR1寄存器中的TXEIE为1，则产生中断。对USART_DR的写操作，将该位清零。
																															//0：数据还没有被转移到移位寄存器；
																															//1：数据已经被转移到移位寄存器。
																															//注意：单缓冲器传输中使用该位。
#define USART_FLAG_TC                        ((u16)0x0040)		//发送完成 (Transmission complete)
																															//当包含有数据的一帧发送完成后，并且TXE=1时，由硬件将该位置’1’。如果USART_CR1中的TCIE为’1’，则产生中断。
																															//由软件序列清除该位(先读USART_SR，然后写入USART_DR)。TC位也可以通过写入’0’来清除，只有在多缓存通讯中才推荐这种清除程序。
																															//0：发送还未完成；
																															//1：发送完成。
#define USART_FLAG_RXNE                      ((u16)0x0020)		//RXNE：读数据寄存器非空 (Read data register not empty)
																															//当RDR移位寄存器中的数据被转移到USART_DR寄存器中，该位被硬件置位。如果USART_CR1寄存器中的RXNEIE为1，则产生中断。
																															//对USART_DR的读操作可以将该位清零。RXNE位也可以通过写入0来清除，只有在多缓存通讯中才推荐这种清除程序。
																															//0：数据没有收到；
																															//1：收到数据，可以读出。
#define USART_FLAG_IDLE                      ((u16)0x0010)		//IDLE：监测到总线空闲 (IDLE line detected)
																															//当检测到总线空闲时，该位被硬件置位。如果USART_CR1中的IDLEIE为’1’，则产生中断。由软件序列清除该位(先读USART_SR，然后读USART_DR)。
																															//0：没有检测到空闲总线；
																															//1：检测到空闲总线。
																															//注意：IDLE位不会再次被置高直到RXNE位被置起(即又检测到一次空闲总线)
#define USART_FLAG_ORE                       ((u16)0x0008)		//过载错误 (Overrun error)
																															//当RXNE仍然是’1’的时候，当前被接收在移位寄存器中的数据，需要传送至RDR寄存器时，硬件将该位置位。
																															//如果USART_CR1中的RXNEIE为’1’的话，则产生中断。由软件序列将其清零(先读USART_SR，然后读USART_CR)。
																															//0：没有过载错误；
																															//1：检测到过载错误。
																															//注意：该位被置位时，RDR寄存器中的值不会丢失，但是移位寄存器中的数据会被覆盖。如果设置了EIE位，在多缓冲器通信模式下，ORE标志置位会产生中断的。
#define USART_FLAG_NE                        ((u16)0x0004)		//噪声错误标志 (Noise error flag)
																															//在接收到的帧检测到噪音时，由硬件对该位置位。由软件序列对其清玲(先读USART_SR，再读USART_DR)。
																															//0：没有检测到噪声；
																															//1：检测到噪声。
																															//注意：该位不会产生中断，因为它和RXNE一起出现，硬件会在设置RXNE标志时产生中断。在多缓冲区通信模式下，如果设置了EIE位，则设置NE标志时会产生中断。
#define USART_FLAG_FE                        ((u16)0x0002)		//帧错误 (Framing error)
																															//当检测到同步错位，过多的噪声或者检测到断开符，该位被硬件置位。由软件序列将其清零(先读USART_SR，再读USART_DR)。
																															//0：没有检测到帧错误；
																															//1：检测到帧错误或者break符。
#define USART_FLAG_PE                        ((u16)0x0001)		//校验错误 (Parity error)
																															//在接收模式下，如果出现奇偶校验错误，硬件对该位置位。由软件序列对其清零(依次读USART_SR和USART_DR)。在清除PE位前，软件必须等待RXNE标志位被置’1’。如果USART_CR1中的PEIE为’1’，则产生中断。

#define IS_USART_FLAG(FLAG) (((FLAG) == USART_FLAG_PE) || ((FLAG) == USART_FLAG_TXE) || \
                             ((FLAG) == USART_FLAG_TC) || ((FLAG) == USART_FLAG_RXNE) || \
                             ((FLAG) == USART_FLAG_IDLE) || ((FLAG) == USART_FLAG_LBD) || \
                             ((FLAG) == USART_FLAG_CTS) || ((FLAG) == USART_FLAG_ORE) || \
                             ((FLAG) == USART_FLAG_NE) || ((FLAG) == USART_FLAG_FE))
                              
#define IS_USART_CLEAR_FLAG(FLAG) ((((FLAG) & (u16)0xFC80) == 0x00) && ((FLAG) != (u16)0x00))

#define IS_USART_PERIPH_FLAG(PERIPH, USART_FLAG) ((((*(u32*)&(PERIPH)) != UART4_BASE) &&\
                                                  ((*(u32*)&(PERIPH)) != UART5_BASE)) \
                                                  || ((USART_FLAG) != USART_FLAG_CTS)) 

#define IS_USART_BAUDRATE(BAUDRATE) (((BAUDRATE) > 0) && ((BAUDRATE) < 0x0044AA21))
#define IS_USART_ADDRESS(ADDRESS) ((ADDRESS) <= 0xF)
#define IS_USART_DATA(DATA) ((DATA) <= 0x1FF)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void USART_DeInit(USART_TypeDef* USARTx);
void USART_Init(USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStruct);
void USART_StructInit(USART_InitTypeDef* USART_InitStruct);
void USART_ClockInit(USART_TypeDef* USARTx, USART_ClockInitTypeDef* USART_ClockInitStruct);
void USART_ClockStructInit(USART_ClockInitTypeDef* USART_ClockInitStruct);
void USART_Cmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_ITConfig(USART_TypeDef* USARTx, u16 USART_IT, FunctionalState NewState);
void USART_DMACmd(USART_TypeDef* USARTx, u16 USART_DMAReq, FunctionalState NewState);
void USART_SetAddress(USART_TypeDef* USARTx, u8 USART_Address);
void USART_WakeUpConfig(USART_TypeDef* USARTx, u16 USART_WakeUp);
void USART_ReceiverWakeUpCmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_LINBreakDetectLengthConfig(USART_TypeDef* USARTx, u16 USART_LINBreakDetectLength);
void USART_LINCmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_SendData(USART_TypeDef* USARTx, u16 Data);
u16 USART_ReceiveData(USART_TypeDef* USARTx);
void USART_SendBreak(USART_TypeDef* USARTx);
void USART_SetGuardTime(USART_TypeDef* USARTx, u8 USART_GuardTime);
void USART_SetPrescaler(USART_TypeDef* USARTx, u8 USART_Prescaler);
void USART_SmartCardCmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_SmartCardNACKCmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_HalfDuplexCmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_IrDAConfig(USART_TypeDef* USARTx, u16 USART_IrDAMode);
void USART_IrDACmd(USART_TypeDef* USARTx, FunctionalState NewState);
FlagStatus USART_GetFlagStatus(USART_TypeDef* USARTx, u16 USART_FLAG);
void USART_ClearFlag(USART_TypeDef* USARTx, u16 USART_FLAG);
ITStatus USART_GetITStatus(USART_TypeDef* USARTx, u16 USART_IT);
void USART_ClearITPendingBit(USART_TypeDef* USARTx, u16 USART_IT);

#endif /* __STM32F10x_USART_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
