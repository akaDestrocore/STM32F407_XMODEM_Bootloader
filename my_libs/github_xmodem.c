#include <github_xmodem.h>

/* Global variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
static uint8_t xmodem_packet_number = 1U;         		//packet number counter
static uint32_t xmodem_actual_flash_address = 0U; 		//address where we have to write
static uint8_t x_first_packet_received = false;   		// check if it's first packet or not

/* Local functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
static uint16_t XMDM_Calc_CRC(uint8_t *data, uint16_t length);
static xmodem_status XMDM_Handle_Packet(uint8_t size);
static xmodem_status XMDM_Error_Handler(uint8_t *error_number, uint8_t max_error_number);
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> XMODEM RECEIVE FUNCTION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void XMDM_Receive(void)
{
  volatile xmodem_status status = X_OK;
  uint8_t error_number = 0u;

  x_first_packet_received = false;
  xmodem_packet_number = 1U;
  xmodem_actual_flash_address = FLASH_APP_START_ADDRESS;

  while (X_OK == status)								//loop until there isn't any error (or until we jump to the user application)
  {
	uint8_t header = 0x00U;

	/* Get the header from UART. */
	uart_status comm_status = UART4_ReceiveString(&header, 1U);

	/* Spam the host (until we receive something) with ACSII "C", to notify it, we want to use CRC-16. */
	if ((UART_OK != comm_status) && (false == x_first_packet_received))
	{
	  UART4_Write(X_C);
	}
	/* Uart timeout or any other errors. */
	else if ((UART_OK != comm_status) && (true == x_first_packet_received))
	{
	  status = XMDM_Error_Handler(&error_number, X_MAX_ERRORS);
	}
	else
	{
	  /* Do nothing. */
	}

	UART4_ReceiveString(&header, 1U);
	/* The header can be: SOH, STX, EOT and CAN. */
	switch(header)
	{
	  xmodem_status packet_status = X_ERROR;
	  /* 128 or 1024 bytes of data. */
	  case X_SOH:
	  case X_STX:
		/* If the handling was successful, then send an ACK. */
		packet_status = XMDM_Handle_Packet(header);
		if (X_OK == packet_status)
		{
		  (void)UART4_Write(X_ACK);
		}
		/* If the error was flash related, then immediately set the error counter to max (graceful abort). */
		else if (X_ERROR_FLASH == packet_status)
		{
		  error_number = X_MAX_ERRORS;
		  status = XMDM_Error_Handler(&error_number, X_MAX_ERRORS);
		}
		/* Error while processing the packet, either send a NAK or do graceful abort. */
		else
		{
		  status = XMDM_Error_Handler(&error_number, X_MAX_ERRORS);
		}
		break;
	  /* End of Transmission. */
	  case X_EOT:
		/* ACK, feedback to user (as a text), then jump to user application. */
		(void)UART4_Write(X_ACK);
		(void)UART4_SendString((uint8_t*)"\n\rFirmware updated!\n\r");
		(void)UART4_SendString((uint8_t*)"Jumping to user application...\n\r");
		BFL_Jump();
		break;
	  /* Abort from host. */
	  case X_CAN:
		status = X_ERROR;
		break;
	  default:
		/* Wrong header. */
		if (UART_OK == comm_status)
		{
		  status = XMDM_Error_Handler(&error_number, X_MAX_ERRORS);
		}
		break;
	}
  }
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */





