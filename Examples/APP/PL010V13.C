/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : PC001V21.c
* Author             : WOW
* Version            : V2.0.1
* Date               : 06/26/2017
* Description        : PC001V21层控制板.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#ifdef PL010V13				//智能药架称重LCD板

#include "PL010V13.H"

#include "R61509V.h"
#include "CS5530.H"

#include "GT32L32M0180.H"
#include "STM32F10x_BitBand.H"
#include "STM32_GPIO.H"
#include "STM32_SYS.H"
#include "STM32_SYSTICK.H"
#include "STM32_WDG.H"
#include "STM32_PWM.H"
#include "STM32_USART.H"

#include "string.h"				//串和内存操作函数头文件
#include "stm32f10x_dma.h"

u16	DelayTime=0x1000;
u16	LCDTime=0;
u16	DSPTime=0;
u32 testADC=0;
u32 bacADC=0;
u32 bacADC2=0;

u16 ADC_dotx=0;
u16 ADC_doty=0;
u16 ADC_dotx1=0;
u16 ADC_doty1=0;

u16 SumFed[8]={0};		//总共已发药数量
u16	SumFQ[8]={0};			//总共发药请求数量
u8	NumFW=0;		//待发药槽位
u8	Onlinede=0;		//待发药槽位


R61509V_Pindef R61509V_Pinfo;
CS5530_Pindef CS5530_Pinfo;
GT32L32_Info_TypeDef 	GT32L32_Info;
u32 CS5530_ADC_Value=0xFFFFFFFF;
t_Point point;
u8 zimo[720]="R61509V_DrawRectangle(11,11,229,389,0X07FF)";

RS485_TypeDef  RS485_Conf;

u8 RxdBuffe[256]={0};
u8 RevBuffe[256]={0};
u16 RxNum=0;
char	Char_Buffer[256]={0xFF};		//记录format内码
//t_LcdCfg **pLcdpara;

