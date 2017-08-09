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


#include "STM32_USART.H"

#include "STM32_GPIO.H"
//#include	"stdio.h"			//����printf
//#include	"string.h"			//����printf
//#include	"stdarg.h"			//���ڻ�ȡ��ȷ�������Ĳ���
//#include	"stdlib.h"		//malloc��̬�����ڴ�ռ�
/*##############################################################################
################################################################################
���ò�����ʱ��������
--------------------------------------------------------------------------------
������				|fPCLK = 36MHz	���%			|		fPCLK = 72MHz	���%			|
--------------------------------------------------------------------------------
2400				|						0%						|						0%							
--------------------------------------------------------------------------------
9600				|						0%						|						0%								
--------------------------------------------------------------------------------
19200				|						0%						|						0%							
--------------------------------------------------------------------------------
57600				|						0%						|						0%							
--------------------------------------------------------------------------------
115200			|						0.15%					|						0%							
--------------------------------------------------------------------------------
230400			|						0.16%					|						0.16%							
--------------------------------------------------------------------------------
460800			|						0.16%					|						0.16%							
--------------------------------------------------------------------------------
921600			|						0.16%					|						0.16%							
--------------------------------------------------------------------------------
2250000			|						0%						|						0%							
--------------------------------------------------------------------------------

################################################################################
###############################################################################*/
//u8 Usart_Buffer[32]={0};
//ѡ�񴮿ڿ���
char *USART_BUFFER;
u32	USART_BUFFERSIZE;
/*****************************USART DMA��ʽ�շ������ù���*******************************/
//****1.RCC����λ��ʱ�ӿ��ƼĴ�������ʼ��������GPIO��DMA��USARTʱ�ӡ�********************/ 
//****2.NVIC��Ƕ�������жϿ��ƼĴ�������ʼ������ɸ���Ӳ���жϵ����á�********************/
//****3.USART��ʼ�������ô��ڣ�����DMAͨ���ȡ�********************/ 
//****4.DMA��ʼ�������DMA�����á�********************/

//#define USE_USART USARTSELECT_TypeDef*;
/*****************************USART DMA��ʽ�շ������ù���*******************************/
/*******************************************************************************
*������		: USART_Configuration
*��������	:	��������
*����			:	���ں�	--------USARTx��USART1/USART2/USART3/UART4/UART5
						������	--------USART_BaudRate
						�ֳ�----------USART_WordLength
						ֹͣλ	--------USART_StopBits
						��żУ��------USART_Parity
						����DMAʹ��---UsartDMAState NewState
*���			:	��
*����ֵ		:	��
*����			:	USART_Configuration(USART1,19200,8,1,0);
*******************************************************************************/
void USART_BASIC_Configuration(USART_TypeDef* USARTx,u32 USART_BaudRate,u8 NVICPreemptionPriority,u8 NVIC_SubPriority)	//��������
{

	//1)**********�������
//	USART_InitTypeDef USART_InitStructure;				//USART�ṹ��	
//	GPIO_InitTypeDef GPIO_InitStructure;					//GPIO�ṹ��
//	NVIC_InitTypeDef NVIC_InitStructure; 					//NVIC�ṹ��

//	
//	u16 TXD_Pin=0;																//���ڷ��ͽ�
//	u16 RXD_Pin=0;																//���ڽ��ս�
//	GPIO_TypeDef* GPIO_TX=0;
//	GPIO_TypeDef* GPIO_RX=0;
//	u8 USARTx_IRQChannel=0;
	
	
	//2)**********���ڻ�������
	USART_Set(USARTx,USART_BaudRate,NVICPreemptionPriority,NVIC_SubPriority);				//���ݴ��ں�����������źͷ����ж��ź�Դ
	
//	USART_Cmd(USARTx, ENABLE);
	//4)**********���ڽ����жϿ�
	//--��DMA����δ��������ʹ�ô��ڽ����ж�
	//--��DMA���տ��������ڽ����ж�Ӧ�ùرգ���DMA�����лὫ���ڽ����жϹر�
//	USART_ITConfig(USARTx,USART_IT_RXNE, ENABLE);		//�����ж�--��
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);		//�����ж�--��
//	USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);     //���տ����ж�ʹ��
	USART_ClearITPendingBit(USARTx,USART_IT_IDLE); 	//������д��ڱ�־λ
