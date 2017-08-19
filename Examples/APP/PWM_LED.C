#ifdef PWM_LED
#include "PWM_LED.H"

#include "string.h"
#include "math.h"


#include "STM32_EXTI.H"
#include "STM32_USART.H"
#include "STM32_WDG.H"

#include "STM32_SYS.H"
#include "STM32_SYSTICK.H"
#include "STM32_GPIO.H"
#include "STM32F10x_BitBand.H"

#include "stm32f10x_flash.h" //flash操作接口文件（在库文件中），必须要包含 


#define	STARTADDR 0x08010400 	//STM32F103RB 其他型号基本适用，未测试
#define	InfoAddr		5000	//20*1024+0x08010400		//存储有效起始地址信息（FLASH块坏了后切换地址）

u32	BaseAddr;		//备份数据存储起始基地址
u32	UsedAddr;		//备份数据存储起始基地址


FLASH_Status	FLASHStatus;


u32	BackUpPage=0;
u32 PWM_CountBAC=0;		//PWM_Count备份值

#define RxBufferSize	128

#define StepLen	50		//步长
#define StepMax	1201	//总步数，包含0
#define CountMax	60000	//总步数，包含0

u32 PWM_Count=10000;		//占空比计数器 计时范围0~60000
u32	PWM_STEP[StepMax]={0};		//1200步每步数据
u16 STEP=0;			//输出数组序号

double Npow=0.0;

u8 	ButtonFlag=0;	//按键
u32 ButtonCount=0;	//按键计时

u32 TimeCount=0;
u32 TimeSave=0;			//10秒更新一次备份数据
u32 ECount=0;		//旋转编码器干扰处理变量
u32 ECountA=0;
u32 ECountB=0;

u8 RevBuffer[RxBufferSize]={0};
u8 RxdBuffer[RxBufferSize]={0};
u8 RxNum=0;

