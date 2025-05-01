#include <xmodem.h>

// Timeout values in ms
#define PACKET_TIMEOUT 5000
#define C_RETRY_INTERVAL 3000
#define MAX_RETRIES 10

#define DATA_SIZE 128

static uint16_t calculate_crc16(const uint8_t* data, size_t len) {
    uint16_t crc = 0;
    
    for (size_t i = 0; i < len; i++) {
        crc ^= (uint16_t)data[i] << 8;
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            } else {
                crc <<= 1;
            }
        }
    }
    
    return crc;
}

void xmodem_init(XmodemManager_t* manager, const XmodemConfig_t* config) {
    memset(manager, 0, sizeof(XmodemManager_t));
    manager->state = XMODEM_STATE_IDLE;
    manager->config = *config;
    manager->header_size = config->image_hdr_size;
    manager->use_encryption = config->use_encryption;
}

void xmodem_start(XmodemManager_t* manager, uint32_t intended_addr) {
    manager->state = XMODEM_STATE_SENDING_INITIAL_C;
    manager->intended_addr = intended_addr;
    
    // using PATCH_ADDR as staging area for reception
    uint32_t staging_addr = PATCH_ADDR;
    manager->target_addr = staging_addr;
    manager->current_addr = staging_addr;
    manager->expected_packet_num = 1;
    manager->buffer_index = 0;
    manager->packet_count = 0;
    manager->retries = 0;
    manager->first_packet_processed = 0;
    manager->next_byte_to_send = XMODEM_C;
    manager->last_poll_time = HAL_GetTick();
    manager->total_data_received = 0;
    manager->actual_firmware_size = 0;
    manager->received_eot = 0;
    manager->first_sector_erased = 0;
    manager->expected_total_packets = 0;
    manager->last_packet_useful_bytes = 0;
    manager->is_patch = 0;
    
    // Set magic based on destination
    if (intended_addr == manager->config.app_addr) {
        manager->expected_magic = IMAGE_MAGIC_APP;
        manager->expected_img_type = IMAGE_TYPE_APP;
    } else if (intended_addr == manager->config.updater_addr) {
        manager->expected_magic = IMAGE_MAGIC_UPDATER;
        manager->expected_img_type = IMAGE_TYPE_UPDATER;
    } else if (intended_addr == manager->config.loader_addr) {
        manager->expected_magic = IMAGE_MAGIC_LOADER;
        manager->expected_img_type = IMAGE_TYPE_LOADER;
    } else {
        manager->state = XMODEM_STATE_ERROR;
        return;
    }
    
    // Determine sector for staging
    uint8_t sector = flash_get_sector(staging_addr);
    if (sector != 0xFF) {
        manager->current_sector = sector;
        manager->current_sector_base = flash_get_sector_start(sector);
        
        // Erase staging area before we start receiving
        flash_erase_sector(staging_addr);
        // Erase additional sector to ensure we have enough space
        flash_erase_sector(staging_addr + 0x20000);
        manager->first_sector_erased = 1;
    } else {
        manager->state = XMODEM_STATE_ERROR;
        return;
    }
}

