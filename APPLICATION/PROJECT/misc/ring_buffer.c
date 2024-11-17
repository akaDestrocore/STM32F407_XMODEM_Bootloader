#include <ring_buffer.h>

// need to extern this flag from main.c
extern uint8_t TxInProgress;

// extern ring buffer that is used for data transmission
extern Ring_Buffer_t txBuff;

/*
 * Initialization
 */
/********************************************************************************************************/
/* @function name 		- RingBuffer_Init																*/
/*																										*/
/* @brief				- This function initializes a ring buffer of RingBuffer_t						*/
/*																										*/
/* @parameter[in]		- pointer to buffer base address												*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note					- none																			*/
/********************************************************************************************************/
void RingBuffer_Init(Ring_Buffer_t * pRingBuff)
{
	pRingBuff->magic = 42;
	pRingBuff->dataCount = 0;
	pRingBuff->head = 0;
	pRingBuff->tail = 0;
	memset(pRingBuff->data_buff, 0,RING_BUFFER_SIZE);
}

/*
 * Read/Write functions
 */
/********************************************************************************************************/
/* @function name 		- RingBuffer_Write																*/
/*																										*/
/* @brief				- function to write inside a ring buffer										*/
/*																										*/
/* @parameter[in]		- pointer to buffer base address												*/
/*																										*/
/* @return				- status. Should return `0` on success											*/
/*																										*/
/* @Note					- none																			*/
/********************************************************************************************************/
uint8_t RingBuffer_Write(Ring_Buffer_t * pRingBuff, uint8_t ch)
{
	__disable_irq();

	if ((0 == RING_BUFFER_SIZE) || (pRingBuff->magic != 42))
	{
		return 1;
	}

    if (pRingBuff->dataCount < RING_BUFFER_SIZE)
    {
        // Write data to the buffer at the current tail position
        pRingBuff->data_buff[pRingBuff->tail] = ch;

        // Move head to the next position and wrap around if necessary
		pRingBuff->tail = (pRingBuff->tail + 1) % RING_BUFFER_SIZE;

        // Increment data count
        pRingBuff->dataCount++;
    }
    else
    {
        // If the buffer is full, overwrite the tail and move both head and tail
        pRingBuff->data_buff[pRingBuff->tail] = ch;
        pRingBuff->head = (pRingBuff->head + 1) % RING_BUFFER_SIZE;
        pRingBuff->tail = (pRingBuff->tail + 1) % RING_BUFFER_SIZE;
    }

	__enable_irq();

	return 0;
}

/********************************************************************************************************/
/* @function name 		- RingBuffer_Read																*/
/*																										*/
/* @brief				- function to get data outside of a ring buffer into a char buffer				*/
/*																										*/
/* @parameter[in]		- pointer to char variable that holds data from tail							*/
/*																										*/
/* @parameter[in]		- pointer to ring buffer base address											*/
/*																										*/
/* @return				- 1 in case of success,0 in case if ring buffer was empty,2 if magic is not set	*/
/*																										*/
/* @Note					- none																			*/
/********************************************************************************************************/
uint8_t RingBuffer_Read(uint8_t *ch , Ring_Buffer_t * pRingBuff)
{
	if(pRingBuff->magic != 42)
	{
		return 2;
	}
	__disable_irq();

	if(0 != pRingBuff->dataCount)
	{
		//get data from head
		*ch = pRingBuff->data_buff[pRingBuff->head];

		pRingBuff->head = (pRingBuff->head + 1) % RING_BUFFER_SIZE;

		pRingBuff->dataCount--;

		__enable_irq();
		return 1;
	}
	else
	{
		__enable_irq();
		return 0;
	}
}

/********************************************************************************************************/
/* @function name 		- RingBuffer_Transmit															*/
/*																										*/
/* @brief				- function to initiate transmission of data over USART							*/
/*																										*/
/* @parameter[in]		- pointer to TX ring buffer														*/
/*																										*/
/* @return				- status. Should return `0` on success											*/
/*																										*/
/* @Note					- none																			*/
/********************************************************************************************************/
uint8_t RingBuffer_Transmit(Ring_Buffer_t * pTXBuff)
{
	if( pTXBuff->magic != 42)
	{
		return 1;
	}

	uint8_t temp = 0;
	if(0 == TxInProgress)
	{
		if(RingBuffer_Read(&temp, pTXBuff))
		{
			// transmit any available data byte
			huart.Instance->DR = temp;

			// turn on TX empty interruption right after transmission is complete
			SET_BIT(huart.Instance->CR1, USART_CR1_TXEIE);
			TxInProgress = SET;

		}
		// turn off TX empty interruption
		else CLEAR_BIT(huart.Instance->CR1,USART_CR1_TXEIE);
	}

	return 0;
}

/********************************************************************************************************/
/* @function name 		- USART_SendString																*/
/*																										*/
/* @brief				- example of ring buffer usage with UART										*/
/*																										*/
/* @parameter[in]		- pointer to char string 														*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note					- Also some adjustments in UART interrupt handler are required in order for this*/
/*						  to work properly																*/
/********************************************************************************************************/
void USART_SendString(uint8_t str[])
{
	uint8_t i = 0;
	while(0 != str[i])
	{
		RingBuffer_Write(&txBuff, str[i]);
		i++;
	}
	RingBuffer_Transmit(&txBuff);
	HAL_Delay(10);
}

/****************************************************** End of file *************************************************************/
