/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : readme.txt
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Description of the binary directory.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

Description
===================
This directory contains a set of sources files that build the application to be
loaded into into Flash memory using device firmware upgrade (DFU).

To build such application, some special configuration has to be peformed: 
1. Set the program load address at 0x08003000, using your toolcahin linker file
2. Relocate the vector table at address 0x08003000, using the "NVIC_SetVectorTable"
   function.
 
The SysTick example provided within the STM32F10x Firmware library is used as 
illustration.    
This example configures the SysTick to generate a time base equal to 1 ms.
The system clock is set to 72 MHz, the SysTick is clocked by the AHB clock (HCLK)
divided by 8.
A "Delay" function is implemented based on the SysTick end-of-count event.
Four LEDs connected to the GPIO_LED Pin 6, Pin 7, Pin 8 and Pin 9 are toggled
with a timing defined by the Delay function.


Directory contents
==================
 + template project
     - RVMDK: This folder contains a pre-configured project file that produces a
              binary image of SysTick example to be loaded with DFU.              

 + platform_config.h   Evaluation board specific configuration file     
 + stm32f10x_it.c      Interrupt handlers
 + main.c              Main program
 + main.h              Header for main.c


Hardware environment
====================
This example runs on STMicroelectronics STM3210B-EVAL and STM3210E-EVAL evaluation
boards and can be easily tailored to any other hardware.
To select the STMicroelectronics evaluation board used to run the example, uncomment
the corresponding line in platform_config.h file.

 + STM3210B-EVAL 
    - Use LD1, LD2, LD3 and LD4 leds connected respectively to PC.06, PC.07, PC.08
      and PC.09 pins
 
 + STM3210E-EVAL
    - Use LD1, LD2, LD3 and LD4 leds connected respectively to PF.06, PF0.7, PF.08
      and PF.09 pins
                    
   
How to use it
=============
In order to load the SysTick example with the DFU, you must do the following:
 + RVMDK
    - Open the SysTick.Uv2 project
    - In the build toolbar select the project config:
        - STM3210B-EVAL: to configure the project for STM32 Medium-density devices
        - STM3210E-EVAL: to configure the project for STM32 High-density devices
    - Rebuild all files: Project->Rebuild all target files
    - Run "axftobin.bat"
    - A binary file "STM3210B-EVAL_SysTick.bin" or "STM3210E-EVAL_SysTick.bin"
      will be generated under "\Obj" folder

 + The generated .bin file should be converted to the DFU format using the “DFU File
   Manager Tool” included in the “DfuSe” PC software install. For more details on
   how to convert a .bin file to DFU format please refer to the UM0412 user manual
   “Getting started with DfuSe USB device firmware upgrade STMicroelectronics extension”
   available from the STMicroelectronics microcontroller website www.st.com.

NOTE:
 - Medium-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 32 and 128 Kbytes.
 - High-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 256 and 512 Kbytes. 


******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE******
