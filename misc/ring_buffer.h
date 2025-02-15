/*
 @file     ring_buffer.h							@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 @brief    Generic circular/ring buffer	header	@@@@@@@@@@@@@@@@##@@@@@@@@`%@@@@@@@@@@@@@@@@@@@@
												@@@@@@@@@@@@@@@@‾‾* `        ` *@@@@@@@@@@@@@@@@@
 @author   destrocore							@@@@@@@@@@@@@@#                   #@@@@@@@@@@@@@
 @version  V1.0									@@@@@@@@@@@@                        @@@@@@@@@@@@
												@@@@@@@@@@@          _ @@@@@@@\     ``\@@@@@@@@@
This file provides functions to initialize		@@@@@@@@%       %@@@@ ``*@@@@@@\_      \@@@@@@@@
circular buffer and use it for FIFO operations	@@@@@@@*      +@@@@@  /@@#  `*@@@@\_    \@@@@@@@
												@@@@@@/      /@@@@@   /@@  @@@@@@@@@|    !@@@@@@
 	 	 	 	 	 	 	 	 	 	 	 	@@@@/       /@@@@@@@%  *  /` ___*@@@|    |@@@@@@
												@@@#       /@@@@@@@@@       ###}@@@@|    |@@@@@@
												@@@@@|     |@@@@@@@@@      	  __*@@@      @@@@@@
												@@@@@*     |@@@@@@@@@        /@@@@@@@/     '@@@@
												@@@@@@|    |@@ \@@          @@@@@@@@@      /@@@@
												@@@@@@|     |@@ _____     @@@@@@@@*       @@@@@@
												@@@@@@*     \@@@@@@@@@    @@@@@@@/         @@@@@
												@@@@@@@\     \@@@@@@@@@  @@@@@@@%        @@@@@@@
												@@@@@@@@\     \@@@@@@@@  @\  ‾‾‾     		    @@@@@@
												@@@@@@@@@@\    \@@@@@@@  @@/ _==> $     @@@@@@@@
												@@@@@@@@@@@@*    \@@@@@@@@@@@##‾‾   ``  @@@@@@@@@
												@@@@@@@@@@@@@@@@\     ___*@@`*    /@@@@@@@@@@@@@
												@@@@@@@@@@@@@@@@@@@@@--`@@@@__@@@@@@@@@@@@@@@@@@
												@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@												*/
#ifndef __RING_BUFFER_H_
#define __RING_BUFFER_H_

#include <main.h>

// extern your UART handle structure that you use for data transmission to host
extern UART_HandleTypeDef huart;

typedef enum
{
	RINGBUFFER_SIZE_8BITS = 8,
	RINGBUFFER_SIZE_16BITS = 16,
	RINGBUFFER_SIZE_32BITS = 32,
	RINGBUFFER_SIZE_64BITS = 64,
	RINGBUFFER_SIZE_128BITS = 128,
	RINGBUFFER_SIZE_256BITS = 256,
}RINGBUFFER_SIZE_t;

#define RING_BUFFER_SIZE	RINGBUFFER_SIZE_256BITS

/*
 * Ring buffer configuration structure definition
 */
typedef struct{
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
void RingBuffer_Write(Ring_Buffer_t * pRingBuff, uint8_t ch);
void RingBuffer_Transmit(Ring_Buffer_t * pTXBuff);
uint8_t RingBuffer_Read(uint8_t *ch , Ring_Buffer_t * pRingBuff);
/*
 * non-blocking UART send string api for UART communication using ring buffer (also requires changes in
 * UART interrupt handler)
 */
void USART_SendString(uint8_t str[]);


#endif /* __RING_BUFFER_H_ */