/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> XMODEM CRC CALCULATION FUNCTION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static uint16_t XMDM_Calc_CRC(uint8_t *data, uint16_t length)
{
    uint16_t crc = 0U;
    while (length)
    {
        length--;
        crc = crc ^ ((uint16_t)*data++ << 8U);
        for (uint8_t i = 0U; i < 8U; i++)
        {
            if (crc & 0x8000U)
            {
                crc = (crc << 1U) ^ 0x1021U;
            }
            else
            {
                crc = crc << 1U;
            }
        }
    }
    return crc;
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */





/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> XMODEM HANDLE PACKET FUNCTION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static xmodem_status XMDM_Handle_Packet(uint8_t header)
{
  xmodem_status status = X_OK;
  uint16_t size = 0U;

  /* 2 bytes for packet number, 1024 for data, 2 for CRC*/
  uint8_t received_packet_number[X_PACKET_NUMBER_SIZE];
  uint8_t received_packet_data[X_PACKET_1024_SIZE];
  uint8_t received_packet_crc[X_PACKET_CRC_SIZE];

  /* Get the size of the data. */
  if (X_SOH == header)
  {
    size = X_PACKET_128_SIZE;
  }
  else if (X_STX == header)
  {
    size = X_PACKET_1024_SIZE;
  }
  else
  {
    /* Wrong header type. This shoudn't be possible... */
    status |= X_ERROR;
  }

  uart_status comm_status = UART_OK;
  /* Get the packet number, data and CRC from UART. */
  comm_status |= UART4_ReceiveString(&received_packet_number[0u], X_PACKET_NUMBER_SIZE);
  comm_status |= UART4_ReceiveString(&received_packet_data[0u], size);
  comm_status |= UART4_ReceiveString(&received_packet_crc[0u], X_PACKET_CRC_SIZE);
  /* Merge the two bytes of CRC. */
  uint16_t crc_received = ((uint16_t)received_packet_crc[X_PACKET_CRC_HIGH_INDEX] << 8u) | ((uint16_t)received_packet_crc[X_PACKET_CRC_LOW_INDEX]);
  /* We calculate it too. */
  uint16_t crc_calculated = XMDM_Calc_CRC(&received_packet_data[0u], size);

  /* Communication error. */
  if (UART_OK != comm_status)
  {
    status |= X_ERROR_UART;
  }

  /* If it is the first packet, then erase the memory. */
  if ((X_OK == status) && (false == x_first_packet_received))
  {
    BFL_FLASH_Erase(FLASH_APP_START_ADDRESS);
    x_first_packet_received = true;
  }

  /* Error handling and flashing. */
  if (X_OK == status)
  {
    if (xmodem_packet_number != received_packet_number[0u])
    {
      /* Packet number counter mismatch. */
      status |= X_ERROR_NUMBER;
    }
    if (255u != (received_packet_number[X_PACKET_NUMBER_INDEX] + received_packet_number[X_PACKET_NUMBER_COMPLEMENT_INDEX]))
    {
      /* The sum of the packet number and packet number complement aren't 255. */
      /* The sum always has to be 255. */
      status |= X_ERROR_NUMBER;
    }
    if (crc_calculated != crc_received)
    {
      /* The calculated and received CRC are different. */
      status |= X_ERROR_CRC;
    }
  }
    /* Do the actual flashing (if there weren't any errors). */
    if ((X_OK == status) && (0 != BFL_FLASH_Write((uint32_t*)&received_packet_data[0U], (uint32_t)size/4U, xmodem_actual_flash_address)))
    {
      /* Flashing error. */
      status |= X_ERROR_FLASH;
    }

  /* Raise the packet number and the address counters (if there weren't any errors). */
  if (X_OK == status)
  {
    xmodem_packet_number++;
    xmodem_actual_flash_address += size;
  }

  return status;
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */





/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> XMODEM ERROR HANDLER FUNCTION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static xmodem_status XMDM_Error_Handler(uint8_t *error_number, uint8_t max_error_number)
{
  xmodem_status status = X_OK;
  /* Raise the error counter. */
  (*error_number)++;
  /* If the counter reached the max value, then abort. */
  if ((*error_number) >= max_error_number)
  {
    /* Graceful abort. */
    (void)UART4_Write(X_CAN);
    (void)UART4_Write(X_CAN);
    status = X_ERROR;
  }
  /* Otherwise send a NAK for a repeat. */
  else
  {
    (void)UART4_Write(X_NAK);
    status = X_OK;
  }
  return status;
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
