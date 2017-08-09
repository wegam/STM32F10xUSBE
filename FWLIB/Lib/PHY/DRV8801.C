
#include "DRV8801.H"			//ֱ���������IC
#include "STM32_GPIO.H"
//#include "STM32F10x_BitBand.H"

/************************************************************************************************************************************
PHASE	|	ENABLE	|	MODE1	|	MODE2	|	nSLEEP	|	OUT+	|	OUT-	|	״̬
	1		|		1			| 	X		|		X		|		1			|		H		|		L		|	Forward
	0		|		1			|		X		|		X		|		1			|		L		|		H		|	Reverse
	X		|		0			|		1		|		0		|		1			|		L		|		L		|	Brake (slow decay)
	X		|		0			|		1		|		1		|		1			|		H		|		H		|	Brake (slow decay)
	1		|		0			|		0		|		X		|		1			|		L		|		H		|	Fast-decay synchronous rectification(2)
	0		|		0			|		0		|		X		|		1			|		H		|		L		|	Fast-decay synchronous rectification(2)
	X		|		X			|		X		|		X		|		0			|		Z		|		Z		|	Sleep mode

(1) X = Don��t care, Z = high impedance
(2) To prevent reversal of current during fast-decay synchronous rectification, outputs go to the high-impedance state as the current
approaches 0 A.
*************************************************************************************************************************************/

