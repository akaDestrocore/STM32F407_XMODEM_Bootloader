#include "main.h"
#include <xmodem.h>
#include <updater_flash.h>
#include <janpatch.h>
#include <mbedtls.h>
#include <mbedtls/gcm.h>
#include <uart_log.h>

/*
 * Structure of application binary header
 */
typedef struct __attribute__((packed))
{
	uint32_t signature0;
	uint8_t signature1;
	uint8_t version_major;
	uint8_t version_minor;
	uint8_t version_patch;
}image_version_t;

uint8_t pKeyAES[16] = { 0xAC,0x00,0xD6,0x7F,0x21,0xD2,0x94,0x46,0x2F,0x2A,
						0xB6,0x84,0xE2,0xE7,0x83,0xF7 };

uint8_t nonce_counter[12] = { 0x76,0x4c,0x10,0x12,0x61,0x75,0xc3,0xa1,0xc3,0x94,0x5f,0x06 };

uint8_t HeaderAES[16] = { 0x02,0x00,0x07,0x07,0x60,0x61,0x5F,0x6B,0x65,0x73,
							0x01,0x00,0x00,0x01,0xFF,0xFF };

/* Private define ------------------------------------------------------------*/
uint8_t iv_key[16];
unsigned char pTagGCM[16];

UART_HandleTypeDef huart;
uint8_t FullOrPatch = UPDATER_STATE_UPDATE_PATCH;
unsigned char calculated_tag[16] = { 0 };

Ring_Buffer_t rxBuff, txBuff;

// initialize variables and arrays needed for XMODEM protocol
uint8_t rx_byte = 0, prev_index1 = 0, prev_index2 = 0xFF, dataCounter = 0, checksum = 0;
uint32_t total_packet_count = 0;
uint8_t a_ReadRxData[133] = { 0 };
uint8_t a_EncryptedData[128] = { 0 };
uint8_t a_DecryptedData[128] = { 0 };
uint8_t a_SendData[133] = { 0 };
int crc_received = 0, crc_calculated = 0;
uint32_t currentAddress = RESET;

#define 	BUFFER_SIZE      	 2048

uint32_t file_size = 0, total_encrypted_length = 0, total_packets = 0,
remaining_encrypted_length = 0, remaining_packets = 0, remaining_bytes_in_last_packet = 0;

uint8_t first_packet_complete = 0;

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
mbedtls_gcm_context aes;

// flag to indicate that transmission is in progress (needed to use ring buffers efficiently)
uint8_t TxInProgress = RESET;
// initial state of XMODEM state holder
XMODEM_State_t x_state = WAIT_SOH;
// holder for user selection
uint8_t UpdaterOptionSelected = RESET;
// flags required for packet processing
uint8_t PacketReceived = RESET;
uint8_t copyData = SET;
uint8_t firstPacket = RESET;
//flags required for packet transmission (from MCU to host)
uint8_t blockNum = 1;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void decrypt_rest_of_firmware(void);
uint32_t extract_size(uint8_t* file);
void clear_screen(void);
void set_cursor_position(int x, int y);
void load_backup(void);
void prepare_header(void);
void ShowMessage(MESSAGE_t msg);
void Read_Key(void);
void Download_Patch(void);
void DownloadUserApplication(void);
void UpdateUserApplication(void);
void JumpToAppLoader(void);
void Download_Firmware(uint8_t* pUploadAddress);
void ApplyPatch(void);
void gcm_begin_encryption(uint8_t* src, size_t len);
void X_SendCan(void);
void SelectUpdateMethod(void);
void X_SendPacket(uint8_t* packet);

/*
 * initialization of MBEDTLS context
 */
void mbedTLS_AES_Init(void)
{
	mbedtls_gcm_init(&aes);

	if (HAL_OK != mbedtls_gcm_setkey(&aes, MBEDTLS_CIPHER_ID_AES, pKeyAES, 128))
	{
		// Handle error
		Error_Handler();
	}
}

int main(void)
{

	RingBuffer_Init(&rxBuff);
	RingBuffer_Init(&txBuff);
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_USART2_UART_Init();
	MX_MBEDTLS_Init();
	mbedTLS_AES_Init();

	memcpy(&huart, &huart2, sizeof(huart));
	__HAL_UART_ENABLE_IT(&huart, UART_IT_ERR);
	__HAL_UART_ENABLE_IT(&huart, UART_IT_RXNE);
	ShowMessage(PRINT_OPTIONS);

	while (1)
	{
		// constantly read user input
		if (RESET == UpdaterOptionSelected)
		{
			// Updater option is selected in this function
			Read_Key();
		}
		else if (UPDATER_STATE_UPLOAD == UpdaterOptionSelected)
		{
			rx_byte = 0;
			// select update method
			SelectUpdateMethod();
		}
		else if (UPDATER_STATE_UPDATE_FULL == UpdaterOptionSelected)
		{
			// full update
			Download_Firmware((uint8_t*)SLOT_2_VER_ADDR);
		}
		else if (UPDATER_STATE_UPDATE_PATCH == UpdaterOptionSelected)
		{
			// patching
			Download_Firmware((uint8_t*)PATCH_ADDR);
		}
		else if (UPDATER_STATE_DOWNLOAD == UpdaterOptionSelected)
		{
			// upload current application firmware to host using XMODEM protocol
			while (X_C != rx_byte)
			{
				RingBuffer_Read(&rx_byte, &rxBuff);
			}
			DownloadUserApplication();
		}
	}
}

void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 90;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
		| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}
}

static void MX_USART2_UART_Init(void)
{


	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_Handler();
	}

}

static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	HAL_GPIO_WritePin(GPIOD, GREEN_Pin | ORANGE_Pin | RED_Pin | BLUE_Pin, GPIO_PIN_RESET);

	GPIO_InitStruct.Pin = GREEN_Pin | ORANGE_Pin | RED_Pin | BLUE_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/*
 * update firmware using XMODEM CRC 16 bit
 */
 /* Each received packet takes 128 bytes of data and looks like this

  *      Each packet looks like this:
  *      +-----+-------+-------+------+-----+
  *      | SOH | Idx1. | Idx2. | data | SUM |
  *      +-----+-------+-------+------+-----+
  *      SOH  = 0x01
  *      Idx1 = The sequence number.
  *      Idx2 = The complement of the sequence number.
  *      Data = A 128 bytes of data.
  *      SUM  = Add the contents of the 128 bytes and use the low-order
  *             8 bits of the result.
  *
  * @Note	- Data is received in 128 byte packets. Each 128 byte packet is being de-crypted before
  * 				writing to flash. After receiving whole binary, 'UpdateUserApplication()' is called
  */
