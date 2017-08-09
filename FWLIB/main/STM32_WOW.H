#ifndef __STM32_WOW_H
#define __STM32_WOW_H

/* Includes ------------------------------------------------------------------*/
//#include "stm32f10x_map.h"
//#include "STM32_WDG.H"
//#include "STM32_SYSTICK.H"
//#include "STM32_SYS.H"

//#include "STM32_SYS.H"
//#include "stm32f10x_gpio.h"
//#include "STM32_WDG.H"
//#include "STM32_TIM.H"
//#include "STM32_SYSLED.h"
//#include "STM32_EXTI.H"
//#include "STM32_NVIC.H"

//#include "STM32F10x_BitBand.H"
//#define STM32_SYS


//***********************************数字调音板***********************************//
#ifdef	DEMO
	#include "DEMO.H"
#endif

//***********************************数字调音板***********************************//
#ifdef	R2A15908SP
	#include "R2A15908SP.H"
#endif
//***********************************串口测试***********************************//
#ifdef	USART_TEST
	#include "USART_TEST.H"
#endif
//***********************************7寸LCD***********************************//
#ifdef	SSD1963
	#include "SSD1963.H"
#endif
//***********************************7寸LCD***********************************//
#ifdef	STM32_LCD
	#include "STM32_LCD.H"
#endif
//***********************************ADC_TEST***********************************//
#ifdef	ADC_TEST
	#include "ADC_TEST.H"
#endif
//***********************************ADC_TEST***********************************//
#ifdef ADC_Temperature
	#include "ADC_Temperature.H"
#endif
//***********************************ADC_TEST***********************************//
#ifdef CAN_TEST
	#include "CAN_TEST.H"
#endif

//***********************************ADC_TEST***********************************//
#ifdef SPI_TEST
	#include "SPI_TEST.H"
#endif
//***********************************OFFLINE_ISP***********************************//
#ifdef OFFLINE_ISP
#include "OFFLINE_ISP.H"
#endif
//***********************************ADC_TEST***********************************//
//#ifdef MS0800_WOW
//	#include "MS0800_WOW.H"
//#endif
//***********************************CD4511BM_TEST***********************************//
#ifdef CD4511BM_TEST
	#include "CD4511BM_TEST.H"
#endif

//***********************************CD4511BM_TEST***********************************//
#ifdef MS0800_PL001V20
	#include "MS0800_PL001V20.H"
#endif

//***********************************CD4511BM_TEST***********************************//
#ifdef MS0800_PC004V10
	#include "MS0800_PC004V10.H"
#endif
//#ifndef R2A15908SP
//	#warning "R2A15908SP项目未定义"
//#endif

//***********************************CD4511BM_TEST***********************************//
#ifdef MS0800_PM001V20
	#include "MS0800_PM001V20.H"
#endif

//***********************************CD4511BM_TEST***********************************//
#ifdef MX25L4006E
	#include "MX25L4006E.H"
#endif

//***********************************TM1618_TEST***********************************//
#ifdef TM1618
	#include "TM1618_TEST.H"
#endif

//***********************************VirtualCom***********************************//
#ifdef VirtualCom
	#include "VirtualCom.H"
#endif

//***********************************USBtoSPI***********************************//
#ifdef USBtoSPI
	#include "USBtoSPI.H"
#endif

//***********************************SPItoUSART***********************************//
#ifdef SPItoUSART
	#include "SPItoUSART.H"
#endif

//***********************************PWM_TEST***********************************//
#ifdef PWM_TEST
	#include "PWM_TEST.H"
#endif

//*********************************OLED096_TEST*********************************//
#ifdef OLED096_TEST
	#include "OLED096_TEST.H"
#endif
//*********************************LK3U_40MR*********************************//
#ifdef LK3U_40MR
	#include "LK3U_40MR.H"
#endif

//*********************************FX2N_14MR*********************************//
#ifdef FX2N_14MR
	#include "FX2N_14MR.H"
#endif

//*********************************A3987_TEST*********************************//
#ifdef A3987_TEST
	#include "A3987_TEST.H"
