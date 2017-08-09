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



#include "STM32_CAN.H"
//#include "STM32F10x_BitBand.H"

/*##############################################################################
################################################################################
# 模块名		:	STM32_CAN	
# 功能描述	:	CAN配置使用

-------------------------------------------------------------------------------	
********bxCAN总体描述:********
在当今的CAN应用中，CAN网络的节点在不断增加，并且多个CAN常常通过网关连接起来，因此整个CAN网中的报文数量(每个节点都需要处理)急剧增加。除了应用层报文外，网络管理和诊断报文也被引入。
● 需要一个增强的过滤机制来处理各种类型的报文
此外，应用层任务需要更多CPU时间，因此报文接收所需的实时响应程度需要减轻。
● 接收FIFO的方案允许，CPU花很长时间处理应用层任务而不会丢失报文。
构筑在底层CAN驱动程序上的高层协议软件，要求跟CAN控制器之间有高效的接口。

********前言:********
● 在CAN协议里，报文的标识符不代表节点的地址，而是跟报文的内容相关的。因此，发送者以广播的形式把报文发送给所有的接收者。节点在接收报文时,根据标识符(CAN ID)的值决定软件是否需要该报文；如果需要，就拷贝到SRAM里；如果不需要，报文就被丢弃且无需软件的干预。
为满足这一需求，bxCAN为应用程序提供了14个位宽可变的、可配置的过滤器组(13~0)，以便只接收那些软件需要的报文。硬件过滤的做法节省了CPU开销，否则就必须由软件过滤从而占用一定的CPU开销。每个过滤器组x由2个32位寄存器，CAN_FxR0和CAN_FxR1组成。
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------	
********bxCAN主要特点:********
bxCAN是基本扩展CAN(Basic Extended CAN)的缩写，它支持CAN协议2.0A和2.0B。它的设计目标是，以最小的CPU负荷来高效处理大量收到的报文。
它也支持报文发送的优先级要求(优先级特性可软件配置)。
对于安全紧要的应用，bxCAN提供所有支持时间触发通信模式所需的硬件功能。
-------------------------------------------------------------------------------
********CAN主要特征:********
● 支持CAN协议2.0A和2.0B主动模式
● 波特率最高可达1兆位/秒
● 支持时间触发通信功能
《发送》
● 3个发送邮箱
● 发送报文的优先级特性可软件配置
● 记录发送SOF时刻的时间戳
《接收》
● 3级深度的2个接收FIFO
● 可变的过滤器组：
	─ 在互联型产品中，CAN1和CAN2分享28个过滤器组
	─ 其它STM32F103xx系列产品中有14个过滤器组
● 标识符列表
● FIFO溢出处理方式可配置
● 记录接收SOF时刻的时间戳
《时间触发通信模式》
● 禁止自动重传模式
● 16位自由运行定时器
● 可在最后2个数据字节发送时间戳
《管理》
● 中断可屏蔽
● 邮箱占用单独1块地址空间，便于提高软件效率
《双CAN》
● CAN1：是主bxCAN，它负责管理在从bxCAN和512字节的SRAM存储器之间的通信
● CAN2：是从bxCAN，它不能直接访问SRAM存储器
● 这2个bxCAN模块共享512字节的SRAM存储器


注：	在中容量和大容量产品中，USB和CAN共用一个专用的512字节的SRAM存储器用于数据的发送和接收，
			因此不同同时使用USB和CAN(共享的SRAM被USB和CAN模块互斥地访问)。USB和CAN可以同时用于一个应用中但不能在同一个时间使用。

*************************************************************************************************************************

################################################################################
###############################################################################*/

//CanRxMsg *RxMessage;

