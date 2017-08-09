;/*****************************************************************************/
;/* STARTUP.S: Startup file for ST STR73x device series                       */
;/*****************************************************************************/
;/* <<< Use Configuration Wizard in Context Menu >>>                          */ 
;/*****************************************************************************/
;/* This file is part of the uVision/ARM development tools.                   */
;/* Copyright (c) 2005-2006 Keil Software. All rights reserved.               */
;/* This software may only be used under the terms of a valid, current,       */
;/* end user licence from KEIL for a compatible version of KEIL software      */
;/* development tools. Nothing else gives you the right to use this software. */
;/*****************************************************************************/


;/*
; *  The STARTUP.S code is executed after CPU Reset. This file may be 
; *  translated with the following SET symbols. In uVision these SET 
; *  symbols are entered under Options - ASM - Define.
; *
; *  REMAP: when set internal RAM is remapped to address 0. It replaces the
; *  Flash memory normally accessible in this location. Even when remapped,
; *  the RAM is also accessible at its physical address 0xA0000000.
; */


; Standard definitions of Mode bits and Interrupt (I & F) flags in PSRs

Mode_USR        EQU     0x10
Mode_FIQ        EQU     0x11
Mode_IRQ        EQU     0x12
Mode_SVC        EQU     0x13
Mode_ABT        EQU     0x17
Mode_UND        EQU     0x1B
Mode_SYS        EQU     0x1F

I_Bit           EQU     0x80            ; when I bit is set, IRQ is disabled
F_Bit           EQU     0x40            ; when F bit is set, FIQ is disabled


;// <h> Stack Configuration (Stack Sizes in Bytes)
;//   <o0> Undefined Mode      <0x0-0xFFFFFFFF:8>
;//   <o1> Supervisor Mode     <0x0-0xFFFFFFFF:8>
;//   <o2> Abort Mode          <0x0-0xFFFFFFFF:8>
;//   <o3> Fast Interrupt Mode <0x0-0xFFFFFFFF:8>
;//   <o4> Interrupt Mode      <0x0-0xFFFFFFFF:8>
;//   <o5> User/System Mode    <0x0-0xFFFFFFFF:8>
;// </h>

UND_Stack_Size  EQU     0x00000000
SVC_Stack_Size  EQU     0x00000008
ABT_Stack_Size  EQU     0x00000000
FIQ_Stack_Size  EQU     0x00000000
IRQ_Stack_Size  EQU     0x00000100
USR_Stack_Size  EQU     0x00000400

ISR_Stack_Size  EQU     (UND_Stack_Size + SVC_Stack_Size + ABT_Stack_Size + \
                         FIQ_Stack_Size + IRQ_Stack_Size)

                AREA    STACK, NOINIT, READWRITE, ALIGN=3

Stack_Mem       SPACE   USR_Stack_Size
__initial_sp    SPACE   ISR_Stack_Size
Stack_Top


;// <h> Heap Configuration
;//   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF>
;// </h>

Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


; System Configuration registers (CFG) definitions
CFG_BASE        EQU     0x40000000      ; Configuration Registers  Base Address
CFG_R0_OFS      EQU     0x00            ; Configuration Register 0       Offset
CFG_PCGR0_OFS   EQU     0x08            ; Periph Clock Gating Register 0 Offset
CFG_PCGR1_OFS   EQU     0x0C            ; Periph Clock Gating Register 1 Offset
CFG_PCGR0_Rst   EQU     0x00000001      ; PCGR0 Peripherals Mask
CFG_PCGR1_Rst   EQU     0x00000000      ; PCGR1 Peripherals Mask

;// <e> Reset all Device Peripherals (except RAM)
PERIPH_RESET    EQU     0
;// </e>


; Enhanced Interrupt Controller (EIC) definitions
EIC_BASE        EQU     0xFFFFFC00      ; EIC Base Address
ICR_OFS         EQU     0x00            ; Interrupt Control Reg. Offset
CIPR_OFS        EQU     0x08            ; Curr. Int. Priority R. Offset
IVR_OFS         EQU     0x18            ; Interrupt Vector Reg. Offset
FIR_OFS         EQU     0x1C            ; Fast Interrupt Reg. Offset
IER0_OFS        EQU     0x20            ; Interrupt Enable Reg. 0 Offset
IER1_OFS        EQU     0x24            ; Interrupt Enable Reg. 1 Offset
IPR0_OFS        EQU     0x40            ; Interrupt Pending Reg. 0 Offset
IPR1_OFS        EQU     0x44            ; Interrupt Pending Reg. 1 Offset
SIR0_OFS        EQU     0x60            ; Source Interrupt Reg. 0 Offset

