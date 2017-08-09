#ifdef ADC_TEST
#include "ADC_TEST.H"


#define ADC_TEST_BUFFERSIZE 128

u8 ADC_TEST_BUFFER[ADC_TEST_BUFFERSIZE]={0};
u8 nump=0;
u16 tema;
u16 tempr=30;
u16 ADCD=30;
u16 DACD=30;

void ADC1_GPIO_Config(void);
void ADC1_Mode_Config(void);
void ADC1_Init(void);
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void ADC_TEST_Configuration(void)
{
	TIM_Configuration(TIM3,7200,10000);	//定时时间设定
	PWM_Configuration(TIM2,7200,200,10);
	USART_DMA_Configuration(USART1,115200,1,1,(u32*)ADC_TEST_BUFFER,(u32*)ADC_TEST_BUFFER,ADC_TEST_BUFFERSIZE);	//USART_DMA配置
	
	
//	ADC1_DiscConfiguration((u32*)&ADCD,1,ADC_Channel_16,1,1,ADC_SampleTime_239Cycles5);				//ADC1规则通道组配置
	
//	ADC_TempSensorVrefintCmd(ENABLE);
//	ADC1_Cmd(ENABLE);					//开启ADC1转换
//	DAC_Configuration();
//	DAC_DMA_Configuration((u32*)&DACD);
//	ADC1_Cmd(ENABLE);					//开启ADC1转换
	
	ADC_TempSensorConfiguration((u32*)&ADCD);		//STM32内部温度传感器配置
	
//		ADC1_Init();
//	GPIO_Configuration(GPIOE,GPIO_Pin_All,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
	GPIO_Configuration(GPIOA,GPIO_Pin_3,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
	SysTick_Configuration(10000);	//系统嘀嗒时钟配置72MHz,单位为uS
	
}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void ADC_TEST_Server(void)
{
	u32 num=0;
	float Temperature=0.0; 
	num=USART_RX_FlagClear(USART1);						//清除串口DMA发送全局中断标志
	if(num!=0)
	{
		USART_DMAPrintf(USART1,"接收到的数据个数:%d;接收到的数据:%s\n",num,ADC_TEST_BUFFERSIZE);
		tema=0;
	}
	else if(!USART_TX_DMAFlagClear(USART1))
	{
		memset(ADC_TEST_BUFFER,0,ADC_TEST_BUFFERSIZE);			//初始化缓冲
		Temperature=Get_ADC_Temperature(ADCD);										//获取内部温度传感器温度
		
		if(DACD<=4095)
		{		
//			USART_DMASend(USART1,"设置DAC输出值:%d,ADC读取值%d\n",DACD,ADCD);
//			USART_DMASend(USART1,"%d\n",ADCD);
			USART_DMAPrintf(USART1,"当前STM32内部温度为：%4.1f℃\n",Temperature);
//			USART_DMASend(USART1,"%d\n",DMA1_Channel1->CMAR);
			
			DACD++;
		}
		else
			DACD=0;
	}
	nump++;
	if(nump<=20)
	{
//		GPIO_Write(GPIOE,0XAAAA);
//		GPIO_SetBits(GPIOA,GPIO_Pin_3);
		GPIO_ResetBits(GPIOA,GPIO_Pin_3);
//		nump++;
	}
	else
	{
//		GPIO_Write(GPIOE,0X5555);
		GPIO_SetBits(GPIOA,GPIO_Pin_3);
//		GPIO_ResetBits(GPIOA,GPIO_Pin_3);
		if(nump>40)
		{
			nump=0;
		}
		
	}
}


//////////////////////////////////////////////////////////
/*
今天学习STM32的内部自带12位ADC，

1、特点：

（1）12位分辨率，最短时钟周期为14个，时钟周期可调，分别可以调整为14、20、26、41、54、68、252，因此当时钟为14MHz时候，最快转换时间为1us；

（2）供电电压为2.4V到3.6V，注意stm32的最低供电电压可以是2V，但是使用ADC时候，必须达到2.4V以上；

（3）输入电压范围：Vref- 

（4）最小量化单位为：LSB=Vref+/4096mV;

（5）为逐次比较型AD；

2、处理AD转换的方法有两个：一个是常用的中断，另一个是DMA，相对来说，DMA模式下的效率要高，所以这里研究DMA下的ADC。

3、实验目标：通过ADC1的11通道采集外部电压，然后又DMA传送到缓存，然后通过串口发到到PC上。

4、实验程序：


//#include"stm32f10x.h"

//#include"adc.h"

*/ 

 /******************

  function name：ADC1_Init()

  discribe ：ADC1初始化函数，调用配置函数和模式选择函数

  input：null

  output:null

 *******************/

  void ADC1_Init(void)

{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	ADC1_GPIO_Config();

	ADC1_Mode_Config(); 

}

 

/******************

  function name：ADC1_GPIO_Config()

  discribe ：ADC1 config function

     (1)enable ADC,GPIO,DMA's clock,

(2)ADC1's IO is PC1,Mode is analog input(AIN)

  input：null

  output:null

  return: null

 *******************/

 void ADC1_GPIO_Config(void)

 {
GPIO_InitTypeDef GPIO_InitStructure;					//GPIO结构体
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);		//使能GPIOA时钟
	 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;							//GPIO_Pin_x
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;					//模拟输入
	GPIO_Init(GPIOA,&GPIO_InitStructure);	   

 }

 

