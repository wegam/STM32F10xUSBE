/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : readme.txt
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Description of the USB Device_Firmware_Upgrade Demo.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

Example description
===================
This Demo presents the implementation of a device firmware upgrade (DFU) capability
in the STM32F10xxx microcontroller. It follows the DFU class specification defined
by the USB Implementers Forum for reprogramming an application through USB. 
The DFU principle is particularly well suited to USB applications that need to be
reprogrammed in the field.

More details about this Demo implementation is given in the User manual 
"UM0424 STM32F10xxx USB development kit", available for download from the ST
microcontrollers website: www.st.com/stm32


Directory contents
==================
 + \binary     contains a set of sources files that build the application to be
               loaded with DFU 
 + \images     contains DFU images to be loaded in the STM32F10xxx microcontroller
 + \include    contains the Demo firmware header files
 + \project    contains pre-configured projects for RVMDK
 + \source     contains the Demo firmware source files


Hardware environment
====================
This example runs on STMicroelectronics STM3210B-EVAL and STM3210E-EVAL evaluation
boards and can be easily tailored to any other hardware.
To select the STMicroelectronics evaluation board used to run the example, uncomment
the corresponding line in platform_config.h file.

Note: After each device reset, hold down the Key push-button on the STM3210B-EVAL 
      or STM3210E-EVAL evaluation boards to enter the DFU mode.

    
How to use it
=============
 + RVMDK
    - Open the DFU.Uv2 project
    - In the build toolbar select the project config:
        - STM3210B-EVAL: to configure the project for STM32 Medium-density devices
        - STM3210E-EVAL: to configure the project for STM32 High-density devices
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)    

NOTE:
 - Medium-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 32 and 128 Kbytes.
 - High-density devices are STM32F101xx and STM32F103xx microcontrollers where
   the Flash memory density ranges between 256 and 512 Kbytes.

******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE******
