/*
 * this library has all the UART required initialization functions
 */
#ifndef INC_UART_INIT_H_
#define INC_UART_INIT_H_

#include "stm32f407xx.h"
#include "circularBuffer.h"

/* Private define ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define		UART4_TX			PC10
#define		UART4_RX			PC11
#define 	BAUD_RATE			115200
#define		APBx_freq			42000000


/* Status report for the functions. */
typedef enum {
  UART_OK     = 0x00U, /**< The action was successful. */
  UART_ERROR  = 0xFFU  /**< Generic error. */
} uart_status;


void UART4_GPIO_Config(void);
void UART4_Config(int freq, int baudrate);
void UART4_APB_Config(void);
void UART4_Write(uint8_t ch);
void UART4_SendByte(uint8_t Char);
void UART4_SendString(uint8_t String[]);
uint8_t UART4_Receive(void);
uint8_t UART4_ReceiveString(uint8_t *input_data, uint16_t length);

#endif /* INC_UART_INIT_H_ */
