/******************************** User_library *********************************
* �ļ��� 	: STM32_SDCard.H
* ����   	: wegam@sina.com
* �汾   	: V
* ����   	: 2016/01/01
* ˵��   	: 
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
* ������		:	SYS_Configuration	
* ��������	:	 
* ����		:	
* ���		:
* ���� 		:
*******************************************************************************/
void SYS_Configuration(void)
{
	RCC_Configuration();
}

/*******************************************************************************
* ������		:	RCC_Configuration
* ��������	:	ϵͳʱ������ 
* ����		:	
* ���		:
* ���� 		:
*******************************************************************************/
void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;
	/* RCC system reset(for debug purpose) */
	RCC_DeInit();																	//��λϵͳʱ��
	
	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);										//���ⲿ����ʱ��
	
	/* Wait till HSE is ready */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();		//�ȴ��ⲿ����ʱ������
	
	if(HSEStartUpStatus == SUCCESS)
	{
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);	//ʹ�ܻ���ʧ��Ԥȡָ����
		
		/* Flash 2 wait state */
		FLASH_SetLatency(FLASH_Latency_2);					//���ô�����ʱֵ
		
		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 						//����AHBʱ�ӣ�HCLK��
		
		/* PCLK2 = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1); 						//���ø���AHBʱ�ӣ�PCLK2��
		
		/* PCLK1 = HCLK/2 */
		RCC_PCLK1Config(RCC_HCLK_Div2);							//���õ���AHBʱ�ӣ�PCLK1��
		
		/* PLLCLK = 8MHz * 9 = 72 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);	//����PLLʱ��Դ����Ƶϵ��
		
		/* Enable PLL */ 
		RCC_PLLCmd(ENABLE);			//ʹ�ܻ���ʧ��PLL
		
		/* Wait till PLL is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}
		
		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);	//����ϵͳʱ��Դ��SYSCLK��
		
		/* Wait till PLL is used as system clock source */
		while(RCC_GetSYSCLKSource() != 0x08)				//��������ϵͳʱ�ӵ�ʱ��Դ
		{
		}
	}
	GPIO_DeInitAll();				//�����е�GPIO�ر�----V20170605
}