static int process_first_packet(XmodemManager_t* manager, const uint8_t* data) {
    
        // Normal unencrypted processing
        // First check if data is enough for compact header
        if (DATA_SIZE < sizeof(ImageHeader_Packet_t)) {
            return 0;
        }
        
        // Parse header using the compact version
        ImageHeader_Packet_t* packet_header = (ImageHeader_Packet_t*)data;
        
        // Check magic
        if (packet_header->image_magic != manager->expected_magic) {
            // Apply special handling for patches
            if (manager->is_patch && packet_header->image_magic == IMAGE_MAGIC_APP) {
                // Allow valid patch for the application
            } else {
                // Invalid magic number
                return 0;
            }
        }
        
        // Check image type
        if (packet_header->image_type != manager->expected_img_type) {
            // Apply special handling for patches
            if (manager->is_patch && packet_header->image_type == IMAGE_TYPE_APP) {
                // This is a valid patch for the application, allow it
            } else {
                // Wrong image
                return 0;
            }
        }
        
        // Store patch flag
        manager->is_patch = packet_header->is_patch;
        
        // Store firmware size
        if (packet_header->data_size > 0) {
            manager->actual_firmware_size = packet_header->data_size + manager->header_size;
            
            // Calculate total packets
            uint32_t total_bytes = packet_header->data_size + manager->header_size;
            uint32_t full_packets = total_bytes / DATA_SIZE;
            uint32_t remainder = total_bytes % DATA_SIZE;
            
            manager->expected_total_packets = full_packets + (remainder > 0 ? 1 : 0);
            
            if (remainder > 0) {
                manager->last_packet_useful_bytes = remainder;
            } else {
                manager->last_packet_useful_bytes = DATA_SIZE;
            }
        } else {
            // If no size info provided then use hardcoded value
            manager->actual_firmware_size = 0x40000; // 256 kB
        }
        
        // Check version - we need to read the full header from flash
        if (manager->target_addr != 0xFFFFFFFF) {
            // Read the header from target address
            ImageHeader_t current_full_header;
            memcpy(&current_full_header, (void*)manager->target_addr, sizeof(ImageHeader_t));
            
            // Convert to packet header for comparison
            ImageHeader_Packet_t current_packet_header;
            header_to_packet(&current_full_header, &current_packet_header);
            
            // Check if it's a valid image and if new firmware is newer
            if (is_image_valid_packet(&current_packet_header) && 
                !is_newer_version_packet(packet_header, &current_packet_header)) {
                return 0;
            }
        }
        
        // Erase first sector if not done yet
        if (!manager->first_sector_erased) {
            if (!flash_erase_sector(manager->target_addr)) {
                // failed
                return 0;
            }
            manager->first_sector_erased = 1;
        }
        
        // Write first packet data to flash - the entire packet
        if (!flash_write(manager->current_addr, data, DATA_SIZE)) {
            return 0;
        }
        
        manager->total_data_received = DATA_SIZE;
        manager->current_addr += DATA_SIZE;
        manager->first_packet_processed = 1;
        
        return 1;
    }

static int process_data_packet(XmodemManager_t* manager, const uint8_t* data, uint8_t packet_num) {

    // Get useful data length for unecrypted transfers
    size_t useful_bytes = DATA_SIZE;
    if (manager->expected_total_packets > 0 && 
        packet_num == manager->expected_total_packets && 
        manager->last_packet_useful_bytes > 0) {
        useful_bytes = manager->last_packet_useful_bytes;
    }
    
    if (useful_bytes == 0) {
        // Skip useless bytes
        return 1;
    }
    
    // Track received data
    manager->total_data_received += useful_bytes;
    
    if (manager->actual_firmware_size > 0 && 
        manager->total_data_received > manager->actual_firmware_size) {
        // Adjust useful bytes if we're receiving more than needed
        uint32_t excess = manager->total_data_received - manager->actual_firmware_size;
        if (excess < useful_bytes) {
            useful_bytes -= excess;
            manager->total_data_received = manager->actual_firmware_size;
        } else {
            useful_bytes = 0;
        }
    }
    
    // If we have useful data to write
    if (useful_bytes > 0) {
        // Check if we need to cross a sector boundary
        uint32_t next_addr = manager->current_addr + useful_bytes;
        uint8_t current_sector = manager->current_sector;
        uint8_t target_sector = flash_get_sector(next_addr - 1);
        
        if (target_sector != current_sector && target_sector != 0xFF) {
            // Crossing a sector boundary
            uint32_t next_sector_base = flash_get_sector_start(target_sector);
            
            // Erase the next sector
            if (!flash_erase_sector(next_sector_base)) {
                return 0;
            }
            
            // Calculate how much data goes in current sector
            uint32_t current_sector_end = flash_get_sector_end(current_sector);
            uint32_t bytes_in_current = current_sector_end - manager->current_addr + 1;
            
            // Align to 4
            bytes_in_current = (bytes_in_current / 4) * 4;
            
            // Write to current sector if needed
            if (bytes_in_current > 0) {
                if (!flash_write(manager->current_addr, data, bytes_in_current)) {
                    return 0;
                }
            }
            
            // Write to next sector
            uint32_t bytes_in_next = useful_bytes - bytes_in_current;
            if (bytes_in_next > 0) {
                if (!flash_write(next_sector_base, data + bytes_in_current, bytes_in_next)) {
                    return 0;
                }
            }
            
            // Update current address and sector tracking
            manager->current_addr = next_sector_base + bytes_in_next;
            manager->current_sector = target_sector;
            manager->current_sector_base = next_sector_base;
        } else {
            // Standard write in the same sector
            if (!flash_write(manager->current_addr, data, useful_bytes)) {
                return 0;
            }
            
            manager->current_addr += useful_bytes;
        }
    }
    
    return 1;
}

