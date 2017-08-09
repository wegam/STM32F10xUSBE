/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : stm32f10x_adc.h
* Author             : MCD Application Team
* Version            : V2.0.1
* Date               : 06/13/2008
* Description        : This file contains all the functions prototypes for the
*                      ADC firmware library.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_ADC_H
#define __STM32F10x_ADC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_map.h"

/* Exported types ------------------------------------------------------------*/
/* ADC Init structure definition */
typedef struct
{
  u32 ADC_Mode;
  FunctionalState ADC_ScanConvMode; 			//ADC_ScanConvMode规定了模数转换工作在扫描模式（多通道）还是单次（单通道）模式。可以设置这个参数为ENABLE或者DISABLE。
  FunctionalState ADC_ContinuousConvMode;	//ADC_ContinuousConvMode规定了模数转换工作在连续还是单次模式。可以设置这个参数为ENABLE或者DISABLE。
  u32 ADC_ExternalTrigConv;								//ADC_ExternalTrigConv定义了使用外部触发来启动规则通道的模数转换，这个参数可以取的值见<ADC extrenal trigger sources for regular channels conversion>.
  u32 ADC_DataAlign;											//ADC_DataAlign规定了ADC数据向左边对齐还是向右边对齐。这个参数可以取的值见<ADC data align>.
  u8 ADC_NbrOfChannel;										//ADC_NbreOfChannel 规定了顺序进行规则转换的ADC通道的数目。这个数目的取值范围是1到16
}ADC_InitTypeDef;

/* Exported constants --------------------------------------------------------*/
#define IS_ADC_ALL_PERIPH(PERIPH) (((*(u32*)&(PERIPH)) == ADC1_BASE) || \
                                   ((*(u32*)&(PERIPH)) == ADC2_BASE) || \
                                   ((*(u32*)&(PERIPH)) == ADC3_BASE))
                                 
#define IS_ADC_DMA_PERIPH(PERIPH) (((*(u32*)&(PERIPH)) == ADC1_BASE) || \
                                   ((*(u32*)&(PERIPH)) == ADC3_BASE))

/* ADC dual mode -------------------------------------------------------------*/		//ADC_Mode设置ADC工作在独立或者双ADC模式
#define ADC_Mode_Independent                       ((u32)0x00000000)								//ADC1和ADC2工作在独立模式
#define ADC_Mode_RegInjecSimult                    ((u32)0x00010000)								//ADC1和ADC2工作在同步规则和同步注入模式
#define ADC_Mode_RegSimult_AlterTrig               ((u32)0x00020000)								//ADC1和ADC2工作在同步规则模式和交替触发模式
#define ADC_Mode_InjecSimult_FastInterl            ((u32)0x00030000)								//ADC1和ADC2工作在同步规则模式和快速交替模式
#define ADC_Mode_InjecSimult_SlowInterl            ((u32)0x00040000)								//ADC1和ADC2工作在同步注入模式和慢速交替模式
#define ADC_Mode_InjecSimult                       ((u32)0x00050000)								//ADC1和ADC2工作在同步注入模式
#define ADC_Mode_RegSimult                         ((u32)0x00060000)								//ADC1和ADC2工作在同步规则模式
#define ADC_Mode_FastInterl                        ((u32)0x00070000)								//ADC1和ADC2工作在快速交替模式
#define ADC_Mode_SlowInterl                        ((u32)0x00080000)								//ADC1和ADC2工作在慢速交替模式
#define ADC_Mode_AlterTrig                         ((u32)0x00090000)								//ADC1和ADC2工作在交替触发模式

#define IS_ADC_MODE(MODE) (((MODE) == ADC_Mode_Independent) || \
                           ((MODE) == ADC_Mode_RegInjecSimult) || \
                           ((MODE) == ADC_Mode_RegSimult_AlterTrig) || \
                           ((MODE) == ADC_Mode_InjecSimult_FastInterl) || \
                           ((MODE) == ADC_Mode_InjecSimult_SlowInterl) || \
                           ((MODE) == ADC_Mode_InjecSimult) || \
                           ((MODE) == ADC_Mode_RegSimult) || \
                           ((MODE) == ADC_Mode_FastInterl) || \
                           ((MODE) == ADC_Mode_SlowInterl) || \
                           ((MODE) == ADC_Mode_AlterTrig))

