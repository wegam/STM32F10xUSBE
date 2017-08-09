/******************************** User_library *********************************
* 文件名 	: SPI_FLASH.C
* 作者   	: wegam@sina.com
* 版本   	: V
* 日期   	: 2017/04/16
* 说明   	: 
********************************************************************************
SPI_FLASH使用功能测试
1）需要宏定义 SPI_FLASH
2）使用USB_TEST 板测试
3）需要宏定义SPI引脚

*******************************************************************************/
#include "STM32_A3987.H"

//#define USB_TEST_BD				//USB_TEST板
//#define CMSIS_CDC_BD			//CMSIS-CDC板

//#include "stm32f10x_spi.h"
//#include "stm32f10x_gpio.h"
//#include "stm32f10x_rcc.h"
//#include "stm32f10x_dma.h"
////#include "stm32f10x_nvic.h"

////#include "STM32_WOW.H"
//#include "STM32_PWM.H"
//#include "STM32_SYS.H"

/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void A3987_GPIO_Configuration(void)
{
	//结构体定义
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//打开相应管脚时钟
	A3987_EN_CLOCK(ENABLE);		//A3987_EN
	A3987_PUL_CLOCK(ENABLE);	//A3987_PUL
	A3987_DIR_CLOCK(ENABLE);	//A3987_DIR
	A3987_MS1_CLOCK(ENABLE);	//A3987_MS1
	A3987_MS2_CLOCK(ENABLE);	//A3987_MS2
	
	//A3987_EN管脚定义
	GPIO_InitStructure.GPIO_Pin=A3987_EN_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(A3987_EN_PORT,&GPIO_InitStructure);
	//A3987_PUL管脚定义
	GPIO_InitStructure.GPIO_Pin=A3987_PUL_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;					//GPIO_Mode_IPU			//GPIO_Mode_AF_PP
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(A3987_PUL_PORT,&GPIO_InitStructure);
	//A3987_DIR管脚定义
	GPIO_InitStructure.GPIO_Pin=A3987_DIR_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(A3987_DIR_PORT,&GPIO_InitStructure);
	//A3987_MS1管脚定义
	GPIO_InitStructure.GPIO_Pin=A3987_MS1_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;				//GPIO_Mode_Out_PP  //GPIO_Mode_AF_PP
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(A3987_MS1_PORT,&GPIO_InitStructure);
	//A3987_MS2管脚定义
	GPIO_InitStructure.GPIO_Pin=A3987_MS2_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;				//GPIO_Mode_Out_PP  //GPIO_Mode_AF_PP
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(A3987_MS2_PORT,&GPIO_InitStructure);
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void A3987_SETUP(void)
{
	A3987_EN_ENABLE();
	A3987_DIR_HIGH();
	A3987_MS1_LOW();
	A3987_MS2_LOW();
}