XmodemError_t xmodem_process_byte(XmodemManager_t* manager, uint8_t byte) {
    uint32_t current_time = HAL_GetTick();
    
    switch (manager->state) {
        case XMODEM_STATE_IDLE:
            return XMODEM_ERROR_NONE;
            
        case XMODEM_STATE_SENDING_INITIAL_C:
            // Check for SOH
            if (byte == XMODEM_SOH) {
                manager->buffer[0] = byte;
                manager->buffer_index = 1;
                manager->state = XMODEM_STATE_RECEIVING_DATA;
                manager->last_poll_time = current_time;
                return XMODEM_ERROR_NONE;
            } else if (byte == XMODEM_CAN) {
                manager->state = XMODEM_STATE_ERROR;
                return XMODEM_ERROR_CANCELLED;
            } else {
                // Spam 'C'
                if (current_time - manager->last_poll_time >= C_RETRY_INTERVAL) {
                    manager->next_byte_to_send = XMODEM_C;
                    manager->last_poll_time = current_time;
                    manager->retries++;
                    
                    if (manager->retries >= MAX_RETRIES) {
                        manager->state = XMODEM_STATE_ERROR;
                        return XMODEM_ERROR_TIMEOUT;
                    }
                    
                    return XMODEM_ERROR_NONE;
                }
                return XMODEM_ERROR_NONE;
            }
            
        case XMODEM_STATE_WAITING_FOR_DATA:
            // Check for timeout
            if (current_time - manager->last_poll_time >= PACKET_TIMEOUT) {
                manager->state = XMODEM_STATE_ERROR;
                return XMODEM_ERROR_TIMEOUT;
            }
            
            // Process received byte
            if (byte == XMODEM_SOH) {
                manager->buffer[0] = byte;
                manager->buffer_index = 1;
                manager->state = XMODEM_STATE_RECEIVING_DATA;
                manager->last_poll_time = current_time;
                return XMODEM_ERROR_NONE;
            } else if (byte == XMODEM_EOT) {
                // End of transmission
                manager->received_eot = 1;
                manager->state = XMODEM_STATE_COMPLETE;
                manager->next_byte_to_send = XMODEM_ACK;
                
                return XMODEM_ERROR_TRANSFER_COMPLETE;

            } else if (byte == XMODEM_CAN) {
                manager->state = XMODEM_STATE_ERROR;
                return XMODEM_ERROR_CANCELLED;
            }
            return XMODEM_ERROR_NONE;
            
        case XMODEM_STATE_RECEIVING_DATA:
            // Check for timeout
            if (current_time - manager->last_poll_time >= PACKET_TIMEOUT) {
                manager->state = XMODEM_STATE_ERROR;
                return XMODEM_ERROR_TIMEOUT;
            }
            
            // Add byte to buffer
            manager->buffer[manager->buffer_index++] = byte;
            
            // Check if we have a complete packet
            if (manager->buffer_index == 133) {
                manager->state = XMODEM_STATE_PROCESSING_PACKET;
                
                // Process the packet
                uint8_t packet_num = manager->buffer[1];
                uint8_t packet_num_comp = manager->buffer[2];
                
                // Check packet number integrity
                if ((packet_num + packet_num_comp) != 0xFF) {
                    manager->state = XMODEM_STATE_WAITING_FOR_DATA;
                    manager->buffer_index = 0;
                    manager->next_byte_to_send = XMODEM_NAK;
                    return XMODEM_ERROR_SEQUENCE_ERROR;
                }
                
                // Check packet sequence
                if (packet_num != manager->expected_packet_num) {
                    manager->state = XMODEM_STATE_WAITING_FOR_DATA;
                    manager->buffer_index = 0;
                    manager->next_byte_to_send = XMODEM_NAK;
                    return XMODEM_ERROR_SEQUENCE_ERROR;
                }
                
                // Verify CRC
                uint16_t received_crc = (manager->buffer[131] << 8) | manager->buffer[132];
                uint16_t calculated_crc = calculate_crc16(&manager->buffer[3], DATA_SIZE);
                
                if (received_crc != calculated_crc) {
                    manager->state = XMODEM_STATE_WAITING_FOR_DATA;
                    manager->buffer_index = 0;
                    manager->next_byte_to_send = XMODEM_NAK;
                    return XMODEM_ERROR_CRC_ERROR;
                }
                
                // Process data based on packet number
                if (packet_num == 1 && !manager->first_packet_processed) {
                    // Check if it's a patch by examining the header
                    uint8_t* data = &manager->buffer[3];
                    
                    // Only check if we're targeting the application
                    if (manager->target_addr == APP_ADDR) {
                        // Check the is_patch flag in the header
                        if (data[7] == 1) {
                            // Set flag and switch target to PATCH_ADDR
                            manager->is_patch = 1;
                            
                            // Erase patch sectors
                            uint8_t sector = flash_get_sector(PATCH_ADDR);
                            if (sector != 0xFF) {
                                flash_erase_sector(PATCH_ADDR);
                                // Erase additional sectors to ensure enough space
                                flash_erase_sector(PATCH_ADDR + 0x20000);
                            }
                            
                            // Update target address
                            manager->target_addr = PATCH_ADDR;
                            manager->current_addr = PATCH_ADDR;
                            
                            // Update current sector information
                            manager->current_sector = flash_get_sector(PATCH_ADDR);
                            manager->current_sector_base = flash_get_sector_start(manager->current_sector);
                        }
                    }
                    
                    // Process first packet as usual
                    if (!process_first_packet(manager, &manager->buffer[3])) {
                        manager->state = XMODEM_STATE_ERROR;
                        return XMODEM_ERROR_INVALID_MAGIC;
                    }
                } else {
                    // Regular data packet
                    if (packet_num == 2 && !manager->first_packet_processed) {
                        manager->state = XMODEM_STATE_ERROR;
                        return XMODEM_ERROR_INVALID_PACKET;
                    }
                    
                    int result = process_data_packet(manager, &manager->buffer[3], packet_num);
                    if (result == 0) {
                        manager->state = XMODEM_STATE_ERROR;
                        return XMODEM_ERROR_FLASH_WRITE_ERROR;
                    }
                    else if (result < 0) {
                        manager->state = XMODEM_STATE_ERROR;
                        return XMODEM_ERROR_AUTHENTICATION_FAILED;
                    }
                }
                
                // Move to next packet
                manager->state = XMODEM_STATE_WAITING_FOR_DATA;
                manager->buffer_index = 0;
                manager->expected_packet_num++;
                manager->packet_count++;
                manager->next_byte_to_send = XMODEM_ACK;
                manager->last_poll_time = current_time;
                return XMODEM_ERROR_NONE;
            }
            return XMODEM_ERROR_NONE;
            
        case XMODEM_STATE_ERROR:
        case XMODEM_STATE_COMPLETE:
        default:
            return XMODEM_ERROR_NONE;
    }
}

