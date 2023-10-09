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

#include <main.h>

/* Private definitions **********************************************************************************/
USART_Handle_t USART = {0};
CircularBuffer_t rxCB, txCB;
XMODEM_State_t x_state;
uint8_t rx_byte = 0, prev_index1 = 0, prev_index2 = 0xFF, dataCounter = 0, checksum = 0;
uint8_t a_ReadRxData[132] = "\0";
uint8_t a_ActualData[128] = "\0";
uint8_t sum;

uint32_t currentAddress = FLASH_TEMP_START_ADDRESS;
/* Private flags ****************************************************************************************/
extern uint8_t TxInProgress;

uint8_t PacketReceived;
uint8_t copyData;
uint8_t versionAlreadyChecked = RESET;
uint8_t versionApplicable;

/*
 * main body
 */
int main(void)
{
	SystemClock_Config();

	//Initialize circular buffers for TX and RX
	CircularBuffer_Init(&txCB);
	CircularBuffer_Init(&rxCB);

	//Initialize other peripherals
	USART_Config();
	LED_Config();
	UserButton_Config();

	TIM1_Config();

	//Initialize FLASH
	FLASH_Init(DATA_TYPE_8B);

	//Welcome message
	ShowMessage(MSG_WELCOME);

	//if the button is pressed, then jump to the user application, otherwise stay in the bootloader
	if(RESET == GPIOA->IDR.bit.idr_0)
	{
		ShowMessage(MSG_JUMP);
		FLASH_Jump();
	}

	PacketReceived = RESET;
	copyData = SET;

	//Show 'update in progress' message
	ShowMessage(MSG_UPDT_IN_PRGS);

	//Do some orange LED blinks :D
	LED_Control(ORANGE, ENABLE);
	LED_Control(ORANGE, DISABLE);
	LED_Control(ORANGE, ENABLE);
	LED_Control(ORANGE, DISABLE);
	LED_Control(ORANGE, ENABLE);
	LED_Control(ORANGE, DISABLE);

	//Erase temporary firmware storage
	LED_Control(ORANGE, ENABLE);
	FLASH_EraseSector(FLASH_TEMP_START_ADDRESS);
	LED_Control(ORANGE, DISABLE);

	/* Ask for new data and start the XMODEM protocol communication. */
	ShowMessage(MSG_ASK_FOR_A_FILE);
	x_state = WAIT_SOH;

	while(1)
	{
		//main XMODEM routine
		Firmware_Update();
	}
}

/*
 * update firmware using XMODEM protocol
 */
