/**************************************************************************************************
* Copyright (C), 1997-2016,  HAICHUANG IOT Tech. Co., Ltd.
* 文件<Bsp_LcdR61509V.h>
* 模块描述 :	定义、功能、说明
* 历史修改记录
*     <主要修改者>  <时间>   <版本 >   <说明>
*     xlyan        2016/9/26   1.0.0    创建文件
**************************************************************************************************/
#ifndef __LCDDISPLAY_H_
#define __LCDDISPLAY_H

#include "GT32L32M0180.H"
#include "R61509V.H"

typedef struct _LCD_Info
{
	R61509V_Pindef 				R61509V_sPinfo;
	GT32L32_Info_TypeDef	GT32L32_Info;

}LCD_Info_TypeDef;






#endif
/************************************** The End Of FILE **************************************/
