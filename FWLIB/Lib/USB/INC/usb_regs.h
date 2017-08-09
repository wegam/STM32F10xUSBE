/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_regs.h
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Interface prototype functions to USB cell registers
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_REGS_H
#define __USB_REGS_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum _EP_DBUF_DIR
{
  /* double buffered endpoint direction */
  EP_DBUF_ERR,
  EP_DBUF_OUT,
  EP_DBUF_IN
}EP_DBUF_DIR;

/* endpoint buffer number */
enum EP_BUF_NUM
{
  EP_NOBUF,
  EP_BUF0,
  EP_BUF1
};

/* Exported constants --------------------------------------------------------*/
#define RegBase  (0x40005C00L)  /* USB_IP Peripheral Registers base address */
#define PMAAddr  (0x40006000L)  /* USB_IP Packet Memory Area base address   */

/******************************************************************************/
/*                         General registers                                  */
/******************************************************************************/

/* Control register */							//控制寄存器
#define CNTR    ((volatile unsigned *)(RegBase + 0x40))
/* Interrupt status register */			//中断状态寄存器
#define ISTR    ((volatile unsigned *)(RegBase + 0x44))
/* Frame number register */					//帧编号寄存器
#define FNR     ((volatile unsigned *)(RegBase + 0x48))
/* Device address register */				//设备地址寄存器
#define DADDR   ((volatile unsigned *)(RegBase + 0x4C))
/* Buffer Table address register */	//缓冲区列表寄存器
#define BTABLE  ((volatile unsigned *)(RegBase + 0x50))
/******************************************************************************/
/*                         Endpoint registers                                 */
/******************************************************************************/
#define EP0REG  ((volatile unsigned *)(RegBase)) /* endpoint 0 register address */

/* endpoints enumeration */
#define ENDP0   ((u8)0)
#define ENDP1   ((u8)1)
#define ENDP2   ((u8)2)
#define ENDP3   ((u8)3)
#define ENDP4   ((u8)4)
#define ENDP5   ((u8)5)
#define ENDP6   ((u8)6)
#define ENDP7   ((u8)7)
/******************************************************************************/
/*                       ISTR interrupt events                                */
/*											USB中断状态寄存器(USB_ISTR)														*/
/******************************************************************************/
#define ISTR_CTR    (0x8000) /* Correct TRansfer (clear-only bit) */				//正确的传输 (Correct transfer)
#define ISTR_DOVR   (0x4000) /* DMA OVeR/underrun (clear-only bit) */				//分组缓冲区溢出 (Packet memory area over / underrun)
#define ISTR_ERR    (0x2000) /* ERRor (clear-only bit) */										//出错 (Error)
#define ISTR_WKUP   (0x1000) /* WaKe UP (clear-only bit) */									//唤醒请求 (Wakeup)
#define ISTR_SUSP   (0x0800) /* SUSPend (clear-only bit) */									//挂起模块请求 (Suspend mode request)
#define ISTR_RESET  (0x0400) /* RESET (clear-only bit) */										//USB复位请求 (USB reset request)
#define ISTR_SOF    (0x0200) /* Start Of Frame (clear-only bit) */					//帧首标志 (Start of frame)
#define ISTR_ESOF   (0x0100) /* Expected Start Of Frame (clear-only bit) */	//期望帧首标识位 (Expected start of frame)


#define ISTR_DIR    (0x0010)  /* DIRection of transaction (read-only bit)  */	//传输方向 (Direction of transaction)	DIR＝0:标志一个IN分组(数据从USB模块传输到PC主机)的传输完成;DIR＝1:标志一个OUT分组(数据从PC主机传输到USB模块)的传输完成
#define ISTR_EP_ID  (0x000F)  /* EndPoint IDentifier (read-only bit)  */			//端点ID (Endpoint Identifier)

#define CLR_CTR    (~ISTR_CTR)   /* clear Correct TRansfer bit */
#define CLR_DOVR   (~ISTR_DOVR)  /* clear DMA OVeR/underrun bit*/
#define CLR_ERR    (~ISTR_ERR)   /* clear ERRor bit */
#define CLR_WKUP   (~ISTR_WKUP)  /* clear WaKe UP bit     */
#define CLR_SUSP   (~ISTR_SUSP)  /* clear SUSPend bit     */
#define CLR_RESET  (~ISTR_RESET) /* clear RESET bit      */
#define CLR_SOF    (~ISTR_SOF)   /* clear Start Of Frame bit   */
#define CLR_ESOF   (~ISTR_ESOF)  /* clear Expected Start Of Frame bit */

