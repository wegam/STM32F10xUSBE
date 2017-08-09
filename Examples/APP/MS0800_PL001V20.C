#ifdef MS0800_PL001V20


#include "MS0800_PL001V20.H"


/*##############################################################################
################################################################################
# ��Ŀ��		:	MS0800_PL001V20	
# ��������	:	����ҩ���������ʾ��
-------------------------------------------------------------------------------	
********************************����˵��***************************************
-------------------------------------------------------------------------------
�� ��ʾ��ֵ0~999
�� ��ʾ��ַ����6λ���뿪�ط�Ϊ3+3������λΪʮλ��ַλ������λΪ��λ��ַΪ����ַ������7*7
						ʮλΪ��ţ���λΪ���
�� ���뷽ʽ��SW1~SW3Ϊ����λ��SW1Ϊ��λ��SW4~SW6Ϊ����λ��SW6Ϊ���λ��
�� CANͨѶ˵����
	�����ʣ�	100K
	���е�ַ��0X3FF
	˽�е�ַ�����뿪�ص�ַ
-------------------------------------------------------------------------------
********************************��������***************************************
-------------------------------------------------------------------------------
1��������Ŀ
2���ϵ���߸�λ�������������ʾ��⣨��000~999��ʾ��
3����ȡ���뿪�ص�ַ���趨˽�е�ַ����ʾ��
4�����CAN����
5������DP����ʾ����ʱ��������ʱ��ͬ��
-------------------------------------------------------------------------------
********************************ָ��˵��***************************************
-------------------------------------------------------------------------------
1������ָ�			��Ԫ���·�
		CAN��ID��					0X3FF(StdId=0X3FF;)
		���ȣ�						1	(DLC=1;)
		��ʱ��ͬ����			0XAA;(Data[0]=0XAA;)
		��ȡIDָ�			0X01;(Data[0]=0X01;)
		�Լ�ָ�				0X02;(Data[0]=0X02;)
		Ϩ��ָ�				0X03;(Data[0]=0X02;)
		��ʾIDָ�			0X05;(Data[0]=0X02;)
-------------------------------------------------------------------------------		
2������ܰ��ϱ���ַ���趨������ܰ�ID������Ԫ��������ַ
		CAN��ID��		SW_ID�����뿪�ص�ַ��
		���ȣ�				1	(DLC=1;)
		��ʱ��ͬ����	0XAA;(Data[0]=0XAA;)
		
3������ܰ�������ݣ������ϼ���Ԫ�����ݣ���ʾ���߹ر�
		CAN��ID��		SW_ID�����뿪�ص�ַ��
		���ȣ�				3	(DLC=3;)
		������ʾ��ʶ��	0X05;(Data[0]=0X05;)
		��ʾ�������ݣ������ʾ999
									��λ8λ��Data[1]
									��λ8λ��Data[2]
									
4������ܰ�Ӧ�����������յ���ȷ�����ݺ�Ļ�ִ
		CAN��ID��		SW_ID�����뿪�ص�ַ��
		���ȣ�				1	(DLC=1;)
		��ʱ��ͬ����	0XA0;(Data[0]=0XA0;)
		
********bxCAN��Ҫ�ص�:********

-------------------------------------------------------------------------------


********************************************************************************

################################################################################
###############################################################################*/


//************CAN�����ʶ��
#define	GET_ID					0x01			//��ȡID���ϱ�IDָ��---Ȼ�������ʾ
#define	Self_Check			0x02			//�Լ�ָ��--��000��999ѭ����ʾ
#define	DSP_CLS					0x03			//Ϩ��ָ��/ȷ��ָ��
#define	DSP_ID					0x04			//��ʾIDָ��---�ϱ�ID��Ȼ��IDһֱ��˸
#define	DSP_DATA				0x05			//��ʾ��ֵָ��---0~999��ʾ
#define	Time_Updata			0x06			//ʱ��ͬ��/��ʾͬ��
#define	Updata_Addr			0xAA			//��Ԫ���ϱ���ַ��־

