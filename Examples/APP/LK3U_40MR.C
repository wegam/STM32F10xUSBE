#ifdef LK3U_40MR
#include "LK3U_40MR.H"

#include "STM32_GPIO.H"
#include "STM32_SYS.H"
#include "STM32F10x_BitBand.H"
#include "STM32_SYSTICK.H"
#include "STM32_USART.H"


u32	sys_led=0;
u8 add_flag=0;		//流水类左右标志

u32	num_temp=0;					//输入数值
u16	tema=0;
u8	USART_TX_FLAG=0;		//485发送标志
u8	USART_RX_FLAG=0;		//485接收标志
u32 DMASTAST=0;
u32 input_num=0;		//读取光耦变量
u8 send_num=0;		//485发送变量
u8 recev_num=0;		//485发送变量

unsigned long led_num=0;	//LED显示

#define	FX2N_14MR_BufferSize 4		//DMA1缓冲大小

u8 FX2N_14MR_RXBuffer[FX2N_14MR_BufferSize]={0};
u8 FX2N_14MR_TXBuffer[FX2N_14MR_BufferSize]={0};

/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void LK3U_40MR_Configuration(void)
{
	SYS_Configuration();	//系统配置 STM32_SYS.H
	USART_DMA_Configuration(UART4,9600,1,1,(u32*)FX2N_14MR_RXBuffer,(u32*)FX2N_14MR_TXBuffer,FX2N_14MR_BufferSize);	//USART_DMA配置
	SysTick_Configuration(200000);	//系统嘀嗒时钟配置72MHz,单位为uS	
	LK3U_40MR_GPIOSET();
}

