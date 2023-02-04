#ifndef BASIC_FLASH_LIB_H_
#define BASIC_FLASH_LIB_H_

/* Includes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "stm32f407xx.h"
#include "led_init.h"


/* Private defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define	 FLASH_KEY1						0x45670123
#define	 FLASH_KEY2						0xCDEF89AB
#define	 FLASH_OPT_KEY1				0x08192A3BU
#define	 FLASH_OPT_KEY2				0x4C5D6E7F
#define FLASH_APP_START_ADDRESS		((uint32_t)0x08008000U)
#define FLASH_SECTOR_TOTAL			12U

typedef enum{
	DATA_TYPE_8b = 0,
	DATA_TYPE_16b,
	DATA_TYPE_32b,
	DATA_TYPE_64b
}BFL_data_t;

/* Function prototypes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void BFL_FLASH_Unlock(void);
void BFL_FLASH_Lock(void);
void BFL_FLASH_Init(BFL_data_t blocksize);
uint32_t BFL_EraseSector(uint32_t destination);
void BFL_FLASH_Erase(uint32_t destination);
uint32_t BFL_FLASH_Write(uint8_t *sourcedata, uint32_t len, uint32_t destination);
void BFL_FLASH_Read(uint32_t source, uint8_t *destination, uint32_t len);
void DeInit(void);
void BFL_Jump(void);


#endif /* BASIC_FLASH_LIB_H_ */
