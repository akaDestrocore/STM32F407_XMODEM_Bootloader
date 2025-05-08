#include "delta_update.h"
#include "uart_transport.h"

static unsigned char source_buf[DELTA_BUFFER_SIZE];
static unsigned char target_buf[DELTA_BUFFER_SIZE];
static unsigned char patch_buf[DELTA_BUFFER_SIZE];

/**
 * @brief  Callback function to report the progress of the patching operation.
 * @param  percentage: [in] Progress percentage (0-100).
 * @note   Displays progress in steps of 10% over UART.
 */
static void delta_progress_callback(uint8_t percentage) {
    static uint8_t last_percent = 0;
    
    if (percentage != last_percent && percentage % 10 == 0) {
        char progress[50];
        sprintf(progress, "\rPatching: %u%%", percentage);
        uart_transport_send((const uint8_t*)progress, strlen(progress));
        last_percent = percentage;
    }
}


/**
 * @brief  Applies a delta patch to a source firmware image.
 * @param  source_addr: [in] Address of the source firmware.
 * @param  patch_addr: [in] Address of the patch.
 * @param  target_addr: [in] Address where the patched firmware will be stored.
 * @param  source_size: [in] Size of the source firmware in bytes.
 * @param  patch_size: [in] Size of the patch in bytes.
 * @return 1 on success, 0 on failure.
 * @note   This function uses the `JANPATCH` library to apply the delta patch.
 */
int apply_delta_patch(uint32_t source_addr, uint32_t patch_addr, uint32_t target_addr,
                     uint32_t source_size, uint32_t patch_size) {
    char debug[120];
    sprintf(debug, "Source size=%lu bytes, Patch size=%lu bytes\r\n", 
            source_size, patch_size);
    uart_transport_send((const uint8_t*)debug, strlen(debug));
    
    // Initialize janpatch context
    janpatch_ctx ctx;
    
    ctx.source_buffer.buffer = source_buf;
    ctx.source_buffer.size = DELTA_BUFFER_SIZE;
    ctx.source_buffer.current_page = 0xFFFFFFFF;
    
    ctx.patch_buffer.buffer = patch_buf;
    ctx.patch_buffer.size = DELTA_BUFFER_SIZE;
    ctx.patch_buffer.current_page = 0xFFFFFFFF;
    
    ctx.target_buffer.buffer = target_buf;
    ctx.target_buffer.size = DELTA_BUFFER_SIZE;
    ctx.target_buffer.current_page = 0xFFFFFFFF;
    
    ctx.fread = &sfio_fread;
    ctx.fwrite = &sfio_fwrite;
    ctx.fseek = &sfio_fseek;
    ctx.ftell = NULL;
    ctx.progress = delta_progress_callback;
    ctx.max_file_size = source_size + patch_size;
    
    // Setup source
    sfio_stream_t source;
    source.type = SFIO_STREAM_SLOT;
    source.offset = 0;
    source.size = source_size;
    source.slot = source_addr;
    
    // Setup patch stream
    sfio_stream_t patch;
    patch.type = SFIO_STREAM_RAM;
    patch.offset = 0;
    patch.size = patch_size;
    patch.ptr = (uint8_t*)patch_addr;
    
    // Setup target
    sfio_stream_t target;
    target.type = SFIO_STREAM_SLOT;
    target.offset = 0;
    target.size = source_size * 1.6;
    target.slot = target_addr;
    
    // Apply patch
    uart_transport_send((const uint8_t*)"Starting janpatch operation...\r\n", 31);
    int result = janpatch(ctx, &source, &patch, &target);
    
    if (result == 0) {
        // Success
        uart_transport_send((const uint8_t*)"Patch operation completed successfully\r\n", 41);
        return 1;
    } else {
        // Error
        sprintf(debug, "Patch operation failed with error code: %d\r\n", result);
        uart_transport_send((const uint8_t*)debug, strlen(debug));
        return 0;
    }
}

/**
 * @brief  Calculates the CRC of the firmware image.
 * @param  addr: [in] Address of the firmware to calculate CRC for.
 * @param  size: [in] Size of the firmware in bytes.
 * @return The calculated CRC.
 */
static uint32_t calculate_firmware_crc(uint32_t addr, uint32_t size) {
    // Initialize CRC
    crc_init();
    crc_reset();
    
    return crc_calculate_memory(addr, size);
}

/**
 * @brief  Verifies the CRC of the patched firmware.
 * @param  target_addr: [in] Address of the patched firmware.
 * @param  header_size: [in] Size of the firmware header.
 * @return 1 if CRC matches, 0 otherwise.
 */
