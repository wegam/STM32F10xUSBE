;/*****************************************************************************/
;/* STR75x.S: Startup file for ST STR75x device series                        */
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
; *  The STR75x.S code is executed after CPU Reset. This file may be 
; *  translated with the following SET symbols. In uVision these SET 
; *  symbols are entered under Options -> ASM -> Define.
; *
; *  REMAP: when set, the startup code initializes the register CFG_GLCONF 
; *  which overwrites the settings of the CPU boot configuration pins. The 
; *  startup and interrupt vectors are remapped from:
; *     0x20000000  when default setting  is used
; *     0x2010C000  when SYSMEM_MODE      is used
; *     0x40000000  when RAM_MODE         is used
; *     0x80000000  when EXT_SEEPROM_MODE is used
; *     0x80000000  when EXT_SFLASH_MODE  is used
; *
; *  SYSMEM_MODE:      when set the System Memory     is remapped to address 0.
; *  RAM_MODE:         when set the Internal SRAM     is remapped to address 0.
; *  EXT_SEEPROM_MODE: when set the Ext Serial EEPROM is remapped to address 0.
; *  EXT_SFLASH_MODE:  when set the Ext Serial Flash  is remapped to address 0.
; */


;----------------------- Global Constants Definitions --------------------------

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


;----------------------- Stack/Heap Definitons ---------------------------------

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


;----------------------- Global Configuration Definitions ----------------------

CFG_GLCONF_ADR  EQU     0x60000010      ; Global Configuration Register  Address 

;// <e> Global Configuration (CFG_GLCONF)
;//   <o1.9>          USB_FILTEN: USB Filter Enable
;//   <o1.8>          FLASH_BURST: Flash Burst Mode Enable
;// </e>

GLBCONF_SETUP   EQU     0
CFG_GLCONF_Val  EQU     0x00000300


;----------------------- MCU Reset and Clock Control (MRCC) Definitions --------

MRCC_BASE       EQU     0x60000020      ; MCU Reset and Clock Ctrl  Base Address
MRCC_CLKCTL_OFS EQU     0x00            ; Clock Control Register          Offset
MRCC_RFSR_OFS   EQU     0x04            ; Reset Flag and Status Register  Offset
MRCC_PCLKEN_OFS EQU     0x10            ; Periph Clock Enable Register    Offset
MRCC_PSWRES_OFS EQU     0x14            ; Periph Software Reset Register  Offset
MRCC_BKP0_OFS   EQU     0x20            ; Backup Register 0               Offset
MRCC_BKP1_OFS   EQU     0x24            ; Backup Register 1               Offset

; Constants
CKSEL_BIT       EQU     (1<<23)         ; Clock Source Select                Bit  
CKOSCSEL_BIT    EQU     (1<<21)         ; Oscillator Selection               Bit  
BCOUNTF_BIT     EQU     (1<<24)         ; Builder Counter Flag               Bit
NCKDF_BIT       EQU     (1<<18)         ; No Clock Detected Flag             Bit
PLL_EN_BIT      EQU     (1<<24)         ; PLL Enabled                        Bit  
PLL2_EN_BIT     EQU     (1<<25)         ; PLL Output 2 Enable                Bit  
PLL_LOCK_BIT    EQU     (1<<31)         ; PLL Locked                         Bit  
MUL_MASK        EQU     0x18000000      ; Clock Multiplication              Mask
DIV_MASK        EQU     0x0000001F      ; Clock Divider                     Mask
PERIPH_MASK     EQU     0x1975623F      ; Existing Peripherals              Mask

;// <e0> Setup Reset and Clock Control
CLOCK_SETUP     EQU     0

