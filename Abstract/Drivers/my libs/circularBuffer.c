/*										@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@@@@@@@
										@@@@@@@@@@@@@@@@@  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@
										@@@@@@@@@@@@@@@   @@@@@@@         @@@@@@@@@@@@@@
										@@@@@@@@@@@@@     @@@@@@@@  @@@@@@@@@@@@@@@@@@@@
										@@@@@@@@@@@@ @@@  (@@@@@@  @@@@@@@@@@@@@@@@@@@@@
										@@@@@@@@@@@@@@@@   @@@@/  @@@@@@@&         @@@@@
										@@@@@@@@@@@@@@@@   @@@&  @@@@@     @@@@@@@@ @@@@
										@@@@@@@@@@@@@@@@@   @   @@@.    &@@@@@@@@@@@@@@@
										@@@@@@@@@@@@@@@@@             @@@             @@
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

#include <circularBuffer.h>


uint8_t TxInProgress = RESET;


/*
 * Initialization
 */
/********************************************************************************************************/
/* @function name 		- CircularBuffer_Init															*/
/*																										*/
/* @brief				- This function initializes a circular buffer of CircularBuffer_t				*/
/*																										*/
/* @parameter[in]		- pointer to buffer base address												*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void CircularBuffer_Init(CircularBuffer_t * pCircBuff)
{
	pCircBuff->dataCount = 0;
	pCircBuff->head = 0;
	pCircBuff->tail = 0;
	pCircBuff->data_buff[CIRCULAR_BUFFER_SIZE] = 0;
}

/*
 * Read/Write functions
 */
/********************************************************************************************************/
/* @function name 		- CircularBuffer_Write															*/
/*																										*/
/* @brief				- function to write inside a circular buffer									*/
/*																										*/
/* @parameter[in]		- pointer to buffer base address												*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void CircularBuffer_Write(CircularBuffer_t * pCircBuff, uint8_t ch )
{
	__disable_irq();

	//if buffer is not full yet
	if( pCircBuff->dataCount < CIRCULAR_BUFFER_SIZE )
	{
		//first element of buffer is getting overwritten with the incoming data
		pCircBuff->data_buff[pCircBuff->head] = ch;

		//increment head
		pCircBuff->head ++;

		//cycling mechanism
		pCircBuff->head %= CIRCULAR_BUFFER_SIZE;

		//increment data count
		pCircBuff->dataCount++;
	}
	__enable_irq();
}

/********************************************************************************************************/
/* @function name 		- CircularBuffer_Read															*/
/*																										*/
/* @brief				- function to get data outside of a circular buffer into a char buffer			*/
/*																										*/
/* @parameter[in]		- pointer to char variable that holds data from tail							*/
/*																										*/
/* @parameter[in]		- pointer to circular buffer base address										*/
/*																										*/
/* @return				- 1 in case of success and 0 in case if circular buffer was empty				*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
uint8_t CircularBuffer_Read(uint8_t *ch , CircularBuffer_t * pCircBuff)
{
	__disable_irq();
	if(pCircBuff->dataCount)
	{
		//get data from tail
		*ch = pCircBuff->data_buff[pCircBuff->tail];

		//increment tail
		pCircBuff->tail ++;

		//cycling mechanism
		pCircBuff->tail %= CIRCULAR_BUFFER_SIZE;

		//decrement data count
		pCircBuff->dataCount--;


		__enable_irq();
		return 1;

	}
	else{
		__enable_irq();
		return 0;
	}
}

/********************************************************************************************************/
/* @function name 		- CircularBuffer_Transmit														*/
/*																										*/
/* @brief				- function to initiate transmission of data over USART							*/
/*																										*/
/* @parameter[in]		- pointer to TX Circular buffer													*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void CircularBuffer_Transmit(CircularBuffer_t * pTXBuff)
{

	uint8_t temp;
	if(RESET == TxInProgress)
	{
		if(CircularBuffer_Read(&temp, pTXBuff))
		{	//check if TX buffer has any data and if it does assign its value to temp variable
			//send to UART
			USART_Write(temp);						//print temp

			//turn on TX empty interruption
			USART.pUSARTx->CR1.bit.txeie = ENABLE;
			TxInProgress = SET;
		}
		else USART.pUSARTx->CR1.bit.txeie = DISABLE; //turn off TX empty interruption
	}

}

/****************************************************** End of file *************************************************************/