/* ADC extrenal trigger sources for regular channels conversion --------------*/		//ADC_ExternalTrigConv定义使用外部触发来启动规则通道的模数转换
/* for ADC1 and ADC2 */
#define ADC_ExternalTrigConv_T1_CC1                ((u32)0x00000000)							//选择定时器1的捕获比较1作为转换外部触发
#define ADC_ExternalTrigConv_T1_CC2                ((u32)0x00020000)							//选择定时器1的捕获比较2作为转换外部触发
#define ADC_ExternalTrigConv_T2_CC2                ((u32)0x00060000)							//选择定时器2的捕获比较2作为转换外部触发
#define ADC_ExternalTrigConv_T3_TRGO               ((u32)0x00080000)							//选择定时器3的TRGO作为转换外部触发
#define ADC_ExternalTrigConv_T4_CC4                ((u32)0x000A0000)							//选择定时器4的捕获比较4作为转换外部触发
#define ADC_ExternalTrigConv_Ext_IT11_TIM8_TRGO    ((u32)0x000C0000)							//选择外部中断线11事件作为转换外部触发
/* for ADC1, ADC2 and ADC3 */
#define ADC_ExternalTrigConv_T1_CC3                ((u32)0x00040000)							//选择定时器1的捕获比较3作为转换外部触发
#define ADC_ExternalTrigConv_None                  ((u32)0x000E0000)							//转换由软件而不是外部触发启动--外部触发转换关闭
/* for ADC3 */
#define ADC_ExternalTrigConv_T3_CC1                ((u32)0x00000000)							//选择定时器3的捕获比较1作为转换外部触发
#define ADC_ExternalTrigConv_T2_CC3                ((u32)0x00020000)							//选择定时器2的捕获比较3作为转换外部触发
#define ADC_ExternalTrigConv_T8_CC1                ((u32)0x00060000)							//选择定时器8的捕获比较1作为转换外部触发
#define ADC_ExternalTrigConv_T8_TRGO               ((u32)0x00080000)							//选择定时器8的TRGO作为转换外部触发
#define ADC_ExternalTrigConv_T5_CC1                ((u32)0x000A0000)							//选择定时器5的捕获比较1作为转换外部触发
#define ADC_ExternalTrigConv_T5_CC3                ((u32)0x000C0000)							//选择定时器5的捕获比较3作为转换外部触发

#define IS_ADC_EXT_TRIG(REGTRIG) (((REGTRIG) == ADC_ExternalTrigConv_T1_CC1) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T1_CC2) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T1_CC3) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T2_CC2) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T3_TRGO) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T4_CC4) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_Ext_IT11_TIM8_TRGO) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_None) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T3_CC1) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T2_CC3) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T8_CC1) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T8_TRGO) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T5_CC1) || \
                                  ((REGTRIG) == ADC_ExternalTrigConv_T5_CC3))

/* ADC data align ------------------------------------------------------------*/
#define ADC_DataAlign_Right                        ((u32)0x00000000)							//ADC数据右对齐
#define ADC_DataAlign_Left                         ((u32)0x00000800)							//ADC数据左对齐

#define IS_ADC_DATA_ALIGN(ALIGN) (((ALIGN) == ADC_DataAlign_Right) || \
                                  ((ALIGN) == ADC_DataAlign_Left))

/* ADC channels --------------------------------------------------------------*/
#define ADC_Channel_0                               ((u8)0x00)							//ADC_Channel_0
#define ADC_Channel_1                               ((u8)0x01)
#define ADC_Channel_2                               ((u8)0x02)
#define ADC_Channel_3                               ((u8)0x03)
#define ADC_Channel_4                               ((u8)0x04)
#define ADC_Channel_5                               ((u8)0x05)
#define ADC_Channel_6                               ((u8)0x06)
#define ADC_Channel_7                               ((u8)0x07)
#define ADC_Channel_8                               ((u8)0x08)
#define ADC_Channel_9                               ((u8)0x09)
#define ADC_Channel_10                              ((u8)0x0A)
#define ADC_Channel_11                              ((u8)0x0B)
#define ADC_Channel_12                              ((u8)0x0C)
#define ADC_Channel_13                              ((u8)0x0D)
#define ADC_Channel_14                              ((u8)0x0E)
#define ADC_Channel_15                              ((u8)0x0F)
#define ADC_Channel_16                              ((u8)0x10)
#define ADC_Channel_17                              ((u8)0x11)