void Download_Firmware(uint8_t* pUploadAddress)
{
	// Spam 'C' character to start the communication
	if ((RESET == PacketReceived) && (RESET == firstPacket))
	{
		RingBuffer_Write(&txBuff, X_C);
		RingBuffer_Transmit(&txBuff);
		HAL_Delay(3000);
	}

	// Send NAK if packet wasn't received
	if ((RESET == PacketReceived) && (SET == firstPacket))
	{
		RingBuffer_Write(&txBuff, X_NAK);
		RingBuffer_Transmit(&txBuff);
		HAL_Delay(50);
	}

	if (WAIT_SOH == x_state)
	{
		if (SET == PacketReceived)
		{
			RingBuffer_Write(&txBuff, X_ACK);
			RingBuffer_Transmit(&txBuff);
		}

		if (rxBuff.dataCount >= 133)
		{
			firstPacket = SET;
		}
	}

	RingBuffer_Read(&rx_byte, &rxBuff);

	// end of data transmission indicator
	if (X_EOT == rxBuff.data_buff[0])
	{
		// End the communication with host by sending ACK if EOT byte was received
		RingBuffer_Write(&txBuff, X_ACK);
		RingBuffer_Transmit(&txBuff);

		uint8_t unauth = memcmp(pTagGCM, calculated_tag, 16);

		// TEST
		// unauth = 0;

		// Reinitialize all variables to the initial values
		rxBuff.head = RESET;
		rxBuff.tail = RESET;
		rxBuff.dataCount = RESET;
		firstPacket = RESET;
		copyData = SET;
		PacketReceived = RESET;
		prev_index1 = 0;
		prev_index2 = 0xFF;
		dataCounter = 0;
		checksum = 0;
		total_packets = 0;

		mbedtls_gcm_free(&aes);

		memset(calculated_tag, 0, sizeof(calculated_tag));
		memset(a_EncryptedData, 0, sizeof(a_EncryptedData));
		HAL_Delay(2000);
		clear_screen();
		set_cursor_position(0, 0);


		if (RESET == unauth)
		{
			if (UPDATER_STATE_UPDATE_FULL == FullOrPatch)
			{
				UpdateUserApplication();
			}

			else if (UPDATER_STATE_UPDATE_PATCH == FullOrPatch)
			{
				ApplyPatch();
			}
		}
		else
		{

			DBG("Received data couldn't be authentificated\n\r");
			HAL_Delay(1000);

			uint32_t data_length = 0;
			uint32_t* src_addr = (uint32_t*)pUploadAddress;

			while (src_addr[data_length] != 0xFFFFFFFF)
			{
				data_length++;
			}
			data_length = data_length * 4; // convert to bytes

			if (UPDATER_STATE_UPDATE_FULL == FullOrPatch)
			{
				FLASH_EraseSector(SLOT_2_APP_ADDR - 0x20000);
				FLASH_EraseSector(SLOT_2_APP_ADDR);
				FLASH_EraseSector(SLOT_2_APP_ADDR + 0x20000);

				load_backup();
			}
			else if (UPDATER_STATE_UPDATE_PATCH == FullOrPatch)
			{
				FLASH_EraseSector(PATCH_ADDR);
			}

			clear_screen();
			set_cursor_position(0, 0);
			rx_byte = 0;
			UpdaterOptionSelected = RESET;
			ShowMessage(PRINT_OPTIONS);
		}
	}

	if (SET == copyData)
	{
		//read one packet frame of XMODEM from circular buffer
		for (dataCounter = 0; dataCounter < 133; dataCounter++)
		{
			a_ReadRxData[dataCounter] = rxBuff.data_buff[dataCounter];
		}
		PacketReceived = RESET;
		//empty the ring buffer
		rxBuff.head = RESET;
		rxBuff.tail = RESET;
		rxBuff.dataCount = RESET;
	}

	// XMODEM switch case to parse the received packet
	switch (x_state)
	{
	case WAIT_SOH:
	{
		if (X_SOH == a_ReadRxData[0])
		{
			x_state = WAIT_INDEX1;
			PacketReceived = SET;
			copyData = RESET;
		}
		else PacketReceived = RESET;
		break;
	}
	case WAIT_INDEX1:
	{
		//if index is bigger than the previous packet's index by 1 than it's correct
		if ((prev_index1 + 1) == a_ReadRxData[1])
		{
			x_state = WAIT_INDEX2;
			prev_index1++;
		}
		else if ((prev_index1 + 1) == 256)
		{
			x_state = WAIT_INDEX2;
			prev_index1 = 0;
		}
		else
		{
			X_SendCan();

			// print error message to terminal to indicate index error

			ShowMessage(PRINT_IDX1_ERR);

			// reset option selection to go back to updater menu

			UpdaterOptionSelected = 0;

			rx_byte = 0;
		}
		break;
	}
	case WAIT_INDEX2:
	{
		//index 2 should be equal to the difference of 255 and index1
		if ((prev_index2 - 1) == a_ReadRxData[2])
		{
			x_state = READ_DATA;
			prev_index2--;
		}
		else if ((prev_index2 - 1) == -1)
		{
			x_state = READ_DATA;
			prev_index2 = 255;
		}
		else
		{
			X_SendCan();
			// print to indicate that index 2 caused the error
			ShowMessage(PRINT_IDX2_ERR);

			// reset selection to go back to updater menu
			UpdaterOptionSelected = 0;
			rx_byte = 0;
		}
		break;
	}
	case READ_DATA:
	{
		for (dataCounter = 0; dataCounter < 129; dataCounter++)
		{
			a_EncryptedData[dataCounter] = a_ReadRxData[dataCounter + 3];
		}
		x_state = WAIT_CRC;
		break;
	}
	case WAIT_CRC:
	{
		crc_received = (a_ReadRxData[131] << 8) | a_ReadRxData[132];
		crc_calculated = X_CRC16_Calculate(a_EncryptedData, 128);

		if (crc_received != crc_calculated)
		{
			// send CANCEL
			X_SendCan();

			// print to terminal to indicate that CRC error happened
			ShowMessage(PRINT_CRC_ERR);

			// reset selected option to go back to updater menu
			UpdaterOptionSelected = 0;
			rx_byte = 0;
		}

		if (crc_received == crc_calculated)
		{
			if (0x1 == a_ReadRxData[1])
			{
				// If CRC is correct, proceed with firmware update
				for (uint32_t i = 0; i < BUFFER_SIZE; i++)
				{// first check if flash erase is needed
					if (*(uint8_t*)(currentAddress + i) != 0xFF)
					{
						HAL_GPIO_TogglePin(BLUE_GPIO_Port, BLUE_Pin);
						FLASH_EraseSector(currentAddress);
						HAL_GPIO_TogglePin(BLUE_GPIO_Port, BLUE_Pin);
					}
				}

				HAL_GPIO_TogglePin(GREEN_GPIO_Port, GREEN_Pin);
				// decrypt the 257-th packet
				if (*pUploadAddress != 0xFF)
				{
					decrypt_rest_of_firmware();

					memset(rxBuff.data_buff, 0, sizeof rxBuff.data_buff);
					memset(a_ReadRxData, 0, sizeof(a_ReadRxData));
					PacketReceived = SET;
					copyData = SET;
					x_state = WAIT_SOH;
				}
				// decrypt and store first packet to application address
				else if (*pUploadAddress == 0xFF)
				{
					// Extract nonce from the first packet
					memcpy(nonce_counter, a_EncryptedData, 12);

					// Extract the size field (encrypted data + tag)
					file_size = extract_size(a_EncryptedData);

					if (file_size > 262144)
					{
						X_SendCan();
						DBG("Size of the firmware is too big. Aborting!\n\r");
						HAL_Delay(1000);

						// load previous firmware
						load_backup();

						// Reinitialize all variables to the initial values
						rxBuff.head = RESET;
						rxBuff.tail = RESET;
						rxBuff.dataCount = RESET;
						firstPacket = RESET;
						copyData = SET;
						PacketReceived = RESET;
						prev_index1 = 0;
						prev_index2 = 0xFF;
						dataCounter = 0;
						checksum = 0;
						total_packets = 0;
						total_encrypted_length = 0;
						file_size = 0;
						HAL_Delay(800);
						clear_screen();
						set_cursor_position(0, 0);
						rx_byte = 0;
						UpdaterOptionSelected = RESET;
						ShowMessage(PRINT_OPTIONS);
					}

					total_encrypted_length = file_size - sizeof(pTagGCM);; // Subtracting the tag length

					total_packets = (file_size + sizeof(nonce_counter) + 4) / 128;

					remaining_bytes_in_last_packet = (file_size + sizeof(nonce_counter) + 4) % 128;

					if ((0 != remaining_bytes_in_last_packet) && (0 != total_packets))
					{
						total_packets++;
					}

					size_t length_to_decrypt = 0;
					for (size_t i = sizeof(nonce_counter) + 4; i < 128; i++)
					{
						if (a_EncryptedData[i] == 0x1A && a_EncryptedData[i + 1] == 0x1A &&
							a_EncryptedData[i + 2] == 0x1A && a_EncryptedData[i + 3] == 0x1A)
						{
							break;
						}
						length_to_decrypt++;
					}

					// decrement tag size if there is only one packet of data
					if (file_size <= (128 - sizeof(pTagGCM) - 4))
					{

						length_to_decrypt = length_to_decrypt - sizeof(pTagGCM);
					}

					// Update HeaderAES with current value of message_counter
					prepare_header();

					// Initialize the GCM context for decryption
					if (HAL_OK != mbedtls_gcm_starts(&aes, MBEDTLS_GCM_DECRYPT, nonce_counter, sizeof(nonce_counter), HeaderAES, sizeof(HeaderAES)))
					{
						Error_Handler();
					}

					// Update the GCM context with the first chunk (112 bytes of encrypted data)
					if (HAL_OK != mbedtls_gcm_update(&aes, length_to_decrypt, a_EncryptedData + 16, a_DecryptedData))
					{
						Error_Handler();
					}

					FLASH_Write(a_DecryptedData, length_to_decrypt, currentAddress);
					currentAddress = currentAddress + length_to_decrypt;

					remaining_encrypted_length = total_encrypted_length - length_to_decrypt;
					remaining_packets = total_packets - 1;
				}

				HAL_GPIO_TogglePin(GREEN_GPIO_Port, GREEN_Pin);

				memset(rxBuff.data_buff, 0, sizeof(rxBuff.data_buff));
				memset(a_ReadRxData, 0, sizeof(a_ReadRxData));
				PacketReceived = SET;
				copyData = SET;
				x_state = WAIT_SOH;
				total_packet_count++;
			}
			if ((a_ReadRxData[1] > X_SOH) || ((0 == a_ReadRxData[1]) && (0 != a_ReadRxData[0])))
			{
				for (uint32_t i = 0; i < 2048; i++)
				{
					if (0xFF != *(uint8_t*)(currentAddress + i))
					{
						HAL_GPIO_TogglePin(BLUE_GPIO_Port, BLUE_Pin);
						FLASH_EraseSector(currentAddress);
						HAL_GPIO_TogglePin(BLUE_GPIO_Port, BLUE_Pin);
					}
				}

				decrypt_rest_of_firmware();

				memset(rxBuff.data_buff, 0, sizeof rxBuff.data_buff);
				memset(a_ReadRxData, 0, sizeof(a_ReadRxData));
				PacketReceived = SET;
				copyData = SET;
				x_state = WAIT_SOH;
			}
		}
		break;
	}
	}
}

