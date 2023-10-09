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
CircularBuffer_t rxCB, txCB;
XMODEM_State_t x_state;
uint8_t rx_byte = 0, prev_index1 = 0, prev_index2 = 0xFF, dataCounter = 0, checksum = 0;
uint8_t a_ReadRxData[132] = "\0";
uint8_t a_ActualData[128] = "\0";
uint8_t sum;
uint32_t sectcount;

volatile uint32_t SystemCoreClock = ((uint32_t)16000000U);

typedef void (*fnc_ptr)(void);		//function pointer for jumping to user application

uint32_t currentAddress = FLASH_TEMP_START_ADDRESS;

const uint32_t a_flash_sectors[] = {
  //512 kB
  16,  //sector 0
  16,  //sector 1
  16,  //sector 2
  16,  //sector 3
  64,  //sector 4
  128, //sector 5
  128, //sector 6
  128, //sector 7
  //1024 kB
  128, //sector 8
  128, //sector 9
  128, //sector 10
  128  //sector 11
};
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
	FLASH_Init();

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
	//Initialize necessary GPIO first

	//enable GPIOB for USART1
	RCC->AHB1ENR.bit.gpioben = ENABLE;
	//alternate mode for pin 6 and 7
	GPIOB->MODER.bit.moder_6 = 0x2;	//tx
	GPIOB->MODER.bit.moder_7 = 0x2;	//rx

	// enable SYSCFG peripheral clock
	RCC->APB2ENR.bit.syscfgen = ENABLE;

	GPIOB->OSPEEDR.bit.ospeedr_6 = 0x3;
	GPIOB->OSPEEDR.bit.ospeedr_7 = 0x3;

	//alternate functioning mode
	GPIOB->AFRL.bit.afrx_6 = 0x7;
	GPIOB->AFRL.bit.afrx_7 = 0x7;

	//Initialize USART1

	USART_CR1_Reg_t CR1_temp;

	//Enbale USART1 clock
	RCC->APB2ENR.bit.usart1en = ENABLE;

	//configure CR1
	CR1_temp.bit.te = ENABLE;
	CR1_temp.bit.re = ENABLE;
	USART1->CR1.reg = CR1_temp.reg;

	//set USART baudrate to 115200
	USART1->BRR.reg = 0x2D9;\

	//enable USART1 peripheral
	USART1->CR1.bit.ue = ENABLE;

	USART1->CR1.bit.rxneie = ENABLE;
	USART1->CR3.bit.eie = ENABLE;

	//enable USART1 IRQ
	NVIC->ISER[1] = 0x20;
	NVIC->ICER[1] = 0x20;
	*(NVIC_IPR_BASE + (USART1_IRQn/4) ) |= (14 << 12);

	NVIC_EnableIRQ(USART1_IRQn);

}


/*
 * local green and orange LEDs configuration function
 */
void LED_Config(void)
{
	//Enable GPIOD clock first
	RCC->AHB1ENR.bit.gpioden = ENABLE;

	//Select GPIOD mode for LED pins
	GPIOD->MODER.reg = 0x55;

	//GPIOD speed for LED pins
	GPIOD->OSPEEDR.reg = 0xFF;

	//GPIOD pull down select
	GPIOD->PUPDR.reg = 0xAA;
}

/*
 * user button configuration function
 */
void UserButton_Config(void)
{

	//Enable GPIOA clock first
	RCC->AHB1ENR.bit.gpioaen = ENABLE;

	//Select GPIOA input mode for button pin
	GPIOA->MODER.bit.moder_0 = 0x0;

	//GPIOA speed for button pin
	GPIOA->OSPEEDR.bit.ospeedr_0 = 0x3;

	//GPIOA pull down select
	GPIOA->PUPDR.bit.pupdr_0 = 0x2;
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
	USART1->DR.bit.dr = ch;
}


/*
 * USART IRQ handler is required for using circular buffer
 */
