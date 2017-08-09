
#include "DRV8801.H"			//直流电机驱动IC
#include "STM32_GPIO.H"
//#include "STM32F10x_BitBand.H"

/************************************************************************************************************************************
PHASE	|	ENABLE	|	MODE1	|	MODE2	|	nSLEEP	|	OUT+	|	OUT-	|	状态
	1		|		1			| 	X		|		X		|		1			|		H		|		L		|	Forward
	0		|		1			|		X		|		X		|		1			|		L		|		H		|	Reverse
	X		|		0			|		1		|		0		|		1			|		L		|		L		|	Brake (slow decay)
	X		|		0			|		1		|		1		|		1			|		H		|		H		|	Brake (slow decay)
	1		|		0			|		0		|		X		|		1			|		L		|		H		|	Fast-decay synchronous rectification(2)
	0		|		0			|		0		|		X		|		1			|		H		|		L		|	Fast-decay synchronous rectification(2)
	X		|		X			|		X		|		X		|		0			|		Z		|		Z		|	Sleep mode

(1) X = Don’t care, Z = high impedance
(2) To prevent reversal of current during fast-decay synchronous rectification, outputs go to the high-impedance state as the current
approaches 0 A.
*************************************************************************************************************************************/

/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void DRV8801_Delay(u16 time)
{
	while(time--);
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void DRV8801_PinConf(DRV8801_Pindef *Pinfo)
{
	GPIO_Configuration_INA		(Pinfo->DRV8801_VPORTI_PORT,	Pinfo->DRV8801_VPORTI_Pin);			//将GPIO相应管脚配置为模拟输入模式----V20170605
	GPIO_Configuration_OPP50	(Pinfo->DRV8801_EN_PORT,			Pinfo->DRV8801_EN_Pin);					//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->DRV8801_SLEEP_PORT,		Pinfo->DRV8801_SLEEP_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->DRV8801_PHASE_PORT,		Pinfo->DRV8801_PHASE_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->DRV8801_MODE1_PORT,		Pinfo->DRV8801_MODE1_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->DRV8801_MODE2_PORT,		Pinfo->DRV8801_MODE2_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->DRV8801_FAULT_PORT,		Pinfo->DRV8801_FAULT_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度2MHz----V20170605
	
	GPIO_ResetBits(Pinfo->DRV8801_EN_PORT, 		Pinfo->DRV8801_EN_Pin);
	GPIO_ResetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);
}
/*******************************************************************************
*函数名			:	DRV8801_CW
*功能描述		:	顺时针
*输入				: 
*返回值			:	无
*******************************************************************************/
void DRV8801_CW(DRV8801_Pindef *Pinfo)
{
	//顺时针：PHASE=1;ENABLE=1;nSLEEP=1;MODE1=X;MODE2=X;
	GPIO_ResetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);			//关闭睡眠模式
	
	GPIO_SetBits(Pinfo->DRV8801_PHASE_PORT, Pinfo->DRV8801_PHASE_Pin);		//正转--顺时针	
	GPIO_SetBits(Pinfo->DRV8801_EN_PORT, Pinfo->DRV8801_EN_Pin);					//使能
	GPIO_SetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);		//关闭睡眠模式
	
	
//	GPIO_SetBits(Pinfo->DRV8801_MODE1_PORT, Pinfo->DRV8801_MODE1_Pin);		//无效
//	GPIO_SetBits(Pinfo->DRV8801_MODE2_PORT, Pinfo->DRV8801_MODE2_Pin);		//无效
}
/*******************************************************************************
*函数名			:	DRV8801_CW
*功能描述		:	顺时针
*输入				: 
*返回值			:	无
*******************************************************************************/
void DRV8801_CCW(DRV8801_Pindef *Pinfo)
{
	//逆时针：PHASE=0;ENABLE=1;nSLEEP=1;MODE1=X;MODE2=X;
	GPIO_ResetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);			//关闭睡眠模式
	
	GPIO_ResetBits(Pinfo->DRV8801_PHASE_PORT, Pinfo->DRV8801_PHASE_Pin);	//反转--逆时针
	GPIO_SetBits(Pinfo->DRV8801_EN_PORT, Pinfo->DRV8801_EN_Pin);					//使能	
	GPIO_SetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);		//关闭睡眠模式
	
