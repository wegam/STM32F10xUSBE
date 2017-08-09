#include "STM32F10x_ADS1230H20160808.H"


#define	ADS1230_CLKP GPIO_Pin_12	//时钟
#define	GPIO_ADSCLK GPIOA

#define ADS1230_DATP GPIO_Pin_11	//数据
#define GPIO_ADSDAT GPIOA

#define ADS1230_SPWNP GPIO_Pin_10	//低电平休眠
#define GPIO_ADSSPWN GPIOA

#define ADS1230_GAINP GPIO_Pin_8	//增益
#define GPIO_ADSGAIN GPIOA

#define ADS1230_SPEENP GPIO_Pin_9	//转换率
#define GPIO_ADSSPEEN GPIOA

#define ADS1230_GAIN	64		//低电平-增益64,高电平-增益128
#define ADS1230_SPEED	10		//低电平-10SPS,高电平-80SPS

u32 ADS1230_ADC=0;

/*******************************************************************************
*函数名		:ADS1230_Delay
*功能描述	:ADS1230延时函数
*输入			:j-延时时间					
*输出			:无
*返回值		:无
*例程			：
*******************************************************************************/
void ADS1230_Delay(u16 j)
{
	u8 i=36;
	for(;j>0;j--)
		for(i=36;i>0;i--);
}
/*******************************************************************************
*函数名		:ADS1230_Configuration
*功能描述	:ADS1230管脚初始化
*输入			:无
*输出			:无
*返回值		:无
*例程			：
*******************************************************************************/
void ADS1230_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef	EXTI_InitStructure;		
	NVIC_InitTypeDef	NVIC_InitStructure;	
	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);				//关闭AFIO时钟,为关闭JTAG功能
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //关闭JTAG功能
	
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//读数据中断
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource11);
	EXTI_InitStructure.EXTI_Line=EXTI_Line11;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;			//下降沿中断
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;		//中断线使能
	EXTI_Init(&EXTI_InitStructure);			
	EXTI_GenerateSWInterrupt(EXTI_Line11);		//使能中断
//	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQChannel;//中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//?????
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//?????
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//??
	NVIC_Init(&NVIC_InitStructure);
	EXTI_ClearITPendingBit(EXTI_Line11);
}

void ADSREAD_SET(u8 EN)
{
	EXTI->PR=1<<11;  //清除LINE11上的中断标志
	if(EN)
		EXTI->IMR|=1<<11;//不屏蔽line11上的中断
	else
		EXTI->IMR&=~(1<<11);//屏蔽line11的中断
}