/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void DRV8801_Delay(u16 time)
{
	while(time--);
}
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void DRV8801_PinConf(DRV8801_Pindef *Pinfo)
{
	GPIO_Configuration_INA		(Pinfo->DRV8801_VPORTI_PORT,	Pinfo->DRV8801_VPORTI_Pin);			//��GPIO��Ӧ�ܽ�����Ϊģ������ģʽ----V20170605
	GPIO_Configuration_OPP50	(Pinfo->DRV8801_EN_PORT,			Pinfo->DRV8801_EN_Pin);					//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->DRV8801_SLEEP_PORT,		Pinfo->DRV8801_SLEEP_Pin);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->DRV8801_PHASE_PORT,		Pinfo->DRV8801_PHASE_Pin);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->DRV8801_MODE1_PORT,		Pinfo->DRV8801_MODE1_Pin);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->DRV8801_MODE2_PORT,		Pinfo->DRV8801_MODE2_Pin);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->DRV8801_FAULT_PORT,		Pinfo->DRV8801_FAULT_Pin);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	
	GPIO_ResetBits(Pinfo->DRV8801_EN_PORT, 		Pinfo->DRV8801_EN_Pin);
	GPIO_ResetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);
}
/*******************************************************************************
*������			:	DRV8801_CW
*��������		:	˳ʱ��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void DRV8801_CW(DRV8801_Pindef *Pinfo)
{
	//˳ʱ�룺PHASE=1;ENABLE=1;nSLEEP=1;MODE1=X;MODE2=X;
	GPIO_ResetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);			//�ر�˯��ģʽ
	
	GPIO_SetBits(Pinfo->DRV8801_PHASE_PORT, Pinfo->DRV8801_PHASE_Pin);		//��ת--˳ʱ��	
	GPIO_SetBits(Pinfo->DRV8801_EN_PORT, Pinfo->DRV8801_EN_Pin);					//ʹ��
	GPIO_SetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);		//�ر�˯��ģʽ
	
	
//	GPIO_SetBits(Pinfo->DRV8801_MODE1_PORT, Pinfo->DRV8801_MODE1_Pin);		//��Ч
//	GPIO_SetBits(Pinfo->DRV8801_MODE2_PORT, Pinfo->DRV8801_MODE2_Pin);		//��Ч
}
/*******************************************************************************
*������			:	DRV8801_CW
*��������		:	˳ʱ��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void DRV8801_CCW(DRV8801_Pindef *Pinfo)
{
	//��ʱ�룺PHASE=0;ENABLE=1;nSLEEP=1;MODE1=X;MODE2=X;
	GPIO_ResetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);			//�ر�˯��ģʽ
	
	GPIO_ResetBits(Pinfo->DRV8801_PHASE_PORT, Pinfo->DRV8801_PHASE_Pin);	//��ת--��ʱ��
	GPIO_SetBits(Pinfo->DRV8801_EN_PORT, Pinfo->DRV8801_EN_Pin);					//ʹ��	
	GPIO_SetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);		//�ر�˯��ģʽ
	
//	GPIO_SetBits(Pinfo->DRV8801_MODE1_PORT, Pinfo->DRV8801_MODE1_Pin);		//��Ч
//	GPIO_SetBits(Pinfo->DRV8801_MODE2_PORT, Pinfo->DRV8801_MODE2_Pin);		//��Ч
}
/*******************************************************************************
*������			:	DRV8801_CW_Brake
*��������		:	Brake (slow decay)
*����				: 
*����ֵ			:	��
*******************************************************************************/
void DRV8801_Brake_SN(DRV8801_Pindef *Pinfo)
{
	//����ͣ Nͨ���ŵ磺PHASE=X;ENABLE=0;nSLEEP=1;MODE1=1;MODE2=0;
	GPIO_ResetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);			//�ر�˯��ģʽ
	
	
	GPIO_ResetBits(Pinfo->DRV8801_PHASE_PORT, Pinfo->DRV8801_PHASE_Pin);	//��ת--��ʱ��
	GPIO_ResetBits(Pinfo->DRV8801_EN_PORT, Pinfo->DRV8801_EN_Pin);					//ʹ��	
	GPIO_SetBits(Pinfo->DRV8801_MODE1_PORT, Pinfo->DRV8801_MODE1_Pin);		//��Ч
	GPIO_ResetBits(Pinfo->DRV8801_MODE2_PORT, Pinfo->DRV8801_MODE2_Pin);		//��Ч
	
	GPIO_SetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);		//�ر�˯��ģʽ
}
/*******************************************************************************
*������			:	DRV8801_Brake_SP
*��������		:	Brake (high decay)
*����				: 
*����ֵ			:	��
*******************************************************************************/
void DRV8801_Brake_SP(DRV8801_Pindef *Pinfo)
{
	//����ͣ Pͨ���ŵ磺PHASE=X;ENABLE=0;nSLEEP=1;MODE1=1;MODE2=1;
	GPIO_ResetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);			//�ر�˯��ģʽ
	
	
	GPIO_ResetBits(Pinfo->DRV8801_PHASE_PORT, Pinfo->DRV8801_PHASE_Pin);	//��ת--��ʱ��
	GPIO_ResetBits(Pinfo->DRV8801_EN_PORT, Pinfo->DRV8801_EN_Pin);					//ʹ��	
	GPIO_SetBits(Pinfo->DRV8801_MODE1_PORT, Pinfo->DRV8801_MODE1_Pin);		//��Ч
	GPIO_SetBits(Pinfo->DRV8801_MODE2_PORT, Pinfo->DRV8801_MODE2_Pin);		//��Ч
	
	GPIO_SetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);		//�ر�˯��ģʽ
	
}
/*******************************************************************************
*������			:	DRV8801_Brake_FCW
*��������		:	˳ʱ����ټ�ͣ
*����				: 
*����ֵ			:	��
*******************************************************************************/
void DRV8801_Brake_FCW(DRV8801_Pindef *Pinfo)
{
	//����ͣ Nͨ���ŵ磺PHASE=1;ENABLE=0;nSLEEP=1;MODE1=0;MODE2=X;
	GPIO_ResetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);			//�ر�˯��ģʽ
	
	GPIO_SetBits(Pinfo->DRV8801_PHASE_PORT, Pinfo->DRV8801_PHASE_Pin);		//��ת--��ʱ��
	GPIO_ResetBits(Pinfo->DRV8801_EN_PORT, Pinfo->DRV8801_EN_Pin);				//ʹ��	
	
	GPIO_ResetBits(Pinfo->DRV8801_MODE1_PORT, Pinfo->DRV8801_MODE1_Pin);		//1
	GPIO_SetBits(Pinfo->DRV8801_MODE2_PORT, Pinfo->DRV8801_MODE2_Pin);		//��Ч
	
	GPIO_SetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);		//�ر�˯��ģʽ
}
/*******************************************************************************
*������			:	DRV8801_Brake_FCCW
*��������		:	��ʱ����ټ�ͣ
*����				: 
*����ֵ			:	��
*******************************************************************************/
void DRV8801_Brake_FCCW(DRV8801_Pindef *Pinfo)
{
	//����ͣ Pͨ���ŵ磺PHASE=0;ENABLE=0;nSLEEP=1;MODE1=0;MODE2=X;
	GPIO_ResetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);			//�ر�˯��ģʽ
	
	GPIO_ResetBits(Pinfo->DRV8801_PHASE_PORT, Pinfo->DRV8801_PHASE_Pin);		//��ת--��ʱ��
	GPIO_ResetBits(Pinfo->DRV8801_EN_PORT, Pinfo->DRV8801_EN_Pin);					//ʹ��	
	
	
	GPIO_ResetBits(Pinfo->DRV8801_MODE1_PORT, Pinfo->DRV8801_MODE1_Pin);		//0
	GPIO_SetBits(Pinfo->DRV8801_MODE2_PORT, Pinfo->DRV8801_MODE2_Pin);		//��Ч
	
	GPIO_SetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);			//�ر�˯��ģʽ
}
/*******************************************************************************
*������			:	DRV8801_Brake_FCCW
*��������		:	��ʱ����ټ�ͣ
*����				: 
*����ֵ			:	��
*******************************************************************************/
void DRV8801_Sleep(DRV8801_Pindef *Pinfo)
{
	//����ͣ Pͨ���ŵ磺PHASE=0;ENABLE=0;nSLEEP=1;MODE1=0;MODE2=X;
	GPIO_ResetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);			//�ر�˯��ģʽ
}
/*******************************************************************************
*������			:	DRV8801_Brake_FCCW
*��������		:	��ʱ����ټ�ͣ
*����				: 
*����ֵ			:	��
*******************************************************************************/
void DRV8801_WKup(DRV8801_Pindef *Pinfo)
{
	//����ͣ Pͨ���ŵ磺PHASE=0;ENABLE=0;nSLEEP=1;MODE1=0;MODE2=X;
	GPIO_SetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);			//�ر�˯��ģʽ
}
