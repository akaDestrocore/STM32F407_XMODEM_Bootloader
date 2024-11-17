#include <updater_flash.h>

static uint32_t a_flash_sectors[] =
{
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

static uint32_t sectcount = 12;

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
/* @Note					- none																			*/
/********************************************************************************************************/
uint32_t FLASH_EraseSector(uint32_t destination)
{
    int i = 0;
    uint32_t addr = FLASH_BASE;
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t SectorError;

    if(HAL_FLASH_GetError() != HAL_FLASH_ERROR_NONE)
    {
        return 1;
    }

    for(i = 0; i < sectcount; i++)
    {
        if (addr == destination)
        {
            break;
        }
        else if (addr > destination)
        {
            return 2;
        }
        addr = addr + (a_flash_sectors[i] << 10);
    }
    if (i == sectcount)
    {
        return 3;
    }

    HAL_FLASH_Unlock();

    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector = i;
    EraseInitStruct.NbSectors = 1;

    if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
    {
        HAL_FLASH_Lock();
        return 1;
    }

    HAL_FLASH_Lock();

    return (a_flash_sectors[i] << 10);
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
    int i, k;
    uint32_t flash_base_addr = FLASH_BASE;
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t SectorError;

    if (HAL_FLASH_GetError() != HAL_FLASH_ERROR_NONE)
    {
        return;
    }

    for (i = 0; i < sectcount; i++)
    {
        if (flash_base_addr == destination)
        {
            break;
        }
        else if (flash_base_addr > destination)
        {
            return;
        }
        flash_base_addr += (a_flash_sectors[i] << 10);
    }
    if (i == sectcount)
    {
        return;
    }

    HAL_FLASH_Unlock();

    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);

    k = i;
    for (i = k; i < FLASH_SECTOR_TOTAL; i++)
    {
        EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
        EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
        EraseInitStruct.Sector = i;
        EraseInitStruct.NbSectors = 1;

        if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
        {
            HAL_FLASH_Lock();
            return;
        }
    }
    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);

    HAL_FLASH_Lock();
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
    uint32_t blocksize = (FLASH->CR & FLASH_CR_PSIZE) >> FLASH_CR_PSIZE_Pos;

    switch (blocksize)
    {
        case 0:
            blocksize = 1; // 8-bit
            break;
        case 1:
            blocksize = 2; // 16-bit
            break;
        case 2:
            blocksize = 4; // 32-bit
            break;
        case 3:
            blocksize = 8; // 64-bit
            break;
        default:
            return 1;
    }

    if (len % blocksize != 0)
    {
        return 2;
    }

    HAL_FLASH_Unlock();

    for (uint32_t offset = 0; offset < len; offset += blocksize)
    {
        uint32_t data;
        memcpy(&data, sourcedata + offset, blocksize);

        if (HAL_FLASH_Program(blocksize == 1 ? FLASH_TYPEPROGRAM_BYTE : blocksize == 2 ? FLASH_TYPEPROGRAM_HALFWORD : blocksize == 4 ? FLASH_TYPEPROGRAM_WORD : FLASH_TYPEPROGRAM_DOUBLEWORD, destination + offset, data) != HAL_OK)
        {
            HAL_FLASH_Lock();
            return 1;
        }

        while (__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY))
        {
            // Wait for last operation to be completed
        }
    }

    HAL_FLASH_Lock();
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
    for (uint32_t i = 0; i < len; ++i)
    {
        destination[i] = *(volatile uint8_t *)(source + i);
    }
}