u16	SYSTime=0;
u16	DSPLAY_time=0;
u16	DSPLAY_NUM=0;
u8	STATUS_FLAG=0;			//״̬��־λ
u8 	RxData[8]={0};
u8 	SW_ID=0;
u16 txTemp=0;
CanRxMsg RxMessage;
/*******************************************************************************
* ������		:	
* ��������	:	 
* ����			:	
* ���			:
* ���� 			:
*******************************************************************************/
void MS0800_PL001V20_Configuration(void)
{
//	CAN_Configuration();	//CAN����
	
//	CD4511BM_Configuration();
	
	//********************���뿪������
//	GPIO_Configuration(GPIOB,GPIO_Pin_8,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//GPIO����
//	GPIO_Configuration(GPIOB,GPIO_Pin_9,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//GPIO����
//	GPIO_Configuration(GPIOB,GPIO_Pin_10,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//GPIO����
//	GPIO_Configuration(GPIOB,GPIO_Pin_11,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//GPIO����
//	GPIO_Configuration(GPIOB,GPIO_Pin_12,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//GPIO����
//	GPIO_Configuration(GPIOB,GPIO_Pin_13,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//GPIO����
	
	GPIO_Configuration(GPIOA,GPIO_Pin_8,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//���
	GPIO_Configuration(GPIOB,GPIO_Pin_11,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//��
	
	GPIO_Configuration(GPIOA,GPIO_Pin_15,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);			//GPIO����

	GPIO_SetBits(GPIOA,GPIO_Pin_15);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_15);
	//********************ϵͳ����ָʾ������
	PWM_Configuration(TIM2,7200,10000,50);

}
/*******************************************************************************
* ������		:	
* ��������	:	 
* ����		:	
* ���		:
* ���� 		:
*******************************************************************************/
void MS0800_PL001V20_Server(void)
{
	SYSTime++;

	if(SYSTime>=10000)
		SYSTime=0;
	
	if(SYSTime%500==0)
	{
		if((SW_ID!=Get_SW_ID())&&(STATUS_FLAG!=0))
		{
			SW_ID=Get_SW_ID();
			STATUS_FLAG=1;					//���ذ���ģʽ
			DSPLAY_time=0;
		}
		STATUS_FLAG=4;
	}
	
	MS0800_PL001V20_PROCESS(SYSTime);

}
/*******************************************************************************
*������		:	function
*��������	:	��������˵��
*����			: 
*���			:	��
*����ֵ		:	��
*����			:
*******************************************************************************/
void MS0800_PL001V20_PROCESS(u16 time)
{
	//1)***********************����/�ϵ��Լ�״̬
	if(STATUS_FLAG==0)						//�ϵ��������ʾ�Լ�
	{
		CD4511BM_DISPLAY(0,0);							//�����ʾ---��Ӱ	
		CD4511BM_DISPLAY(DSPLAY_NUM,time%3+31);
		if(time%200==0)
		{
			DSPLAY_NUM+=111;
		}
		if(DSPLAY_NUM>999)		//������ʾ�Լ�
		{			
			DSPLAY_NUM=0;
			STATUS_FLAG=1;				//��ʾ�Լ����
			SW_ID=Get_SW_ID();
		}
	}
	//2)***********************��ʾ��ǰ���뿪��״̬��ID������3λ��SW1~SW3��Ϊʮλ��������λ��SW~SW6)Ϊ��λ��
	else if(STATUS_FLAG==1)			//��ʾ��ǰ���뿪��״̬��ID������3λ��SW1~SW3��Ϊʮλ��������λ��SW~SW6)Ϊ��λ��
	{		
		if(time%1000==0)
		{
			DSPLAY_time++;
		}
		if(DSPLAY_time<10)			//10--10��������ֵ��������һ����
		{
			if(time%1000<500)
			{
				CD4511BM_DISPLAY(0,0);							//�����ʾ---��Ӱ		
				CD4511BM_DISPLAY(SW_ID,time%3+21);				
			}
			else
			{
				CD4511BM_DISPLAY(0,0);							//�����ʾ
				
			}
		}
		else
		{
			STATUS_FLAG=2;				//��ʾ�Լ����
			DSPLAY_time=0;
		}
	}
	//3)***********************����ID��ַע���˲�����Ϊ��0
	else if(STATUS_FLAG==2)								//�ϱ���ַ�����ݵ�ַ��ʼ���˲���
	{		
		PL001V20_CAN_TX();									//�ϱ���ַ
		CAN_FilterInitConfiguration(0,SW_ID,SW_ID);		//ע���ַ����������Ψһ��ַ
		CAN_FilterInitConfiguration(3,0X3FF,0X3FF);		//CAN�˲�������-----����ָ��ID----�����ն�ͨ��
		STATUS_FLAG=5;						//��ַ�ϱ����
	}
	//4)***********************���������ж�
	else if(STATUS_FLAG==3)
	{	

	}
	//5)***********************��ʾ���յ�������
	else if(STATUS_FLAG==4)
	{
		CD4511BM_DISPLAY(0,0);							//�����ʾ---��Ӱ	
		CD4511BM_DISPLAY(txTemp,time%3+1);
	}
	//6)***********************Ϩ��/������ʾ---С������˸
	else if(STATUS_FLAG==5)	
	{	
		if(time%500==0)
		{
			
			CD4511BM_DISPLAY_DP();	
		}
	}
	//6)***********************��ʾID--��˸
	else if(STATUS_FLAG==6)
	{		
		if(time%1000<500)
		{
			CD4511BM_DISPLAY(0,0);							//�����ʾ---��Ӱ		
			CD4511BM_DISPLAY(SW_ID,time%3+21);				
		}
		else
		{
			CD4511BM_DISPLAY(SW_ID,0);			
		}				
	}
	//6)***********************�Լ�---����ܴ�000ѭ����ʾ��999
	else if(STATUS_FLAG==7)			//��������һλС������˸
	{
		CD4511BM_DISPLAY(0,0);							//�����ʾ---��Ӱ		
		CD4511BM_DISPLAY(DSPLAY_NUM,time%3+31);
		if(time%200==0)
		{
			DSPLAY_NUM+=111;
		}
		if(DSPLAY_NUM>999)		//������ʾ�Լ�
		{			
			DSPLAY_NUM=0;
		}				
	}
}
/*******************************************************************************
* ������		:
* ��������	: 
* ����			: ��
* ���			: ��
* ����			: ��
*******************************************************************************/
void PL001V20_CAN_TX(void)
{
		CanTxMsg TxMessage;
		u8	TransmitMailbox = 0;
		u32	i;			
		TxMessage.StdId=SW_ID;			//�豸ID
		//	TxMessage.ExtId=0XFF;
		TxMessage.RTR=CAN_RTR_DATA;
		TxMessage.IDE=CAN_ID_STD;
		TxMessage.DLC=1;
		TxMessage.Data[0]=Updata_Addr;			//��������
		TransmitMailbox = CAN_Transmit(&TxMessage);
		i = 0;
		// ���ڼ����Ϣ�����Ƿ�����
		while((CAN_TransmitStatus(TransmitMailbox) != CANTXOK) && (i != 0xFF))
		{
			i++;
		}
		i = 0;
		// ��鷵�صĹҺŵ���Ϣ��Ŀ
		while((CAN_MessagePending(CAN_FIFO0) < 1) && (i != 0xFF))
		{
			i++;
		}

}
/*******************************************************************************
* ������		:
* ��������	: 
* ����			: ��
* ���			: ��
* ����			: ��
*******************************************************************************/
void PL001V20_CAN_ACK(void)
{
		CanTxMsg TxMessage;
		u8	TransmitMailbox = 0;
		u32	i;			
		TxMessage.StdId=Get_SW_ID();			//�豸ID
		//	TxMessage.ExtId=0XFF;
		TxMessage.RTR=CAN_RTR_DATA;
		TxMessage.IDE=CAN_ID_STD;
		TxMessage.DLC=1;
		TxMessage.Data[0]=Updata_Addr;			//��������
		TransmitMailbox = CAN_Transmit(&TxMessage);
		i = 0;
		// ���ڼ����Ϣ�����Ƿ�����
		while((CAN_TransmitStatus(TransmitMailbox) != CANTXOK) && (i != 0xFF))
		{
			i++;
		}
		i = 0;
		// ��鷵�صĹҺŵ���Ϣ��Ŀ
		while((CAN_MessagePending(CAN_FIFO0) < 1) && (i != 0xFF))
		{
			i++;
		}

}
/*******************************************************************************
* ������		:
* ��������	: 
* ����			: ��
* ���			: ��
* ����			: ��
*******************************************************************************/
void PL001V20_CAN_RX(void)
{
	if(SET == CAN_GetITStatus(CAN_IT_FF0))
	{
		CAN_ClearITPendingBit(CAN_IT_FF0);
	}
	else if(SET == CAN_GetITStatus(CAN_IT_FOV0))
	{
		CAN_ClearITPendingBit(CAN_IT_FOV0);
	}
	else if(SET == CAN_GetITStatus(CAN_IT_FMP0))
	{
		CAN_ClearITPendingBit(CAN_IT_FMP0);
	}
	else
	{
		CAN_Receive(CAN_FIFO0,&RxMessage);	
//----------------------------------------------------------------------------------------		
		if(((RxMessage.StdId==SW_ID)||(RxMessage.StdId==0X3FF))&&RxMessage.DLC==3)
		{	

			CD4511BM_DISPLAY(SW_ID,0);
			//1)************��ʾ����ָ��0x05
			if(RxMessage.Data[0]==DSP_DATA)
			{
//				txTemp=(RxMessage.Data[1]*100+RxMessage.Data[2]);
				txTemp=0;
				txTemp=RxMessage.Data[1];
				txTemp<<=8;
				txTemp+=RxMessage.Data[2];
				STATUS_FLAG=4;
			}
			//2)************��ȡIDָ��0x01
			else if(RxMessage.Data[0]==GET_ID)				//��ȡIDָ��
			{
				STATUS_FLAG=1;
			}
			//4.1)************�Լ�ָ��0x02---����ܴ�000ѭ����ʾ��999
			else if(RxMessage.Data[0]==Self_Check)		//�Լ�ָ��
			{
				STATUS_FLAG=7;
			}
			//4.1)************Ϩ��ָ��--С������˸0x03
			else if(RxMessage.Data[0]==DSP_CLS)				//Ϩ��ָ��
			{
				STATUS_FLAG=5;
			}
			//4.1)************��ʾIDָ��0x04
			else if(RxMessage.Data[0]==DSP_ID)							//��ʾIDָ��--����ID
			{
				SW_ID=Get_SW_ID();
				PL001V20_CAN_TX();														//�ϱ���ַ
				CAN_FilterInitConfiguration(0,SW_ID,SW_ID);		//ע���ַ����������Ψһ��ַ
				STATUS_FLAG=6;
			}
			//4.1)************��ʱ��ͬ��0x06----����ID��λ�趨��ʼʱ��ﵽ����ʾͬ��
			else if(RxMessage.Data[0]==Time_Updata)			//��ʱ��ͬ��
			{
				SYSTime=(SW_ID/10)*200;					//
			}			
			//4.1)************�����ָ��--Ϩ��
			else
			{
				STATUS_FLAG=5;
			
			}
		}
//----------------------------------------------------------------------------------------
	}
}

/*******************************************************************************
*������		:	function
*��������	:	��������˵��
*����			: 
*���			:	��
*����ֵ		:	��
*����			:
*******************************************************************************/
u8 Get_SW_ID(void)
{
	u8 temp1=0,temp2=0;
	//1)***************��ȡʮλ��ID��SW1,SW2,SW3��ǰ��λ��ʮλ��
	if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8))		//SW1���λ��ʮλ���λ
	{
		temp1|=1;
	}
	temp1<<=1;
	if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9))		//SW2
	{
		temp1|=1;
	}
	temp1<<=1;
	if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10))		//SW3ʮλ���λ
	{
		temp1|=1;
	}

	temp1=temp1*10;
	
	//2)***************��ȡ��λ��ID��SW4,SW5,SW6������λ����λ��
	if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11))		//SW4��λ���λ
	{
		temp2|=1;
	}
	temp2<<=1;
	if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12))		//SW5
	{
		temp2|=1;
	}
	temp2<<=1;
	if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13))		//SW6��λ���λ
	{
		temp2|=1;
	}
	
	//3)***************���ػ�ȡ��IDֵ
	return (temp1+temp2);

}






#endif