/*
 * send current firmware to host using XMODEM CRC 16 bit
 */
 /* Take 128 bytes of data and make a packet out of it.

  *      Each packet looks like this:
  *      +-----+-------+-------+------+-----+
  *      | SOH | Idx1. | Idx2. | data | SUM |
  *      +-----+-------+-------+------+-----+
  *      SOH  = 0x01
  *      Idx1 = The sequence number.
  *      Idx2 = The complement of the sequence number.
  *      Data = A 128 bytes of data.
  *      SUM  = Add the contents of the 128 bytes and use the low-order
  *             8 bits of the result.
  *
  * @Note	- present application data is encrypted in chunks before being placed into 128 byte
  * 			  buffer. Then the buffer is assembled with XMODEM header and CRC. Each XMODEM packet
  * 			  is a part of the whole encrypted binary which may be de-crypted on the host side using the same
  * 			  key and initial iv key
  */
void DownloadUserApplication(void)
{
	uint32_t firmwareSize = 0;
	uint8_t* pFirmwareData = (uint8_t*)SLOT_2_VER_ADDR;

	// Calculate firmware size
	while (*(uint32_t*)&pFirmwareData[firmwareSize] != 0xFFFFFFFF)
	{
		firmwareSize++;
	}

	// copy firmware size value into global variable
	total_encrypted_length = firmwareSize;

	// Calculate total size to be encrypted and the tag
	file_size = firmwareSize + sizeof(pTagGCM);
	uint32_t currentData = 0;

	while (currentData < firmwareSize)
	{
		// empty buffers
		memset(a_EncryptedData, 0, sizeof(a_EncryptedData));
		memset(a_SendData, 0, sizeof(a_SendData));

		// Calculate remaining size based on the offset
		uint32_t remainingSize = firmwareSize - currentData;
		uint32_t chunkSize = remainingSize > 128 ? 128 : remainingSize;

		// Prepare packet
		a_SendData[0] = X_SOH;

		if (blockNum == 256)
		{
			blockNum = 0;
		}

		a_SendData[1] = blockNum;
		a_SendData[2] = ~blockNum;

		if (RESET == first_packet_complete)
		{
			uint8_t len = firmwareSize > 112 ? 112 : firmwareSize;
			gcm_begin_encryption(pFirmwareData, len);
			chunkSize = len;
		}
		else
		{
			if (HAL_OK != mbedtls_gcm_update(&aes, chunkSize, pFirmwareData + currentData, a_EncryptedData))
			{
				Error_Handler();
			}

			memcpy(&a_SendData[3], a_EncryptedData, chunkSize);

			// Check if this is the last chunk
			if (currentData + chunkSize >= firmwareSize)
			{
				// Finalize the GCM encryption to get the tag
				if (HAL_OK != mbedtls_gcm_finish(&aes, calculated_tag, 16))
				{
					Error_Handler();
				}

				uint32_t remainingSpace = 128 - chunkSize;
				if (remainingSpace >= 16)
				{
					// If the remaining space in the last packet is enough to include the whole tag
					memcpy(&a_SendData[3 + chunkSize], calculated_tag, 16);
					memset(&a_SendData[3 + chunkSize + 16], 0x1A, 128 - (chunkSize + 16)); // Pad with ^Z
				}
				else
				{
					// If the tag needs to be split across the last two packets
					uint8_t tag_part1_size = remainingSpace;
					uint8_t tag_part2_size = 16 - tag_part1_size;

					memcpy(&a_SendData[3 + chunkSize], calculated_tag, tag_part1_size);
					memset(&a_SendData[3 + chunkSize + tag_part1_size], 0x1A, 128 - (chunkSize + tag_part1_size)); // Pad with ^Z

					// Calculate CRC for the current packet
					crc_calculated = X_CRC16_Calculate(&a_SendData[3], 128);
					a_SendData[131] = (crc_calculated >> 8) & 0xFF;
					a_SendData[132] = crc_calculated & 0xFF;

					// Send the current packet
					X_SendPacket(a_SendData);

					// Prepare next packet with the remaining tag part
					currentData = currentData + chunkSize;
					chunkSize = 0;  // Reset chunk size for the next packet
					blockNum++;
					if (blockNum == 256)
					{
						blockNum = 0;
					}

					memset(a_SendData, 0, sizeof(a_SendData));
					a_SendData[0] = X_SOH;
					a_SendData[1] = blockNum;
					a_SendData[2] = ~blockNum;
					memcpy(&a_SendData[3], &calculated_tag[tag_part1_size], tag_part2_size);
					memset(&a_SendData[3 + tag_part2_size], 0x1A, 128 - tag_part2_size); // Pad with ^Z

					// Calculate CRC for the tag part packet
					crc_calculated = X_CRC16_Calculate(&a_SendData[3], 128);
					a_SendData[131] = (crc_calculated >> 8) & 0xFF;
					a_SendData[132] = crc_calculated & 0xFF;

					// Send the tag part packet
					X_SendPacket(a_SendData);

					continue;
				}
			}
		}

		// if all the data fits into one packet
		if (firmwareSize <= (128 - sizeof(pTagGCM)))
		{
			if (HAL_OK != mbedtls_gcm_finish(&aes, calculated_tag, 16))
			{
				Error_Handler();
			}
			memcpy(&a_SendData[3 + sizeof(nonce_counter) + 4 + chunkSize], calculated_tag, sizeof(calculated_tag));
			memset(&a_SendData[3 + sizeof(nonce_counter) + 4 + chunkSize + sizeof(calculated_tag)], 0x1A, 128 - (chunkSize + sizeof(calculated_tag) + sizeof(nonce_counter) + 4));
		}

		// Calculate CRC
		crc_calculated = X_CRC16_Calculate(&a_SendData[3], 128);
		a_SendData[131] = (crc_calculated >> 8) & 0xFF;
		a_SendData[132] = crc_calculated & 0xFF;

		// Send packet
		X_SendPacket(a_SendData);

		first_packet_complete = SET;

		currentData = currentData + chunkSize;
		blockNum++;
	}

	// Finalize the GCM context
	mbedtls_gcm_free(&aes);

	// Finish the transfer with EOT
	RingBuffer_Write(&txBuff, X_EOT);
	RingBuffer_Transmit(&txBuff);

	// Wait for ACK after EOT
	while (1)
	{
		if (SET == RingBuffer_Read(&rx_byte, &rxBuff))
		{
			if (X_ACK == rx_byte)
			{
				DBG("Transfer complete successfully.\n\r");
				UpdaterOptionSelected = RESET;
				first_packet_complete = RESET;
				HAL_Delay(1200);
				clear_screen();
				set_cursor_position(0, 0);
				ShowMessage(PRINT_OPTIONS);
				rx_byte = 0;
				return;
			}
			else
			{
				DBG("Failed to transmit. No ACK after transferring.\n\r");
				UpdaterOptionSelected = RESET;
				first_packet_complete = RESET;
				ShowMessage(PRINT_OPTIONS);
				rx_byte = 0;
				return;
			}
		}
	}
}