#define IS_ADC_CHANNEL(CHANNEL) (((CHANNEL) == ADC_Channel_0) || ((CHANNEL) == ADC_Channel_1) || \
                                 ((CHANNEL) == ADC_Channel_2) || ((CHANNEL) == ADC_Channel_3) || \
                                 ((CHANNEL) == ADC_Channel_4) || ((CHANNEL) == ADC_Channel_5) || \
                                 ((CHANNEL) == ADC_Channel_6) || ((CHANNEL) == ADC_Channel_7) || \
                                 ((CHANNEL) == ADC_Channel_8) || ((CHANNEL) == ADC_Channel_9) || \
                                 ((CHANNEL) == ADC_Channel_10) || ((CHANNEL) == ADC_Channel_11) || \
                                 ((CHANNEL) == ADC_Channel_12) || ((CHANNEL) == ADC_Channel_13) || \
                                 ((CHANNEL) == ADC_Channel_14) || ((CHANNEL) == ADC_Channel_15) || \
                                 ((CHANNEL) == ADC_Channel_16) || ((CHANNEL) == ADC_Channel_17))

/* ADC sampling times --------------------------------------------------------*/		//ADC_SampleTime设定选中通道的ADC采样时间
#define ADC_SampleTime_1Cycles5                    ((u8)0x00)							//采样时间为1.5周期
#define ADC_SampleTime_7Cycles5                    ((u8)0x01)							//采样时间为7.5周期
#define ADC_SampleTime_13Cycles5                   ((u8)0x02)							//采样时间为13.5周期
#define ADC_SampleTime_28Cycles5                   ((u8)0x03)							//采样时间为28.5周期
#define ADC_SampleTime_41Cycles5                   ((u8)0x04)							//采样时间为41.5周期
#define ADC_SampleTime_55Cycles5                   ((u8)0x05)							//采样时间为55.5周期
#define ADC_SampleTime_71Cycles5                   ((u8)0x06)							//采样时间为71.5周期
#define ADC_SampleTime_239Cycles5                  ((u8)0x07)							//采样时间为239.5周期

#define IS_ADC_SAMPLE_TIME(TIME) (((TIME) == ADC_SampleTime_1Cycles5) || \
                                  ((TIME) == ADC_SampleTime_7Cycles5) || \
                                  ((TIME) == ADC_SampleTime_13Cycles5) || \
                                  ((TIME) == ADC_SampleTime_28Cycles5) || \
                                  ((TIME) == ADC_SampleTime_41Cycles5) || \
                                  ((TIME) == ADC_SampleTime_55Cycles5) || \
                                  ((TIME) == ADC_SampleTime_71Cycles5) || \
                                  ((TIME) == ADC_SampleTime_239Cycles5))

/* ADC extrenal trigger sources for injected channels conversion -------------*/	//ADC_ExternalTrigInjectedConv指定所使用的注入转换启动触发
/* For ADC1 and ADC2 */
#define ADC_ExternalTrigInjecConv_T2_TRGO           ((u32)0x00002000)							//选择定时器2的TRGO作为注入转换外部触发
#define ADC_ExternalTrigInjecConv_T2_CC1            ((u32)0x00003000)							//选择定时器2的捕获比较1作为注入转换外部触发
#define ADC_ExternalTrigInjecConv_T3_CC4            ((u32)0x00004000)							//选择定时器3的捕获比较4作为注入转换外部触发
#define ADC_ExternalTrigInjecConv_T4_TRGO           ((u32)0x00005000)							//选择定时器4的TRGO作为注入转换外部触发
#define ADC_ExternalTrigInjecConv_Ext_IT15_TIM8_CC4 ((u32)0x00006000)							//选择外部中断线15事件作为注入转换外部触发
/* For ADC1, ADC2 and ADC3 */
#define ADC_ExternalTrigInjecConv_T1_TRGO           ((u32)0x00000000)							//选择定时器1的TRGO作为注入转换外部触发
#define ADC_ExternalTrigInjecConv_T1_CC4            ((u32)0x00001000)							//选择定时器1的捕获比较4作为注入转换外部触发
#define ADC_ExternalTrigInjecConv_None              ((u32)0x00007000)							//注入转换由软件而不是外部触发启动
/* For ADC3 */
#define ADC_ExternalTrigInjecConv_T4_CC3            ((u32)0x00002000)							//
#define ADC_ExternalTrigInjecConv_T8_CC2            ((u32)0x00003000)							//
#define ADC_ExternalTrigInjecConv_T8_CC4            ((u32)0x00004000)							//
#define ADC_ExternalTrigInjecConv_T5_TRGO           ((u32)0x00005000)							//
#define ADC_ExternalTrigInjecConv_T5_CC4            ((u32)0x00006000)							//