/******************************************************************************/
/*             CNTR control register bits definitions                         */
/*									USB控制寄存器(USB_CNTR)																		*/
/******************************************************************************/
#define CNTR_CTRM   (0x8000) /* Correct TRansfer Mask */					//正确传输(CTR)中断屏蔽位 (Correct transfer interrupt mask)
#define CNTR_DOVRM  (0x4000) /* DMA OVeR/underrun Mask */					//分组缓冲区溢出中断屏蔽位 (Packet memory area over / underrun interrupt mask)
#define CNTR_ERRM   (0x2000) /* ERRor Mask */											//出错中断屏蔽位 (Error interrupt mask)
#define CNTR_WKUPM  (0x1000) /* WaKe UP Mask */										//唤醒中断屏蔽位 (Wakeup interrupt mask)
#define CNTR_SUSPM  (0x0800) /* SUSPend Mask */										//挂起中断屏蔽位 (Suspend mode interrupt mask)
#define CNTR_RESETM (0x0400) /* RESET Mask   */										//USB复位中断屏蔽位 (USB reset interrupt mask)
#define CNTR_SOFM   (0x0200) /* Start Of Frame Mask */						//帧首中断屏蔽位 (Start of frame interrupt mask)
#define CNTR_ESOFM  (0x0100) /* Expected Start Of Frame Mask */		//期望帧首中断屏蔽位 (Expected start of frame interrupt mask)


#define CNTR_RESUME (0x0010) /* RESUME request */									//唤醒请求 (Resume request)
#define CNTR_FSUSP  (0x0008) /* Force SUSPend */									//强制挂起 (Force suspend)
#define CNTR_LPMODE (0x0004) /* Low-power MODE */									//低功耗模式 (Low-power mode)
#define CNTR_PDWN   (0x0002) /* Power DoWN */											//断电模式 (Power down)
#define CNTR_FRES   (0x0001) /* Force USB RESet */								//强制USB复位

/******************************************************************************/
/*                FNR Frame Number Register bit definitions                   */
/*									USB帧编号寄存器(USB_FNR)																		*/
/******************************************************************************/
#define FNR_RXDP (0x8000) /* status of D+ data line */	//D+状态位 (Receive data + line status)
#define FNR_RXDM (0x4000) /* status of D- data line */	//D-状态位 (Receive data - line status)
#define FNR_LCK  (0x2000) /* LoCKed */									//锁定位 (Locked)
#define FNR_LSOF (0x1800) /* Lost SOF */								//帧首丢失标志位 (Lost SOF)
#define FNR_FN  (0x07FF) /* Frame Number */							//帧编号 (Frame number)
/******************************************************************************/
/*               DADDR Device ADDRess bit definitions                         */
/*  								USB设备地址寄存器(USB_DADDR)																*/
/******************************************************************************/
#define DADDR_EF (0x80)				//USB模块使能位 (Enable function)
#define DADDR_ADD (0x7F)			//设备地址 (evice address)
/******************************************************************************/
/*                            Endpoint register                               */
/* 												USB 端点n寄存器(USB_EPnR), n=[0..7]									*/
/******************************************************************************/
/* bit positions */
#define EP_CTR_RX      (0x8000) /* EndPoint Correct TRansfer RX */	//正确接收标志位 (Correct Transfer for reception)
#define EP_DTOG_RX     (0x4000) /* EndPoint Data TOGGLE RX */				//用于数据接收的数据翻转位 (Data Toggle, for reception transfers)
#define EPRX_STAT      (0x3000) /* EndPoint RX STATus bit field */	//用于数据接收的状态位 (Status bits, for reception transfers)
#define EP_SETUP       (0x0800) /* EndPoint SETUP */								//SETUP分组传输完成标志位 (Setup transaction completed)
#define EP_T_FIELD     (0x0600) /* EndPoint TYPE */									//端点类型位 (Endpoint type)
#define EP_KIND        (0x0100) /* EndPoint KIND */									//端点特殊类型位 (Endpoint kind)
#define EP_CTR_TX      (0x0080) /* EndPoint Correct TRansfer TX */	//正确发送标志位 (Correct transfer for transmission)
#define EP_DTOG_TX     (0x0040) /* EndPoint Data TOGGLE TX */				//发送数据翻转位 (Data Toggle, for transmission transfers)
#define EPTX_STAT      (0x0030) /* EndPoint TX STATus bit field */	//用于发送数据的状态位 (Status bits, for transmission transfers)
#define EPADDR_FIELD   (0x000F) /* EndPoint ADDRess FIELD */				//端点地址 (Endpoint address)