//	USART_ClearITPendingBit(USARTx,USART_IT_IDLE); 	//������д��ڱ�־λ
	/* ����DMA��ʽ����*/
	USART_DMACmd(USARTx,USART_DMAReq_Rx,ENABLE);
	/* ����DMA��ʽ����*/
	USART_DMACmd(USARTx,USART_DMAReq_Tx,ENABLE);
	//5)**********ʹ�ܴ���
	USART_Cmd(USARTx, ENABLE);
}
/*******************************************************************************
*������		:	USART_DMA_Configuration
*��������	:	DMA�������ã���Ҫ�����ڽ����жϹرգ�Ȼ�������ڿ����жϣ�����DMA�Զ����մ�������
*����			:	RXDBuffer--���ջ������������뷢�ͻ���������,
						TXDBuffer--���ͻ���������������ջ���������,
						BufferSize--�����С
*���			:	��
*����ֵ		:	��
*����			:
*******************************************************************************/
void USART_DMA_Configuration(USART_TypeDef* USARTx,u32 USART_BaudRate,u8 NVICPreemptionPriority,u8 NVIC_SubPriority,u32 *RXDBuffer,u32 *TXDBuffer,u32 BufferSize)	//USART_DMA����
{
	//1)**********�������	
	DMA_InitTypeDef DMA_Initstructure;
	NVIC_InitTypeDef NVIC_Initstructure;
	
	DMA_Channel_TypeDef* DMAx_Channeltx=0;			//DMA����ͨ�������ź�---��DMA���ڷ����������ʱ���ᷢ��DMA�ж�
	DMA_Channel_TypeDef* DMAx_Channelrx=0;			//DMA����ͨ�������ź�---DMA���ڽ����ɴ��ڷ����жϣ���˴˴�����ͨ���жϲ�ʹ��
	u8 DMAx_Channelx_IRQChannel=0;							//DMA�ж�Դ
	u32 DMAx_FLAG_GLtx=0;												//DMA���ڷ����ж�ȫ�ֱ���			
	u32 DMAx_FLAG_GLrx=0;												//DMA���ڽ����ж�ȫ�ֱ���
	
	USART_BUFFERSIZE=BufferSize;
	
//	//2)**********�رմ��ڵȴ�DMA�������	
//	USART_Cmd(USARTx, DISABLE);
	
//	//3)**********�رմ��ڵȴ�DMA�������
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);	
	//2)**********���ڻ�������
	USART_Set(USARTx,USART_BaudRate,NVICPreemptionPriority,NVIC_SubPriority);				//���ݴ��ں�����������źͷ����ж��ź�Դ
	//4)**********���ݴ����������DMAͨ������������
	if(USARTx==USART1)
	{
		DMAx_Channeltx=DMA1_Channel4;
		DMAx_Channelrx=DMA1_Channel5;
		DMAx_Channelx_IRQChannel=DMA1_Channel4_IRQChannel;
		DMAx_FLAG_GLtx=DMA1_FLAG_GL4;
		DMAx_FLAG_GLrx=DMA1_FLAG_GL5;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);	
	}
	else if(USARTx==USART2)
	{
		DMAx_Channeltx=DMA1_Channel7;
		DMAx_Channelrx=DMA1_Channel6;
		DMAx_Channelx_IRQChannel=DMA1_Channel7_IRQChannel;
		DMAx_FLAG_GLtx=DMA1_FLAG_GL7;
		DMAx_FLAG_GLrx=DMA1_FLAG_GL6;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);	
	}
	else if(USARTx==USART3)
	{
		DMAx_Channeltx=DMA1_Channel2;
		DMAx_Channelrx=DMA1_Channel3;
		DMAx_Channelx_IRQChannel=DMA1_Channel2_IRQChannel;
		DMAx_FLAG_GLtx=DMA1_FLAG_GL2;
		DMAx_FLAG_GLrx=DMA1_FLAG_GL3;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);	
	}
	else if(USARTx==UART4)
	{
		DMAx_Channeltx=DMA2_Channel5;
		DMAx_Channelrx=DMA2_Channel3;
		DMAx_Channelx_IRQChannel=DMA2_Channel3_IRQChannel;
		DMAx_FLAG_GLtx=DMA2_FLAG_GL5;
		DMAx_FLAG_GLrx=DMA2_FLAG_GL3;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);	
	}
	else if(USARTx==UART5)
	{
	}
	//5)**********DMA���ͳ�ʼ����������ΪDMA��Ŀ�Ķ�
	DMA_Initstructure.DMA_PeripheralBaseAddr =  (u32)(&USARTx->DR);					//DMA����Դ��ַ
	DMA_Initstructure.DMA_MemoryBaseAddr     = (u32)TXDBuffer;							//DMA�����ڴ��ַ
	DMA_Initstructure.DMA_DIR = DMA_DIR_PeripheralDST;											//DMA_DIR_PeripheralDST��������ΪDMA��Ŀ�Ķˣ���DMA_DIR_PeripheralSRC��������Ϊ���ݴ������Դ��
	DMA_Initstructure.DMA_BufferSize = USART_BUFFERSIZE; 													//ָ��DMAͨ����DMA����Ĵ�С
//	DMA_Initstructure.DMA_BufferSize = sizeof(MemoryAddr[0]); 						//ָ��DMAͨ����DMA����Ĵ�С
	DMA_Initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//DMA_PeripheralInc_Enable�������ַ�Ĵ�����������DMA_PeripheralInc_Disable�������ַ�Ĵ������䣩��
	DMA_Initstructure.DMA_MemoryInc =DMA_MemoryInc_Enable;									//DMA_MemoryInc_Enable���ڴ��ַ�Ĵ�����������DMA_MemoryInc_Disable���ڴ��ַ�Ĵ������䣩
	DMA_Initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//�������ݿ���--DMA_PeripheralDataSize_Byte�����ݿ���Ϊ8λ����DMA_PeripheralDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_PeripheralDataSize_Word�����ݿ���Ϊ32λ��
	DMA_Initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//�ڴ����ݿ���--DMA_MemoryDataSize_Byte�����ݿ���Ϊ8λ����DMA_MemoryDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_MemoryDataSize_Word�����ݿ���Ϊ32λ��
	DMA_Initstructure.DMA_Mode = DMA_Mode_Normal;														//DMA����ģʽ--DMA_Mode_Normal��ֻ����һ�Σ�, DMA_Mode_Circular����ͣ�ش��ͣ�
	DMA_Initstructure.DMA_Priority = DMA_Priority_High; 										//DMAͨ����ת�����ȼ�--DMA_Priority_VeryHigh���ǳ��ߣ�DMA_Priority_High����)��DMA_Priority_Medium���У���DMA_Priority_Low���ͣ�
	DMA_Initstructure.DMA_M2M = DMA_M2M_Disable;														//DMAͨ�����ڴ浽�ڴ洫��--DMA_M2M_Enable(����Ϊ�ڴ浽�ڴ洫��)��DMA_M2M_Disable�����ڴ浽�ڴ洫�䣩
	DMA_Init(DMAx_Channeltx,&DMA_Initstructure);														//��ʼ��DMA

	//6)**********DMA���ճ�ʼ����������ΪDMA��Դ��
	DMA_Initstructure.DMA_PeripheralBaseAddr =  (u32)(&USARTx->DR);					//DMA����Դ��ַ
	DMA_Initstructure.DMA_MemoryBaseAddr     = 	(u32)RXDBuffer;						//DMA�����ڴ��ַ
	DMA_Initstructure.DMA_DIR = DMA_DIR_PeripheralSRC;											//DMA_DIR_PeripheralDST��������ΪDMA��Ŀ�Ķˣ���DMA_DIR_PeripheralSRC��������Ϊ���ݴ������Դ��
	DMA_Initstructure.DMA_BufferSize = USART_BUFFERSIZE; 													//ָ��DMAͨ����DMA����Ĵ�С
