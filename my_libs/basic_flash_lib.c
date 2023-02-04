#include "basic_flash_lib.h"


/* Private variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
static uint32_t sectcount;

typedef void (*fnc_ptr)(void);		//function pointer for jumping to user application


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


/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> FLASH UNLOCK FUNCTION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void BFL_FLASH_Unlock(void)
{
	while ((FLASH->SR & FLASH_SR_BSY) != 0 );				//wait for the flash memory not to be busy
	if((FLASH->CR & FLASH_CR_LOCK) != 0)					//extra lock check
	{
		FLASH->KEYR = FLASH_KEY1;							//KEY1 = 0x45670123
		FLASH->KEYR = FLASH_KEY2;							//KEY2 = 0xCDEF89AB
	}
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */





/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> FLASH LOCK FUNCTION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void BFL_FLASH_Lock(void)
{
	uint32_t reg;
	reg = FLASH_CR_LOCK;
	FLASH->CR |= reg;					//	FLASH->CR |= FLASH_CR_LOCK;
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */





/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> FLASH INITIALIZATION FUNCTION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void BFL_FLASH_Init(BFL_data_t blocksize)
{
	uint32_t reg;
	FLASH->OPTKEYR = FLASH_OPT_KEY1;				//OPTKEY1 = 0x08192A3B
	FLASH->OPTKEYR = FLASH_OPT_KEY2;				// OPTKEY2 = 0x4C5D6E7F

	FLASH->ACR |= FLASH_ACR_LATENCY_5WS;			//set latency

  reg =   FLASH->OPTCR;
  reg |=  FLASH_OPTCR_OPTLOCK; 						//relock OPTCR
  FLASH->OPTCR = reg;

  BFL_FLASH_Unlock();

  blocksize &= 3; 									//set up blocksize
  reg = FLASH->CR;
  reg &= ~0x00000300;
  reg |= blocksize<<8;
  FLASH->CR = reg;									//program parallelism set to x64

  BFL_FLASH_Lock();

//  sectcount = FLASH_SECTOR_TOTAL;									//12 sectors for stm32f407
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */





/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ERASE SECTOR FUNCTION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
uint32_t BFL_EraseSector(uint32_t destination)
{
	int i;												//sector number index
	uint32_t addr = FLASH_BASE;
	uint32_t reg;

	if((FLASH->SR & FLASH_SR_BSY) != 0)
	{
		return 0;										//flash memory operation is in progress
	}

	for(i = 0; i < sectcount; i++)						//search for the sector
	{
		if(addr == destination)
		{
			break;
		}
		else if(addr > destination)
		{
		  return 0; 									//not a sector beginning address
		}
		addr = addr + (a_flash_sectors[i]<<10);
	}
	if(i == sectcount)
	{
	  return 0;											//not found in a_flash_sectors[]
	}

  BFL_FLASH_Unlock();

  //time to erase
  reg = FLASH->CR;
  reg |= FLASH_CR_SER;
  FLASH->CR = reg;										//set sector erase

  reg &= ~(0x1F << 3);
  reg |=  (i    << 3);
  FLASH->CR = reg;										//set sector index (SNB)

  reg |= FLASH_CR_STRT;
  FLASH->CR = reg;										//start the erase

  while(FLASH->SR & FLASH_SR_BSY);						//wait until erase complete

  FLASH->CR &= ~FLASH_CR_SER;							//sector erase flag does not clear automatically.

  BFL_FLASH_Lock();

  return(a_flash_sectors[i]<<10);						//return kB in this sector
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */




/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ERASE MULTIPLE SECTORS FUNCTION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void BFL_FLASH_Erase(uint32_t destination)
{
	  sectcount = FLASH_SECTOR_TOTAL;									//12 sectors for stm32f407
	int i;
		uint32_t addr = FLASH_BASE;
		uint32_t reg;

		if((FLASH->SR & FLASH_SR_BSY) != 0)
		{
			return 0;										//flash memory operation is in progress
		}

		for(i = 0; i < sectcount; i++)						//search for the sector
		{
			if(addr == destination)
			{
				break;
			}
			else if(addr > destination)
			{
			  return 0; 									//not a sector beginning address
			}
			addr = addr + (a_flash_sectors[i]<<10);
		}
		if(i == sectcount)
		{
		  return 0;											//not found in a_flash_sectors[]
		}

	  BFL_FLASH_Unlock();

	  LED_LightUp(ORANGE);
	  UART4_SendString((uint8_t*)"Flash erase in progress! Please wait until green lights up!\n\r");
	  //time to erase
	  reg = FLASH->CR;
	  reg |= FLASH_CR_SER;
	  FLASH->CR = reg;										//set sector erase

	  int k = i;
	  for(i = k; i<FLASH_SECTOR_TOTAL; i++)
	  {
		  reg &= ~(0x1F << 3);
		  reg |=  (i    << 3);
		  FLASH->CR = reg;										//set sector index (SNB)

		  reg |= FLASH_CR_STRT;
		  FLASH->CR = reg;										//start the erase

		  while(FLASH->SR & FLASH_SR_BSY);						//wait until erase complete
	  }

	  BFL_FLASH_Lock();
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */




/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> WRITE TO FLASH FUNCTION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
uint32_t BFL_FLASH_Write(uint8_t *sourcedata, uint32_t len, uint32_t destination)
{
	  uint32_t blocksize = (FLASH->CR>>8) & 0x03;				//get current element's size from command register
	  uint32_t offset;
	  uint32_t reg;

	  blocksize = 1 << blocksize;

	  if( (len & (blocksize-1))!=0)								//size control
	  {
	    return 1;
	  }

	  BFL_FLASH_Unlock();

	  for(offset = 0; offset < len; offset = offset + blocksize)
	  {
		  reg = FLASH->CR;
		  reg &= ~FLASH_CR_SER;
		  reg |= FLASH_CR_PG;
		  FLASH->CR = reg;

		  switch(blocksize)
	      {
	        case 1: 														//write 8 bit
	          *((volatile uint8_t*)destination) = *sourcedata;
	          	  break;

	        case 2: 														//write 16 bit
	          *((volatile uint16_t*)destination) = *(uint16_t*)sourcedata;
	          	  break;

	        case 4: 														//write 32 bit
	          *((volatile uint32_t*)destination) = *(uint32_t*)sourcedata;
	          	  break;

	        case 8: 														//write 64 bit
	          *((volatile uint64_t*)destination) = *(uint64_t*)sourcedata;
	          	  break;
	        default:
	        	return 1; 													//error
	        }

	      while(FLASH->SR & FLASH_SR_BSY);									//wait until complete programming

	      sourcedata  = sourcedata  + blocksize;
	      destination = destination + blocksize;
	    }

	  BFL_FLASH_Lock();
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */





/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> READ FLASH FUNCTION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void BFL_FLASH_Read(uint32_t source, uint8_t *destination, uint32_t len)
{
	uint32_t addr = destination;

	for(uint32_t i=0; i<len; i++)
	{
		*((uint8_t *)destination + i) = *(uint8_t *)addr;
		addr++;
	}
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */





/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> DEINITIALIZATION FUNCTION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void DeInit(void)
{
	RCC->APB1RSTR = 0xFFFFFFFFU;					//Reset of all peripherals
	RCC->APB1RSTR = 0x00U;

	RCC->APB2RSTR = 0xFFFFFFFFU;
	RCC->APB2RSTR = 0x00U;

	RCC->AHB1RSTR = 0xFFFFFFFFU;
	RCC->AHB1RSTR = 0x00U;

	RCC->AHB2RSTR = 0xFFFFFFFFU;
	RCC->AHB2RSTR = 0x00U;

	RCC->AHB3RSTR = 0xFFFFFFFFU;
	RCC->AHB3RSTR = 0x00U;

 return;
}

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */





/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> JUMP TO APP FUNCTION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */


void BFL_Jump(void)
{
	fnc_ptr jump_to_app;
	jump_to_app = (fnc_ptr)(*(volatile uint32_t*) (FLASH_APP_START_ADDRESS+4u));
	DeInit();
	SysTick->CTRL = 0; //disable SysTick
	SCB->VTOR = FLASH_APP_START_ADDRESS;
	__set_MSP(*(volatile uint32_t*)FLASH_APP_START_ADDRESS); 			//change the main stack pointer
	jump_to_app();
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> END OF FILE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
