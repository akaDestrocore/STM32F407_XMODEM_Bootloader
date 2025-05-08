#include "image.h"
#include "flash.h"


/**
 * @brief  Initializes the CRC peripheral clock.
 * @note   This function is a wrapper around the HAL function to enable the CRC clock.
 * @note   Must be called before performing any CRC operations.
 */
void crc_init(void) {
    __HAL_RCC_CRC_CLK_ENABLE();
}

/**
 * @brief  Resets the CRC calculation unit.
 * @note   This clears the internal CRC calculation register and prepares it for a new computation.
 */
void crc_reset(void) {
    CRC->CR = CRC_CR_RESET;
}


/**
 * @brief  Calculates the CRC for a given data buffer.
 * @param  data: [in] Pointer to the data buffer.
 * @param  len: [in] Length of the buffer in bytes.
 * @return The calculated CRC value.
 * @note   This function processes data as 32-bit words, and handles trailing bytes manually.
 */
uint32_t crc_calculate(const uint8_t* data, size_t len) {
    crc_reset();
    
    // Process data word by word
    for (size_t i = 0; i < len / 4; i++) {
        CRC->DR = ((uint32_t*)data)[i];
    }
    
    // Handle remaining bytes
    uint32_t remaining = len % 4;
    if (remaining > 0) {
        uint32_t last_word = 0;
        size_t offset = len - remaining;
        
        for (size_t i = 0; i < remaining; i++) {
            last_word |= (uint32_t)data[offset + i] << (i * 8);
        }
        
        CRC->DR = last_word;
    }
    
    return CRC->DR;
}

/**
 * @brief  Calculates the CRC of a memory region starting at a given address.
 * @param  addr: [in] Start address of the memory region.
 * @param  size: [in] Size of the memory region in bytes.
 * @return The computed CRC value.
 * @note   Memory is read as 32-bit words; remaining bytes are padded and processed.
 */
uint32_t crc_calculate_memory(uint32_t addr, uint32_t size) {
    crc_init();
    crc_reset();
    
    // Process data word by word
    for (uint32_t i = 0; i < size / 4; i++) {
        uint32_t data = *((uint32_t*)(addr + i * 4));
        CRC->DR = data;
    }
    
    // Handle remaining bytes
    uint32_t remaining = size % 4;
    if (remaining > 0) {
        uint32_t last_word = 0;
        uint32_t offset = addr + size - remaining;
        
        for (uint32_t i = 0; i < remaining; i++) {
            last_word |= (uint32_t)(*((uint8_t*)(offset + i))) << (i * 8);
        }
        
        CRC->DR = last_word;
    }
    
    return CRC->DR;
}

/**
 * @brief  Verifies the CRC of a firmware image.
 * @param  addr: [in] Start address of the firmware image in flash.
 * @param  header_size: [in] Size of the image header in bytes.
 * @return 1 if CRC matches, 0 if data size is invalid.
 * @note   This function reads the header to get the expected CRC and data size, then calculates and compares.
 */
int verify_firmware_crc(uint32_t addr, uint32_t header_size) {
    ImageHeader_t header;
    memcpy(&header, (void*)addr, sizeof(ImageHeader_t));
    
    // Check data size
    if (header.data_size == 0 || header.data_size > 0x100000) {
        return 0;
    }
    
    // Calculate CRC for the image
    uint32_t firmware_addr = addr + header_size;
    uint32_t calculated_crc = crc_calculate_memory(firmware_addr, header.data_size);
    
    // Compare with stored CRC
    return calculated_crc == header.crc;
}

int invalidate_firmware(uint32_t addr) {
    // Simply erase the sector containing the header
    return flash_erase_sector(addr);
}