//	DMA_Initstructure.DMA_BufferSize = sizeof(MemoryAddr); 								//ָ��DMAͨ����DMA����Ĵ�С
	DMA_Initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//DMA_PeripheralInc_Enable�������ַ�Ĵ�����������DMA_PeripheralInc_Disable�������ַ�Ĵ������䣩��
	DMA_Initstructure.DMA_MemoryInc =DMA_MemoryInc_Enable;									//DMA_MemoryInc_Enable���ڴ��ַ�Ĵ�����������DMA_MemoryInc_Disable���ڴ��ַ�Ĵ������䣩
	DMA_Initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//�������ݿ���--DMA_PeripheralDataSize_Byte�����ݿ���Ϊ8λ����DMA_PeripheralDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_PeripheralDataSize_Word�����ݿ���Ϊ32λ��
	DMA_Initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//�ڴ����ݿ���--DMA_MemoryDataSize_Byte�����ݿ���Ϊ8λ����DMA_MemoryDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_MemoryDataSize_Word�����ݿ���Ϊ32λ��
	DMA_Initstructure.DMA_Mode = DMA_Mode_Normal;														//DMA����ģʽ--DMA_Mode_Normal��ֻ����һ�Σ�, DMA_Mode_Circular����ͣ�ش��ͣ�
	DMA_Initstructure.DMA_Priority = DMA_Priority_High; 										//DMAͨ����ת�����ȼ�--DMA_Priority_VeryHigh���ǳ��ߣ�DMA_Priority_High����)��DMA_Priority_Medium���У���DMA_Priority_Low���ͣ�
	DMA_Initstructure.DMA_M2M = DMA_M2M_Disable;														//DMAͨ�����ڴ浽�ڴ洫��--DMA_M2M_Enable(����Ϊ�ڴ浽�ڴ洫��)��DMA_M2M_Disable�����ڴ浽�ڴ洫�䣩
	DMA_Init(DMAx_Channelrx,&DMA_Initstructure);														//��ʼ��DMA
	
	//7)**********DMAͨ���жϳ�ʼ��---��ΪDMA�����ж�----DMA��������ж�
	NVIC_Initstructure.NVIC_IRQChannel = DMAx_Channelx_IRQChannel;       		//ͨ������Ϊ����1�ж�
	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority=NVICPreemptionPriority;
	NVIC_Initstructure.NVIC_IRQChannelSubPriority = NVIC_SubPriority; 			//�ж���Ӧ���ȼ�0	
	NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;        									//���ж�
	NVIC_Init(&NVIC_Initstructure);
	
	
	//8)**********��������ж�
	//8.1)**********���ڽ����ж�����
	//--�����ڽ����жϹرգ�Ȼ�������ڿ����жϣ�����DMA�Զ����մ�������
	//--��DMA����δ��������ʹ�ô��ڽ����ж�
	//--��DMA���տ��������ڽ����ж�Ӧ�ùرգ���DMA�����лὫ���ڽ����жϹر�
//	USART_ITConfig(USARTx,USART_IT_RXNE, DISABLE);				//�����ж�--�أ���ΪDMA�Զ����գ�
	
	/* ����DMA1ͨ��5*/
	DMA_Cmd(DMAx_Channeltx,DISABLE);
//	DMA_Cmd(DMA1_Channel7,DISABLE);									//DMA���Ϳ���
	//10.2)**********ʹ�ܴ���
	DMA_Cmd(DMAx_Channelrx,ENABLE);	
	//9.1)**********�ر�DMA����	
	
	
	//8.2)**********ʹ�ܴ���DMA��ʽ����
	USART_DMACmd(USARTx,USART_DMAReq_Rx,ENABLE);
	//8.3)**********ʹ�ܴ���DMA��ʽ����
	USART_DMACmd(USARTx,USART_DMAReq_Tx,ENABLE);
	//9)**********�������жϱ�־λ	
	//	DMA_Cmd(DMAx_Channeltx,ENABLE);
	//9.2)**********ʹ�����DMAͨ����������ж�
	DMA_ITConfig(DMAx_Channeltx,DMA_IT_TC, ENABLE);
	//9.3)**********�������DMA��ʽ�����ж�ȫ�ֱ�־
	DMA_ClearFlag(DMAx_FLAG_GLtx);                                 					// ���DMA���б�־
	//9.3)**********�������DMA��ʽ�����ж�ȫ�ֱ�־
	DMA_ClearFlag(DMAx_FLAG_GLrx);                                 					// ���DMA���б�־	
	//10.1)**********��������DMA��ʽ����	
	
	USART_ITConfig(USARTx,USART_IT_IDLE, ENABLE);					//ʹ�ÿ����жϣ�DMA�Զ����գ���⵽���߿��б�ʾ���Ͷ��Ѿ�������ɣ����ݱ�����DMA��������
	USART_ClearITPendingBit(USARTx,USART_IT_IDLE); 				//������д��ڱ�־λ
	
	USART_Cmd(USARTx, ENABLE);
}
/*******************************************************************************
*������		:USART_Set
*��������	:���ڻ�������
*����			: 
*���			:��
*����ֵ		:��
*����			:
*******************************************************************************/
void USART_Set(USART_TypeDef* USARTx,u32 USART_BaudRate,u8 NVICPreemptionPriority,u8 NVIC_SubPriority)
{
	//1)**********�������
	USART_InitTypeDef USART_InitStructure;				//USART�ṹ��	
	GPIO_InitTypeDef GPIO_InitStructure;					//GPIO�ṹ��
	NVIC_InitTypeDef NVIC_InitStructure; 					//NVIC�ṹ��

	
	u16 TXD_Pin=0;																//���ڷ��ͽ�
	u16 RXD_Pin=0;																//���ڽ��ս�
	GPIO_TypeDef* GPIO_TX=0;
	GPIO_TypeDef* GPIO_RX=0;
	u8 USARTx_IRQChannel=0;
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);				//�ر�AFIOʱ��,Ϊ�ر�JTAG����

//>>>>>>>>>>>>>>>>>>>>>��ʼ������-��ʼ>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	//2)**********��ʼ���������������ʱ��
	//2.1)**********USART1
	if(USARTx==USART1)
	{
		TXD_Pin=GPIO_Pin_9;											//USART1-TX>PA9
		RXD_Pin=GPIO_Pin_10;										//USART1-RX>PA10
		
		GPIO_TX=GPIOA;
		GPIO_RX=GPIOA;
		
		USARTx_IRQChannel=USART1_IRQChannel;		//�ж�
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//USART1ʱ�ӿ���

	}
	//2.2)**********USART2
	else if(USARTx==USART2)
	{
		TXD_Pin=GPIO_Pin_2;		//USART2-TX>PA2
		RXD_Pin=GPIO_Pin_3;		//USART2-RX>PA3
		
		GPIO_TX=GPIOA;
		GPIO_RX=GPIOA;
		
		USARTx_IRQChannel=USART2_IRQChannel;		//�ж�
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//USART1ʱ�ӿ���

	}
	//2.3)**********USART3
	else if(USARTx==USART3)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);				//�ر�AFIOʱ��,Ϊ�ر�JTAG����
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //�ر�JTAG����
//		GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);  		//�ر�SWD����
		
		
		TXD_Pin=GPIO_Pin_10;	//USART3-TX>PB10
		RXD_Pin=GPIO_Pin_11;	//USART3-RX>PB11
		
		GPIO_TX=GPIOB;
		GPIO_RX=GPIOB;
		
		USARTx_IRQChannel=USART3_IRQChannel;		//�ж�
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//USART1ʱ�ӿ���
		

	}
	//2.4)**********USART4
	else if(USARTx==UART4)
	{
		TXD_Pin=GPIO_Pin_10;	//USART1-TX>PC10
		RXD_Pin=GPIO_Pin_11;	//USART1-RX>PC11
		
		GPIO_TX=GPIOC;
		GPIO_RX=GPIOC;
		
		USARTx_IRQChannel=UART4_IRQChannel;		//�ж�
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	//USART1ʱ�ӿ���

	}
	//2.5)**********USART5
	else if(USARTx==UART5)
	{
		TXD_Pin=GPIO_Pin_12;	//USART1-TX>PC12
		RXD_Pin=GPIO_Pin_2;		//USART1-RX>PD2
		
		GPIO_TX=GPIOC;
		GPIO_RX=GPIOD;
		
		USARTx_IRQChannel=UART5_IRQChannel;		//�ж�
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);	//USART1ʱ�ӿ���
	}

