/******************************** User_library *********************************
* 文件名 	: STM32_SDCard.H
* 作者   	: wegam@sina.com
* 版本   	: V
* 日期   	: 2016/01/01
* 说明   	: 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#ifndef __STM32_SDCard_H
#define __STM32_SDCard_H

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_nvic.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_dma.h"


#include "stm32f10x_sdio.h"			//SDIO

#include "STM32F10x_BitBand.H"					//使用位绑定

#include	"stdio.h"			//用于printf
#include	"string.h"			//用于printf
#include	"stdarg.h"			//用于获取不确定个数的参数
#include	"stdlib.h"			//malloc动态申请内存空间


/* Private define ------------------------------------------------------------*/
//____________项目配置
#define	SDCard_DMA
#define	SDCard_BITBAND			//位绑定


//____________SPI速度定义
#define SD_CLK_SpeedL	0
#define SD_CLK_SpeedH	1
//____________端口定义
#define SDCard_SPI_PORT			SPI1

#define SDCard_SPI_Periph		RCC_APB2Periph_SPI1

#define SDCard_CS_PORT			GPIOC
#define SDCard_CS_PIN				GPIO_Pin_6
#define SDCard_CS_Periph		RCC_APB2Periph_GPIOC

#define SDCard_CLK_PORT			GPIOA
#define SDCard_CLK_PIN			GPIO_Pin_5
#define SDCard_CLK_Periph		RCC_APB2Periph_GPIOA

#define SDCard_MISO_PORT		GPIOA
#define SDCard_MISO_PIN			GPIO_Pin_6
#define SDCard_MISO_Periph		RCC_APB2Periph_GPIOA

#define SDCard_MOSI_PORT		GPIOA
#define SDCard_MOSI_PIN			GPIO_Pin_7
#define SDCard_MOSI_Periph		RCC_APB2Periph_GPIOA

//____________CS控制定义
#ifdef	SDCard_BITBAND			//位绑定
	#define SDCard_CS_OFF			PC6=1
	#define SDCard_CS_ON			PC6=0
#else
	#define SDCard_CS_OFF			GPIO_SetBits(SDCard_CS_PORT,	SDCard_CS_PIN)			//取消片选
	#define SDCard_CS_ON			GPIO_ResetBits(SDCard_CS_PORT,	SDCard_CS_PIN)		//使能片选
#endif


#define		__IO			volatile
#define		uint8_t		unsigned char
#define		uint16_t		unsigned short
#define		uint32_t	unsigned int

// SD卡类型定义  
//#define SD_TYPE_ERR     0X00
//#define SD_TYPE_MMC     0X01
//#define SD_TYPE_V1      0X02
//#define SD_TYPE_V2      0X04
//#define SD_TYPE_V2HC    0X06

// SD指令表 	   
#define CMD0    0       //卡复位
#define CMD1    1
#define CMD8    8       //命令8 ，SEND_IF_COND
#define CMD9    9       //命令9 ，读CSD数据
#define CMD10   10      //命令10，读CID数据
#define CMD12   12      //命令12，停止数据传输
#define CMD16   16      //命令16，设置SectorSize 应返回0x00
#define CMD17   17      //命令17，读sector
#define CMD18   18      //命令18，读Multi sector
#define CMD23   23      //命令23，设置多sector写入前预先擦除N个block
#define CMD24   24      //命令24，写sector
#define CMD25   25      //命令25，写Multi sector
#define CMD41   41      //命令41，应返回0x00
#define CMD55   55      //命令55，应返回0x01
#define CMD58   58      //命令58，读OCR信息
#define CMD59   59      //命令59，使能/禁止CRC，应返回0x00
//数据写入回应字意义
#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF
//SD卡回应标记字
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF
	
#define SD_NULL 0
#define SD_STATIC_FLAGS               ((u32)0x000005FF)
#define SD_CMD0TIMEOUT                ((u32)0x00002710)
#define SD_FIFO_Address               ((u32)0x40018080)

