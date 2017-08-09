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

#include "STM32_ADC.H"

/*##############################################################################
################################################################################
# 模块名		:	STM32_ADC	
# 功能描述	:	ADC配置使用
-------------------------------------------------------------------------------	
********ADC介绍:********
12位ADC是一种逐次逼近型模拟数字转换器。它有多达18个通道，可测量16个外部和2个内部信号源。
各通道的A/D转换可以单次、连续、扫描或间断模式执行。ADC的结果可以左对齐或右对齐方式存储在
16位数据寄存器中。 模拟看门狗特性允许应用程序检测输入电压是否超出用户定义的高/低阀值。 
ADC的输入时钟不得超过14MHz，它是由PCLK2经分频产生。
-------------------------------------------------------------------------------
********ADC主要特征:********
● 12位分辨率
● 转换结束、注入转换结束和发生模拟看门狗事件时产生中断 
● 单次和连续转换模式 
● 从通道0到通道n的自动扫描模式
● 自校准 
● 带内嵌数据一致性的数据对齐 
● 采样间隔可以按通道分别编程 
● 规则转换和注入转换均有外部触发选项 
● 间断模式 
● 双重模式(带2个或以上ADC的器件) 
● ADC转换时间： 
	─ STM32F103xx增强型产品：时钟为56MHz时为1μs(时钟为72MHz为1.17μs) 
	─ STM32F101xx基本型产品：时钟为28MHz时为1μs(时钟为36MHz为1.55μs) 
	─ STM32F102xxUSB型产品：时钟为48MHz时为1.2μs 
	─ STM32F105xx和STM32F107xx产品：时钟为56MHz时为1μs(时钟为72MHz为1.17μs) 
● ADC供电要求：2.4V到3.6V 
● ADC输入范围：VREF- ≤ VIN ≤ VREF+ 
● 规则通道转换期间有DMA请求产生。

************************ADC管脚:依据STM32F107VC引脚分布***********************
*	CH0	CH1	CH2	CH3	CH4	CH5	CH6	CH7	CH8	CH9	CH10	CH11	CH12	CH13	CH14	CH15
*	PA0	PA1	PA2	PA3	PA4	PA5	PA6	PA7	PB0	PB1	PC0		PC1		PC2		PC3		PC4		PC5				
*****************************************************************************

################################################################################
###############################################################################*/




#define N 50 //每通道采50 次
#define M 12 //为12 个通道
vu16 AD_Value[N][M]; //用来存放ADC 转换结果，也是DMA 的目标地址
vu16 After_filter[M]; //用来存放求平均值之后的结果