/*
 * @function name    - gcm_begin_encryption
 *
 * @brief            - This function starts GCM encryption
 *
 * @parameter[in]    - pointer to buffer that contains 112 bytes of plaintext data
 *
 * @return           - None
 *
 * @Note             - none
 */
void gcm_begin_encryption(uint8_t* src, size_t len)
{
	uint8_t sz[4] = { 0 };
	sz[0] = file_size >> 24;
	sz[1] = file_size >> 16;
	sz[2] = file_size >> 8;
	sz[3] = file_size;

	// Update header
	prepare_header();

	if (HAL_OK != mbedtls_gcm_setkey(&aes, MBEDTLS_CIPHER_ID_AES, pKeyAES, 128))
	{
		Error_Handler();
	}

	// Initialize the GCM context for encryption
	if (HAL_OK != mbedtls_gcm_starts(&aes, MBEDTLS_GCM_ENCRYPT, nonce_counter, sizeof(nonce_counter), HeaderAES, sizeof(HeaderAES)))
	{
		Error_Handler();
	}

	// decrypt the first data chunk
	if (HAL_OK != mbedtls_gcm_update(&aes, len, src, a_EncryptedData))
	{
		Error_Handler();
	}

	memcpy(&a_SendData[3], nonce_counter, sizeof(nonce_counter));
	memcpy(&a_SendData[15], sz, sizeof(sz));
	memcpy(&a_SendData[19], a_EncryptedData, len);
}