/* EndPoint REGister MASK (no toggle fields) */
#define EPREG_MASK     (EP_CTR_RX|EP_SETUP|EP_T_FIELD|EP_KIND|EP_CTR_TX|EPADDR_FIELD)

/* EP_TYPE[1:0] EndPoint TYPE */	//端点类型字段
#define EP_TYPE_MASK   (0x0600) /* EndPoint TYPE Mask */
#define EP_BULK        (0x0000) /* EndPoint BULK */					//大容量块传输
#define EP_CONTROL     (0x0200) /* EndPoint CONTROL */			//控制传输
#define EP_ISOCHRONOUS (0x0400) /* EndPoint ISOCHRONOUS */	//同步传输
#define EP_INTERRUPT   (0x0600) /* EndPoint INTERRUPT */		//中断传输
#define EP_T_MASK      (~EP_T_FIELD & EPREG_MASK)


/* EP_KIND EndPoint KIND */
#define EPKIND_MASK    (~EP_KIND & EPREG_MASK)

/* STAT_TX[1:0] STATus for TX transfer */
#define EP_TX_DIS      (0x0000) /* EndPoint TX DISabled */					//端点发送关闭
#define EP_TX_STALL    (0x0010) /* EndPoint TX STALLed */						//端点发送延迟
#define EP_TX_NAK      (0x0020) /* EndPoint TX NAKed */							//端点发送不应答
#define EP_TX_VALID    (0x0030) /* EndPoint TX VALID */							//端点发送有效
#define EPTX_DTOG1     (0x0010) /* EndPoint TX Data TOGgle bit1 */	//	
#define EPTX_DTOG2     (0x0020) /* EndPoint TX Data TOGgle bit2 */	//	
#define EPTX_DTOGMASK  (EPTX_STAT|EPREG_MASK)												//	

/* STAT_RX[1:0] STATus for RX transfer */
#define EP_RX_DIS      (0x0000) /* EndPoint RX DISabled */					//端点接收关闭
#define EP_RX_STALL    (0x1000) /* EndPoint RX STALLed */						//端点接收延迟
#define EP_RX_NAK      (0x2000) /* EndPoint RX NAKed */							//端点接收不应答
#define EP_RX_VALID    (0x3000) /* EndPoint RX VALID */							//端点接收有效
#define EPRX_DTOG1     (0x1000) /* EndPoint RX Data TOGgle bit1 */
#define EPRX_DTOG2     (0x2000) /* EndPoint RX Data TOGgle bit1 */
#define EPRX_DTOGMASK  (EPRX_STAT|EPREG_MASK)
/* Exported macro ------------------------------------------------------------*/
/* SetCNTR */
#define _SetCNTR(wRegValue)  (*CNTR   = (u16)wRegValue)

/* SetISTR */
#define _SetISTR(wRegValue)  (*ISTR   = (u16)wRegValue)

/* SetDADDR */
#define _SetDADDR(wRegValue) (*DADDR  = (u16)wRegValue)

/* SetBTABLE */
#define _SetBTABLE(wRegValue)(*BTABLE = (u16)(wRegValue & 0xFFF8))

/* GetCNTR */
#define _GetCNTR()   ((u16) *CNTR)			//USB控制寄存器

/* GetISTR */
#define _GetISTR()   ((u16) *ISTR)			//USB中断状态寄存

/* GetFNR */
#define _GetFNR()    ((u16) *FNR)				//USB帧编号寄存

