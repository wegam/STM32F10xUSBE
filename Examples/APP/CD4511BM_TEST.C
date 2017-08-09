#ifdef	CD4511BM_TEST


//1������ܲ���ָ��
//2��С����������ʾָ��
//3�����⹦�ܣ�����ָ��ȷ�����޳�����CAN,485ͨѶ
#include "CD4511.H"
#include "CD4511BM_TEST.H"
#include "STM32_PWM.H"
#include "STM32_SYS.H"
#include "STM32_SYSTICK.H"
#include "STM32F10x_BitBand.H"


#define	CD4511BM_DEBUG_TEST			//����ʱ����ܼ�����ʾ������ʹ��ʱӦ��ע�͵�


u16 Time=0;
u16 data=0;


CD4511_Pindef CD4511_Pin;


void SEG_DISPALY(u8 wei,u16 num);
/*******************************************************************************
*������		:	function
*��������	:	��������˵��
*����			: 
*���			:	��
*����ֵ		:	��
*����			:
*******************************************************************************/
void CD4511BM_TEST_Configuration(void)
{
	SYS_Configuration();											//ϵͳ���� STM32_SYS.H	
	PWM_OUT(TIM2,PWM_OUTChannel2,1,500);
	SysTick_Configuration(1000);							//ϵͳ���ʱ������72MHz,��λΪuS
	
	//λ
	GPIO_Configuration_OPP50	(GPIOB,		GPIO_Pin_5);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�50MHz----V20170605
	GPIO_Configuration_OPP50	(GPIOB,		GPIO_Pin_14);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�50MHz----V20170605
	GPIO_Configuration_OPP50	(GPIOB,		GPIO_Pin_15);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�50MHz----V20170605
	
	//Pin
	CD4511_Pin.CD4511_A0_PORT=GPIOB;
	CD4511_Pin.CD4511_A0_Pin=GPIO_Pin_0;
	
	CD4511_Pin.CD4511_A1_PORT=GPIOB;
	CD4511_Pin.CD4511_A1_Pin=GPIO_Pin_1;
	
	CD4511_Pin.CD4511_A2_PORT=GPIOB;
	CD4511_Pin.CD4511_A2_Pin=GPIO_Pin_2;
	
	CD4511_Pin.CD4511_A3_PORT=GPIOB;
	CD4511_Pin.CD4511_A3_Pin=GPIO_Pin_3;
	
	CD4511_PinConf(&CD4511_Pin);
}

/*******************************************************************************
*������		:	function
*��������	:	��������˵��
*����			: 
*���			:	��
*����ֵ		:	��
*����			:
*******************************************************************************/
void CD4511BM_TEST_Server(void)
{
	++Time;	
	if(Time>=20)
	{
		Time=0;	
		++data;		
	}
	if(data>999)
		data=0;
	SEG_DISPALY(Time%3,data);
}

/*******************************************************************************
*������		:	function
*��������	:	��������˵��
*����			: 
*���			:	��
*����ֵ		:	��
*����			:
*******************************************************************************/
void SEG_DISPALY(u8 wei,u16 num)
{
	if(wei==2)
	{
		if(num>100)
		{
			PB5=1;
			PB14=0;
			PB15=0;
			CD4511_WriteData(&CD4511_Pin,num/100);
		}
		else
		{
			PB5=0;
			PB14=0;
			PB15=0;
		}
	}
	else if(wei==1)
	{
		if(num>=10||num>=100)
		{
			PB5=0;
			PB14=1;
			PB15=0;
			if(num>=100)
			{
				num=num%100;
			}
			CD4511_WriteData(&CD4511_Pin,num/10);
		}
		else
		{
			PB5=0;
			PB14=0;
			PB15=0;
		}
	}
	else
	{
		PB5=0;
		PB14=0;
		PB15=1;
		CD4511_WriteData(&CD4511_Pin,num%10);
	}
	
//	if(time%100>0)
//	{
//		PB5=1;
//		CD4511_WriteData(&CD4511_Pin,time%100);
//	}
//	else
//	{
//		PB5=0;
//	}	
//	if((time>100)||((time/10)%10>0))
//	{
//		PB14=1;
//		CD4511_WriteData(&CD4511_Pin,(time/10)%10);
//	}
//	else
//	{
//		PB14=0;
//	}
//	if(time%10>0)
//	{
//		PB15=1;
//		CD4511_WriteData(&CD4511_Pin,time%10);
//	}
//	else
//	{
//		PB15=0;
//	}
}
/*******************************************************************************
* ������			:	function
* ��������		:	��������˵�� 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void CD4511BM_KEY_FUNC(void)
{
//	GPIO_InitTypeDef GPIO_InitStructure;						//GPIO�ṹ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	
//	//1)**********S3�ܽ����ü���ʼ��
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				//GPIO���ź�			
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				//GPIOģʽ
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//GPIO����ٶȣ�����ʱ��Ч
//	GPIO_Init(GPIOB,&GPIO_InitStructure);	

}








#endif
