#include "flash.h"
#include <string.h>

static const uint32_t FLASH_SECTORS_KB[] = {
    16,  // sector 0
    16,  // sector 1
    16,  // sector 2
    16,  // sector 3
    64,  // sector 4
    128, // sector 5
    128, // sector 6
    128, // sector 7
    128, // sector 8
    128, // sector 9
    128, // sector 10
    128  // sector 11
};

static const uint8_t FLASH_SECTOR_COUNT = sizeof(FLASH_SECTORS_KB) / sizeof(FLASH_SECTORS_KB[0]);

/**
 * @brief Unlocks the Flash memory for write/erase operations.
 * @return int Returns 1 if the Flash is successfully unlocked or already unlocked, 0 otherwise.
 */
int flash_unlock(void) {
    // Check if already unlocked
    if ((FLASH->CR & FLASH_CR_LOCK) == 0) {
        return 1;
    }
    
    HAL_StatusTypeDef status = HAL_FLASH_Unlock();
    return (status == HAL_OK) ? 1 : 0;
}

/**
 * @brief Locks the flash memory to prevent further write/erase operations.
 */
void flash_lock(void) {
    HAL_FLASH_Lock();
}

/**
 * @brief Waits for the last flash operation to complete and clears any errors.
 * @retval 1 if successful, 0 if timeout or error occurred.
 */
int flash_wait_for_last_operation(void) {
    uint32_t timeout = HAL_GetTick() + 100; // 100ms
    
    while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY)) {
        if (HAL_GetTick() > timeout) {
            return 0;
        }
    }
    
    if (__HAL_FLASH_GET_FLAG(FLASH_FLAG_OPERR) || 
        __HAL_FLASH_GET_FLAG(FLASH_FLAG_WRPERR) || 
        __HAL_FLASH_GET_FLAG(FLASH_FLAG_PGAERR) || 
        __HAL_FLASH_GET_FLAG(FLASH_FLAG_PGPERR) || 
        __HAL_FLASH_GET_FLAG(FLASH_FLAG_PGSERR)) {
        
        // Clear error flags
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                             FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | 
                             FLASH_FLAG_PGSERR);
        return 0;
    }
    
    return 1;
}


/**
 * @brief Determines the flash sector index for a given address.
 * @param addr The flash memory address.
 * @retval Sector index or 0xFF if address is invalid.
 */
uint8_t flash_get_sector(uint32_t addr) {
    if (addr < FLASH_BASE) {
        return 0xFF; // Invalid addr
    }
    
    uint32_t offset = addr - FLASH_BASE;
    uint32_t current_offset = 0;
    
    for (uint8_t i = 0; i < FLASH_SECTOR_COUNT; i++) {
        uint32_t sector_size_bytes = FLASH_SECTORS_KB[i] * 1024;
        if (offset >= current_offset && offset < current_offset + sector_size_bytes) {
            return i;
        }
        current_offset += sector_size_bytes;
    }
    
    return 0xFF; // addr
}

/**
 * @brief Gets the starting address of a given sector.
 * @param sector Sector index.
 * @retval Sector start address, or 0 if invalid.
 */
uint32_t flash_get_sector_start(uint8_t sector) {
    if (sector >= FLASH_SECTOR_COUNT) {
        return 0;
    }
    
    uint32_t address = FLASH_BASE;
    for (uint8_t i = 0; i < sector; i++) {
        address += FLASH_SECTORS_KB[i] * 1024;
    }
    
    return address;
}

/**
 * @brief Gets the ending address of a given sector.
 * @param sector Sector index.
 * @retval Sector end address, or 0 if invalid.
 */
uint32_t flash_get_sector_end(uint8_t sector) {
    if (sector >= FLASH_SECTOR_COUNT) {
        return 0;
    }
    
    uint32_t end_address = FLASH_BASE;
    for (uint8_t i = 0; i <= sector; i++) {
        end_address += FLASH_SECTORS_KB[i] * 1024;
    }
    
    return end_address - 1; // Last valid address in sector
}

/**
 * @brief Erases a single flash sector by its address.
 * @param sector_addr Address located in the target sector.
 * @retval Number of bytes that were erased if successful, 0 otherwise.
 */
int flash_erase_sector(uint32_t sector_addr) {
    // Check if any pending operations
    if (HAL_FLASH_GetError() != HAL_FLASH_ERROR_NONE) {
        // Clear error flags
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                             FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | 
                             FLASH_FLAG_PGSERR);
        return 0;
    }
    
    // Get sector number
    uint8_t sector = flash_get_sector(sector_addr);
    if (sector == 0xFF) {
        return 0;
    }
    
    // Prepare for erase
    FLASH_EraseInitTypeDef EraseInitStruct = {0};
    uint32_t SectorError = 0;
    
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3; // 2.7V to 3.6V
    EraseInitStruct.Sector = sector;
    EraseInitStruct.NbSectors = 1;
    
    // Unlock flash
    if (!flash_unlock()) {
        return 0;
    }
    
    // Erase the sector
    HAL_StatusTypeDef status = HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError);
    
    // Lock flash again
    flash_lock();
    
    if (status != HAL_OK) {
        return 0;
    }
    
    // Return the size of the erased sector in bytes
    return FLASH_SECTORS_KB[sector] * 1024;
}

/**
 * @brief Erases all flash sectors starting from a specified address.
 * @param destination Starting address for erase.
 * @retval 1 if successful, 0 otherwise.
 */
