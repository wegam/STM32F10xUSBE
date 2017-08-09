/******************************** User_library *********************************
* 文件名 	: STM32_SDCard.H
* 作者   	: wegam@sina.com
* 版本   	: V
* 日期   	: 2016/01/01
* 说明   	: 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


#include "STM32_SYS.H"
#include "STM32_GPIO.H"


/*******************************************************************************
* 函数名		:	SYS_Configuration	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void SYS_Configuration(void)
{
	RCC_Configuration();
}

/*******************************************************************************
* 函数名		:	RCC_Configuration
* 功能描述	:	系统时钟配置 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;
	/* RCC system reset(for debug purpose) */
	RCC_DeInit();																	//复位系统时钟
	
	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);										//打开外部高速时钟
	
	/* Wait till HSE is ready */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();		//等待外部高速时钟启动
	
	if(HSEStartUpStatus == SUCCESS)
	{
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);	//使能或者失能预取指缓存
		
		/* Flash 2 wait state */
		FLASH_SetLatency(FLASH_Latency_2);					//设置代码延时值
		
		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 						//设置AHB时钟（HCLK）
		
		/* PCLK2 = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1); 						//设置高速AHB时钟（PCLK2）
		
		/* PCLK1 = HCLK/2 */
		RCC_PCLK1Config(RCC_HCLK_Div2);							//设置低速AHB时钟（PCLK1）
		
		/* PLLCLK = 8MHz * 9 = 72 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);	//设置PLL时钟源及倍频系数
		
		/* Enable PLL */ 
		RCC_PLLCmd(ENABLE);			//使能或者失能PLL
		
		/* Wait till PLL is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}
		
		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);	//设置系统时钟源（SYSCLK）
		
		/* Wait till PLL is used as system clock source */
		while(RCC_GetSYSCLKSource() != 0x08)				//返回用作系统时钟的时钟源
		{
		}
	}
	GPIO_DeInitAll();				//将所有的GPIO关闭----V20170605
}




