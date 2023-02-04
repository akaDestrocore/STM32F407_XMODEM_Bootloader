#include "xmodem.h"

/* Global variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
static uint8_t xmodem_packet_number;         		//packet number counter
static uint32_t xmodem_actual_flash_address; 		//address where we have to write