/*******************************************************************************
*函数名		:ADS1230_INIT
*功能描述	:ADS1230管脚初始化
*输入			:无
*输出			:无
*返回值		:无
*例程			：
*******************************************************************************/
void ADS1230_INIT(void)
{
#ifdef	ADS1230_GAIN		//低电平-增益64,高电平-增益128
		GPIO_WriteBit(GPIO_ADSGAIN,ADS1230_GAINP,Bit_RESET);		//PGA==64
#else
		GPIO_WriteBit(GPIO_ADSGAIN,ADS1230_GAINP,Bit_SET);			//PGA==128
#endif
	
#ifdef	ADS1230_SPEED		//低电平-10SPS,高电平-80SPS
		GPIO_WriteBit(GPIO_ADSSPEEN,ADS1230_SPEENP,Bit_RESET);	//10SPS
#else
		GPIO_WriteBit(GPIO_ADSSPEEN,ADS1230_SPEENP,Bit_SET);		//80SPS
#endif
	
		GPIO_WriteBit(GPIO_ADSSPWN,ADS1230_SPWNP,Bit_SET);			//启动转换
}
/*******************************************************************************
*函数名		:ADS1230_INIT
*功能描述	:ADS1230管脚初始化
*输入			:无
*输出			:无
*返回值		:无
*例程			：
*******************************************************************************/
void ADS1230_START(void)
{
//	GPIO_ResetBits(GPIOB,GPIO_Pin_1);			//红灯			
//	GPIO_ResetBits(GPIOB,GPIO_Pin_10);		//蓝灯		
//	GPIO_ResetBits(GPIOB,GPIO_Pin_11);		//黄灯

//	GPIO_ResetBits(GPIOB,GPIO_Pin_3);		//红灯
//	GPIO_ResetBits(GPIOB,GPIO_Pin_5);		//蓝灯
//	GPIO_ResetBits(GPIOB,GPIO_Pin_4);		//黄灯
//	
////		ADSREAD_SET(1);	//使用中断
//	GPIO_SetBits(GPIOB,GPIO_Pin_10);		//蓝灯
//	GPIO_SetBits(GPIOB,GPIO_Pin_5);			//蓝灯
	
	GPIO_WriteBit(GPIO_ADSSPWN,ADS1230_SPWNP,Bit_SET);			//启动
	GPIO_WriteBit(GPIO_ADSCLK,ADS1230_CLKP,Bit_RESET);			//时钟信号拉低
}
/*******************************************************************************
*函数名		:ADS1230_INIT
*功能描述	:ADS1230管脚初始化
*输入			:无
*输出			:无
*返回值		:无
*例程			：
*******************************************************************************/
void ADS1230_STOP(void)
{
//	GPIO_ResetBits(GPIOB,GPIO_Pin_1);			//红灯			
//	GPIO_ResetBits(GPIOB,GPIO_Pin_10);		//蓝灯		
//	GPIO_ResetBits(GPIOB,GPIO_Pin_11);		//黄灯

//	GPIO_ResetBits(GPIOB,GPIO_Pin_3);		//红灯
//	GPIO_ResetBits(GPIOB,GPIO_Pin_5);		//蓝灯
//	GPIO_ResetBits(GPIOB,GPIO_Pin_4);		//黄灯

//	GPIO_SetBits(GPIOB,GPIO_Pin_1);	//红灯
//	GPIO_SetBits(GPIOB,GPIO_Pin_3);	//红灯	
	GPIO_WriteBit(GPIO_ADSSPWN,ADS1230_SPWNP,Bit_RESET);			//停止
}
/*******************************************************************************
*函数名		:ADS1230_INIT
*功能描述	:ADS1230管脚初始化
*输入			:无
*输出			:无
*返回值		:无
*例程			：
*******************************************************************************/
u32 ADS1230_READ(void)
{
	u8 i=0;
//	u32 ADS1230_ADC=0;
		ADS1230_ADC=0;
//	GPIO_ResetBits(GPIOB,GPIO_Pin_1);			//红灯			
//	GPIO_ResetBits(GPIOB,GPIO_Pin_10);		//蓝灯		
//	GPIO_ResetBits(GPIOB,GPIO_Pin_11);		//黄灯

//	GPIO_ResetBits(GPIOB,GPIO_Pin_3);		//红灯
//	GPIO_ResetBits(GPIOB,GPIO_Pin_5);		//蓝灯
//	GPIO_ResetBits(GPIOB,GPIO_Pin_4);		//黄灯
	
////	ADS1230_Delay(10);

//	GPIO_SetBits(GPIOB,GPIO_Pin_11);		//黄灯
//	GPIO_SetBits(GPIOB,GPIO_Pin_4);			//黄灯
	ADS1230_Delay(50);
	for(i=0;i<20;i++)
	{
		GPIO_SetBits(GPIO_ADSCLK,ADS1230_CLKP);			//时钟高
		GPIO_SetBits(GPIOB,GPIO_Pin_11);		//黄灯
		GPIO_SetBits(GPIOB,GPIO_Pin_4);			//黄灯
		ADS1230_Delay(5);
		if(GPIO_ReadInputDataBit(GPIO_ADSDAT,ADS1230_DATP))
		{
			ADS1230_ADC|=0x01;
			ADS1230_ADC<<=1;
		}
		else
		{
			ADS1230_ADC<<=1;
		}
		GPIO_ResetBits(GPIO_ADSCLK,ADS1230_CLKP);			//时钟低
		GPIO_ResetBits(GPIOB,GPIO_Pin_11);		//黄灯
		GPIO_ResetBits(GPIOB,GPIO_Pin_4);			//黄灯
		ADS1230_Delay(5);
	}
	
	GPIO_ResetBits(GPIO_ADSDAT,ADS1230_DATP);			//时钟低
//	ADS1230_ADC>>=2;
	return ADS1230_ADC;
	
//	GPIO_WriteBit(GPIO_ADSSPWN,ADS1230_SPWNP,Bit_RESET);			//停止
}
/*******************************************************************************
*函数名		:ADS1230_INIT
*功能描述	:ADS1230管脚初始化
*输入			:无
*输出			:无
*返回值		:无
*例程			：
*******************************************************************************/
void ADS1230(void)
{
	EXTI_ClearITPendingBit(EXTI_Line11);
//	ADSREAD_SET(0);
	ADS1230_Configuration();
	GPIO_Write(GPIOB,0XFFFF);			
	GPIO_Write(GPIOB,0XFF00);	
	
	GPIO_SetBits(GPIOB,GPIO_Pin_1);			//红灯			
	GPIO_SetBits(GPIOB,GPIO_Pin_10);		//蓝灯		
	GPIO_SetBits(GPIOB,GPIO_Pin_11);		//黄灯

	GPIO_SetBits(GPIOB,GPIO_Pin_3);		//红灯
	GPIO_SetBits(GPIOB,GPIO_Pin_5);		//蓝灯
	GPIO_SetBits(GPIOB,GPIO_Pin_4);		//黄灯
	
//	ADS1230_START();
//	ADS1230_READ();
//	GPIO_WriteBit(GPIO_ADSSPWN,ADS1230_SPWNP,Bit_RESET);			//启动
}
/*******************************************************************************
*函数名		:ADS1230_LED_DISPLAY
*功能描述	:ADS1230管脚初始化
*输入			:无
*输出			:无
*返回值		:无
*例程			：
*******************************************************************************/
void ADS1230_LED_DISPLAY(void)
{
	if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_10))
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);			//红灯			
		GPIO_ResetBits(GPIOB,GPIO_Pin_10);		//蓝灯		
		GPIO_ResetBits(GPIOB,GPIO_Pin_11);		//黄灯

		GPIO_SetBits(GPIOB,GPIO_Pin_3);		//红灯
		GPIO_SetBits(GPIOB,GPIO_Pin_5);		//蓝灯
		GPIO_SetBits(GPIOB,GPIO_Pin_4);		//黄灯
		
	}
	else
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_1);			//红灯			
		GPIO_SetBits(GPIOB,GPIO_Pin_10);		//蓝灯		
		GPIO_SetBits(GPIOB,GPIO_Pin_11);		//黄灯
		
		GPIO_ResetBits(GPIOB,GPIO_Pin_3);		//红灯
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);		//蓝灯
		GPIO_ResetBits(GPIOB,GPIO_Pin_4);		//黄灯
		
	}
}


