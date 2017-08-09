
#include "A3987.H"				//步进电机驱动IC
#include "STM32_GPIO.H"
//#include "STM32F10x_BitBand.H"

/************************************************************************************************************************************
	MS1	|	MS2		|Microstep Resolution	|	Excitation Mode	|
	L		|		L		| 	Full step					|		2 phase				|
	H		|		L		|		Half step					|		1-2 phase			|	
	X		|		H		|		Quarter step			|		W1-2 phase		|	
	H		|		H		|		Sixteenth step		|		4W1-2 phase		|
	

(1) X = Don’t care, Z = high impedance
(2) To prevent reversal of current during fast-decay synchronous rectification, outputs go to the high-impedance state as the current
approaches 0 A.
*************************************************************************************************************************************/

u16	A3987_StepCnt=0;
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void A3987_Delay(u16 time)
{
	while(time--);
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void A3987_PinConf(A3987_Pindef *Pinfo)
{
	GPIO_Configuration_OPP50	(Pinfo->A3987_PUL_PORT,			Pinfo->A3987_PUL_Pin);					//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->A3987_DIR_PORT,			Pinfo->A3987_DIR_Pin);					//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->A3987_EN_PORT,			Pinfo->A3987_EN_Pin);					//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->A3987_MS1_PORT,			Pinfo->A3987_MS1_Pin);					//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->A3987_MS2_PORT,			Pinfo->A3987_MS2_Pin);					//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->A3987_RS_PORT,			Pinfo->A3987_RS_Pin);					//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	
	GPIO_SetBits(Pinfo->A3987_EN_PORT, Pinfo->A3987_EN_Pin);				//禁止电机工作
	GPIO_ResetBits(Pinfo->A3987_EN_PORT, Pinfo->A3987_EN_Pin);			//使能电机工作
	
	GPIO_ResetBits(Pinfo->A3987_MS1_PORT,			Pinfo->A3987_MS1_Pin);			//使能电机工作
	GPIO_ResetBits(Pinfo->A3987_MS2_PORT,			Pinfo->A3987_MS2_Pin);			//使能电机工作
	GPIO_SetBits(Pinfo->A3987_DIR_PORT,			Pinfo->A3987_DIR_Pin);			//运转方向
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void A3987_StepDriver(A3987_Pindef *Pinfo)
{
	A3987_StepCnt++;
	if(A3987_StepCnt>1)
	{
		A3987_StepCnt=0;
		GPIO_Toggle(Pinfo->A3987_PUL_PORT,			Pinfo->A3987_PUL_Pin);			//运转方向
	}
}