/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PL010V13_Configuration(void)
{
	SYS_Configuration();					//系统配置---打开系统时钟 STM32_SYS.H
	
	GPIO_DeInitAll();							//将所有的GPIO关闭----V20170605
	
	PL010V13_PinSet();
	
	RS485_DMA_ConfigurationNR	(&RS485_Conf,115200,(u32*)RxdBuffe,256);	//USART_DMA配置--查询方式，不开中断,配置完默认为接收状态
	
	SysTick_Configuration(1000);	//系统嘀嗒时钟配置72MHz,单位为uS
	
//	IWDG_Configuration(1000);			//独立看门狗配置---参数单位ms	
	
//	PWM_OUT(TIM2,PWM_OUTChannel1,1,200);		//PWM设定-20161127版本
}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PL010V13_Server(void)
{	
	IWDG_Feed();								//独立看门狗喂狗
	
	DelayTime++;
	
	LCDTime++;
	
//	if(LCDTime==500)
//	{
//		PL010V13_PrintfString(0		,134+50+16	,16	,"发药层控制器通信异常！！！！");				//错误状态
//	}
//	else if(LCDTime==1000)
//	{
//		PL010V13_PrintfString(0		,134+50+16	,16	,"XXXXXXXXXXXXXXXXXXXXXXXXXX");				//错误状态
//	}	
		
	DSPTime++;
	if(DSPTime>500)
	{
		DSPTime=0;
		LCD_WXS();		//位显示
		LCD_DDSP();		//显示总共请求数量和已发数量
	}	
	LCD_WS();		//位闪烁
	
	if(LCDTime>=1000)
	{			
		LCDTime=0;		
	}	
	
	RxNum=RS485_ReadBufferIDLE(&RS485_Conf,(u32*)RevBuffe,(u32*)RxdBuffe);	//串口空闲模式读串口接收缓冲区，如果有数据，将数据拷贝到RevBuffer,并返回接收到的数据个数，然后重新将接收缓冲区地址指向RxdBuffer
	if(RxNum==4&&RevBuffe[0]==0x00&&RevBuffe[1]==0xFF)		//RS485接收到数据
	{
		NumFW=RevBuffe[2];
		PL010V13_PrintfString(96		,0	,16	,"%2d",RevBuffe[2]);				//待发药槽位，后边的省略号就是可变参数
		PL010V13_PrintfString(96		,20	,16	,"%2d",RevBuffe[3]);				//待发药数量，后边的省略号就是可变参数
//		PL010V13_PrintfString(192		,68	,16	,"%2d",RevBuffe[1]);				//已发药数量，后边的省略号就是可变参数
	}
	else if(RxNum==3&&RevBuffe[0]==0x02)		//RS485接收到数据
	{
		SumFQ[RevBuffe[1]-1]+=RevBuffe[2];
		NumFW=RevBuffe[1];
		PL010V13_PrintfString(96		,0	,16	,"%2d",RevBuffe[1]);				//待发药槽位，后边的省略号就是可变参数
		PL010V13_PrintfString(96		,20	,16	,"%2d",RevBuffe[2]);				//待发药数量，后边的省略号就是可变参数
		PL010V13_PrintfString(96		,40	,16	,"%2d",0);									//已发药数量，后边的省略号就是可变参数
		
//		PL010V13_PrintfString(0		,60	,16	,"总共请求数量：%4d",SumFQ);				//总共发药请求数量
		PL010V13_PrintfString(0		,120	,16	,"正在发药！！！！！！！！！！");				//错误状态
	}
	else if(RxNum==4&&RevBuffe[0]==0x82)		//RS485接收到数据
	{
		SumFed[RevBuffe[1]-1]+=RevBuffe[2];
		PL010V13_PrintfString(96		,0	,16	,"%2d",RevBuffe[1]);				//待发药槽位，后边的省略号就是可变参数
		PL010V13_PrintfString(96		,20	,16	,"%2d",0);									//待发药数量，后边的省略号就是可变参数
		PL010V13_PrintfString(96		,40	,16	,"%2d",RevBuffe[2]);				//已发药数量，后边的省略号就是可变参数		
		
//		PL010V13_PrintfString(112		,80	,16	,"%4d",SumFed);					//总共已发药数量
		
	}
	else if(RxNum==6&&RevBuffe[0]==0x81)	//槽位信息0x01--获取,0x81--上报
	{
		Onlinede=RevBuffe[4];
	}
	else if(RxNum==1&&RevBuffe[0]==0x01)	//槽位信息0x01--获取,0x81--上报
	{
		PL010V13_PrintfString(0		,120	,16		,"获取槽位信息！！！");				//错误状态
		NumFW=0;
		Onlinede=0;
		memset(SumFed,0x00,8);
		memset(SumFQ,0x00,8);

		PL010V13_PrintfString(96		,0	,16	,"%2d",0);				//待发药槽位，后边的省略号就是可变参数
		PL010V13_PrintfString(96		,20	,16	,"%2d",0);				//待发药数量，后边的省略号就是可变参数
	}
	
	for(DelayTime=0;DelayTime<1000;DelayTime++)
	{
		
	}
	if(RxNum&&(RevBuffe[0]==0x82))		//错误状态显示
	{
		switch(RevBuffe[3])
		{
			case 0x00:	PL010V13_PrintfString(0		,120	,16	,"！！！！！！！！！！！！！！");				//错误状态
				break;
			case 0x80:	PL010V13_PrintfString(0		,120	,16	,"药品被卡住！！！！！！！！！");				//错误状态
				break;
			case 0x81:	PL010V13_PrintfString(0		,120	,16	,"缺药！！！！！！！！！！！！");				//错误状态
				break;
			case 0x82:	PL010V13_PrintfString(0		,120	,16	,"等待命令结果超时！！！！！！");				//错误状态
				break;
			case 0xC0:	PL010V13_PrintfString(0		,120	,16	,"单元柜控制器通信异常！！！！");				//错误状态
				break;
			case 0xC1:	PL010V13_PrintfString(0		,120	,16	,"发药层控制器通信异常！！！！");				//错误状态
				break;
		}
//		RevBuffe[0]=0;
//		RevBuffe[1]=0;
//		RevBuffe[2]=0;
//		RevBuffe[3]=0;
	}
	
	#if 0
	if(DelayTime>=100)
	{
		
		DelayTime=0;
		CS5530_ADC_Value=CS5530_ReadData(&CS5530_Pinfo);	//读取AD值，如果返回0xFFFFFFFF,则未读取到24位AD值
//		R61509V_Fill(0,0,64,16,R61509V_BLACK);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
		R61509V_ShowEn(0,0,CS5530_ADC_Value);
		
		if(CS5530_ADC_Value!=0xFFFFFFFF)
		{
			if(testADC==0)
			{				
				testADC=CS5530_ADC_Value;
				bacADC=CS5530_ADC_Value;
//				R61509V_Clean(R61509V_BLACK);			//清除屏幕函数--蓝白	
//				R61509V_DrawLine(0,120,400,120,0X5458);						//AB 两个坐标画一条直线				
//				R61509V_DrawLine(0,230,400,230,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,220,400,220,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,210,400,210,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,200,400,200,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,190,400,190,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,180,400,180,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,170,400,170,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,160,400,160,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,150,400,150,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,140,400,140,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,130,400,130,0X5458);						//AB 两个坐标画一条直线
				R61509V_DrawLine(0,120,400,120,R61509V_YELLOW);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,110,400,110,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,100,400,100,R61509V_YELLOW);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,90,400,90,0X5458);							//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,80,400,80,0X5458);							//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,70,400,70,0X5458);							//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,60,400,60,0X5458);							//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,50,400,50,0X5458);							//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,40,400,40,0X5458);							//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,30,400,30,0X5458);							//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,20,400,20,0X5458);							//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,10,400,10,0X5458);							//AB 两个坐标画一条直线
			}
			bacADC2=CS5530_ADC_Value;
			
			if(CS5530_ADC_Value>=bacADC)
			{				
				CS5530_ADC_Value=CS5530_ADC_Value-bacADC;
			}
			else
			{
				CS5530_ADC_Value=bacADC-CS5530_ADC_Value;
			}
			bacADC=bacADC2;				
//			R61509V_Fill(57,8,100,16,0xF800);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
			R61509V_ShowEn(57,0,CS5530_ADC_Value);
			
			ADC_dotx++;
			
			if(ADC_dotx>=400)
			{
				ADC_dotx=0;
				R61509V_Clean(R61509V_BLACK);			//清除屏幕函数--蓝白
				
//				R61509V_DrawLine(0,120,400,120,0X5458);						//AB 两个坐标画一条直线
				
				
//				R61509V_DrawLine(0,230,400,230,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,220,400,220,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,210,400,210,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,200,400,200,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,190,400,190,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,180,400,180,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,170,400,170,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,160,400,160,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,150,400,150,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,140,400,140,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,130,400,130,0X5458);						//AB 两个坐标画一条直线
				R61509V_DrawLine(0,120,400,120,R61509V_YELLOW);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,110,400,110,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,100,400,100,R61509V_YELLOW);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,90,400,90,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,80,400,80,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,70,400,70,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,60,400,60,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,50,400,50,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,40,400,40,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,30,400,30,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,20,400,20,0X5458);						//AB 两个坐标画一条直线