void Firmware_Update(void)
{
	if(RESET == PacketReceived)
	{
		//write X_NAK inside TX circular Buffer and then Transmit it to host
		CircularBuffer_Write(&txCB,X_NAK);
		CircularBuffer_Transmit(&txCB);
		delay_ms(200);
	}
	if((WAIT_SOH == x_state) && (SET == PacketReceived))
	{
		//send ACK after flashing is also over
		CircularBuffer_Write(&txCB,X_ACK);
		CircularBuffer_Transmit(&txCB);
	}
	//get data from host
	CircularBuffer_Read(&rx_byte, &rxCB);
	if(X_EOT == rxCB.data_buff[0])
	{
		//ACK if EOT byte received and and the communication with host
		CircularBuffer_Write(&txCB,X_ACK);
		CircularBuffer_Transmit(&txCB);
		//Set PacketReceived since the whole packet is received
		PacketReceived = SET;
		rxCB.head = RESET;
		rxCB.tail = RESET;
		rxCB.dataCount = RESET;

		//Now user application may be updated without fear of packet loss during update
		UpdateUserApplication();

	}
	if(SET == copyData)
	{
		//read frame of XMODEM from circular buffer
		for(dataCounter = 0; dataCounter < 132; dataCounter++)
		{
			a_ReadRxData[dataCounter] = rxCB.data_buff[dataCounter];
		}
		PacketReceived = RESET;
		//empty the ring buffer
		rxCB.head = RESET;
		rxCB.tail = RESET;
		rxCB.dataCount = RESET;
	}
	switch(x_state)
	{
		  case WAIT_SOH:
		  {
			  if(X_SOH == a_ReadRxData[0])
			  {
				  //packet received so check for packet index
				  x_state = WAIT_INDEX1;
				  PacketReceived = SET;
				  copyData = RESET;
			  }
			  else PacketReceived = RESET;
			  break;
		  }
		  case WAIT_INDEX1:
		  {
			  //if index is bigger than the previous packet's index by 1 than it's alright
			  if((prev_index1+1) == a_ReadRxData[1])
			  {
				  x_state = WAIT_INDEX2;
				  prev_index1++;
			  }
			  else
			  {
				  CircularBuffer_Write(&txCB,X_CAN);
				  CircularBuffer_Transmit(&txCB);
				  CircularBuffer_Write(&txCB,X_CAN);
				  CircularBuffer_Transmit(&txCB);
				  CircularBuffer_Write(&txCB,X_CAN);
				  CircularBuffer_Transmit(&txCB);
				  USART_SendString((uint8_t*)"Index error! Booting to the current firmware!\n\r\n\r");
				  FLASH_Jump();
			  }
			  break;
		  }
		  case WAIT_INDEX2:
		  {
			  //index 2 should be equal to the difference of 255 and index1
			  if((prev_index2-1) == a_ReadRxData[2])
			  {
				  x_state = READ_DATA;
				  prev_index2--;
			  }
			  else
			  {
				  CircularBuffer_Write(&txCB,X_CAN);
				  CircularBuffer_Transmit(&txCB);
				  CircularBuffer_Write(&txCB,X_CAN);
				  CircularBuffer_Transmit(&txCB);
				  CircularBuffer_Write(&txCB,X_CAN);
				  CircularBuffer_Transmit(&txCB);
				  USART_SendString((uint8_t*)"Index 2 error! Booting to the current firmware!\n\r\n\r");
				  FLASH_Jump();
			  }
			  break;
		  }
		  case READ_DATA:
		  {
			  //eject data from XMODEM frame
			  for(dataCounter = 0; dataCounter < 129; dataCounter++)
			  {
				  a_ActualData[dataCounter] = a_ReadRxData[dataCounter+3];
			  }
			  x_state = WAIT_CHKSM;
			  break;
		  }
		  case WAIT_CHKSM:
		  {
			  //check if checksum is the same with checksum frame of XMODEM
			  sum = SumOfArray(a_ActualData,128);
			  checksum = sum%256;
			  if(checksum == a_ReadRxData[131])
			  {
				  if(X_SOH == a_ReadRxData[1])
				  {
					  //first check if it a newer version
					 if(RESET == versionAlreadyChecked)
					 {
						 uint32_t new_ver = 0;
						//get version of the new firmware from a_ActualData[]
						for (int i = 0; i < 4; i++)
						{
							new_ver |= (uint32_t)(a_ActualData[i]) << (8 * i);
						}
						//finally check if it is applicable
						versionApplicable = is_newer_version(new_ver);
						versionAlreadyChecked = SET;
					 }
					 if(true == versionApplicable)
					 {
						 //check if sector erase is needed
						 if((currentAddress & 0xFFFFC000) != ((currentAddress + 128) & 0xFFFFC000))
						 {
							 LED_Control(ORANGE, ENABLE);
							 FLASH_EraseSector(currentAddress + 0x80);
							 LED_Control(ORANGE, DISABLE);
						 }
						 //store first byte to temporary firmware address
						 LED_Control(GREEN, ENABLE);
						 FLASH_Write(a_ActualData, 128, FLASH_TEMP_START_ADDRESS);
						 LED_Control(GREEN, DISABLE);
						 memset(rxCB.data_buff, 0, sizeof rxCB.data_buff);
						 memset(a_ReadRxData, 0, sizeof rxCB.data_buff);
						 PacketReceived = SET;
						 copyData = SET;
						 x_state = WAIT_SOH;
						 currentAddress = currentAddress + 128;
					 }
					 else
					 {
						 CircularBuffer_Write(&txCB,X_CAN);
						 CircularBuffer_Transmit(&txCB);
						 CircularBuffer_Write(&txCB,X_CAN);
						 CircularBuffer_Transmit(&txCB);
						 CircularBuffer_Write(&txCB,X_CAN);
						 CircularBuffer_Transmit(&txCB);
						 USART_SendString((uint8_t*)"Newer version is already installed! Doing regular booting...\n\r\n\r");
						 FLASH_Jump();
					 }
				  }
				  if(a_ReadRxData[1] > X_SOH)
				  {
					  if(true == versionApplicable)
					  {
						  //check if sector erase is needed
						  if((currentAddress & 0xFFFFC000) != ((currentAddress + 128) & 0xFFFFC000))
						  {
							  LED_Control(ORANGE, ENABLE);
							  FLASH_EraseSector(currentAddress + 128);
							  LED_Control(ORANGE, DISABLE);
						  }
						  //store rest of data to temporary firmware address
						  LED_Control(GREEN, ENABLE);
						  FLASH_Write(a_ActualData, 128, FLASH_TEMP_START_ADDRESS+((rxCB.data_buff[1]-1)*128));
						  LED_Control(GREEN, DISABLE);
						  memset(rxCB.data_buff, 0, sizeof rxCB.data_buff);
						  memset(a_ReadRxData, 0, sizeof rxCB.data_buff);
						  PacketReceived = SET;
						  copyData = SET;
						  x_state = WAIT_SOH;
						  currentAddress = currentAddress + 128;
					  }
				  }
			  }
			  break;
		  }
	}
}


