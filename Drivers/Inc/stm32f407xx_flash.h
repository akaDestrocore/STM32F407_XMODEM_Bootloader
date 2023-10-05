/*												@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@@@@@@@
 @file     stm32f407xx_flash.h					@@@@@@@@@@@@@@@@@  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 @brief    MC specific header file for		 	@@@@@@@@@@@@@@@   @@@@@@@     @   @@@@@@@@@@@@@@
  		   STM32F407VGT6 based Discovery board	@@@@@@@@@@@@@     @@@@@@@@  @@@@@@@@@@@@@@@@@@@@
 @author   destrocore							@@@@@@@@@@@@ @@@  (@@@@@@  @@@@@@@@@@@@@@@@@@@@@
 @version  V1.0									@@@@@@@@@@@@@@@@   @@@@/  @@@@@@@&   &@@.  @@@@@
												@@@@@@@@@@@@@@@@   @@@&  @@@@@     @@@@@@@@ @@@@
This file provides firmware functions to manage @@@@@@@@@@@@@@@@@   @   @@@.    &@@@@@@@@@@@@@@@
the following functionalities of the FLASH	 	@@@@@@@@@@@@@@@@@             @@@         %   @@
Module:											@@@@@@@@@@@@@@@@@   @@@@@          @@@@@@@@@@@ @
+ Initialization and de-initialization function @@@@@@@@@@@@@@@@@@@@@@@.%@  @@@@@  @@@@@@@@@@@@@
+ Module Control functions						@@@@@@@@@@@@@@@@@@              @@@@@@@@@@@@@@@@
												@ @@@@@@@@@@@@@@                  @@@@@@@@@@@@@@
												@@  @@@@@@@@@                  @@@@@@@@@@@@@@@@@
												@@@@  @@@    @@@@@@@&         .@@@@@@@@@@@@@@@@@
												@@@@@@@#   ###@@@@( @        &@@@@@@@@@@@@@@@@@@
												@@@@@@@@@@@@@@@#     @@     (@     @@@@@@@@@@@@@
												@@@@@@@@@@@@@@     @@@@     @@     @@@@@@@@@@@@@
												@@@@@@@@@@@&     @@@@@@/   @@@@@@    @@@@@@@@@@@
												@@@@@@@@@@@*    @@@@@@@@  @@@@@@@@      @@@@@@@@
												@@@@@@@@@@@      @@@@@@@  @@@@@@@@   %  @@@@@@@@
												@@@@@@@@@@@@       /&@@@  @@@@@@&   @ @@@@@@@@@@
												@@@@@@@@@@@@@@&  ,@@@@@@@@@@@@  @ @@@@@@@@@@@@@@
												@@@@@@@@@@@@@@@@@@  @@@@@@@@@@@%@@@@@@@@@@@@@@@@													*/
#ifndef __STM32F407XX_FLASH_H_
#define __STM32F407XX_FLASH_H_


#include <stm32f407xx.h>


#define FLASH_APP_START_ADDRESS		((uint32_t)0x08008000U)
#define FLASH_SECTOR_TOTAL			12U

/*
 * FLASH keys
 */
typedef enum
{
	FLASH_KEY1 = 0x45670123U,
	FLASH_KEY2 = 0xCDEF89ABU,
	FLASH_OPT_KEY1 = 0x08192A3BU,
	FLASH_OPT_KEY2 = 0x4C5D6E7FU
}FLASH_KEY_t;

/*
 * FLASH data types
 */
typedef enum{
	DATA_TYPE_8B  = 0x1U,
	DATA_TYPE_16B = 0x2U,
	DATA_TYPE_32B = 0x4U,
	DATA_TYPE_64B = 0x8U
}FLASH_DATA_t;

/*
 * FLASH latency wait states
 */
typedef enum
{
	FLASH_ACR_LATENCY_0WS = 0x0U,
	FLASH_ACR_LATENCY_1WS = 0x1U,
	FLASH_ACR_LATENCY_2WS = 0x2U,
	FLASH_ACR_LATENCY_3WS = 0x3U,
	FLASH_ACR_LATENCY_4WS = 0x4U,
	FLASH_ACR_LATENCY_5WS = 0x5U,
	FLASH_ACR_LATENCY_6WS = 0x6U,
	FLASH_ACR_LATENCY_7WS = 0x7U
}FLASH_ACR_LATENCY_t;

/********************************************************************************************/
/*								APIs supported by this driver								*/
/*		 For more information about the APIs check the function definitions					*/
/********************************************************************************************/
/*
 * Initialization and de-initialization
 */
void 		FLASH_Init(FLASH_DATA_t blocksize);
void 		DeInit(void);
/*
 * Lock/Unlock functions
 */
void 		FLASH_Unlock(void);
void 		FLASH_Lock(void);
/*
 * Data manipulation functions
 */
uint32_t 	FLASH_EraseSector(uint32_t destination);
void		FLASH_Erase(uint32_t destination);
void	 	FLASH_Write(uint8_t *sourcedata, uint32_t len, uint32_t destination);
void 		FLASH_Read(uint32_t source, uint8_t *destination, uint32_t len);
/*
 * Jump to application
 */
void 		FLASH_Jump(void);

#endif /* __STM32F407XX_FLASH_H_ */