//<<<<<<<<<<<<<<<<<<<<<��ʼ������-����<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	//3.1)**********��ʼ��TXD����
	GPIO_InitStructure.GPIO_Pin = TXD_Pin;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_TX,&GPIO_InitStructure);

	//3.2)**********��ʼ��RXD����
	GPIO_InitStructure.GPIO_Pin = RXD_Pin;
	//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;							//��������
	GPIO_Init(GPIO_RX,&GPIO_InitStructure);
	

	//4)**********����ȫ���жϳ�ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVICPreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=NVIC_SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//3)**********��ʼ��USART
	//���ò���
	USART_DeInit(USARTx);
	USART_InitStructure.USART_BaudRate = USART_BaudRate; 					//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//����λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;				//ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No ; 					//��żУ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//����
	USART_Init(USARTx, &USART_InitStructure);											//��ʼ������
	
//	//4)**********�����ж��ź�Դ
//	return 0;

	
}
/*******************************************************************************
*������		:USART_DMASet
*��������	:���ڽ��շ�����
*����			: 
*���			:��
*����ֵ		:��
*����			:
*******************************************************************************/
void USART_DMASet(USART_TypeDef* USARTx,u32 USART_BaudRate,u8 NVICPreemptionPriority,u8 NVIC_SubPriority,u32 *RXDBuffer,u32 *TXDBuffer,u32 BufferSize)	//USART_DMA����
{
	//1)**********�������	
	DMA_InitTypeDef DMA_Initstructure;
	NVIC_InitTypeDef NVIC_Initstructure;
	
	DMA_Channel_TypeDef* DMAx_Channeltx=0;			//DMA����ͨ�������ź�---��DMA���ڷ����������ʱ���ᷢ��DMA�ж�
	DMA_Channel_TypeDef* DMAx_Channelrx=0;			//DMA����ͨ�������ź�---DMA���ڽ����ɴ��ڷ����жϣ���˴˴�����ͨ���жϲ�ʹ��
	u8 DMAx_Channelx_IRQChannel=0;							//DMA�ж�Դ
	u32 DMAx_FLAG_GLtx=0;												//DMA���ڷ����ж�ȫ�ֱ���			
	u32 DMAx_FLAG_GLrx=0;												//DMA���ڽ����ж�ȫ�ֱ���
	
//	//2)**********�رմ��ڵȴ�DMA�������	
//	USART_Cmd(USARTx, DISABLE);
	
//	//3)**********�رմ��ڵȴ�DMA�������
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);	
	//2)**********���ڻ�������
	USART_Set(USARTx,USART_BaudRate,NVICPreemptionPriority,NVIC_SubPriority);				//���ݴ��ں�����������źͷ����ж��ź�Դ
	//4)**********���ݴ����������DMAͨ������������
	if(USARTx==USART1)
	{
		DMAx_Channeltx=DMA1_Channel4;
		DMAx_Channelrx=DMA1_Channel5;
		DMAx_Channelx_IRQChannel=DMA1_Channel4_IRQChannel;
		DMAx_FLAG_GLtx=DMA1_FLAG_GL4;
		DMAx_FLAG_GLrx=DMA1_FLAG_GL5;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);	
	}
	else if(USARTx==USART2)
	{
		DMAx_Channeltx=DMA1_Channel7;
		DMAx_Channelrx=DMA1_Channel6;
		DMAx_Channelx_IRQChannel=DMA1_Channel7_IRQChannel;
		DMAx_FLAG_GLtx=DMA1_FLAG_GL7;
		DMAx_FLAG_GLrx=DMA1_FLAG_GL6;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);	
	}
	else if(USARTx==USART3)
	{
		DMAx_Channeltx=DMA1_Channel2;
		DMAx_Channelrx=DMA1_Channel3;
		DMAx_Channelx_IRQChannel=DMA1_Channel2_IRQChannel;
		DMAx_FLAG_GLtx=DMA1_FLAG_GL2;
		DMAx_FLAG_GLrx=DMA1_FLAG_GL3;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);	
	}
	else if(USARTx==UART4)
	{
		DMAx_Channeltx=DMA2_Channel5;
		DMAx_Channelrx=DMA2_Channel3;
		DMAx_Channelx_IRQChannel=DMA2_Channel3_IRQChannel;
		DMAx_FLAG_GLtx=DMA2_FLAG_GL5;
		DMAx_FLAG_GLrx=DMA2_FLAG_GL3;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);	
	}
	else if(USARTx==UART5)
	{
	}
	//5)**********DMA���ͳ�ʼ����������ΪDMA��Ŀ�Ķ�
	DMA_Initstructure.DMA_PeripheralBaseAddr =  (u32)(&USARTx->DR);					//DMA����Դ��ַ
	DMA_Initstructure.DMA_MemoryBaseAddr     = (u32)TXDBuffer;							//DMA�����ڴ��ַ
	DMA_Initstructure.DMA_DIR = DMA_DIR_PeripheralDST;											//DMA_DIR_PeripheralDST��������ΪDMA��Ŀ�Ķˣ���DMA_DIR_PeripheralSRC��������Ϊ���ݴ������Դ��
	DMA_Initstructure.DMA_BufferSize = BufferSize; 													//ָ��DMAͨ����DMA����Ĵ�С
