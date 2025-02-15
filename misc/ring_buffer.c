/*										@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
										@@@@@@@@@@@@@@@@##@@@@@@@@`%@@@@@@@@@@@@@@@@@@@@
										@@@@@@@@@@@@@@@@‾‾* `        ` *@@@@@@@@@@@@@@@@@
										@@@@@@@@@@@@@@#                   #@@@@@@@@@@@@@
										@@@@@@@@@@@@                        @@@@@@@@@@@@
										@@@@@@@@@@@          _ @@@@@@@\     ``\@@@@@@@@@
										@@@@@@@@%       %@@@@ ``*@@@@@@\_      \@@@@@@@@
										@@@@@@@*      +@@@@@  /@@#  `*@@@@\_    \@@@@@@@
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
										@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
										@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@													*/

#include <ring_buffer.h>

// you need to extern this flag from your main.c file
extern uint8_t TxInProgress;

//extern ring buffer that is used for data transmission over UART
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
	pRingBuff->dataCount = 0;
	pRingBuff->head = 0;
	pRingBuff->tail = 0;
	pRingBuff->data_buff[RING_BUFFER_SIZE] = 0;
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
/* @return				- none																			*/
/*																										*/
/* @Note					- none																			*/
/********************************************************************************************************/
void RingBuffer_Write(Ring_Buffer_t * pRingBuff, uint8_t ch )
{
	__disable_irq();

	//if buffer is not full yet
	if(pRingBuff->dataCount < RING_BUFFER_SIZE)
	{
		//first element of buffer is getting overwritten with the incoming data
		pRingBuff->data_buff[pRingBuff->head] = ch;

		//increment head
		pRingBuff->head ++;

		//cycling mechanism
		pRingBuff->head %= RING_BUFFER_SIZE;

		//increment data count
		pRingBuff->dataCount++;
	}
	__enable_irq();
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
/* @return				- 1 in case of success and 0 in case if ring buffer was empty					*/
/*																										*/
/* @Note					- none																			*/
/********************************************************************************************************/
uint8_t RingBuffer_Read(uint8_t *ch , Ring_Buffer_t * pRingBuff)
{
	__disable_irq();
	if(pRingBuff->dataCount)
	{
		//get data from tail
		*ch = pRingBuff->data_buff[pRingBuff->tail];

		//increment tail
		pRingBuff->tail ++;

		//cycling mechanism
		pRingBuff->tail %= RING_BUFFER_SIZE;

		//decrement data count
		pRingBuff->dataCount--;


		__enable_irq();
		return 1;

	}
	else{
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
/* @return				- none																			*/
/*																										*/
/* @Note					- none																			*/
/********************************************************************************************************/
void RingBuffer_Transmit(Ring_Buffer_t * pTXBuff)
{

	uint8_t temp;
	if(RESET == TxInProgress)
	{
		if(RingBuffer_Read(&temp, pTXBuff))
		{	//check if TX buffer has any data and if it does assign its value to temp variable
			//send to UART
			huart.Instance->DR = temp;

			//turn on TX empty interruption
			huart.Instance->CR1 |= USART_CR1_TXEIE;
			TxInProgress = SET;
		}
		else huart.Instance->CR1 &= ~USART_CR1_TXEIE; //turn off TX empty interruption
	}

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
		//fill TX circular buffer with data to print
		RingBuffer_Write(&txBuff, str[i]);
		i++;
	}
	//print the data that is inside the TX circular buffer
	RingBuffer_Transmit(&txBuff);
	HAL_Delay(10);
}

/****************************************************** End of file *************************************************************/
