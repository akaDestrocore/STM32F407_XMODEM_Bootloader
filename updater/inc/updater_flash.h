#ifndef UPDATER_FLASH_H_
#define UPDATER_FLASH_H_

#include <stdint.h>
#include <string.h>
#include <stm32f4xx_hal.h>

/* Private functions ---------------------------------------------------------*/
/** @defgroup  FLASH Private Functions
  * @{
  */
uint32_t FLASH_EraseSector(uint32_t destination);
void FLASH_Erase(uint32_t destination);
uint8_t FLASH_Write(uint8_t *sourcedata, uint32_t len, uint32_t destination);
void FLASH_Read(uint32_t source, uint8_t *destination, uint32_t len);

#endif /* UPDATER_FLASH_H_ */
