#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#ifdef __cplusplus
extern "C"
{
#endif


#include "main.h"
#include <stdint.h>
#include <string.h>

// extern your UART handle structure that you use for data transmission to host
extern UART_HandleTypeDef huart;


typedef enum
{
	RINGBUFFER_SIZE_8B = 8,
	RINGBUFFER_SIZE_16B = 16,
	RINGBUFFER_SIZE_32B = 32,
	RINGBUFFER_SIZE_64B = 64,
	RINGBUFFER_SIZE_128B = 128,
	RINGBUFFER_SIZE_256B = 256,
}RINGBUFFER_SIZE_t;

#ifndef RING_BUFFER_SIZE
#define RING_BUFFER_SIZE 1024
#endif

/*
 * Ring buffer configuration structure definition
 */
typedef struct{
	uint8_t  magic;
	uint8_t  data_buff[RING_BUFFER_SIZE];
	uint16_t head;
	uint16_t tail;
	uint16_t dataCount;
}Ring_Buffer_t;


/*
 * Initialization
 */
void RingBuffer_Init(Ring_Buffer_t * pRingBuff);
/*
 * Read/Write functions
 */
uint8_t RingBuffer_Write(Ring_Buffer_t * pRingBuff, uint8_t ch);
uint8_t RingBuffer_Transmit(Ring_Buffer_t * pTXBuff);
uint8_t RingBuffer_Read(uint8_t *ch , Ring_Buffer_t * pRingBuff);
/*
 * non-blocking UART send string api for UART communication using ring buffer (also requires changes in
 * UART interrupt handler)
 */
void USART_SendString(uint8_t str[]);



#ifdef __cplusplus
}
#endif
#endif /* RING_BUFFER_H_ */