/* GetDADDR */
#define _GetDADDR()  ((u16) *DADDR)			//USB设备地址寄存

/* GetBTABLE */
#define _GetBTABLE() ((u16) *BTABLE)		//USB分组缓冲区描述表地址寄存

/* SetENDPOINT */
#define _SetENDPOINT(bEpNum,wRegValue)  (*(EP0REG + bEpNum)= \
    (u16)wRegValue)

/* GetENDPOINT */
#define _GetENDPOINT(bEpNum)        ((u16)(*(EP0REG + bEpNum)))

/*******************************************************************************
* Macro Name     : SetEPType
* Description    : sets the type in the endpoint register(bits EP_TYPE[1:0])
* Input          : bEpNum: Endpoint Number. 
*                  wType
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPType(bEpNum,wType) (_SetENDPOINT(bEpNum,\
                                  ((_GetENDPOINT(bEpNum) & EP_T_MASK) | wType)))

/*******************************************************************************
* Macro Name     : GetEPType
* Description    : gets the type in the endpoint register(bits EP_TYPE[1:0]) 
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : Endpoint Type
*******************************************************************************/
#define _GetEPType(bEpNum) (_GetENDPOINT(bEpNum) & EP_T_FIELD)

/*******************************************************************************
* Macro Name     : SetEPTxStatus
* Description    : sets the status for tx transfer (bits STAT_TX[1:0]).
* Input          : bEpNum: Endpoint Number. 
*                  wState: new state
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPTxStatus(bEpNum,wState) {\
    register u16 _wRegVal;       \
    _wRegVal = _GetENDPOINT(bEpNum) & EPTX_DTOGMASK;\
    /* toggle first bit ? */     \
    if((EPTX_DTOG1 & wState)!= 0)      \
      _wRegVal ^= EPTX_DTOG1;        \
    /* toggle second bit ?  */         \
    if((EPTX_DTOG2 & wState)!= 0)      \
      _wRegVal ^= EPTX_DTOG2;        \
    _SetENDPOINT(bEpNum, _wRegVal);    \
  } /* _SetEPTxStatus */

/*******************************************************************************
* Macro Name     : SetEPRxStatus
* Description    : sets the status for rx transfer (bits STAT_TX[1:0])
* Input          : bEpNum: Endpoint Number. 
*                  wState: new state.
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPRxStatus(bEpNum,wState) {\
    register u16 _wRegVal;   \
    \
    _wRegVal = _GetENDPOINT(bEpNum) & EPRX_DTOGMASK;\
    /* toggle first bit ? */  \
    if((EPRX_DTOG1 & wState)!= 0) \
      _wRegVal ^= EPRX_DTOG1;  \
    /* toggle second bit ? */  \
    if((EPRX_DTOG2 & wState)!= 0) \
      _wRegVal ^= EPRX_DTOG2;  \
    _SetENDPOINT(bEpNum, _wRegVal); \
  } /* _SetEPRxStatus */
/*******************************************************************************
* Macro Name     : GetEPTxStatus / GetEPRxStatus 
* Description    : gets the status for tx/rx transfer (bits STAT_TX[1:0]
*                  /STAT_RX[1:0])
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : status .
*******************************************************************************/
#define _GetEPTxStatus(bEpNum) ((u16)_GetENDPOINT(bEpNum) & EPTX_STAT)

#define _GetEPRxStatus(bEpNum) ((u16)_GetENDPOINT(bEpNum) & EPRX_STAT)

/*******************************************************************************
* Macro Name     : SetEPTxValid / SetEPRxValid 
* Description    : sets directly the VALID tx/rx-status into the enpoint register
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPTxValid(bEpNum)     (_SetEPTxStatus(bEpNum, EP_TX_VALID))

#define _SetEPRxValid(bEpNum)     (_SetEPRxStatus(bEpNum, EP_RX_VALID))

/*******************************************************************************
* Macro Name     : GetTxStallStatus / GetRxStallStatus.
* Description    : checks stall condition in an endpoint.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : TRUE = endpoint in stall condition.
*******************************************************************************/
#define _GetTxStallStatus(bEpNum) (_GetEPTxStatus(bEpNum) \
                                   == EP_TX_STALL)