/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void LK3U_40MR_Server(void)
{
	char i=0;
	u32 temp=0;
	
	LK3U_40MR_LEDSYS();
	i=USART_RX_FlagClear(UART4);
	USART_TX_DMAFlagClear(UART4);						//清除串口DMA发送全局中断标志	
	if(i)		//------------串口接收
	{
		u8 temp[4]={0};
		USART_RX_FLAG=1;
		tema=0;
		memcpy(temp,FX2N_14MR_RXBuffer,i);
		memset(FX2N_14MR_RXBuffer,0,i);			//初始化缓冲
		
		input_num=0;
		input_num|=temp[3];
		input_num<<=8;
		input_num|=temp[2];
		input_num<<=8;
		input_num|=temp[1];
		input_num<<=8;
		input_num|=temp[0];
	}
	else if(USART_RX_FLAG==0)		//------------串口发送
	{
		input_num=LK3U_40MR_INPUT();
		if(input_num!=0x01)
		{
			temp=input_num;
			//DMA发送
			USART_DMASend(UART4,(u32*)&temp,4);
			//常规发送
//			for(i=0;i<4;i++)
//			{
//				USART_SendData(UART4,temp&0xFF);
//				while(!(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == SET));
//				temp>>=8;
//			}
		}
	}
	
	LK3U_40MR_LEDDSP(input_num);
	
	if(USART_RX_FLAG==1)
	{
		if(tema++>1)
		{
			tema=0;
			input_num=0;
			USART_RX_FLAG=0;
		}
	}

}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void LK3U_40MR_GPIOSET(void)
{
	//输入端口--光耦--低有效
//	GPIO_Configuration(GPIOA,GPIO_Pin_0,GPIO_Mode_IPU,GPIO_Speed_2MHz);			//GPIO配置
	
	GPIO_Configuration(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//GPIO配置
	GPIO_Configuration(GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_9,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//GPIO配置
	GPIO_Configuration(GPIOC,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_13,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//GPIO配置
	GPIO_Configuration(GPIOD,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_15,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//GPIO配置
	GPIO_Configuration(GPIOE,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_5|GPIO_Pin_9|GPIO_Pin_11,GPIO_Mode_IPU,GPIO_Speed_50MHz);			//GPIO配置
	
//	PA0=0;
	PA1=1;
	PA8=1;
	PA11=1;
	PA12=1;
	
	PB3=1;
	PB4=1;
	PB5=1;
	PB9=1;
	
	PC6=1;
	PC7=1;
	PC9=1;
	PC13=1;
	
	PD3=1;
	PD4=1;
	PD5=1;
	PD6=1;
	PD7=1;
	PD15=1;
	
	PE0=1;
	PE1=1;
	PE2=1;
	PE5=1;
	PE9=1;
	PE11=1;
	
	
	//输出端口--ULN2803--高有效
	GPIO_Configuration(GPIOA,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_7,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);			//GPIO配置
	GPIO_Configuration(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_6,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);			//GPIO配置
	GPIO_Configuration(GPIOC,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);			//GPIO配置
	GPIO_Configuration(GPIOE,GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);			//GPIO配置
	
	PA2=1;
	PA3=1;
	PA6=1;
	PA7=1;
	PB0=1;
	PB1=1;
	PB6=1;
	PC4=1;
	PC5=1;
	PC8=1;
	PE7=1;
	PE8=1;
	PE10=1;
	PE13=1;
	PE14=1;
	PE15=1;
	
	//输出端口--LED--低有效
	//ERR
	GPIO_Configuration(GPIOB,GPIO_Pin_15,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);			//GPIO配置
	PB15=0;
	//485
	GPIO_Configuration(GPIOB,GPIO_Pin_13,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);			//GPIO配置
	PB13=0;
	//232
	GPIO_Configuration(GPIOB,GPIO_Pin_12,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);			//GPIO配置
	PB12=0;
	//RUN
	GPIO_Configuration(GPIOB,GPIO_Pin_14,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);			//GPIO配置
	PB14=0;
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void LK3U_40MR_LEDDSP(unsigned long led_num)
{
	u32	num_out=0;
	if(led_num==0x01)
	{
		DROUT_ALL_OFF;
		
		if(add_flag==0)
		{
			if(num_temp++>16)
			{
				add_flag=1;				
			}
		}
		else
		{
			if(num_temp--<=1)
			{
				add_flag=0;				
			}
		}
		
		num_out=num_temp;
		
		if(num_out==0)
			DROUT(1)=1;
		else	if(num_out==1)
			DROUT(1)=1;
		else	if(num_out==2)
			DROUT(2)=1;
		else	if(num_out==3)
			DROUT(3)=1;
		else	if(num_out==4)
			DROUT(4)=1;
		
		
		else	if(num_out==5)
			DROUT(5)=1;
		else	if(num_out==6)
			DROUT(6)=1;
		else	if(num_out==7)
			DROUT(7)=1;
		else	if(num_out==8)
			DROUT(8)=1;
		
		
		else	if(num_out==9)
			DROUT(9)=1;
		else	if(num_out==10)
			DROUT(10)=1;
		else	if(num_out==11)
			DROUT(11)=1;
		else	if(num_out==12)
			DROUT(12)=1;
		
		
		else	if(num_out==13)
			DROUT(13)=1;
		else	if(num_out==14)
			DROUT(14)=1;
		else	if(num_out==15)
			DROUT(15)=1;
		else	if(num_out==16)
			DROUT(16)=1;
		else	if(num_out==17)
			DROUT(16)=1;
	}
	else
	{
		num_temp=0;
		DROUT_ALL_OFF;
		num_out=led_num;
		num_out=num_out>>1;
		if(num_out&0xFF)
		{
			
			if(num_out&0x01)
			{
				DROUT(1)=1;
			}
			if(num_out&0x02)
			{
				DROUT(2)=1;
			}
			if(num_out&0x04)
			{
				DROUT(3)=1;
			}
			if(num_out&0x08)
			{
				DROUT(4)=1;
			}
			if(num_out&0x10)
			{
				DROUT(5)=1;	
			}
			if(num_out&0x20)
			{
				DROUT(6)=1;	
			}
			if(num_out&0x40)
			{
				DROUT(7)=1;	
			}
			if(num_out&0x80)
			{
				DROUT(8)=1;	
			}			
		}
		num_out=num_out>>8;
		if(num_out&0xFF)
		{
			if(num_out&0x01)
				DROUT(9)=1;
			if(num_out&0x02)
				DROUT(10)=1;
			if(num_out&0x04)
				DROUT(11)=1;
			if(num_out&0x08)
				DROUT(12)=1;
			if(num_out&0x10)
			{
				DROUT(13)=1;	
			}
			if(num_out&0x20)
			{
				DROUT(14)=1;	
			}
			if(num_out&0x40)
			{
				DROUT(15)=1;
			}
			if(num_out&0x80)
			{
				DROUT(16)=1;	
			}			
		}
		num_out=num_out>>8;
		if(num_out&0xFF)
		{
			if(num_out&0x01)
			{
				DROUT(1)=1;
				DROUT(2)=1;
			}
			if(num_out&0x02)
			{
				DROUT(3)=1;
				DROUT(4)=1;
			}
			if(num_out&0x04)
			{
				DROUT(5)=1;
				DROUT(6)=1;
			}
			if(num_out&0x08)
			{
				DROUT(7)=1;
				DROUT(8)=1;
			}
			if(num_out&0x10)
			{
				DROUT(9)=1;	
				DROUT(10)=1;
			}
			if(num_out&0x20)
			{
				DROUT(11)=1;	
				DROUT(12)=1;
			}
			if(num_out&0x40)
			{
				DROUT(13)=1;	
				DROUT(14)=1;
			}
			if(num_out&0x80)
			{
				DROUT(15)=1;	
				DROUT(16)=1;
			}			
		}
	}
}

void LK3U_40MR_LEDSYS(void)
{
	u32	num_out=0;
	if(sys_led++>4)
	{
		sys_led=0;
		DROUT_ALL_OFF;
		SIN_ALL_OFF;
	}
	num_out=sys_led;
	
	LK3U_40MR_LEDRUN=1;
	LK3U_40MR_LED232=1;
	LK3U_40MR_LED485=1;
	LK3U_40MR_LEDERR=1;
	
	//RUN_LED
	if((num_out%4)==0)
	{
		LK3U_40MR_LEDRUN=0;
	}
	else	if((num_out%4)==1)
	{
		LK3U_40MR_LED232=0;
	}
	else	if((num_out%4)==2)
	{
		LK3U_40MR_LED485=0;
	}
	else	if((num_out%4)==3)
	{
		LK3U_40MR_LEDERR=0;
	}
}
unsigned long LK3U_40MR_INPUT(void)
{
	u32 input_num=0;
	if(!SIN(1))
		input_num|=1<<0;
	if(!SIN(2))
		input_num|=1<<1;
	if(!SIN(3))
		input_num|=1<<2;
	if(!SIN(4))
		input_num|=1<<3;
	if(!SIN(5))
		input_num|=1<<4;
	if(!SIN(6))
		input_num|=1<<5;
	if(!SIN(7))
		input_num|=1<<6;
	if(!SIN(8))
		input_num|=1<<7;
	if(!SIN(9))
		input_num|=1<<8;
	if(!SIN(10))
		input_num|=1<<9;
	if(!SIN(11))
		input_num|=1<<10;
	if(!SIN(12))
		input_num|=1<<11;
	if(!SIN(13))
		input_num|=1<<12;
	if(!SIN(14))
		input_num|=1<<13;
	if(!SIN(15))
		input_num|=1<<14;
	if(!SIN(16))
		input_num|=1<<15;
	if(!SIN(17))
		input_num|=1<<16;
	if(!SIN(18))
		input_num|=1<<17;
	if(!SIN(19))
		input_num|=1<<18;
	if(!SIN(20))
		input_num|=1<<19;
	if(!SIN(21))
		input_num|=1<<20;
	if(!SIN(22))
		input_num|=1<<21;
	if(!SIN(23))
		input_num|=1<<22;
	if(!SIN(24))
		input_num|=1<<23;
	if(!SIN(25))
		input_num|=1<<24;

	return input_num;
	
}




#endif
