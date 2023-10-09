/*												@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@@@@@@@
 @file     stm32f407xx_usart.h					@@@@@@@@@@@@@@@@@  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 @brief    CMSIS STM32F407xx Device Peripheral 	@@@@@@@@@@@@@@@   @@@@@@@     @   @@@@@@@@@@@@@@
  		   USART HAL driver.					@@@@@@@@@@@@@     @@@@@@@@  @@@@@@@@@@@@@@@@@@@@
 @author   destrocore							@@@@@@@@@@@@ @@@  (@@@@@@  @@@@@@@@@@@@@@@@@@@@@
 @version  V1.0									@@@@@@@@@@@@@@@@   @@@@/  @@@@@@@&   &@@.  @@@@@
												@@@@@@@@@@@@@@@@   @@@&  @@@@@     @@@@@@@@ @@@@
This file provides firmware functions to manage @@@@@@@@@@@@@@@@@   @   @@@.    &@@@@@@@@@@@@@@@
the following functionalities of the Universal 	@@@@@@@@@@@@@@@@@             @@@         %   @@
Synchronous Asynchronous Receiver Transmitter	@@@@@@@@@@@@@@@@@   @@@@@          @@@@@@@@@@@ @
peripheral: 	 	 	 	 	 	 	 	 	@@@@@@@@@@@@@@@@@@@@@@@.%@  @@@@@  @@@@@@@@@@@@@
+ Initialization and de-initialization function	@@@@@@@@@@@@@@@@@@              @@@@@@@@@@@@@@@@
+ Peripheral Control functions					@ @@@@@@@@@@@@@@                  @@@@@@@@@@@@@@
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
#ifndef __STM32F407XX_USART_H_
#define __STM32F407XX_USART_H_

#include <stm32f407xx.h>

/*
 * Configuration structure for USARTx peripheral
 */
typedef struct
{
	uint8_t USART_Mode;
	uint32_t USART_Baud;
	uint8_t USART_NoOfStopBits;
	uint8_t USART_WordLength;
	uint8_t USART_ParityControl;
	uint8_t USART_HWFlowControl;
}USART_Config_t;


/*
 * Handle structure for USARTx peripheral
 */
typedef struct
{
	USART_RegDef_t *pUSARTx;
	USART_Config_t  USART_Config;
	uint8_t *pTxBuffer;
	uint8_t *pRxBuffer;
	uint32_t TxLen;
	uint32_t RxLen;
	uint8_t TxBusyState;
	uint8_t RxBusyState;
}USART_Handle_t;


/*
 * @Possible options for USART_Mode
 */
typedef enum
{
	USART_MODE_ONLY_TX = 0x0U,
	USART_MODE_ONLY_RX = 0x1U,
	USART_MODE_TXRX = 0x2U
}USART_Mode_t;

/*
 * @Possible options for USART_Baud
 */
typedef enum
{
	USART_BAUD_1200	= 1200U,
	USART_BAUD_2400	= 2400U,
	USART_BAUD_9600	= 9600U,
	USART_BAUD_19200 = 19200U,
	USART_BAUD_38400 = 38400U,
	USART_BAUD_57600 = 57600U,
	USART_BAUD_115200 = 115200U,
	USART_BAUD_230400 = 230400U,
	USART_BAUD_460800 = 460800U,
	USART_BAUD_921600 = 921600U,
	USART_BAUD_2M = 2000000U,
	USART_BAUD_3M = 3000000U
}USART_BAUD_t;


/*
 * @Possible options for USART_ParityControl
 */
typedef enum
{
	USART_PARITY_DISABLE = 0x0U,
	USART_PARITY_EN_EVEN = 0x1U,
	USART_PARITY_EN_ODD = 0x2U
}USART_Parity_t;

/*
 * @Possible options for USART_WordLength
 */
typedef enum
{
	USART_WORDLEN_8BITS = 0x0U,
	USART_WORDLEN_9BITS = 0x1U
}USART_WordLen_t;


/*
 * @Possible options for USART_NoOfStopBits
 */
typedef enum
{
	USART_STOPBITS_1 = 0x0U,
	USART_STOPBITS_0_5 = 0x1U,
	USART_STOPBITS_2 = 0x2U,
	USART_STOPBITS_1_5 = 0x3U
}USART_StopBit_t;


/*
 * @Possible options for USART_HWFlowControl
 */
typedef enum
{
	USART_HW_FLOW_CTRL_NONE = 0x0U,
	USART_HW_FLOW_CTRL_CTS = 0x1U,
	USART_HW_FLOW_CTRL_RTS = 0x2U,
	USART_HW_FLOW_CTRL_CTS_RTS = 0x3U
}USART_HWFlow_t;

/*
 * @USART flags
 */
typedef enum
{
    USART_SR_PE = 0x0U,   			/*!< Parity error 									*/
	USART_SR_FE = 0x1U,   			/*!< Framing error 									*/
	USART_SR_NF = 0x2U,   			/*!< Noise detected flag 							*/
	USART_SR_ORE = 0x3U,  			/*!< Overrun error 									*/
	USART_SR_IDLE = 0x4U, 			/*!< IDLE line detected 							*/
	USART_SR_RXNE = 0x5U, 			/*!< Read data register not empty 					*/
	USART_SR_TC = 0x6U,   			/*!< Transmission complete 							*/
	USART_SR_TXE = 0x7U,  			/*!< Transmit data register empty 					*/
	USART_SR_LBD = 0x8U,  			/*!< LIN break detection flag 						*/
	USART_SR_CTS = 0x9U  			/*!< CTS flag 										*/
}USART_SR_flag_t;

/*
 * @Application states
 */
typedef enum
{
	USART_READY = 0x0U,
	USART_BUSY_IN_RX = 0x1U,
	USART_BUSY_IN_TX = 0x2U
}USART_AppState_t;

/*
 * @Application events
 */
typedef enum
{
	USART_EVENT_TX_CMPLT = 0x0U,
	USART_EVENT_RX_CMPLT = 0x1U,
	USART_EVENT_IDLE = 0x2U,
	USART_EVENT_CTS = 0x3U,
	USART_EVENT_PE = 0x4U,
	USART_ERR_FE = 0x5U,
	USART_ERR_NF = 0x6U,
	USART_ERR_ORE = 0x7U
}USART_AppEvent_t;

/********************************************************************************************/
/*								APIs supported by this driver								*/
/*		 For more information about the APIs check the function definitions					*/
/********************************************************************************************/

/*
 * Peripheral Clock setup
 */
void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t state);

/*
 * Init and De-init
 */
void USART_Init(USART_Handle_t *pUSARTHandle);
void USART_DeInit(USART_Handle_t *pUSARTHandle);

/*
 * Data Send and Receive
 */
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len);
void  USART_ReceiveData(USART_Handle_t *pUSARTHandle,uint8_t *pRxBuffer, uint32_t Len);
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t Len);
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pRxBuffer, uint32_t Len);

/*
 * IRQ Configuration and ISR handling
 */
void USART_IRQInterruptConfig(uint8_t IRQNumber,uint32_t IRQPriority,uint8_t state);
void USART_IRQHandling(USART_Handle_t *pUSARTHandle);

/*
 * Other Peripheral Control APIs
 */

uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, USART_SR_flag_t StatusFlagName);
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint16_t StatusFlagName);
void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t state);
void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate);


/*
 * Application Callbacks
 */
__weak void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle,uint8_t event);

#endif /* __STM32F407XX_USART_H_ */

