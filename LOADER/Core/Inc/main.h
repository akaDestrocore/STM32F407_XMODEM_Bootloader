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
#include <ring_buffer.h>
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
	MSG_WELCOME 					= 0x0U,
	MSG_SELECT_OPTION 				= 0x1U,
	MSG_NO_SUCH_KEY					= 0x2U,
	MSG_GOTO_UPDATER				= 0x3U,
	MSG_GOTO_APP					= 0x4U
}MESSAGE_t;
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
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
