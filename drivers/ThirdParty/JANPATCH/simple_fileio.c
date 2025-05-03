#include "simple_fileio.h"

size_t sfio_fread(void *ptr, size_t size, size_t count, sfio_stream_t *stream)
{
    assert(size == 1); // We only support size=1 for simplicity
    
    // Check if we'd read past the end of the stream
    if (stream->offset + count > stream->size) {
        count = stream->size - stream->offset;
    }
    
    if (count == 0) {
        return 0;
    }
    
    if (stream->type == SFIO_STREAM_SLOT) {
        // Reading from flash memory
        uint32_t addr = stream->slot + stream->offset;
        memcpy(ptr, (void*)addr, count);
    } else {
        // Reading from RAM
        memcpy(ptr, stream->ptr + stream->offset, count);
    }
    
    // Update the stream offset
    stream->offset += count;
    
    return count;
}

size_t sfio_fwrite(const void *ptr, size_t size, size_t count, sfio_stream_t *stream)
{
    assert(size == 1); // We only support size=1 for simplicity
    
    // Check if we'd write past the end of the stream
    if (stream->offset + count > stream->size) {
        count = stream->size - stream->offset;
    }
    
    if (count == 0) {
        return 0;
    }
    
    if (stream->type == SFIO_STREAM_SLOT) {
        // Writing to flash memory
        uint32_t addr = stream->slot + stream->offset;
        if (!flash_write(addr, (const uint8_t*)ptr, count)) {
            return 0; // Write failed
        }
    } else {
        // Writing to RAM
        memcpy(stream->ptr + stream->offset, ptr, count);
    }
    
    // Update the stream offset
    stream->offset += count;
    
    return count;
}

int sfio_fseek(sfio_stream_t *stream, long int offset, int origin)
{
    long int new_offset;
    
    // Calculate the new offset based on the origin
    if (origin == SEEK_SET) {
        new_offset = offset;
    } else if (origin == SEEK_CUR) {
        new_offset = stream->offset + offset;
    } else {
        // SEEK_END or invalid origin
        return -1;
    }
    
    // Check if the new offset is valid
    if (new_offset < 0 || (size_t)new_offset > stream->size) {
        return -1;
    }
    
    // Set the new offset
    stream->offset = new_offset;
    
    return 0;
}