;// <e> Setup EIC and Exception Handlers
EIC_SETUP       EQU     0
;// </e>


                PRESERVE8
                

; Area Definition and Entry Point
;  Startup Code must be linked first at Address at which it expects to run.

		EXPORT  Vectors
                AREA    Reset, CODE, READONLY
                ARM

                IF      EIC_SETUP <> 0

                ; Import Exceptions Handlers
                IMPORT  UndefHandler
                IMPORT  SWIHandler
                IMPORT  PAbtHandler
                IMPORT  DAbtHandler
                IMPORT  IRQHandler
                IMPORT  FIQHandler

                ; Import IRQ Handlers
                IMPORT  PRCCUCMUIRQHandler
                IMPORT  EXTIT01IRQHandler
                IMPORT  EXTIT02IRQHandler
                IMPORT  EXTIT03IRQHandler
                IMPORT  EXTIT04IRQHandler
                IMPORT  EXTIT05IRQHandler
                IMPORT  EXTIT06IRQHandler
                IMPORT  EXTIT07IRQHandler
                IMPORT  EXTIT08IRQHandler
                IMPORT  EXTIT09IRQHandler
                IMPORT  EXTIT10IRQHandler
                IMPORT  EXTIT11IRQHandler
                IMPORT  EXTIT12IRQHandler
                IMPORT  EXTIT13IRQHandler
                IMPORT  EXTIT14IRQHandler
                IMPORT  EXTIT15IRQHandler
                IMPORT  DMATRERRIRQHandler
                IMPORT  TIM1IRQHandler
                IMPORT  TIM2IRQHandler
                IMPORT  TIM3IRQHandler
                IMPORT  TIM4IRQHandler
                IMPORT  TB0IRQHandler
                IMPORT  TB1IRQHandler
                IMPORT  TB2IRQHandler
                IMPORT  TIM5IRQHandler
                IMPORT  TIM6IRQHandler
                IMPORT  TIM7IRQHandler
                IMPORT  TIM8IRQHandler
                IMPORT  TIM9IRQHandler
                IMPORT  UART2IRQHandler
                IMPORT  UART3IRQHandler
                IMPORT  FLASHEOPIRQHandler
                IMPORT  PWM0IRQHandler
                IMPORT  PWM1IRQHandler
                IMPORT  PWM2IRQHandler
                IMPORT  PWM3IRQHandler
                IMPORT  PWM4IRQHandler
                IMPORT  PWM5IRQHandler
                IMPORT  WIUIRQHandler
                IMPORT  WDGWUTIRQHandler
                IMPORT  BSPI0IRQHandler
                IMPORT  BSPI1IRQHandler
                IMPORT  BSPI2IRQHandler
                IMPORT  UART0IRQHandler
                IMPORT  UART1IRQHandler
                IMPORT  I2C0ITERRIRQHandler
                IMPORT  I2C1ITERRIRQHandler
                IMPORT  I2C0ITDDCIRQHandler
                IMPORT  I2C1ITDDCIRQHandler
                IMPORT  CAN0IRQHandler
                IMPORT  CAN1IRQHandler
                IMPORT  CAN2IRQHandler
                IMPORT  DMA0IRQHandler
                IMPORT  DMA1IRQHandler
                IMPORT  DMA2IRQHandler
                IMPORT  DMA3IRQHandler
                IMPORT  ADCIRQHandler
                IMPORT  RTCIRQHandler

                ENDIF

; Exception Vectors
;  Mapped to Address 0.
;  Absolute addressing mode must be used.
;  Dummy Handlers are implemented as infinite loops which can be modified.

