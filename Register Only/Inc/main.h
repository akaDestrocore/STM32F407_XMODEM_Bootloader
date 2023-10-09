/*												@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@@@@@@@
 @file     main.h								@@@@@@@@@@@@@@@@@  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 @brief    Header for main.c file				@@@@@@@@@@@@@@@   @@@@@@@     @   @@@@@@@@@@@@@@
												@@@@@@@@@@@@@     @@@@@@@@  @@@@@@@@@@@@@@@@@@@@
 @author   destrocore							@@@@@@@@@@@@ @@@  (@@@@@@  @@@@@@@@@@@@@@@@@@@@@
 @version  V1.0									@@@@@@@@@@@@@@@@   @@@@/  @@@@@@@&   &@@.  @@@@@
												@@@@@@@@@@@@@@@@   @@@&  @@@@@     @@@@@@@@ @@@@
This file contains the common defines of the   	@@@@@@@@@@@@@@@@@   @   @@@.    &@@@@@@@@@@@@@@@
application.									@@@@@@@@@@@@@@@@@             @@@         %   @@
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
#ifndef MAIN_H_
#define MAIN_H_


#include <stm32f407xx.h>
#include <circularBuffer.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>


#define FLASH_APP_START_ADDRESS		((uint32_t)0x08008004U)
#define FLASH_TEMP_START_ADDRESS 	((uint32_t)0x080A0000U)
#define FIRMWARE_VERSION_ADDRESS 	((uint32_t)0x8008000U)

typedef enum{
	WAIT_SOH 	= 0x1U,
	WAIT_INDEX1 = 0X2U,
	WAIT_INDEX2 = 0x3U,
	READ_DATA 	= 0x4U,
	WAIT_CHKSM  = 0x5U
}XMODEM_State_t;

typedef enum
{
	MSG_WELCOME = 0x0U,
	MSG_JUMP = 0x1U,
	MSG_UPDT_IN_PRGS = 0x2U,
	MSG_ASK_FOR_A_FILE = 0x3U,
	MSG_CHKSUM_ERR = 0x4U,
	MSG_FLASH_ERR_WRITE = 0x5U,
	MSG_FLASH_ERR_ERASE = 0x6U,
	MSG_FLASH_ERR_BUSY = 0x7U,
	MSG_FLASH_ERR_INVALID_SECTOR = 0x8U,
	MSG_FLASH_ERR_WRONG_ADDR = 0x9U
}MESSAGE_t;

/*
 *	On-board LED colors
 */
typedef enum
{
	GREEN  = 0x0U,
	ORANGE = 0x1U,
	RED	   = 0x2U,
	BLUE   = 0x3U
}LED_Color_t;

/*
 *  Bytes defined by the protocol.
 */
typedef enum
{
	X_SOH = ((uint8_t)0x01U),  //start of header (128 bytes)
	X_EOT = ((uint8_t)0x04U),  //end of transmission
	X_ACK = ((uint8_t)0x06U),  //acknowledge
	X_NAK = ((uint8_t)0x15U),  //not acknowledge
	X_CAN = ((uint8_t)0x18U),  //cancel
}X_Bytes_t;

/*
 * FLASH data types
 */
typedef enum{
	DATA_TYPE_8B  = 0x1U,
	DATA_TYPE_16B = 0x2U,
	DATA_TYPE_32B = 0x4U,
	DATA_TYPE_64B = 0x8U
}FLASH_DATA_t;

/*
 * FLASH error types
 */
typedef enum
{
	FLASH_ERR_WRITE = 0x5U,
	FLASH_ERR_ERASE = 0x6U,
	FLASH_ERR_BUSY = 0x7U,
	FLASH_ERR_INVALID_SECTOR = 0x8U,
	FLASH_ERR_WRONG_ADDR = 0x9U
}FLASH_Error_t;

/* Private function prototypes **************************************************************/
void SystemClock_Config(void);
void USART_Write(uint8_t ch);
void USART_SendString(uint8_t str[]);
void USART_Config(void);
void LED_Config(void);
void UserButton_Config(void);
void ShowMessage(MESSAGE_t msg);
void Firmware_Update(void);
uint8_t SumOfArray(uint8_t array[], uint16_t length);
void LED_Control(LED_Color_t led, uint8_t state);
uint32_t get_firmware_current_version(void);
bool is_newer_version(uint32_t ver);
void UpdateUserApplication(void);
void FLASH_Init(void);
void FLASH_Erase(uint32_t destination);
uint32_t FLASH_EraseSector(uint32_t destination);
uint8_t FLASH_Write(uint8_t *sourcedata, uint32_t len, uint32_t destination);
void FLASH_Jump(void);
void TIM1_Config(void);
void delay_ms(int ms);

#endif /* MAIN_H_ */