int xmodem_should_send_byte(XmodemManager_t* manager) {
    uint32_t current_time = HAL_GetTick();
    
    if (manager->state == XMODEM_STATE_SENDING_INITIAL_C) {
        if (current_time - manager->last_poll_time >= C_RETRY_INTERVAL || manager->next_byte_to_send != 0) {
            if (manager->next_byte_to_send == 0) {
                manager->next_byte_to_send = XMODEM_C;
            }
            manager->last_poll_time = current_time;
            manager->retries++;
            
            if (manager->retries >= MAX_RETRIES) {
                manager->state = XMODEM_STATE_ERROR;
            }
            
            return 1;
        }
        return 0;
    }
    
    return manager->next_byte_to_send != 0;
}

uint8_t xmodem_get_response(XmodemManager_t* manager) {
    uint8_t response = manager->next_byte_to_send;
    manager->next_byte_to_send = 0;
    return response;
}

XmodemState_t xmodem_get_state(XmodemManager_t* manager) {
    return manager->state;
}

uint16_t xmodem_get_packet_count(XmodemManager_t* manager) {
    return manager->packet_count;
}

void xmodem_cancel_transfer(XmodemManager_t* manager) {
    manager->state = XMODEM_STATE_ERROR;
    manager->next_byte_to_send = 0;
}

void xmodem_cleanup(XmodemManager_t* manager) {
#ifdef FIRMWARE_ENCRYPTED
    if (manager->use_encryption && manager->gcm_initialized) {
        mbedtls_gcm_free(&manager->aes);
        manager->gcm_initialized = 0;
    }
#endif
}