#ifdef Buzzer_TEST
#include "Buzzer_TEST.H"

#include "STM32_SYS.H"
#include "STM32_SYSTICK.H"

unsigned long TheSysClock=0;
// 定义乐曲：《化蝶》（梁祝） 
const tNote MyScore[ ] = 
{ 
	{L3, T/4}, 
	{L5, T/8+T/16}, 
	{L6, T/16}, 
	{M1, T/8+T/16}, 
	{M2, T/16}, 
	{L6, T/16}, 
	{M1, T/16},
	{L5, T/8}, 
	{M5, T/8+T/16}, 
	{H1, T/16}, 
	{M6, T/16}, 
	{M5, T/16}, 
	{M3, T/16}, 
	{M5, T/16}, 
	{M2, T/2}, 
	// 省略后续乐曲数据，请感兴趣的读者补充完整
	{ 0, 0} // 结束 
}; 

/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void Buzzer_TEST_Configuration(void)
{
	SYS_Configuration();				//系统配置
//	SysTick_Configuration(1000000);	//系统嘀嗒时钟配置72MHz,单位为uS
	
	PWM_OUT(TIM1,PWM_OUTChannel1,10,200);		//PWM设定
	PWM_OUT(TIM2,PWM_OUTChannel1,1,500);		//sys_led
}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void Buzzer_TEST_Server(void)
{
//	TIM_Cmd(TIM1, DISABLE); 									//使能TIM
	TheSysClock++;
	if(TheSysClock>=65536UL)
		TheSysClock=0;
	musicPlay(); 
}


// 蜂鸣器发出指定频率的声音
// usFreq是发声频率，取值 (系统时钟/65536)+1 ～ 20000，单位：Hz 
void buzzerSound(unsigned short usFreq) 
{ 
	unsigned long ulVal; 
	 
	if ((usFreq <= TheSysClock / 65536UL) || (usFreq > 20000)) 
	{ 
		buzzerQuiet();// 蜂鸣器静音
	} 
	else 
	{ 
//		GPIOPinTypeTimer(CCP3_PORT, CCP3_PIN); // 设置相关管脚为Timer功能 
		ulVal = TheSysClock / usFreq; 
//		TimerLoadSet(TIM1, TIMER_B, ulVal); // 设置TimerB初值 
//		TimerMatchSet(TIM1, TIMER_B, ulVal / 2); // 设置TimerB匹配值 
		PWM_OUT(TIM1,PWM_OUTChannel1,ulVal,ulVal/2);		//PWM设定
		TIM_Cmd(TIM1, ENABLE); // 使能TimerB计数 
	} 
}

// 蜂鸣器停止发声
void buzzerQuiet(void) 
{ 
	TIM_Cmd(TIM1, DISABLE); 									//使能TIM
//	TimerDisable(TIMER1_BASE, TIMER_B); // 禁止TimerB计数 
//	GPIOPinTypeOut(CCP3_PORT, CCP3_PIN); // 配置CCP3管脚为GPIO输出 
//	GPIOPinWrite(CCP3_PORT, CCP3_PIN, 0x00); // 使CCP3管脚输出低电平
}

//演奏乐曲
void musicPlay(void) 
{ 
	short i = 0; 
	for (;;) 
	{ 
		if (MyScore[i].mTime == 0) break; 
			buzzerSound(MyScore[i].mName); 
//		SysCtlDelay(MyScore[i].mTime * (TheSysClock/ 3000)); 
		i++; 
//		buzzerQuiet( ); 
//		SysCtlDelay(10 * (TheSysClock/ 3000)); 
	} 
}




#endif