/* Mask for errors Card Status R1 (OCR Register) */
#define SD_OCR_ADDR_OUT_OF_RANGE        ((u32)0x80000000)
#define SD_OCR_ADDR_MISALIGNED          ((u32)0x40000000)
#define SD_OCR_BLOCK_LEN_ERR            ((u32)0x20000000)
#define SD_OCR_ERASE_SEQ_ERR            ((u32)0x10000000)
#define SD_OCR_BAD_ERASE_PARAM          ((u32)0x08000000)
#define SD_OCR_WRITE_PROT_VIOLATION     ((u32)0x04000000)
#define SD_OCR_LOCK_UNLOCK_FAILED       ((u32)0x01000000)
#define SD_OCR_COM_CRC_FAILED           ((u32)0x00800000)
#define SD_OCR_ILLEGAL_CMD              ((u32)0x00400000)
#define SD_OCR_CARD_ECC_FAILED          ((u32)0x00200000)
#define SD_OCR_CC_ERROR                 ((u32)0x00100000)
#define SD_OCR_GENERAL_UNKNOWN_ERROR    ((u32)0x00080000)
#define SD_OCR_STREAM_READ_UNDERRUN     ((u32)0x00040000)
#define SD_OCR_STREAM_WRITE_OVERRUN     ((u32)0x00020000)
#define SD_OCR_CID_CSD_OVERWRIETE       ((u32)0x00010000)
#define SD_OCR_WP_ERASE_SKIP            ((u32)0x00008000)
#define SD_OCR_CARD_ECC_DISABLED        ((u32)0x00004000)
#define SD_OCR_ERASE_RESET              ((u32)0x00002000)
#define SD_OCR_AKE_SEQ_ERROR            ((u32)0x00000008)
#define SD_OCR_ERRORBITS                ((u32)0xFDFFE008)

/* Masks for R6 Response */
#define SD_R6_GENERAL_UNKNOWN_ERROR     ((u32)0x00002000)
#define SD_R6_ILLEGAL_CMD               ((u32)0x00004000)
#define SD_R6_COM_CRC_FAILED            ((u32)0x00008000)

#define SD_VOLTAGE_WINDOW_SD            ((u32)0x80100000)
#define SD_HIGH_CAPACITY                ((u32)0x40000000)
#define SD_STD_CAPACITY                 ((u32)0x00000000)
#define SD_CHECK_PATTERN                ((u32)0x000001AA)

#define SD_MAX_VOLT_TRIAL               ((u32)0x0000FFFF)
#define SD_ALLZERO                      ((u32)0x00000000)

#define SD_WIDE_BUS_SUPPORT             ((u32)0x00040000)
#define SD_SINGLE_BUS_SUPPORT           ((u32)0x00010000)
#define SD_CARD_LOCKED                  ((u32)0x02000000)
#define SD_CARD_PROGRAMMING             ((u32)0x00000007)
#define SD_CARD_RECEIVING               ((u32)0x00000006)
#define SD_DATATIMEOUT                  ((u32)0x000FFFFF)
#define SD_0TO7BITS                     ((u32)0x000000FF)
#define SD_8TO15BITS                    ((u32)0x0000FF00)
#define SD_16TO23BITS                   ((u32)0x00FF0000)
#define SD_24TO31BITS                   ((u32)0xFF000000)
#define SD_MAX_DATA_LENGTH              ((u32)0x01FFFFFF)

#define SD_HALFFIFO                     ((u32)0x00000008)
#define SD_HALFFIFOBYTES                ((u32)0x00000020)

/* Command Class Supported */
#define SD_CCCC_LOCK_UNLOCK             ((u32)0x00000080)
#define SD_CCCC_WRITE_PROT              ((u32)0x00000040)
#define SD_CCCC_ERASE                   ((u32)0x00000020)

/* Following commands are SD Card Specific commands.
   SDIO_APP_CMD should be sent before sending these commands. */
#define SDIO_SEND_IF_COND               ((u32)0x00000008)

#define SDIO_INIT_CLK_DIV                  ((u8)0xB2)
#define SDIO_TRANSFER_CLK_DIV              ((u8)0x1)


