#ifndef _FLASH_H
#define _FLASH_H

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stddef.h>

// Core flash functions
int flash_unlock(void);
void flash_lock(void);
int flash_wait_for_last_operation(void);
uint8_t flash_get_sector(uint32_t address);
int flash_erase_sector(uint32_t sector_addr);
int flash_erase(uint32_t destination);
int flash_write(uint32_t address, const uint8_t* data, size_t len);
void flash_read(uint32_t address, uint8_t* data, size_t len);

// Get sector information
uint32_t flash_get_sector_start(uint8_t sector);
uint32_t flash_get_sector_end(uint8_t sector);

// Write across sector boundaries
int flash_write_across_sectors(uint32_t current_addr, uint8_t current_sector,
                              const uint8_t* data, size_t data_len,
                              uint32_t* new_addr, uint8_t* new_sector);

#endif /* _FLASH_H */