#define _GetRxStallStatus(bEpNum) (_GetEPRxStatus(bEpNum) \
                                   == EP_RX_STALL)

/*******************************************************************************
* Macro Name     : SetEP_KIND / ClearEP_KIND.
* Description    : set & clear EP_KIND bit.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEP_KIND(bEpNum)    (_SetENDPOINT(bEpNum, \
                                (_GetENDPOINT(bEpNum) | EP_KIND) & EPREG_MASK))
#define _ClearEP_KIND(bEpNum)  (_SetENDPOINT(bEpNum, \
                                (_GetENDPOINT(bEpNum) & EPKIND_MASK)))

/*******************************************************************************
* Macro Name     : Set_Status_Out / Clear_Status_Out.
* Description    : Sets/clears directly STATUS_OUT bit in the endpoint register.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _Set_Status_Out(bEpNum)    _SetEP_KIND(bEpNum)
#define _Clear_Status_Out(bEpNum)  _ClearEP_KIND(bEpNum)

/*******************************************************************************
* Macro Name     : SetEPDoubleBuff / ClearEPDoubleBuff.
* Description    : Sets/clears directly EP_KIND bit in the endpoint register.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPDoubleBuff(bEpNum)   _SetEP_KIND(bEpNum)
#define _ClearEPDoubleBuff(bEpNum) _ClearEP_KIND(bEpNum)

/*******************************************************************************
* Macro Name     : ClearEP_CTR_RX / ClearEP_CTR_TX.
* Description    : Clears bit CTR_RX / CTR_TX in the endpoint register.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _ClearEP_CTR_RX(bEpNum)   (_SetENDPOINT(bEpNum,\
                                   _GetENDPOINT(bEpNum) & 0x7FFF & EPREG_MASK))
#define _ClearEP_CTR_TX(bEpNum)   (_SetENDPOINT(bEpNum,\
                                   _GetENDPOINT(bEpNum) & 0xFF7F & EPREG_MASK))

/*******************************************************************************
* Macro Name     : ToggleDTOG_RX / ToggleDTOG_TX .
* Description    : Toggles DTOG_RX / DTOG_TX bit in the endpoint register.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _ToggleDTOG_RX(bEpNum)    (_SetENDPOINT(bEpNum, \
                                   EP_DTOG_RX | _GetENDPOINT(bEpNum) & EPREG_MASK))
#define _ToggleDTOG_TX(bEpNum)    (_SetENDPOINT(bEpNum, \
                                   EP_DTOG_TX | _GetENDPOINT(bEpNum) & EPREG_MASK))

/*******************************************************************************
* Macro Name     : ClearDTOG_RX / ClearDTOG_TX.
* Description    : Clears DTOG_RX / DTOG_TX bit in the endpoint register.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _ClearDTOG_RX(bEpNum)  if((_GetENDPOINT(bEpNum) & EP_DTOG_RX) != 0)\
    _ToggleDTOG_RX(bEpNum)
#define _ClearDTOG_TX(bEpNum)  if((_GetENDPOINT(bEpNum) & EP_DTOG_TX) != 0)\
    _ToggleDTOG_TX(bEpNum)
/*******************************************************************************
* Macro Name     : SetEPAddress.
* Description    : Sets address in an endpoint register.
* Input          : bEpNum: Endpoint Number.
*                  bAddr: Address. 
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPAddress(bEpNum,bAddr) _SetENDPOINT(bEpNum,\
    _GetENDPOINT(bEpNum) & EPREG_MASK | bAddr)

/*******************************************************************************
* Macro Name     : GetEPAddress.
* Description    : Gets address in an endpoint register.
* Input          : bEpNum: Endpoint Number.
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _GetEPAddress(bEpNum) ((u8)(_GetENDPOINT(bEpNum) & EPADDR_FIELD))

#define _pEPTxAddr(bEpNum) ((u32 *)((_GetBTABLE()+bEpNum*8  )*2 + PMAAddr))
#define _pEPTxCount(bEpNum) ((u32 *)((_GetBTABLE()+bEpNum*8+2)*2 + PMAAddr))
#define _pEPRxAddr(bEpNum) ((u32 *)((_GetBTABLE()+bEpNum*8+4)*2 + PMAAddr))
#define _pEPRxCount(bEpNum) ((u32 *)((_GetBTABLE()+bEpNum*8+6)*2 + PMAAddr))

/*******************************************************************************
* Macro Name     : SetEPTxAddr / SetEPRxAddr.
* Description    : sets address of the tx/rx buffer.
* Input          : bEpNum: Endpoint Number.
*                  wAddr: address to be set (must be word aligned).
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPTxAddr(bEpNum,wAddr) (*_pEPTxAddr(bEpNum) = ((wAddr >> 1) << 1))
#define _SetEPRxAddr(bEpNum,wAddr) (*_pEPRxAddr(bEpNum) = ((wAddr >> 1) << 1))

/*******************************************************************************
* Macro Name     : GetEPTxAddr / GetEPRxAddr.
* Description    : Gets address of the tx/rx buffer.
* Input          : bEpNum: Endpoint Number.
* Output         : None.
* Return         : address of the buffer.
*******************************************************************************/
#define _GetEPTxAddr(bEpNum) ((u16)*_pEPTxAddr(bEpNum))
#define _GetEPRxAddr(bEpNum) ((u16)*_pEPRxAddr(bEpNum))

