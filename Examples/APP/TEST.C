#ifdef TEST
#include "TEST.H"



/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void TEST_Configuration(void)
{
	SYS_Configuration();
//	SysTick_Configuration(10000);	//系统嘀嗒时钟配置72MHz,单位为uS
	
//	GPIO_Configuration0();
//	TIM_Configuration(TIM1,7200,3000);	//定时时间设定
//	PWM_Configuration(TIM2,7200,10000,51);
	PWM_OUT(TIM1,PWM_OUTChannel1,20000,50);		//PWM设定
	PWM_OUT(TIM2,PWM_OUTChannel1,20000,500);	//PWM设定
	PWM_OUT(TIM3,PWM_OUTChannel1,20000,30);		//PWM设定
	PWM_OUT(TIM4,PWM_OUTChannel1,20000,40);		//PWM设定
	
	PWM_OUT(TIM1,PWM_OUTChannel2,20000,50);		//PWM设定
	PWM_OUT(TIM2,PWM_OUTChannel2,20000,500);	//PWM设定

}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void TEST_Server(void)
{
	

}





#endif
