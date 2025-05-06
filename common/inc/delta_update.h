#ifndef _DELTA_UPDATE_H
#define _DELTA_UPDATE_H

#include "flash.h"
#include "crc.h"
#include "janpatch.h"
#include <string.h>
#include <stdint.h>
#include <stddef.h>

#define APP_SIZE            ((uint32_t)0x64000U)
#define PATCH_SIZE          ((uint32_t)0x19000U)
#define DELTA_BUFFER_SIZE   2048

// Apply a delta patch to a firmware image
int apply_delta_patch(uint32_t source_addr, uint32_t patch_addr, uint32_t target_addr,
                      uint32_t source_size, uint32_t patch_size);

// Verify the patched firmware
int verify_patched_firmware(uint32_t target_addr, uint32_t header_size);

// Apply patch and handle the full patching process
int handle_firmware_patch(uint32_t app_addr, uint32_t patch_addr, uint32_t target_addr, 
    uint32_t backup_addr, uint32_t header_size);

#endif /* _DELTA_UPDATE_H */