/*
 * @function name    - X_SendPacket
 *
 * @brief            - This function sends a XMODEM packet
 *
 * @parameter[in]    - pointer to buffer that contains 133 bytes of a single XMODEM packet
 *
 * @return           - None
 *
 * @Note             - none
 */
void X_SendPacket(uint8_t* packet)
{
	// Send the packet
	for (uint8_t i = 0; i < 133; i++)
	{
		RingBuffer_Write(&txBuff, packet[i]);
	}
	RingBuffer_Transmit(&txBuff);

	// Wait for ACK
	while (1)
	{
		if (SET == RingBuffer_Read(&rx_byte, &rxBuff))
		{
			if (X_ACK == rx_byte)
			{
				break;
			}
			else if (X_NAK == rx_byte)
			{
				// Resend the packet if NAK is received
				for (uint8_t i = 0; i < 133; i++)
				{
					RingBuffer_Write(&txBuff, packet[i]);
				}
				RingBuffer_Transmit(&txBuff);
			}
		}
	}
}

/*
 * message expressions for terminal
 */
void ShowMessage(MESSAGE_t msg)
{
	switch (msg)
	{
	case PRINT_BOOT_LOADER:
	{
		DBG(" Boot into application loader...\n\r");
		break;
	}
	case PRINT_OPTIONS:
	{
		DBG("Please select an option:\n\r\n\r");
		DBG(" > 'SHIFT+U' --> Upload new firmware\n\r\n\r");
		DBG(" > 'SHIFT+D' --> Download current firmware to host\n\r\n\r");
		DBG(" > 'SHIFT+V' --> Print version of the current user application\n\r\n\r");
		DBG(" > 'SHIFT+E' --> Erase current user application\n\r\n\r");
		DBG(" > 'SHIFT+B' --> Load backup firmware into user application space\n\r\n\r");
		DBG(" > 'ENTER'   --> Regular boot\n\r");
		break;
	}
	case PRINT_UPDT_SEL:
	{
		DBG("Please select an update method:\n\r\n\r");
		DBG(" > 'SHIFT+A' --> Install major update(current version will be removed irretrievably)\n\r\n\r");
		DBG(" > 'SHIFT+P' --> Apply a patch to currently installed application\n\r\n\r");
		break;
	}
	case PRINT_SELECTION_ERR:
	{
		DBG("Please select from available options.\n\r");
		break;
	}
	case PRINT_WAIT_UPDATE:
	{
		DBG("Firmware update! Please wait until the operation is over!\n\r");
		break;
	}
	case PRINT_WAIT_FILE:
	{
		DBG("Send a new binary file using XMODEM protocol to update device firmware.\n\r");
		break;
	}
	case PRINT_CRC_ERR:
	{

		DBG("CRC error!!!\n\r\n\r");
		break;
	}
	case PRINT_WRONG_DATA_LEN:
	{
		DBG("Data length doesn't fit any block size format!\n\r\n\r");
		break;
	}
	case PRINT_IDX1_ERR:
	{
		DBG("Index error! Booting to the current firmware!\n\r\n\r");
		break;
	}
	case PRINT_IDX2_ERR:
	{
		DBG("Index 2 error! Booting to the current firmware!\n\r\n\r");
		break;
	}
	case PRINT_ALREADY_NEWER:
	{
		HAL_Delay(200);
		DBG("\n\rNewer version is already installed! Doing regular booting...\n\r\n\r");
		break;
	}
	case PRINT_RDY_2_SEND:
	{
		DBG("\n\rData is ready to be sent. ");
		break;
	}
	default:
		break;
	}
}

/*
 * @function name    - SelectUpdateMethod
 *
 * @brief            - Read user input from serial terminal
 *
 * @parameter[in]    - None
 *
 * @return           - None
 *
 * @Note             - This function prompts user to select what kind of update they want to begin
 */