/*******************************************************************************
*函数名		:	DMA_Configuration
*功能描述	:	串口配置
*输入			:	MemoryAddr--内存地址,
						BufferSize--缓存大小
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void CAN_Configuration(void)	//CAN配置
{
	CAN_INIT();
}
/*******************************************************************************
*函数名		:DMA_Server
*功能描述	:DMA中断服务函数
*输入			: 
*输出			:无
*返回值		:无
*例程			:
*******************************************************************************/
void CAN_Server(void)
{
//	CAN_RX_DATA();
	WOW_Server();
//	CAN_RX_DATA(RxMessage);
}
/*******************************************************************************
*函数名		:	CAN_PinSet
*功能描述	:	CAN引脚配置函数
*输入			:	CANRemap_TypeDef CANRemap_Status:
						CANRemapNone:	默认引脚，未重定向
						CANRemap1:	CAN重定义地址1模式
						CANRemap2:	CAN重定义地址1模式
*输出			:	无
*返回值		:	返回配置状态
						SUCCESS:	返回配置成功
						ERROR:	返回配置失败
*例程			:
*******************************************************************************/
ErrorStatus CAN_PinSet(CANRemap_TypeDef CANRemap_Status)						//
{
	GPIO_InitTypeDef GPIO_InitStructure;					//GPIO结构体	
	
	assert_param(IS_CANRemap_Status(CANRemap_Status));
//*****************************CAN默认引脚配置**********************************
	if(CANRemap_Status==CANRemapNone)
	{
		//1.1)**********CAN_RX默认接收引脚配置，PA11
		GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_11; 
		GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IPU;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		//1.2)**********CAN_TX默认发送引脚配置，PA12
		GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_12; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		//1.3)**********返回配置状态
		return SUCCESS;				//返回配置成功
	}
	
//*****************************CAN重定义地址1模式引脚配置**********************************
	else if(CANRemap_Status==CANRemap1)
	{
		//1.1)**********CAN_RX重定义地址1模式接收引脚配置，PB8 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
		GPIO_Init(GPIOB, &GPIO_InitStructure); 
		//1.2)**********CAN_RX重定义地址1模式发送引脚配置，PB9  
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		//1.3)********** Configure CAN Remap 重影射
		GPIO_PinRemapConfig(GPIO_Remap1_CAN, ENABLE);
		//1.4)**********返回配置状态
		return SUCCESS;				//返回配置成功
		
	}

//*****************************CAN重定义地址2模式引脚配置**********************************
	else if(CANRemap_Status==CANRemap2)
	{
		//1.1)**********CAN_RX重定义地址2模式接收引脚配置，PD0 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
		GPIO_Init(GPIOD, &GPIO_InitStructure); 
		//1.2)**********CAN_RX重定义地址2模式发送引脚配置，PD1  
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		//1.3)********** Configure CAN Remap 重影射
		GPIO_PinRemapConfig(GPIO_Remap2_CAN, ENABLE);
		//1.4)**********返回配置状态
		return SUCCESS;				//返回配置成功
	}
	else
		return ERROR;			//返回配置失败
}
/*******************************************************************************
*函数名		:	function
*功能描述	:	函数功能说明
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void CAN_INIT(void)
{
	CAN_InitTypeDef        	CAN_InitStructure;
  
	NVIC_InitTypeDef 				NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO |RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN, ENABLE);

	CAN_PinSet(CANRemapNone);
	
	//1.1)**********CAN初始化 
  CAN_InitStructure.CAN_TTCM=DISABLE;		//CAN_TTCM用来使能或者失能时间触发通讯模式，可以设置这个参数的值为ENABLE或者DISABLE。
																				//->0：禁止时间触发通信模式；
																				//->1：允许时间触发通信模式。
																				//->在该模式下，CAN硬件的内部定时器被激活，并且被用于产生(发送与接收邮箱的)时间戳，分别存储在CAN_RDTxR/CAN_TDTxR寄存器中。
																				//->内部定时器在每个CAN位时间累加。内部定时器在接收和发送的帧起始位的采样点位置被采样，并生成时间戳
	
  CAN_InitStructure.CAN_ABOM=DISABLE;		//CAN_ABOM用来使能或者失能自动离线管理，可以设置这个参数的值为ENABLE或者DISABLE。
																				//该位决定CAN硬件在什么条件下可以退出离线状态。
																				//->如果ABOM位为’1’，bxCAN进入离线状态后，就自动开启恢复过程。
																				//->如果ABOM位为’0’，软件必须先请求bxCAN进入然后再退出初始化模式，随后恢复过程才被开启。
																				//->当TEC大于255时，bxCAN就进入离线状态，同时CAN_ESR寄存器的BOFF位被置’1’。在离线状态下，bxCAN无法接收和发送报文。
	
  CAN_InitStructure.CAN_AWUM=DISABLE;		//CAN_AWUM用来使能或者失能自动唤醒模式，可以设置这个参数的值为ENABLE或者DISABLE。
																				//该位决定CAN处在睡眠模式时由硬件还是软件唤醒
																				//->0：睡眠模式通过清除CAN_MCR寄存器的SLEEP位，由软件唤醒；
																				//->1：睡眠模式通过检测CAN报文，由硬件自动唤醒。唤醒的同时，硬件自动对CAN_MSR寄存器的SLEEP和SLAK位清’0’ 。
	
  CAN_InitStructure.CAN_NART=DISABLE;		//CAN_NARM用来使能或者失能非自动重传输模式，可以设置这个参数的值为ENABLE或者DISABLE。
																				//->0：按照CAN标准，CAN硬件在发送报文失败时会一直自动重传直到发送成功；
																				//->1：CAN报文只被发送1次，不管发送的结果如何(成功、出错或仲裁丢失)。
	
  CAN_InitStructure.CAN_RFLM=DISABLE;		//CAN_RFLM用来使能或者失能接收FIFO锁定模式，可以设置这个参数的值为ENABLE或者DISABLE。
																				//->0：在接收溢出时FIFO未被锁定，当接收FIFO的报文未被读出，下一个收到的报文会覆盖原有的报文；
																				//->1：在接收溢出时FIFO被锁定，当接收FIFO的报文未被读出，下一个收到的报文会被丢弃。
	
  CAN_InitStructure.CAN_TXFP=DISABLE;		//CAN_TXFP用来使能或者失能发送FIFO优先级，可以设置这个参数的值为ENABLE或者DISABLE。
																				//当有多个报文同时在等待发送时，该位决定这些报文的发送顺序
																				//->0：优先级由报文的标识符来决定；
																				//->1：优先级由发送请求的顺序来决定。
	
  CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;			//CAN_Mode设置了CAN的工作模式，<CAN operating mode>.给出了该参数可取的值
																									//CAN_Mode_Normal:CAN硬件工作在正常模式
																									//CAN_Mode_LoopBack:CAN硬件工作在环回模式
																																		//在环回模式下，bxCAN把发送的报文当作接收的报文并保存(如果可以通过接收过滤)在接收邮箱里。
																																		//环回模式可用于自测试。为了避免外部的影响，在环回模式下CAN内核忽略确认错误(在数据/远程帧的确认位时刻，不检测是否有显性位)。
																																		//在环回模式下，bxCAN在内部把Tx输出回馈到Rx输入上，而完全忽略CANRX引脚的实际状态。发送的报文可以在CANTX引脚上检测到。
																									//CAN_Mode_Silent:CAN硬件工作在静默模式
																																		//->在静默模式下，bxCAN可以正常地接收数据帧和远程帧，但只能发出隐性位，而不能真正发送报文。
																																		//->如果bxCAN需要发出显性位(确认位、过载标志、主动错误标志)，那么这样的显性位在内部被接回来从而可以被CAN内核检测到，
																																		//->同时CAN总线不会受到影响而仍然维持在隐性位状态。因此，静默模式通常用于分析CAN总线的活动，
																																		//->而不会对总线造成影响－显性位(确认位、错误帧)不会真正发送到总线上。
																									//CAN_Mode_Silent_LoopBack:CAN硬件工作在静默环回模式
																																		//->可以选择环回静默模式。该模式可用于“热自测试”，即可以像环回模式那样测试bxCAN，但却不会影响CANTX和CANRX所连接的整个CAN系统。
																																		//->在环回静默模式下，CANRX引脚与CAN总线断开，同时CANTX引脚被驱动到隐性位状态。
																																		
			//1.2)**********CAN波特率
	/* Configure the CAN Baud Rate -------------------------------------------*/
