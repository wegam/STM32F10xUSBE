;/*****************************************************************************/
;/* STR91xCFG.S: STR91x Flash Configuration - requires ICP Flash Algorithms   */
;/*              OTP, User Code, Configuration and Security Bit               */
;/*****************************************************************************/
;/* <<< Use Configuration Wizard in Context Menu >>>                          */
;/*****************************************************************************/
;/* This file is part of the uVision/ARM development tools.                   */
;/* Copyright (c) 2005-2008 Keil Software. All rights reserved.               */
;/* This software may only be used under the terms of a valid, current,       */
;/* end user licence from KEIL for a compatible version of KEIL software      */
;/* development tools. Nothing else gives you the right to use this software. */
;/*****************************************************************************/


;// <e0> OTP Memory
;//   <o1>  OTP Byte 0  <0x0-0xFF>  
;//   <o2>  OTP Byte 1  <0x0-0xFF>
;//   <o3>  OTP Byte 2  <0x0-0xFF>
;//   <o4>  OTP Byte 3  <0x0-0xFF>
;//   <o5>  OTP Byte 4  <0x0-0xFF>
;//   <o6>  OTP Byte 5  <0x0-0xFF>
;//   <o7>  OTP Byte 6  <0x0-0xFF>
;//   <o8>  OTP Byte 7  <0x0-0xFF>
;//   <o9>  OTP Byte 8  <0x0-0xFF>
;//   <o10> OTP Byte 9  <0x0-0xFF>
;//   <o11> OTP Byte 10 <0x0-0xFF>
;//   <o12> OTP Byte 11 <0x0-0xFF>
;//   <o13> OTP Byte 12 <0x0-0xFF>
;//   <o14> OTP Byte 13 <0x0-0xFF>
;//   <o15> OTP Byte 14 <0x0-0xFF>
;//   <o16> OTP Byte 15 <0x0-0xFF>
;//   <o17> OTP Byte 16 <0x0-0xFF>
;//   <o18> OTP Byte 17 <0x0-0xFF>
;//   <o19> OTP Byte 18 <0x0-0xFF>
;//   <o20> OTP Byte 19 <0x0-0xFF>
;//   <o21> OTP Byte 20 <0x0-0xFF>
;//   <o22> OTP Byte 21 <0x0-0xFF>
;//   <o23> OTP Byte 22 <0x0-0xFF>
;//   <o24> OTP Byte 23 <0x0-0xFF>
;//   <o25> OTP Byte 24 <0x0-0xFF>
;//   <o26> OTP Byte 25 <0x0-0xFF>
;//   <o27> OTP Byte 26 <0x0-0xFF>
;//   <o28> OTP Byte 27 <0x0-0xFF>
;//   <o29> OTP Byte 28 <0x0-0xFF>
;//   <o30> OTP Byte 29 <0x0-0xFF>
;//   <o31> OTP Byte 30 <0x0-0xFF> <i> Mask Major/Minor Identifier
;//   <o32> OTP Byte 31 <0x0-0xFF> <i> Family Major/Minor Identifier
;// </e0>
OTP             EQU     0
OTP_B0          EQU     0xFF
OTP_B1          EQU     0xFF
OTP_B2          EQU     0xFF
OTP_B3          EQU     0xFF
OTP_B4          EQU     0xFF
OTP_B5          EQU     0xFF
OTP_B6          EQU     0xFF
OTP_B7          EQU     0xFF
OTP_B8          EQU     0xFF
OTP_B9          EQU     0xFF
OTP_B10         EQU     0xFF
OTP_B11         EQU     0xFF
OTP_B12         EQU     0xFF
OTP_B13         EQU     0xFF
OTP_B14         EQU     0xFF
OTP_B15         EQU     0xFF
OTP_B16         EQU     0xFF
OTP_B17         EQU     0xFF
OTP_B18         EQU     0xFF
OTP_B19         EQU     0xFF
OTP_B20         EQU     0xFF
OTP_B21         EQU     0xFF
OTP_B22         EQU     0xFF
OTP_B23         EQU     0xFF
OTP_B24         EQU     0xFF
OTP_B25         EQU     0xFF
OTP_B26         EQU     0xFF
OTP_B27         EQU     0xFF
OTP_B28         EQU     0xFF
OTP_B29         EQU     0xFF
OTP_B30         EQU     0x13       ; Factory Programmed
OTP_B31         EQU     0x91       ; Factory Programmed