void SelectUpdateMethod(void)
{
	while (0 == rx_byte)
	{
		RingBuffer_Read(&rx_byte, &rxBuff);

		switch (rx_byte)
		{
		case 'A':
		{
			UpdaterOptionSelected = UPDATER_STATE_UPDATE_FULL;
			FullOrPatch = UpdaterOptionSelected;
			currentAddress = SLOT_2_VER_ADDR;
			ShowMessage(PRINT_WAIT_UPDATE);
			ShowMessage(PRINT_WAIT_FILE);
			break;
		}
		case 'P':
		{
			UpdaterOptionSelected = UPDATER_STATE_UPDATE_PATCH;
			FullOrPatch = UpdaterOptionSelected;
			currentAddress = PATCH_ADDR;
			ShowMessage(PRINT_WAIT_UPDATE);
			ShowMessage(PRINT_WAIT_FILE);
			break;
		}
		default:
		{
			if (0 != rx_byte)
			{
				ShowMessage(PRINT_SELECTION_ERR);
				rx_byte = 0;
			}
			break;
		}
		}
	}
}

/*
 * @function name    - load_backup
 *
 * @brief            - helper function to copy data from backup to application address
 *
 * @parameter[in]    - None
 *
 * @return           - None
 *
 * @Note             - None
 */
void load_backup(void)
{
	uint32_t data_length = 0;
	uint8_t* pBackup = (uint8_t*)BACKUP_ADDR;
	while (*(uint32_t*)&pBackup[data_length] != 0xFFFFFFFF)
	{
		data_length++;
	}

	FLASH_Write(pBackup, data_length, SLOT_2_VER_ADDR);
}

/*
 * @function name    - UpdateUserApplication
 *
 * @brief            - Backup the downloaded application and then do the jump to application loader
 *
 * @parameter[in]    - None
 *
 * @return           - None
 *
 * @Note             - None
 */
void UpdateUserApplication(void)
{

	uint32_t data_length = 0;
	uint32_t* src_addr = (uint32_t*)SLOT_2_VER_ADDR;

	// calculate encrypted firmware size
	while (src_addr[data_length] != 0xFFFFFFFF)
	{
		data_length++;
	}
	data_length = data_length * 4; // convert to bytes

	// calculate length of the old firmware
	uint32_t old_datalength = 0;
	src_addr = (uint32_t*)BACKUP_ADDR;

	while (src_addr[old_datalength] != 0xFFFFFFFF)
	{
		old_datalength++;
	}

	old_datalength = old_datalength * 4; // convert to bytes

	// Erase the copy of old firmware at backup address
	FLASH_EraseSector(BACKUP_ADDR);
	FLASH_EraseSector(BACKUP_ADDR + 0x20000);

	// Copy data from slot 2 to backup
	src_addr = (uint32_t*)SLOT_2_VER_ADDR;
	FLASH_Write((uint8_t*)src_addr, data_length, BACKUP_ADDR);

	DBG("\n\rUpdate is over! Going to application loader...\n\r\n\r");

	clear_screen();
	set_cursor_position(0, 0);
	//Copy operation is over. Can do the jump.
	JumpToAppLoader();
}

/*
 * @function name    - ApplyPatch
 *
 * @brief            - applies delta patch to currently present application
 *
 * @parameter[in]    - None
 *
 * @return           - None
 *
 * @Note             - None
 */
void ApplyPatch(void)
{
	// allocate static space for patching
	static unsigned char source_buf[BUFFER_SIZE] = { 0 };
	static unsigned char target_buf[BUFFER_SIZE] = { 0 };
	static unsigned char patch_buf[BUFFER_SIZE] = { 0 };

	// Erase the sector at firmware address
	FLASH_EraseSector(SLOT_2_APP_ADDR - 0x20000);
	FLASH_EraseSector(SLOT_2_APP_ADDR);
	FLASH_EraseSector(SLOT_2_APP_ADDR + 0x20000);

	// initialize janpatch context
	janpatch_ctx ctx;

	ctx.source_buffer.buffer = source_buf;
	ctx.source_buffer.size = BUFFER_SIZE;

	ctx.patch_buffer.buffer = patch_buf;
	ctx.patch_buffer.size = BUFFER_SIZE;

	ctx.target_buffer.buffer = target_buf;
	ctx.target_buffer.size = BUFFER_SIZE;

	ctx.fread = &sfio_fread;
	ctx.fwrite = &sfio_fwrite;
	ctx.fseek = &sfio_fseek;

	ctx.ftell = NULL;
	ctx.progress = NULL;

	// source
	uint32_t bkp_length = 0;
	uint8_t* src_addr = (uint8_t*)BACKUP_ADDR;

	// Calculate firmware size
	while (*(uint32_t*)&src_addr[bkp_length] != 0xFFFFFFFF)
	{
		bkp_length++;
	}

	JANPATCH_STREAM source;

	source.type = SFIO_STREAM_SLOT;
	source.offset = 0;
	source.size = (size_t)bkp_length;
	source.slot = BACKUP_ADDR;

	// patch
	uint32_t data_length = 0;
	src_addr = (uint8_t*)PATCH_ADDR;

	// Calculate firmware size
	while (*(uint32_t*)&src_addr[data_length] != 0xFFFFFFFF)
	{
		data_length++;
	}

	JANPATCH_STREAM patch;

	patch.type = SFIO_STREAM_RAM;
	patch.offset = 0;
	patch.size = (size_t)data_length;
	patch.ptr = (uint8_t*)PATCH_ADDR;

	// target
	JANPATCH_STREAM target;

	target.type = SFIO_STREAM_SLOT;
	target.offset = 0;
	target.size = (source.size + patch.size);
	target.slot = SLOT_2_VER_ADDR;

	// Apply the patch
	if (HAL_OK != janpatch(ctx, &source, &patch, &target))
	{
		DBG("\n\rUpdate failed!\n\r\n\r");
		// Handle error
		UpdaterOptionSelected = 0;
		rx_byte = 0;
		ShowMessage(PRINT_OPTIONS);
		return;
	}

	data_length = 0;
	src_addr = (uint8_t*)SLOT_2_VER_ADDR;

	// Calculate firmware size
	while (*(uint32_t*)&src_addr[data_length] != 0xFFFFFFFF)
	{
		data_length++;
	}

	// Erase the sector at backup
	FLASH_EraseSector(BACKUP_ADDR);
	FLASH_EraseSector(BACKUP_ADDR + 0x20000);

	// Write data to backup
	FLASH_Write((uint8_t*)src_addr, data_length, BACKUP_ADDR);

	DBG("\n\rUpdate is over! Going to application loader...\n\r\n\r");

	clear_screen();
	set_cursor_position(0, 0);
	//Copy operation is over. Can do the jump.
	JumpToAppLoader();
}