/*******************************************************************************
* 函数名		:	ADC1_InjectedConfiguration	
* 功能描述	:	ADC1注入通道组配置 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void ADC1_InjectedConfiguration(void)			//ADC1注入通道组配置
{	
}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	ADC1规则通道组配置 
* 输入		:	ADC_Channel_x->通道号:ADC_Channel_0~ADC_Channel_15
						ADC_mode->
						ADC_NbrOfChannel->顺序进行规则转换的ADC通道的数目
* 输出		:
* 返回 		:
*******************************************************************************/
void ADC1_DiscConfiguration(u32 *ADC_DATA,u32 DMA_BufferSize,u8 ADC_Channel_x,u8 ADC_NbrOfChannel, u8 Rank, u8 ADC_SampleTime)				//ADC1规则通道组配置
{
	//1)**********打开ADC时钟，并设置分频因子	
	//由时钟控制器提供的ADCCLK时钟和PCLK2(APB2时钟)同步。
	//RCC控制器为ADC时钟提供一个专用的可编程预分频器。
//	if((RCC->APB2ENR&RCC_APB2Periph_ADC1)!=RCC_APB2Periph_ADC1)
//	{
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//使能GPIOA和复用时钟
//		RCC_ADCCLKConfig(RCC_PCLK2_Div6);	//72M/6=12,ADC最大时间不能超过14M
//	}	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//使能ADC时钟
	//2)**********打开相应GPIO时钟，配置GPIO	
	ADC_PinConf(ADC_Channel_x);				//ADC管脚配置 依据STM32F107VC引脚分布	
	//5)**********设置ADC的规则系列相关信息---通道选择
	ADC1_DMAConfiguration(ADC_DATA,DMA_BufferSize); 		//ADC1 DMA配置	
	//3)**********设置ADC的工作模式
	ADC1_InitStructure(ADC_NbrOfChannel);				//ADC初始化	
	//4)**********设置ADC的规则系列相关信息---通道选择
	//ADC1,ADC 通道x,规则采样顺序值为y,采样时间为239.5 周期
	ADC_RegularChannelConfig(ADC1, ADC_Channel_x, Rank,ADC_SampleTime);
	
	//6)**********开启ADC转换并校准	
	ADC1_Cmd(ENABLE);					//开启ADC1转换
}
/*******************************************************************************
*函数名		:	ADC_TempSensorConfiguration
*功能描述	:	STM32内部温度传感器配置
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void ADC_TempSensorConfiguration(u32 *ADC_DATA)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//使能ADC时钟
	ADC1_InitStructure(1);				//ADC初始化
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1,ADC_SampleTime_239Cycles5);
	ADC1_DMAConfiguration(ADC_DATA,1); 		//ADC1 DMA配置
	ADC_DMACmd(ADC1, ENABLE);   //ADC命令，使能 
	ADC_TempSensorVrefintCmd(ENABLE);	//使能温度传感器和内部参考电压通道	
	ADC1_Cmd(ENABLE);					//开启ADC1转换
	
	ADC_ResetCalibration(ADC1);   //重新校准 
	while(ADC_GetResetCalibrationStatus(ADC1));  //等待重新校准完成
	ADC_StartCalibration(ADC1); 	//开始校准
	while(ADC_GetCalibrationStatus(ADC1));    //等待校准完成
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); 	//连续转换开始，ADC通过DMA方式不断的更新RAM区。

}
/*******************************************************************************
*函数名		:	Get_ADC_Temperature
*功能描述	:	获取内部温度传感器温度
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
float Get_ADC_Temperature(u32 ADC_DATA)
{
	float Result=0.0;
	Result=(1.43-ADC_DATA*3.3/4096)*1000/4.35 + 25;
	return Result;

}
/*******************************************************************************
* 函数名		:	ADC_PinConf
* 功能描述	:	打开相应通道的GPIO时钟，将相应的GPIO_Pin配置为模拟输入模式 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void	ADC_PinConf(u8 ADC_Channel_x)				//ADC管脚配置 依据STM32F107VC引脚分布
{
	GPIO_InitTypeDef GPIO_InitStructure;					//GPIO结构体
	GPIO_TypeDef* GPIOx=0;
	u16 GPIO_Pin_x=0;
//	ErrorStatus status = ERROR;
	/*****************************ADC管脚****************************************
	*	CH0	CH1	CH2	CH3	CH4	CH5	CH6	CH7	CH8	CH9	CH10	CH11	CH12	CH13	CH14	CH15
	*	PA0	PA1	PA2	PA3	PA4	PA5	PA6	PA7	PB0	PB1	PC0		PC1		PC2		PC3		PC4		PC5				
	*****************************************************************************/
	//1)**********打开相应GPIO时钟	
	if(ADC_Channel_x<=ADC_Channel_7)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);		//使能GPIOA时钟
	}
	else if(ADC_Channel_x>ADC_Channel_7&&ADC_Channel_x<=ADC_Channel_9)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);		//使能GPIOB时钟
	}
	else if(ADC_Channel_x>ADC_Channel_9&&ADC_Channel_x<=ADC_Channel_15)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);		//使能GPIOC时钟
	}
	else
	{
	}
	//2)**********选择相应GPIO
	switch (ADC_Channel_x)
	{
		case	ADC_Channel_0:	GPIOx=GPIOA;	GPIO_Pin_x=GPIO_Pin_0;	break;
		case	ADC_Channel_1:	GPIOx=GPIOA;	GPIO_Pin_x=GPIO_Pin_1;	break;
		case	ADC_Channel_2:	GPIOx=GPIOA;	GPIO_Pin_x=GPIO_Pin_2;	break;
		case	ADC_Channel_3:	GPIOx=GPIOA;	GPIO_Pin_x=GPIO_Pin_3;	break;
		case	ADC_Channel_4:	GPIOx=GPIOA;	GPIO_Pin_x=GPIO_Pin_4;	break;
		case	ADC_Channel_5:	GPIOx=GPIOA;	GPIO_Pin_x=GPIO_Pin_5;	break;
		case	ADC_Channel_6:	GPIOx=GPIOA;	GPIO_Pin_x=GPIO_Pin_6;	break;
		case	ADC_Channel_7:	GPIOx=GPIOA;	GPIO_Pin_x=GPIO_Pin_7;	break;
		case	ADC_Channel_8:	GPIOx=GPIOB;	GPIO_Pin_x=GPIO_Pin_0;	break;
		case	ADC_Channel_9:	GPIOx=GPIOB;	GPIO_Pin_x=GPIO_Pin_1;	break;
		case	ADC_Channel_10:	GPIOx=GPIOC;	GPIO_Pin_x=GPIO_Pin_0;	break;
		case	ADC_Channel_11:	GPIOx=GPIOC;	GPIO_Pin_x=GPIO_Pin_1;	break;
		case	ADC_Channel_12:	GPIOx=GPIOC;	GPIO_Pin_x=GPIO_Pin_2;	break;
		case	ADC_Channel_13:	GPIOx=GPIOC;	GPIO_Pin_x=GPIO_Pin_3;	break;
		case	ADC_Channel_14:	GPIOx=GPIOC;	GPIO_Pin_x=GPIO_Pin_4;	break;
		case	ADC_Channel_15:	GPIOx=GPIOC;	GPIO_Pin_x=GPIO_Pin_5;	break;
		
		default :break;					
	}
	//3)**********配置GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;							//GPIO_Pin_x
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;					//模拟输入
	GPIO_Init(GPIOx,&GPIO_InitStructure);									//GPIO初始化
}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	ADC_NbrOfChannel>顺序进行规则转换的ADC通道的数目
* 输出		:
* 返回 		:
*******************************************************************************/
void ADC1_InitStructure(u8 ADC_NbrOfChannel)				//ADC初始化
{
	ADC_InitTypeDef ADC_InitStructure; 						//ADC结构体
//	ADC_DeInit(ADC1); 																												//将外设 ADC1 的全部寄存器重设为缺省值
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 													//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode =ENABLE; 																//模数转换工作在扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 													//模数转换工作在连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 				//转换由软件而不是外部触发启动--外部触发转换关闭
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 											//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = ADC_NbrOfChannel; 											//顺序进行规则转换的ADC通道的数目
//	ADC_InitStructure.ADC_NbrOfChannel = ADC_InitStructure.ADC_NbrOfChannel++;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure); 																				//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器
	
//	RCC_ADCCLKConfig(RCC_PCLK2_Div8);//PCLK2/8=9Mhz
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_1Cycles5);
//	ADC_DMACmd(ADC1,ENABLE);
//	ADC_Cmd(ADC1,ENABLE);
//	ADC_ResetCalibration(ADC1);
//	while(ADC_GetResetCalibrationStatus(ADC1));
//	ADC_StartCalibration(ADC1);
//	while(ADC_GetCalibrationStatus(ADC1));
//	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}