void CheckFlash(void);
void WriteBackUp(void);	//先对比数据，如果数据不同再重新备份数据
void PWM_ECODE(void);		//编码器
void USART_UCODE(void);	//通过串口设置时间比
u32 ReadFlashNBtye(u32 ReadAddress, u8 *ReadBuf, u32 ReadNum);
void WriteFlashOneWord(u32 WriteAddress,u32 WriteData);
void SetDuty(void);			//设置比较时间
void Init_Data(void);		//初始化输出数据
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PWM_LED_Configuration(void)
{
	SYS_Configuration();				//系统配置
	GPIO_DeInitAll();													//将所有的GPIO关闭----V20170605
	

	PWM_OUT2(TIM3,PWM_OUTChannel3,1200,0);		//PWM设定
	PWM_OUT2(TIM3,PWM_OUTChannel4,1200,0);		//PWM设定

	
	GPIO_Configuration_IPU(GPIOB,	GPIO_Pin_12);			//将GPIO相应管脚配置为上拉输入模式----V20170605--BUTTON
	GPIO_Configuration_IPU(GPIOB,	GPIO_Pin_13);			//将GPIO相应管脚配置为上拉输入模式----V20170605--A
	GPIO_Configuration_IPU(GPIOB,	GPIO_Pin_14);			//将GPIO相应管脚配置为上拉输入模式----V20170605--B
	
	USART_DMA_ConfigurationNR	(USART1,115200,(u32*)RxdBuffer,RxBufferSize);	//USART_DMA配置--查询方式，不开中断	
	
//	USART_DMAPrintf(USART1,"参数设置方法1（十六进制:FE A9 XX XX）\r\n");					//自定义printf串口DMA发送程序,后边的省略号就是可变参数
	
	for(TimeCount=0;TimeCount<0xFFFF;TimeCount++)
	{
	}
	USART_DMAPrintf(USART1,"\r\n使用串口设置参数方法2（SET:xx (xx取值为0~60000,精度1/60000--0.02%%））\r\n\r\n");					//自定义printf串口DMA发送程序,后边的省略号就是可变参数
	for(TimeCount=0;TimeCount<0xFFFF;TimeCount++)
	{
	}
	USART_DMAPrintf(USART1,"按键使用：精度10%%，按键一次增加10%%，增加到100%%后再按键回到0%%\r\n\r\n");					//自定义printf串口DMA发送程序,后边的省略号就是可变参数
	for(TimeCount=0;TimeCount<0xFFFF;TimeCount++)
	{
	}
	USART_DMAPrintf(USART1,"旋转编码器：精度1/1200\r\n\r\n");					//自定义printf串口DMA发送程序,后边的省略号就是可变参数
	for(TimeCount=0;TimeCount<0xFFFF;TimeCount++)
	{
	}
//	WriteFlashOneWord(InfoAddr,STARTADDR);
	CheckFlash();	//开机检测FLASH数据
//	WriteFlashOneWord(InfoAddr,STARTADDR+1024);
//	ReadFlashNBtye(InfoAddr,(u8*)(&BaseAddr), 4);
	
	IWDG_Configuration(2000);					//独立看门狗配置---参数单位ms	
	SysTick_Configuration(100);	//系统嘀嗒时钟配置72MHz,单位为uS
	Init_Data();		//初始化输出数据
//	SetDuty();			//设置比较时间
}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void PWM_LED_Server(void)
{	
	TimeCount++;
	TimeSave++;			//10秒后更新一次备份数据
	if(TimeCount>=10000)
	{
		TimeCount=0;
		IWDG_Feed();								//独立看门狗喂狗2秒
//		WriteBackUp();			//先对比数据，如果数据不同再重新备份数据
	}
	if(TimeSave>=50000)	//5秒更新一次备份数据
	{
		TimeSave=0;					//10秒更新一次备份数据
		WriteBackUp();			//先对比数据，如果数据不同再重新备份数据
	}
	USART_UCODE();		//通过串口设置时间比
	PWM_ECODE();			//编码器
	
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void CheckFlash(void)
{	
//	WriteFlashOneWord(InfoAddr,STARTADDR);
	ReadFlashNBtye(InfoAddr,(u8*)(&BaseAddr), 4);								//获取默认起始备份数据起始地址（正常情况下为STARTADDR）
	if(BaseAddr==0xFFFFFFFF)
	{
		WriteFlashOneWord(InfoAddr,STARTADDR);
	}
	else
	{
		UsedAddr=BaseAddr-STARTADDR;
		ReadFlashNBtye(UsedAddr,(u8*)(&PWM_CountBAC), 4);		//获取默认起始备份数据起始地址（正常情况下为STARTADDR）
		WriteFlashOneWord(UsedAddr,PWM_CountBAC);

		if(PWM_CountBAC<=60000)
		{
			PWM_Count=PWM_CountBAC;
			STEP=STEP=PWM_Count/StepLen;
			
			Npow=(double)PWM_Count/StepLen;
			
			TIM3->CCR3 =PWM_Count;
			TIM3->CCR4 =PWM_Count;
			
			USART_DMAPrintf(USART1,"前次备份记录:%5d/60000；\t步数:%7.2f/1200；\t 占空比：%7.3f%%\r\n",PWM_Count,Npow,Npow/1200*100);					//自定义printf串口DMA发送程序,后边的省略号就是可变参数
		}
	}
}
/*******************************************************************************
*函数名			:	WriteBackUp
*功能描述		:	先对比数据，如果数据不同再重新备份数据
*输入				: 
*返回值			:	无
*******************************************************************************/
void WriteBackUp(void)	//先对比数据，如果数据不同再重新备份数据
{
//	u8 ReTry=0;
//	WriteFlashOneWord(InfoAddr,STARTADDR);
	ReadFlashNBtye(InfoAddr,(u8*)(&BaseAddr), 4);							//获取默认起始备份数据起始地址（正常情况下为STARTADDR）
	UsedAddr=BaseAddr-STARTADDR;
	ReadFlashNBtye(UsedAddr,(u8*)(&PWM_CountBAC), 4);		//获取默认起始备份数据起始地址（正常情况下为STARTADDR）
	if(PWM_CountBAC!=PWM_Count)
	{
		PWM_CountBAC=PWM_Count;
		WriteFlashOneWord(UsedAddr,PWM_CountBAC);
		
		ReadFlashNBtye(UsedAddr,(u8*)(&PWM_CountBAC), 4);		//获取默认起始备份数据起始地址（正常情况下为STARTADDR）
		
//		if(PWM_CountBAC!=PWM_Count)
//		{
////			ReadFlashNBtye(InfoAddr,(u8*)(&BaseAddr), 4);							//获取默认起始备份数据起始地址（正常情况下为STARTADDR）
////			WriteFlashOneWord(InfoAddr,BaseAddr+1);		//存储新地址
//			WriteFlashOneWord(InfoAddr,STARTADDR+10);
//			WriteBackUp();
//		}		
	}
	ReadFlashNBtye(UsedAddr,(u8*)(&PWM_Count), 4);		//获取默认起始备份数据起始地址（正常情况下为STARTADDR）
}
/****************************************************************
*Name: ReadFlashNBtye
*Function: 从内部Flash读取N字节数据
*Input: ReadAddress：数据地址（偏移地址）ReadBuf：数据指针 ReadNum：读取字节数
*Output: 读取的字节数
*Author: ValerianFan
****************************************************************/
u32 ReadFlashNBtye(u32 ReadAddress, u8 *ReadBuf, u32 ReadNum)
{
	int DataNum = 0;
	ReadAddress = (u32)STARTADDR + ReadAddress;
	while(DataNum < ReadNum)
	{
		*(ReadBuf + DataNum) = *(u8*) ReadAddress++;
		DataNum++;
	}
	return DataNum; 
}
/**************************************************************** 

*Name: WriteFlashOneWord
*Function: 向内部Flash写入32位数据 
*Input: WriteAddress：数据地址（偏移地址）WriteData：写入数据
*Output: NULL 
*Other:
****************************************************************/
void WriteFlashOneWord(u32 WriteAddress,u32 WriteData)
{
	FLASH_Unlock();		//解锁FLASH编写擦除控制器
//	FLASH_UnlockBank1();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	FLASHStatus = FLASH_ErasePage(STARTADDR);
	if(FLASHStatus == FLASH_COMPLETE)
	{
		FLASHStatus = FLASH_ProgramWord(STARTADDR + WriteAddress, WriteData); //flash.c 中API函数
		//FLASHStatus = FLASH_ProgramWord(StartAddress+4, 0x56780000); //需要写入更多数据时开启
		//FLASHStatus = FLASH_ProgramWord(StartAddress+8, 0x87650000); //需要写入更多数据时开启
	}
//	FLASH_LockBank1();
	FLASH_Lock();			//锁定FLASH编写擦除控制器
} 

/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void USART_UCODE(void)		//通过串口设置时间比
{
	RxNum=USART_ReadBufferIDLE	(USART1,(u32*)RevBuffer,(u32*)RxdBuffer);	//串口空闲模式读串口接收缓冲区，如果有数据，将数据拷贝到RevBuffer,并返回接收到的数据个数，然后重新将接收缓冲区地址指向RxdBuffer
	if(RxNum)
	{
		TimeSave=0;					//10秒更新一次备份数据
		if(RevBuffer[0]==0xFE&&RevBuffer[1]==0xA9)
		{
//			PWM_Ratio=(RevBuffer[2]<<8)|(RevBuffer[3]<<0);
//			
//			if(PWM_Ratio>1200)
//				PWM_Ratio=1200;
//			USART_DMAPrintf(USART1,"当前输出比:%5d/30000；\t等级比:%4.2f/1200；\t 占空比：%4.3f%%\r\n",PWM_Ratio*25,PWM_Ratio,PWM_Ratio/1200*100);					//自定义printf串口DMA发送程序,后边的省略号就是可变参数
//			TIM3->CCR3 =PWM_Ratio*STep;
//			TIM3->CCR4 =PWM_Ratio*STep;
		}
		else if(RevBuffer[0]==0x53&&RevBuffer[1]==0x45&&RevBuffer[2]==0x54&&RevBuffer[3]==0x3A)
		{
			u8 i=0;

			PWM_Count=0;
			if(RxNum>=9&&RevBuffer[4]>=0x36)
			{
				PWM_Count=CountMax;
			}
			else
			{
				RxNum=RxNum-4;
				while(RxNum>i)
				{
					i++;
					Npow=(double)pow((float)10,(float)(RxNum-i));
					
					PWM_Count+=(u16)(RevBuffer[i+3]-0x30)*Npow;				
				}
			}
			STEP=(double)PWM_Count/StepLen;
//			SetDuty();	//设置比较时间
			Npow=(double)PWM_Count/StepLen;
//			PWM_Ratio	=(u16)Npow;
			TIM3->CCR3 =PWM_Count;
			TIM3->CCR4 =PWM_Count;
			USART_DMAPrintf(USART1,"当前输出比:%5d/60000；\t步数:%7.2f/1200；\t 占空比：%7.3f%%\r\n",PWM_Count,Npow,Npow/1200*100);					//自定义printf串口DMA发送程序,后边的省略号就是可变参数
		}
		memset(RevBuffer, 0x00,16);
	}
}
/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void PWM_ECODE(void)
{
	//-----------------------按键****旋钮时按键无效
	if(PB12in==0&&ECountA==0&&ECountB==0)			//EXTI_GetFlagStatus(u32 EXTI_Line);//PA6in==0
	{
		if(ButtonFlag==0)
		{
			ButtonCount++;	//按键计时
			if(ButtonCount>=500)		//按键时间大于50ms才有效
			{
				TimeSave=0;					//10秒更新一次备份数据
				ButtonFlag=1;				//防止长按
				
				STEP=((STEP/((StepMax-1)/10))*((StepMax-1)/10));	//1200  12		//STEP转换成10%的倍数值	120的倍数
				STEP=STEP+(StepMax-1)/10;	//1200  12
				
				if(STEP>StepMax)
				{
					STEP=0;
				}

				SetDuty();	//设置比较时间
				Npow=(double)PWM_Count/StepLen;
				USART_DMAPrintf(USART1,"当前输出比:%5d/60000；\t步数:%7d/1200；\t 占空比：%7.3f%%\r\n",PWM_Count,STEP,Npow/1200*100);					//自定义printf串口DMA发送程序,后边的省略号就是可变参数	
			}
		}
	}
	else
	{
		ButtonCount=0;	//按键计时
		ButtonFlag=0;
	}
	//-----------------------旋钮****按键时旋钮无效
	if(PB13in==0&&ECountA<0xFF&&ECountB<0xFF&&ButtonCount==0)			//EXTI_GetFlagStatus(u32 EXTI_Line);//PA6in==0
	{
		ECountA++;
	}
	if(PB14in==0&&ECountA<0xFF&&ECountB<0xFF&&ButtonCount==0)
	{
		ECountB++;
	}
	if((PB13in&&PB14in)&&(ECountB>5&&ECountA>5))	//旋钮都在空置状态，并且计时大于1ms，防止干扰，旋钮才有效
	{
		TimeSave=0;					//10秒更新一次备份数据
		STEP=PWM_Count/StepLen;
		if(ECountA>ECountB)
		{
			if(STEP<StepMax-1)
				STEP+=1;
		}
		else
		{
			if(STEP>0)
				STEP-=1;
		}
		
		SetDuty();	//设置比较时间	
			
		Npow=(double)((PWM_Count/60000.0)*100);
		
		USART_DMAPrintf(USART1,"当前输出比:%5d/60000；\t步数:%7d/1200；\t 占空比：%7.3f%%\r\n",PWM_Count,STEP,(double)Npow);					//自定义printf串口DMA发送程序,后边的省略号就是可变参数					

		ECountA=0;
		ECountB=0;
	}
	else if(PB13in&&PB14in)		//编码器AB两脚都为空置(高电平)状态，则表示编码器在空闲状态，清除计时
	{
		ECountA=0;
		ECountB=0;
	}
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void SetDuty(void)
{
	PWM_Count=PWM_STEP[STEP];
	TIM3->CCR3 =PWM_Count;
	TIM3->CCR4 =PWM_Count;
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void Init_Data(void)
{
	u16 i=STEP;
	PWM_STEP[STEP]=0;
	for(i=1;i<StepMax;i++)
	{
		PWM_STEP[i]=PWM_STEP[i-1]+StepLen;
	}
//	STEP=600;		//开机默认中间值 50%
//	PWM_STEP[601]=30002;
//	PWM_Count=PWM_STEP[STEP];			//取计时值
}
/*******************************************************************************
*函数名			:	function
*功能描述		:	函数功能说明
*输入				: 
*返回值			:	无
*******************************************************************************/
void PWM_Count_BackUp(void)
{

}

#endif