/*
 * Helper API to read firmware version from memory
 */
void get_firmware_current_version(uint8_t* major, uint8_t* minor, uint8_t* patch)
{
	image_version_t* ver_ptr = (image_version_t*)SLOT_2_VER_ADDR;

	*major = ver_ptr->version_major;
	*minor = ver_ptr->version_minor;
	*patch = ver_ptr->version_patch;
}

/*
 * Helper API to check if new version should be uploaded into device's memory or not
 */
uint8_t is_newer_version(uint8_t* major, uint8_t* minor, uint8_t* patch)
{
	uint8_t cur_major, cur_minor, cur_patch;

	get_firmware_current_version(&cur_major, &cur_minor, &cur_patch);

	if (0xFFFFFFFF == *((uint32_t*)SLOT_2_VER_ADDR))
	{
		//no firmware on device
		return 1;
	}
	if (*major > cur_major)
	{
		return 1;
	}
	else if (*major == cur_major)
	{
		if (*minor > cur_minor)
		{
			return 1;
		}
		else if (*minor == cur_minor)
		{
			if (*patch > cur_patch)
			{
				return 1;
			}
		}
	}

	return 0;

}

/*
 * @function name    - Read_Key
 *
 * @brief            - Read user input from serial terminal
 *
 * @parameter[in]    - None
 *
 * @return           - None
 *
 * @Note             - This is the main routine of the UPDATER and it allows user
 * 						to do firmware related actions like application update,
 * 						application erase, download the current application from
 * 						MCU to host or copy application from backup address
 */
void Read_Key(void)
{
	while (0 == rx_byte)
	{
		RingBuffer_Read(&rx_byte, &rxBuff);

		switch (rx_byte)
		{
		case 'U':
		{
			// reinit cryptographic module for decryption operation
			mbedTLS_AES_Init();

			clear_screen();
			set_cursor_position(0, 0);

			ShowMessage(PRINT_UPDT_SEL);

			rx_byte = 0;
			UpdaterOptionSelected = UPDATER_STATE_UPLOAD;
			SelectUpdateMethod();
			break;
		}
		case 'D':
		{
			ShowMessage(PRINT_RDY_2_SEND);
			UpdaterOptionSelected = UPDATER_STATE_DOWNLOAD;
			break;
		}
		case 'V':
		{
			uint8_t major, minor, patch;
			get_firmware_current_version(&major, &minor, &patch);
			if (0xFFFFFFFF == *((uint32_t*)SLOT_2_VER_ADDR))
			{
				DBG("There is no user application!\n\r");
			}
			else
			{
				char ver[80];
				sprintf(ver, "Current user application version: %d.%d.%d\n\r", major, minor, patch);
				DBG(ver);
			}
			rx_byte = 0;
			break;

		}
		case '\r':
		{// jump to app loader
			if (0xFFFFFFFF == *((uint32_t*)SLOT_2_VER_ADDR))
			{
				DBG("There is no user application!\n\r");
				rx_byte = 0;
				break;
			}
			ShowMessage(PRINT_BOOT_LOADER);
			clear_screen();
			set_cursor_position(0, 0);
			JumpToAppLoader();
			break;
		}
		case 'E':
		{//erase user application that is in slot 2
			if (0xFFFFFFFF == *((uint32_t*)SLOT_2_APP_ADDR))
			{
				DBG("Flash memory is already empty!\n\r");
			}
			else
			{
				DBG("Erasing flash. Please wait...\n\r");
				FLASH_EraseSector(SLOT_2_VER_ADDR);
				DBG("Done.\n\r");
			}
			rx_byte = 0;
			break;

		}
		case 'B':
		{
			if (0xFFFFFFFF == *((uint32_t*)BACKUP_ADDR))
			{
				DBG("No backup data available!\n\r");
				rx_byte = 0;
				break;
			}
			if (0xFFFFFFFF == *((uint32_t*)SLOT_2_VER_ADDR))
			{
				// Read data from backup
				uint32_t* src_addr = (uint32_t*)BACKUP_ADDR;

				uint32_t data_length = 0;
				while (data_length < (16 * 1024) && src_addr[data_length] != 0xFF)
				{
					data_length++;
				}

				data_length = data_length * 4;
				DBG("Copying from backup. Please wait...\n\r");
				HAL_GPIO_TogglePin(GREEN_GPIO_Port, GREEN_Pin);
				// Write data to version address
				FLASH_Write((uint8_t*)src_addr, data_length, SLOT_2_VER_ADDR);
				DBG("Done.\n\r");
				HAL_GPIO_TogglePin(GREEN_GPIO_Port, GREEN_Pin);
			}
			else
			{
				DBG("You have to erase user application first!\n\r");
			}
			rx_byte = 0;
			break;
		}
		default:
		{
			if (0 != rx_byte)
			{
				ShowMessage(PRINT_SELECTION_ERR);
				rx_byte = 0;
			}
			break;
		}
		}
	}
}

/*
 * @function name    - X_SendCan
 *
 * @brief            - This function sends X_CAN byte to host to cancel ongoing communication
 *
 * @return           - None
 *
 * @Note             - none
 */
void X_SendCan(void)
{
	RingBuffer_Write(&txBuff, X_CAN);
	RingBuffer_Transmit(&txBuff);
	RingBuffer_Write(&txBuff, X_CAN);
	RingBuffer_Transmit(&txBuff);
	RingBuffer_Write(&txBuff, X_CAN);
	RingBuffer_Transmit(&txBuff);
}

/*
 * @function name    - prepare_header
 *
 * @brief            - Update header with current value of message_counter
 *
 * @return           - None
 *
 * @Note             - none
 */