;//   <e0> Clock Control Register (MRCC_CLKCTL)
;//     <o1.27..28>   MX: PLL Multiplication factors
;//       <0=>  Multiplication by 16   <1=> Multiplication by 15
;//       <2=>  Multiplication by 14   <3=> Multiplication by 12
;//     <o1.25>       PLL2EN: PLL output 2 enable
;//     <o1.24>       PLLEN: PLL enable
;//     <o1.23>       CKSEL: CK_SYS Clock Source selection
;//       <0=> CK_PLL1 clock as CK_SYS source
;//       <1=> CK_OSC clock as CK_SYS source
;//     <o1.22>       CKUSBSEL: USB clock selection
;//       <0=> Internal USB clock used
;//       <1=> External Alternate Function USB clock
;//     <o1.21>       CKOSCSEL: CK_OSC source selection
;//       <0=> CK_OSC4M as CK_OSC source
;//       |1=| CK_RTC as CK_OSC source
;//     <o1.19>       NCKDIE: No Clock detected interrupt Enable
;//     |o1.17|       OSC4MOFF: OSC4M Oscillator Disable
;//     |o1.16|       OSC4MBYP: OSC4M Oscillator Bypass Enable
;//     <o1.6..7>     MCOS: Main clock Out selection
;//       <0=> HCLK  <1=> PCLK  <2=> CK_OSC4M  <3=> CK_PLL2
;//     <o1.5>        MCOP: Main Clock Out Prescaler
;//       <0=> MCO is directly applied on MCO pin
;//       <1=> Additional divider by 2 is applied to MCO
;//     <o1.3..4>     HPRESC: Prescaler selection for HCLK
;//       <0=> HCLK = CK_SYS    <1=> HCLK = CK_SYS/2
;//       <2=> HCLK = CK_SYS/4  <3=> HCLK = CK_SYS/8
;//     <o1.2>        PPRESC2: Prescaler selection for PCLK
;//       <0=> PCLK = CK_TIM    <1=> PCLK = CK_TIM/2
;//     <o1.0..1>     PPRESC: Prescaler selection for CK_TIM
;//       <0=> CK_TIM = HCLK    <1=> CK_TIM = HCLK/2
;//       <2=> CK_TIM = HCLK/4  <3=> CK_TIM = HCLK/8
;//   </e>
;//   <h> Peripheral Clock Enable Register (MRCC_PCLKEN)
;//     <o2.28>       EXTIT Clock Enable
;//     <o2.27>       RTC Clock Enable
;//     <o2.24>       GPIO Clock Enable
;//     <o2.22>       UART2 Clock Enable
;//     <o2.21>       UART1 Clock Enable
;//     <o2.20>       UART0 Clock Enable
;//     <o2.18>       I2C Clock Enable
;//     <o2.16>       CAN Clock Enable
;//     <o2.14>       SSP1 Clock Enable
;//     <o2.13>       SSP0 Clock Enable
;//     <o2.9>         USB Clock Enable
;//     <o2.5>        PWM Clock Enable
;//     <o2.4>        TIM2 Clock Enable
;//     <o2.3>        TIM1 Clock Enable
;//     <o2.2>        TIM0 Clock Enable
;//     <o2.1>        TB Clock Enable
;//     <o2.0>        ADC Clock Enable
;//   </h>
;//   <e3.0> Reset all peripherals
;//   </e>
;//   <e4.0> Setup Backup Registers
;//     <o5> Backup 0 Register (MRCC_BKP0) <0x00000000-0xFFFFFFFF>
;//     <o6> Backup 1 Register (MRCC_BKP1) <0x00000000-0xFFFFFFFF>
;//   </e>
;// </e>
MRCC_SETUP      EQU     1
MRCC_CLKCTL_Val EQU     0x0B000003
MRCC_PCLKEN_Val EQU     0x1975623F
PERIPH_RESET    EQU     0x00000001
BKP_SETUP       EQU     0x00000000
MRCC_BKP0_Val   EQU     0x55555555
MRCC_BKP1_Val   EQU     0xAAAAAAAA


;----------------------- Serial Memory Interface (SMI) Definitions -------------

GPIO_BASE       EQU     0xFFFFE400      ; GPIO                      Base Address
SMI_BASE        EQU     0x90000000      ; SMI                       Base Address
GPIO_REMAP0R_OFS EQU    0x20            ; I/O Remapping Register 0        Offset
SMI_CR1_OFS     EQU     0x00            ; SMI Control Register 0          Offset
SMI_CR2_OFS     EQU     0x04            ; SMI Control Register 1          Offset