//	DMA_Initstructure.DMA_BufferSize = sizeof(MemoryAddr[0]); 						//ָ��DMAͨ����DMA����Ĵ�С
	DMA_Initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//DMA_PeripheralInc_Enable�������ַ�Ĵ�����������DMA_PeripheralInc_Disable�������ַ�Ĵ������䣩��
	DMA_Initstructure.DMA_MemoryInc =DMA_MemoryInc_Enable;									//DMA_MemoryInc_Enable���ڴ��ַ�Ĵ�����������DMA_MemoryInc_Disable���ڴ��ַ�Ĵ������䣩
	DMA_Initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//�������ݿ���--DMA_PeripheralDataSize_Byte�����ݿ���Ϊ8λ����DMA_PeripheralDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_PeripheralDataSize_Word�����ݿ���Ϊ32λ��
	DMA_Initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//�ڴ����ݿ���--DMA_MemoryDataSize_Byte�����ݿ���Ϊ8λ����DMA_MemoryDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_MemoryDataSize_Word�����ݿ���Ϊ32λ��
	DMA_Initstructure.DMA_Mode = DMA_Mode_Normal;														//DMA����ģʽ--DMA_Mode_Normal��ֻ����һ�Σ�, DMA_Mode_Circular����ͣ�ش��ͣ�
	DMA_Initstructure.DMA_Priority = DMA_Priority_High; 										//DMAͨ����ת�����ȼ�--DMA_Priority_VeryHigh���ǳ��ߣ�DMA_Priority_High����)��DMA_Priority_Medium���У���DMA_Priority_Low���ͣ�
	DMA_Initstructure.DMA_M2M = DMA_M2M_Disable;														//DMAͨ�����ڴ浽�ڴ洫��--DMA_M2M_Enable(����Ϊ�ڴ浽�ڴ洫��)��DMA_M2M_Disable�����ڴ浽�ڴ洫�䣩
	DMA_Init(DMAx_Channeltx,&DMA_Initstructure);														//��ʼ��DMA

	//6)**********DMA���ճ�ʼ����������ΪDMA��Դ��
	DMA_Initstructure.DMA_PeripheralBaseAddr =  (u32)(&USARTx->DR);					//DMA����Դ��ַ
	DMA_Initstructure.DMA_MemoryBaseAddr     = 	(u32)RXDBuffer;						//DMA�����ڴ��ַ
	DMA_Initstructure.DMA_DIR = DMA_DIR_PeripheralSRC;											//DMA_DIR_PeripheralDST��������ΪDMA��Ŀ�Ķˣ���DMA_DIR_PeripheralSRC��������Ϊ���ݴ������Դ��
	DMA_Initstructure.DMA_BufferSize = BufferSize; 													//ָ��DMAͨ����DMA����Ĵ�С
//	DMA_Initstructure.DMA_BufferSize = sizeof(MemoryAddr); 								//ָ��DMAͨ����DMA����Ĵ�С
	DMA_Initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//DMA_PeripheralInc_Enable�������ַ�Ĵ�����������DMA_PeripheralInc_Disable�������ַ�Ĵ������䣩��
	DMA_Initstructure.DMA_MemoryInc =DMA_MemoryInc_Enable;									//DMA_MemoryInc_Enable���ڴ��ַ�Ĵ�����������DMA_MemoryInc_Disable���ڴ��ַ�Ĵ������䣩
	DMA_Initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//�������ݿ���--DMA_PeripheralDataSize_Byte�����ݿ���Ϊ8λ����DMA_PeripheralDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_PeripheralDataSize_Word�����ݿ���Ϊ32λ��
	DMA_Initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//�ڴ����ݿ���--DMA_MemoryDataSize_Byte�����ݿ���Ϊ8λ����DMA_MemoryDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_MemoryDataSize_Word�����ݿ���Ϊ32λ��
	DMA_Initstructure.DMA_Mode = DMA_Mode_Normal;														//DMA����ģʽ--DMA_Mode_Normal��ֻ����һ�Σ�, DMA_Mode_Circular����ͣ�ش��ͣ�
	DMA_Initstructure.DMA_Priority = DMA_Priority_High; 										//DMAͨ����ת�����ȼ�--DMA_Priority_VeryHigh���ǳ��ߣ�DMA_Priority_High����)��DMA_Priority_Medium���У���DMA_Priority_Low���ͣ�
	DMA_Initstructure.DMA_M2M = DMA_M2M_Disable;														//DMAͨ�����ڴ浽�ڴ洫��--DMA_M2M_Enable(����Ϊ�ڴ浽�ڴ洫��)��DMA_M2M_Disable�����ڴ浽�ڴ洫�䣩
	DMA_Init(DMAx_Channelrx,&DMA_Initstructure);														//��ʼ��DMA
	
	//7)**********DMAͨ���жϳ�ʼ��---��ΪDMA�����ж�----DMA��������ж�
	NVIC_Initstructure.NVIC_IRQChannel = DMAx_Channelx_IRQChannel;       		//ͨ������Ϊ����1�ж�
	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority=NVICPreemptionPriority;
	NVIC_Initstructure.NVIC_IRQChannelSubPriority = NVIC_SubPriority; 			//�ж���Ӧ���ȼ�0	
	NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;        									//���ж�
	NVIC_Init(&NVIC_Initstructure);
	
	
	//8)**********��������ж�
	//8.1)**********���ڽ����ж�����
	//--�����ڽ����жϹرգ�Ȼ�������ڿ����жϣ�����DMA�Զ����մ�������
	//--��DMA����δ��������ʹ�ô��ڽ����ж�
	//--��DMA���տ��������ڽ����ж�Ӧ�ùرգ���DMA�����лὫ���ڽ����жϹر�
//	USART_ITConfig(USARTx,USART_IT_RXNE, DISABLE);				//�����ж�--�أ���ΪDMA�Զ����գ�
	USART_ITConfig(USARTx,USART_IT_IDLE, ENABLE);					//ʹ�ÿ����жϣ�DMA�Զ����գ���⵽���߿��б�ʾ���Ͷ��Ѿ�������ɣ����ݱ�����DMA��������
	USART_ClearITPendingBit(USARTx,USART_IT_IDLE); 				//������д��ڱ�־λ

	//8.2)**********ʹ�ܴ���DMA��ʽ����
	USART_DMACmd(USARTx,USART_DMAReq_Rx,ENABLE);
	//8.3)**********ʹ�ܴ���DMA��ʽ����
	USART_DMACmd(USARTx,USART_DMAReq_Tx,ENABLE);
	//9)**********�������жϱ�־λ	
	//	DMA_Cmd(DMAx_Channeltx,ENABLE);
	//9.2)**********ʹ�����DMAͨ����������ж�
	DMA_ITConfig(DMAx_Channeltx,DMA_IT_TC, ENABLE);
	//9.3)**********�������DMA��ʽ�����ж�ȫ�ֱ�־
	DMA_ClearFlag(DMAx_FLAG_GLtx);                                 					// ���DMA���б�־
	//9.3)**********�������DMA��ʽ�����ж�ȫ�ֱ�־
	DMA_ClearFlag(DMAx_FLAG_GLrx);                                 					// ���DMA���б�־	
	//10.1)**********��������DMA��ʽ����
	/* ����DMA1ͨ��5*/
	DMA_Cmd(DMAx_Channelrx,ENABLE);	
	//9.1)**********�ر�DMA����
	DMA_Cmd(DMAx_Channeltx,ENABLE);
	//10.2)**********ʹ�ܴ���
	USART_Cmd(USARTx, ENABLE);
	

}
/*******************************************************************************
*������		: function
*��������	:	���ڽ��շ�����
*����			: 
*���			:	��
*����ֵ		:	��
*����			:	USART_DMASend(USART2,"����ENG=%d\n",num);
*�ر�˵��	:	��DMA������ɺ���Ҫ�ͷŶ�̬�ռ䣬free(USART_BUFFER);
					:	USART_BUFFER������STM32_USART.H
*******************************************************************************/
int USART_DMAPrintf(USART_TypeDef* USARTx,const char *format,...)		//��ߵ�ʡ�Ժž��ǿɱ����
{
	
//		va_list ap; 										//VA_LIST ����C�����н����������һ��꣬����ͷ�ļ���#include <stdarg.h>,���ڻ�ȡ��ȷ�������Ĳ���
//    static char string[ 256 ];			//�������飬
//    va_start( ap, format );
//    vsprintf( string , format, ap );    
//    va_end( ap );

	//1)**********��ȡ���ݿ���
	u16 num=strlen((const char*)format);		//��ȡ���ݿ���
	//2)**********���建������С����
	unsigned int BufferSize;
	//3)**********argsΪ�����һ��ָ��ɱ�����ı�����va_list�Լ��±�Ҫ�õ���va_start,va_end�������ڶ��壬�ɱ���������б���Ҫ�õ��꣬ ��stdarg.hͷ�ļ��ж���
	va_list args;                                        
	//4)**********���붯̬�ռ�
	USART_BUFFER = (char*)malloc(sizeof(char) * num);	
	//5)**********��ʼ��args�ĺ�����ʹ��ָ��ɱ�����ĵ�һ��������format�ǿɱ������ǰһ������
	va_start(args, format);
	//6)**********��������·��������ִ��ĳ���(��ȥ\0),����������ظ�ֵ
	BufferSize = vsprintf(USART_BUFFER, format, args);
	//7)**********�����ɱ�����Ļ�ȡ
	va_end(args);                                      		
	//8)**********���ȷ��ͻ�������С�����ݸ���������������ַ����DMA��������
	//8)**********DMA������ɺ�ע��Ӧ���ͷŻ�������free(USART_BUFFER);
	if(USARTx==USART1)
	{
		DMA1_Channel4->CNDTR =BufferSize;										//�趨�����ͻ�������С
		DMA1_Channel4->CMAR =(u32)USART_BUFFER;							//���ͻ�����	
		DMA_Cmd(DMA1_Channel4,ENABLE);											//DMA���Ϳ���3
	}
	else if(USARTx==USART2)
	{
		DMA1_Channel7->CNDTR =BufferSize;										//�趨�����ͻ�������С
		DMA1_Channel7->CMAR =(u32)USART_BUFFER;							//���ͻ�����	
		DMA_Cmd(DMA1_Channel7,ENABLE);											//DMA���Ϳ���3
	}
	else if(USARTx==USART3)
	{
		DMA1_Channel2->CNDTR =BufferSize;										//�趨�����ͻ�������С
		DMA1_Channel2->CMAR =(u32)USART_BUFFER;							//���ͻ�����	
		DMA_Cmd(DMA1_Channel2,ENABLE);											//DMA���Ϳ���3
	}
	else if(USARTx==UART4)
	{
		DMA1_Channel5->CNDTR =BufferSize;										//�趨�����ͻ�������С
		DMA1_Channel5->CMAR =(u32)USART_BUFFER;							//���ͻ�����	
		DMA_Cmd(DMA1_Channel5,ENABLE);											//DMA���Ϳ���3
	}
//	else if(USARTx==UART5)
//	{
//		DMA1_Channel7->CNDTR =BufferSize;										//�趨�����ͻ�������С
//		DMA1_Channel7->CMAR =(u32)USART_BUFFER;							//���ͻ�����	
//		DMA_Cmd(DMA1_Channel7,ENABLE);											//DMA���Ϳ���3
//	}
	
	return BufferSize;

}
/*******************************************************************************
*������		: function
*��������	:	���ڽ��շ�����
*����			: 
*���			:	��
*����ֵ		:	��
*����			:	USART_DMASend(USART2,"����ENG=%d\n",num);
*�ر�˵��	:	��DMA������ɺ���Ҫ�ͷŶ�̬�ռ䣬free(USART_BUFFER);
					:	USART_BUFFER������STM32_USART.H
*******************************************************************************/
int USART_DMASend(USART_TypeDef* USARTx,u32 *USART_BUFFER,u16 BufferSize)		//
{
                                  		
	//8)**********���ȷ��ͻ�������С�����ݸ���������������ַ����DMA��������
	//8)**********DMA������ɺ�ע��Ӧ���ͷŻ�������free(USART_BUFFER);
	if(USARTx==USART1)
	{
		DMA1_Channel4->CNDTR =BufferSize;										//�趨�����ͻ�������С
		DMA1_Channel4->CMAR =(u32)USART_BUFFER;							//���ͻ�����	
		DMA_Cmd(DMA1_Channel4,ENABLE);											//DMA���Ϳ���3
	}
	else if(USARTx==USART2)
	{
		DMA1_Channel7->CNDTR =BufferSize;										//�趨�����ͻ�������С
		DMA1_Channel7->CMAR =(u32)USART_BUFFER;							//���ͻ�����	
		DMA_Cmd(DMA1_Channel7,ENABLE);											//DMA���Ϳ���3
	}
	else if(USARTx==USART3)
	{
		DMA1_Channel2->CNDTR =BufferSize;										//�趨�����ͻ�������С
		DMA1_Channel2->CMAR =(u32)USART_BUFFER;							//���ͻ�����	
		DMA_Cmd(DMA1_Channel2,ENABLE);											//DMA���Ϳ���3
	}
	else if(USARTx==UART4)
	{
		DMA1_Channel5->CNDTR =BufferSize;										//�趨�����ͻ�������С
		DMA1_Channel5->CMAR =(u32)USART_BUFFER;							//���ͻ�����	
		DMA_Cmd(DMA1_Channel5,ENABLE);											//DMA���Ϳ���3
	}
//	else if(USARTx==UART5)
//	{
//		DMA1_Channel7->CNDTR =BufferSize;										//�趨�����ͻ�������С
//		DMA1_Channel7->CMAR =(u32)USART_BUFFER;							//���ͻ�����	
//		DMA_Cmd(DMA1_Channel7,ENABLE);											//DMA���Ϳ���3
//	}
	
	return BufferSize;

}
/*******************************************************************************
*������		:	USART_RX_DMAFlagClear
*��������	:	�������DMA�����жϱ�־
						->USART_IT_IDLE:����⵽���߿���ʱ����λ��Ӳ����λ�����USART_CR1�е�IDLEIEΪ��1����������жϡ����������������λ(�ȶ�USART_SR��Ȼ���USART_DR)��
*����			: 
*���			:	��
*����ֵ		:	��
*����			:
*******************************************************************************/
u32 USART_RX_FlagClear(USART_TypeDef* USARTx)				//������ڽ����жϱ�־
{
//	ErrorStatus status = ERROR;
	u32 num=0;

	if(USARTx==USART1)
	{
		if(USART_GetITStatus(USART1,USART_IT_IDLE))
		{
			USART_ClearITPendingBit(USART1,USART_IT_IDLE); 									//������д��ڱ�־λ
			DMA_Cmd(DMA1_Channel5,DISABLE);    														//�رս���DMA
			num = USART1->DR; 																							//������������������־			
			num = USART_BUFFERSIZE -  DMA_GetCurrDataCounter(DMA1_Channel5);	//�õ������������ݸ���
//			DMA1_Channel5->CNDTR=10;       										//�������ý������ݸ���	
			DMA1_Channel5->CNDTR=USART_BUFFERSIZE;	//�õ������������ݸ���			
			DMA_Cmd(DMA1_Channel5,ENABLE);  																//��������DMA			
			return num;
		}
	}
	else if(USARTx==USART2)
	{
		if(USART_GetITStatus(USART2,USART_IT_IDLE))
		{
			USART_ClearITPendingBit(USART2,USART_IT_IDLE); 									//������д��ڱ�־λ
			DMA_ClearFlag(DMA1_FLAG_TC6);
			DMA_Cmd(DMA1_Channel6,DISABLE);			//DMA���͹ر�
		//			free(USART_BUFFER);									//�ͷŶ�̬�ռ�
			return num;
		}
	}
	else if(USARTx==USART3)
	{
		if(USART_GetITStatus(USART3,USART_IT_IDLE))
		{
			USART_ClearITPendingBit(USART3,USART_IT_IDLE); 									//������д��ڱ�־λ
			DMA_ClearFlag(DMA1_FLAG_TC3);
			DMA_Cmd(DMA1_Channel3,DISABLE);			//DMA���͹ر�
		//			free(USART_BUFFER);									//�ͷŶ�̬�ռ�
			return num;
		}
	}
	else if(USARTx==UART4)
	{
		if(USART_GetITStatus(UART4,USART_IT_IDLE))
		{
			USART_ClearITPendingBit(UART4,USART_IT_IDLE); 									//������д��ڱ�־λ
			DMA_ClearFlag(DMA2_FLAG_TC3);
			DMA_Cmd(DMA2_Channel3,DISABLE);			//DMA���͹ر�
		//			free(USART_BUFFER);									//�ͷŶ�̬�ռ�
			return num;
		}
	}
	else if(USARTx==UART5)
	{
//		if(DMA_GetFlagStatus(DMA1_FLAG_TC4))
//		{
//			DMA_ClearFlag(DMA1_FLAG_TC4);
//			DMA_Cmd(DMA1_Channel4,DISABLE);			//DMA���͹ر�
//		//			free(USART_BUFFER);									//�ͷŶ�̬�ռ�
			return num;
	}
	else
	{
		return 0;
	}
	
	return num;
//	if(status==SUCCESS)
//	{
//		free(USART_BUFFER);						//�ͷŶ�̬�ռ�
//	}	
//	return status;			//����״ֵ̬
}
/*******************************************************************************
*������		:	USART_TX_DMAFlagClear
*��������	:	�������DMA����ȫ���жϱ�־
						->���������DMA������ָ����������ʱ��DMA�������ڸ�DMAͨ�����ж������ϲ���һ�жϡ�
						->�ڷ���ģʽ�£���DMA����������Ҫ���͵�����ʱ��DMA����������DMA_ISR�Ĵ�����TCIF��־��
						->����USART_SR�Ĵ�����TC��־����ȷ��USARTͨ���Ƿ���������������ڹر�USART�����ͣ��ģʽ֮ǰ�����ƻ����һ�δ�������ݣ�
						->������Ҫ�ȵȴ�TXE=1���ٵȴ�TC=1��
*����			: 
*���			:	��
*����ֵ		:	��
*����			:
*******************************************************************************/
ErrorStatus USART_TX_DMAFlagClear(USART_TypeDef* USARTx)				//�������DMA����ȫ���жϱ�־
{
	ErrorStatus status = ERROR;
//	status = SUCCESS;
//	if(DMA_GetFlagStatus(DMA1_FLAG_GL4))
//	{
//		DMA_ClearFlag(DMA1_FLAG_GL4);
//		DMA_Cmd(DMA1_Channel4,DISABLE);			//DMA���͹ر�
//		free(USART_BUFFER);						//�ͷŶ�̬�ռ�
//	}
	//4)**********���ݴ����������DMAͨ������������
//	u8 DMAx_Channelx_IRQChannel=0;							//DMA�ж�Դ
//	u32 DMAx_FLAG_GLtx=0;												//DMA���ڷ����ж�ȫ�ֱ���			
//	u32 DMAx_FLAG_GLrx=0;												//DMA���ڽ����ж�ȫ�ֱ���
	if(USARTx==USART1)
	{
		if(DMA_GetFlagStatus(DMA1_FLAG_TC4))
		{
			DMA_ClearFlag(DMA1_FLAG_TC4);
			DMA_Cmd(DMA1_Channel4,DISABLE);			//DMA���͹ر�
		//			free(USART_BUFFER);									//�ͷŶ�̬�ռ�
			status = SUCCESS;
		}
		else
		{
			status = ERROR;
		}
	}
	else if(USARTx==USART2)
	{
		if(DMA_GetFlagStatus(DMA1_FLAG_TC7))
		{
			DMA_ClearFlag(DMA1_FLAG_TC7);
			DMA_Cmd(DMA1_Channel7,DISABLE);			//DMA���͹ر�
		//			free(USART_BUFFER);									//�ͷŶ�̬�ռ�
			status = SUCCESS;
		}
		else
		{
			status = ERROR;
		}
	}
	else if(USARTx==USART3)
	{
		if(DMA_GetFlagStatus(DMA1_FLAG_TC2))
		{
			DMA_ClearFlag(DMA1_FLAG_TC2);
			DMA_Cmd(DMA1_Channel2,DISABLE);			//DMA���͹ر�
		//			free(USART_BUFFER);									//�ͷŶ�̬�ռ�
			status = SUCCESS;
		}
		else
		{
			status = ERROR;
		}
	}
	else if(USARTx==UART4)
	{
		if(DMA_GetFlagStatus(DMA2_FLAG_TC5))
		{
			DMA_ClearFlag(DMA2_FLAG_TC5);
			DMA_Cmd(DMA2_Channel5,DISABLE);			//DMA���͹ر�
		//			free(USART_BUFFER);									//�ͷŶ�̬�ռ�
			status = SUCCESS;
		}
		else
		{
			status = ERROR;
		}
	}
	else if(USARTx==UART5)
	{
//		if(DMA_GetFlagStatus(DMA1_FLAG_TC4))
//		{
//			DMA_ClearFlag(DMA1_FLAG_TC4);
//			DMA_Cmd(DMA1_Channel4,DISABLE);			//DMA���͹ر�
//		//			free(USART_BUFFER);									//�ͷŶ�̬�ռ�
//			status = SUCCESS;
//		}
//		else
//		{
			status = ERROR;
//		}
	}
	if(status==SUCCESS)
	{
		free(USART_BUFFER);						//�ͷŶ�̬�ռ�
	}	
	return status;			//����״ֵ̬

}