/*******************************************************************************
* Macro Name     : SetEPCountRxReg.
* Description    : Sets counter of rx buffer with no. of blocks.
* Input          : pdwReg: pointer to counter.
*                  wCount: Counter.
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _BlocksOf32(dwReg,wCount,wNBlocks) {\
    wNBlocks = wCount >> 5;\
    if((wCount & 0x1f) == 0)\
      wNBlocks--;\
    *pdwReg = (u32)((wNBlocks << 10) | 0x8000);\
  }/* _BlocksOf32 */

#define _BlocksOf2(dwReg,wCount,wNBlocks) {\
    wNBlocks = wCount >> 1;\
    if((wCount & 0x1) != 0)\
      wNBlocks++;\
    *pdwReg = (u32)(wNBlocks << 10);\
  }/* _BlocksOf2 */

#define _SetEPCountRxReg(dwReg,wCount)  {\
    u16 wNBlocks;\
    if(wCount > 62){_BlocksOf32(dwReg,wCount,wNBlocks);}\
    else {_BlocksOf2(dwReg,wCount,wNBlocks);}\
  }/* _SetEPCountRxReg */



#define _SetEPRxDblBuf0Count(bEpNum,wCount) {\
    u32 *pdwReg = _pEPTxCount(bEpNum); \
    _SetEPCountRxReg(pdwReg, wCount);\
  }
/*******************************************************************************
* Macro Name     : SetEPTxCount / SetEPRxCount.
* Description    : sets counter for the tx/rx buffer.
* Input          : bEpNum: endpoint number.
*                  wCount: Counter value.
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPTxCount(bEpNum,wCount) (*_pEPTxCount(bEpNum) = wCount)
#define _SetEPRxCount(bEpNum,wCount) {\
    u32 *pdwReg = _pEPRxCount(bEpNum); \
    _SetEPCountRxReg(pdwReg, wCount);\
  }
/*******************************************************************************
* Macro Name     : GetEPTxCount / GetEPRxCount.
* Description    : gets counter of the tx buffer.
* Input          : bEpNum: endpoint number.
* Output         : None.
* Return         : Counter value.
*******************************************************************************/
#define _GetEPTxCount(bEpNum)((u16)(*_pEPTxCount(bEpNum)) & 0x3ff)
#define _GetEPRxCount(bEpNum)((u16)(*_pEPRxCount(bEpNum)) & 0x3ff)

/*******************************************************************************
* Macro Name     : SetEPDblBuf0Addr / SetEPDblBuf1Addr.
* Description    : Sets buffer 0/1 address in a double buffer endpoint.
* Input          : bEpNum: endpoint number.
*                : wBuf0Addr: buffer 0 address.
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPDblBuf0Addr(bEpNum,wBuf0Addr) {_SetEPTxAddr(bEpNum, wBuf0Addr);}
#define _SetEPDblBuf1Addr(bEpNum,wBuf1Addr) {_SetEPRxAddr(bEpNum, wBuf1Addr);}

/*******************************************************************************
* Macro Name     : SetEPDblBuffAddr.
* Description    : Sets addresses in a double buffer endpoint.
* Input          : bEpNum: endpoint number.
*                : wBuf0Addr: buffer 0 address.
*                : wBuf1Addr = buffer 1 address.
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPDblBuffAddr(bEpNum,wBuf0Addr,wBuf1Addr) { \
    _SetEPDblBuf0Addr(bEpNum, wBuf0Addr);\
    _SetEPDblBuf1Addr(bEpNum, wBuf1Addr);\
  } /* _SetEPDblBuffAddr */