#endif

//*********************************Buzzer_TEST*********************************//
#ifdef Buzzer_TEST
	#include "Buzzer_TEST.H"
#endif

//*********************************PM001V20*********************************//
#ifdef PM001V20
	#include "PM001V20.H"
#endif
//*********************************PC001V21*********************************//
#ifdef PC001V21
	#include "PC001V21.H"
#endif

//*********************************PD002V30*********************************//
#ifdef PD002V30
	#include "PD002V30.H"
#endif
//*********************************PL001V20*********************************//
#ifdef PL001V20
	#include "PL001V20.H"
#endif
//*********************************PC006V20*********************************//
#ifdef PC006V20
	#include "PC006V20.H"
#endif
//*********************************PL006V10*********************************//
#ifdef PL006V10
	#include "PL006V10.H"
#endif
//*********************************PD011V10*********************************//
#ifdef PD011V10
	#include "PD011V10.H"
#endif
//*********************************PD006V20*********************************//
#ifdef PD006V20
	#include "PD006V20.H"
#endif
//*********************************PD004V10*********************************//
#ifdef PD004V10
	#include "PD004V10.H"
#endif
//*********************************PL010V13*********************************//
#ifdef PL010V13
	#include "PL010V13.H"
#endif
//*********************************PL013V10*********************************//
#ifdef PL013V10
	#include "PL013V10.H"
#endif
//*********************************PC004V21*********************************//
#ifdef PC004V21
	#include "PC004V21.H"
#endif
//*********************************PD003VG*********************************//
#ifdef PD003VG
	#include "PD003VG.H"
#endif
//*********************************PD014V14*********************************//
#ifdef PD014V14
	#include "PD014V14.H"
#endif
//*********************************PM004V10*********************************//
#ifdef PM004V10
	#include "PM004V10.H"
#endif
//*********************************USBTEST_RS485TOUSART*********************************//
#ifdef USBTEST_RS485TOUSART
	#include "USBTEST_RS485TOUSART.H"
#endif
//*********************************PC001V22*********************************//
#ifdef PC001V22
	#include "PC001V22.H"
#endif

















































/*****************************************************************************************************/
/*===========================================分割线==================================================*/
/*****************************************************************************************************/
/********************************位带定义**************************************
* 位带操作,实现51类似的GPIO控制功能
* 具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
* IO口操作宏定义
******************************************************************************/

/********************************说明**************************************
* 1 位绑定公式--位绑定两个区域
* SRAM 区 0x2000 0000 ~ 0x200f ffff
* 假设绑定后新的地址为 AliasAddr = 0x22000000 +（(A-0x2000 0000)*8 +n）4；
* A 是变量的地址是在SRAM 区之间 具体是哪一个不知道
* n 是在变量中的位
* 片上外设 0x4000 0000 ~ 0x400f ffff
* 假设绑定后新的地址为  AliasAddr = 0x42000000 +（(A-0x4000 0000)*8 +n）4；
* 									0x42000000 +（(A-0x4000 0000)*32 +n*4）；
* 需要注意的是位绑定只有最后一位为有效值；
* (需要先配置IO）
*******************************************************************************/



//#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
// //把该地址转换成一个指针
// #define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))

// #define BIT_ADDR(addr, bitnum)   MEM_ADDR( BITBAND(addr, bitnum)  )

// #define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C
// #define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C
// #define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C
// #define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C
// #define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C