/* SDIO Commands  Index */
#define SDIO_GO_IDLE_STATE                       ((uint8_t)0)
#define SDIO_SEND_OP_COND                        ((uint8_t)1)
#define SDIO_ALL_SEND_CID                        ((uint8_t)2)
#define SDIO_SET_REL_ADDR                        ((uint8_t)3) /* SDIO_SEND_REL_ADDR for SD Card */
#define SDIO_SET_DSR                             ((uint8_t)4)
#define SDIO_SDIO_SEN_OP_COND                    ((uint8_t)5)
#define SDIO_HS_SWITCH                           ((uint8_t)6)
#define SDIO_SEL_DESEL_CARD                      ((uint8_t)7)
#define SDIO_HS_SEND_EXT_CSD                     ((uint8_t)8)
#define SDIO_SEND_CSD                            ((uint8_t)9)
#define SDIO_SEND_CID                            ((uint8_t)10)
#define SDIO_READ_DAT_UNTIL_STOP                 ((uint8_t)11) /* SD Card doesn't support it */
#define SDIO_STOP_TRANSMISSION                   ((uint8_t)12)
#define SDIO_SEND_STATUS                         ((uint8_t)13)
#define SDIO_HS_BUSTEST_READ                     ((uint8_t)14)
#define SDIO_GO_INACTIVE_STATE                   ((uint8_t)15)
#define SDIO_SET_BLOCKLEN                        ((uint8_t)16)
#define SDIO_READ_SINGLE_BLOCK                   ((uint8_t)17)
#define SDIO_READ_MULT_BLOCK                     ((uint8_t)18)
#define SDIO_HS_BUSTEST_WRITE                    ((uint8_t)19)
#define SDIO_WRITE_DAT_UNTIL_STOP                ((uint8_t)20) /* SD Card doesn't support it */
#define SDIO_SET_BLOCK_COUNT                     ((uint8_t)23) /* SD Card doesn't support it */
#define SDIO_WRITE_SINGLE_BLOCK                  ((uint8_t)24)
#define SDIO_WRITE_MULT_BLOCK                    ((uint8_t)25)
#define SDIO_PROG_CID                            ((uint8_t)26) /* reserved for manufacturers */
#define SDIO_PROG_CSD                            ((uint8_t)27)
#define SDIO_SET_WRITE_PROT                      ((uint8_t)28)
#define SDIO_CLR_WRITE_PROT                      ((uint8_t)29)
#define SDIO_SEND_WRITE_PROT                     ((uint8_t)30)
#define SDIO_SD_ERASE_GRP_START                  ((uint8_t)32) /* To set the address of the first write
                                                                  block to be erased. (For SD card only) */
#define SDIO_SD_ERASE_GRP_END                    ((uint8_t)33) /* To set the address of the last write block of the
                                                                  continuous range to be erased. (For SD card only) */
#define SDIO_ERASE_GRP_START                     ((uint8_t)35) /* To set the address of the first write block to be erased.
                                                                  (For MMC card only spec 3.31) */

#define SDIO_ERASE_GRP_END                       ((uint8_t)36) /* To set the address of the last write block of the
                                                                  continuous range to be erased. (For MMC card only spec 3.31) */

#define SDIO_ERASE                               ((uint8_t)38)
#define SDIO_FAST_IO                             ((uint8_t)39) /* SD Card doesn't support it */
#define SDIO_GO_IRQ_STATE                        ((uint8_t)40) /* SD Card doesn't support it */
#define SDIO_LOCK_UNLOCK                         ((uint8_t)42)
#define SDIO_APP_CMD                             ((uint8_t)55)
#define SDIO_GEN_CMD                             ((uint8_t)56)
#define SDIO_NO_CMD                              ((uint8_t)64)

/* Following commands are SD Card Specific commands.
   SDIO_APP_CMD should be sent before sending these
   commands. */
#define SDIO_APP_SD_SET_BUSWIDTH                 ((uint8_t)6)  /* For SD Card only */
#define SDIO_SD_APP_STAUS                        ((uint8_t)13) /* For SD Card only */
#define SDIO_SD_APP_SEND_NUM_WRITE_BLOCKS        ((uint8_t)22) /* For SD Card only */
#define SDIO_SD_APP_OP_COND                      ((uint8_t)41) /* For SD Card only */
#define SDIO_SD_APP_SET_CLR_CARD_DETECT          ((uint8_t)42) /* For SD Card only */
#define SDIO_SD_APP_SEND_SCR                     ((uint8_t)51) /* For SD Card only */
#define SDIO_SDIO_RW_DIRECT                      ((uint8_t)52) /* For SD I/O Card only */
#define SDIO_SDIO_RW_EXTENDED                    ((uint8_t)53) /* For SD I/O Card only */

/* Following commands are SD Card Specific security commands.
   SDIO_APP_CMD should be sent before sending these commands. */