//				R61509V_DrawLine(0,10,400,10,0X5458);						//AB 两个坐标画一条直线
				
				ADC_dotx1=0;
				ADC_doty1=0;
			}
			ADC_doty=CS5530_ADC_Value;
			
			if(ADC_doty>=240)
				ADC_doty=230;
			
//			ADC_doty=ADC_doty/10;
			
			ADC_doty=(R61509V_H-ADC_doty+1);
			
//			R61509V_DrawDot(ADC_dotx,ADC_doty,0X07FF);			//画点
//			R61509V_DrawLine(ADC_dotx1,ADC_doty1,ADC_dotx,ADC_doty,R61509V_BRRED);						//AB 两个坐标画一条直线
			ADC_dotx1=ADC_dotx;
			ADC_doty1=ADC_doty;
		}
		else
		{
			R61509V_PinConf(&R61509V_Pinfo);
			
		}
	}
	#else
//	R61509V_ShowEn(0,112,DelayTime);
//	R61509V_Clean(DelayTime);			//清除屏幕函数
//	R61509V_Delay(5000);
	if(DelayTime==1)
	{
//		R61509V_Clean(R61509V_BLACK);			//清除屏幕函数--蓝白
//		R61509V_Fill(0,0,400,240,R61509V_BLACK);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)--90/270		
//		R61509V_Fill(0,0,240,400,R61509V_BLACK);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)--0/180
		
//		R61509V_DrawLine(0,120,400,120,R61509V_RED);						//AB 两个坐标画一条直线--90/270
//		R61509V_DrawLine(200,0,200,240,R61509V_YELLOW);						//AB 两个坐标画一条直线--90/270
//		R61509V_DrawLine(0,0,400,240,R61509V_MAGENTA);						//AB 两个坐标画一条直线--90/270
//		R61509V_DrawLine(400,0,0,240,R61509V_BLUE);						//AB 两个坐标画一条直线--90/270
	}
	else if(DelayTime==500)
	{
//			R61509V_Clean(R61509V_CYAN);			//清除屏幕函数--蓝白	
//		R61509V_Fill(0,0,400,240,R61509V_CYAN);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)--90/270		
//		R61509V_Fill(0,0,240,400,R61509V_CYAN);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)--0/180
		
//		R61509V_DrawLine(0,120,400,120,R61509V_YELLOW);						//AB 两个坐标画一条直线--90/270
//		R61509V_DrawLine(200,0,200,240,R61509V_RED);						//AB 两个坐标画一条直线--90/270
//		R61509V_DrawLine(0,0,400,240,R61509V_BLUE);						//AB 两个坐标画一条直线--90/270
//		R61509V_DrawLine(400,0,0,240,R61509V_MAGENTA);						//AB 两个坐标画一条直线--90/270
	}
	else if(DelayTime>=1000)
	{
			DelayTime=0;
			
	}
	#endif
	

}

