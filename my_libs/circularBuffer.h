#ifndef SRC_CIRCULARBUFFER_H_
#define SRC_CIRCULARBUFFER_H_

#define CIRCULAR_BUFFER_SIZE 256

#include "stm32f407xx.h"
#include "main.h"

struct CircularBuffer{
	uint8_t data[CIRCULAR_BUFFER_SIZE];
	uint16_t head;
	uint16_t tail;
	uint16_t dataCount;
};

void initializeCircularBuffer( struct CircularBuffer * cb );

void writeCircularBuffer( struct CircularBuffer * cb, uint8_t ch );

uint8_t readCircularBuffer( struct CircularBuffer * cb, uint8_t *ch );

void initiateTransmission(struct CircularBuffer * tx);

void RingBufferMain( struct CircularBuffer * rx,  struct CircularBuffer * tx);

#endif /* SRC_CIRCULARBUFFER_H_ */