#define IS_ADC_EXT_INJEC_TRIG(INJTRIG) (((INJTRIG) == ADC_ExternalTrigInjecConv_T1_TRGO) || \
                                        ((INJTRIG) == ADC_ExternalTrigInjecConv_T1_CC4) || \
                                        ((INJTRIG) == ADC_ExternalTrigInjecConv_T2_TRGO) || \
                                        ((INJTRIG) == ADC_ExternalTrigInjecConv_T2_CC1) || \
                                        ((INJTRIG) == ADC_ExternalTrigInjecConv_T3_CC4) || \
                                        ((INJTRIG) == ADC_ExternalTrigInjecConv_T4_TRGO) || \
                                        ((INJTRIG) == ADC_ExternalTrigInjecConv_Ext_IT15_TIM8_CC4) || \
                                        ((INJTRIG) == ADC_ExternalTrigInjecConv_None) || \
                                        ((INJTRIG) == ADC_ExternalTrigInjecConv_T4_CC3) || \
                                        ((INJTRIG) == ADC_ExternalTrigInjecConv_T8_CC2) || \
                                        ((INJTRIG) == ADC_ExternalTrigInjecConv_T8_CC4) || \
                                        ((INJTRIG) == ADC_ExternalTrigInjecConv_T5_TRGO) || \
                                        ((INJTRIG) == ADC_ExternalTrigInjecConv_T5_CC4))

/* ADC injected channel selection --------------------------------------------*/	//参数ADC_InjectedChannel指定了必须设置转换偏移值的ADC通道
#define ADC_InjectedChannel_1                       ((u8)0x14)										//选择注入通道1
#define ADC_InjectedChannel_2                       ((u8)0x18)										//选择注入通道2
#define ADC_InjectedChannel_3                       ((u8)0x1C)										//选择注入通道3
#define ADC_InjectedChannel_4                       ((u8)0x20)										//选择注入通道4

#define IS_ADC_INJECTED_CHANNEL(CHANNEL) (((CHANNEL) == ADC_InjectedChannel_1) || \
                                          ((CHANNEL) == ADC_InjectedChannel_2) || \
                                          ((CHANNEL) == ADC_InjectedChannel_3) || \
                                          ((CHANNEL) == ADC_InjectedChannel_4))

/* ADC analog watchdog selection ---------------------------------------------*/	//ADC_AnalogWatchdog规定了ADC模拟看门狗的设置
#define ADC_AnalogWatchdog_SingleRegEnable         ((u32)0x00800200)							//单个规则通道上设置模拟看门狗
#define ADC_AnalogWatchdog_SingleInjecEnable       ((u32)0x00400200)							//单个注入通道上设置模拟看门狗
#define ADC_AnalogWatchdog_SingleRegOrInjecEnable  ((u32)0x00C00200)							//单个规则通道或者注入通道上设置模拟看门狗
#define ADC_AnalogWatchdog_AllRegEnable            ((u32)0x00800000)							//所有规则通道上设置模拟看门狗
#define ADC_AnalogWatchdog_AllInjecEnable          ((u32)0x00400000)							//所有注入通道上设置模拟看门狗
#define ADC_AnalogWatchdog_AllRegAllInjecEnable    ((u32)0x00C00000)							//所有规则通道和所有注入通道上上设置模拟看门狗
#define ADC_AnalogWatchdog_None                    ((u32)0x00000000)							//不设置模拟看门狗