;// <e> Setup Serial Memory Interface (SMI)
;//   <e1.0> Bank 0 Enable
;//   </e>
;//   <e1.1> Bank 1 Enable
;//   </e>
;//   <e1.2> Bank 2 Enable
;//   </e>
;//   <e1.3> Bank 3 Enable
;//   </e>
;//   <h> SMI Control Register 1
;//     <o2.29>       WBM: Write Burst Mode
;//       <0=> Chip select is released at the end of AHB request
;//       <1=> Chip select is active until this bit is reset
;//     <o2.28>       SW: Software Mode
;//       <0=> Hardware Mode: External memory is accessible with AHB transfers
;//       <1=> Software Mode: External memory access with AHB not allowed
;//     <o2.16..23>   HOLD: Clock Hold Period Selection <0-255>
;//       <i> Number of SMI_CK hold periods between each byte
;//     <o2.25>       FAST: Fast Read Mode Selection
;//       <0=> Normal Read: opcode 03h + address + reception
;//       <1=> Fast Read: opcode 0Bh + address + dummy byte + reception
;//     <o2.8..14>    PRESC: Prescaler value <0-127>
;//       <i> fAHB divider value (divider is for value 0 also 1)
;//     <o2.4..7>     TCS: Deselect Time <0-15>
;//       <i> CS remains deselected for (this value + 1) SMI_CK periods after deselection
;//     <o2.3>        BE: Bank 3 Enable Bit
;//     <o2.2>        BE: Bank 2 Enable Bit
;//     <o2.1>        BE: Bank 1 Enable Bit
;//     <o2.0>        BE: Bank 0 Enable Bit
;//   </h>
;//   <h> SMI Control Register 2
;//     <o3.12..13>   BS: Bank Select
;//       <0=> Bank 0  <1=> Bank 1  <2=> Bank 2  <3=> Bank 3
;//     <o3.9>        WCIE: Write Complete Interrupt Enable
;//     <o3.8>        TFIE: Transfer Finished Interrupt Enable
;//     <o3.4..6>     RECEPTION LENGTH: Reception Length
;//       <0=> 0 bytes  <1=> 1 bytes  <2=> 2 bytes  <3=> 3 bytes <4=> 4 bytes
;//       <i> Number of bytes to be received from external memory
;//     <o3.0..2>     TRANSMISSION LENGTH: Transmission Length
;//       <0=> 0 bytes  <1=> 1 bytes  <2=> 2 bytes  <3=> 3 bytes <4=> 4 bytes
;//       <i> Number of bytes to be transmitted to external memory
;//   </h>
;// </e>

SMI_SETUP       EQU     0
GPIO_REMAP0R_Val EQU    0x00000001
SMI_CR1_Val     EQU     0x00000251
SMI_CR2_Val     EQU     0x00000000


;----------------------- Enhanced Interrupt Controller (EIC) Definitions -------

EIC_BASE        EQU     0xFFFFF800      ; EIC                       Base Address
EIC_ICR_OFS     EQU     0x00            ; Interrupt Control Register      Offset
EIC_CIPR_OFS    EQU     0x08            ; Current Interrupt Priority Reg  Offset
EIC_IVR_OFS     EQU     0x18            ; Interrupt Vector Register       Offset
EIC_FIR_OFS     EQU     0x1C            ; Fast Interrupt Register         Offset
EIC_IER_OFS     EQU     0x20            ; Interrupt Enable Register       Offset
EIC_IPR_OFS     EQU     0x40            ; Interrupt Pending Register      Offset
EIC_SIR0_OFS    EQU     0x60            ; Source Interrupt Register 0     Offset

;// <e> Setup Enhanced Interrupt Controller (EIC) and Exception Handlers
EIC_SETUP       EQU     0
;// </e>


;----------------------- CODE --------------------------------------------------

                PRESERVE8
                