//  RCC_GetClocksFreq(&RCC_ClocksStatus);
//  if (usartxbase == USART1_BASE)
//  {
//    apbclock = RCC_ClocksStatus.PCLK2_Frequency;
//  }
//  else
//  {
//    apbclock = RCC_ClocksStatus.PCLK1_Frequency;
//  }
	/*
	CAN时钟是RCC_APB1PeriphClock，你要注意CAN时钟频率 
	CAN波特率 = RCC_APB1PeriphClock/(CAN_SJW+CAN_BS1+CAN_BS2)/CAN_Prescaler; 
	如果CAN时钟为8M， CAN_SJW = 1，CAN_BS1 = 8，CAN_BS2 = 7，CAN_Prescaler = 2 
	那么波特率就是=8M/(1+8+7)/2=250K 
	*/
//******************CAN波特率50K
//  CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;	//CAN_SJW定义了重新同步跳跃宽度(SJW)，即在每位中可以延长或缩短多少个时间单位的上限，<CAN synchronisation jump width>给出了该参数可取的值
//																					//CAN_SJW_1tq:重新同步跳跃宽度1个时间单位
//																					//CAN_SJW_2tq:重新同步跳跃宽度2个时间单位
//																					//CAN_SJW_3tq:重新同步跳跃宽度3个时间单位
//																					//CAN_SJW_4tq:重新同步跳跃宽度4个时间单位
//  CAN_InitStructure.CAN_BS1=CAN_BS1_8tq;	//CAN_BS1设定了时间段1的时间单位数目，<time quantum in bit segment 1>给出了该参数可取的值
//	
//  CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;	//CAN_BS2设定了时间段1的时间单位数目，<time quantum in bit segment 2>给出了该参数可取的值
//	
//  CAN_InitStructure.CAN_Prescaler=45;			//CAN_Prescaler设定了一个时间单位的长度，它的范围是1到1024。