/*
 * helper API to find checksum of the array
 */
uint8_t SumOfArray(uint8_t array[], uint16_t length)
{
	int sum = 0;
	// Iterate through all elements and add them to sum
	for (int i = 0; i < length; i++)
		sum += array[i];
	return sum;
}


/*
 * message expression for terminal
 */
void ShowMessage(MESSAGE_t msg)
{
	switch(msg)
	{
		case MSG_WELCOME:
		{	USART_SendString((uint8_t*)"             @@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@@@@@@@\n\r\n\r");
			USART_SendString((uint8_t*)"             @@@@@@@@@@@@@@@@@  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\r\n\r");
			USART_SendString((uint8_t*)"             @@@@@@@@@@@@@@@   @@@@@@@         @@@@@@@@@@@@@@\n\r\n\r");
			USART_SendString((uint8_t*)"             @@@@@@@@@@@@@     @@@@@@@@  @@@@@@@@@@@@@@@@@@@@\n\r\n\r");
			USART_SendString((uint8_t*)"             @@@@@@@@@@@@ @@@  (@@@@@@  @@@@@@@@@@@@@@@@@@@@@\n\r\n\r");
			USART_SendString((uint8_t*)"             @@@@@@@@@@@@@@@@   @@@@/  @@@@@@@&         @@@@@\n\r\n\r");
			USART_SendString((uint8_t*)"             @@@@@@@@@@@@@@@@   @@@&  @@@@@     @@@@@@@@ @@@@\n\r\n\r");
			USART_SendString((uint8_t*)"             @@@@@@@@@@@@@@@@@   @   @@@.    &@@@@@@@@@@@@@@@\n\r\n\r");
			USART_SendString((uint8_t*)"             @@@@@@@@@@@@@@@@@             @@@              @@\n\r\n\r");
			USART_SendString((uint8_t*)"             @@@@@@@@@@@@@@@@@   @@@@@          @@@@@@@@@@@ @\n\r\n\r");
			USART_SendString((uint8_t*)"             @@@@@@@@@@@@@@@@@@@@@@@.%@  @@@@@  @@@@@@@@@@@@@\n\r\n\r");
			USART_SendString((uint8_t*)"             @@@@@@@@@@@@@@@@@@              @@@@@@@@@@@@@@@@\n\r\n\r");
			USART_SendString((uint8_t*)"             @ @@@@@@@@@@@@@@                  @@@@@@@@@@@@@@\n\r\n\r");
			USART_SendString((uint8_t*)"             @@  @@@@@@@@@                  @@@@@@@@@@@@@@@@@\n\r\n\r");
			USART_SendString((uint8_t*)"             @@@@  @@@    @@@@@@@&         .@@@@@@@@@@@@@@@@@\n\r\n\r");
			USART_SendString((uint8_t*)"             @@@@@@@#   ###@@@@( @        &@@@@@@@@@@@@@@@@@@\n\r\n\r");
			USART_SendString((uint8_t*)"             @@@@@@@@@@@@@@@#     @@     (@     @@@@@@@@@@@@@\n\r\n\r");
			USART_SendString((uint8_t*)"             @@@@@@@@@@@@@@     @@@@     @@     @@@@@@@@@@@@@\n\r\n\r");
			USART_SendString((uint8_t*)"             @@@@@@@@@@@&     @@@@@@/   @@@@@@    @@@@@@@@@@@\n\r\n\r");
			USART_SendString((uint8_t*)"             @@@@@@@@@@@*    @@@@@@@@  @@@@@@@@      @@@@@@@@\n\r\n\r");
			USART_SendString((uint8_t*)"             @@@@@@@@@@@      @@@@@@@  @@@@@@@@   %%  @@@@@@@@\n\r\n\r");
			USART_SendString((uint8_t*)"             @@@@@@@@@@@@       /&@@@  @@@@@@&   @ @@@@@@@@@@\n\r\n\r");
			USART_SendString((uint8_t*)"             @@@@@@@@@@@@@@&  ,@@@@@@@@@@@@  @ @@@@@@@@@@@@@@\n\r\n\r");
			USART_SendString((uint8_t*)"             @@@@@@@@@@@@@@@@@@  @@@@@@@@@@@%@@@@@@@@@@@@@@@@\n\r\n\r");

			USART_SendString((uint8_t*)"             >>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<\n\r\n\r");
			USART_SendString((uint8_t*)"                       Welcome to XMODEM bootloader!\n\r");
			USART_SendString((uint8_t*)"                             by destrocore\n\r");
			USART_SendString((uint8_t*)"             >>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<\n\r\n\r");
			break;
		}
		case MSG_JUMP:
		{
			USART_SendString((uint8_t*)"Jumping to user application...\n\r");
			break;
		}
		case MSG_UPDT_IN_PRGS:
		{
			USART_SendString((uint8_t*)"Update in progress! Please wait until the operation is over!\n\r");
		}
		case MSG_ASK_FOR_A_FILE:
		{
			USART_SendString((uint8_t*)"Please send a new binary file with XMODEM protocol to update the firmware.\n\r");
			break;
		}
		default:
			break;
	}
}