/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PL010V13_PinSet(void)
{	
//	u16 i=0,j=0;
	
	RS485_Conf.USARTx=USART1;
	RS485_Conf.RS485_CTL_PORT=GPIOA;
	RS485_Conf.RS485_CTL_Pin=GPIO_Pin_11;
	
	GT32L32_Info.GT32L32_Port.sSPIx=SPI1;
	GT32L32_Info.GT32L32_Port.sGT32L32_CS_PORT=GPIOC;
	GT32L32_Info.GT32L32_Port.sGT32L32_CS_PIN=GPIO_Pin_14;
	GT32L32_Info.GT32L32_Port.SPI_BaudRatePrescaler_x=SPI_BaudRatePrescaler_128;
	
	GT32L32_ConfigurationNR(&GT32L32_Info);				//普通SPI通讯方式配置
	
	
	
	CS5530_Pinfo.CS5530_CS_PORT=GPIOC;
	CS5530_Pinfo.CS5530_CS_Pin=GPIO_Pin_3;
	
	CS5530_Pinfo.CS5530_SDI_PORT=GPIOC;
	CS5530_Pinfo.CS5530_SDI_Pin=GPIO_Pin_2;
	
	CS5530_Pinfo.CS5530_SDO_PORT=GPIOC;
	CS5530_Pinfo.CS5530_SDO_Pin=GPIO_Pin_1;
	
	CS5530_Pinfo.CS5530_SCLK_PORT=GPIOC;
	CS5530_Pinfo.CS5530_SCLK_Pin=GPIO_Pin_0;
	
	CS5530_PinConf(&CS5530_Pinfo);
	
	
	R61509V_Pinfo.R61509V_sCS_PORT		=	GPIOA;
	R61509V_Pinfo.R61509V_sCS_Pin			=	GPIO_Pin_12;
	
	R61509V_Pinfo.R61509V_sRS_PORT		=	GPIOA;
	R61509V_Pinfo.R61509V_sRS_Pin			=	GPIO_Pin_8;
	
	R61509V_Pinfo.R61509V_sWR_PORT		=	GPIOA;
	R61509V_Pinfo.R61509V_sWR_Pin			=	GPIO_Pin_15;
	
	R61509V_Pinfo.R61509V_sRD_PORT		=	GPIOC;
	R61509V_Pinfo.R61509V_sRD_Pin			=	GPIO_Pin_5;
	
	R61509V_Pinfo.R61509V_sREST_PORT	=	GPIOD;
	R61509V_Pinfo.R61509V_sREST_Pin		=	GPIO_Pin_2;
	
	R61509V_Pinfo.R61509V_sBL_PORT		=	GPIOA;
	R61509V_Pinfo.R61509V_sBL_Pin			=	GPIO_Pin_3;
	
	R61509V_Pinfo.R61509V_sTE_PORT		=	GPIOC;
	R61509V_Pinfo.R61509V_sTE_Pin			=	GPIO_Pin_4;
	
	R61509V_Pinfo.R61509V_sDATABUS_PORT	=	GPIOB;
	R61509V_Pinfo.R61509V_sDATABUS_Pin	=	GPIO_Pin_All;
	
	R61509V_PinConf(&R61509V_Pinfo);
	

	R61509V_Clean(0X01CF);			//清除屏幕函数--黄色
	R61509V_Delay(0xFFFFFF);
	
	R61509V_Clean(0X07FF);			//清除屏幕函数--红色
	R61509V_Delay(0xFFFFFF);
	
	R61509V_Clean(0x07E0);			//清除屏幕函数--紫色
	R61509V_Delay(0xFFFFFF);
	
	R61509V_Clean(0XFC07);			//清除屏幕函数--蓝色
	R61509V_Delay(0xFFFFFF);
	
	R61509V_Clean(0xF800);			//清除屏幕函数--蓝白
	R61509V_Delay(0xFFFFFF);

	R61509V_Clean(R61509V_BLACK);			//清除屏幕函数------
	R61509V_Delay(0xFFFFFF);
	
//	R61509V_DrawDot(50,50,0X5458);
//	R61509V_DrawDot_big(50,50,0X5458);
//	R61509V_DrawLine(0,0,240,400,0X5458);
//	R61509V_DrawLine(0,400,240,0,0X5458);
	
//	R61509V_DrawCircle(120,100, 50,0,0X5458);				//画一个圆形框
	
//	R61509V_DrawCircle(120,200, 120,0,0X01CF);				//画一个圆形框
	
//	R61509V_DrawCircle(120,400, 80,0,0X5458);				//画一个圆形框
//	
//	R61509V_DrawCircle(120,0, 80,0,0X5458);					//画一个圆形框
//	
//	R61509V_DrawCircle(120,200, 80,1,0X5458);				//画一个圆形框
//	
//	R61509V_DrawCircle(200,120, 50,0,0xF800);				//画一个圆形框
//	
//	R61509V_DrawCircle(120,200, 30,1,0X07FF);				//画一个圆形框
	
//	R61509V_DrawRectangle(3,3,237,397,0x07E0);			//画一个矩形框
//	R61509V_DrawRectangle(4,4,236,396,0x07E0);			//画一个矩形框
//	R61509V_DrawRectangle(5,5,235,395,0x07E0);			//画一个矩形框
//	R61509V_DrawRectangle(6,6,234,394,0x07E0);			//画一个矩形框
//	
//	R61509V_DrawRectangle(7,7,233,393,0X07FF);			//画一个矩形框
//	R61509V_DrawRectangle(8,8,232,392,0X07FF);			//画一个矩形框
//	R61509V_DrawRectangle(9,9,231,391,0X07FF);			//画一个矩形框
//	R61509V_DrawRectangle(10,10,230,390,0X07FF);		//画一个矩形框
//	R61509V_DrawRectangle(11,11,229,389,0X07FF);		//画一个矩形框
//	R61509V_DrawRectangle(12,12,228,388,0X07FF);			//画一个矩形框
//	R61509V_DrawLine(12,12,228,388,0X5458);						//AB 两个坐标画一条直线
//	R61509V_DrawLine(12,388,228,12,0X5458);						//AB 两个坐标画一条直线
	
//	R61509V_DrawLine(0,10,400,10,0X5458);						//AB 两个坐标画一条直线
//	
//	R61509V_DrawLine(0,20,400,20,0X5458);						//AB 两个坐标画一条直线
	
//	R61509V_DrawLine(0,100,160,100,0X5458);						//AB 两个坐标画一条直线
//	
//	R61509V_DrawLine(80,0,80,400,0X5458);						//AB 两个坐标画一条直线
	
//	R61509V_DrawRectangle(10,10,390,230,0X07FF);		//画一个矩形框
	
//	R61509V_DrawCircle(200,120, 10,1,0X5458);					//画一个圆形框
//	
//	R61509V_DrawCircle(200,120, 80,0,0X5458);					//画一个圆形框
	
//	R61509V_DrawLine(240,240,20,240,0X5458);						//AB 两个坐标画一条直线
	
//	R61509V_ShowChar(1,1,32,100,zimo);								//高通字库测试程序
//	
//	R61509V_ShowCharT(50,50,15,0);
//	R61509V_ShowEn(200,120,12);
	
//	PL010V13_PrintfString(0		,16	,16	,"待发药槽位：%3d",RevBuffe[0]);				//后边的省略号就是可变参数
//	PL010V13_PrintfString(0		,32	,16	,"待发药数量：%3d",RevBuffe[1]);				//后边的省略号就是可变参数

//	PL010V13_PrintfString(0		,0	,32	,"槽位-%2d数量-%2d",RevBuffe[0],RevBuffe[1]);				//后边的省略号就是可变参数
	
//	PL010V13_PrintfString(0		,0	,32	,"待发药数量：%2d",RevBuffe[1]);				//后边的省略号就是可变参数
//	
//	
//	PL010V13_PrintfString(0		,100	,32	,"待发药数量：%2d",RevBuffe[1]);				//后边的省略号就是可变参数
//	PL010V13_PrintfString(0		,100	,32	,"待发药数量：%2d",RevBuffe[1]);				//后边的省略号就是可变参数
//	
	#if 1
	PL010Delay(0xFFFF);
	PL010V13_PrintfString(1		,0	,16	,"待发药槽位：%2d",RevBuffe[0]);				//后边的省略号就是可变参数
	PL010V13_PrintfString(1		,0	,16	,"待发药槽位：%2d",RevBuffe[0]);				//后边的省略号就是可变参数
	PL010Delay(0xFFFF);
	PL010V13_PrintfString(1		,20	,16	,"待发药数量：%2d",RevBuffe[1]);				//后边的省略号就是可变参数
	PL010Delay(0xFFFF);
	PL010V13_PrintfString(1		,40	,16	,"已发药数量：%2d",RevBuffe[1]);				//后边的省略号就是可变参数
	PL010Delay(0xFFFF);
	
//	PL010V13_PrintfString(0		,60	,16	,"总共请求数量：%4d",SumFQ);				//总共发药请求数量
//	PL010V13_PrintfString(0		,80	,16	,"总共发药数量：%4d",SumFed);				//总共已发药数量
	PL010V13_PrintfString(1		,100	,16	,"错误状态：");				//错误状态
	PL010Delay(0xFFFF);
	
	PL010V13_PrintfString(1		,160	,16	,"提示：长按3秒读取发药头");				//错误状态
	PL010Delay(0xFFFF);
	

	//平行线
	R61509V_DrawLine(221,0,221,400,R61509V_WHITE);						//AB 两个坐标画一条直线
	R61509V_DrawLine(201,0,201,400,R61509V_WHITE);						//AB 两个坐标画一条直线
	R61509V_DrawLine(181,0,181,400,R61509V_WHITE);						//AB 两个坐标画一条直线
	R61509V_DrawLine(161,0,161,400,R61509V_WHITE);						//AB 两个坐标画一条直线
	//垂直线--中
	R61509V_DrawLine(161,200,221,200,R61509V_WHITE);					//AB 两个坐标画一条直线
	//垂直线--右
	R61509V_DrawLine(161,250-1,221,250-1,R61509V_WHITE);			//AB 两个坐标画一条直线
	R61509V_DrawLine(161,300-1,221,300-1,R61509V_WHITE);			//AB 两个坐标画一条直线
	R61509V_DrawLine(161,350-1,221,350-1,R61509V_WHITE);			//AB 两个坐标画一条直线
	R61509V_DrawLine(161,400-1,221,400-1,R61509V_WHITE);			//AB 两个坐标画一条直线
	
	//垂直线--左
	R61509V_DrawLine(161,1,221,1,					R61509V_WHITE);							//AB 两个坐标画一条直线
	R61509V_DrawLine(161,51,221,51,				R61509V_WHITE);						//AB 两个坐标画一条直线
	R61509V_DrawLine(161,101,221,101,			R61509V_WHITE);					//AB 两个坐标画一条直线
	R61509V_DrawLine(161,151,221,151,			R61509V_WHITE);					//AB 两个坐标画一条直线

	//填充
	R61509V_Fill(2,180,50,200-2,R61509V_RED);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
	
	
	
	
//	R61509V_DrawLine(10,200,10,280,R61509V_WHITE);						//AB 两个坐标画一条直线
	
	#else
//	R61509V_Clean(R61509V_WHITE);			//清除屏幕函数------
//	PL010Delay(0x8FFFFF);
	R61509V_Clean(R61509V_BLUE);			//清除屏幕函数------
	PL010Delay(0x8FFFFF);
	R61509V_Clean(R61509V_GRED);			//清除屏幕函数------
	PL010Delay(0x8FFFFF);
//	R61509V_Clean(R61509V_BLACK);			//清除屏幕函数------
//	PL010Delay(0xFFFF);
	PL010V13_PrintfString(0	,0,16	,"TEST");				//错误状态
	#endif
//	R61509V_Clean(R61509V_BLACK);			//清除屏幕函数------
	
}
/*******************************************************************************
*函数名		:	LCD_ShowString
*功能描述	:	显示字符串高通字库
*输入			: x,y:起点坐标
						*p:字符串起始地址
						用16字体
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
unsigned int PL010V13_PrintfString(u16 x,u16 y,u8 font,const char *format,...)				//后边的省略号就是可变参数
{ 
		
//		va_list ap; 										//VA_LIST 是在C语言中解决变参问题的一组宏，所在头文件：#include <stdarg.h>,用于获取不确定个数的参数
//		static char string[ 256 ];			//定义数组，
//  	va_start( ap, format );
//		vsprintf( string , format, ap );    
//		va_end( ap );
	
//	char	*Char_Buffer=NULL;		//记录format内码
	u16 i=0;		//显示

	//1)**********获取数据宽度
	u16 num=strlen((const char*)format);		//获取数据宽度
	//2)**********定义缓冲区大小变量
	unsigned int BufferSize;
	//3)**********args为定义的一个指向可变参数的变量，va_list以及下边要用到的va_start,va_end都是是在定义，可变参数函数中必须要用到宏， 在stdarg.h头文件中定义
	va_list args; 
	free(Char_Buffer);						//释放动态空间	
	//4)**********申请动态空间
//	Char_Buffer = (char*)malloc(sizeof(char) * num);
//	if(Char_Buffer==NULL)
//	{
//		Char_Buffer=NULL;
//		return 0;
//	}
	//5)**********初始化args的函数，使其指向可变参数的第一个参数，format是可变参数的前一个参数
	va_start(args, format);
	//6)**********正常情况下返回生成字串的长度(除去\0),错误情况返回负值
	BufferSize = vsprintf(Char_Buffer, format, args);
	num=BufferSize;
	//7)**********结束可变参数的获取
	va_end(args);                                      		
	//8)**********将等发送缓冲区大小（数据个数）及缓冲区地址发给DMA开启发送
//	while(*Char_Buffer!='\0')
	for(i=0;i<num;i++)
	{ 
		unsigned char dst=Char_Buffer[i];
//		u8 GTBuffer[512]={0};		//点阵数据存储空间
		u32 lengh=0;						//汉字点阵的数据长度		
		if(dst>0x80)		//双字节--汉字
		{
			u16 word=dst<<8;			
//			Char_Buffer++;
			dst=Char_Buffer[i+1];
			word=word|dst;			
			//显示超限判断
			if(font==16&&x>R61509V_W-16)
			{
				x=0;
				y+=16;
			}
			if(font==32&&x>R61509V_W-32)
			{
				x=0;
				y+=32;
			}
			if(font==16&&y>R61509V_H-16)
			{
				y=x=0;
			}
			if(font==32&&y>R61509V_H-32)
			{
				y=x=0;
			}
			lengh=GT32L32_ReadBuffer(&GT32L32_Info,font,word,GT32L32_Info.GT32L32_Data.GT32L32_Buffer);		//从字库中读数据函数
			//写入屏幕
			R61509V_ShowChar(x,y,font,lengh,GT32L32_Info.GT32L32_Data.GT32L32_Buffer);
			//显示地址增加	
			if(font==12)
			{
				x+=12;
			}
			else if(font==16)
			{
				x+=16;
			}
			else if(font==24)
			{
				x+=24;
			}
			else if(font==32)
			{
				x+=32;
			}
//			Char_Buffer++;
			i++;		//双字节，减两次
		}
		else		//单字节
		{			
			if(x>R61509V_W-16)
			{
				x=0;
				y+=32;
			}
			if(font==16&&y>R61509V_H-16)
			{
				y=x=0;
			}
			if(font==32&&y>R61509V_H-32)
			{
				y=x=0;
			}
			lengh=GT32L32_ReadBuffer(&GT32L32_Info,font,(u16)dst,GT32L32_Info.GT32L32_Data.GT32L32_Buffer);		//从字库中读数据函数
//			//写入屏幕
			R61509V_ShowChar(x,y,font,lengh,GT32L32_Info.GT32L32_Data.GT32L32_Buffer);			
			//显示地址增加
			if(font==12)
			{
				x+=6;
			}
			else if(font==16)
			{
				x+=8;
			}
			else if(font==24)
			{
				x+=12;
			}
			else if(font==32)
			{
				x+=16;
			}			
//			Char_Buffer++;
//			i++;		//双字节，减两次
		}
	}
	//9)**********DMA发送完成后注意应该释放缓冲区：free(USART_BUFFER);
//	free(Char_Buffer);		//发送完成后注意应该释放缓冲区：free(Char_Buffer); 
	return BufferSize;
}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PL010V13_DISPLAY(void)
{
//	R61509V_DrawPixelEx( 100, 100,LCD_FORE_COLOR);
//	R61509V_DrawHLine( 10, 100, 200, LCD_FORE_COLOR);
}
void PL010Delay(u32 time)
{	
	while(time--);
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void LCD_WS(void)		//位闪烁
{
	if(NumFW)		//槽位闪烁	DSPTime
	{
		if(NumFW==1&&(Onlinede>>0&0x01))
		{
			if(DSPTime==500)
			{
				R61509V_Fill(2,180,50,200-2,R61509V_GREEN);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
			}
			else if(DSPTime==250)
			{
				R61509V_Fill(2,180,50,200-2,R61509V_MAGENTA);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
			}
		}
		else if(NumFW==2&&(Onlinede>>1&0x01))
		{
			if(DSPTime==500)
			{
				R61509V_Fill(52,180,100,200-2,R61509V_GBLUE);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
			}
			else if(DSPTime==250)
			{
				R61509V_Fill(52,180,100,200-2,R61509V_MAGENTA);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
			}			
		}
		else if(NumFW==3&&(Onlinede>>2&0x01))
		{
			if(DSPTime==500)
			{
				R61509V_Fill(102,180,150,200-2,R61509V_GREEN);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
			}
			else if(DSPTime==250)
			{
				R61509V_Fill(102,180,150,200-2,R61509V_MAGENTA);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
			}			
		}
		else if(NumFW==4&&(Onlinede>>3&0x01))
		{
			if(DSPTime==500)
			{
				R61509V_Fill(152,180,200,200-2,R61509V_GREEN);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
			}
			else if(DSPTime==250)
			{
				R61509V_Fill(152,180,200,200-2,R61509V_MAGENTA);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
			}			
		}
		else if(NumFW==5&&(Onlinede>>4&0x01))
		{
			if(DSPTime==500)
			{
				R61509V_Fill(202,180,250,200-2,R61509V_GREEN);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
			}
			else if(DSPTime==250)
			{
				R61509V_Fill(202,180,250,200-2,R61509V_MAGENTA);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
			}			
		}
		else if(NumFW==6&&(Onlinede>>5&0x01))
		{
			if(DSPTime==500)
			{
				R61509V_Fill(252,180,300,200-2,R61509V_GREEN);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
			}
			else if(DSPTime==250)
			{
				R61509V_Fill(252,180,300,200-2,R61509V_MAGENTA);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
			}			
		}
		else if(NumFW==7&&(Onlinede>>6&0x01))
		{
			if(DSPTime==500)
			{
				R61509V_Fill(302,180,350,200-2,R61509V_GREEN);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
			}
			else if(DSPTime==250)
			{
				R61509V_Fill(302,180,350,200-2,R61509V_MAGENTA);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
			}			
		}
		else if(NumFW==8&&(Onlinede>>7&0x01))
		{
			if(DSPTime==500)
			{
				R61509V_Fill(352,180,400-2,200-2,R61509V_GREEN);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
			}
			else if(DSPTime==250)
			{
				R61509V_Fill(352,180,400-2,200-2,R61509V_MAGENTA);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
			}			
		}
	}
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void LCD_WXS(void)		//位显示
{
		if((Onlinede>>0&0x01)==0x01)
		{
			R61509V_Fill(2,180,50,200-2,R61509V_GREEN);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
		}
		else
		{
			R61509V_Fill(2,180,50,200-2,R61509V_RED);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
		}
		if((Onlinede>>1&0x01)==0x01)
		{
			R61509V_Fill(52,180,100,200-2,R61509V_GREEN);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
		}
		else
		{
			R61509V_Fill(52,180,100,200-2,R61509V_RED);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
		}
		if((Onlinede>>2&0x01)==0x01)
		{
			R61509V_Fill(102,180,150,200-2,R61509V_GREEN);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
		}
		else
		{
			R61509V_Fill(102,180,150,200-2,R61509V_RED);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
		}
		if((Onlinede>>3&0x01)==0x01)
		{
			R61509V_Fill(152,180,200,200-2,R61509V_GREEN);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
		}
		else
		{
			R61509V_Fill(152,180,200,200-2,R61509V_RED);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
		}
		if((Onlinede>>4&0x01)==0x01)
		{
			R61509V_Fill(202,180,250,200-2,R61509V_GREEN);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
		}
		else
		{
			R61509V_Fill(202,180,250,200-2,R61509V_RED);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
		}
		if((Onlinede>>5&0x01)==0x01)
		{
			R61509V_Fill(252,180,300,200-2,R61509V_GREEN);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
		}
		else
		{
			R61509V_Fill(252,180,300,200-2,R61509V_RED);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
		}
		if((Onlinede>>6&0x01)==0x01)
		{
			R61509V_Fill(302,180,350,200-2,R61509V_GREEN);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
		}
		else
		{
			R61509V_Fill(302,180,350,200-2,R61509V_RED);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
		}
		if((Onlinede>>7&0x01)==0x01)
		{
			R61509V_Fill(352,180,400-2,200-2,R61509V_GREEN);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
		}
		else
		{
			R61509V_Fill(352,180,400-2,200-2,R61509V_RED);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)
		}
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void LCD_DDSP(void)		//显示总共请求数量和已发数量
{
	u8 i=0;
	u8 w=2;
	//请求数量显示
//	for(i=0;i<8;i++)
//	{
//		PL010V13_PrintfString(w,200,16	,"%5d",SumFQ[i]);				//待发药数量，后边的省略号就是可变参数
//		w+=50;
//	}
	//请求数量显示
	PL010V13_PrintfString(2,200,16	,"%5d",SumFQ[0]);				//待发药数量，后边的省略号就是可变参数
	PL010V13_PrintfString(52,200,16	,"%5d",SumFQ[1]);				//待发药数量，后边的省略号就是可变参数
	PL010V13_PrintfString(102,200,16	,"%5d",SumFQ[2]);				//待发药数量，后边的省略号就是可变参数
	PL010V13_PrintfString(152,200,16	,"%5d",SumFQ[3]);				//待发药数量，后边的省略号就是可变参数
	PL010V13_PrintfString(201,200,16	,"%5d",SumFQ[4]);				//待发药数量，后边的省略号就是可变参数
	PL010V13_PrintfString(251,200,16	,"%5d",SumFQ[5]);				//待发药数量，后边的省略号就是可变参数
	PL010V13_PrintfString(301,200,16	,"%5d",SumFQ[6]);				//待发药数量，后边的省略号就是可变参数
	PL010V13_PrintfString(351,200,16	,"%5d",SumFQ[7]);				//待发药数量，后边的省略号就是可变参数

	//已发数量显示	
	PL010V13_PrintfString(2,220,16		,"%5d",SumFed[0]);				//待发药数量，后边的省略号就是可变参数
	PL010V13_PrintfString(52,220,16		,"%5d",SumFed[1]);				//待发药数量，后边的省略号就是可变参数
	PL010V13_PrintfString(102,220,16	,"%5d",SumFed[2]);				//待发药数量，后边的省略号就是可变参数
	PL010V13_PrintfString(152,220,16	,"%5d",SumFed[3]);				//待发药数量，后边的省略号就是可变参数
	PL010V13_PrintfString(201,220,16	,"%5d",SumFed[4]);				//待发药数量，后边的省略号就是可变参数
	PL010V13_PrintfString(251,220,16	,"%5d",SumFed[5]);				//待发药数量，后边的省略号就是可变参数
	PL010V13_PrintfString(301,220,16	,"%5d",SumFed[6]);				//待发药数量，后边的省略号就是可变参数
	PL010V13_PrintfString(351,220,16	,"%5d",SumFed[7]);				//待发药数量，后边的省略号就是可变参数
}
#endif
