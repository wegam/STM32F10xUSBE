/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : i2s_codec.h
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : This file contains all the functions prototypes for the
*                      I2S codec firmware driver.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2S_CODEC_H
#define __I2S_CODEC_H

/* .WAV file format :

 Endian      Offset      Length      Contents
  big         0           4 bytes     'RIFF'             // 0x52494646
  little      4           4 bytes     <file length - 8>
  big         8           4 bytes     'WAVE'             // 0x57415645

Next, the fmt chunk describes the sample format:

  big         12          4 bytes     'fmt '          // 0x666D7420
  little      16          4 bytes     0x00000010      // Length of the fmt data (16 bytes)
  little      20          2 bytes     0x0001          // Format tag: 1 = PCM
  little      22          2 bytes     <channels>      // Channels: 1 = mono, 2 = stereo
  little      24          4 bytes     <sample rate>   // Samples per second: e.g., 22050
  little      28          4 bytes     <bytes/second>  // sample rate * block align
  little      32          2 bytes     <block align>   // channels * bits/sample / 8
  little      34          2 bytes     <bits/sample>   // 8 or 16

Finally, the data chunk contains the sample data:

  big         36          4 bytes     'data'        // 0x64617461
  little      40          4 bytes     <length of the data block>
  little      44          *           <sample data>

*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"

/* Exported types ------------------------------------------------------------*/

/* Audio file information structure */
typedef struct
{
  u32  RIFFchunksize;
  u16  FormatTag;
  u16  NumChannels;
  u32  SampleRate;
  u32  ByteRate;
  u16  BlockAlign;
  u16  BitsPerSample;
  u32  DataSize;
} WAVE_FormatTypeDef;

/* Error Identification structure */
typedef enum
{
  Valid_WAVE_File = 0,
  Unvalid_RIFF_ID,
  Unvalid_WAVE_Format,
  Unvalid_FormatChunk_ID,
  Unsupporetd_FormatTag,
  Unsupporetd_Number_Of_Channel,
  Unsupporetd_Sample_Rate,
  Unsupporetd_Bits_Per_Sample,
  Unvalid_DataChunk_ID,
  Unsupporetd_ExtraFormatBytes,
  Unvalid_FactChunk_ID
} ErrorCode;
 
/* Exported constants --------------------------------------------------------*/

/* Codec output DEVICE */
#define OutputDevice_SPEAKER          1
#define OutputDevice_HEADPHONE        2  
#define OutputDevice_BOTH             3

/* VOLUME control constants */
#define DEFAULT_VOL                   0x52
#define VolumeDirection_HIGH          0xF
#define VolumeDirection_LOW           0xA
#define VolumeDirection_LEVEL         0x0
#define VOLStep                       4 

/* Forward and Rewind constants */
#define STEP_FORWARD                  2 /* 2% of wave file data length*/
#define STEP_BACK                     6 /* 6% of wave file data length*/

/* Codec POWER DOWN modes */
#define CodecPowerDown_HW             1
#define CodecPowerDown_SW 	          2

/* Audio Play STATUS */
#define AudioPlayStatus_STOPPED       0
#define AudioPlayStatus_PLAYING	      1
#define AudioPlayStatus_PAUSED		    2	 

/* MUTE commands */ 
#define MUTE_ON                       1
#define MUTE_OFF							        0

/* I2S configuration parameters */
#define I2S_STANDARD                  I2S_Standard_Phillips
#define I2S_MCLKOUTPUT                I2S_MCLKOutput_Enable

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
 
  /* CONFIGURATION */
u32 I2S_CODEC_Init(u32 OutputDevice, u32 Address);
void I2S_CODEC_ReplayConfig(u32 Repetions);

	/* AUDIO CONTROL */
u32 I2S_CODEC_Play(u32 AudioStartPosition);
u32 I2S_CODEC_Pause(void);
u32 I2S_CODEC_Stop(void);	
u32 I2S_CODEC_ControlVolume(u32 direction, u8 volume);
void I2S_CODEC_Mute(u32 Command);
void I2S_CODEC_ForwardPlay(u32 Step);
void I2S_CODEC_RewindPlay(u32 Step);

   /* EXTRA CONTROLS */								 
void I2S_CODEC_PowerDown(u32 CodecPowerDown_Mode);
void I2S_CODEC_Reset(void);
u32 I2S_CODEC_SpeakerHeadphoneSwap(u32 CODEC_AudioOutput, u32 Address);
u8 GetVar_CurrentVolume(void);

/* Medium Layer Codec Functions ----------------------------------------------*/						
void I2S_CODEC_DataTransfer(void);
void I2S_CODEC_UpdateStatus(void);
u32 GetVar_DataStartAddr(void);	
ErrorCode I2S_CODEC_WaveParsing(u8* HeaderTab);
u32 GetVar_CurrentOutputDevice(void);

u32 GetVar_AudioDataIndex(void);
void SetVar_AudioDataIndex(u32 value);
void ResetVar_AudioDataIndex(void);
void IncrementVar_AudioDataIndex(u32 IncrementNumber);
void DecrementVar_AudioDataIndex(u32 DecrementNumber);

u32 GetVar_AudioReplay(void);
void Decrement_AudioReplay(void);

u32 GetVar_AudioPlayStatus(void);
u32 SetVar_AudioPlayStatus(u32 Status);

u16 GetVar_i2saudiofreq(void);
u32 GetVar_AudioDataLength(void);

/* Low Layer Codec Fuctions --------------------------------------------------*/
u32 SetVar_SendDummyData(void);
u32 ResetVar_SendDummyData(void);
u32 GetVar_SendDummyData(void);
u32 AudioFile_Init(void);

void I2S_GPIO_Config(void); 
void I2S_Config(u16 Standard, u16 MCLKOutput, u16 AudioFreq);
u32 CODEC_Config(u16 AudioOutput, u16 I2S_Standard, u16 I2S_MCLKOutput, u8 Volume);

u32 Media_Init(void);
void Media_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
void Media_StartReadSequence(u32 ReadAddr);
u16 Media_ReadHalfWord(u32 Offset);
u8 Media_ReadByte(u32 Offset);

void delay(vu32 nCount); 

#endif /* __I2S_CODEC_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
