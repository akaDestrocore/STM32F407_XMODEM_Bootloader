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

#include <stm32f407xx_flash.h>


static const uint32_t a_flash_sectors[] = {
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

typedef void (*fnc_ptr)(void);		//function pointer for jumping to user application

static uint32_t sectcount;

/*
 * Initialization and de-initialization
 */
/********************************************************************************************************/
/* @function name 		- FLASH_Init																	*/
/*																										*/
/* @brief				- This function initializes FLASH module										*/
/*																										*/
/* @parameter[in]		- FLASH program data block size													*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void FLASH_Init(FLASH_DATA_t blocksize)
{
	FLASH->OPTKEYR = FLASH_OPT_KEY1;				//OPTKEY1 = 0x08192A3B

	FLASH->OPTKEYR = FLASH_OPT_KEY2;				//OPTKEY2 = 0x4C5D6E7F

	//set latency
	FLASH->ACR.bit.latency = FLASH_ACR_LATENCY_5WS;

	//relock OPTCR
	FLASH->OPTCR.bit.optlock = ENABLE;

	FLASH_Unlock();

	//set block size
	FLASH->CR.bit.psize = blocksize >> 1;

	FLASH_Lock();

	sectcount = FLASH_SECTOR_TOTAL;									//12 sectors for stm32f407
}

/********************************************************************************************************/
/* @function name 		- DeInit																		*/
/*																										*/
/* @brief				- This function de-initializes all peripherals									*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void DeInit(void)
{
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
}

/*
 * Lock/Unlock functions
 */
/********************************************************************************************************/
/* @function name 		- FLASH_Unlock																	*/
/*																										*/
/* @brief				- This function unlocks FLASH module for data manipulation operations			*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void FLASH_Unlock(void)
{
	while(RESET != FLASH->SR.bit.bsy)
	{
		//wait for the flash memory not to be busy
	}
	//extra lock check
	if(SET == FLASH->CR.bit.lock)
	{
		FLASH->KEYR = FLASH_KEY1;							//KEY1 = 0x45670123
		FLASH->KEYR = FLASH_KEY2;							//KEY2 = 0xCDEF89AB
	}
}



/********************************************************************************************************/
/* @function name 		- FLASH_Lock																	*/
/*																										*/
/* @brief				- This function locks FLASH module to disable data modification					*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void FLASH_Lock(void)
{
	FLASH->CR.bit.lock = ENABLE;
}

/*
 * Data manipulation functions
 */
/********************************************************************************************************/
/* @function name 		- FLASH_EraseSector																*/
/*																										*/
/* @brief				- This function erases data in desired FLASH sector 							*/
/*																										*/
/* @parameter[in]		- FLASH sector beginning address												*/
/*																										*/
/* @return				- kB count in this sector														*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
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

	FLASH_Unlock();

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

	FLASH_Lock();

	//return kB in this sector
	return (a_flash_sectors[i]<<10);
}

/********************************************************************************************************/
/* @function name 		- FLASH_Erase																	*/
/*																										*/
/* @brief				- This function erases all data beginning at desired FLASH sector address		*/
/*																										*/
/* @parameter[in]		- FLASH sector beginning address												*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void FLASH_Erase(uint32_t destination)
{
	sectcount = FLASH_SECTOR_TOTAL;
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

	FLASH_Unlock();

	//light up orange on-board LED
	GPIOD->ODR.bit.odr_13 = ENABLE;

	//set sector erase
	FLASH->CR.bit.ser = ENABLE;

	k = i;
	for(i = k; i<FLASH_SECTOR_TOTAL; i++)
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

	FLASH_Lock();
}

/********************************************************************************************************/
/* @function name 		- FLASH_Write																	*/
/*																										*/
/* @brief				- This function writes data to desired FLASH sector 							*/
/*																										*/
/* @parameter[in]		- source data 																	*/
/*																										*/
/* @parameter[in]		- source data length															*/
/*																										*/
/* @parameter[in]		- FLASH sector beginning address												*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
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

	FLASH_Unlock();

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

	FLASH_Lock();
	return 0;
}

/********************************************************************************************************/
/* @function name 		- FLASH_Read																	*/
/*																										*/
/* @brief				- This function reads data from the FLASH sector to a destination buffer		*/
/*																										*/
/* @parameter[in]		- FLASH sector beginning address  												*/
/*																										*/
/* @parameter[in]		- length of data to read														*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void FLASH_Read(uint32_t source, uint8_t *destination, uint32_t len)
{
    // Copy data from Flash to destination buffer
    for (uint32_t i = 0; i < len; ++i)
    {
        destination[i] = *((volatile uint8_t*)(source + i));
    }
}

/*
 * Jump to application
 */
/********************************************************************************************************/
/* @function name 		- FLASH_Erase																	*/
/*																										*/
/* @brief				- This function erases all data beginning at desired FLASH sector address		*/
/*																										*/
/* @parameter[in]		- FLASH sector beginning address												*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void FLASH_Jump(void)
{
	fnc_ptr jump_to_app;
	jump_to_app = (fnc_ptr)(*(volatile uint32_t*) (FLASH_APP_START_ADDRESS+4U));
	DeInit();
	SysTick->CTRL = 0; //disable SysTick
	SCB->VTOR = FLASH_APP_START_ADDRESS;
	//change the main stack pointer
	__set_MSP(*(volatile uint32_t*)FLASH_APP_START_ADDRESS);
	__set_PSP(*(volatile uint32_t*)FLASH_APP_START_ADDRESS);
	jump_to_app();
}
/****************************************************** End of file *************************************************************/
