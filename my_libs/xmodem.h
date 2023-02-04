#ifndef XMODEM_H_
#define XMODEM_H_


#include "uart_init.h"
#include "basic_flash_lib.h"
#include "simple_delay.h"


#define PACKET_128_SIZE     ((uint16_t)128u)
#define PACKET_1024_SIZE    ((uint16_t)1024u)

typedef enum{
	WAIT_SOH = 1,
	WAIT_INDEX1,
	WAIT_INDEX2,
	READ_DATA,
	WAIT_CHKSM

}xmodem_state_t;


/* Bytes defined by the protocol. */
#define X_SOH ((uint8_t)0x01U)  //start of header (128 bytes)
#define X_EOT ((uint8_t)0x04U)  //end of transmission
#define X_ACK ((uint8_t)0x06U)  //acknowledge
#define X_NAK ((uint8_t)0x15U)  //not acknowledge
#define X_CAN ((uint8_t)0x18U)  //cancel
#define X_ETB ((uint8_t)0x17U)  //end of transmission block


/* Function prototypes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
uint8_t XModem_Receive(void);


#endif /* XMODEM_H_ */