//	GPIO_SetBits(Pinfo->DRV8801_MODE1_PORT, Pinfo->DRV8801_MODE1_Pin);		//无效
//	GPIO_SetBits(Pinfo->DRV8801_MODE2_PORT, Pinfo->DRV8801_MODE2_Pin);		//无效
}
/*******************************************************************************
*函数名			:	DRV8801_CW_Brake
*功能描述		:	Brake (slow decay)
*输入				: 
*返回值			:	无
*******************************************************************************/
void DRV8801_Brake_SN(DRV8801_Pindef *Pinfo)
{
	//慢急停 N通道放电：PHASE=X;ENABLE=0;nSLEEP=1;MODE1=1;MODE2=0;
	GPIO_ResetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);			//关闭睡眠模式
	
	
	GPIO_ResetBits(Pinfo->DRV8801_PHASE_PORT, Pinfo->DRV8801_PHASE_Pin);	//反转--逆时针
	GPIO_ResetBits(Pinfo->DRV8801_EN_PORT, Pinfo->DRV8801_EN_Pin);					//使能	
	GPIO_SetBits(Pinfo->DRV8801_MODE1_PORT, Pinfo->DRV8801_MODE1_Pin);		//无效
	GPIO_ResetBits(Pinfo->DRV8801_MODE2_PORT, Pinfo->DRV8801_MODE2_Pin);		//无效
	
	GPIO_SetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);		//关闭睡眠模式
}
/*******************************************************************************
*函数名			:	DRV8801_Brake_SP
*功能描述		:	Brake (high decay)
*输入				: 
*返回值			:	无
*******************************************************************************/
void DRV8801_Brake_SP(DRV8801_Pindef *Pinfo)
{
	//慢急停 P通道放电：PHASE=X;ENABLE=0;nSLEEP=1;MODE1=1;MODE2=1;
	GPIO_ResetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);			//关闭睡眠模式
	
	
	GPIO_ResetBits(Pinfo->DRV8801_PHASE_PORT, Pinfo->DRV8801_PHASE_Pin);	//反转--逆时针
	GPIO_ResetBits(Pinfo->DRV8801_EN_PORT, Pinfo->DRV8801_EN_Pin);					//使能	
	GPIO_SetBits(Pinfo->DRV8801_MODE1_PORT, Pinfo->DRV8801_MODE1_Pin);		//无效
	GPIO_SetBits(Pinfo->DRV8801_MODE2_PORT, Pinfo->DRV8801_MODE2_Pin);		//无效
	
	GPIO_SetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);		//关闭睡眠模式
	
}
/*******************************************************************************
*函数名			:	DRV8801_Brake_FCW
*功能描述		:	顺时针快速急停
*输入				: 
*返回值			:	无
*******************************************************************************/
void DRV8801_Brake_FCW(DRV8801_Pindef *Pinfo)
{
	//慢急停 N通道放电：PHASE=1;ENABLE=0;nSLEEP=1;MODE1=0;MODE2=X;
	GPIO_ResetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);			//关闭睡眠模式
	
	GPIO_SetBits(Pinfo->DRV8801_PHASE_PORT, Pinfo->DRV8801_PHASE_Pin);		//反转--逆时针
	GPIO_ResetBits(Pinfo->DRV8801_EN_PORT, Pinfo->DRV8801_EN_Pin);				//使能	
	
	GPIO_ResetBits(Pinfo->DRV8801_MODE1_PORT, Pinfo->DRV8801_MODE1_Pin);		//1
	GPIO_SetBits(Pinfo->DRV8801_MODE2_PORT, Pinfo->DRV8801_MODE2_Pin);		//无效
	
	GPIO_SetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);		//关闭睡眠模式
}
/*******************************************************************************
*函数名			:	DRV8801_Brake_FCCW
*功能描述		:	逆时针快速急停
*输入				: 
*返回值			:	无
*******************************************************************************/
void DRV8801_Brake_FCCW(DRV8801_Pindef *Pinfo)
{
	//慢急停 P通道放电：PHASE=0;ENABLE=0;nSLEEP=1;MODE1=0;MODE2=X;
	GPIO_ResetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);			//关闭睡眠模式
	
	GPIO_ResetBits(Pinfo->DRV8801_PHASE_PORT, Pinfo->DRV8801_PHASE_Pin);		//反转--逆时针
	GPIO_ResetBits(Pinfo->DRV8801_EN_PORT, Pinfo->DRV8801_EN_Pin);					//使能	
	
	
	GPIO_ResetBits(Pinfo->DRV8801_MODE1_PORT, Pinfo->DRV8801_MODE1_Pin);		//0
	GPIO_SetBits(Pinfo->DRV8801_MODE2_PORT, Pinfo->DRV8801_MODE2_Pin);		//无效
	
	GPIO_SetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);			//关闭睡眠模式
}
/*******************************************************************************
*函数名			:	DRV8801_Brake_FCCW
*功能描述		:	逆时针快速急停
*输入				: 
*返回值			:	无
*******************************************************************************/
void DRV8801_Sleep(DRV8801_Pindef *Pinfo)
{
	//慢急停 P通道放电：PHASE=0;ENABLE=0;nSLEEP=1;MODE1=0;MODE2=X;
	GPIO_ResetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);			//关闭睡眠模式
}
/*******************************************************************************
*函数名			:	DRV8801_Brake_FCCW
*功能描述		:	逆时针快速急停
*输入				: 
*返回值			:	无
*******************************************************************************/
void DRV8801_WKup(DRV8801_Pindef *Pinfo)
{
	//慢急停 P通道放电：PHASE=0;ENABLE=0;nSLEEP=1;MODE1=0;MODE2=X;
	GPIO_SetBits(Pinfo->DRV8801_SLEEP_PORT, Pinfo->DRV8801_SLEEP_Pin);			//关闭睡眠模式
}

