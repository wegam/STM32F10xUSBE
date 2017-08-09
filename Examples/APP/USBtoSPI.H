#ifndef __USBtoSPI_H
#define __USBtoSPI_H


#include "stm32f10x_gpio.h"

#include "usb_core.h"			//USB�������ݴ����ĺ����ļ�
#include "usb_pwr.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "hw_config.h"
#include "usb_pwr.h"

#include "usb_lib.h"
#include "usb_istr.h"
#include "hw_config.h"







void USBtoSPI_Configuration(void);
void USBtoSPI_Server(void);
void USB_EN(void);
void LEDCON(void);
void LEDSET(u8 states);

	
#endif