// #define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808
// #define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08
// #define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008
// #define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408
// #define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808
////-----------------------------------------------------
// //-----------------------------------------------------
// //使用方法：
// //例：PAout(0)=1;PBout(3)=0;
// //-----------------------------------------------------
//#define PAout(n) BITBAND(GPIOA_ODR_Addr,n)
//#define PAin(n) BITBAND(GPIOA_IDR_Addr,n)
//#define PBout(n) BITBAND(GPIOB_ODR_Addr,n)
//#define PBin(n) BITBAND(GPIOB_IDR_Addr,n)
//#define PCout(n) BITBAND(GPIOC_ODR_Addr,n)
//#define PCin(n) BITBAND(GPIOC_IDR_Addr,n)
//#define PDout(n) BITBAND(GPIOD_ODR_Addr,n)
//#define PDin(n) BITBAND(GPIOD_IDR_Addr,n)
//#define PEout(n) BITBAND(GPIOE_ODR_Addr,n)
//#define PEin(n) BITBAND(GPIOE_IDR_Addr,n)
////-----------------------------------------------------
// //-----------------------------------------------------
// //使用方法：
// //例：PA0=1;PB0=0;
// //-----------------------------------------------------
// #define PA0   BIT_ADDR(GPIOA_ODR_Addr, 0)  //输出
// #define PA1   BIT_ADDR(GPIOA_ODR_Addr, 1)  //输出
// #define PA2   BIT_ADDR(GPIOA_ODR_Addr, 2)  //输出
// #define PA3   BIT_ADDR(GPIOA_ODR_Addr, 3)  //输出
// #define PA4   BIT_ADDR(GPIOA_ODR_Addr, 4)  //输出
// #define PA5   BIT_ADDR(GPIOA_ODR_Addr, 5)  //输出
// #define PA6   BIT_ADDR(GPIOA_ODR_Addr, 6)  //输出
// #define PA7   BIT_ADDR(GPIOA_ODR_Addr, 7)  //输出
// #define PA8   BIT_ADDR(GPIOA_ODR_Addr, 8)  //输出
// #define PA9   BIT_ADDR(GPIOA_ODR_Addr, 9)  //输出
// #define PA10  BIT_ADDR(GPIOA_ODR_Addr, 10) //输出
// #define PA11  BIT_ADDR(GPIOA_ODR_Addr, 11) //输出
// #define PA12  BIT_ADDR(GPIOA_ODR_Addr, 12) //输出
// #define PA13  BIT_ADDR(GPIOA_ODR_Addr, 13) //输出
// #define PA14  BIT_ADDR(GPIOA_ODR_Addr, 14) //输出
// #define PA15  BIT_ADDR(GPIOA_ODR_Addr, 15) //输出

// #define PA0in   BIT_ADDR(GPIOA_IDR_Addr, 0)  //输入
// #define PA1in   BIT_ADDR(GPIOA_IDR_Addr, 1)  //输入
// #define PA2in   BIT_ADDR(GPIOA_IDR_Addr, 2)  //输入
// #define PA3in   BIT_ADDR(GPIOA_IDR_Addr, 3)  //输入
// #define PA4in   BIT_ADDR(GPIOA_IDR_Addr, 4)  //输入
// #define PA5in   BIT_ADDR(GPIOA_IDR_Addr, 5)  //输入
// #define PA6in   BIT_ADDR(GPIOA_IDR_Addr, 6)  //输入
// #define PA7in   BIT_ADDR(GPIOA_IDR_Addr, 7)  //输入
// #define PA8in   BIT_ADDR(GPIOA_IDR_Addr, 8)  //输入
// #define PA9in   BIT_ADDR(GPIOA_IDR_Addr, 9)  //输入
// #define PA10in  BIT_ADDR(GPIOA_IDR_Addr, 10) //输入
// #define PA11in  BIT_ADDR(GPIOA_IDR_Addr, 11) //输入
// #define PA12in  BIT_ADDR(GPIOA_IDR_Addr, 12) //输入
// #define PA13in  BIT_ADDR(GPIOA_IDR_Addr, 13) //输入
// #define PA14in  BIT_ADDR(GPIOA_IDR_Addr, 14) //输入
// #define PA15in  BIT_ADDR(GPIOA_IDR_Addr, 15) //输入