void prepare_header(void)
{
	memset(HeaderAES, 0, 16);

	memcpy(HeaderAES, (uint8_t*)0x1FFF7A10, 12);

	image_version_t* ver_ptr = (image_version_t*)BACKUP_ADDR;

	uint8_t ver[4] = { ver_ptr->signature1 , ver_ptr->version_major, ver_ptr->version_minor, ver_ptr->version_patch };

	memcpy(HeaderAES + 12, ver, sizeof(ver));
}


/*
 * @function name    - extract_size
 *
 * @brief            - extract size value from data sent by host
 *
 * @parameter[in]    - pointer to received file
 *
 * @return           - None
 *
 * @Note             - None
 */
uint32_t extract_size(uint8_t* file)
{
	uint8_t sz[4] = { 0 };
	memcpy(sz, a_EncryptedData + 12, 4);

	uint32_t fs = (sz[0] << 24) | (sz[1] << 16) | (sz[2] << 8) | sz[3];

	return fs;
}


/*
 * @function name    - JumpToAppLoader
 *
 * @brief            - Function to jump to LOADER
 *
 * @parameter[in]    - None
 *
 * @return           - None
 *
 * @Note             - Jump to LOADER's reset handler
 */
void JumpToAppLoader(void)
{
	typedef void (*fnc_ptr)(void);		//function pointer for jumping to user application
	fnc_ptr jump_to_app_loader;
	jump_to_app_loader = (fnc_ptr)(*(volatile uint32_t*)(SLOT_1_APP_LOADER_ADDR + 4U));

	HAL_RCC_DeInit();
	HAL_DeInit();

	SYSCFG->MEMRMP = 0x01;

	SysTick->CTRL = 0; //disable SysTick

	SCB->ICSR |= SCB_ICSR_PENDSTCLR_Msk;

	SCB->SHCSR &= ~(SCB_SHCSR_USGFAULTENA_Msk | \
		SCB_SHCSR_BUSFAULTENA_Msk | \
		SCB_SHCSR_MEMFAULTENA_Msk);

	// move vector table
	SCB->VTOR = (uint32_t)SLOT_1_APP_LOADER_ADDR;
	//change the main stack pointer
	__set_MSP(*(volatile uint32_t*)SLOT_1_APP_LOADER_ADDR);
	__set_PSP(*(volatile uint32_t*)SLOT_1_APP_LOADER_ADDR);

	jump_to_app_loader();
}

/*
 * serial terminal related API
 */
void clear_screen(void)
{
	DBG("\033[2J");
}

void set_cursor_position(int x, int y)
{
	char buffer[32];
	sprintf(buffer, "\033[%d;%dH", y, x);
	DBG(buffer);
}

/*
 * @function name    - decrypt_rest_of_firmware
 *
 * @brief            - De-crypts data packets sent by host to MCU and writes them to FLASH
 *
 * @parameter[in]    - None
 *
 * @return           - None
 *
 * @Note             - This function handles the de-cryption of received data using GCM
 */
void decrypt_rest_of_firmware(void)
{
	HAL_GPIO_TogglePin(GREEN_GPIO_Port, GREEN_Pin);

	// Initialize length to decrypt to the full packet size
	size_t length_to_decrypt = (remaining_packets > 0) ? 128 : remaining_bytes_in_last_packet;

	if (length_to_decrypt == 0)
	{
		length_to_decrypt = 128;
	}

	if (remaining_packets == 0 && remaining_bytes_in_last_packet <= 16)
	{
		// If there are only tag bytes left
		size_t tag_position = remaining_bytes_in_last_packet;
		if (tag_position != 0)
		{
			memcpy(pTagGCM, a_EncryptedData, 16);
		}
		else
		{
			memcpy(pTagGCM, a_EncryptedData + length_to_decrypt - 16, 16);
		}

		// Finalize decryption
		if (HAL_OK != mbedtls_gcm_finish(&aes, calculated_tag, 16))
		{
			Error_Handler();
		}
	}
	else if (remaining_packets == 1 && remaining_bytes_in_last_packet < 128)
	{
		// Handle the last packet with both data and tag
		size_t encrypted_data_length = remaining_bytes_in_last_packet - 16;
		if (encrypted_data_length > 0)
		{
			if (HAL_OK != mbedtls_gcm_update(&aes, encrypted_data_length, a_EncryptedData, a_DecryptedData))
			{
				Error_Handler();
			}

			FLASH_Write(a_DecryptedData, encrypted_data_length, currentAddress);
			currentAddress += encrypted_data_length;
		}

		memcpy(pTagGCM, a_EncryptedData + encrypted_data_length, 16);

		// Finalize decryption
		if (HAL_OK != mbedtls_gcm_finish(&aes, calculated_tag, 16))
		{
			Error_Handler();
		}

	}
	else
	{
		// Handle regular packets
		if (HAL_OK != mbedtls_gcm_update(&aes, length_to_decrypt, a_EncryptedData, a_DecryptedData))
		{
			Error_Handler();
		}

		FLASH_Write(a_DecryptedData, length_to_decrypt, currentAddress);
		currentAddress = currentAddress + length_to_decrypt;

		remaining_encrypted_length -= length_to_decrypt;
		remaining_packets--;

		// If it's the last packet and there are remaining bytes
		if (remaining_packets == 0 && remaining_bytes_in_last_packet > 16)
		{
			size_t last_chunk_size = remaining_bytes_in_last_packet - 16;
			if (last_chunk_size > 0)
			{
				if (HAL_OK != mbedtls_gcm_update(&aes, last_chunk_size, a_EncryptedData, a_DecryptedData))
				{
					Error_Handler();
				}

				FLASH_Write(a_DecryptedData, last_chunk_size, currentAddress);
				currentAddress += last_chunk_size;

				memcpy(pTagGCM, a_EncryptedData + last_chunk_size, 16);
			}

			// Finalize decryption
			if (HAL_OK != mbedtls_gcm_finish(&aes, calculated_tag, 16))
			{
				Error_Handler();
			}
		}
	}

	HAL_GPIO_TogglePin(GREEN_GPIO_Port, GREEN_Pin);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	   ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	   /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
