/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <ring_buffer.h>
#include <xmodem.h>
#include <updater_flash.h>
#include <janpatch.h>
#include <mbedtls.h>
#include <mbedtls/gcm.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
/*
 * Macros for ShowMessage()
 */
typedef enum
{
	MSG_JUMP_TO_LOADER				= 0x0U,
	MSG_SELECT_OPTION 				= 0x1U,
	MSG_NO_SUCH_KEY					= 0x2U,
	MSG_UPDT_IN_PRGS 				= 0x3U,
	MSG_ASK_FOR_A_FILE 				= 0x4U,
	MSG_CRC_ERR 					= 0x5U,
	MSG_FLASH_ERR_WRITE 			= 0x6U,
	MSG_INDEX1_ERR					= 0x7U,
	MSG_INDEX2_ERR					= 0x8U,
	MSG_ALREADY_NEWER				= 0x9U,
	MSG_FILE_RDY_2_SEND				= 0x10U,
	MSG_SLCT_UPDT_METHOD			= 0x11U
}MESSAGE_t;

/*
 * Global states of updater
 */
typedef enum
{
	UPDATER_STATE_UPLOAD 		= 0x1,
	UPDATER_STATE_DOWNLOAD 		= 0x2,
	UPDATER_STATE_LOAD_BKP		= 0x3,
	UPDATER_STATE_UPDATE_FULL	= 0x4,
	UPDATER_STATE_UPDATE_PATCH 	= 0x5
}UPDATER_STATE_t;


/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
// bootloader options selection from terminal
void Read_Key(void);
// wrapper function to print messages to terminal via UART
void ShowMessage(MESSAGE_t msg);
// firmware update state of bootloader
void Download_Firmware(uint8_t * pUploadAddress);
// helper functions to decrypt data while downloading it to MCU
void X_SendCan(void);
void prepare_header(void);
void decrypt_rest_of_firmware(void);
uint32_t extract_size(uint8_t *file);
// returns current firmware version which is at FIRMWARE_VERSION_ADDRESS
void get_firmware_current_version(uint8_t *major, uint8_t *minor, uint8_t *patch);
// compare current version with the uploaded one
uint8_t is_newer_version(uint8_t *major, uint8_t *minor, uint8_t *patch);
// copy new firmware from temporary location to current location
void UpdateUserApplication(void);
// apply patch to already installed application
void ApplyPatch(void);
// select update method
void SelectUpdateMethod(void);
// Load backup data to SLOT_2_VER_ADDR
void load_backup(void);
//firmware download related APIs
void DownloadUserApplication(void);
void gcm_begin_encryption(uint8_t* src, size_t len);
void X_SendPacket(uint8_t* packet);
//Jump to app loader
void JumpToAppLoader(void);

//serial terminal related API
void clear_screen(void);
void set_cursor_position(int x, int y);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define GREEN_Pin GPIO_PIN_12
#define GREEN_GPIO_Port GPIOD
#define ORANGE_Pin GPIO_PIN_13
#define ORANGE_GPIO_Port GPIOD
#define RED_Pin GPIO_PIN_14
#define RED_GPIO_Port GPIOD
#define BLUE_Pin GPIO_PIN_15
#define BLUE_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */
#define BOOTLOADER					((uint32_t)0x08000000U)				//FLASH_SECTOR_0_ADDR
#define SLOT_1_APP_LOADER_ADDR 		((uint32_t)0x08004000U)				//FLASH_SECTOR_1_ADDR
#define UPDATER_ADDR				((uint32_t)0x08008000U)				//FLASH_SECTOR_2_ADDR
#define SLOT_2_APP_ADDR				((uint32_t)0x08040000U)				//FLASH_SECTOR_6_ADDR
#define SLOT_2_VER_ADDR				((uint32_t)(SLOT_2_APP_ADDR - 8U))
#define BACKUP_ADDR					((uint32_t)0x08080000U)				//FLASH_SECTOR_8_ADDR
#define PATCH_ADDR					((uint32_t)0x080C0000U)				//FLASH_SECTOR_10_ADDR
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