#define IS_ADC_ANALOG_WATCHDOG(WATCHDOG) (((WATCHDOG) == ADC_AnalogWatchdog_SingleRegEnable) || \
                                          ((WATCHDOG) == ADC_AnalogWatchdog_SingleInjecEnable) || \
                                          ((WATCHDOG) == ADC_AnalogWatchdog_SingleRegOrInjecEnable) || \
                                          ((WATCHDOG) == ADC_AnalogWatchdog_AllRegEnable) || \
                                          ((WATCHDOG) == ADC_AnalogWatchdog_AllInjecEnable) || \
                                          ((WATCHDOG) == ADC_AnalogWatchdog_AllRegAllInjecEnable) || \
                                          ((WATCHDOG) == ADC_AnalogWatchdog_None))

/* ADC interrupts definition -------------------------------------------------*/	//ADC_IT可以用来使能或者失能ADC中断
#define ADC_IT_EOC                                 ((u16)0x0220)									//EOC中断屏蔽
#define ADC_IT_AWD                                 ((u16)0x0140)									//AWDOG中断屏蔽
#define ADC_IT_JEOC                                ((u16)0x0480)									//JEOC中断屏蔽

#define IS_ADC_IT(IT) ((((IT) & (u16)0xF81F) == 0x00) && ((IT) != 0x00))
#define IS_ADC_GET_IT(IT) (((IT) == ADC_IT_EOC) || ((IT) == ADC_IT_AWD) || \
                           ((IT) == ADC_IT_JEOC))

/* ADC flags definition ------------------------------------------------------*/	//ADC_FLAG标志位
#define ADC_FLAG_AWD                               ((u8)0x01)											//模拟看门狗标志位
#define ADC_FLAG_EOC                               ((u8)0x02)											//转换结束标志位
#define ADC_FLAG_JEOC                              ((u8)0x04)											//注入组转换结束标志位
#define ADC_FLAG_JSTRT                             ((u8)0x08)											//注入组转换开始标志位
#define ADC_FLAG_STRT                              ((u8)0x10)											//规则组转换开始标志位

#define IS_ADC_CLEAR_FLAG(FLAG) ((((FLAG) & (u8)0xE0) == 0x00) && ((FLAG) != 0x00))
#define IS_ADC_GET_FLAG(FLAG) (((FLAG) == ADC_FLAG_AWD) || ((FLAG) == ADC_FLAG_EOC) || \
                               ((FLAG) == ADC_FLAG_JEOC) || ((FLAG)== ADC_FLAG_JSTRT) || \
                               ((FLAG) == ADC_FLAG_STRT))

/* ADC thresholds ------------------------------------------------------------*/
#define IS_ADC_THRESHOLD(THRESHOLD) ((THRESHOLD) <= 0xFFF)

/* ADC injected offset -------------------------------------------------------*/
#define IS_ADC_OFFSET(OFFSET) ((OFFSET) <= 0xFFF)

/* ADC injected length -------------------------------------------------------*/
#define IS_ADC_INJECTED_LENGTH(LENGTH) (((LENGTH) >= 0x1) && ((LENGTH) <= 0x4))

/* ADC injected rank ---------------------------------------------------------*/
#define IS_ADC_INJECTED_RANK(RANK) (((RANK) >= 0x1) && ((RANK) <= 0x4))

/* ADC regular length --------------------------------------------------------*/
#define IS_ADC_REGULAR_LENGTH(LENGTH) (((LENGTH) >= 0x1) && ((LENGTH) <= 0x10))

/* ADC regular rank ----------------------------------------------------------*/
#define IS_ADC_REGULAR_RANK(RANK) (((RANK) >= 0x1) && ((RANK) <= 0x10))