#define SDIO_SD_APP_GET_MKB                      ((uint8_t)43) /* For SD Card only */
#define SDIO_SD_APP_GET_MID                      ((uint8_t)44) /* For SD Card only */
#define SDIO_SD_APP_SET_CER_RN1                  ((uint8_t)45) /* For SD Card only */
#define SDIO_SD_APP_GET_CER_RN2                  ((uint8_t)46) /* For SD Card only */
#define SDIO_SD_APP_SET_CER_RES2                 ((uint8_t)47) /* For SD Card only */
#define SDIO_SD_APP_GET_CER_RES1                 ((uint8_t)48) /* For SD Card only */
#define SDIO_SD_APP_SECURE_READ_MULTIPLE_BLOCK   ((uint8_t)18) /* For SD Card only */
#define SDIO_SD_APP_SECURE_WRITE_MULTIPLE_BLOCK  ((uint8_t)25) /* For SD Card only */
#define SDIO_SD_APP_SECURE_ERASE                 ((uint8_t)38) /* For SD Card only */
#define SDIO_SD_APP_CHANGE_SECURE_AREA           ((uint8_t)49) /* For SD Card only */
#define SDIO_SD_APP_SECURE_WRITE_MKB             ((uint8_t)48) /* For SD Card only */

/* Exported constants --------------------------------------------------------*/
#define SD_DMA_MODE                     ((uint32_t)0x00000000)
#define SD_INTERRUPT_MODE               ((uint32_t)0x00000001)
#define SD_POLLING_MODE                 ((uint32_t)0x00000002)

/* Supported Memory Cards */
#define SDIO_STD_CAPACITY_SD_CARD_V1_1     ((uint32_t)0x0)
#define SDIO_STD_CAPACITY_SD_CARD_V2_0     ((uint32_t)0x1)
#define SDIO_HIGH_CAPACITY_SD_CARD         ((uint32_t)0x2)
#define SDIO_MULTIMEDIA_CARD               ((uint32_t)0x3)
#define SDIO_SECURE_DIGITAL_IO_CARD        ((uint32_t)0x4)
#define SDIO_HIGH_SPEED_MULTIMEDIA_CARD    ((uint32_t)0x5)
#define SDIO_SECURE_DIGITAL_IO_COMBO_CARD  ((uint32_t)0x6)
#define SDIO_HIGH_CAPACITY_MMC_CARD        ((uint32_t)0x7)





/* Private typedef -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum {SDCard_ERROR = 0,SDCard_SUCCESS  = !SDCard_ERROR} SDCard_ErrorStatus;

typedef enum
{
  /* SDIO specific error defines */
  SD_CMD_CRC_FAIL                    = (1), /* Command response received (but CRC check failed) */
  SD_DATA_CRC_FAIL                   = (2), /* Data bock sent/received (CRC check Failed) */
  SD_CMD_RSP_TIMEOUT                 = (3), /* Command response timeout */
  SD_DATA_TIMEOUT                    = (4), /* Data time out */
  SD_TX_UNDERRUN                     = (5), /* Transmit FIFO under-run */
  SD_RX_OVERRUN                      = (6), /* Receive FIFO over-run */
  SD_START_BIT_ERR                   = (7), /* Start bit not detected on all data signals in widE bus mode */
  SD_CMD_OUT_OF_RANGE                = (8), /* CMD's argument was out of range.*/
  SD_ADDR_MISALIGNED                 = (9), /* Misaligned address */
  SD_BLOCK_LEN_ERR                   = (10), /* Transferred block length is not allowed for the card or the number of transferred bytes does not match the block length */
  SD_ERASE_SEQ_ERR                   = (11), /* An error in the sequence of erase command occurs.*/
  SD_BAD_ERASE_PARAM                 = (12), /* An Invalid selection for erase groups */
  SD_WRITE_PROT_VIOLATION            = (13), /* Attempt to program a write protect block */
  SD_LOCK_UNLOCK_FAILED              = (14), /* Sequence or password error has been detected in unlock command or if there was an attempt to access a locked card */
  SD_COM_CRC_FAILED                  = (15), /* CRC check of the previous command failed */
  SD_ILLEGAL_CMD                     = (16), /* Command is not legal for the card state */
  SD_CARD_ECC_FAILED                 = (17), /* Card internal ECC was applied but failed to correct the data */
  SD_CC_ERROR                        = (18), /* Internal card controller error */
  SD_GENERAL_UNKNOWN_ERROR           = (19), /* General or Unknown error */
  SD_STREAM_READ_UNDERRUN            = (20), /* The card could not sustain data transfer in stream read operation. */
  SD_STREAM_WRITE_OVERRUN            = (21), /* The card could not sustain data programming in stream mode */
  SD_CID_CSD_OVERWRITE               = (22), /* CID/CSD overwrite error */
  SD_WP_ERASE_SKIP                   = (23), /* only partial address space was erased */
  SD_CARD_ECC_DISABLED               = (24), /* Command has been executed without using internal ECC */
  SD_ERASE_RESET                     = (25), /* Erase sequence was cleared before executing because an out of erase sequence command was received */
  SD_AKE_SEQ_ERROR                   = (26), /* Error in sequence of authentication. */
  SD_INVALID_VOLTRANGE               = (27),
  SD_ADDR_OUT_OF_RANGE               = (28),
  SD_SWITCH_ERROR                    = (29),
  SD_SDIO_DISABLED                   = (30),
  SD_SDIO_FUNCTION_BUSY              = (31),
  SD_SDIO_FUNCTION_FAILED            = (32),
  SD_SDIO_UNKNOWN_FUNCTION           = (33),

  /* Standard error defines */
  SD_INTERNAL_ERROR, 
  SD_NOT_CONFIGURED,
  SD_REQUEST_PENDING, 
  SD_REQUEST_NOT_APPLICABLE, 
  SD_INVALID_PARAMETER,  
  SD_UNSUPPORTED_FEATURE,  
  SD_UNSUPPORTED_HW,  
  SD_ERROR,  
  SD_OK  
} SD_Error_TypeDef;