; Area Definition and Entry Point
;  Startup Code must be linked first at Address at which it expects to run.

		EXPORT  Vectors
                AREA    Reset, CODE, READONLY
                ARM

                IF      EIC_SETUP != 0

                ; Import Exceptions Handlers
                IMPORT  UndefHandler
                IMPORT  SWIHandler
                IMPORT  PAbtHandler
                IMPORT  DAbtHandler
                IMPORT  IRQHandler
                IMPORT  FIQHandler

                ; Import IRQ Handlers
                IMPORT  WAKUP_IRQHandler
                IMPORT  TIM2_OC2_IRQHandler
                IMPORT  TIM2_OC1_IRQHandler
                IMPORT  TIM2_IC12_IRQHandler
                IMPORT  TIM2_UP_IRQHandler
                IMPORT  TIM1_OC2_IRQHandler
                IMPORT  TIM1_OC1_IRQHandler
                IMPORT  TIM1_IC12_IRQHandler
                IMPORT  TIM1_UP_IRQHandler
                IMPORT  TIM0_OC2_IRQHandler
                IMPORT  TIM0_OC1_IRQHandler
                IMPORT  TIM0_IC12_IRQHandler
                IMPORT  TIM0_UP_IRQHandler
                IMPORT  PWM_OC123_IRQHandler
                IMPORT  PWM_EM_IRQHandler
                IMPORT  PWM_UP_IRQHandler
                IMPORT  I2C_IRQHandler
                IMPORT  SSP1_IRQHandler
                IMPORT  SSP0_IRQHandler
                IMPORT  UART2_IRQHandler
                IMPORT  UART1_IRQHandler
                IMPORT  UART0_IRQHandler
                IMPORT  CAN_IRQHandler
                IMPORT  USB_LP_IRQHandler
                IMPORT  USB_HP_IRQHandler
                IMPORT  ADC_IRQHandler
                IMPORT  DMA_IRQHandler
                IMPORT  EXTIT_IRQHandler
                IMPORT  MRCC_IRQHandler
                IMPORT  FLASHSMI_IRQHandler
                IMPORT  RTC_IRQHandler
                IMPORT  TB_IRQHandler

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
                IF      EIC_SETUP != 0                
                LDR     PC, IRQ_Addr
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
WAKUP_Addr      DCD	    WAKUP_IRQHandler
TIM2_OC2_Addr   DCD	    TIM2_OC2_IRQHandler
TIM2_OC1_Addr   DCD	    TIM2_OC1_IRQHandler
TIM2_IC12_Addr  DCD	    TIM2_IC12_IRQHandler
TIM2_UP_Addr    DCD	    TIM2_UP_IRQHandler
TIM1_OC2_Addr   DCD	    TIM1_OC2_IRQHandler
TIM1_OC1_Addr   DCD	    TIM1_OC1_IRQHandler
TIM1_IC12_Addr  DCD	    TIM1_IC12_IRQHandler
TIM1_UP_Addr    DCD	    TIM1_UP_IRQHandler
TIM0_OC2_Addr   DCD	    TIM0_OC2_IRQHandler
TIM0_OC1_Addr   DCD	    TIM0_OC1_IRQHandler
TIM0_IC12_Addr  DCD	    TIM0_IC12_IRQHandler
TIM0_UP_Addr    DCD	    TIM0_UP_IRQHandler
PWM_OC123_Addr  DCD	    PWM_OC123_IRQHandler
PWM_EM_Addr     DCD	    PWM_EM_IRQHandler
PWM_UP_Addr     DCD	    PWM_UP_IRQHandler
I2C_Addr        DCD	    I2C_IRQHandler
SSP1_Addr       DCD	    SSP1_IRQHandler
SSP0_Addr       DCD	    SSP0_IRQHandler
UART2_Addr      DCD	    UART2_IRQHandler
UART1_Addr      DCD	    UART1_IRQHandler
UART0_Addr      DCD	    UART0_IRQHandler
CAN_Addr        DCD	    CAN_IRQHandler
USB_LP_Addr     DCD	    USB_LP_IRQHandler
USB_HP_Addr     DCD	    USB_HP_IRQHandler
ADC_Addr        DCD	    ADC_IRQHandler
DMA_Addr        DCD	    DMA_IRQHandler
EXTIT_Addr      DCD	    EXTIT_IRQHandler
MRCC_Addr       DCD	    MRCC_IRQHandler
FLASHSMI_Addr   DCD	    FLASHSMI_IRQHandler
RTC_Addr        DCD	    RTC_IRQHandler
TB_Addr         DCD	    TB_IRQHandler

                ENDIF


; Code Start - Reset Handler ---------------------------------------------------

                EXPORT  Reset_Handler
Reset_Handler   


; Wait for clock to stabilize --------------------------------------------------

                LDR     R1, =10000
WAIT            SUBS    R1, R1, #1
                BNE     WAIT


; Setup Global Configuration Register ------------------------------------------

                IF      GLBCONF_SETUP != 0
                LDR     R0, =CFG_GLCONF_ADR
                LDR     R1, [R0]
                LDR     R2, =CFG_GLCONF_Val
                BIC     R1, R1, #0x0300
                ORR     R1, R1, R2
                STR     R1, [R0]
                ENDIF