/* ADC regular discontinuous mode number -------------------------------------*/
#define IS_ADC_REGULAR_DISC_NUMBER(NUMBER) (((NUMBER) >= 0x1) && ((NUMBER) <= 0x8))

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void ADC_DeInit(ADC_TypeDef* ADCx);
void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct);
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct);
void ADC_Cmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_DMACmd(ADC_TypeDef* ADCx, FunctionalState NewState);								//使能或者失能指定的ADC的DMA请求
void ADC_ITConfig(ADC_TypeDef* ADCx, u16 ADC_IT, FunctionalState NewState);	//使能或者失能指定的ADC的中断.
void ADC_ResetCalibration(ADC_TypeDef* ADCx);																//重置指定的ADC的校准寄存器
FlagStatus ADC_GetResetCalibrationStatus(ADC_TypeDef* ADCx);								//获取指定的ADC重置校准寄存器的状态.
void ADC_StartCalibration(ADC_TypeDef* ADCx);																//启动指定的ADC1的校准程序.
FlagStatus ADC_GetCalibrationStatus(ADC_TypeDef* ADCx);											//获取指定ADC的校准状态.
void ADC_SoftwareStartConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);	//使能或者失能指定的ADC的软件转换启动功能
FlagStatus ADC_GetSoftwareStartConvStatus(ADC_TypeDef* ADCx);								//获取ADC软件转换启动状态
void ADC_DiscModeChannelCountConfig(ADC_TypeDef* ADCx, u8 Number);					//配置间断模式规则组通道计数器的值.
void ADC_DiscModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState);					//使能或者失能指定的ADC规则组通道的间断模式
void ADC_RegularChannelConfig(ADC_TypeDef* ADCx, u8 ADC_Channel, u8 Rank, u8 ADC_SampleTime);	//设置指定ADC的规则组通道，设置它们的转化顺序和采样时间
void ADC_ExternalTrigConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);	//使能或者失能ADCx的经外部触发启动转换功能
u16 ADC_GetConversionValue(ADC_TypeDef* ADCx);															//返回最近一次ADCx规则组的转换结果
u32 ADC_GetDualModeConversionValue(void);																		//返回最近一次双ADC模式下的转换结果
void ADC_AutoInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);	//使能或者失能指定ADC在规则组转化后自动开始注入组转换
void ADC_InjectedDiscModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState);	//使能或者失能指定ADC的注入组间断模式
void ADC_ExternalTrigInjectedConvConfig(ADC_TypeDef* ADCx, u32 ADC_ExternalTrigInjecConv);		//配置ADCx的外部触发启动注入组转换功能
void ADC_ExternalTrigInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);						//使能或者失能ADCx的经外部触发启动注入组转换功能
void ADC_SoftwareStartInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);						//使能或者失能ADCx软件启动注入组转换功能
FlagStatus ADC_GetSoftwareStartInjectedConvCmdStatus(ADC_TypeDef* ADCx);											//获取指定ADC的软件启动注入组转换状态
void ADC_InjectedChannelConfig(ADC_TypeDef* ADCx, u8 ADC_Channel, u8 Rank, u8 ADC_SampleTime);//设置指定ADC的注入组通道，设置它们的转化顺序和采样时间
void ADC_InjectedSequencerLengthConfig(ADC_TypeDef* ADCx, u8 Length);													//设置注入组通道的转换序列长度
void ADC_SetInjectedOffset(ADC_TypeDef* ADCx, u8 ADC_InjectedChannel, u16 Offset);						//设置注入组通道的转换偏移值
u16 ADC_GetInjectedConversionValue(ADC_TypeDef* ADCx, u8 ADC_InjectedChannel);								//返回ADC指定注入通道的转换结果
void ADC_AnalogWatchdogCmd(ADC_TypeDef* ADCx, u32 ADC_AnalogWatchdog);												//使能或者失能指定单个/全体，规则/注入组通道上的模拟看门狗
void ADC_AnalogWatchdogThresholdsConfig(ADC_TypeDef* ADCx, u16 HighThreshold, u16 LowThreshold);	//设置模拟看门狗的高/低阈值
void ADC_AnalogWatchdogSingleChannelConfig(ADC_TypeDef* ADCx, u8 ADC_Channel);	//对单个ADC通道设置模拟看门狗
void ADC_TempSensorVrefintCmd(FunctionalState NewState);												//使能或者失能温度传感器和内部参考电压通道
FlagStatus ADC_GetFlagStatus(ADC_TypeDef* ADCx, u8 ADC_FLAG);										//获取指定的ADC标志位置1与否
void ADC_ClearFlag(ADC_TypeDef* ADCx, u8 ADC_FLAG);															//清除ADCx的待处理标志位
ITStatus ADC_GetITStatus(ADC_TypeDef* ADCx, u16 ADC_IT);												//检查指定的ADC中断是否发生
void ADC_ClearITPendingBit(ADC_TypeDef* ADCx, u16 ADC_IT);											//清除ADCx的中断待处理标志位

#endif /*__STM32F10x_ADC_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
