#ifdef R2A15908SP

#include "R2A15908SP.H"

u8 ack_flag=0;
u8 volume=10;

/*******************************************************************************
* 函数名		:	R2A15908SP_Configuration
* 功能描述	:	R2A15908SP项目配置 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void R2A15908SP_Configuration(void)
{

//	PWM_Configuration(TIM2,7200,10000,20);
	TIM_Configuration(TIM4,7200,10000);										//定时时间设定
	R2A15908SP_GPIO();
	
	IIC_Configuration(GPIOC,GPIO_Pin_1,GPIOC,GPIO_Pin_0);	//AT24C02
	IIC_Configuration(GPIOB,GPIO_Pin_9,GPIOB,GPIO_Pin_8);	//R2A15908SP
	
	EXTI_Configuration(GPIOB,GPIO_Pin_3,EXTI_Mode_Interrupt);
//	EXTI_Configuration(GPIOB,GPIO_Pin_4,EXTI_Mode_Interrupt);
	EXTI_Configuration(GPIOB,GPIO_Pin_5,EXTI_Mode_Interrupt);
	EXTI_Configuration(GPIOB,GPIO_Pin_6,EXTI_Mode_Interrupt);
	EXTI_Configuration(GPIOC,GPIO_Pin_12,EXTI_Mode_Interrupt);		//测试音量
	EXTI_Configuration(GPIOC,GPIO_Pin_13,EXTI_Mode_Interrupt);
	EXTI_Configuration(GPIOC,GPIO_Pin_15,EXTI_Mode_Interrupt);		//volume
//	EXTI_Configuration(GPIOA,GPIO_Pin_15,EXTI_Mode_Interrupt);
}
/*******************************************************************************
* 函数名		:	R2A15908SP_Server
* 功能描述	:	R2A15908SP服务程序
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void R2A15908SP_Server(void)
{
//	u8 ack_flag=0;
	ack_flag=0;
//	GPIO_SetBits(GPIOA, GPIO_Pin_0);			//红灯
	R2A15908SP_Ecoder();	//编码器解析
	if(ack_flag==1)
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);		//绿灯
	}
	else
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_0);			//红灯
	}
	
//	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_2)==1)
//	{
////		GPIO_SetBits(GPIOA, GPIO_Pin_2);
//		GPIO_ResetBits(GPIOA, GPIO_Pin_2);
//	}
//	else
//	{
//		GPIO_SetBits(GPIOA, GPIO_Pin_2);
////		GPIO_ResetBits(GPIOA, GPIO_Pin_2);
//	}
	R2A15908SP_DELAY(65000);R2A15908SP_DELAY(65000);R2A15908SP_DELAY(65000);R2A15908SP_DELAY(65000);R2A15908SP_DELAY(65000);

}

/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void R2A15908SP_DELAY(u32 Time)
{
	while(Time--);

}

/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void R2A15908SP_GPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_AFIO, ENABLE);
	//********************指示灯********************//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//********************电子开关********************//
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				//音量
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;   //上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;				//音量
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;   //上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
u8 Write_R2A15908SP(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin,u8 txd)
{
	u8 ack_fla=0;
	//********************启动I2C总线********************//
	IIC_Start(GPIOx_SCL,IIC_SCLPin,GPIOx_SDA,IIC_SDAPin);													//启动I2C总线
	
	IIC_Send_Byte(GPIOx_SCL,IIC_SCLPin,GPIOx_SDA,IIC_SDAPin,0x82);							//0X82设备地址
	ack_fla=IIC_Wait_Ack(GPIOx_SCL,IIC_SCLPin,GPIOx_SDA,IIC_SDAPin); 						//IIC等待ACK信号
	IIC_Send_Byte(GPIOx_SCL,IIC_SCLPin,GPIOx_SDA,IIC_SDAPin,0x00);							//IIC发送一个字节
	ack_fla=IIC_Wait_Ack(GPIOx_SCL,IIC_SCLPin,GPIOx_SDA,IIC_SDAPin); 						//IIC等待ACK信号
	IIC_Send_Byte(GPIOx_SCL,IIC_SCLPin,GPIOx_SDA,IIC_SDAPin,volume&0XFE);							//左声道输出音量
	ack_fla=IIC_Wait_Ack(GPIOx_SCL,IIC_SCLPin,GPIOx_SDA,IIC_SDAPin); 						//IIC等待ACK信号
	IIC_Send_Byte(GPIOx_SCL,IIC_SCLPin,GPIOx_SDA,IIC_SDAPin,volume&0XFE);							//右声道输出音量
	ack_fla=IIC_Wait_Ack(GPIOx_SCL,IIC_SCLPin,GPIOx_SDA,IIC_SDAPin); 						//IIC等待ACK信号
	IIC_Send_Byte(GPIOx_SCL,IIC_SCLPin,GPIOx_SDA,IIC_SDAPin,0x0C);							//输入选择及增益设定
	ack_fla=IIC_Wait_Ack(GPIOx_SCL,IIC_SCLPin,GPIOx_SDA,IIC_SDAPin); 						//IIC等待ACK信号
	IIC_Send_Byte(GPIOx_SCL,IIC_SCLPin,GPIOx_SDA,IIC_SDAPin,0x00);							//单声道输入/双声道输入选择/音效选择
	ack_fla=IIC_Wait_Ack(GPIOx_SCL,IIC_SCLPin,GPIOx_SDA,IIC_SDAPin); 						//IIC等待ACK信号
	IIC_Send_Byte(GPIOx_SCL,IIC_SCLPin,GPIOx_SDA,IIC_SDAPin,0xFF);							//音效调节
	ack_fla=IIC_Wait_Ack(GPIOx_SCL,IIC_SCLPin,GPIOx_SDA,IIC_SDAPin); 						//IIC等待ACK信号
	
	//********************停止I2C总线********************//
	IIC_Stop(GPIOx_SCL,IIC_SCLPin,GPIOx_SDA,IIC_SDAPin);												//停止I2C总线											
	return ack_fla;
}
/*******************************************************************************
* 函数名		:	R2A15908SP_Ecoder
* 功能描述	:	编码器解析
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void R2A15908SP_Ecoder(void)
{
	//********************************VOLUME
	if(EXTI_GetFlagStatus(EXTI_Line13))			//BUTTON
	{
		volume=0X10;
		ack_flag=Write_R2A15908SP(GPIOB,GPIO_Pin_9,GPIOB,GPIO_Pin_8,volume);
//		IIC_Start(GPIOC,GPIO_Pin_1,GPIOC,GPIO_Pin_0);
//		IIC_Send_Byte(GPIOC,GPIO_Pin_1,GPIOC,GPIO_Pin_0,0xA0);						//IIC发送一个字节
//		ack_flag=IIC_Wait_Ack(GPIOC,GPIO_Pin_1,GPIOC,GPIO_Pin_0); 				//IIC等待ACK信号
//		IIC_Send_Byte(GPIOB,GPIO_Pin_9,GPIOB,GPIO_Pin_8,0x02);						//IIC发送一个字节
//		ack_flag|=IIC_Wait_Ack(GPIOB,GPIO_Pin_9,GPIOB,GPIO_Pin_8); 				//IIC等待ACK信号
//		IIC_Send_Byte(GPIOB,GPIO_Pin_9,GPIOB,GPIO_Pin_8,0x00);						//IIC发送一个字节
//		ack_flag|=IIC_Wait_Ack(GPIOB,GPIO_Pin_9,GPIOB,GPIO_Pin_8); 				//IIC等待ACK信号
		
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
	if(EXTI_GetFlagStatus(EXTI_Line15))			//VOLUME
	{
//		R2A15908SP_DELAY(7200);
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14)==0)
		{
			if(volume<0XFE)
			volume+=1;
		}
		else
		{
			if(volume>0)
			volume-=1;
		}
		ack_flag=Write_R2A15908SP(GPIOB,GPIO_Pin_9,GPIOB,GPIO_Pin_8,volume);
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
	
	if(EXTI_GetFlagStatus(EXTI_Line6))
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_0);			//红灯
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	if(EXTI_GetFlagStatus(EXTI_Line4))
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_0);			//红灯
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
	if(EXTI_GetFlagStatus(EXTI_Line12))			//MID
	{
		R2A15908SP_DELAY(7200);
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11)==1)
		{
			if(volume<0XFE)
			volume+=1;
		}
		else
		{
			if(volume>0)
			volume-=1;
		}
		ack_flag=Write_R2A15908SP(GPIOB,GPIO_Pin_9,GPIOB,GPIO_Pin_8,volume);
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
}


#endif