//******************CAN波特率100K
	
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;	//CAN_SJW定义了重新同步跳跃宽度(SJW)，即在每位中可以延长或缩短多少个时间单位的上限，<CAN synchronisation jump width>给出了该参数可取的值
																					//CAN_SJW_1tq:重新同步跳跃宽度1个时间单位
																					//CAN_SJW_2tq:重新同步跳跃宽度2个时间单位
																					//CAN_SJW_3tq:重新同步跳跃宽度3个时间单位
																					//CAN_SJW_4tq:重新同步跳跃宽度4个时间单位
  CAN_InitStructure.CAN_BS1=CAN_BS1_8tq;	//CAN_BS1设定了时间段1的时间单位数目，<time quantum in bit segment 1>给出了该参数可取的值
	
  CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;	//CAN_BS2设定了时间段1的时间单位数目，<time quantum in bit segment 2>给出了该参数可取的值
	
  CAN_InitStructure.CAN_Prescaler=30;			//CAN_Prescaler设定了一个时间单位的长度，它的范围是1到1024。
	
  CAN_Init(&CAN_InitStructure);
	
//	CAN_FilterInitConfiguration(0,0X316);		//CAN滤波器配置
	
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN_RX0_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    
	NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN_TX_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	CAN_ITConfig(CAN_IT_FMP0 | CAN_IT_FF0 | CAN_IT_FOV0, ENABLE);  	// fifo0中断
	CAN_ITConfig(CAN_IT_FMP1 | CAN_IT_FF1 | CAN_IT_FOV1, DISABLE);  // fifo1中断
	CAN_ITConfig(CAN_IT_TME, DISABLE); 
	
	              // 发送中断
		 
	/* Enable CAN FIFO 0 overrun interrupt */
