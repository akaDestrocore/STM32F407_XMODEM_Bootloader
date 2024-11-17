#ifndef __XMODEM_H_
#define __XMODEM_H_

#include <stdint.h>

/*
 * XMODEM states
 */
typedef enum
{
	WAIT_SOH 	= 0x1U,
	WAIT_INDEX1 = 0X2U,
	WAIT_INDEX2 = 0x3U,
	READ_DATA 	= 0x4U,
	WAIT_CRC  	= 0x5U
}XMODEM_State_t;

/*
 *  Bytes defined by the protocol
 */
typedef enum
{
	X_C	  = ((uint8_t)0x43U),  // ACII 'C'
	X_SOH = ((uint8_t)0x01U),  //start of header (128 bytes)
	X_EOT = ((uint8_t)0x04U),  //end of transmission
	X_ACK = ((uint8_t)0x06U),  //acknowledge
	X_NAK = ((uint8_t)0x15U),  //not acknowledge
	X_CAN = ((uint8_t)0x18U),  //cancel
}X_Bytes_t;

// CRC 16 bit calculation for a char buffer
int X_CRC16_Calculate(uint8_t *ptr, int count);

#endif /* __XMODEM_H_ */
