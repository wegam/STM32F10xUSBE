
#ifndef __CS5530_H
#define __CS5530_H
//#include <stdint.h>
#include "stm32f10x_gpio.h"

#define CS5530_OF_MAX_NUM					10	/*最近连续采样的10次都溢出,表示数据已经溢出了*/
#define CS5530_CONTINUOUS_IGNORE	10	/*开始连续转换时,要忽略前10(资料上推荐是5)个不准确的数据*/

#define CS5530_WRITE_OFFSET				(unsigned char)0x01	/*写偏移寄存器*/
#define CS5530_READ_OFFSET				(unsigned char)0x09	/*读偏移寄存器*/
#define CS5530_WRITE_GAIN					(unsigned char)0x02	/*写增益寄存器*/
#define CS5530_READ_GAIN					(unsigned char)0x0A	/*读增益寄存器*/
#define CS5530_WRITE_CONFIG				(unsigned char)0x03	/*写配置寄存器*/
#define CS5530_READ_CONFIG				(unsigned char)0x0B	/*读配置寄存器*/
#define CS5530_START_SINGLE				(unsigned char)0x80	//执行单次转换
#define CS5530_START_CONTINUOUS		(unsigned char)0xC0	//执行连续转换
#define CS5530_SYSTEM_OFFSET_CAL	(unsigned char)0x85	/*执行系统偏移校准*/
#define CS5530_SYSTEM_GAIN_CAL		(unsigned char)0x86	/*执行系统增益校准*/
#define CS5530_SYNC1							(unsigned char)0xFF	/*串行口初始化序列命令之一*/
#define CS5530_SYNC0							(unsigned char)0xFE	/*串行口初始化序列结束命令*/
#define CS5530_NULL_BYTE					(unsigned char)0x00	/*用来清除端口标志位使得转换器保持在连续转换模式*/

// Configuration Register
#define CS5530_CONF_STANDBY_MODE				(0x00UL<<31)	/*待机模式,功耗降到4mW,待机模式下保留振荡器
																											和模拟偏置电路工作,这就容许当PDW返回为0时,
																											转换器可以马上从省电模式进入正常模式*/
#define CS5530_CONF_SLEEP_MODE					(0x01UL<<31)	/*休眠模式,进入睡眠模式,功耗降为500μW,这种
																											模式下由于关闭了振荡器,当要进入正常模式时,
																											振荡器要延时20mS才能起振工作*/
																											
#define CS5530_CONF_POWER_SAVE_MODE			(0x01UL<<30)	/*节电模式*/
#define CS5530_CONF_POWER_ON_MODE				(0x00UL<<30)	/*正常模式*/
#define CS5530_CONF_NORMAL_MODE					(0x00UL<<29)	/*正常操作*/
#define CS5530_CONF_SYSTEM_RESET				(0x01UL<<29)	/*激活一个复位周期 复位后需要软件清0*/
#define CS5530_CONF_RESET_STATUS				(0x01UL<<28)	/*复位有效 系统已复位 只读,0-正常工作，1-复位成功*/
#define CS5530_CONF_SHORT_INPUTS				(0x01UL<<27)	//设置为输入短路
#define CS5530_CONF_NOSHORT_INPUTS			(0x00UL<<27)	//正常输入,通道输入不短接
#define CS5530_CONF_VREF_HIGH						(0x00UL<<25)	/*参考电压 2.5~((VA+)-(VA-))V*/
#define CS5530_CONF_VREF_LOW						(0x01UL<<25)	/*参考电压 1~2.5V*/
#define CS5530_CONF_CR_A1_0							(0x00UL<<24)	/*将A1管脚电平置为VA-*/
#define CS5530_CONF_CR_A1_1							(0x01UL<<24)	/*将A1管脚电平置为VA+*/
#define CS5530_CONF_CR_A0_0							(0x00UL<<23)	/*将A0管脚电平置为VA-*/
#define CS5530_CONF_CR_A0_1							(0x01UL<<23)	/*将A0管脚电平置为VA+*/
#define CS5530_CONF_LINE_FREQ_60				(0x00UL<<19)	/*滤波数率选择 缺省输入字,较低数据率下可提供60Hz抑制*/
#define CS5530_CONF_LINE_FREQ_50				(0x01UL<<19)	/*输出字数率及相应滤波器特性乘以系数5/6,较低数据率下可提供50Hz抑制*/
#define CS5530_CONF_DATARATE_120_100		(0x00UL<<11)	/*字速率是在时钟为4.9152MHz,连续转换模式下的字速率*/
#define CS5530_CONF_DATARATE_60_50			(0x01UL<<11)
#define CS5530_CONF_DATARATE_30_25			(0x02UL<<11)
#define CS5530_CONF_DATARATE_15_12P5		(0x03UL<<11)
#define CS5530_CONF_DATARATE_7P5_6P25		(0x04UL<<11)
#define CS5530_CONF_DATARATE_3840_3200	(0x08UL<<11)
#define CS5530_CONF_DATARATE_1920_1600	(0x09UL<<11)
#define CS5530_CONF_DATARATE_960_800		(0x0AUL<<11)
#define CS5530_CONF_DATARATE_480_400		(0x0BUL<<11)
#define CS5530_CONF_DATARATE_240_200		(0x0CUL<<11)
#define CS5530_CONF_DATARATE_MASK				(0x0FUL<<11)
#define CS5530_CONF_BIPOLAR_MODE				(0x00UL<<10)	//极性选择双极性模式
#define CS5530_CONF_UNIPOLAR_MODE				(0x01UL<<10)	//极性选择单极性模式
#define CS5530_CONF_TURN_OFF_300NA			(0x00UL<<9)		/*不激活电流源*/
#define CS5530_CONF_TURN_ON_300NA				(0x01UL<<9)		/*激活电流源300ma*/
	
#define CS5530_CONTINUOUS_ON			(unsigned char)0x00	/*继续连续转换模式--清除SDO标志*/
#define CS5530_CONTINUOUS_OFF			(unsigned char)0xFF	/*退出连续转换模式*/

/*转换数据寄存器低8位状态*/
#define CS5530_CDR_OF_MASK				0x00000004	/*溢出标志位:
																	0-无益处;
																	1-溢出,当正输入信号大于正满量程范围,负输入信号小于
																	0(单极性)或小于负满量程范围(双极性)时被置1;*/

typedef struct	_CS5530_Pin
{
	//CS-PIN
	GPIO_TypeDef* 	CS5530_CS_PORT;				//GPIOX
	unsigned short 	CS5530_CS_Pin;				//GPIO_Pin_x
	
	//SDI-PIN
	GPIO_TypeDef* 	CS5530_SDI_PORT;				//GPIOX
	unsigned short 	CS5530_SDI_Pin;				//GPIO_Pin_x
	
	//SDO-PIN
	GPIO_TypeDef* 	CS5530_SDO_PORT;				//GPIOX
	unsigned short 	CS5530_SDO_Pin;				//GPIO_Pin_x
	
	//CS-PIN
	GPIO_TypeDef* 	CS5530_SCLK_PORT;				//GPIOX
	unsigned short 	CS5530_SCLK_Pin;				//GPIO_Pin_x

}CS5530_Pindef;



void CS5530_PinConf(CS5530_Pindef *Pinfo);
u32	CS5530_ReadData(CS5530_Pindef *Pinfo);		//读取AD值，如果返回0xFFFFFFFF,则未读取到24位AD值


#endif

