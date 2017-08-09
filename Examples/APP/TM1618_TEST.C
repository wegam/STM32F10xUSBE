#ifdef TM1618

#include "TM1618_TEST.H"

#include "TM1618.H"

#include "STM32_PWM.H"
#include "STM32_GPIO.H"
#include "STM32_SYS.H"
#include "STM32_WDG.H"
#include "STM32_SYSTICK.H"
#include "STM32F10x_BitBand.H"

//#include "stdio.h"
//#include "stm32f10x_type.h"
//#include "STM32_SPI.H"
//#include "STM32_PWM.H"
//#include "STM32_ADC.H"
//#include "STM32_USART.H"
//#include "STM32F10x_BitBand.H"
//#include "STM32_SYSTICK.H"

//#include "STM32_SPI.H"

/******************************MX25L4006E命令定义*******************************/


//sbit stb=P3^4;		//片选，在上升或下降沿初始化串行接口，随后等待接收指令。
//									//STB为低后的第一个字节作为指令，当处理指令时，当前其它处
//									//理被终止。当STB 为高时，CLK 被忽略
////_________________________________________________________________________________________________
//sbit clk=P3^0;		//时钟上升沿输入串行数据。
////__________________________________________________________________________________________________
//sbit dio=P3^1;	  	//在时钟上升沿输入串行数据，从低位开始。

#define clk	PC8
#define	dio	PC9
#define	stb	PC10

#define	SEGEN		PC12
#define SEG1EN	PC10
#define	SEG2EN	PC11

#define	delayt	5

unsigned char seg4[]={0x3F,0X06,0X5B,0X4F,0x66,0X6D,0X7D,0x07,0x7f,0x6f,0x00,0xff};
unsigned char addr[]={0XC0,0XC2,0XC4,0XC6,0XC8,0XCA,0XCC,0XCE};
unsigned char s[8]={0};
unsigned char d[8]={0};
unsigned char flag=0;
		

TM1618_Pindef	TM1618_1,TM1618_2;

u32	dspdata=0;
u16 us=0;
u16	mm=0;
u8	ss=0;
u8	hh=0;
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void TM1618_Configuration(void)
{	
	SYS_Configuration();					//系统配置---打开系统时钟 STM32_SYS.H
	
	GPIO_DeInitAll();							//将所有的GPIO关闭----V20170605
	
	TM1618_PinSet();
	
	SysTick_Configuration(1000);	//系统嘀嗒时钟配置72MHz,单位为uS
	
	IWDG_Configuration(1000);			//独立看门狗配置---参数单位ms	
	
	PWM_OUT(TIM2,PWM_OUTChannel1,5000,20);						//PWM设定-20161127版本
	
//	TM1618_WriteByte(&TM1618_1,5);
	

}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void TM1618_Server(void)
{
	IWDG_Feed();								//独立看门狗喂狗
	TM1618_DIS();
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void TM1618_PinSet(void)
{
	TM1618_1.TM1618_CLK_PORT=GPIOC;
	TM1618_1.TM1618_CLK_Pin=GPIO_Pin_8;
	
	TM1618_1.TM1618_DIO_PORT=GPIOC;
	TM1618_1.TM1618_DIO_Pin=GPIO_Pin_9;	
	
	TM1618_1.TM1618_STB_PORT=GPIOC;
	TM1618_1.TM1618_STB_Pin=GPIO_Pin_10;
	
	TM1618_2.TM1618_CLK_PORT=GPIOC;
	TM1618_2.TM1618_CLK_Pin=GPIO_Pin_8;
	
	TM1618_2.TM1618_DIO_PORT=GPIOC;
	TM1618_2.TM1618_DIO_Pin=GPIO_Pin_9;	
	
	TM1618_2.TM1618_STB_PORT=GPIOC;
	TM1618_2.TM1618_STB_Pin=GPIO_Pin_11;
	
	TM1618_PinConf(&TM1618_1);
	TM1618_PinConf(&TM1618_2);
	
	GPIO_Configuration_OPP50	(GPIOC,	GPIO_Pin_12);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	
	PC12=1;
}

/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void TM1618_DIS(void)
{
	if(us++>=1000)
	{
		us=0;
		mm++;
		if(mm>=60)
		{
			mm=0;
			ss++;
		}
		if(ss>=60)
		{
			ss=0;
			hh++;
		}
		if(hh>=24)
		{
			hh=0;
		}
		
		dspdata=(u32)mm+(u32)ss*100+(u32)((ss%10)*10+(ss/10))*10000+(u32)((mm%10)*10+(mm/10))*1000000;
		TM1618_WriteDataN(&TM1618_1,dspdata/10000);
		TM1618_WriteDataN(&TM1618_2,dspdata%10000);	
	}
}


#endif


