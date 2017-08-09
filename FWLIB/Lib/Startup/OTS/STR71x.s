;/*****************************************************************************/
;/* STARTUP.S: Startup file for ST STR71x device series                       */
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
; *  REMAP: when set the startup code initializes the register BOOTCR 
; *  which overwrites the settings of the CPU configuration pins. The 
; *  startup and interrupt vectors are remapped from:
; *     0x40000000  default setting
; *     0x60000000  when EXTMEM_MODE is used
; *     0x20000000  when RAM_MODE is used
; *
; *  EXTMEM_MODE: when set the device is configured for code execution
; *  from external memory starting at address 0x60000000. The startup
; *  vectors are located to 0x60000000.
; *
; *  RAM_MODE: when set the device is configured for code execution
; *  from on-chip RAM starting at address 0x20000000. The startup
; *  vectors are located to 0x20000000.
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


; APB Bridge 1 & 2 definitions (Peripherals)
APB1_BASE       EQU     0xC0000000      ; APB Bridge 1 Base Address
APB2_BASE       EQU     0xE0000000      ; APB Bridge 2 Base Address
CKDIS_OFS       EQU     0x10            ; Clock Disable Register Offset
SWRES_OFS       EQU     0x14            ; Software Reset Register Offset
APB1_Mask       EQU     0x27FB          ; APB1 Peripherals Mask
APB2_Mask       EQU     0x7FDD          ; APB2 Peripherals Mask

;// <e> Reset all Device Peripherals
PERIPH_RESET    EQU     0
;// </e>


; External Memory Interface (EMI) definitions
EMI_BASE        EQU     0x6C000000      ; EMI Base Address
BCON0_OFS       EQU     0x00            ; BCON0 Offset
BCON1_OFS       EQU     0x04            ; BCON1 Offset
BCON2_OFS       EQU     0x08            ; BCON2 Offset
BCON3_OFS       EQU     0x0C            ; BCON3 Offset

;// <e> External Memory Interface (EMI)
EMI_SETUP       EQU     0

;//   <h> Bank Configuration 0 (BCON0)
;//     <o.15>    BE: Bank Enable
;//     <o.0..1>  B_SIZE: Bus Size  <0=>  8-bit    <1=> 16-bit
;//                                 <2=> Reserved  <3=> Reserved
;//     <o.2..5>  C_LENGTH: Cycle Length <0-15>
;//               <i> Number of Wait States
;//   </h>
BCON0_Val       EQU     0x0000801D

;//   <h> Bank Configuration 1 (BCON1)
;//     <o.15>    BE: Bank Enable
;//     <o.0..1>  B_SIZE: Bus Size  <0=>  8-bit    <1=> 16-bit
;//                                 <2=> Reserved  <3=> Reserved
;//     <o.2..5>  C_LENGTH: Cycle Length <0-15>
;//               <i> Number of Wait States
;//   </h>
BCON1_Val       EQU     0x0000801D

;//   <h> Bank Configuration 2 (BCON2)
;//     <o.15>    BE: Bank Enable
;//     <o.0..1>  B_SIZE: Bus Size  <0=>  8-bit    <1=> 16-bit
;//                                 <2=> Reserved  <3=> Reserved
;//     <o.2..5>  C_LENGTH: Cycle Length <0-15>
;//               <i> Number of Wait States
;//   </h>
BCON2_Val       EQU     0x0000001D

;//   <h> Bank Configuration 3 (BCON3)
;//     <o.15>    BE: Bank Enable
;//     <o.0..1>  B_SIZE: Bus Size  <0=>  8-bit    <1=> 16-bit
;//                                 <2=> Reserved  <3=> Reserved
;//     <o.2..5>  C_LENGTH: Cycle Length <0-15>
;//               <i> Number of Wait States
;//   </h>
BCON3_Val       EQU     0x0000001D

;// </e> End of EMI


; GPIO Port 2 definitions (for External Memory Interface setup)
GPIO2_BASE      EQU     0xE0005000      ; GPIO2 Base Address
PC0_OFS         EQU     0x00            ; PC0 Offset
PC1_OFS         EQU     0x04            ; PC1 Offset
PC2_OFS         EQU     0x08            ; PC2 Offset
PD_OFS          EQU     0x0C            ; PD Offset