int flash_erase(uint32_t destination) {
    // Check if any pending operations
    if (HAL_FLASH_GetError() != HAL_FLASH_ERROR_NONE) {
        return 0;
    }
    
    // Get starting sector
    uint8_t start_sector = flash_get_sector(destination);
    if (start_sector == 0xFF) {
        return 0;
    }
    
    // Unlock flash
    if (!flash_unlock()) {
        return 0;
    }
    
    // Erase all sectors from start_sector to the end
    FLASH_EraseInitTypeDef EraseInitStruct = {0};
    uint32_t SectorError = 0;
    
    for (uint8_t i = start_sector; i < FLASH_SECTOR_COUNT; i++) {
        EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
        EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
        EraseInitStruct.Sector = i;
        EraseInitStruct.NbSectors = 1;
        
        if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK) {
            flash_lock();
            return 0;
        }
    }
    
    // Lock flash
    flash_lock();
    
    return 1;
}

/**
 * @brief Writes data to flash memory.
 * @param addr Target flash address.
 * @param data Pointer to data buffer.
 * @param len Number of bytes to write.
 * @retval 1 if successful, 0 otherwise.
 */
int flash_write(uint32_t addr, const uint8_t* data, size_t len) {
    if (len == 0) {
        return 1; // Nothing to do
    }
    
    // Check alignment
    if (len % 4 != 0) {
        // Use static buffer for alignment
        static uint8_t aligned_buffer[256]; // Static buffer for alignment
        
        // Check if buffer is large enough
        size_t aligned_len = ((len + 3) / 4) * 4;
        if (aligned_len > sizeof(aligned_buffer)) {
            return 0; // Buffer too small
        }
        
        // Copy data to aligned buffer
        memcpy(aligned_buffer, data, len);
        
        // Pad with 0xFF (erased flash state)
        for (size_t i = len; i < aligned_len; i++) {
            aligned_buffer[i] = 0xFF;
        }
        
        // Call ourselves with the aligned data
        return flash_write(addr, aligned_buffer, aligned_len);
    }
    
    // Wait for any previous operations
    if (!flash_wait_for_last_operation()) {
        return 0;
    }
    
    // Unlock
    if (!flash_unlock()) {
        return 0;
    }
    
    // Program flash
    for (size_t offset = 0; offset < len; offset += 4) {
        uint32_t data_word = 0;
        memcpy(&data_word, data + offset, 4);
        
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr + offset, data_word) != HAL_OK) {
            flash_lock();
            return 0;
        }
        
        // Wait with timeout
        if (!flash_wait_for_last_operation()) {
            flash_lock();
            return 0;
        }
        
        // Verify written data
        uint32_t read_data = *(__IO uint32_t*)(addr + offset);
        if (read_data != data_word) {
            flash_lock();
            return 0;
        }
    }
    
    // Lock flash
    flash_lock();
    
    return 1;
}

/**
 * @brief Reads data from flash memory.
 * @param addr Source flash address.
 * @param data Pointer to destination buffer.
 * @param len Number of bytes to read.
 */
void flash_read(uint32_t addr, uint8_t* data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        data[i] = *(__IO uint8_t*)(addr + i);
    }
}


/**
 * @brief Writes data across sector boundaries, handling erasure and alignment.
 * @param current_addr Current write address.
 * @param current_sector Current sector index.
 * @param data Data to write.
 * @param data_len Length of data.
 * @param new_addr Optional output pointer to receive next write address.
 * @param new_sector Optional output pointer to receive new sector index.
 * @retval 1 if successful, 0 otherwise.
 */
int flash_write_across_sectors(uint32_t current_addr, uint8_t current_sector,
                              const uint8_t* data, size_t data_len,
                              uint32_t* new_addr, uint8_t* new_sector) {
    
    // Check if we need to cross a sector boundary
    uint32_t next_addr = current_addr + data_len;
    uint8_t target_sector = flash_get_sector(next_addr - 1);
    
    if (target_sector != current_sector && target_sector != 0xFF) {
        // Calculate sector boundaries
        uint32_t current_sector_end = flash_get_sector_end(current_sector);
        uint32_t next_sector_base = flash_get_sector_start(target_sector);
        
        // Calculate how much data goes in each sector
        uint32_t bytes_in_current = current_sector_end - current_addr + 1;
        
        // Make sure it's aligned
        bytes_in_current = (bytes_in_current / 4) * 4;
        
        // Erase the next sector first
        if (!flash_erase_sector(next_sector_base)) {
            return 0;
        }
        
        // Write to current sector if needed
        if (bytes_in_current > 0 && bytes_in_current <= data_len) {
            if (!flash_write(current_addr, data, bytes_in_current)) {
                return 0;
            }
        }
        
        // Write to next sector
        uint32_t bytes_in_next = data_len - bytes_in_current;
        if (bytes_in_next > 0) {
            if (!flash_write(next_sector_base, data + bytes_in_current, bytes_in_next)) {
                return 0;
            }
        }
        
        // Update output parameters
        if (new_addr) {
            *new_addr = next_sector_base + bytes_in_next;
        }
        
        if (new_sector) {
            *new_sector = target_sector;
        }
    } else {
        // Standard write within the same sector
        if (!flash_write(current_addr, data, data_len)) {
            return 0;
        }
        
        // Update output parameters
        if (new_addr) {
            *new_addr = current_addr + data_len;
        }
        
        if (new_sector) {
            *new_sector = current_sector;
        }
    }
    
    return 1;
}