typedef enum
{
  SD_NO_TRANSFER  = 0,
  SD_TRANSFER_IN_PROGRESS
} SDTransferState_TypeDef;

typedef enum 	/*Card OCR*/
{
	SD_TYPE_ERR     =	0X00,	//读卡类型错误，无法识别
	SD_TYPE_MMC     =	0X01,	//MMC卡
	SD_TYPE_V1      =	0X02,	//V1.0SD卡
	SD_TYPE_V2      =	0X03,	//V2.0卡
	SD_TYPE_V2MC   	=	0X04,	//小容量V2.0SD卡
	SD_TYPE_V2HC    =	0X06	//大容量V2.0SD卡	
}SDCard_TYPE_TypeDef;

typedef enum 	/*Card R1*/
{
	In_Idle_State  						=	0X00,	//Bit0
	Erase_Reset     					=	0X01,	//Bit1
	Illegal_Command      			=	0X02,	//Bit2
	Communication_CRC_Error 	=	0X03,	//Bit3
	Erase_Sequence_Error			=	0X04,	//Bit4
	Address_Error    					=	0X06,	//Bit5
	Parameter_Error						=	0X06,	//Bit6
}SDCard_R1_TypeDef;
typedef enum 	/*Card R1*/
{
	SDCard_Ready  						=	0X00,	//Bit0
	SDCard_DMA_EN  						=	0X01,	//Bit0
	SDCard_DMA_DIS  					=	0X02,	//Bit0
	SDCard_Busy     					=	0X03,	//Bit1
}SDCard_Status_TypeDef;
//typedef enum 	/*Card R1b*/
//{
////	SDCard_R1_TypeDef	SDCard_R1;
//	Card_Is_Locked,
//}SDCard_R1b_TypeDef;

typedef enum 	/*Card OCR*/
{
	Reserved,
} SDCard_OCR_TypeDef;
//SD卡操作步骤
typedef enum 	/*Card OCR*/
{
	SDCard_DeInit						=	0X00,	//SD卡未初始化
	SDCard_StepReady				=	0X01,	//使SD卡进入准备状态，SD卡要求复位前至少发送74个clock
	SDCard_StepIDLE					=	0X02,	//进入IDLE状态
	SDCard_StepRESET0				=	0X03,	//复位SD卡命令0
	SDCard_StepRESET1				=	0X04,	//复位SD卡命令1
	SDCard_StepGetOCR				=	0X05,	//获取SD卡的OSR信息，包括容量和速度信息和操作电压R3返回类型
	SDCard_StepGetCSD				=	0X06,	//获取SD卡的CSD信息，包括容量和速度信息
	SDCard_StepGetCID				=	0X07,	//获取SD卡的CID信息，包括制造商信息	
} SDCard_Step_TypeDef;
typedef struct
{
  uint16_t TransferredBytes;
  SD_Error_TypeDef TransferError;
  uint8_t  padding;
} SDLastTransferInfo_TypeDef;

typedef struct      /*Card Identification Data*/
{
  __IO uint8_t  ManufacturerID;       /* ManufacturerID */					//8b	厂商标识
  __IO uint16_t OEM_AppliID;          /* OEM/Application ID */			//16b
//  __IO uint32_t ProdName1;            /* Product Name part1 */			//32b
//  __IO uint8_t  ProdName2;            /* Product Name part2*/				//8b
  __IO uint8_t  ProdRev;              /* Product Revision */				//8b
  __IO uint32_t ProdSN;               /* Product Serial Number */		//32b
  __IO uint8_t  Reserved1;            /* Reserved1 */								//4b
  __IO uint16_t ManufactDate;         /* Manufacturing Date */			//12b
  __IO uint8_t  CID_CRC;              /* CID CRC */									//7b
  __IO uint8_t  Reserved2;            /* always 1 */								//1b
	__IO uint8_t  ProdName[5];						//产品名称
} SDCard_CID_TypeDef;