int verify_patched_firmware(uint32_t target_addr, uint32_t header_size) {
    ImageHeader_t header;
    memcpy(&header, (void*)target_addr, sizeof(ImageHeader_t));
    
    // Validate header
    if (!is_image_valid(&header)) {
        uart_transport_send((const uint8_t*)"Invalid target header\r\n", 23);
        return 0;
    }
    
    // Calculate CRC
    uint32_t calculated_crc = calculate_firmware_crc(target_addr + header_size, header.data_size);
    
    // Compare with header CRC
    char debug[100];
    sprintf(debug, "CRC Verification - Calculated: 0x%08lX, Expected: 0x%08lX\r\n", 
            calculated_crc, header.crc);
    uart_transport_send((const uint8_t*)debug, strlen(debug));
    
    return (calculated_crc == header.crc);
}

/**
 * @brief  Safely writes data to flash memory.
 * @param  addr: [in] Address to write to.
 * @param  data: [in] Pointer to the data to write.
 * @param  size: [in] Size of the data to write.
 * @param  operation: [in] Operation description (error reporting).
 * @return 1 on success, 0 on failure.
 */
static int safe_flash_write(uint32_t addr, const uint8_t* data, uint32_t size, const char* operation) {
    char debug[120];
    
    // Write to flash
    if (!flash_write(addr, data, size)) {
        sprintf(debug, "ERROR: %s failed\r\n", operation);
        uart_transport_send((const uint8_t*)debug, strlen(debug));
        return 0;
    }
    
    // Basic verification
    for (uint32_t i = 0; i < size; i += 4) {
        uint32_t read_value = *((uint32_t*)(addr + i));
        uint32_t write_value = 0;
        memcpy(&write_value, data + i, 4);
        
        if (read_value != write_value) {
            uart_transport_send((const uint8_t*)"Flash write verification failed\r\n", 32);
            return 0;
        }
    }
    
    return 1;
}

/**
 * @brief  Erases memory sectors to prepare for a new firmware write.
 * @param  addr: [in] Address of the memory to erase.
 * @param  size: [in] Size of the memory to erase.
 * @param  description: [in] Description for error msg.
 * @return 1 on success, 0 on failure.
 */
static int erase_memory_sectors(uint32_t addr, uint32_t size, const char* description) {
    char debug[120];
    sprintf(debug, "Erasing %s sectors at 0x%08lX...\r\n", description, addr);
    uart_transport_send((const uint8_t*)debug, strlen(debug));
    
    // Erase first sector
    if (!flash_erase_sector(addr)) {
        sprintf(debug, "ERROR: Failed to erase %s sector\r\n", description);
        uart_transport_send((const uint8_t*)debug, strlen(debug));
        return 0;
    }
    
    // Calculate additional sectors needed
    // Round up to next full sector
    uint32_t sectors_needed = (size + 0x1FFFF) / 0x20000;
    for (uint32_t i = 1; i < sectors_needed; i++) {
        sprintf(debug, "Erasing additional %s sector at 0x%08lX\r\n", 
                description, addr + (i * 0x20000));
        uart_transport_send((const uint8_t*)debug, strlen(debug));
        
        if (!flash_erase_sector(addr + (i * 0x20000))) {
            sprintf(debug, "ERROR: Failed to erase additional %s sector\r\n", description);
            uart_transport_send((const uint8_t*)debug, strlen(debug));
            return 0;
        }
    }

    return 1;
}

/**
 * @brief  Restores the firmware from a backup if patching fails.
 * @param  target_addr: [in] Target address to restore to.
 * @param  backup_addr: [in] Backup address containing the original firmware.
 * @param  size: [in] Size of the firmware to restore.
 * @return 1 on success, 0 on failure.
 */
static int restore_from_backup(uint32_t target_addr, uint32_t backup_addr, uint32_t size) {
    uart_transport_send((const uint8_t*)"Restoring from backup...\r\n", 26);
    
    // Erase target sectors
    if (!erase_memory_sectors(target_addr, size, "target")) {
        return 0;
    }
    
    // Copy from backup to target
    if (!flash_write(target_addr, (uint8_t*)backup_addr, size)) {
        uart_transport_send((const uint8_t*)"Failed to copy backup\r\n", 23);
        return 0;
    }
    
    uart_transport_send((const uint8_t*)"Restore completed successfully\r\n", 32);
    return 1;
}

/**
 * @brief  Handles the full firmware patching, including backup, patch application and verification.
 * @param  source_addr: [in] Address of the source firmware.
 * @param  patch_addr: [in] Address of the patch.
 * @param  target_addr: [in] Address for the patched firmware.
 * @param  backup_addr: [in] Address for storing a backup.
 * @param  header_size: [in] Size of the header for the firmware images.
 * @return 0 on success, error code on failure.
 * @note   This function handles the entire firmware patching flow including error handling and restoration.
 */