// //-----------------------------------------------------
// #define PB0   BIT_ADDR(GPIOB_ODR_Addr, 0)  //输出
// #define PB1   BIT_ADDR(GPIOB_ODR_Addr, 1)  //输出
// #define PB2   BIT_ADDR(GPIOB_ODR_Addr, 2)  //输出
// #define PB3   BIT_ADDR(GPIOB_ODR_Addr, 3)  //输出
// #define PB4   BIT_ADDR(GPIOB_ODR_Addr, 4)  //输出
// #define PB5   BIT_ADDR(GPIOB_ODR_Addr, 5)  //输出
// #define PB6   BIT_ADDR(GPIOB_ODR_Addr, 6)  //输出
// #define PB7   BIT_ADDR(GPIOB_ODR_Addr, 7)  //输出
// #define PB8   BIT_ADDR(GPIOB_ODR_Addr, 8)  //输出
// #define PB9   BIT_ADDR(GPIOB_ODR_Addr, 9)  //输出
// #define PB10  BIT_ADDR(GPIOB_ODR_Addr, 10) //输出
// #define PB11  BIT_ADDR(GPIOB_ODR_Addr, 11) //输出
// #define PB12  BIT_ADDR(GPIOB_ODR_Addr, 12) //输出
// #define PB13  BIT_ADDR(GPIOB_ODR_Addr, 13) //输出
// #define PB14  BIT_ADDR(GPIOB_ODR_Addr, 14) //输出
// #define PB15  BIT_ADDR(GPIOB_ODR_Addr, 15) //输出

// #define PB0in   BIT_ADDR(GPIOB_IDR_Addr, 0)  //输入
// #define PB1in   BIT_ADDR(GPIOB_IDR_Addr, 1)  //输入
// #define PB2in   BIT_ADDR(GPIOB_IDR_Addr, 2)  //输入
// #define PB3in   BIT_ADDR(GPIOB_IDR_Addr, 3)  //输入
// #define PB4in   BIT_ADDR(GPIOB_IDR_Addr, 4)  //输入
// #define PB5in   BIT_ADDR(GPIOB_IDR_Addr, 5)  //输入
// #define PB6in   BIT_ADDR(GPIOB_IDR_Addr, 6)  //输入
// #define PB7in   BIT_ADDR(GPIOB_IDR_Addr, 7)  //输入
// #define PB8in   BIT_ADDR(GPIOB_IDR_Addr, 8)  //输入
// #define PB9in   BIT_ADDR(GPIOB_IDR_Addr, 9)  //输入
// #define PB10in  BIT_ADDR(GPIOB_IDR_Addr, 10) //输入
// #define PB11in  BIT_ADDR(GPIOB_IDR_Addr, 11) //输入
// #define PB12in  BIT_ADDR(GPIOB_IDR_Addr, 12) //输入
// #define PB13in  BIT_ADDR(GPIOB_IDR_Addr, 13) //输入
// #define PB14in  BIT_ADDR(GPIOB_IDR_Addr, 14) //输入
// #define PB15in  BIT_ADDR(GPIOB_IDR_Addr, 15) //输入
// //----------------------------------------------------
// #define PC0   BIT_ADDR(GPIOC_ODR_Addr, 0)  //输出
// #define PC1   BIT_ADDR(GPIOC_ODR_Addr, 1)  //输出
// #define PC2   BIT_ADDR(GPIOC_ODR_Addr, 2)  //输出
// #define PC3   BIT_ADDR(GPIOC_ODR_Addr, 3)  //输出
// #define PC4   BIT_ADDR(GPIOC_ODR_Addr, 4)  //输出
// #define PC5   BIT_ADDR(GPIOC_ODR_Addr, 5)  //输出
// #define PC6   BIT_ADDR(GPIOC_ODR_Addr, 6)  //输出
// #define PC7   BIT_ADDR(GPIOC_ODR_Addr, 7)  //输出
// #define PC8   BIT_ADDR(GPIOC_ODR_Addr, 8)  //输出
// #define PC9   BIT_ADDR(GPIOC_ODR_Addr, 9)  //输出
// #define PC10  BIT_ADDR(GPIOC_ODR_Addr, 10) //输出
// #define PC11  BIT_ADDR(GPIOC_ODR_Addr, 11) //输出
// #define PC12  BIT_ADDR(GPIOC_ODR_Addr, 12) //输出
// #define PC13  BIT_ADDR(GPIOC_ODR_Addr, 13) //输出
// #define PC14  BIT_ADDR(GPIOC_ODR_Addr, 14) //输出
// #define PC15  BIT_ADDR(GPIOC_ODR_Addr, 15) //输出