/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void ADC1_DMAConfiguration(u32 *ADC_DATA,u32 DMA_BufferSize) 		//ADC1 DMA配置
{  
	DMA_InitTypeDef DMA_InitStructure; 
	
	DMA_DeInit(DMA1_Channel1); 																									//将DMA的通道1寄存器重设为缺省值
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR; 									//DMA外设ADC基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_DATA; 											//DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 													//内存作为数据传输的目的地
	DMA_InitStructure.DMA_BufferSize = 1; 													//DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 						//外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; 										//内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //数据宽度为16位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;					//数据宽度为16位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; 														//工作在循环缓存模式---不停地采集数据
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; 												//DMA通道 x拥有高优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 																//DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); 																//根据DMA_InitStruct中指定的参数初始化DMA的通道
	DMA_Cmd(DMA1_Channel1,ENABLE);
}
/*******************************************************************************
* 函数名		:	ADC1_Cmd
* 功能描述	:	开启ADC1转换 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void ADC1_Cmd(FunctionalState NewState)		//开启ADC1转换
{
//	assert_param(IS_FUNCTIONAL_STATE(ENABLE));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
		
//	RCC_ADCCLKConfig(RCC_PCLK2_Div8);//PCLK2/8=9Mhz
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_1Cycles5);
//	ADC_DMACmd(ADC1,ENABLE);
//	ADC_Cmd(ADC1,ENABLE);
//	ADC_ResetCalibration(ADC1);
//	while(ADC_GetResetCalibrationStatus(ADC1));
//	ADC_StartCalibration(ADC1);
//	while(ADC_GetCalibrationStatus(ADC1));
//	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
		
//		ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_1Cycles5);
    ADC_DMACmd(ADC1, ENABLE); 									//开启ADC的DMA支持（要实现DMA功能，还需独立配置DMA通道等参数）ADC_DMACmd(ADC1, ENABLE);
//		DMA_Cmd(DMA1_Channel1,ENABLE);
		ADC_Cmd(ADC1, ENABLE); 											//使能指定的ADC1	
		
		ADC_ResetCalibration(ADC1); 								//复位指定的ADC1的校准寄存器
		
		while(ADC_GetResetCalibrationStatus(ADC1)); //获取ADC1复位校准寄存器的状态,设置状态则等待
		ADC_StartCalibration(ADC1); 								//开始指定ADC1的校准状态
		while(ADC_GetCalibrationStatus(ADC1)); 			//获取指定ADC1的校准程序,设置状态则等待	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);		
  }
  else
  {
    ADC_DMACmd(ADC1, DISABLE); 	//开启ADC的DMA支持（要实现DMA功能，还需独立配置DMA通道等参数）ADC_DMACmd(ADC1, ENABLE);
		ADC_Cmd(ADC1, DISABLE); 		//使能指定的ADC1	
  }
}


//***********************************************************************************************************************************
///*******************************************************************************
//* 函数名		:	
//* 功能描述	:	 
//* 输入		:	
//* 输出		:
//* 返回 		:
//*******************************************************************************/
//void ADC_Configuration(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;				//GPIO结构体
//	ADC_InitTypeDef ADC_InitStructure; 						//ADC结构体
//	
////	u32 ADC_DAAAA;
//	
////	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	//使能GPIOA和复用时钟
////	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);		//使能GPIOA和复用时钟
////	RCC_ADCCLKConfig(RCC_PCLK2_Div6);											//72M/6=12,ADC最大时间不能超过14M 
////	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 		//使能DMA传输