typedef struct       /* Card Specific Data */	//R2
{
  __IO uint8_t  CSDStruct;            /* CSD structure V1.0-00,V2.0-01*/						//CSD结构										2b	V1.0-00				V2.0-01
//  __IO uint8_t  SysSpecVersion;       /* System specification version */						//系统规范版本							//Reserved1
  __IO uint8_t  Reserved1;            /* Reserved */																//保留											6b	000000
  __IO uint8_t  TAAC;                 /* Data read access-time 1 */									//读取时间1									8b	xxh
  __IO uint8_t  NSAC;                 /* Data read access-time 2 in CLK cycles */		//数据在CLK周期内读取时间2		8b	xxh
  __IO uint8_t  MaxBusClkFrec;        /* Max. bus clock frequency */								//最大总线速度								8b	32h or 5Ah				
  __IO uint16_t CardComdClasses;      /* Card command classes */										//卡命令集合									12b	01x110110101b
  __IO uint8_t  RdBlockLen;           /* Max. read data block length */							//最大读取数据块长						4b	xh
  __IO uint8_t  PartBlockRead;        /* Partial blocks for read allowed */					//允许读的部分块							1b	1b	
  __IO uint8_t  WrBlockMisalign;      /* Write block misalignment */								//非线写块										1b	xb
  __IO uint8_t  RdBlockMisalign;      /* Read block misalignment */									//非线读块										1b	xb
  __IO uint8_t  DSRImpl;              /* DSR implemented */													//DSR条件										1b	xb
  __IO uint8_t  Reserved2;            /* Reserved */																//保留											2b	00b
  __IO uint32_t DeviceSize;           /* Device Size */															//设备容量										12b	xxxh
  __IO uint8_t  MaxRdCurrentVDDMin;   /* Max. read current @ VDD min */							// 最小读取电流 @ VDD min		3b	xxxb
  __IO uint8_t  MaxRdCurrentVDDMax;   /* Max. read current @ VDD max */							// 最大读取电流 @ VDD max		3b	xxxb
  __IO uint8_t  MaxWrCurrentVDDMin;   /* Max. write current @ VDD min */						// 最小写入电流 @ VDD min		3b	xxxb
  __IO uint8_t  MaxWrCurrentVDDMax;   /* Max. write current @ VDD max */						// 最大写入电流 @ VDD max		3b	xxxb
  __IO uint8_t  DeviceSizeMul;        /* Device size multiplier */									// 设备容量乘积因子					3b	xxxb
  __IO uint8_t  EraseGrSize;          /* Erase group size */												// 擦除块大小								1b	xb
  __IO uint8_t  EraseGrMul;           /* Erase group size multiplier */							// 擦除扇区大小							7b	xxxxxxxb
  __IO uint8_t  WrProtectGrSize;      /* Write protect group size */								// 写保护群大小							7b	xxxxxxxb
  __IO uint8_t  WrProtectGrEnable;    /* Write protect group enable */							// 写保护群使能							1b	xb
  __IO uint8_t  ManDeflECC;           /* Manufacturer default ECC */								// 厂家默认ECC 							2b	00b(保留)
  __IO uint8_t  WrSpeedFact;          /* Write speed factor */											// 写速度因子								3b	xxxb
  __IO uint8_t  MaxWrBlockLen;        /* Max. write data block length */						// 最大写数据块长度					4b	xxxxb
  __IO uint8_t  WriteBlockPaPartial;  /* Partial blocks for write allowed */				// 允许写的部分							1b	xb
  __IO uint8_t  Reserved3;            /* Reserded */																// 保留											5b	00000b
  __IO uint8_t  ContentProtectAppli;  /* Content protection application */					// 内容保护中的应用 					//Reserved3
  __IO uint8_t  FileFormatGrouop;     /* File format group */												// 文件系统群								1b	xb
  __IO uint8_t  CopyFlag;             /* Copy flag (OTP) */													// 拷贝标志									1b	xb
  __IO uint8_t  PermWrProtect;        /* Permanent write protection */							// 永久写保护								1b	xb
  __IO uint8_t  TempWrProtect;        /* Temporary write protection */							// 暂时写保护								1b	xb
  __IO uint8_t  FileFormat;           /* File Format */															// 文件系统									2b	xxb
  __IO uint8_t  ECC;                  /* ECC code */																// ECC code									2b	00b(保留)
  __IO uint8_t  CSD_CRC;              /* CSD CRC */																	// CSD CRC									7b	xxxxxxxb
  __IO uint8_t  Reserved4;            /* always 1*/																	// 结束标志，始终为1					1b	1b
} SDCard_CSD_TypeDefbac;