void USART1_IRQHandler(void)
{
	//if RX is not empty and RX interrupt is enabled
	if((SET == USART1->SR.bit.rxne) && (ENABLE == USART1->CR1.bit.rxneie))
	{
		//store data from RX FIFO to RX ring buffer so data is not lost
		CircularBuffer_Write(&rxCB, USART1->DR.bit.dr);
	}
	//if TX is empty and TX interrupt is enabled
	if((SET == USART1->SR.bit.txe) && (ENABLE == USART1->CR1.bit.txeie))
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

    LED_Control(ORANGE, ENABLE);

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

/*
 * FLASH initialization
 */
void FLASH_Init(void)
{
	FLASH->OPTKEYR = 0x08192A3B;				//OPTKEY1 = 0x08192A3B

	FLASH->OPTKEYR = 0x4C5D6E7F;				//OPTKEY2 = 0x4C5D6E7F

	//set latency
	FLASH->ACR.bit.latency = 5;

	//relock OPTCR
	FLASH->OPTCR.bit.optlock = ENABLE;

	//unlock
	while(RESET != FLASH->SR.bit.bsy)
	{
		//wait for the flash memory not to be busy
	}
	//extra lock check
	if(SET == FLASH->CR.bit.lock)
	{
		FLASH->KEYR = 0x45670123;							//KEY1 = 0x45670123
		FLASH->KEYR = 0xCDEF89AB;							//KEY2 = 0xCDEF89AB
	}

	//set block size
	FLASH->CR.bit.psize = 1 >> 1;

	//lock
	FLASH->CR.bit.lock = ENABLE;;

	sectcount = 12;									//12 sectors for stm32f407
}

/*
 * Full erase FLASH beginning from specific address
 */
void FLASH_Erase(uint32_t destination)
{
	sectcount = 12;
	int i, k;
	uint32_t flash_base_addr = FLASH_BASE;

	if(RESET != FLASH->SR.bit.bsy)
	{
		//flash memory operation is in progress
		return;
	}

	for(i = 0; i < sectcount; i++)
	{
		//search for the sector
		if(flash_base_addr == destination)
		{
			break;
		}
		else if(flash_base_addr > destination)
		{
			//not a sector beginning address
			return;
		}
		flash_base_addr = flash_base_addr + (a_flash_sectors[i]<<10);
	}
	if(i == sectcount)
	{
		//not found in a_flash_sectors[]
		return;
	}

	//FLASH unlock
	while(RESET != FLASH->SR.bit.bsy)
	{
		//wait for the flash memory not to be busy
	}
	//extra lock check
	if(SET == FLASH->CR.bit.lock)
	{
		FLASH->KEYR = 0x45670123;							//KEY1 = 0x45670123
		FLASH->KEYR = 0xCDEF89AB;							//KEY2 = 0xCDEF89AB
	}

	//light up orange on-board LED
	GPIOD->ODR.bit.odr_13 = ENABLE;

	//set sector erase
	FLASH->CR.bit.ser = ENABLE;

	k = i;
	for(i = k; i<12; i++)
	{
		//set sector index (SNB)
		FLASH->CR.bit.snb = i;

		//start the erase
		FLASH->CR.bit.strt = ENABLE;

		while(RESET != FLASH->SR.bit.bsy)
		{
			//wait until erase complete
		}
	}

	//FLASH lock
	FLASH->CR.bit.lock = ENABLE;
}

/*
 * FLASH erase sector function
 */
uint32_t FLASH_EraseSector(uint32_t destination)
{

	int i;	//sector number index
	uint32_t addr = FLASH_BASE;

	if(RESET != FLASH->SR.bit.bsy)
	{
		//flash memory operation is in progress
		return FLASH_ERR_BUSY;
	}

	for(i = 0; i < sectcount; i++)
	{
		//search for the sector
		if(addr == destination)
		{
			break;
		}
		else if(addr > destination)
		{
			//not a sector beginning address
			return FLASH_ERR_WRONG_ADDR;
		}
		addr = addr + (a_flash_sectors[i]<<10);
	}
	if(i == sectcount)
	{
		//not found in a_flash_sectors[]
		return FLASH_ERR_INVALID_SECTOR;
	}

	//unlock FLASH
	while(RESET != FLASH->SR.bit.bsy)
	{
		//wait for the flash memory not to be busy
	}
	//extra lock check
	if(SET == FLASH->CR.bit.lock)
	{
		FLASH->KEYR = 0x45670123;							//KEY1 = 0x45670123
		FLASH->KEYR = 0xCDEF89AB;							//KEY2 = 0xCDEF89AB
	}

	//set sector erase
	FLASH->CR.bit.ser = ENABLE;

	//set sector index (SNB)
	FLASH->CR.bit.snb = i;

	//start the erase
	FLASH->CR.bit.strt = ENABLE;

	while(RESET != FLASH->SR.bit.bsy)
	{
		//wait until erase complete
	}

	//sector erase flag does not clear automatically
	FLASH->CR.bit.ser = DISABLE;

	//lock FLASH
	FLASH->CR.bit.lock = ENABLE;

	//return kB in this sector
	return (a_flash_sectors[i]<<10);
}

/*
 * FLASH Write function
 */
uint8_t FLASH_Write(uint8_t *sourcedata, uint32_t len, uint32_t destination)
{
	//get current element's size from command register
	uint32_t blocksize = FLASH->CR.bit.psize;

	uint32_t offset;

	blocksize = 1 << blocksize;

	//size control
	if(0 != (len & (blocksize-1)))
	{
		//length doesn't fit any block size format
		return 1;
	}

	//FLASH unlock
	while(RESET != FLASH->SR.bit.bsy)
	{
		//wait for the flash memory not to be busy
	}
	//extra lock check
	if(SET == FLASH->CR.bit.lock)
	{
		FLASH->KEYR = 0x45670123;							//KEY1 = 0x45670123
		FLASH->KEYR = 0xCDEF89AB;							//KEY2 = 0xCDEF89AB
	}

	for(offset = 0; offset < len; offset = offset + blocksize)
	{
		FLASH->CR.bit.ser = DISABLE;
		FLASH->CR.bit.pg  = ENABLE;;

		switch(blocksize)
		{
			case DATA_TYPE_8B:
			{
				//write 8 bit
				*((volatile uint8_t*)destination) = *sourcedata;
				break;
			}
			case DATA_TYPE_16B:
			{
				//write 16 bit
				*((volatile uint16_t*)destination) = *(uint16_t*)sourcedata;
				break;
			}
			case DATA_TYPE_32B:
			{
				//write 32 bit
				*((volatile uint32_t*)destination) = *(uint32_t*)sourcedata;
				break;
			}
			case DATA_TYPE_64B:
			{
				//write 64 bit
				*((volatile uint64_t*)destination) = *(uint64_t*)sourcedata;
				break;
			}
			default:
				return FLASH_ERR_WRITE;
		}
		while(RESET != FLASH->SR.bit.bsy)
		{
			//wait until complete programming
		}
		sourcedata  = sourcedata  + blocksize;
		destination = destination + blocksize;
		}

	//FLASH lock
	FLASH->CR.bit.lock = ENABLE;

	return 0;
}

/*
 * Jump to user application
 */
void FLASH_Jump(void)
{
	fnc_ptr jump_to_app;
	jump_to_app = (fnc_ptr)(*(volatile uint32_t*) (FLASH_APP_START_ADDRESS+4U));
	//Reset of all peripherals
	RCC->APB1RSTR.reg = 0xFFFFFFFFU;
	RCC->APB1RSTR.reg = RESET;

	RCC->APB2RSTR.reg = 0xFFFFFFFFU;
	RCC->APB2RSTR.reg = RESET;

	RCC->AHB1RSTR.reg = 0xFFFFFFFFU;
	RCC->AHB1RSTR.reg = RESET;

	RCC->AHB2RSTR.reg = 0xFFFFFFFFU;
	RCC->AHB2RSTR.reg = RESET;

	RCC->AHB3RSTR.reg = 0xFFFFFFFFU;
	RCC->AHB3RSTR.reg = RESET;
	SysTick->CTRL = 0; //disable SysTick
	SCB->VTOR = FLASH_APP_START_ADDRESS;
	//change the main stack pointer
	__set_MSP(*(volatile uint32_t*)FLASH_APP_START_ADDRESS);
	__set_PSP(*(volatile uint32_t*)FLASH_APP_START_ADDRESS);
	jump_to_app();
}


/*
 * TIM1 for delays
 */
void TIM1_Config(void)
{
	RCC->APB2ENR.bit.tim1en = ENABLE;  	//TIM1 EN
	TIM1->PSC = 167; 					// ~1 uS delay
	TIM1->ARR |= 0xFFFF; 				//all bits set to 1
	TIM1->CR1.bit.cen = ENABLE; 		//timer counter EN

	while(SET != TIM1->SR.bit.uif)
	{
		//Update interrupt pending. This bit is set by hardware when the registers are updated
	}
}

void delay_us(int micro)
{
	TIM1->CNT = 0x0000;  				//reset counter register
	while(TIM1->CNT < micro);
}

void delay_ms(int ms)
{
	int i;
	for (i = 0; i < ms; i++)
	{
		delay_us (1000); 			//1 ms delay
	}
}

void SystemClock_Config(void)
{

	//RCC OSC config first

	//turn on HSE
	RCC->CR.bit.hseon = RCC_CR_HSEON;

	//configure PLL
	RCC->PLLCFGR.bit.pllsrc = RCC_PLLCFGR_PLLSRC_HSE;
	RCC->PLLCFGR.bit.pllm = 0x4;
	RCC->PLLCFGR.bit.pllp = 0x0;  //div by 2
	RCC->PLLCFGR.bit.plln = 168;
	RCC->PLLCFGR.bit.pllq = 0x4;
	RCC->CR.bit.pllon = RCC_CR_PLLON;

	//RCC CLK init

	// Set flash latency based on new system clock frequency
	FLASH->ACR.bit.latency = 0x5U; //5WS for 150 < HCLK ≤ 168
	while(!(FLASH->ACR.bit.latency = 0x5U))
	{

	}

	//HCLK Configuration
	RCC->CFGR.bit.ppre1 = 0x7U;
	RCC->CFGR.bit.ppre2 = 0x7U;
	RCC->CFGR.bit.hpre  = 0x0U;

	//SYSCLK Configuration
	//wait PLL ready flag
	while(!(SET == RCC->CR.bit.pllrdy))
	{

	}

	//system source clock selected as PLL
	RCC->CFGR.bit.sw = 0x2U;

	//PCLK1 Configuration
	RCC->CFGR.bit.ppre1 = 0x5U; //div 4

	//PCLK2 Configuration
	RCC->CFGR.bit.ppre2 = 0x4U; //div 2

	//Update the SystemCoreClock global variable
	SystemCoreClock = 168000000;
}