;// <e0> User Code
;//   <o1> User Code Word
;// </e0>
UC              EQU     1
UC_W            EQU     0xFFFFFFFF

;// <e0> Configuration
;//   <h> Main Flash Sector Protect
;//     <o1.0> Sector 0
;//     <o1.1> Sector 1
;//     <o1.2> Sector 2
;//     <o1.3> Sector 3
;//     <o1.4> Sector 4
;//     <o1.5> Sector 5
;//     <o1.6> Sector 6
;//     <o1.7> Sector 7
;//     <o1.8> Sector 8
;//     <o1.9> Sector 9
;//     <o1.10> Sector 10
;//     <o1.11> Sector 11
;//     <o1.12> Sector 12
;//     <o1.13> Sector 13
;//     <o1.14> Sector 14
;//     <o1.15> Sector 15
;//     <o1.16> Sector 16
;//     <o1.17> Sector 17 
;//     <o1.18> Sector 18
;//     <o1.19> Sector 19
;//     <o1.20> Sector 20
;//     <o1.21> Sector 21
;//     <o1.22> Sector 22
;//     <o1.23> Sector 23
;//     <o1.24> Sector 24
;//     <o1.25> Sector 25
;//     <o1.26> Sector 26
;//     <o1.27> Sector 27
;//     <o1.28> Sector 28
;//     <o1.29> Sector 29
;//     <o1.30> Sector 30
;//     <o1.31> Sector 31
;//   </h>
;//   <h> Secondary Flash Sector Protect
;//     <o2.0> Sector 0
;//     <o2.1> Sector 1
;//     <o2.2> Sector 2
;//     <o2.3> Sector 3
;//     <o2.4> Sector 4
;//     <o2.5> Sector 5
;//     <o2.6> Sector 6
;//     <o2.7> Sector 7
;//   </h>
;//   <o2.16> CSX (Chip Select Mapping)
;//   <o2.17> LVD_TH (VDDQ Threshold Voltage)
;//   <o2.18> LVD_RST_SEL (LVD Reset Select)
;//   <o2.19> LVD_WNG_SEL (LVD Warning Select)
;//   <o2.31> OTP_LOCK (OTP Lock Bit)
;// </e0>
CFG             EQU     1
CFG_W0          EQU     0x00000000
CFG_W1          EQU     0x00000000

;// <e0> Chip Security
;//   <o1.0> CSEC (Security Bit)
;// </e0>
CSB             EQU     1
CSB_B           EQU     0x00


                IF      OTP <> 0
                AREA    |.ARM.__AT_0x500000|, CODE, READONLY
                DCB     OTP_B0,  OTP_B1,  OTP_B2,  OTP_B3
                DCB     OTP_B4,  OTP_B5,  OTP_B6,  OTP_B7
                DCB     OTP_B8,  OTP_B9,  OTP_B10, OTP_B11
                DCB     OTP_B12, OTP_B13, OTP_B14, OTP_B15
                DCB     OTP_B16, OTP_B17, OTP_B18, OTP_B19
                DCB     OTP_B20, OTP_B21, OTP_B22, OTP_B23
                DCB     OTP_B24, OTP_B25, OTP_B26, OTP_B27
                DCB     OTP_B28, OTP_B29, OTP_B30, OTP_B31
                ENDIF

                IF      UC <> 0
                AREA    |.ARM.__AT_0x510000|, CODE, READONLY
                DCD     UC_W
                ENDIF

                IF      CFG <> 0
                AREA    |.ARM.__AT_0x520000|, CODE, READONLY
                DCD     CFG_W0
                DCD     CFG_W1
                ENDIF

                IF      CSB <> 0
                AREA    |.ARM.__AT_0x520008|, CODE, READONLY
                DCB     CSB_B
                ENDIF


                END
