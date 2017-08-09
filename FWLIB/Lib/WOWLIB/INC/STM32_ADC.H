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

#ifndef __STM32_ADC_H
#define __STM32_ADC_H

#include "stm32f10x_type.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"

void ADC1_DiscConfiguration(u32 *ADC_DATA,u32 DMA_BufferSize,u8 ADC_Channel_x,u8 ADC_NbrOfChannel, u8 Rank, u8 ADC_SampleTime);				//ADC1规则通道组配置
void ADC_TempSensorConfiguration(u32 *ADC_DATA);		//STM32内部温度传感器配置
float Get_ADC_Temperature(u32 ADC_DATA);										//获取内部温度传感器温度
void ADC_PinConf(u8 ADC_Channel_x);																			//ADC管脚配置 依据STM32F107VC引脚分布
void ADC1_InitStructure(u8 ADC_NbrOfChannel);																		//ADC初始化
void ADC1_DMAConfiguration(u32 *ADC_DATA,u32 DMA_BufferSize) ;			//ADC1 DMA配置
void ADC1_Cmd(FunctionalState NewState);		//开启ADC1转换


#endif