//	
//	//1)**********打开相应GPIO时钟，配置GPIO	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	//使能GPIOA和复用时钟
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;							//PB1
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;					//模拟输入
//	GPIO_Init(GPIOB,&GPIO_InitStructure);	
//	
//	
//	//2)**********打开ADC时钟，并设置分频因子	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//使能GPIOA和复用时钟
//	RCC_ADCCLKConfig(RCC_PCLK2_Div6);	//72M/6=12,ADC最大时间不能超过14M 
//	
//	
//	//3)**********设置ADC的工作模式
//	ADC_DeInit(ADC1); 																											//将外设 ADC1 的全部寄存器重设为缺省值
//	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 											//ADC工作模式:ADC1和ADC2工作在独立模式
//	ADC_InitStructure.ADC_ScanConvMode =ENABLE; 														//模数转换工作在扫描模式
//	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 											//模数转换工作在连续转换模式
//	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 		//外部触发转换关闭
//	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 									//ADC数据右对齐
//	ADC_InitStructure.ADC_NbrOfChannel = 1; 																//顺序进行规则转换的ADC通道的数目
//	ADC_Init(ADC1, &ADC_InitStructure); 																		//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器
//	
//	
//	//4)**********设置ADC的规则系列相关信息
//	//ADC1,ADC 通道x,规则采样顺序值为y,采样时间为239.5 周期
////	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5 );
////	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5 );
////	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5 );
////	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_239Cycles5 );
////	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 5, ADC_SampleTime_239Cycles5 );
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_239Cycles5 );
////	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 7, ADC_SampleTime_239Cycles5 );
////	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 8, ADC_SampleTime_239Cycles5 );
////	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 9, ADC_SampleTime_239Cycles5 );
////	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 10, ADC_SampleTime_239Cycles5 );
////	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 11, ADC_SampleTime_239Cycles5 );
////	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 12, ADC_SampleTime_239Cycles5 );


//	DMA0_Configuration();						//DMA配置	
//	
//	
//	//5)**********开启ADC转换并校准	
//	ADC_DMACmd(ADC1, ENABLE); 			// 开启ADC的DMA支持（要实现DMA功能，还需独立配置DMA通道等参数）ADC_DMACmd(ADC1, ENABLE);   
//	ADC_Cmd(ADC1, ENABLE); 					//使能指定的ADC1

//	ADC_ResetCalibration(ADC1); //复位指定的ADC1的校准寄存器
//	while(ADC_GetResetCalibrationStatus(ADC1)); //获取ADC1复位校准寄存器的状态,设置状态则等待
//	ADC_StartCalibration(ADC1); //开始指定ADC1的校准状态
//	while(ADC_GetCalibrationStatus(ADC1)); //获取指定ADC1的校准程序,设置状态则等待
//	
//	
//	//6)**********读取AD值
//	
//	
//}
///*******************************************************************************
//* 函数名		:	
//* 功能描述	:	 
//* 输入		:	
//* 输出		:
//* 返回 		:
//*******************************************************************************/
//void DMA0_Configuration() 
//{  
//	DMA_InitTypeDef DMA_InitStructure; 
//	
//	u32 ADC_DATA=0;
//	
//	DMA_DeInit(DMA1_Channel1); //将DMA的通道1寄存器重设为缺省值
//	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR; //DMA外设ADC基地址
//	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_DATA; //DMA内存基地址
//	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //内存作为数据传输的目的地
//	DMA_InitStructure.DMA_BufferSize = 1; //DMA通道的DMA缓存的大小
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址寄存器不变
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址寄存器递增
//	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //数据宽度为16位
//	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //数据宽度为16位
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //工作在循环缓存模式
//	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA通道 x拥有高优先级
//	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //DMA通道x没有设置为内存到内存传输
//	DMA_Init(DMA1_Channel1, &DMA_InitStructure); //根据DMA_InitStruct中指定的参数初始化DMA的通道
//}








