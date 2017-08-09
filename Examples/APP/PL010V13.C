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


#include "STM32F10x_BitBand.H"
#include "STM32_GPIO.H"
#include "STM32_SYS.H"
#include "STM32_SYSTICK.H"
#include "STM32_WDG.H"
#include "STM32_PWM.H"

#include "string.h"				//串和内存操作函数头文件
#include "stm32f10x_dma.h"

u16	DelayTime=0x1000;
u32 testADC=0;
u32 bacADC=0;
u32 bacADC2=0;

u16 ADC_dotx=0;
u16 ADC_doty=0;
u16 ADC_dotx1=0;
u16 ADC_doty1=0;

R61509V_Pindef R61509V_Pinfo;
CS5530_Pindef CS5530_Pinfo;
u32 CS5530_ADC_Value=0xFFFFFFFF;
t_Point point;
u8 zimo[720]="R61509V_DrawRectangle(11,11,229,389,0X07FF)";
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
	
	SysTick_Configuration(1000);	//系统嘀嗒时钟配置72MHz,单位为uS
	
//	IWDG_Configuration(1000);			//独立看门狗配置---参数单位ms	
	
//	PWM_OUT(TIM2,PWM_OUTChannel1,1,200);						//PWM设定-20161127版本
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
	
	#if 1
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
		
		R61509V_DrawLine(0,120,400,120,R61509V_RED);						//AB 两个坐标画一条直线--90/270
		R61509V_DrawLine(200,0,200,240,R61509V_YELLOW);						//AB 两个坐标画一条直线--90/270
		R61509V_DrawLine(0,0,400,240,R61509V_MAGENTA);						//AB 两个坐标画一条直线--90/270
		R61509V_DrawLine(400,0,0,240,R61509V_BLUE);						//AB 两个坐标画一条直线--90/270
	}
	else if(DelayTime==500)
	{
//			R61509V_Clean(R61509V_CYAN);			//清除屏幕函数--蓝白	
//		R61509V_Fill(0,0,400,240,R61509V_CYAN);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)--90/270		
//		R61509V_Fill(0,0,240,400,R61509V_CYAN);				//在指定区域内填充指定颜色;区域大小:(xend-xsta)*(yend-ysta)--0/180
		
		R61509V_DrawLine(0,120,400,120,R61509V_YELLOW);						//AB 两个坐标画一条直线--90/270
		R61509V_DrawLine(200,0,200,240,R61509V_RED);						//AB 两个坐标画一条直线--90/270
		R61509V_DrawLine(0,0,400,240,R61509V_BLUE);						//AB 两个坐标画一条直线--90/270
		R61509V_DrawLine(400,0,0,240,R61509V_MAGENTA);						//AB 两个坐标画一条直线--90/270
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
	

//	R61509V_Clean(0X01CF);			//清除屏幕函数--黄色
//	R61509V_Delay(1000);
	
//	R61509V_Clean(0X07FF);			//清除屏幕函数--红色
//	R61509V_Delay(1000);
//	
//	R61509V_Clean(0x07E0);			//清除屏幕函数--紫色
//	R61509V_Delay(1000);
//	
//	R61509V_Clean(0XFC07);			//清除屏幕函数--蓝色
//	R61509V_Delay(1000);
	
//	R61509V_Clean(0xF800);			//清除屏幕函数--蓝白
//	R61509V_Delay(1000);

	R61509V_Clean(R61509V_WHITE);			//清除屏幕函数------
	R61509V_Delay(1000);
	
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
	
	R61509V_DrawLine(0,100,160,100,0X5458);						//AB 两个坐标画一条直线
	
	R61509V_DrawLine(80,0,80,400,0X5458);						//AB 两个坐标画一条直线
	
//	R61509V_DrawRectangle(10,10,390,230,0X07FF);		//画一个矩形框
	
//	R61509V_DrawCircle(200,120, 10,1,0X5458);					//画一个圆形框
//	
//	R61509V_DrawCircle(200,120, 80,0,0X5458);					//画一个圆形框
	
//	R61509V_DrawLine(240,240,20,240,0X5458);						//AB 两个坐标画一条直线
	
//	R61509V_ShowChar(1,1,32,100,zimo);								//高通字库测试程序
//	
//	R61509V_ShowCharT(50,50,15,0);
	R61509V_ShowEn(0,0,12);
	
	R61509V_Clean(R61509V_BLACK);			//清除屏幕函数------
	
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
#endif