Vectors         LDR     PC, Reset_Addr         
                LDR     PC, Undef_Addr
                LDR     PC, SWI_Addr
                LDR     PC, PAbt_Addr
                LDR     PC, DAbt_Addr
                NOP                         ; Reserved Vector 
                IF      EIC_SETUP <> 0                
                LDR     PC,IRQ_Addr
                ELSE
                LDR     PC,[PC, #-0x0408]   ; Vector from EIC_IVR
                ENDIF
                LDR     PC, FIQ_Addr

Reset_Addr      DCD     Reset_Handler
Undef_Addr      DCD     UndefHandler
SWI_Addr        DCD     SWIHandler
PAbt_Addr       DCD     PAbtHandler
DAbt_Addr       DCD     DAbtHandler
                DCD     0                   ; Reserved Address 
IRQ_Addr        DCD     IRQHandler
FIQ_Addr        DCD     FIQHandler


                IF      EIC_SETUP = 0

UndefHandler    B       UndefHandler
SWIHandler      B       SWIHandler
PAbtHandler     B       PAbtHandler
DAbtHandler     B       DAbtHandler
IRQHandler      B       IRQHandler
FIQHandler      B       FIQHandler

PRCCUCMU_Addr   DCD     0                   ; Dummy reference for library

                ELSE

                ; Peripherals IRQ Handlers Address Table
PRCCUCMU_Addr   DCD     PRCCUCMUIRQHandler
EXTIT01_Addr    DCD     EXTIT01IRQHandler
EXTIT02_Addr    DCD     EXTIT02IRQHandler
EXTIT03_Addr    DCD     EXTIT03IRQHandler
EXTIT04_Addr    DCD     EXTIT04IRQHandler
EXTIT05_Addr    DCD     EXTIT05IRQHandler
EXTIT06_Addr    DCD     EXTIT06IRQHandler
EXTIT07_Addr    DCD     EXTIT07IRQHandler
EXTIT08_Addr    DCD     EXTIT08IRQHandler
EXTIT09_Addr    DCD     EXTIT09IRQHandler
EXTIT10_Addr    DCD     EXTIT10IRQHandler
EXTIT11_Addr    DCD     EXTIT11IRQHandler
EXTIT12_Addr    DCD     EXTIT12IRQHandler
EXTIT13_Addr    DCD     EXTIT13IRQHandler
EXTIT14_Addr    DCD     EXTIT14IRQHandler
EXTIT15_Addr    DCD     EXTIT15IRQHandler
DMATRERR_Addr   DCD     DMATRERRIRQHandler
TIM1_Addr       DCD     TIM1IRQHandler
TIM2_Addr       DCD     TIM2IRQHandler
TIM3_Addr       DCD     TIM3IRQHandler
TIM4_Addr       DCD     TIM4IRQHandler
TB0_Addr        DCD     TB0IRQHandler
TB1_Addr        DCD     TB1IRQHandler
TB2_Addr        DCD     TB2IRQHandler
TIM5_Addr       DCD     TIM5IRQHandler
TIM6_Addr       DCD     TIM6IRQHandler
TIM7_Addr       DCD     TIM7IRQHandler
TIM8_Addr       DCD     TIM8IRQHandler
TIM9_Addr       DCD     TIM9IRQHandler
                DCD     0                   ; Reserved
                DCD     0                   ; Reserved
UART2_Addr      DCD     UART2IRQHandler
UART3_Addr      DCD     UART3IRQHandler
FlashEOP_Addr   DCD     FLASHEOPIRQHandler
PWM0_Addr       DCD     PWM0IRQHandler
PWM1_Addr       DCD     PWM1IRQHandler
PWM2_Addr       DCD     PWM2IRQHandler
PWM3_Addr       DCD     PWM3IRQHandler
PWM4_Addr       DCD     PWM4IRQHandler
PWM5_Addr       DCD     PWM5IRQHandler
WIUI_Addr       DCD     WIUIRQHandler
WDGWUT_Addr     DCD     WDGWUTIRQHandler
BSPI0_Addr      DCD     BSPI0IRQHandler
BSPI1_Addr      DCD     BSPI1IRQHandler
BSPI2_Addr      DCD     BSPI2IRQHandler
UART0_Addr      DCD     UART0IRQHandler
UART1_Addr      DCD     UART1IRQHandler
I2C0ITERR_Addr  DCD     I2C0ITERRIRQHandler
I2C1ITERR_Addr  DCD     I2C1ITERRIRQHandler
                DCD     0                   ; Reserved
                DCD     0                   ; Reserved
I2C0ITDDC_Addr  DCD     I2C0ITDDCIRQHandler
I2C1ITDDC_Addr  DCD     I2C1ITDDCIRQHandler
                DCD     0                   ; Reserved
                DCD     0                   ; Reserved
CAN0_Addr       DCD     CAN0IRQHandler
CAN1_Addr       DCD     CAN1IRQHandler
CAN2_Addr       DCD     CAN2IRQHandler
DMA0_Addr       DCD     DMA0IRQHandler
DMA1_Addr       DCD     DMA1IRQHandler
DMA2_Addr       DCD     DMA2IRQHandler
DMA3_Addr       DCD     DMA3IRQHandler
ADC_Addr        DCD     ADCIRQHandler
RTC_Addr        DCD     RTCIRQHandler

                ENDIF
                
                EXPORT  PRCCUCMU_Addr


; Reset Handler

                EXPORT  Reset_Handler
Reset_Handler   
                NOP     ; Wait for OSC stabilization
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP


; Memory Remapping
                IF      :DEF:REMAP
                LDR     R1, =CFG_BASE
                LDR     R0, [R1, #CFG_R0_OFS]
                ORR     R0, R0, #1
                STR     R0, [R1, #CFG_R0_OFS]
                ENDIF


; Reset Peripherals (except RAM)
                IF      PERIPH_RESET <> 0
                LDR     R0, =CFG_BASE
                LDR     R1, =CFG_PCGR0_Rst
                STR     R1, [R0, #CFG_PCGR0_OFS]
                LDR     R1, =CFG_PCGR1_Rst
                STR     R1, [R0, #CFG_PCGR1_OFS]
                ENDIF


; Setup Enhanced Interrupt Controller
                IF      EIC_SETUP <> 0

                LDR     R0, =CFG_BASE       ; Enable the EIC clock
                LDR     R1, [R0, #CFG_PCGR0_OFS]
                ORR     R1, R1, #0x20000000
                STR     R1, [R0, #CFG_PCGR0_OFS]

                NOP                         ; Wait for EIC stabilization
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP

                LDR     R0, =EIC_BASE
                LDR     R1, =0xE59F0000     ; LDR PC,[PC,#ofs] (High 16-bits)
                STR     R1, [R0, #IVR_OFS]  ; Store into IVR[31:16]
                LDR     R1, =PRCCUCMU_Addr  ; IRQ Address Table
                LDR     R2, =0x0FFF         ; Offset Mask
                AND     R1, R1, R2          ; Mask Offset
                LDR     R2, =0xF3E0         ; Jump Offset = 0x03E0
                                            ; 0xFXXX is used to complete the
                                            ; LDR PC,[PC,#ofs]
                                            ; 0x03E0 = 0x03E8 - 8 (Prefetch)
                                            ; 0 = IVR Address + 0x3E8
                ADD     R1, R1, R2          ; Add Jump Offset
                MOV     R2, #64             ; Number of Channels
                MOV     R3, #SIR0_OFS       ; Offset to SIR0
EIC_Loop        MOV     R4, R1, LSL #16     ; Use High 16-bits
                STR     R4, [R0, R3]        ; Store into SIRx
                ADD     R1, R1, #4          ; Next IRQ Address
                ADD     R3, R3, #4          ; Next SIRx
                SUBS    R2, R2, #1          ; Decrement Counter
                BNE     EIC_Loop                               
                ENDIF


; Setup Stack for each mode

                LDR     R0, =Stack_Top

;  Enter Undefined Instruction Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_UND:OR:I_Bit:OR:F_Bit
                MOV     SP, R0
                SUB     R0, R0, #UND_Stack_Size

;  Enter Abort Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_ABT:OR:I_Bit:OR:F_Bit
                MOV     SP, R0
                SUB     R0, R0, #ABT_Stack_Size

;  Enter FIQ Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_FIQ:OR:I_Bit:OR:F_Bit
                MOV     SP, R0
                SUB     R0, R0, #FIQ_Stack_Size

;  Enter IRQ Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_IRQ:OR:I_Bit:OR:F_Bit
                MOV     SP, R0
                SUB     R0, R0, #IRQ_Stack_Size

;  Enter Supervisor Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_SVC:OR:I_Bit:OR:F_Bit
                MOV     SP, R0
                SUB     R0, R0, #SVC_Stack_Size

;  Enter User Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_USR
                IF      :DEF:__MICROLIB

                EXPORT __initial_sp

                ELSE

                MOV     SP, R0
                SUB     SL, SP, #USR_Stack_Size

                ENDIF


; Enter the C code

                IMPORT  __main
                LDR     R0, =__main
                BX      R0


                IF      :DEF:__MICROLIB

                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE
; User Initial Stack & Heap
                AREA    |.text|, CODE, READONLY

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + USR_Stack_Size)
                LDR     R2, = (Heap_Mem +      Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR
                ENDIF


                END
