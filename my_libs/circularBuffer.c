#include "circularBuffer.h"


/* Private flags ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
uint8_t TxInProgress = 0;


/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CIRCULAR BUFFER INITIALIZATION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void initializeCircularBuffer( struct CircularBuffer * cb ){
	cb->dataCount = 0;
	cb->head = 0;
	cb->tail = 0;
	cb->data[CIRCULAR_BUFFER_SIZE];
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */





/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CIRCULAR BUFFER WRITE FUNCTION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void writeCircularBuffer( struct CircularBuffer * cb, uint8_t ch ){
	__disable_irq();
	if( cb->dataCount < CIRCULAR_BUFFER_SIZE )					//if buffer is not full yet
	{
		cb->data[cb->head] = ch;								//first element of buffer is getting overwritten with the incoming data
		cb->head ++;											//head index++
		cb->head %= CIRCULAR_BUFFER_SIZE;						//head = head%32
		cb->dataCount++;										//dataCount = dataCount+1
	}
	__enable_irq();
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */





/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CIRCULAR BUFFER READ FUNCTION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
uint8_t readCircularBuffer( struct CircularBuffer * cb, uint8_t *ch ){

	__disable_irq();
	if(cb->dataCount)
	{
		*ch = cb->data[cb->tail];				//var gets the value of data[tail]
		cb->tail ++;							//tail+1
		cb->tail %= CIRCULAR_BUFFER_SIZE;		//tail = tail%size
		cb->dataCount--;						//dataCount = dataCount-1
		__enable_irq();
		return 1;

	}
	else{
		__enable_irq();
		return 0;
	}
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */





/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> UART TRANSMISSION FORCING FUNCTION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void initiateTransmission(struct CircularBuffer * tx)
{

	uint8_t temp;
	if(0 == TxInProgress){
		if(readCircularBuffer(tx, &temp))				//check if TX buffer has any data and if it does assign its value to temp variable
		{
			//send to UART
			UART4_Write(temp);								//print temp
			//Enable TXIE
			UART4->CR1 |= USART_CR1_TXEIE;					//turn on TX empty interruption
			TxInProgress = 1;
		}
		else UART4->CR1 &= ~(USART_CR1_TXEIE);				//turn off TX empty interruption
	}

}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */





/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CIRCULAR BUFFER CONSTANT READ/WRITE FUNCTION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void RingBufferMain( struct CircularBuffer * rx,  struct CircularBuffer * tx)
{
	uint8_t temp;

		  if(readCircularBuffer(rx, &temp ) )	//checks if RX ring buffer has any data and if it does then temp =  data
		  {
			writeCircularBuffer(tx, temp);		//data becomes first elements of the buffer
	//		writeCircularBuffer(&txCB, temp++);
	//		writeCircularBuffer(&txCB, 'N');		//'N' becomes the first element of the buffer
	//		writeCircularBuffer(&txCB, 'I');
	//		writeCircularBuffer(&txCB, 'K');
	//		writeCircularBuffer(&txCB, 'O');

			if(TxInProgress==0)
			{
				//Initiate transmission
				TxInProgress = 1;
				initiateTransmission(tx);			//checks if TX ring buffer has any data and if it does save it to temp, then print temp and turn on TXEIE
			}
		  }
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> END OF FILE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
