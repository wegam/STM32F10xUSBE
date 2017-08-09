
#include "A3987.H"				//�����������IC
#include "STM32_GPIO.H"
//#include "STM32F10x_BitBand.H"

/************************************************************************************************************************************
	MS1	|	MS2		|Microstep Resolution	|	Excitation Mode	|
	L		|		L		| 	Full step					|		2 phase				|
	H		|		L		|		Half step					|		1-2 phase			|	
	X		|		H		|		Quarter step			|		W1-2 phase		|	
	H		|		H		|		Sixteenth step		|		4W1-2 phase		|
	

(1) X = Don��t care, Z = high impedance
(2) To prevent reversal of current during fast-decay synchronous rectification, outputs go to the high-impedance state as the current
approaches 0 A.
*************************************************************************************************************************************/

u16	A3987_StepCnt=0;
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void A3987_Delay(u16 time)
{
	while(time--);
}
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void A3987_PinConf(A3987_Pindef *Pinfo)
{
	GPIO_Configuration_OPP50	(Pinfo->A3987_PUL_PORT,			Pinfo->A3987_PUL_Pin);					//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->A3987_DIR_PORT,			Pinfo->A3987_DIR_Pin);					//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->A3987_EN_PORT,			Pinfo->A3987_EN_Pin);					//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->A3987_MS1_PORT,			Pinfo->A3987_MS1_Pin);					//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->A3987_MS2_PORT,			Pinfo->A3987_MS2_Pin);					//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->A3987_RS_PORT,			Pinfo->A3987_RS_Pin);					//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	
	GPIO_SetBits(Pinfo->A3987_EN_PORT, Pinfo->A3987_EN_Pin);				//��ֹ�������
	GPIO_ResetBits(Pinfo->A3987_EN_PORT, Pinfo->A3987_EN_Pin);			//ʹ�ܵ������
	
	GPIO_ResetBits(Pinfo->A3987_MS1_PORT,			Pinfo->A3987_MS1_Pin);			//ʹ�ܵ������
	GPIO_ResetBits(Pinfo->A3987_MS2_PORT,			Pinfo->A3987_MS2_Pin);			//ʹ�ܵ������
	GPIO_SetBits(Pinfo->A3987_DIR_PORT,			Pinfo->A3987_DIR_Pin);			//��ת����
}
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void A3987_StepDriver(A3987_Pindef *Pinfo)
{
	A3987_StepCnt++;
	if(A3987_StepCnt>1)
	{
		A3987_StepCnt=0;
		GPIO_Toggle(Pinfo->A3987_PUL_PORT,			Pinfo->A3987_PUL_Pin);			//��ת����
	}
}
