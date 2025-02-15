

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
#ifndef P_BOOT
  #include "ring_buffer.h"
#endif
#include <string.h>
#include <math.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

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
#define BOOTLOADER					((uint32_t)0x08000000U)				
#define SLOT_1_APP_LOADER_ADDR 		((uint32_t)0x08004000U)				
#define UPDATER_ADDR				((uint32_t)0x08008000U)				
#define SLOT_2_APP_ADDR				((uint32_t)0x08020200U)				
#define SLOT_2_VER_ADDR				((uint32_t)0x08020000U)
#define BACKUP_ADDR					((uint32_t)0x08060000U)				
#define PATCH_ADDR					((uint32_t)0x080C0000U)


typedef enum
{
	PRINT_HELLO,
	PRINT_OPTIONS,
	PRINT_SELECTION_ERR,
	PRINT_BOOT_UPDATER,
	PRINT_BOOT_FIRMWARE,
  PRINT_BOOT_LOADER,
  PRINT_WAIT_UPDATE,
  PRINT_WAIT_FILE,
  PRINT_CRC_ERR,
  PRINT_WRONG_DATA_LEN,
  PRINT_IDX1_ERR,
  PRINT_IDX2_ERR,
  PRINT_ALREADY_NEWER,
  PRINT_RDY_2_SEND,
  PRINT_UPDT_SEL
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

typedef enum
{
	FRAME0,
	FRAME1,
	FRAME2,
	FRAME3,
	FRAME4
}ANIMATION_t;

#define USER_BTN_Pin GPIO_PIN_0
#define USER_BTN_GPIO_Port GPIOA
#define GREEN_Pin GPIO_PIN_12
#define GREEN_GPIO_Port GPIOD
#define ORANGE_Pin GPIO_PIN_13
#define ORANGE_GPIO_Port GPIOD
#define RED_Pin GPIO_PIN_14
#define RED_GPIO_Port GPIOD
#define BLUE_Pin GPIO_PIN_15
#define BLUE_GPIO_Port GPIOD
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