typedef struct       /* Card Specific Data */	//R2	V2.0
{
  __IO uint8_t  CSD_STRUCTURE;      //	2b	V1.0-00				V2.0-01 	// CSD structure V1.0-00,V2.0-01//CSD结构
  __IO uint8_t  Reserved1;          //	6b	000000b									//Reserved
  __IO uint8_t  TAAC;               //	8b	0Eh											//data read access-time
  __IO uint8_t  NSAC;               //	8b	00h											//data read access-time in CLK cycles (NSAC*100)
  __IO uint8_t  TRAN_SPEED;        	//	8b	32h or 5Ah							//max. data transfer rate			
  __IO uint16_t CCC;     						//	12b	01x110110101b						//card command classes
  __IO uint8_t  READ_BL_LEN;        //	4b	9												//max. read data block length
  __IO uint8_t  READ_BL_PARTIAL;    //	1b	0												//partial blocks for read allowed
  __IO uint8_t  WRITE_BLK_MISALIGN; //	1b	0												//write block misalignment
  __IO uint8_t  READ_BLK_MISALIGN;  //	1b	0												//read block misalignment
  __IO uint8_t  DSR_IMP;            //	1b	x												//DSR implemented
  __IO uint8_t  Reserved2;          //	6b	000000b									//reserved
  __IO uint32_t C_SIZE;          		//	22b	00xxxxh									//device size
	__IO uint8_t  Reserved3;          //	1b	-												//reserved
  __IO uint8_t  ERASE_BLK_EN;   		//	1b	1												//erase single block enable
  __IO uint8_t  SECTOR_SIZE;   			//	7b	7Fh											//erase sector size
  __IO uint8_t  WP_GRP_SIZE;   			//	7b	0000000b								//write protect group size
  __IO uint8_t  WP_GRP_ENABLE;   		//	1b	0												//write protect group enable
	__IO uint8_t  Reserved4;					//	2b	00b											//reserved
  __IO uint8_t  R2W_FACTOR;        	//	3b	010b										//write speed factor
  __IO uint8_t  WRITE_BL_LEN;     	//	4b	9												//max. write data block length
  __IO uint8_t  WRITE_BL_PARTIAL; 	//	1b	0												//partial blocks for write allowed
	__IO uint8_t  Reserved5;					//	5b	00000b									//reserved
  __IO uint8_t  FILE_FORMAT_GRP;   	//	1b	0												//File format group
  __IO uint8_t  COPY;    						//	1b	x												//copy flag (OTP)
  __IO uint8_t  PERM_WRITE_PROTECT;	//	1b	x												//permanent write protection
  __IO uint8_t  TMP_WRITE_PROTECT; 	//	1b	x												//temporary write protection
  __IO uint8_t  FILE_FORMAT;        //	2b	00b											//File format
	__IO uint8_t  Reserved6;					//	2b	00b											//reserved
  __IO uint8_t  CSD_CRC;          	//	7b	xxxxxxxb								//CRC
  __IO uint8_t  Reserved7;        	//	1b	1												//not used, always’1’
} SDCard_CSD_TypeDef;

typedef struct			//SD卡信息结构体
{
  SDCard_CSD_TypeDef		SDCard_CSD;
  SDCard_CID_TypeDef		SDCard_CID;
	SDCard_TYPE_TypeDef		SDCard_TYPE;
	SD_Error_TypeDef			SD_Error;
//	SDCard_Status_TypeDef	SDCard_Status;
  uint32_t CardCapacity; 		/* Card Capacity */	//SD卡容量，单位MB
  uint32_t CardBlockSize; 	/* Card Block Size */
  uint16_t RCA;
  uint8_t Status;
} SDCard_CardInfo_TypeDef;

//////////////////////////////////////////////////////////////////////







/* Exported macro ------------------------------------------------------------*/
#define SDCard_BufferSize	512
extern SDCard_Status_TypeDef	SDCard_Status;
extern SDCard_Step_TypeDef	SDCard_Step;
extern unsigned char SD_TX_Addr[SDCard_BufferSize];
extern unsigned char SD_RX_Addr[SDCard_BufferSize];
/* Exported functions ------------------------------------------------------- */