// #define PC0in   BIT_ADDR(GPIOC_IDR_Addr, 0)  //输入
// #define PC1in   BIT_ADDR(GPIOC_IDR_Addr, 1)  //输入
// #define PC2in   BIT_ADDR(GPIOC_IDR_Addr, 2)  //输入
// #define PC3in   BIT_ADDR(GPIOC_IDR_Addr, 3)  //输入
// #define PC4in   BIT_ADDR(GPIOC_IDR_Addr, 4)  //输入
// #define PC5in   BIT_ADDR(GPIOC_IDR_Addr, 5)  //输入
// #define PC6in   BIT_ADDR(GPIOC_IDR_Addr, 6)  //输入
// #define PC7in   BIT_ADDR(GPIOC_IDR_Addr, 7)  //输入
// #define PC8in   BIT_ADDR(GPIOC_IDR_Addr, 8)  //输入
// #define PC9in   BIT_ADDR(GPIOC_IDR_Addr, 9)  //输入
// #define PC10in  BIT_ADDR(GPIOC_IDR_Addr, 10) //输入
// #define PC11in  BIT_ADDR(GPIOC_IDR_Addr, 11) //输入
// #define PC12in  BIT_ADDR(GPIOC_IDR_Addr, 12) //输入
// #define PC13in  BIT_ADDR(GPIOC_IDR_Addr, 13) //输入
// #define PC14in  BIT_ADDR(GPIOC_IDR_Addr, 14) //输入
// #define PC15in  BIT_ADDR(GPIOC_IDR_Addr, 15) //输入

// //----------------------------------------------------
// #define PD0   BIT_ADDR(GPIOD_ODR_Addr, 0)  //输出
// #define PD1   BIT_ADDR(GPIOD_ODR_Addr, 1)  //输出
// #define PD2   BIT_ADDR(GPIOD_ODR_Addr, 2)  //输出
// #define PD3   BIT_ADDR(GPIOD_ODR_Addr, 3)  //输出
// #define PD4   BIT_ADDR(GPIOD_ODR_Addr, 4)  //输出
// #define PD5   BIT_ADDR(GPIOD_ODR_Addr, 5)  //输出
// #define PD6   BIT_ADDR(GPIOD_ODR_Addr, 6)  //输出
// #define PD7   BIT_ADDR(GPIOD_ODR_Addr, 7)  //输出
// #define PD8   BIT_ADDR(GPIOD_ODR_Addr, 8)  //输出
// #define PD9   BIT_ADDR(GPIOD_ODR_Addr, 9)  //输出
// #define PD10  BIT_ADDR(GPIOD_ODR_Addr, 10) //输出
// #define PD11  BIT_ADDR(GPIOD_ODR_Addr, 11) //输出
// #define PD12  BIT_ADDR(GPIOD_ODR_Addr, 12) //输出
// #define PD13  BIT_ADDR(GPIOD_ODR_Addr, 13) //输出
// #define PD14  BIT_ADDR(GPIOD_ODR_Addr, 14) //输出
// #define PD15  BIT_ADDR(GPIOD_ODR_Addr, 15) //输出

