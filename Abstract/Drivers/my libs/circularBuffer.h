/*												@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@@@@@@@
 @file     circularBuffer.h						@@@@@@@@@@@@@@@@@  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 @brief    Generic circular/ring buffer	header	@@@@@@@@@@@@@@@   @@@@@@@     @   @@@@@@@@@@@@@@
												@@@@@@@@@@@@@     @@@@@@@@  @@@@@@@@@@@@@@@@@@@@
 @author   destrocore							@@@@@@@@@@@@ @@@  (@@@@@@  @@@@@@@@@@@@@@@@@@@@@
 @version  V1.0									@@@@@@@@@@@@@@@@   @@@@/  @@@@@@@&   &@@.  @@@@@
												@@@@@@@@@@@@@@@@   @@@&  @@@@@     @@@@@@@@ @@@@
This file provides functions to initialize  	@@@@@@@@@@@@@@@@@   @   @@@.    &@@@@@@@@@@@@@@@
circular buffer and use it for FIFO operations	@@@@@@@@@@@@@@@@@             @@@         %   @@
												@@@@@@@@@@@@@@@@@   @@@@@          @@@@@@@@@@@ @
 	 	 	 	 	 	 	 	 	 	 	 	@@@@@@@@@@@@@@@@@@@@@@@.%@  @@@@@  @@@@@@@@@@@@@
												@@@@@@@@@@@@@@@@@@              @@@@@@@@@@@@@@@@
												@ @@@@@@@@@@@@@@                  @@@@@@@@@@@@@@
												@@  @@@@@@@@@                  @@@@@@@@@@@@@@@@@
												@@@@  @@@    @@@@@@@&         .@@@@@@@@@@@@@@@@@
												@@@@@@@#   ###@@@@( @        &@@@@@@@@@@@@@@@@@@
												@@@@@@@@@@@@@@@#     @@     (@     @@@@@@@@@@@@@
												@@@@@@@@@@@@@@     @@@@     @@     @@@@@@@@@@@@@
												@@@@@@@@@@@&     @@@@@@/   @@@@@@    @@@@@@@@@@@
												@@@@@@@@@@@*    @@@@@@@@  @@@@@@@@      @@@@@@@@
												@@@@@@@@@@@      @@@@@@@  @@@@@@@@   %  @@@@@@@@
												@@@@@@@@@@@@       /&@@@  @@@@@@&   @ @@@@@@@@@@
												@@@@@@@@@@@@@@&  ,@@@@@@@@@@@@  @ @@@@@@@@@@@@@@
												@@@@@@@@@@@@@@@@@@  @@@@@@@@@@@%@@@@@@@@@@@@@@@@													*/
#ifndef __CIRCULARBUFFER_H_
#define __CIRCULARBUFFER_H_

#include  <main.h>

extern USART_Handle_t USART;

typedef enum
{
	BUFFER_SIZE_8BITS = 8,
	BUFFER_SIZE_16BITS = 16,
	BUFFER_SIZE_32BITS = 32,
	BUFFER_SIZE_64BITS = 64,
	BUFFER_SIZE_128BITS = 128,
	BUFFER_SIZE_256BITS = 256,
}CIRCULARBUFF_SIZE_t;

#define CIRCULAR_BUFFER_SIZE	BUFFER_SIZE_256BITS

/*
 * Circular buffer configuration structure definition
 */
typedef struct{
	uint8_t data_buff[CIRCULAR_BUFFER_SIZE];
	uint16_t head;
	uint16_t tail;
	uint16_t dataCount;
}CircularBuffer_t;

/*
 * Initialization
 */
void CircularBuffer_Init(CircularBuffer_t * pCircBuff);
/*
 * Read/Write functions
 */
void 	CircularBuffer_Write(CircularBuffer_t * cb, uint8_t ch );
uint8_t CircularBuffer_Read(uint8_t *pCharBuff , CircularBuffer_t * pCircBuff);
void 	CircularBuffer_Transmit(CircularBuffer_t * tx);



#endif /* __CIRCULARBUFFER_H_ */