/*******************************************************************************
* Macro Name     : GetEPDblBuf0Addr / GetEPDblBuf1Addr.
* Description    : Gets buffer 0/1 address of a double buffer endpoint.
* Input          : bEpNum: endpoint number.
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _GetEPDblBuf0Addr(bEpNum) (_GetEPTxAddr(bEpNum))
#define _GetEPDblBuf1Addr(bEpNum) (_GetEPRxAddr(bEpNum))

/*******************************************************************************
* Macro Name     : SetEPDblBuffCount / SetEPDblBuf0Count / SetEPDblBuf1Count.
* Description    : Gets buffer 0/1 address of a double buffer endpoint.
* Input          : bEpNum: endpoint number.
*                : bDir: endpoint dir  EP_DBUF_OUT = OUT 
*                                      EP_DBUF_IN  = IN 
*                : wCount: Counter value    
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPDblBuf0Count(bEpNum, bDir, wCount)  { \
    if(bDir == EP_DBUF_OUT)\
      /* OUT endpoint */ \
    {_SetEPRxDblBuf0Count(bEpNum,wCount);} \
    else if(bDir == EP_DBUF_IN)\
      /* IN endpoint */ \
      *_pEPTxCount(bEpNum) = (u32)wCount;  \
  } /* SetEPDblBuf0Count*/

#define _SetEPDblBuf1Count(bEpNum, bDir, wCount)  { \
    if(bDir == EP_DBUF_OUT)\
      /* OUT endpoint */ \
    {_SetEPRxCount(bEpNum,wCount);}\
    else if(bDir == EP_DBUF_IN)\
      /* IN endpoint */\
      *_pEPRxCount(bEpNum) = (u32)wCount; \
  } /* SetEPDblBuf1Count */

#define _SetEPDblBuffCount(bEpNum, bDir, wCount) {\
    _SetEPDblBuf0Count(bEpNum, bDir, wCount); \
    _SetEPDblBuf1Count(bEpNum, bDir, wCount); \
  } /* _SetEPDblBuffCount  */

/*******************************************************************************
* Macro Name     : GetEPDblBuf0Count / GetEPDblBuf1Count.
* Description    : Gets buffer 0/1 rx/tx counter for double buffering.
* Input          : bEpNum: endpoint number.
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _GetEPDblBuf0Count(bEpNum) (_GetEPTxCount(bEpNum))
#define _GetEPDblBuf1Count(bEpNum) (_GetEPRxCount(bEpNum))


/* External variables --------------------------------------------------------*/
extern volatile u16 wIstr;  /* ISTR register last read value */

