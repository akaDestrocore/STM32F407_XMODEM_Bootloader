#include "uart_init.h"
#include "stm32f407xx.h"

/* Private structures ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
extern struct CircularBuffer rxCB,txCB;

/* Flags ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
extern uint8_t TxInProgress;


/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> UART ADVANCED PERIPHERAL BUS CONFIGURATION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void UART4_APB_Config(void)
{

	RCC->APB1ENR = 0x00000000; 								//reset APB1 periph clock enable register

	RCC->APB1ENR |= RCC_APB1ENR_UART4EN;					//enable UART4 clock
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */





/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> UART4 CONFIGURATION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void UART4_Config(int freq, int baudrate)
{
	UART4_APB_Config();
	UART4_GPIO_Config();

	UART4->CR1 =  0x00000000;   							//reset value

	UART4->CR3 |= USART_CR3_EIE;							//turn on Error interrupt

	UART4->CR1 |= USART_CR1_RXNEIE;							//turn on RX not empty interrupt

	UART4->CR1 |= USART_CR1_UE;								//USART enable

	UART4->CR1 &= ~USART_CR1_M;								//M = 0 8bit length

	UART4->BRR |= freq/(baudrate); 	      					//baud rate = 115200, APB/BAUDRATE

	UART4->CR1 |= USART_CR1_RE; 							//Receiver is enabled

	UART4->CR1 |= USART_CR1_TE;								//Transmitter is enabled

	NVIC_EnableIRQ(UART4_IRQn);								//NVIC UART4 global interrupt
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */






/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> GPIO CONFIGURATION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void UART4_GPIO_Config(void)
{
	RCC->AHB1ENR = 0x00100000;														//reset AHB1 periph clock register

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;											//enable GPIOC clock

	GPIOC->MODER = 0x00000000;														//reset GPIOC mode register
	GPIOC->MODER |= GPIO_MODER_MODER10_1;											//10 => alternate function mode (PC10)
	GPIOC->MODER |= GPIO_MODER_MODER11_1;											//alternate function mode (PC11)

	GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED10 | GPIO_OSPEEDR_OSPEED11; 				//PA10 and PA11 speed set to very high

	GPIOC->AFR[1] |= GPIO_AFRH_AFSEL10_3; 											//set AF8 alternate function for UART4(PC10)
	GPIOC->AFR[1] |= GPIO_AFRH_AFSEL11_3;											//set AF8 alternate function for UART4(PC11)
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */





/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> UART RECEIVE/READ FUNCTION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
uint8_t UART4_Receive(void)
{
	uint8_t data = 0;
	if((UART4->SR & USART_SR_RXNE)==0)
	{
		data = UART4->DR;								//write data to data register
		writeCircularBuffer(&rxCB,data);
		return data;
	}
	else
	{
		return 0;
	}
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */





/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> UART RECEIVE STRING FUNCTION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
uint8_t UART4_ReceiveString(uint8_t *input_data, uint16_t length)
{
		uint8_t data;
		int i;
		for(i = 0; i<length; i++)
		{
			readCircularBuffer(&rxCB, &data);
			input_data[i] = data;
		}
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */






/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> UART WRITE/PRINT FUNCTION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void UART4_Write(uint8_t ch)
{
	UART4->DR = ch; 					//data print
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */





/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> UART WRITE/PRINT BYTE FUNCTION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void UART4_SendByte(uint8_t Char)
{
	writeCircularBuffer(&txCB, Char);
	initiateTransmission(&txCB);
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */





/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> UART WRITE/PRINT STRING FUNCTION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void UART4_SendString(uint8_t String[])
{
	uint8_t i = 0;
	while(String[i] != 0)
	{
		writeCircularBuffer(&txCB, String[i]);
		i++;
	}
	initiateTransmission(&txCB);
	delay_ms(10);
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> END OF FILE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
