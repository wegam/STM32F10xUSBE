/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : PC001V21.c
* Author             : WOW
* Version            : V2.0.1
* Date               : 06/26/2017
* Description        : PC001V21����ư�.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#ifdef PD014V15			//�м���ҩ���ư�

#include "PD014V15.H"

#include "DRV8801.H"
#include "A3987.H"

#include "STM32F10x_BitBand.H"
#include "STM32_GPIO.H"
#include "STM32_SYS.H"
#include "STM32_SYSTICK.H"
#include "STM32_WDG.H"
#include "STM32_PWM.H"
#include "STM32_USART.H"
#include "STM32_TOOLS.H"
#include "STM32_EXTI.H"

#include "string.h"				//�����ڴ��������ͷ�ļ�
#include "stm32f10x_dma.h"




u16	DelayTime=0;
u16	StepTime=0;

u16 Lock_Toggle_CNT=0;
PD014_INF_TypeDef	PD014_Conf;
RS485_TypeDef	PD014R485;

/*******************************************************************************
* ������		:	
* ��������	:	 
* ����		:	
* ���		:
* ���� 		:
*******************************************************************************/
void PD014V14_Configuration(void)
{
	SYS_Configuration();					//ϵͳ����---��ϵͳʱ�� STM32_SYS.H
	
	GPIO_DeInitAll();							//�����е�GPIO�ر�----V20170605
	
	PD014V14_PinSet();
	
	RS485_DMA_ConfigurationNR	(&PD014R485,19200,(u32*)PD014_Conf.PD014_DATA.RxdBuffe,RS485BufferSize);	//USART_DMA����--��ѯ��ʽ�������ж�,������Ĭ��Ϊ����״̬
	
	IWDG_Configuration(5000);			//�������Ź�����---������λms	
//	
	PWM_OUT(TIM2,PWM_OUTChannel1,1,500);						//PWM�趨-20161127�汾--SYS-LED
//	
	PD014V14_GetSwitchID();				//��ȡ���뿪�ص�ַ
	
	PD014V14_GetOnlieDevice();			//��ȡ���߷�ҩͷ
	
	SysTick_Configuration(1000);	//ϵͳ���ʱ������72MHz,��λΪuS--1ms
}
/*******************************************************************************
* ������		:	
* ��������	:	 
* ����		:	
* ���		:
* ���� 		:
*******************************************************************************/
void PD014V14_Server(void)
{	
	u8 RxNum=0;
	
//	P_Sens=1;			//���������翪��--��
	
	IWDG_Feed();								//�������Ź�ι��
	

	//���ݸ�ʽ:B0-SWITCHID��B1-CMD��B2~B9:���ݣ�B10:ǰ�������������У��
//	RxNum=RS485_ReadBufferIDLE(&PD014R485,(u32*)PD014_Conf.PD014_DATA.RevBuffe,(u32*)PD014_Conf.PD014_DATA.RxdBuffe);	//���ڿ���ģʽ�����ڽ��ջ���������������ݣ������ݿ�����RevBuffer,�����ؽ��յ������ݸ�����Ȼ�����½����ջ�������ַָ��RxdBuffer
//	if(RxNum)		//��������ַ��ͬ�������Process
//	{
//		unsigned char Bcc=BCC8(PD014_Conf.PD014_DATA.RevBuffe,10);		//���У��;
//		if(Bcc==PD014_Conf.PD014_DATA.RevBuffe[10])			//���У��ͨ��
//		{
//			PD014V14_Process();		//PD014V14���а��ڴ�����������
//		}
//		else
//		{
//			PD014_Conf.PD014_DATA.TxdBuffe[0]=PD014_NACK;
//			PD014_Conf.PD014_DATA.TxdBuffe[1]=PD014_BccError;
//			PD014_Conf.PD014_DATA.TxdBuffe[2]=PD014_NACK^PD014_BccError;
//			PD014V14_NACK();			//����У�鲻ͨ����NACKӦ������
//		}
//	}

//	PD014V14_SendM();			//��ҩ
	Lock_Toggle();			//˫�����������
}
/*******************************************************************************
* ������			:	PD014V14_Process
* ��������		:	PD014V14_Process 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void PD014V14_Process(void)		//PD014V14���а��ڴ�����������
{
	//�ж�������������;���ݸ�ʽ:B0-SWITCHID��B1-CMD��B2~B9:���ݣ�B10:ǰ�������������У��
	PD014_CMD_TypeDef CMD_Temp=PD014_CMD_IDLE;		//��ʱ�������
	CMD_Temp=(PD014_CMD_TypeDef)PD014_Conf.PD014_DATA.RevBuffe[1];		//��ȡ��������
	if(CMD_Temp==PD014_CMD_GetStatus)
	{
		PD014_Conf.PD014_DATA.TxdBuffe[0]=PD014_CMD_GetStatus;
		PD014_Conf.PD014_DATA.TxdBuffe[1]=PD014_Conf.PD014_DATA.SWITCHID;
		memcpy(&(PD014_Conf.PD014_DATA.TxdBuffe[2]),PD014_Conf.PD014_DATA.STATUS,8);
		PD014_Conf.PD014_DATA.TxdBuffe[10]=BCC8(PD014_Conf.PD014_DATA.TxdBuffe,10);		//���У��;
		RS485_DMASend(&PD014R485,(u32*)PD014_Conf.PD014_DATA.TxdBuffe,11);						//RS485-DMA���ͳ���
	}
	else if(memchr(PD014_Conf.PD014_DATA.STATUS,PD014_STA_IDLE,8)==NULL)		//�ǿ���״̬
	{
		PD014_Conf.PD014_DATA.TxdBuffe[0]=PD014_NACK;
		PD014_Conf.PD014_DATA.TxdBuffe[1]=PD014_BUSY;
		PD014_Conf.PD014_DATA.TxdBuffe[2]=PD014_NACK^PD014_BUSY;
		PD014V14_NACK();		//NACK����3�ֽڣ�NACK��ʶ���������ݡ�BCCУ����		
	}
	else if(CMD_Temp==PD014_CMD_SetWSD)		//��ҩ����---������Ҫ��ҩ������
	{
		unsigned char i=0;
		memcpy(&(PD014_Conf.PD014_DATA.WSD),&(PD014_Conf.PD014_DATA.RevBuffe[2]),8);
		for(i=0;i<8;i++)
		{
			if(PD014_Conf.PD014_DATA.WSD[i])
			{
				PD014_Conf.PD014_DATA.STATUS[i]=PD014_STA_SEND;
				PD014_Conf.PD014_DATA.RELAYTIME[i]=0;
				PD014_Conf.PD014_DATA.RELAYCOUNT[i]=0;
				PD014_Conf.PD014_DATA.SENSTIME[i]=0;
				PD014_Conf.PD014_DATA.NOISETIME[i]=0;
			}
			else
			{
				PD014_Conf.PD014_DATA.STATUS[i]=PD014_STA_IDLE;
			}
		}
	}		
}

/*******************************************************************************
* ������		:	
* ��������	:	 
* ����		:	
* ���		:
* ���� 		:
*******************************************************************************/
void PD014V14_PinSet(void)
{	
	//��ҩ���������
	GPIO_Configuration_OPP50	(GPIOA,	GPIO_Pin_7);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�50MHz----V20170605
	GPIO_Configuration_OPP50	(GPIOC,	GPIO_Pin_5);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�50MHz----V20170605
	GPIO_Configuration_OPP50	(GPIOB,	GPIO_Pin_1);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�50MHz----V20170605
	GPIO_Configuration_OPP50	(GPIOB,	GPIO_Pin_11);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�50MHz----V20170605
	GPIO_Configuration_OPP50	(GPIOB,	GPIO_Pin_13);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�50MHz----V20170605
	GPIO_Configuration_OPP50	(GPIOB,	GPIO_Pin_15);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�50MHz----V20170605
	GPIO_Configuration_OPP50	(GPIOC,	GPIO_Pin_7);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�50MHz----V20170605
	GPIO_Configuration_OPP50	(GPIOC,	GPIO_Pin_9);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�50MHz----V20170605
	
	//��ҩ���������翪��
	GPIO_Configuration_OPP50	(GPIOA,	GPIO_Pin_5);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�50MHz----V20170605
	
	//��ҩ������
	GPIO_Configuration_IPU(GPIOA,	GPIO_Pin_6);					//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	GPIO_Configuration_IPU(GPIOC,	GPIO_Pin_4);					//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	GPIO_Configuration_IPU(GPIOB,	GPIO_Pin_0);					//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	GPIO_Configuration_IPU(GPIOB,	GPIO_Pin_10);					//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	GPIO_Configuration_IPU(GPIOB,	GPIO_Pin_12);					//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	GPIO_Configuration_IPU(GPIOB,	GPIO_Pin_14);					//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	GPIO_Configuration_IPU(GPIOC,	GPIO_Pin_6);					//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	GPIO_Configuration_IPU(GPIOC,	GPIO_Pin_8);					//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605	

//	EXTI_Configuration(GPIOA,GPIO_Pin_4,EXTI_Mode_Interrupt);
	
	//���뿪��
	GPIO_Configuration_IPU(GPIOB,	GPIO_Pin_7);					//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	GPIO_Configuration_IPU(GPIOB,	GPIO_Pin_6);					//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	GPIO_Configuration_IPU(GPIOB,	GPIO_Pin_5);					//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	GPIO_Configuration_IPU(GPIOB,	GPIO_Pin_4);					//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	GPIO_Configuration_IPU(GPIOB,	GPIO_Pin_3);					//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	GPIO_Configuration_IPU(GPIOC,	GPIO_Pin_12);					//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	GPIO_Configuration_IPU(GPIOC,	GPIO_Pin_11);					//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	GPIO_Configuration_IPU(GPIOC,	GPIO_Pin_10);					//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	
	PD014R485.USARTx=USART2;
	PD014R485.RS485_CTL_PORT=GPIOA;
	PD014R485.RS485_CTL_Pin=GPIO_Pin_1;
	
	P_Sens=1;			//���������翪��--��
}
/*******************************************************************************
* ������			:	function
* ��������		:	��������˵�� 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void Lock_Toggle(void)
{
	Lock_Toggle_CNT++;
	if(Lock_Toggle_CNT>=2000)
	{
		Lock_Toggle_CNT=0;
		ct_pmos1=0;
		ct_pmos2=0;
		ct_pmos3=0;
		ct_pmos4=0;
		ct_pmos5=0;
		ct_pmos6=0;
		ct_pmos7=0;
		ct_pmos8=0;
	}
	else if(Lock_Toggle_CNT==100)
	{
		if(Sens_In1)
			ct_pmos1=1;
		if(Sens_In2)
			ct_pmos2=1;
		if(Sens_In3)
			ct_pmos3=1;
		if(Sens_In4)
			ct_pmos4=1;
		if(Sens_In5)
			ct_pmos5=1;
		if(Sens_In6)
			ct_pmos6=1;
		if(Sens_In7)
			ct_pmos7=1;
		if(Sens_In8)
			ct_pmos8=1;
		
	}
//	else if(Lock_Toggle_CNT==200)
//	{
//		ct_pmos1=0;
//		ct_pmos2=0;
//		ct_pmos3=0;
//		ct_pmos4=0;
//		ct_pmos5=0;
//		ct_pmos6=0;
//		ct_pmos7=0;
//		ct_pmos8=0;		
//	}
	else if(Lock_Toggle_CNT>=400)
	{
		ct_pmos1=0;
		ct_pmos2=0;
		ct_pmos3=0;
		ct_pmos4=0;
		ct_pmos5=0;
		ct_pmos6=0;
		ct_pmos7=0;
		ct_pmos8=0;
	}
}
/*******************************************************************************
* ������			:	PD014V14_GetSwitchID
* ��������		:	��ȡ���뿪�ص�ַ 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void PD014V14_GetSwitchID(void)				//��ȡ���뿪�ص�ַ
{
	u8 tempID=0;
	tempID=tempID<<1;
	tempID+=PD014_Switch8;		//S8-MSB
	tempID=tempID<<1;
	tempID+=PD014_Switch7;		//
	tempID=tempID<<1;
	tempID+=PD014_Switch6;		//
	tempID=tempID<<1;
	tempID+=PD014_Switch5;		//
	tempID=tempID<<1;
	tempID+=PD014_Switch4;		//
	tempID=tempID<<1;
	tempID+=PD014_Switch3;		//
	tempID=tempID<<1;
	tempID+=PD014_Switch2;		//
	tempID=tempID<<1;
	tempID+=PD014_Switch1;		//
	tempID=tempID^0xFF;
	
	
	if(PD014_Conf.PD014_DATA.SWITCHID&0x80)	//���λΪ1����CAN����
	{
		PD014_Conf.PD014_DATA.CANFLG=1;
	}
	if(PD014_Conf.PD014_DATA.SWITCHID&0x7F)	//���λΪ1����CAN����
	{
		PD014_Conf.MODE=NORMEL;					//�в��룬����ģʽ
	}
	else
	{
		PD014_Conf.MODE=TEST;						//�޲��룬����ģʽ
	}
	
	PD014_Conf.PD014_DATA.SWITCHID=tempID;
}
/*******************************************************************************
* ������			:	PD014V14_GetOnlieDevice
* ��������		:	��������˵�� 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void PD014V14_GetOnlieDevice(void)			//��ȡ���߷�ҩͷ
{
	u8 OnLD=0;		//DeviceOnlie
	if(Sens_In1)
		OnLD+=1;
	OnLD<<=1;
	if(Sens_In2)
		OnLD+=1;
	OnLD<<=1;
	if(Sens_In3)
		OnLD+=1;
	OnLD<<=1;
	if(Sens_In4)
		OnLD+=1;
	OnLD<<=1;
	if(Sens_In5)
		OnLD+=1;
	OnLD<<=1;
	if(Sens_In6)
		OnLD+=1;
	OnLD<<=1;
	if(Sens_In7)
		OnLD+=1;
	OnLD<<=1;
	if(Sens_In8)
		OnLD+=1;
	OnLD=OnLD^0xFF;
	PD014_Conf.PD014_DATA.DeviceOnlie=OnLD;
}

/*******************************************************************************
* ������			:	function
* ��������		:	��������˵�� 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void PD014V14_ACK(void)
{
	PD014_Conf.PD014_DATA.TxdBuffe[0]=PD014_ACK;
	PD014_Conf.PD014_DATA.TxdBuffe[1]=PD014_ACK^0xFF;
	RS485_DMASend(&PD014R485,(u32*)PD014_Conf.PD014_DATA.TxdBuffe,2);	//RS485-DMA���ͳ���
}
/*******************************************************************************
* ������			:	function
* ��������		:	��������˵�� 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void PD014V14_NACK(void)		//NACK����3�ֽڣ�NACK��ʶ���������ݡ�BCCУ����
{
//	PD014_Conf.PD014_DATA.TxdBuffe[0]=PD014_NACK;
//	PD014_Conf.PD014_DATA.TxdBuffe[1]=PD014_NACK^0xFF;
	RS485_DMASend(&PD014R485,(u32*)PD014_Conf.PD014_DATA.TxdBuffe,3);	//RS485-DMA���ͳ���
}
/*******************************************************************************
* ������			:	function
* ��������		:	��������˵�� 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void PD014V14_SendM(void)		//��ҩ
{
	unsigned char i=0;
	for(i=0;i<8;i++)
	{
		if(PD014_Conf.PD014_DATA.STATUS[i]==PD014_STA_SEND)
		{
			if(i==0)
			{
								
				if(PD014_Conf.PD014_DATA.RELAYTIME[i]++<10000)
				{
					ct_pmos1=1;
				}
				else if(PD014_Conf.PD014_DATA.RELAYTIME[i]++>=20000)
				{
					PD014_Conf.PD014_DATA.RELAYTIME[i]=0;
				}
				else
				{
					ct_pmos1=0;
				}
				if(Sens_In1)
				{
					PD014_Conf.PD014_DATA.SENSTIME[i]++;
				}
				else
				{
					if(PD014_Conf.PD014_DATA.SENSTIME[i]>=1000)
					{
						PD014_Conf.PD014_DATA.SED[i]+=1;
						if(PD014_Conf.PD014_DATA.SED[i]>=PD014_Conf.PD014_DATA.WSD[i])
						{
							PD014_Conf.PD014_DATA.STATUS[i]=PD014_STA_SENDED;
						}
					}
					PD014_Conf.PD014_DATA.SENSTIME[i]=0;
				}
			}
			else if(i==1)
			{
								
				if(PD014_Conf.PD014_DATA.RELAYTIME[i]++<10000)
				{
					ct_pmos2=1;
				}
				else if(PD014_Conf.PD014_DATA.RELAYTIME[i]++>=20000)
				{
					PD014_Conf.PD014_DATA.RELAYTIME[i]=0;
				}
				else
				{
					ct_pmos2=0;
				}
				if(Sens_In2)
				{
					PD014_Conf.PD014_DATA.SENSTIME[i]++;
				}
				else
				{
					if(PD014_Conf.PD014_DATA.SENSTIME[i]>=1000)
					{
						PD014_Conf.PD014_DATA.SED[i]+=1;
						if(PD014_Conf.PD014_DATA.SED[i]>=PD014_Conf.PD014_DATA.WSD[i])
						{
							PD014_Conf.PD014_DATA.STATUS[i]=PD014_STA_SENDED;
						}
					}
					PD014_Conf.PD014_DATA.SENSTIME[i]=0;
				}
			}
			else if(i==2)
			{
								
				if(PD014_Conf.PD014_DATA.RELAYTIME[i]++<10000)
				{
					ct_pmos3=1;
				}
				else if(PD014_Conf.PD014_DATA.RELAYTIME[i]++>=20000)
				{
					PD014_Conf.PD014_DATA.RELAYTIME[i]=0;
				}
				else
				{
					ct_pmos3=0;
				}
				if(Sens_In3)
				{
					PD014_Conf.PD014_DATA.SENSTIME[i]++;
				}
				else
				{
					if(PD014_Conf.PD014_DATA.SENSTIME[i]>=1000)
					{
						PD014_Conf.PD014_DATA.SED[i]+=1;
						if(PD014_Conf.PD014_DATA.SED[i]>=PD014_Conf.PD014_DATA.WSD[i])
						{
							PD014_Conf.PD014_DATA.STATUS[i]=PD014_STA_SENDED;
						}
					}
					PD014_Conf.PD014_DATA.SENSTIME[i]=0;
				}
			}
			else if(i==3)
			{
								
				if(PD014_Conf.PD014_DATA.RELAYTIME[i]++<10000)
				{
					ct_pmos4=1;
				}
				else if(PD014_Conf.PD014_DATA.RELAYTIME[i]++>=20000)
				{
					PD014_Conf.PD014_DATA.RELAYTIME[i]=0;
				}
				else
				{
					ct_pmos4=0;
				}
				if(Sens_In4)
				{
					PD014_Conf.PD014_DATA.SENSTIME[i]++;
				}
				else
				{
					if(PD014_Conf.PD014_DATA.SENSTIME[i]>=1000)
					{
						PD014_Conf.PD014_DATA.SED[i]+=1;
						if(PD014_Conf.PD014_DATA.SED[i]>=PD014_Conf.PD014_DATA.WSD[i])
						{
							PD014_Conf.PD014_DATA.STATUS[i]=PD014_STA_SENDED;
						}
					}
					PD014_Conf.PD014_DATA.SENSTIME[i]=0;
				}
			}
			else if(i==4)
			{
								
				if(PD014_Conf.PD014_DATA.RELAYTIME[i]++<10000)
				{
					ct_pmos5=1;
				}
				else if(PD014_Conf.PD014_DATA.RELAYTIME[i]++>=20000)
				{
					PD014_Conf.PD014_DATA.RELAYTIME[i]=0;
				}
				else
				{
					ct_pmos5=0;
				}
				if(Sens_In5)
				{
					PD014_Conf.PD014_DATA.SENSTIME[i]++;
				}
				else
				{
					if(PD014_Conf.PD014_DATA.SENSTIME[i]>=1000)
					{
						PD014_Conf.PD014_DATA.SED[i]+=1;
						if(PD014_Conf.PD014_DATA.SED[i]>=PD014_Conf.PD014_DATA.WSD[i])
						{
							PD014_Conf.PD014_DATA.STATUS[i]=PD014_STA_SENDED;
						}
					}
					PD014_Conf.PD014_DATA.SENSTIME[i]=0;
				}
			}
			else if(i==5)
			{
								
				if(PD014_Conf.PD014_DATA.RELAYTIME[i]++<10000)
				{
					ct_pmos6=1;
				}
				else if(PD014_Conf.PD014_DATA.RELAYTIME[i]++>=20000)
				{
					PD014_Conf.PD014_DATA.RELAYTIME[i]=0;
				}
				else
				{
					ct_pmos6=0;
				}
				if(Sens_In6)
				{
					PD014_Conf.PD014_DATA.SENSTIME[i]++;
				}
				else
				{
					if(PD014_Conf.PD014_DATA.SENSTIME[i]>=1000)
					{
						PD014_Conf.PD014_DATA.SED[i]+=1;
						if(PD014_Conf.PD014_DATA.SED[i]>=PD014_Conf.PD014_DATA.WSD[i])
						{
							PD014_Conf.PD014_DATA.STATUS[i]=PD014_STA_SENDED;
						}
					}
					PD014_Conf.PD014_DATA.SENSTIME[i]=0;
				}
			}
			else if(i==6)
			{
								
				if(PD014_Conf.PD014_DATA.RELAYTIME[i]++<10000)
				{
					ct_pmos7=1;
				}
				else if(PD014_Conf.PD014_DATA.RELAYTIME[i]++>=20000)
				{
					PD014_Conf.PD014_DATA.RELAYTIME[i]=0;
				}
				else
				{
					ct_pmos7=0;
				}
				if(Sens_In7)
				{
					PD014_Conf.PD014_DATA.SENSTIME[i]++;
				}
				else
				{
					if(PD014_Conf.PD014_DATA.SENSTIME[i]>=1000)
					{
						PD014_Conf.PD014_DATA.SED[i]+=1;
						if(PD014_Conf.PD014_DATA.SED[i]>=PD014_Conf.PD014_DATA.WSD[i])
						{
							PD014_Conf.PD014_DATA.STATUS[i]=PD014_STA_SENDED;
						}
					}
					PD014_Conf.PD014_DATA.SENSTIME[i]=0;
				}
			}
			else if(i==7)
			{
								
				if(PD014_Conf.PD014_DATA.RELAYTIME[i]++<10000)
				{
					ct_pmos8=1;
				}
				else if(PD014_Conf.PD014_DATA.RELAYTIME[i]++>=20000)
				{
					PD014_Conf.PD014_DATA.RELAYTIME[i]=0;
				}
				else
				{
					ct_pmos8=0;
				}
				if(Sens_In8)
				{
					PD014_Conf.PD014_DATA.SENSTIME[i]++;
				}
				else
				{
					if(PD014_Conf.PD014_DATA.SENSTIME[i]>=1000)
					{
						PD014_Conf.PD014_DATA.SED[i]+=1;
						if(PD014_Conf.PD014_DATA.SED[i]>=PD014_Conf.PD014_DATA.WSD[i])
						{
							PD014_Conf.PD014_DATA.STATUS[i]=PD014_STA_SENDED;
						}
					}
					PD014_Conf.PD014_DATA.SENSTIME[i]=0;
				}
			}
			//----------��������ʱ

		}
		else
		{
			if(i==1)
				ct_pmos1=0;
			else if(i==2)
				ct_pmos2=0;
			else if(i==3)
				ct_pmos3=0;
			else if(i==4)
				ct_pmos4=0;
			else if(i==5)
				ct_pmos5=0;
			else if(i==6)
				ct_pmos6=0;
			else if(i==7)
				ct_pmos7=0;
			else if(i==8)
				ct_pmos8=0;
		}
	}
}
/*******************************************************************************
* ������			:	PD014V14_ResetData
* ��������		:	��λ���� 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void PD014V14_ResetData(void)					//��λ����
{
	PD014_Conf.PD014_DATA.CANFLG=0;				//CAN����---CANFLG==1��ʹ��CAN--Ϊ���ϲ����ӵİ�
	PD014_Conf.PD014_DATA.SWITCHID=0;			//���뿪��
	PD014_Conf.PD014_DATA.DeviceOnlie=0;	//��ҩͷ��־---�洢�Ƿ���뷢ҩͷ���У���ǰλΪ1������Ϊ0	//1(�����Ϊ���λ)
	
	PD014_Conf.PD014_DATA.TimeCount=0;		//���д�����ʱ��
	PD014_Conf.PD014_DATA.TimeOut=0;			//֧�г�ʱ��ʱ��
	
	memset(PD014_Conf.PD014_DATA.WSD,0x00,8);		//����ҩ����
	memset(PD014_Conf.PD014_DATA.SED,0x00,8);		//�ѷ�ҩ����	
	
	memset(PD014_Conf.PD014_DATA.RELAYTIME,0x64,8);		//�̵�����Ҫ����ʱ��---Ĭ��100ms(1ms*100)
	memset(PD014_Conf.PD014_DATA.RELAYCOUNT,0x00,8);	//�̵����Ѿ�����ʱ��---�ﵽRELAYTIME���ͷ�
	
	memset(PD014_Conf.PD014_DATA.SENSTIME,0x00,8);		//��������Ӧʱ��----��NOISETIME����ʹ�ü��㴫������Ӧʱ�䣬��Ҫ���˸����ź�
	memset(PD014_Conf.PD014_DATA.NOISETIME,0x00,8);		//����������������ʱ��	
	
	memset(PD014_Conf.PD014_DATA.RxdBuffe,0x00,8);
	memset(PD014_Conf.PD014_DATA.RevBuffe,0x00,8);
	memset(PD014_Conf.PD014_DATA.TxdBuffe,0x00,8);
	memset(PD014_Conf.PD014_DATA.TevBuffe,0x00,8);
	
	PD014_Conf.PD014_DATA.Command=PD014_CMD_IDLE;			//����
	memset(PD014_Conf.PD014_DATA.STATUS,PD014_STA_IDLE,8);	//״̬	
}




#endif