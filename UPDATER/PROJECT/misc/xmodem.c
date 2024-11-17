#include <xmodem.h>

/*
 * @function name    - X_CRC16_Calculate
 *
 * @brief            - This function calculates 16 bit CRC for a char buffer	
 *
 * @parameter[in]    - pointer to buffer base address
 *
 * @return           - None
 *
 * @Note             - none
 */
int X_CRC16_Calculate(uint8_t *ptr, int count)
{
	int  crc;
	uint8_t i;

	crc = 0;
	while (--count >= 0)
	{
		crc = crc ^ (int) *ptr++ << 8;
		i = 8;
		do
		{
			if (crc & 0x8000)
				crc = crc << 1 ^ 0x1021;
			else
				crc = crc << 1;
		}while(--i);
	}
	return crc & 0xFFFF;
}