// #define PD0in   BIT_ADDR(GPIOD_IDR_Addr, 0)  //输入
// #define PD1in   BIT_ADDR(GPIOD_IDR_Addr, 1)  //输入
// #define PD2in   BIT_ADDR(GPIOD_IDR_Addr, 2)  //输入
// #define PD3in   BIT_ADDR(GPIOD_IDR_Addr, 3)  //输入
// #define PD4in   BIT_ADDR(GPIOD_IDR_Addr, 4)  //输入
// #define PD5in   BIT_ADDR(GPIOD_IDR_Addr, 5)  //输入
// #define PD6in   BIT_ADDR(GPIOD_IDR_Addr, 6)  //输入
// #define PD7in   BIT_ADDR(GPIOD_IDR_Addr, 7)  //输入
// #define PD8in   BIT_ADDR(GPIOD_IDR_Addr, 8)  //输入
// #define PD9in   BIT_ADDR(GPIOD_IDR_Addr, 9)  //输入
// #define PD10in  BIT_ADDR(GPIOD_IDR_Addr, 10) //输入
// #define PD11in  BIT_ADDR(GPIOD_IDR_Addr, 11) //输入
// #define PD12in  BIT_ADDR(GPIOD_IDR_Addr, 12) //输入
// #define PD13in  BIT_ADDR(GPIOD_IDR_Addr, 13) //输入
// #define PD14in  BIT_ADDR(GPIOD_IDR_Addr, 14) //输入
// #define PD15in  BIT_ADDR(GPIOD_IDR_Addr, 15) //输入

// //----------------------------------------------------
// #define PE0   BIT_ADDR(GPIOE_ODR_Addr, 0)  //输出
// #define PE1   BIT_ADDR(GPIOE_ODR_Addr, 1)  //输出
// #define PE2   BIT_ADDR(GPIOE_ODR_Addr, 2)  //输出
// #define PE3   BIT_ADDR(GPIOE_ODR_Addr, 3)  //输出
// #define PE4   BIT_ADDR(GPIOE_ODR_Addr, 4)  //输出
// #define PE5   BIT_ADDR(GPIOE_ODR_Addr, 5)  //输出
// #define PE6   BIT_ADDR(GPIOE_ODR_Addr, 6)  //输出
// #define PE7   BIT_ADDR(GPIOE_ODR_Addr, 7)  //输出
// #define PE8   BIT_ADDR(GPIOE_ODR_Addr, 8)  //输出
// #define PE9   BIT_ADDR(GPIOE_ODR_Addr, 9)  //输出
// #define PE10  BIT_ADDR(GPIOE_ODR_Addr, 10) //输出
// #define PE11  BIT_ADDR(GPIOE_ODR_Addr, 11) //输出
// #define PE12  BIT_ADDR(GPIOE_ODR_Addr, 12) //输出
// #define PE13  BIT_ADDR(GPIOE_ODR_Addr, 13) //输出
// #define PE14  BIT_ADDR(GPIOE_ODR_Addr, 14) //输出
// #define PE15  BIT_ADDR(GPIOE_ODR_Addr, 15) //输出

// #define PE0in   BIT_ADDR(GPIOE_IDR_Addr, 0)  //输入
// #define PE1in   BIT_ADDR(GPIOE_IDR_Addr, 1)  //输入
// #define PE2in   BIT_ADDR(GPIOE_IDR_Addr, 2)  //输入
// #define PE3in   BIT_ADDR(GPIOE_IDR_Addr, 3)  //输入
// #define PE4in   BIT_ADDR(GPIOE_IDR_Addr, 4)  //输入
// #define PE5in   BIT_ADDR(GPIOE_IDR_Addr, 5)  //输入
// #define PE6in   BIT_ADDR(GPIOE_IDR_Addr, 6)  //输入
// #define PE7in   BIT_ADDR(GPIOE_IDR_Addr, 7)  //输入
// #define PE8in   BIT_ADDR(GPIOE_IDR_Addr, 8)  //输入
// #define PE9in   BIT_ADDR(GPIOE_IDR_Addr, 9)  //输入
// #define PE10in  BIT_ADDR(GPIOE_IDR_Addr, 10) //输入
// #define PE11in  BIT_ADDR(GPIOE_IDR_Addr, 11) //输入
// #define PE12in  BIT_ADDR(GPIOE_IDR_Addr, 12) //输入
// #define PE13in  BIT_ADDR(GPIOE_IDR_Addr, 13) //输入
// #define PE14in  BIT_ADDR(GPIOE_IDR_Addr, 14) //输入
// #define PE15in  BIT_ADDR(GPIOE_IDR_Addr, 15) //输入




void WOW_Configuration(void);		//配置函数
void WOW_Server(void);					//服务函数


#endif