/*
 * local USART configuration function
 */
void USART_Config(void)
{
	GPIO_Handle_t USART_GPIO_h = {0};

	USART_GPIO_h.pGPIOx = GPIOB;
	USART_GPIO_h.GPIO_Config.PinMode = GPIO_MODE_AF;
	USART_GPIO_h.GPIO_Config.PinAltFuncMode = 7;
	USART_GPIO_h.GPIO_Config.PinOPType = GPIO_OUTPUT_PP;
	USART_GPIO_h.GPIO_Config.PinPuPdControl = GPIO_PIN_NO_PUPD;
	USART_GPIO_h.GPIO_Config.PinSpeed = GPIO_SPEED_HIGH;

	//TX
	USART_GPIO_h.GPIO_Config.PinNumber = GPIO_PIN_6;
	GPIO_Init(&USART_GPIO_h);

	//RX
	USART_GPIO_h.GPIO_Config.PinNumber = GPIO_PIN_7;
	USART_GPIO_h.GPIO_Config.PinOPType = GPIO_MODE_INPUT;
	GPIO_Init(&USART_GPIO_h);

	//Peripheral Configuration
	USART.pUSARTx = USART1;
	USART.USART_Config.USART_Baud = USART_BAUD_115200;
	USART.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
	USART.USART_Config.USART_Mode = USART_MODE_TXRX;
	USART.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
	USART.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
	USART.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;

	USART_Init(&USART);

	USART.pUSARTx->CR1.bit.rxneie = ENABLE;
	USART.pUSARTx->CR3.bit.eie = ENABLE;

	USART_IRQInterruptConfig(USART1_IRQn, 14, ENABLE);
}


