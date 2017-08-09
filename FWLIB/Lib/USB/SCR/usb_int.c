/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_int.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Endpoint CTR (Low and High) interrupt's service routines
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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u16 SaveRState;
u16 SaveTState;

/* Extern variables ----------------------------------------------------------*/
extern void (*pEpInt_IN[7])(void);    /*  Handles IN  interrupts   */
extern void (*pEpInt_OUT[7])(void);   /*  Handles OUT interrupts   */

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : CTR_LP.
* Description    : 处理端点数据传输完成中断
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CTR_LP(void)			//被调用：usb_istr.c			//用于端点数据输入输入中断处
{
  u32 wEPVal = 0;
  /* stay in loop while pending ints */
  while (((wIstr = _GetISTR()) & ISTR_CTR) != 0)	//获取正确的传输
  {
    _SetISTR((u16)CLR_CTR); /* clear CTR flag */	//清除正确的传输标志 (Correct transfer)
    /* extract highest priority endpoint number */
    EPindex = (u8)(wIstr & ISTR_EP_ID);						//找到节点索引号//端点ID (Endpoint Identifier)////获取数据传输针对的端点号
    if (EPindex == 0)			//0号端点--控制传输
    {
      /* Decode and service control endpoint interrupt */
      /* calling related service routine */
      /* (Setup0_Process, In0_Process, Out0_Process) */

      /* save RX & TX status */
      /* and set both to NAK */
      SaveRState = _GetEPRxStatus(ENDP0);		//保存接收端点状态
      SaveTState = _GetEPTxStatus(ENDP0);		//保存发送端点状态
      _SetEPRxStatus(ENDP0, EP_RX_NAK);			//端点以NAK分组响应所有的发送请求。
      _SetEPTxStatus(ENDP0, EP_TX_NAK);			//端点以NAK分组响应所有的接收请求。


      /* DIR bit = origin of the interrupt */
			//DIR＝0:标志一个IN分组(数据从USB模块传输到PC主机)的传输完成;DIR＝1:标志一个OUT分组(数据从PC主机传输到USB模块)的传输完成
      if ((wIstr & ISTR_DIR) == 0)		//发送//传输方向 (Direction of transaction)
      {
        /* DIR = 0 */

        /* DIR = 0      => IN  int */
        /* DIR = 0 implies that (EP_CTR_TX = 1) always  */


        _ClearEP_CTR_TX(ENDP0);		//清除接收端点状态
        In0_Process();						//usb_core.c//端点0发送处理函数（主机接收）

           /* before terminate set Tx & Rx status */
          _SetEPRxStatus(ENDP0, SaveRState);		//还原状态
          _SetEPTxStatus(ENDP0, SaveTState);		//还原状态
          return;
      }
      else			//接收
      {
        /* DIR = 1 */

        /* DIR = 1 & CTR_RX       => SETUP or OUT int */
        /* DIR = 1 & (CTR_TX | CTR_RX) => 2 int pending */

        wEPVal = _GetENDPOINT(ENDP0);		//获取端点状态
        if ((wEPVal & EP_CTR_TX) != 0)	//还在发送状态
        {
          _ClearEP_CTR_TX(ENDP0);
          In0_Process();						//usb_core.c//端点0发送处理函数（主机接收）
          /* before terminate set Tx & Rx status */
          _SetEPRxStatus(ENDP0, SaveRState);
          _SetEPTxStatus(ENDP0, SaveTState);
          return;
        }
        else if ((wEPVal &EP_SETUP) != 0)		//建立包状态
        {
          _ClearEP_CTR_RX(ENDP0); /* SETUP bit kept frozen while CTR_RX = 1 */
          Setup0_Process();				//usb_core.c//建立包
          /* before terminate set Tx & Rx status */
          _SetEPRxStatus(ENDP0, SaveRState);
          _SetEPTxStatus(ENDP0, SaveTState);
          return;
        }

        else if ((wEPVal & EP_CTR_RX) != 0)		//接收
        {
          _ClearEP_CTR_RX(ENDP0);
          Out0_Process();			//usb_core.c//端点0输入处理函数（主机发送）
          /* before terminate set Tx & Rx status */
          _SetEPRxStatus(ENDP0, SaveRState);
          _SetEPTxStatus(ENDP0, SaveTState);
          return;
        }
      }
    }/* if(EPindex == 0) */
    else		//其它节点制作
    {
      /* Decode and service non control endpoints interrupt  */

      /* process related endpoint register */
      wEPVal = _GetENDPOINT(EPindex);
      if ((wEPVal & EP_CTR_RX) != 0)		//接收接收缓冲区有数据
      {
        /* clear int flag */
        _ClearEP_CTR_RX(EPindex);				//清楚读端点状态

        /* call OUT service function */
        (*pEpInt_OUT[EPindex-1])();			//处理接收数据

      } /* if((wEPVal & EP_CTR_RX) */

      if ((wEPVal & EP_CTR_TX) != 0)
      {
        /* clear int flag */
        _ClearEP_CTR_TX(EPindex);

        /* call IN service function */
        (*pEpInt_IN[EPindex-1])();
      } /* if((wEPVal & EP_CTR_TX) != 0) */

    }/* if(EPindex == 0) else */

  }/* while(...) */
}

/*******************************************************************************
* Function Name  : CTR_HP.
* Description    : High Priority Endpoint Correct Transfer interrupt's service 
*                  routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CTR_HP(void)
{
  u32 wEPVal = 0;

  while (((wIstr = _GetISTR()) & ISTR_CTR) != 0)
  {
    _SetISTR((u16)CLR_CTR); /* clear CTR flag */
    /* extract highest priority endpoint number */
    EPindex = (u8)(wIstr & ISTR_EP_ID);
    /* process related endpoint register */
    wEPVal = _GetENDPOINT(EPindex);
    if ((wEPVal & EP_CTR_RX) != 0)
    {
      /* clear int flag */
      _ClearEP_CTR_RX(EPindex);

      /* call OUT service function */
      (*pEpInt_OUT[EPindex-1])();

    } /* if((wEPVal & EP_CTR_RX) */
    else if ((wEPVal & EP_CTR_TX) != 0)
    {
      /* clear int flag */
      _ClearEP_CTR_TX(EPindex);

      /* call IN service function */
      (*pEpInt_IN[EPindex-1])();


    } /* if((wEPVal & EP_CTR_TX) != 0) */

  }/* while(...) */
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