/******************

  function name：ADC1_Mode_Config()

  discribe ：ADC1  mode configuration

     (1)config DMA

(2)config ADC1

  input：null

  output:null

  return:null

 *******************/

 void ADC1_Mode_Config(void)

 {

DMA_InitTypeDef DMA_InitStructure;

ADC_InitTypeDef ADC_InitStructure;

 

/*******config DMA channel 1 ********/

DMA_DeInit(DMA1_Channel1);   //reset DMA channel register

 //define the peripheral base address

DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)&ADC1->DR; 

//define the memory base address

DMA_InitStructure.DMA_MemoryBaseAddr=(u32)&tempr;

//choose the peripheral is the destination or source

DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;

//set the DMA buffer size 缓存大小

DMA_InitStructure.DMA_BufferSize=1; //16bits

//set the peripheral address register is incremented or not 递增与否

DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;

//set the memory address register is incremented or not 

DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Disable;

//set the peripheral data size as 16bits(halfword)

DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;

//set the memory data size 

DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;

//set DMA mode

DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;

//set the channel priority 

DMA_InitStructure.DMA_Priority=DMA_Priority_High;

//enable the DMA memory to memory transfer

DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;

 

DMA_Init(DMA1_Channel1,&DMA_InitStructure);

/*****enable channel 1******/

DMA_Cmd(DMA1_Channel1,ENABLE);

 

/*****independent scanmode none external ,right align,***/

ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;

ADC_InitStructure.ADC_ScanConvMode=ENABLE;

ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;

ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;

ADC_InitStructure.ADC_NbrOfChannel=1;

ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;

ADC_Init(ADC1, &ADC_InitStructure);
 

 //set the ADC clock

RCC_ADCCLKConfig(RCC_PCLK2_Div8);//PCLK2/8=9Mhz

 //set the AD sampling time  55.5个采样周期

ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_55Cycles5);

 //enable the ADC begins DMA transfer

ADC_DMACmd(ADC1,ENABLE);

//enable the ADC1

ADC_Cmd(ADC1,ENABLE);

//enable ADC1 reset the calibration register

ADC_ResetCalibration(ADC1);

//when reset is ok ,it returns 0;if not ,wait

while(ADC_GetResetCalibrationStatus(ADC1));

 

//begin ADC1 calibration

ADC_StartCalibration(ADC1);

//wait untill the calibration is finished when it will return 0

while(ADC_GetCalibrationStatus(ADC1));

//software starts the conversation 

ADC_SoftwareStartConvCmd(ADC1,ENABLE);

}








#endif