/*
 * local green and orange LEDs configuration function
 */
void LED_Config(void)
{
	GPIO_Handle_t LED_GPIO_h = {0};

	LED_GPIO_h.pGPIOx = GPIOD;
	LED_GPIO_h.GPIO_Config.PinMode = GPIO_MODE_OUTPUT;
	LED_GPIO_h.GPIO_Config.PinOPType = GPIO_OUTPUT_PP;
	LED_GPIO_h.GPIO_Config.PinPuPdControl = GPIO_PIN_PULL_DOWN;
	LED_GPIO_h.GPIO_Config.PinSpeed = GPIO_SPEED_HIGH;

	//Green
	LED_GPIO_h.GPIO_Config.PinNumber = GPIO_PIN_12;
	GPIO_Init(&LED_GPIO_h);

	//Orange
	LED_GPIO_h.GPIO_Config.PinNumber = GPIO_PIN_13;
	GPIO_Init(&LED_GPIO_h);

	//Red
	LED_GPIO_h.GPIO_Config.PinNumber = GPIO_PIN_14;
	GPIO_Init(&LED_GPIO_h);

	//Blue
	LED_GPIO_h.GPIO_Config.PinNumber = GPIO_PIN_15;
	GPIO_Init(&LED_GPIO_h);
}

/*
 * user button configuration function
 */
void UserButton_Config(void)
{
	GPIO_Handle_t Button_h = {0};

	Button_h.pGPIOx = GPIOA;
	Button_h.GPIO_Config.PinMode = GPIO_MODE_INPUT;
	Button_h.GPIO_Config.PinPuPdControl = GPIO_PIN_PULL_DOWN;
	Button_h.GPIO_Config.PinSpeed = GPIO_SPEED_HIGH;
	Button_h.GPIO_Config.PinNumber = GPIO_PIN_0;
	GPIO_Init(&Button_h);
}

/*
 * Function to control on-board LEDs
 */
void LED_Control(LED_Color_t led, uint8_t state)
{
	switch(led)
	{
		case GREEN:
		{
			if(ENABLE == state)
			{
				GPIOD->ODR.bit.odr_12 = ENABLE;
			}
			else GPIOD->ODR.bit.odr_12 = DISABLE;
			break;
		}
		case ORANGE:
		{
			if(ENABLE == state)
			{
				GPIOD->ODR.bit.odr_13 = ENABLE;
			}
			else GPIOD->ODR.bit.odr_13 = DISABLE;
			break;
		}
		case RED:
		{
			if(ENABLE == state)
			{
				GPIOD->ODR.bit.odr_14 = ENABLE;
			}
			else GPIOD->ODR.bit.odr_14 = DISABLE;
			break;
		}
		case BLUE:
		{
			if(ENABLE == state)
			{
				GPIOD->ODR.bit.odr_15 = ENABLE;
			}
			else GPIOD->ODR.bit.odr_15 = DISABLE;
			break;
		}
	}
}

/*
 * simple USART API to print string in terminal
 */
