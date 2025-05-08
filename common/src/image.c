#include <image.h>

/**
 * @brief Validates an image header based on its type and magic value.
 * @param header Pointer to the image header.
 * @return 1 if valid, 0 otherwise.
 */
int is_image_valid(const ImageHeader_t* header) {
    switch (header->image_type) {
        case IMAGE_TYPE_LOADER:
            return header->image_magic == IMAGE_MAGIC_LOADER;
        case IMAGE_TYPE_UPDATER:
            return header->image_magic == IMAGE_MAGIC_UPDATER;
        case IMAGE_TYPE_APP:
            return header->image_magic == IMAGE_MAGIC_APP;
        default:
            return 0;
    }
}

/**
 * @brief Compares two image headers to determine if the new image has a newer version.
 * @param new_header Pointer to the new image header.
 * @param current_header Pointer to the current image header.
 * @return 1 if new_header is newer, 0 otherwise.
 */
int is_newer_version(const ImageHeader_t* new_header, const ImageHeader_t* current_header) {
    // Compare major
    if (new_header->version_major > current_header->version_major) {
        return 1;
    }
    if (new_header->version_major < current_header->version_major) {
        return 0;
    }
    
    // If major is equal, compare minor
    if (new_header->version_minor > current_header->version_minor) {
        return 1;
    }
    if (new_header->version_minor < current_header->version_minor) {
        return 0;
    }
    
    // If minor equal, compare patch
    return new_header->version_patch > current_header->version_patch;
}

/**
 * @brief Updates the CRC field in an image header.
 * @param header Pointer to the image header.
 * @param crc New CRC value.
 */
void update_header_crc(ImageHeader_t* header, uint32_t crc) {
    header->crc = crc;
}

/**
 * @brief Updates the data size field in an image header.
 * @param header Pointer to the image header.
 * @param size New data size in bytes.
 */
void update_header_data_size(ImageHeader_t* header, uint32_t size) {
    header->data_size = size;
}

/**
 * @brief Updates the vector address field in an image header.
 * @param header Pointer to the image header.
 * @param addr New vector address.
 */
void update_header_vector_addr(ImageHeader_t* header, uint32_t addr) {
    header->vector_addr = addr;
}

/**
 * @brief Validates a packetized image header based on its type and magic value.
 * @param header Pointer to the packet image header.
 * @return 1 if valid, 0 otherwise.
 */
int is_image_valid_packet(const ImageHeader_Packet_t* header) {
    switch (header->image_type) {
        case IMAGE_TYPE_LOADER:
            return header->image_magic == IMAGE_MAGIC_LOADER;
        case IMAGE_TYPE_UPDATER:
            return header->image_magic == IMAGE_MAGIC_UPDATER;
        case IMAGE_TYPE_APP:
            return header->image_magic == IMAGE_MAGIC_APP;
        default:
            return 0;
    }
}

/**
 * @brief Compares two image headers to determine if the new one from received packet is a newer version.
 * @param new_header Pointer to the new packet header.
 * @param current_header Pointer to the current header.
 * @return 1 if new_header is newer, 0 otherwise.
 */
int is_newer_version_packet(const ImageHeader_Packet_t* new_header, const ImageHeader_Packet_t* current_header) {
    // Compare major
    if (new_header->version_major > current_header->version_major) {
        return 1;
    }
    if (new_header->version_major < current_header->version_major) {
        return 0;
    }
    
    // If major is equal, compare minor
    if (new_header->version_minor > current_header->version_minor) {
        return 1;
    }
    if (new_header->version_minor < current_header->version_minor) {
        return 0;
    }
    
    // If minor equal, compare patch
    return new_header->version_patch > current_header->version_patch;
}


/**
 * @brief Copies data from a full image header to a packet header format.
 * @param header Pointer to the source image header.
 * @param packet Pointer to the destination header.
 */
void header_to_packet(const ImageHeader_t* header, ImageHeader_Packet_t* packet) {
    packet->image_magic = header->image_magic;
    packet->image_hdr_version = header->image_hdr_version;
    packet->image_type = header->image_type;
    packet->is_patch = header->is_patch;
    packet->version_major = header->version_major;
    packet->version_minor = header->version_minor;
    packet->version_patch = header->version_patch;
    packet->_padding = header->_padding;
    packet->vector_addr = header->vector_addr;
    packet->crc = header->crc;
    packet->data_size = header->data_size;
}

/**
 * @brief Copies data from a packet image header to a full image header and clears reserved fields.
 * @param packet Pointer to the source packet header.
 * @param header Pointer to the destination image header.
 */
void packet_to_header(const ImageHeader_Packet_t* packet, ImageHeader_t* header) {
    header->image_magic = packet->image_magic;
    header->image_hdr_version = packet->image_hdr_version;
    header->image_type = packet->image_type;
    header->is_patch = packet->is_patch;
    header->version_major = packet->version_major;
    header->version_minor = packet->version_minor;
    header->version_patch = packet->version_patch;
    header->_padding = packet->_padding;
    header->vector_addr = packet->vector_addr;
    header->crc = packet->crc;
    header->data_size = packet->data_size;
    
    // Clear reserved area
    memset(header->reserved, 0, sizeof(header->reserved));
}