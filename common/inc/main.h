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
#include <stdio.h>
#include <string.h>
#include <math.h>

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* Private defines -----------------------------------------------------------*/
#define GREEN_Pin GPIO_PIN_12
#define GREEN_GPIO_Port GPIOD
#define ORANGE_Pin GPIO_PIN_13
#define ORANGE_GPIO_Port GPIOD
#define RED_Pin GPIO_PIN_14
#define RED_GPIO_Port GPIOD
#define BLUE_Pin GPIO_PIN_15
#define BLUE_GPIO_Port GPIOD

#define BOOTLOADER          ((uint32_t)0x08000000U)
#define LOADER_ADDR         ((uint32_t)0x08004000U)
#define UPDATER_ADDR        ((uint32_t)0x08010000U)
#define APP_ADDR            ((uint32_t)0x08020000U)
#define BACKUP_ADDR         ((uint32_t)0x08080000U)
#define PATCH_ADDR          ((uint32_t)0x080C0000U)
#define IMAGE_HDR_SIZE      0x200

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