SDCard_ErrorStatus			SDCard_Configuration(SDCard_CardInfo_TypeDef *SDCard_CardInfo);																								//SD卡项目配置	
SDCard_ErrorStatus			SDCard_Server(void);																											//SD卡项目执行(服务)程序
void										SDCard_GPIO_Configuration(void);																					//SPI管脚配置
void 										SDCard_SPI_Configuration(char Speed);																			//SPI配置
void 										SDCard_DMA_Configuration(u32 *SD_TX_Addr,u32 *SD_RX_Addr,u32 BufferSize);	//DMA配置

SDCard_ErrorStatus			SDCard_Initialize(SDCard_CardInfo_TypeDef *SDCard_CardInfo);							//SDCard初始化
void										SDCard_EnterReady(void);																									//SD卡进入准备状态
//SDCard_ErrorStatus			SDCard_WaitReady(void);																									//等待卡准备好
SDCard_ErrorStatus			SDCard_EnterIDLE(SDCard_CardInfo_TypeDef *SDCard_CardInfo);								//SD卡进入IDLE
SDCard_ErrorStatus			SDCard_GetCID(SDCard_CardInfo_TypeDef *SDCard_CardInfo);									//获取SD卡的CID信息，包括制造商信息
SDCard_ErrorStatus			SDCard_GetCSD(SDCard_CardInfo_TypeDef *SDCard_CardInfo);									//获取SD卡的CSD信息，包括容量和速度信息
SDCard_ErrorStatus			SDCard_GetOCR(SDCard_CardInfo_TypeDef *SDCard_CardInfo);									//获取SD卡的OSR信息，包括容量和速度信息和操作电压R3返回类型
SDCard_ErrorStatus			SDCard_Identification(SDCard_CardInfo_TypeDef *SDCard_CardInfo);					//SD卡识别

void										SDCard_Select(void);																											//使能sd卡,并且等待卡准备OK
void										SDCard_DisSelect(void);																										//取消片选,释放SPI总线
void										SDCard_DMASend(unsigned int BufferSize);																	//DMA方式向SD写入数据，开启DMA发送和接收 
unsigned char 					SDCard_SendCmd(unsigned char cmd, unsigned int arg,unsigned char crc);		//向SD卡发送一个命令
SDCard_ErrorStatus			SDCard_GetResponse(unsigned char Response);																//等待SD卡回应指定值
SDCard_ErrorStatus			SDCard_RecvData(unsigned char *Rxbuf,unsigned int len);										//从sd卡读取一个数据包的内容
unsigned char						SDCard_ReadWriteByte(u8 TxData);																					//读写一个字节
SDCard_ErrorStatus			SDCard_ReadSingleBlock(u32 addr,unsigned char *Rxbuf, u16 BlockSize);			//读SD卡的一个block 



	


//SD_Error SD_Init(void);
//SD_Error SD_PowerON(void);
//SD_Error SD_PowerOFF(void);
//SD_Error SD_InitializeCards(void);
//SD_Error SD_GetCardInfo(SD_CardInfo *cardinfo);
//SD_Error SD_EnableWideBusOperation(uint32_t WideMode);
//SD_Error SD_SetDeviceMode(uint32_t Mode);
//SD_Error SD_SelectDeselect(uint32_t addr);
//SD_Error SD_ReadBlock(uint32_t addr, uint32_t *readbuff, uint16_t BlockSize);
//SD_Error SD_ReadMultiBlocks(uint32_t addr, uint32_t *readbuff, uint16_t BlockSize, uint32_t NumberOfBlocks);
//SD_Error SD_WriteBlock(uint32_t addr, uint32_t *writebuff, uint16_t BlockSize);
//SD_Error SD_WriteMultiBlocks(uint32_t addr, uint32_t *writebuff, uint16_t BlockSize, uint32_t NumberOfBlocks);
//SDTransferState SD_GetTransferState(void);
//SD_Error SD_StopTransfer(void);
//SD_Error SD_Erase(uint32_t startaddr, uint32_t endaddr);
//SD_Error SD_SendStatus(uint32_t *pcardstatus);
//SD_Error SD_SendSDStatus(uint32_t *psdstatus);
//SD_Error SD_ProcessIRQSrc(void);

#endif

/*******************	wegam@sina.com	*******************/
/*********************	2017/01/15	*********************/
/**********************	END OF FILE	*********************/