/*******************************************************************************
* ������		:	
* ��������	:	printf�ض��� 
* ����		:	
* ���		:
* ���� 		:
*******************************************************************************/
int fputc(int ch, FILE *f)				//printf�ض���
{

	USART_SendData(USART2, (unsigned char) ch);// USART1 ���Ի��� USART2 ��

	while(!(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == SET));

	return (ch);

}

  
/*******************************************************************************
* ������		:	
* ��������	:	 
* ����		:	
* ���		:
* ���� 		:
*******************************************************************************/
//int fgetc(FILE *f)
//{
// 
//	while(!(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET))
//	{
//	}
//	return (USART_ReceiveData(USART1));
//}

/*******************************************************************************
*������		:USART_RX_Server
*��������	:���ڽ��շ�����
*����			: 
*���			:��
*����ֵ		:��
*����			��
*******************************************************************************/

void USART_RX_Server(void)		//���ڽ��շ������	
{
//	u16 num=0;
	WOW_Server(); 
//	USART_ClearITPendingBit(USART1,USART_IT_IDLE); 				//������д��ڱ�־λ
//	USART_ClearITPendingBit(USART1,USART_IT_TC); 					//��������ж�
//	USART_ClearITPendingBit(USART1,USART_IT_RXNE); 				//������д��ڱ�־λ
	
//    if(USART_GetITStatus(USART1,USART_IT_IDLE) == SET)
//    {
////#ifdef	USART1_DMA
////		u16 num=0;
//		USART_ClearITPendingBit(USART1,USART_IT_IDLE); 				//������д��ڱ�־λ
////		USART_ClearITPendingBit(USART1,USART_IT_RXNE); 				//������д��ڱ�־λ
//		num = USART1->DR; 																		//������������������־
//		DMA_Cmd(DMA1_Channel5,DISABLE);    										//�رս���DMA
//		num = DMA1_BufferSize -  DMA_GetCurrDataCounter(DMA1_Channel5);  	//�õ������������ݸ���
//		DMA1_Channel4->CNDTR =num;														//�趨���������ݸ���
////		temp =num;   
////		data_uart1[num] = '\0';
//		DMA1_Channel5->CNDTR=DMA1_BufferSize;       				//�������ý������ݸ���
//			
//		DMA_Cmd(DMA1_Channel5,ENABLE);  										//��������DMA
//		DMA_Cmd(DMA1_Channel4,ENABLE);											//DMA���Ϳ���
////#endif
//		}
//			
//	if(USART_GetITStatus(USART2,USART_IT_IDLE) == SET)
//    {
//		USART_ClearITPendingBit(USART2,USART_IT_IDLE); 				//������д��ڱ�־λ
//		num = USART2->DR; 																		//������������������־
//		DMA_Cmd(DMA1_Channel6,DISABLE);    										//�رս���DMA
//		num = DMA1_BufferSize -  DMA_GetCurrDataCounter(DMA1_Channel6);  	//�õ������������ݸ���
//		DMA1_Channel7->CNDTR =num;														//�趨���������ݸ���
//		DMA1_Channel6->CNDTR=DMA1_BufferSize;       				//�������ý������ݸ���
//			
//		DMA_Cmd(DMA1_Channel6,ENABLE);  										//��������DMA			
//		DMA_Cmd(DMA1_Channel7,ENABLE);											//DMA���Ϳ���
//		}
/************************************************************************
***********************����2�շ�****************************************

if(USART_GetITStatus(USART2,USART_IT_IDLE))
  {
			USART_ClearITPendingBit(USART2,USART_IT_IDLE); 					//������д��ڱ�־λ
			num = USART2->DR; 												//������������������־
			DMA_Cmd(DMA1_Channel6,DISABLE);    										//�رս���DMA
			num = DMA1_BufferSize -  DMA_GetCurrDataCounter(DMA1_Channel6);	//�õ������������ݸ���
			DMA1_Channel7->CNDTR =num;										//�趨���������ݸ���
			DMA1_Channel6->CNDTR=DMA1_BufferSize;       					//�������ý������ݸ���				
			DMA_Cmd(DMA1_Channel6,ENABLE);  								//��������DMA			
			DMA_Cmd(DMA1_Channel7,ENABLE);									//DMA���Ϳ���
	}
	else if(DMA_GetFlagStatus(DMA1_FLAG_GL7))
	{
		DMA_ClearFlag(DMA1_FLAG_GL7);
		DMA_Cmd(DMA1_Channel7,DISABLE);									//DMA���Ϳ���
	}
*************************************************************************
************************************************************************/
}



