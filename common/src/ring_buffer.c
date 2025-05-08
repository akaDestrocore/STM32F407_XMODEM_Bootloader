#include "ring_buffer.h"


/**
 * @brief Initializes the ring buffer.
 * @param rb Pointer to the ring buffer structure.
 */
void ring_buffer_init(RingBuffer_t* rb) {
    __disable_irq();
    
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
    
    __enable_irq();
}


/**
 * @brief Writes a byte to the ring buffer.
 * @param rb Pointer to the ring buffer structure.
 * @param byte Byte to write.
 * @return true if the byte was successfully written, false if the buffer is full.
 */
bool ring_buffer_write(RingBuffer_t* rb, uint8_t byte) {
    bool result = false;
    
    __disable_irq();
    
    if (rb->count < RING_BUFFER_SIZE) {
        // Buffer has space
        rb->buffer[rb->head] = byte;
        
        // Move head
        rb->head = (rb->head + 1) % RING_BUFFER_SIZE;
        
        // Increment count
        rb->count++;
        
        result = true;
    }
    
    __enable_irq();
    
    return result;
}


/**
 * @brief Reads a byte from the ring buffer.
 * @param rb Pointer to the ring buffer structure.
 * @param byte Pointer to store the read byte.
 * @return true if a byte was successfully read, false if the buffer is empty.
 */
bool ring_buffer_read(RingBuffer_t* rb, uint8_t* byte) {
    bool result = false;
    
    __disable_irq();
    
    if (rb->count > 0) {
        // Buffer has data
        *byte = rb->buffer[rb->tail];
        
        // Move tail
        rb->tail = (rb->tail + 1) % RING_BUFFER_SIZE;
        
        // Decrement count
        rb->count--;
        
        result = true;
    }
    
    __enable_irq();
    
    return result;
}

/**
 * @brief Checks if the ring buffer is empty.
 * @param rb Pointer to the ring buffer structure.
 * @return true if the buffer is empty, false otherwise.
 */
bool ring_buffer_is_empty(RingBuffer_t* rb) {
    bool result;
    
    __disable_irq();
    result = (rb->count == 0);
    __enable_irq();
    
    return result;
}

/**
 * @brief Checks if the ring buffer is full.
 * @param rb Pointer to the ring buffer structure.
 * @return true if the buffer is full, false otherwise.
 */
bool ring_buffer_is_full(RingBuffer_t* rb) {
    bool result;
    
    __disable_irq();
    result = (rb->count == RING_BUFFER_SIZE);
    __enable_irq();
    
    return result;
}

/**
 * @brief Gets the number of bytes currently stored in the buffer.
 * @param rb Pointer to the ring buffer structure.
 * @return Number of bytes in the buffer.
 */
size_t ring_buffer_len(RingBuffer_t* rb) {
    size_t result;
    
    __disable_irq();
    result = rb->count;
    __enable_irq();
    
    return result;
}


/**
 * @brief Clears the ring buffer.
 * @param rb Pointer to the ring buffer structure.
 */
void ring_buffer_clear(RingBuffer_t* rb) {
    __disable_irq();
    
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
    
    __enable_irq();
}


/**
 * @brief Retrieves the current tail index of the buffer.
 * @param rb Pointer to the ring buffer structure.
 * @return Current tail index.
 */
size_t ring_buffer_get_tail(RingBuffer_t* rb) {
    size_t result;
    
    __disable_irq();
    result = rb->tail;
    __enable_irq();
    
    return result;
}

/**
 * @brief Retrieves the current head index of the buffer.
 * @param rb Pointer to the ring buffer structure.
 * @return Current head index.
 */
size_t ring_buffer_get_head(RingBuffer_t* rb) {
    size_t result;
    
    __disable_irq();
    result = rb->head;
    __enable_irq();
    
    return result;
}

/**
 * @brief Retrieves a byte from the buffer at a specific index without removing it.
 * @param rb Pointer to the ring buffer structure.
 * @param index Index to retrieve the byte from.
 * @return Byte value at the specified index.
 */
uint8_t ring_buffer_get_byte(RingBuffer_t* rb, size_t index) {
    uint8_t result;
    
    __disable_irq();
    result = rb->buffer[index % RING_BUFFER_SIZE];
    __enable_irq();
    
    return result;
}

/**
 * @brief Peeks at the next byte to be read without removing it from the buffer.
 * @param rb Pointer to the ring buffer structure.
 * @param byte Pointer to store the peeked byte.
 * @return true if a byte was successfully peeked, false if the buffer is empty.
 */
bool ring_buffer_peek(RingBuffer_t* rb, uint8_t* byte) {
    bool result = false;
    
    __disable_irq();
    
    if (rb->count > 0) {
        *byte = rb->buffer[rb->tail];
        result = true;
    }
    
    __enable_irq();
    
    return result;
}