//	CAN_ITConfig(CAN_IT_FOV0, ENABLE);	
}
/*******************************************************************************
* 函数名		:
* 功能描述	: 
* 输入			: 无
* 输出			: 无
* 返回			: 无
*******************************************************************************/
void CAN_FilterInitConfiguration(u8 CAN_GROUP,u16 CAN_ID,u16 MaskId)		//CAN滤波器配置
{
	CAN_FilterInitTypeDef		CAN_FilterInitStructure;
//	CAN_ID=0X317;

  //1.3)**********CAN滤波器初始化
	//1.3.1---------过滤器组
  CAN_FilterInitStructure.CAN_FilterNumber=CAN_GROUP;										//设置过滤器组0，范围为0~13
	//1.3.2---------过滤器的过滤模式
	//● 当过滤器为屏蔽模式时，标识符寄存器对应的ID内容可为任意一需求接收的ID值，
	//● 当同时要接收标准帧和扩展帧时，标识符寄存器对应IDE位也随意设置，
	//● 屏蔽寄存器的IDE位设置为0，表示不关心标准帧还是扩展帧。
	//● 而屏蔽寄存器对应的ID内容为各需求接收的ID值依次异或的结果再取反。
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//滤器将被初始化到的模式
																																//->CAN_FilterMode_IdMask：标识符屏蔽位模式
																																//-->A,为了过滤出一组标识符，应该设置过滤器组工作在屏蔽位模式。
																																//-->B,在屏蔽位模式下，标识符寄存器和屏蔽寄存器一起，指定报文标识符的任何一位，应该按照“必须匹配”或“不用关心”处理。
																																
																																//->CAN_FilterMode_IdList：标识符列表模式
																																//-->A,为了过滤出一个标识符，应该设置过滤器组工作在标识符列表模式。
																																//-->B, 在标识符列表模式下，屏蔽寄存器也被当作标识符寄存器用。因此，不是采用一个标识符加一个屏蔽位的方式，
																																//------而是使用2个标识符寄存器。接收报文标识符的每一位都必须跟过滤器标识符相同。
	//1.3.3---------过滤器位宽																															
  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//过滤器位宽
																																//->CAN_FilterScale_Two16bit：2个16位过滤器	
																																//->CAN_FilterScale_One32bit：1个32位过滤器
	//1.3.4---------标识位寄存器的设置 																															
  CAN_FilterInitStructure.CAN_FilterIdHigh=CAN_ID<<5;						//设定过滤器标识符（32位位宽时为其高段位，16位位宽时为第一个）。它的范围是0x0000到0xFFFF。																																
  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;								//设定过滤器标识符（32位位宽时为其低段位，16位位宽时为第二个）。它的范围是0x0000到0xFFFF。
	//1.3.5---------屏蔽寄存器的设置 
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=MaskId<<5;					//设定过滤器屏蔽标识符或者过滤器标识符（32位位宽时为其高段位，16位位宽时为第一个）。它的范围是0x0000到0xFFFF。
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;						//设定过滤器屏蔽标识符或者过滤器标识符（32位位宽时为其低段位，16位位宽时为第二个）。它的范围是0x0000到0xFFFF。
	//1.3.6---------过滤器组关联
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0;						//设定指向过滤器的FIFO（0或1）此过滤器组关联到接收FIFO0
																																//->CAN_FilterFIFO0：过滤器FIFO0指向过滤器x
																																//->CAN_FilterFIFO1：过滤器FIFO1指向过滤器x
	//1.3.7---------激活此过滤器组																															
  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;					//使能或者失能过滤器。该参数可取的值为ENABLE或者DISABLE。
	//1.3.7---------设置过滤器
  CAN_FilterInit(&CAN_FilterInitStructure);


}

/*******************************************************************************
* 函数名		:
* 功能描述	: 
* 输入			: 无
* 输出			: 无
* 返回			: 无
*******************************************************************************/
void CAN_TX_DATA(u16 CAN_ID,u16 data)
{
	CanTxMsg TxMessage;
	u8	TransmitMailbox = 0;
	u32	i;

	TxMessage.StdId=CAN_ID;
//	TxMessage.ExtId=0XFF;
	TxMessage.RTR=CAN_RTR_DATA;
	TxMessage.IDE=CAN_ID_STD;
	TxMessage.DLC=3;
	TxMessage.Data[0]=0X08;
	TxMessage.Data[1]=(data>>8)&0XFF;
	TxMessage.Data[2]=data&0XFF;
	TxMessage.Data[3]=0x08;
	TxMessage.Data[4]=0x16;
	TxMessage.Data[5]=0x32;
	TxMessage.Data[6]=(data>>8)&0XFF;
	TxMessage.Data[7]=data&0XFF;
	TransmitMailbox = CAN_Transmit(&TxMessage);
	i = 0;
	// 用于检查消息传输是否正常
	while((CAN_TransmitStatus(TransmitMailbox) != CANTXOK) && (i != 0xFF))
	{
		i++;
	}
	i = 0;
	// 检查返回的挂号的信息数目
	while((CAN_MessagePending(CAN_FIFO0) < 1) && (i != 0xFF))
	{
		i++;
	}

}
/*******************************************************************************
* 函数名		:
* 功能描述	: 
* 输入			: 无
* 输出			: 无
* 返回			: 无
*******************************************************************************/
void CAN_RX_DATA(CanRxMsg *RxMessage)
{
	if(SET == CAN_GetITStatus(CAN_IT_FF0))
	{
		CAN_ClearITPendingBit(CAN_IT_FF0);
	}
	else if(SET == CAN_GetITStatus(CAN_IT_FOV0))
	{
		CAN_ClearITPendingBit(CAN_IT_FOV0);
	}
	else
	{
			CAN_Receive(CAN_FIFO0, RxMessage);
	}
}
















