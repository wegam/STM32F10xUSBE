#ifdef USART_TEST

#include "USART_TEST.H"

#include "STM32_USART.H"
#include "STM32_DMA.H"
#include "STM32_TIM.H"
#include "STM32_WDG.H"
#include "STM32_PWM.H"
#include "STM32_GPIO.H"
#include "STM32_SYS.H"
#include "STM32_SYSTICK.H"

#define	BufferSize 1000		//DMA1缓冲大小

u32	num_temp=0;
u16	tema=0;

u32 DMASTAST=0;
ErrorStatus status = ERROR;

u8	txflg1=0;	//USART1发送标志
u16	tx1_tcont=0;	//USART1发送超时-计时

u8 rxBuffer1[BufferSize]={0};
u8 txBuffer1[BufferSize]={0};
u8 num=0;
u8 ch[120]="USART_BASIC_Configuration(USART_TypeDef* USARTx,u32 USART_BaudRate,u8 NVICPreemptionPriority,u8 NVIC_SubPriority)\n";

//u8 itf=0;
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void Usart_test_Configuration(void)
{
	SYS_Configuration();					//系统配置---打开系统时钟 STM32_SYS.H
	
	GPIO_DeInitAll();							//将所有的GPIO关闭----V20170605
	
	SysTick_Configuration(1000);	//系统嘀嗒时钟配置72MHz,单位为uS
	
	IWDG_Configuration(1000);			//独立看门狗配置---参数单位ms	
	
	PWM_OUT(TIM2,PWM_OUTChannel1,1,900);						//PWM设定-20161127版本	
	
	
	USART_DMA_Configuration(USART1,256000,1,1,(u32*)rxBuffer1,(u32*)txBuffer1,BufferSize);	//USART_DMA配置
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC, DISABLE);
	USART_ITConfig(USART1,USART_IT_IDLE, DISABLE);				//使用空闲中断，DMA自动接收，检测到总线空闲表示发送端已经发送完成，数据保存在DMA缓冲器中
}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void Usart_test_Server(void)
{	
	IWDG_Feed();								//独立看门狗喂狗
	Usart_test();
	

}
void Usart_test(void)
{
	u16 num=0;
	u8	flg_tx;	
	
	num=USART_RX_FlagClear(USART1);		
	if(num!=0xFFFF)
	{
		num=BufferSize-num;																	//获取接收长度
		memcpy(txBuffer1, rxBuffer1,num);										//复制数据
		memset(rxBuffer1, 0x00, BufferSize);								//清空数据
		USART_DMA_RxEN(USART1,(u32*)rxBuffer1,BufferSize);	//重新设定接收缓冲区地址及大小并使能DMA接收
		USART_DMASend(USART1,(u32*)txBuffer1,num);					//串口DMA发送程序
	}

	flg_tx=USART_TX_FlagClear(USART1);										//清除串口DMA发送完成相关标志
	if(flg_tx!=0)
	{
		memset(txBuffer1, 0x00, BufferSize);
	}			

	
//	u32 num=0;
//	//*********************BTL（USART1)***********************************
//	if(USART_GetFlagStatus(USART1,USART_FLAG_IDLE))
//	{
//		USART_ClearFlag(USART1,USART_FLAG_IDLE); 										//清除空闲串口标志位
//		DMA_Cmd(DMA1_Channel5,DISABLE);    													//关闭接收DMA
//		num = USART1->DR; 																					//读出数据以完成清除标志			
//		num = BufferSize -  DMA_GetCurrDataCounter(DMA1_Channel5);	//得到真正接收数据个数
//		DMA1_Channel5->CMAR=(u32)rxBuffer1;													//重新设置DMA接收地址
//		DMA1_Channel5->CNDTR=BufferSize;														//重新设置接收数据个数			
//		DMA_Cmd(DMA1_Channel5,ENABLE);  														//开启接收DMA	

//		txflg1=1;
//		tx1_tcont=0;
//		memcpy(txBuffer1, rxBuffer1,num);
////		memset(rxBuffer1, 0x00, num);
////		USART_DMA_RxEN(USART1,(u32*)rxBuffer1,BufferSize);	//重新设定接收缓冲区地址及大小并使能DMA接收
////		USART_DMASend(USART1,(u32*)txBuffer1,num);
//		DMA1_Channel4->CNDTR =num;										//设定待发送缓冲区大小
//		DMA1_Channel4->CMAR =(u32)txBuffer1;							//发送缓冲区	
//		DMA_Cmd(DMA1_Channel4,ENABLE);											//DMA发送开启3
//		memset(rxBuffer1, 0x00, BufferSize);
//		
//	}
//	if(txflg1==1)
//	{
//		if(tx1_tcont++>100)		//100ms
//		{
//			txflg1=0;
//			tx1_tcont=0;
//			USART_ClearFlag(USART1,USART_FLAG_TC); 										//清除空闲串口标志位
//			DMA_ClearFlag(DMA1_FLAG_GL4);
//			DMA_Cmd(DMA1_Channel4,DISABLE);  													//关闭DMA发送
//			memset(txBuffer1, 0x00, BufferSize);
//		}
//		else
//		{
//			if(DMA_GetFlagStatus(DMA1_FLAG_GL4))
//			{
//				if(USART_GetFlagStatus(USART1,USART_FLAG_TC))
//				{
//					txflg1=0;
//					tx1_tcont=0;
//					USART_ClearFlag(USART1,USART_FLAG_TC); 										//清除空闲串口标志位
//					DMA_ClearFlag(DMA1_FLAG_GL4);
//					DMA_Cmd(DMA1_Channel4,DISABLE);  													//关闭DMA发送
//					memset(txBuffer1, 0x00, BufferSize);
//				}
//			}
//		}
//	}
	
	

	
	
	
//	ErrorStatus status = ERROR;
//	u32 num=0;
//	num_temp=USART_RX_FlagClear(USART1);						//清除串口DMA发送全局中断标志
//	num_temp=USART_RX_FlagClearADDR(USART1,(u32*)Usart_Test_Buffer);
	
//	status=USART_TX_DMAFlagClear(USART1);						//清除串口DMA发送全局中断标志
//	num_temp=USART_RX_FlagClear(USART1);						//清除串口DMA发送全局中断标志
//	status=USART_TX_DMAFlagClear(USART1);						//清除串口DMA发送全局中断标志
//	DMASTAST=*(vu32*)DMA1_Channel4_BASE;
//	if(num_temp!=0)
//	{
//		memcpy(Usart_TXBuffer,Usart_RXBuffer,num_temp);
//		USART_DMAPrintf(USART1,"接收到的数据个数:%d;接收到的数据:%s\n",num_temp,Usart_TXBuffer);
//		tema=0;
//	}
//	else if(!USART_TX_DMAFlagClear(USART1))
//	{
//		memset(Usart_TXBuffer,0,DMA1_BufferSize);			//初始化缓冲
//		if(tema<=100)
//		{		
//			USART_DMAPrintf(USART1,"测试开始:%d\n",tema);
//			tema++;
//		}
//	}
	
//	if(num_temp)
//	{
////		num_temp=USART_RX_FlagClear(USART1);						//清除串口DMA发送全局中断标志
//		memcpy(Usart_TXBuffer,Usart_RXBuffer,num_temp);
//		USART_DMASend(USART1,(u32*)Usart_TXBuffer,num_temp);
////		num_temp=0;
//	}
//	else if(!USART_TX_DMAFlagClear(USART1))
//	{
//		memset(Usart_Test_Buffer,0,DMA1_BufferSize);			//初始化缓冲
//		if(tema<=100)
//		{		
//			USART_DMAPrintf(USART1,"测试开始:%d\n",tema);
////			USART_DMAPrintf(USART1,"测试开始:%s\n",ch);
////			printf("测试开始:%d\n",tema);
//			tema++;
//		}
////		else
////			tema=0;
//	}
	
/*
	ErrorStatus status = ERROR;
	status=USART_TX_DMAFlagClear(USART1);						//清除串口DMA发送全局中断标志
	USART_RX_FlagClear(USART1);				//清除串口接收中断标志
	if(status==SUCCESS)
	{
//		free(USART_BUFFER);						//释放动态空间
	}
	else
	{
		if(tema<=100)
		{		
//			USART_DMAPrintf(USART1,"测试开始:%d\n",tema);
			USART_DMAPrintf(USART1,"测试开始:%s\n",ch);
//			printf("测试开始:%d\n",tema);
			tema++;
		}
		else
			tema=0;
	}
*/
//	IWDG_Feed();
/*	//USART1
	if(USART_GetITStatus(USART1,USART_IT_IDLE))
  {
			USART_ClearITPendingBit(USART1,USART_IT_IDLE); 					//清除空闲串口标志位
			num = USART1->DR; 												//读出数据以完成清除标志
			DMA_Cmd(DMA1_Channel5,DISABLE);    										//关闭接收DMA
			num = DMA1_BufferSize -  DMA_GetCurrDataCounter(DMA1_Channel6);	//得到真正接收数据个数
			DMA1_Channel4->CNDTR =num;										//设定待发送数据个数
			DMA1_Channel5->CNDTR=DMA1_BufferSize;       					//重新设置接收数据个数				
			DMA_Cmd(DMA1_Channel5,ENABLE);  								//开启接收DMA			
			DMA_Cmd(DMA1_Channel4,ENABLE);									//DMA发送开启
			tema=0;
	}
	else if(DMA_GetFlagStatus(DMA1_FLAG_GL4))
	{
		DMA_ClearFlag(DMA1_FLAG_GL4);
		DMA_Cmd(DMA1_Channel4,DISABLE);			//DMA发送关闭
		free(USART_BUFFER);						//释放动态空间
	}
	else
	{
		if(tema<=100)
		{		
//			USART_DMASend(USART2,"测试开始:%d\n",tema);
			printf("测试开始:%d\n",tema);
			tema++;
		}
	}
*/
/*
	//USART2
	if(USART_GetITStatus(USART2,USART_IT_IDLE))
  {
			USART_ClearITPendingBit(USART2,USART_IT_IDLE); 					//清除空闲串口标志位
			num = USART2->DR; 												//读出数据以完成清除标志
			DMA_Cmd(DMA1_Channel6,DISABLE);    										//关闭接收DMA
			num = DMA1_BufferSize -  DMA_GetCurrDataCounter(DMA1_Channel6);	//得到真正接收数据个数
			DMA1_Channel7->CNDTR =num;										//设定待发送数据个数
			DMA1_Channel6->CNDTR=DMA1_BufferSize;       	//重新设置接收数据个数				
			DMA_Cmd(DMA1_Channel6,ENABLE);  							//开启接收DMA			
			DMA_Cmd(DMA1_Channel7,ENABLE);								//DMA发送开启
			tema=0;
	}
	if(DMA_GetFlagStatus(DMA1_FLAG_TC7))
	{
		DMA_ClearFlag(DMA1_FLAG_TC7);
		DMA_Cmd(DMA1_Channel7,DISABLE);			//DMA发送关闭
		free(USART_BUFFER);						//释放动态空间
	}
	else
	{
		if(tema<=1000)
		{		
			USART_DMASend(USART2,"测试开始:%d\n",tema);
//			printf("测试开始:%d\n",tema);
			tema++;
		}
		else
			tema=0;
	}
*/

//	if(tema<30000)
//		tema++;
//	else
//		tema=0;
	
//	if(DMA_GetFlagStatus(DMA1_FLAG_TC7))
//	{
//		DMA_ClearFlag(DMA1_FLAG_TC7);
//		DMA_Cmd(DMA1_Channel7,DISABLE);			//DMA发送关闭
//		free(USART_BUFFER);									//释放动态空间
//	}
//	else
//	{
//			USART_DMASend(USART2,"测试开始:%d\n",tema);
////			printf("测试开始:%d\n",tema);
//	}
//	if(num!=0)
//	{
//		GPIO_SetBits(GPIOA,GPIO_Pin_1);
////		GPIO_ResetBits(GPIOA,GPIO_Pin_2);
//		GPIO_Write(GPIOE,0XAAAA);
//		num=0;
//	}
//	else
//	{
////		GPIO_SetBits(GPIOA,GPIO_Pin_2);
//		GPIO_ResetBits(GPIOA,GPIO_Pin_1);
//		GPIO_Write(GPIOE,0X5555);
//		num=1;
//	}
}
#endif