; MCU Reset and Clock Control --------------------------------------------------

                IF      CLOCK_SETUP != 0
                IF      MRCC_SETUP != 0

                ; Setup peripheral clock dividers
                LDR     R0, =MRCC_BASE
                LDR     R1, =MRCC_CLKCTL_Val
                LDR     R2, [R0, #MRCC_CLKCTL_OFS]  ; load current CLKCTL value
                LDR     R5, =DIV_MASK
                MVN     R6, R5
                AND     R5, R1, R5
                AND     R6, R2, R6
                ORR     R5, R5, R6
                STR     R5, [R0, #MRCC_CLKCTL_OFS]  ; load div values
                
                ; if OSC4M is used as system clock source (OSC_4M as CK_SYS) ---
                IF      (MRCC_CLKCTL_Val:AND:CKOSCSEL_BIT) = 0
                LDR     R2, [R0, #MRCC_CLKCTL_OFS]	; Wait presence of osc clk
                BIC     R2, R2, #NCKDF_BIT
WAIT_OSC_PRES   STR     R2, [R0, #MRCC_CLKCTL_OFS]  ; Write 0 to NCKDF Flag
                LDR     R1, [R0, #MRCC_CLKCTL_OFS]  ; Read       NCKDF Flag
                ANDS    R1, R1, #NCKDF_BIT
                BNE     WAIT_OSC_PRES				; if NCKDF Flag set to 1

WAIT_CLK_STABIL LDR     R1, [R0, #MRCC_RFSR_OFS]    ; Wait stabil clock
                ANDS    R1, R1, #BCOUNTF_BIT
                BEQ     WAIT_CLK_STABIL

                LDR     R1, [R0, #MRCC_CLKCTL_OFS]  ; Switch CK_SYS to CK_OSC4M
                ORR     R2, R1, #CKSEL_BIT
                STR     R2, [R0, #MRCC_CLKCTL_OFS]

                ; if PLL is used as system clock source (PLL as CK_SYS) ----
                IF      (MRCC_CLKCTL_Val:AND:CKSEL_BIT) = 0

                ; PLL is by default disabled

                ; Setup PLL multiply factor
                LDR     R1, =MRCC_CLKCTL_Val
                LDR     R2, [R0, #MRCC_CLKCTL_OFS]  ; load current CLKCTL value
                LDR     R3, =MUL_MASK
                MVN     R4, R3
                AND     R3, R1, R3
                AND     R4, R2, R4
                ORR     R3, R3, R4
                STR     R3, [R0, #MRCC_CLKCTL_OFS]  ; load mul value

                LDR     R1, [R0, #MRCC_CLKCTL_OFS]  ; Enable the PLL
                ORR     R1, R1, #PLL_EN_BIT
                STR     R1, [R0, #MRCC_CLKCTL_OFS]

PLL_LOCKING     LDR     R1, [R0, #MRCC_CLKCTL_OFS]  ; Wait for PLL to lock
                ANDS    R1, R1, #PLL_LOCK_BIT
                BEQ     PLL_LOCKING


                ; if PLL2EN is used  set it after PLL is locked ----
                IF      (MRCC_CLKCTL_Val:AND:PLL2_EN_BIT) != 0

                LDR     R1, [R0, #MRCC_CLKCTL_OFS]  ; Enable the PLL2EN
                ORR     R1, R1, #PLL2_EN_BIT
                STR     R1, [R0, #MRCC_CLKCTL_OFS]

                ENDIF   ; ------------------------------------------

                LDR     R1, [R0, #MRCC_CLKCTL_OFS]  ; Switch CK_SYS to CK_PLL1
                BIC     R1, R1, #CKSEL_BIT
                STR     R1, [R0, #MRCC_CLKCTL_OFS]

                ENDIF   ; --------------------------------------------------
                ENDIF   ; ------------------------------------------------------
                ENDIF

                ; Enable peripheral clocks if requested
                IF      MRCC_PCLKEN_Val != 0
                LDR     R0, =MRCC_BASE
                LDR     R1, =MRCC_PCLKEN_Val
                STR     R1, [R0, #MRCC_PCLKEN_OFS]  ; Enable Peripheral Clocks
                ENDIF

                IF      PERIPH_RESET != 0           ; Reset Peripherals
                LDR     R0, =MRCC_BASE
                LDR     R1, =PERIPH_MASK
                MOV     R2, #0
                STR     R1, [R0, #MRCC_PSWRES_OFS]  ; Put Periphs Under Reset
                NOP                                 ; Wait for Periphs to Reset
                NOP
                NOP
                NOP
                STR     R2, [R0, #MRCC_PSWRES_OFS]  ; Release Periphs Reset  
                ENDIF

                IF      BKP_SETUP != 0              ; Setup Backup Registers
                LDR     R0, =MRCC_BASE
                LDR     R1, =MRCC_BKP0_Val
                STR     R1, [R0, #MRCC_BKP0_OFS]    ; Backup Register 0
                LDR     R1, =MRCC_BKP1_Val
                STR     R1, [R0, #MRCC_BKP1_OFS]    ; Backup Register 1
                ENDIF

                ENDIF                               ; for CLOCK_SETUP


; Setup Serial Memory Interface (SMI) ------------------------------------------

                IF      SMI_SETUP != 0
                LDR     R0, =GPIO_BASE      ; Setup GPIO port functions for SM
                MOV     R1, #GPIO_REMAP0R_Val
                LDR     R1, [R0, #GPIO_REMAP0R_OFS]

                LDR     R0, =SMI_BASE       ; Setup SMI
                LDR     R1, =SMI_CR1_Val
                LDR     R2, =SMI_CR2_Val
                STR     R1, [R0, #SMI_CR1_OFS]
                STR     R2, [R0, #SMI_CR2_OFS]
                ENDIF


; Setup Enhanced Interrupt Controller (EIC) ------------------------------------

                IF      EIC_SETUP != 0
                LDR     R0, =EIC_BASE
                LDR     R1, =0xE59F0000     ; LDR PC,[PC,#ofs] (High 16-bits)
                STR     R1, [R0, #EIC_IVR_OFS] ; Store into IVR[31:16]
                LDR     R1, =WAKUP_Addr     ; IRQ Address Table
                LDR     R2, =0x0FFF         ; Offset Mask
                AND     R1, R1, R2          ; Mask Offset
                LDR     R2, =0xF7E0         ; Jump Offset = 0x07E0
                                            ; 0xFXXX is used to complete the
                                            ; LDR PC,[PC,#ofs]
                                            ; 0x07E0 = 0x07E8 - 8 (Prefetch)
                                            ; 0 = IVR Address + 0x7E8
                ADD     R1, R1, R2          ; Add Jump Offset
                MOV     R2, #32             ; Number of Channels
                MOV     R3, #EIC_SIR0_OFS   ; Offset to SIR0
EIC_Loop        MOV     R4, R1, LSL #16     ; Use High 16-bits
                STR     R4, [R0, R3]        ; Store into SIRx
                ADD     R1, R1, #4          ; Next IRQ Address
                ADD     R3, R3, #4          ; Next SIRx
                SUBS    R2, R2, #1          ; Decrement Counter
                BNE     EIC_Loop                               
                ENDIF


; Memory Remap -----------------------------------------------------------------

FLASH_BM        EQU     0x00                ; Boot Mode: Int Flash          at 0
SYSMEM_BM       EQU     0x01                ; Boot Mode: System Memory      at 0
RAM_BM          EQU     0x02                ; Boot Mode: Int SRAM           at 0
EXT_SEEPROM_BM  EQU     0x03                ; Boot Mode: Ext Serial EEPROM  at 0
EXT_SFLASH_BM   EQU     0x07                ; Boot Mode: Ext Serial Flash   at 0

                IF      :DEF:REMAP
                MOV     R1, #FLASH_BM
                IF      :DEF:SYSMEM_MODE
                MOV     R1, #SYSMEM_BM
                ELIF   :DEF:RAM_MODE
                MOV     R1, #RAM_BM
                ELIF   :DEF:EXT_SEEPROM_MODE
                MOV     R1, #EXT_SEEPROM_BM
                ELIF   :DEF:EXT_SFLASH_MODE
                MOV     R1, #EXT_SFLASH_BM
                ENDIF
                LDR     R0, =CFG_GLCONF_ADR
                LDR     R2, [R0]            ; Read  CFG_GLCONF
                BIC     R2, R2, #0x07       ; Clear three LSB bits
                ORR     R2, R2, R1          ; Setup three LSB bits
                STR     R2, [R0]            ; Write CFG_GLCONF
                ENDIF


; Reset Handler ----------------------------------------------------------------

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
