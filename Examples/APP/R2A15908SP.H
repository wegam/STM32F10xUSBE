#ifndef __R2A15908SP_H
#define __R2A15908SP_H

#include "STM32_EXTI.H"
#include "stm32f10x_gpio.h"
#include "STM32_PWM.H"
#include "STM32_I2C.H"



void R2A15908SP_Configuration(void);
void R2A15908SP_Server(void);
void R2A15908SP_DELAY(u32 Time);
void R2A15908SP_GPIO(void);
u8 Write_R2A15908SP(GPIO_TypeDef* GPIOx_SCL,u16 IIC_SCLPin,GPIO_TypeDef* GPIOx_SDA,u16 IIC_SDAPin,u8 txd);
void R2A15908SP_Ecoder(void);	//±àÂëÆ÷½âÎö


#endif