void USART_SendString(uint8_t str[])
{
	uint8_t i = 0;
	while(0 != str[i])
	{
		//fill TX circular buffer with data to print
		CircularBuffer_Write(&txCB, str[i]);
		i++;
	}
	//print the data that is inside the TX circular buffer
	CircularBuffer_Transmit(&txCB);
	delay_ms(10);
}

/*
 * simple USART API to write directly to data register FIFO
 */
void USART_Write(uint8_t ch)
{
	USART.pUSARTx->DR.bit.dr = ch;
}


/*
 * USART IRQ handler is required for using circular buffer
 */
void USART1_IRQHandler(void)
{
	//if RX is not empty and RX interrupt is enabled
	if((SET == USART.pUSARTx->SR.bit.rxne) && (ENABLE == USART.pUSARTx->CR1.bit.rxneie))
	{
		//store data from RX FIFO to RX ring buffer so data is not lost
		CircularBuffer_Write(&rxCB, USART.pUSARTx->DR.bit.dr);
	}
	//if TX is empty and TX interrupt is enabled
	if((SET == USART.pUSARTx->SR.bit.txe) && (ENABLE == USART.pUSARTx->CR1.bit.txeie))
	{
		TxInProgress = RESET;

		//check if TX has any data and if it does save it to temp, then print temp and turn on TXEIE
		CircularBuffer_Transmit(&txCB);
	}
}

/*
 * Copy data to the user application address
 */
void UpdateUserApplication(void)
{
    // Read data from FLASH_TEMP_START_ADDRESS
    uint32_t* src_addr = (uint32_t*)FLASH_TEMP_START_ADDRESS;

    // Erase the sector at FIRMWARE_VERSION_ADDRESS
    FLASH_EraseSector(FIRMWARE_VERSION_ADDRESS);

    uint32_t data_length = 0;
	while (data_length < (16 * 1024) && src_addr[data_length] != 0xFF)
	{
		data_length++;
	}

    // Write data to FIRMWARE_VERSION_ADDRESS
	FLASH_Write((uint8_t *)src_addr, data_length, FIRMWARE_VERSION_ADDRESS);


	//erase temporary file
    FLASH_Erase(FLASH_TEMP_START_ADDRESS);

    USART_SendString((uint8_t*)"Update is over! Booting now!\n\r\n\r");


    //Copy operation is over. Can do the jump.
    FLASH_Jump();
}


/*
 * Helper API to read firmware version from memory
 */
uint32_t get_firmware_current_version()
{
    return *((uint32_t*)FIRMWARE_VERSION_ADDRESS);
}

/*
 * Helper API to check if new version should be uploaded into device's memory or not
 */
bool is_newer_version(uint32_t ver)
{
	uint32_t current_version = get_firmware_current_version();

	if(0xFFFFFFFF ==  current_version)
	{
		//no firmware on device
		return true;
	}
	if (ver > current_version)
	{
	    //new version is applicable
		return true;

	}
	else
	{
	    //version doesn't fit the requirement
	    return false;
	}

}

void SystemClock_Config(void)
{
	RCC_OscInit_t Osc = {0};
	RCC_ClkInit_t Clk = {0};


	//osc init
	Osc.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	Osc.HSEState = RCC_HSE_ON;
	Osc.PLL.State = RCC_PLL_ON;
	Osc.PLL.Source = RCC_PLLCFGR_PLLSRC_HSE;
	Osc.PLL.M = 4;
	Osc.PLL.N = 168;
	Osc.PLL.P = 0;
	Osc.PLL.Q = 4;
	RCC_OscConfig(&Osc);

	//clk init
	Clk.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
            |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	Clk.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	Clk.AHBCLKDivider = RCC_SYSCLK_DIV1;
	Clk.APB1CLKDivider = RCC_HCLK_DIV4;
	Clk.APB2CLKDivider = RCC_HCLK_DIV2;

	RCC_ClockConfig(&Clk);
}