/* Exported functions ------------------------------------------------------- */
void SetCNTR(u16 /*wRegValue*/);
void SetISTR(u16 /*wRegValue*/);
void SetDADDR(u16 /*wRegValue*/);
void SetBTABLE(u16 /*wRegValue*/);
void SetBTABLE(u16 /*wRegValue*/);
u16 GetCNTR(void);
u16 GetISTR(void);
u16 GetFNR(void);
u16 GetDADDR(void);
u16 GetBTABLE(void);
//**************端点寄存器函数
//a) 设置/获取端点值
void SetENDPOINT(u8 /*bEpNum*/, u16 /*wRegValue*/);	//设置端点值
u16 GetENDPOINT(u8 /*bEpNum*/);											//获取端点值
//b) 端点类型字段
void SetEPType(u8 /*bEpNum*/, u16 /*wType*/);				//设置端点类型
u16 GetEPType(u8 /*bEpNum*/);												//获取端点类型
//c) 端点状态字段
void SetEPTxStatus(u8 /*bEpNum*/, u16 /*wState*/);	//设置发送端点状态
void SetEPRxStatus(u8 /*bEpNum*/, u16 /*wState*/);	//设置接收端点状态
void SetDouBleBuffEPStall(u8 /*bEpNum*/, u8 bDir);	//c) 双缓存状态字段
u16 GetEPTxStatus(u8 /*bEpNum*/);										//获取发送端点状态
u16 GetEPRxStatus(u8 /*bEpNum*/);										//获取接收端点状态
void SetEPTxValid(u8 /*bEpNum*/);
void SetEPRxValid(u8 /*bEpNum*/);
u16 GetTxStallStatus(u8 /*bEpNum*/);
u16 GetRxStallStatus(u8 /*bEpNum*/);
//d) 端点种类字段
void SetEP_KIND(u8 /*bEpNum*/);
void ClearEP_KIND(u8 /*bEpNum*/);
void Set_Status_Out(u8 /*bEpNum*/);
void Clear_Status_Out(u8 /*bEpNum*/);
//**************双缓存端点函数
void SetEPDoubleBuff(u8 /*bEpNum*/);
void ClearEPDoubleBuff(u8 /*bEpNum*/);
//e) 正确传输 Rx/Tx 字段
void ClearEP_CTR_RX(u8 /*bEpNum*/);				//清除正确接收标志
void ClearEP_CTR_TX(u8 /*bEpNum*/);				//清除正确发送标志
//f) 数据翻转 Rx/Tx 字段
void ToggleDTOG_RX(u8 /*bEpNum*/);				//RX数据翻转
void ToggleDTOG_TX(u8 /*bEpNum*/);				//TX数据翻转
void ClearDTOG_RX(u8 /*bEpNum*/);
void ClearDTOG_TX(u8 /*bEpNum*/);
//g) 地址字段
void SetEPAddress(u8 /*bEpNum*/, u8 /*bAddr*/);		//设置端点地址
u8 GetEPAddress(u8 /*bEpNum*/);

//**************缓冲描述符函数
//a) Tx/Rx 缓冲地址字段
void SetEPTxAddr(u8 /*bEpNum*/, u16 /*wAddr*/);
void SetEPRxAddr(u8 /*bEpNum*/, u16 /*wAddr*/);
u16 GetEPTxAddr(u8 /*bEpNum*/);
u16 GetEPRxAddr(u8 /*bEpNum*/);
//b) Tx/Rx 缓冲计数器字段
void SetEPCountRxReg(u32 * /*pdwReg*/, u16 /*wCount*/);
void SetEPTxCount(u8 /*bEpNum*/, u16 /*wCount*/);
void SetEPRxCount(u8 /*bEpNum*/, u16 /*wCount*/);
u16 GetEPTxCount(u8 /*bEpNum*/);
u16 GetEPRxCount(u8 /*bEpNum*/);
//**************双缓存端点函数
//a) 双缓存地址
void SetEPDblBuf0Addr(u8 /*bEpNum*/, u16 /*wBuf0Addr*/);
void SetEPDblBuf1Addr(u8 /*bEpNum*/, u16 /*wBuf1Addr*/);
void SetEPDblBuffAddr(u8 /*bEpNum*/, u16 /*wBuf0Addr*/, u16 /*wBuf1Addr*/);
u16 GetEPDblBuf0Addr(u8 /*bEpNum*/);
u16 GetEPDblBuf1Addr(u8 /*bEpNum*/);
//b) 双缓存计数器
void SetEPDblBuffCount(u8 /*bEpNum*/, u8 /*bDir*/, u16 /*wCount*/);
void SetEPDblBuf0Count(u8 /*bEpNum*/, u8 /*bDir*/, u16 /*wCount*/);
void SetEPDblBuf1Count(u8 /*bEpNum*/, u8 /*bDir*/, u16 /*wCount*/);
u16 GetEPDblBuf0Count(u8 /*bEpNum*/);
u16 GetEPDblBuf1Count(u8 /*bEpNum*/);
EP_DBUF_DIR GetEPDblBufDir(u8 /*bEpNum*/);
void FreeUserBuffer(u8 bEpNum/*bEpNum*/, u8 bDir);
u16 ToWord(u8, u8);
u16 ByteSwap(u16);

#endif /* __USB_REGS_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