int handle_firmware_patch(uint32_t source_addr, uint32_t patch_addr, uint32_t target_addr, 
    uint32_t backup_addr, uint32_t header_size) {
    
    // Read source header
    ImageHeader_t source_header;
    memcpy(&source_header, (void*)source_addr, sizeof(ImageHeader_t));
    
    // Read patch header
    ImageHeader_t patch_header;
    memcpy(&patch_header, (void*)patch_addr, sizeof(ImageHeader_t));
    
    // Verify source firmware is valid
    if (!is_image_valid(&source_header)) {
        uart_transport_send((const uint8_t*)"ERROR: Source firmware is not valid\r\n", 37);
        return 1;
    }
    
    // Verify patch is valid
    if (!is_image_valid(&patch_header) || !patch_header.is_patch) {
        uart_transport_send((const uint8_t*)"ERROR: Patch is not valid or not marked as a patch\r\n", 51);
        return 2;
    }
    
    // Calculate size
    uint32_t source_data_size = source_header.data_size;
    uint32_t patch_data_size = patch_header.data_size;
    uint32_t source_total_size = source_data_size + header_size;

    uart_transport_send((const uint8_t*)"Step 1: Backing up current firmware...\r\n", 40);
    
    // Erase backup area
    if (!erase_memory_sectors(backup_addr, source_total_size, "backup")) {
        return 4; // Failed to erase backup
    }
    
    // Copy current firmware to backup
    uart_transport_send((const uint8_t*)"Copying firmware to backup...\r\n", 31);
    if (!safe_flash_write(backup_addr, (const uint8_t*)source_addr, source_total_size, "Backup")) {
        uart_transport_send((const uint8_t*)"ERROR: Failed to backup firmware\r\n", 34);
        return 5; // Failed to backup
    }
    
    uart_transport_send((const uint8_t*)"Backup completed successfully\r\n", 31);
    
    // Calculate sectors needed for target
    uint32_t target_expected_size = patch_header.data_size + header_size;

    // Erase target sectors
    if (!erase_memory_sectors(target_addr, target_expected_size, "target")) {
        // Restore from backup
        if (!restore_from_backup(target_addr, backup_addr, source_total_size)) {
            uart_transport_send((const uint8_t*)"ERROR: Failed to restore from backup\r\n", 38);
        }
            
        return 6; // Failed to erase target
    }
    
    // Copy header from patch to target
    if (!safe_flash_write(target_addr, (const uint8_t*)patch_addr, header_size, "Header copy")) {
        uart_transport_send((const uint8_t*)"ERROR: Failed to write header\r\n", 31);
        
        // Restore from backup
        if (!restore_from_backup(target_addr, backup_addr, source_total_size)) {
            uart_transport_send((const uint8_t*)"ERROR: Failed to restore from backup\r\n", 38);
        }
        
        return 7; // Failed to write header
    }
    
    uart_transport_send((const uint8_t*)"Applying delta patch to content...\r\n", 44);
    
    // Apply patch to content
    int result = apply_delta_patch(
        backup_addr + header_size,
        patch_addr + header_size,
        target_addr + header_size,
        source_data_size,
        patch_data_size
    );
    
    if (!result) {
        uart_transport_send((const uint8_t*)"ERROR: Failed to apply patch\r\n", 30);
        
        // Restore from backup
        if (!restore_from_backup(target_addr, backup_addr, source_total_size)) {
            uart_transport_send((const uint8_t*)"ERROR: Failed to restore from backup\r\n", 38);
        }
        
        return 8; // Failed to apply patch
    }
    
    // Verify firmware CRC
    int crc_verified = verify_patched_firmware(target_addr, header_size);
    
    if (!crc_verified) {
        uart_transport_send((const uint8_t*)"ERROR: CRC verification failed!\r\n", 33);
        
        // Restore from backup
        if (!restore_from_backup(target_addr, backup_addr, source_total_size)) {
            uart_transport_send((const uint8_t*)"ERROR: Failed to restore from backup\r\n", 38);
        }
        
        return 9; // CRC verification failed
    }
    
    uart_transport_send((const uint8_t*)"CRC verification successful\r\n", 29);

    uart_transport_send((const uint8_t*)"Cleaning up temporary storage...\r\n", 42);
    if (!erase_memory_sectors(patch_addr, patch_data_size + header_size, "patch")) {
        uart_transport_send((const uint8_t*)"Warning: Failed to clean up patch area\r\n", 39);
    }
    
    // Clean up backup area
    if (!erase_memory_sectors(backup_addr, source_total_size, "backup")) {
        uart_transport_send((const uint8_t*)"Warning: Failed to clean up backup area\r\n", 40);
    }
    
    uart_transport_send((const uint8_t*)"Temporary storage cleaned successfully\r\n", 40);
    uart_transport_send((const uint8_t*)"Patch process completed successfully\r\n", 38);
    return 0; // Success
}