; Enhanced Interrupt Controller (EIC) definitions
EIC_BASE        EQU     0xFFFFF800      ; EIC Base Address
ICR_OFS         EQU     0x00            ; Interrupt Control Reg. Offset
CIPR_OFS        EQU     0x08            ; Curr. Int. Priority R. Offset
IVR_OFS         EQU     0x18            ; Interrupt Vector Reg. Offset
FIR_OFS         EQU     0x1C            ; Fast Interrupt Reg. Offset
IER_OFS         EQU     0x20            ; Interrupt Enable Reg. Offset
IPR_OFS         EQU     0x40            ; Interrupt Pending Reg. Offset
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
                IMPORT  T0TIMIIRQHandler
                IMPORT  FLASHIRQHandler
                IMPORT  RCCUIRQHandler
                IMPORT  RTCIRQHandler
                IMPORT  WDGIRQHandler
                IMPORT  XTIIRQHandler
                IMPORT  USBHPIRQHandler
                IMPORT  I2C0ITERRIRQHandler
                IMPORT  I2C1ITERRIRQHandler
                IMPORT  UART0IRQHandler
                IMPORT  UART1IRQHandler
                IMPORT  UART2IRQHandler
                IMPORT  UART3IRQHandler
                IMPORT  BSPI0IRQHandler
                IMPORT  BSPI1IRQHandler
                IMPORT  I2C0IRQHandler
                IMPORT  I2C1IRQHandler
                IMPORT  CANIRQHandler
                IMPORT  ADC12IRQHandler
                IMPORT  T1TIMIIRQHandler
                IMPORT  T2TIMIIRQHandler
                IMPORT  T3TIMIIRQHandler
                IMPORT  HDLCIRQHandler
                IMPORT  USBLPIRQHandler
                IMPORT  T0TOIIRQHandler
                IMPORT  T0OC1IRQHandler
                IMPORT  T0OC2IRQHandler

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
                LDR     PC,[PC, #-0x0808]   ; Vector from EIC_IVR
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

                ELSE

                ; Peripherals IRQ Handlers Address Table
                EXPORT  T0TIMI_Addr
T0TIMI_Addr     DCD     T0TIMIIRQHandler
FLASH_Addr      DCD     FLASHIRQHandler
RCCU_Addr       DCD     RCCUIRQHandler
RTC_Addr        DCD     RTCIRQHandler
WDG_Addr        DCD     WDGIRQHandler
XTI_Addr        DCD     XTIIRQHandler
USBHP_Addr      DCD     USBHPIRQHandler
I2C0ITERR_Addr  DCD     I2C0ITERRIRQHandler
I2C1ITERR_Addr  DCD     I2C1ITERRIRQHandler
UART0_Addr      DCD     UART0IRQHandler
UART1_Addr      DCD     UART1IRQHandler
UART2_Addr      DCD     UART2IRQHandler
UART3_Addr      DCD     UART3IRQHandler
BSPI0_Addr      DCD     BSPI0IRQHandler
BSPI1_Addr      DCD     BSPI1IRQHandler
I2C0_Addr       DCD     I2C0IRQHandler
I2C1_Addr       DCD     I2C1IRQHandler
CAN_Addr        DCD     CANIRQHandler
ADC12_Addr      DCD     ADC12IRQHandler
T1TIMI_Addr     DCD     T1TIMIIRQHandler
T2TIMI_Addr     DCD     T2TIMIIRQHandler
T3TIMI_Addr     DCD     T3TIMIIRQHandler
                DCD     0                   ; Reserved
                DCD     0                   ; Reserved
                DCD     0                   ; Reserved
HDLC_Addr       DCD     HDLCIRQHandler
USBLP_Addr      DCD     USBLPIRQHandler
                DCD     0                   ; Reserved
                DCD     0                   ; Reserved
T0TOI_Addr      DCD     T0TOIIRQHandler
T0OC1_Addr      DCD     T0OC1IRQHandler
T0OC2_Addr      DCD     T0OC2IRQHandler

                ENDIF


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


; Reset Peripherals
                IF      PERIPH_RESET <> 0
                LDR     R1, =APB1_BASE
                LDR     R2, =APB2_BASE
                LDR     R3, =APB1_Mask
                LDR     R4, =APB2_Mask
                STRH    R3, [R1, #CKDIS_OFS]   ; Disable Clock for APB1 periph.
                STRH    R4, [R2, #CKDIS_OFS]   ; Disable Clock for APB2 periph.
                STRH    R3, [R1, #SWRES_OFS]   ; Keep under Reset APB1 periph.
                STRH    R4, [R2, #SWRES_OFS]   ; Keep under Reset APB2 periph.
                MOV     R0, #10
PR_Loop1        SUBS    R0, R0, #1             ; Wait that selected macrocells
                BNE     PR_Loop1               ; enter reset
                STRH    R0, [R1, #SWRES_OFS]   ; Release Reset of APB1 periph.
                STRH    R0, [R2, #SWRES_OFS]   ; Relase Reset of APB2 periph.
                STRH    R0, [R1, #CKDIS_OFS]   ; Enable Clock for APB1 periph.
                STRH    R0, [R2, #CKDIS_OFS]   ; Enable Clock for APB2 periph.
                MOV     R0, #10
PR_Loop2        SUBS    R0, R0, #1             ; Wait that selected macrocells
                BNE     PR_Loop2               ; exit from reset
                ENDIF


; Setup External Memory Interface (EMI)
                IF      EMI_SETUP <> 0
                LDR     R0, =GPIO2_BASE        ; Configure P2.0..7 for Ext. Bus
                LDR     R1, [R0, #PC0_OFS]
                ORR     R1, R1, #0x0000000F
                STR     R1, [R0, #PC0_OFS]
                LDR     R1, [R0, #PC1_OFS]
                ORR     R1, R1, #0x0000000F
                STR     R1, [R0, #PC1_OFS]
                LDR     R1, [R0, #PC2_OFS]
                ORR     R1, R1, #0x0000000F
                STR     R1, [R0, #PC2_OFS]

                LDR     R0, =EMI_BASE          ; Configure EMI
                LDR     R1, =BCON0_Val
                STR     R1, [R0, #BCON0_OFS]
                LDR     R1, =BCON1_Val
                STR     R1, [R0, #BCON1_OFS]
                LDR     R1, =BCON2_Val
                STR     R1, [R0, #BCON2_OFS]
                LDR     R1, =BCON3_Val
                STR     R1, [R0, #BCON3_OFS]
                ENDIF


; Setup Enhanced Interrupt Controller
                IF      EIC_SETUP <> 0
                LDR     R0, =EIC_BASE
                LDR     R1, =0xE59F0000     ; LDR PC,[PC,#ofs] (High 16-bits)
                STR     R1, [R0, #IVR_OFS]  ; Store into IVR[31:16]
                LDR     R1, =T0TIMI_Addr    ; IRQ Address Table
                LDR     R2, =0x0FFF         ; Offset Mask
                AND     R1, R1, R2          ; Mask Offset
                LDR     R2, =0xF7E0         ; Jump Offset = 0x07E0
                                            ; 0xFXXX is used to complete the
                                            ; LDR PC,[PC,#ofs]
                                            ; 0x07E0 = 0x07E8 - 8 (Prefetch)
                                            ; 0 = IVR Address + 0x7E8
                ADD     R1, R1, R2          ; Add Jump Offset
                MOV     R2, #32             ; Number of Channels
                MOV     R3, #SIR0_OFS       ; Offset to SIR0
EIC_Loop        MOV     R4, R1, LSL #16     ; Use High 16-bits
                STR     R4, [R0, R3]        ; Store into SIRx
                ADD     R1, R1, #4          ; Next IRQ Address
                ADD     R3, R3, #4          ; Next SIRx
                SUBS    R2, R2, #1          ; Decrement Counter
                BNE     EIC_Loop                               
                ENDIF


; Memory Remapping
BOOTCR          EQU     0xA0000050          ; Boot Configuration Register
FLASH_BM        EQU     0x01                ; Boot Mode: Flash at 0
RAM_BM          EQU     0x02                ; Boot Mode: RAM at 0
EXTMEM_BM       EQU     0x03                ; Boot Mode: EXTMEM at 0

                IF      :DEF:REMAP
                MOV     R1, #FLASH_BM
                IF      :DEF:EXTMEM_MODE
                MOV     R1, #EXTMEM_BM
                ENDIF
                IF      :DEF:RAM_MODE
                MOV     R1, #RAM_BM
                ENDIF
                LDR     R0, =BOOTCR
                LDRH    R2, [R0]            ; Read BOOTCR
                BIC     R2, R2, #0x03       ; Clear two LSB bits
                ORR     R2, R2, R1          ; Setup two LSB bits
                STRH    R2, [R0]            ; Write BOOTCR
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
