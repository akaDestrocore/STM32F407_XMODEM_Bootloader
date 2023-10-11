/*										@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@@@@@@@
										@@@@@@@@@@@@@@@@@  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@
										@@@@@@@@@@@@@@@   @@@@@@@     @   @@@@@@@@@@@@@@
										@@@@@@@@@@@@@     @@@@@@@@  @@@@@@@@@@@@@@@@@@@@
										@@@@@@@@@@@@ @@@  (@@@@@@  @@@@@@@@@@@@@@@@@@@@@
										@@@@@@@@@@@@@@@@   @@@@/  @@@@@@@&   &@@.  @@@@@
										@@@@@@@@@@@@@@@@   @@@&  @@@@@     @@@@@@@@ @@@@
										@@@@@@@@@@@@@@@@@   @   @@@.    &@@@@@@@@@@@@@@@
										@@@@@@@@@@@@@@@@@             @@@         %   @@
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

#include <stm32f407xx_gpio.h>
#include "stdio.h"



/*
 * Peripheral Clock setup
 */

/********************************************************************************************************/
/* @function name 		- GPIO_PeriphClockControl														*/
/*																										*/
/* @brief				- This function enables or disables peripheral clock for the given GPIO port	*/
/*																										*/
/* @parameter[in]		- GPIO peripheral base address													*/
/*																										*/
/* @parameter[in]		- ENABLE or DISABLE macro or 1/0												*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void GPIO_PeriphClockControl(GPIO_RegDef_t *pGPIOx, uint8_t state)
{
	if(ENABLE == state)
	{
		if(pGPIOx == GPIOA)
		{
			RCC->AHB1ENR.bit.gpioaen = ENABLE;
		}else if(pGPIOx == GPIOB)
		{
			RCC->AHB1ENR.bit.gpioben = ENABLE;
		}else if(pGPIOx == GPIOC)
		{
			RCC->AHB1ENR.bit.gpiocen = ENABLE;
		}else if(pGPIOx == GPIOD)
		{
			RCC->AHB1ENR.bit.gpioden = ENABLE;
		}else if(pGPIOx == GPIOE)
		{
			RCC->AHB1ENR.bit.gpioeen = ENABLE;
		}else if(pGPIOx == GPIOF)
		{
			RCC->AHB1ENR.bit.gpiofen = ENABLE;
		}else if(pGPIOx == GPIOG)
		{
			RCC->AHB1ENR.bit.gpiogen = ENABLE;
		}else if(pGPIOx == GPIOH)
		{
			RCC->AHB1ENR.bit.gpiohen = ENABLE;
		}else if(pGPIOx == GPIOI)
		{
			RCC->AHB1ENR.bit.gpioien = ENABLE;
		}
	}else
	{
		if(DISABLE == state)
		{
			if(pGPIOx == GPIOA)
			{
				RCC->AHB1ENR.bit.gpioaen = DISABLE;
			}else if(pGPIOx == GPIOB)
			{
				RCC->AHB1ENR.bit.gpioben = DISABLE;
			}else if(pGPIOx == GPIOC)
			{
				RCC->AHB1ENR.bit.gpiocen = DISABLE;
			}else if(pGPIOx == GPIOD)
			{
				RCC->AHB1ENR.bit.gpioden = DISABLE;
			}else if(pGPIOx == GPIOE)
			{
				RCC->AHB1ENR.bit.gpioeen = DISABLE;
			}else if(pGPIOx == GPIOF)
			{
				RCC->AHB1ENR.bit.gpiofen = DISABLE;
			}else if(pGPIOx == GPIOG)
			{
				RCC->AHB1ENR.bit.gpiogen = DISABLE;
			}else if(pGPIOx == GPIOH)
			{
				RCC->AHB1ENR.bit.gpiohen = DISABLE;
			}else if(pGPIOx == GPIOI)
			{
				RCC->AHB1ENR.bit.gpioien = DISABLE;
			}
		}
	}
}

/*
 * Initialization and de-initialization
 */
/********************************************************************************************************/
/* @function name 		- GPIO_Init																		*/
/*																										*/
/* @brief				- This function initializes given GPIO port										*/
/*																										*/
/* @parameter[in]		- GPIO handle base address														*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	uint32_t temp = 0;		//temporary register var

	//enable peripheral clock
	GPIO_PeriphClockControl(pGPIOHandle->pGPIOx, ENABLE);

	if(pGPIOHandle->GPIO_Config.PinMode <= GPIO_MODE_ANALOG)
	{
		temp = pGPIOHandle->GPIO_Config.PinMode << (2 * pGPIOHandle->GPIO_Config.PinNumber); /*< shift to left by 2 bits because
																							 each pin in MODER is two bits >*/
		pGPIOHandle->pGPIOx->MODER.reg &= ~(0x3 << pGPIOHandle->GPIO_Config.PinNumber);
		pGPIOHandle->pGPIOx->MODER.reg |= temp;
	}else
	{
		//interrupt mode
		if(GPIO_MODE_IT_FT == pGPIOHandle->GPIO_Config.PinMode)
		{
			//configure FTSR
			EXTI->FTSR.reg |= (1 << pGPIOHandle->GPIO_Config.PinNumber);	 	//set bit in FTSR
			EXTI->RTSR.reg &= ~(1 << pGPIOHandle->GPIO_Config.PinNumber); 	//clear bit in RTSR just in case it is not reset
		}else if(GPIO_MODE_IT_RT == pGPIOHandle->GPIO_Config.PinMode)
		{
			//configure RTSR
			EXTI->RTSR.reg |= (1 << pGPIOHandle->GPIO_Config.PinNumber);	 	//set bit in RTSR
			EXTI->FTSR.reg &= ~(1 << pGPIOHandle->GPIO_Config.PinNumber); 	//clear bit in FTSR just in case it is not reset
		}else if (GPIO_MODE_IT_RFT == pGPIOHandle->GPIO_Config.PinMode)
		{
			//configure both FTSR and RTSR registers
			EXTI->RTSR.reg |= (1 << pGPIOHandle->GPIO_Config.PinNumber);	 	//set bit in RTSR
			EXTI->FTSR.reg |= (1 << pGPIOHandle->GPIO_Config.PinNumber); 	//set bit in FTSR
		}

		uint8_t temp1 = pGPIOHandle->GPIO_Config.PinNumber / 4;
		uint8_t temp2 = pGPIOHandle->GPIO_Config.PinNumber % 4;

		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
		RCC->APB2ENR.bit.syscfgen = ENABLE; 											// enable SYSCFG peripheral clock
		if((0 == temp1) || (1/4 == temp1) || (2/4 == temp1) || (3/4 == temp1))
		{
			SYSCFG->EXTICR1.reg |= portcode << (temp2 * 4);
		}
		else if((1 == temp1) || (5/4 == temp1) || (6/4 == temp1) || (7/4 == temp1))
		{
			SYSCFG->EXTICR2.reg |= portcode << (temp2 * 4);
		}
		else if((2 == temp1) || (9/4 == temp1) || (10/4 == temp1) || (11/4 == temp1))
		{
			SYSCFG->EXTICR3.reg |= portcode << (temp2 * 4);
		}
		else if((3 == temp1) || (13/4 == temp1) || (14/4 == temp1) || (15/4 == temp1))
		{
			SYSCFG->EXTICR4.reg |= portcode << (temp2 * 4);
		}

		EXTI->IMR.reg |= 1 << pGPIOHandle->GPIO_Config.PinNumber;			//enable the EXTI interrupt delivery using IMR
	}

	temp = pGPIOHandle->GPIO_Config.PinSpeed << (2 * pGPIOHandle->GPIO_Config.PinNumber);//shift to left by 2 bits
	pGPIOHandle->pGPIOx->OSPEEDR.reg &= ~(0x3 <<(2 * pGPIOHandle->GPIO_Config.PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDR.reg |= temp;

	temp = pGPIOHandle->GPIO_Config.PinPuPdControl << (2 * pGPIOHandle->GPIO_Config.PinNumber);//shift to left by 2 bits because
	pGPIOHandle->pGPIOx->PUPDR.reg &= ~(0x3 <<( 2 * pGPIOHandle->GPIO_Config.PinNumber));
	pGPIOHandle->pGPIOx->PUPDR.reg |= temp;

	temp = pGPIOHandle->GPIO_Config.PinOPType << pGPIOHandle->GPIO_Config.PinNumber;//shift to left by 2 bits
	pGPIOHandle->pGPIOx->OTYPER.reg &= ~(0x1 << pGPIOHandle->GPIO_Config.PinNumber);
	pGPIOHandle->pGPIOx->OTYPER.reg |= temp;

	if(GPIO_MODE_AF == pGPIOHandle->GPIO_Config.PinMode)
	{
		uint8_t temp1, temp2;

		temp1 = pGPIOHandle->GPIO_Config.PinNumber/8;	//AFRL = AFR[0]. Any integer smaller than 8 will be 0 after division
		temp2 = pGPIOHandle->GPIO_Config.PinNumber%8;
		if((0 == temp1) || (1/8 == temp1) || (2/8 == temp1) || (3/8 == temp1) ||
			(4/8 == temp1 ) || (5/8 == temp1) || (6/8 == temp1) || (7/8 == temp1))
		{
			pGPIOHandle->pGPIOx->AFRL.reg &= ~(0xF << (4* temp2));
			pGPIOHandle->pGPIOx->AFRL.reg |= pGPIOHandle->GPIO_Config.PinAltFuncMode << (4* temp2);
		}
		else if((1 == temp1) || (9/8 == temp1) || (10/8 == temp1) || (11/8 == temp1) ||
				(12/8 == temp1 ) || (13/8 == temp1) || (14/8 == temp1) || (15/8 == temp1))
		{
			pGPIOHandle->pGPIOx->AFRH.reg &= ~(0xF << (4* temp2));
			pGPIOHandle->pGPIOx->AFRH.reg |= pGPIOHandle->GPIO_Config.PinAltFuncMode << (4* temp2);
		}
	}
}

/********************************************************************************************************/
/* @function name 		- GPIO_DeInit 																	*/
/*																										*/
/* @brief				- This function de-initializes given GPIO port									*/
/*																										*/
/* @parameter[in]		- GPIO port base address														*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{

	if(pGPIOx == GPIOA)
	{
		RCC->AHB1RSTR.bit.gpioarst = ENABLE;
	}else if(pGPIOx == GPIOB)
	{
		RCC->AHB1RSTR.bit.gpiobrst = ENABLE;
	}else if(pGPIOx == GPIOC)
	{
		RCC->AHB1RSTR.bit.gpiocrst = ENABLE;
	}else if(pGPIOx == GPIOD)
	{
		RCC->AHB1RSTR.bit.gpiodrst = ENABLE;
	}else if(pGPIOx == GPIOE)
	{
		RCC->AHB1RSTR.bit.gpioerst = ENABLE;
	}else if(pGPIOx == GPIOF)
	{
		RCC->AHB1RSTR.bit.gpiofrst = ENABLE;
	}else if(pGPIOx == GPIOG)
	{
		RCC->AHB1RSTR.bit.gpiogrst = ENABLE;
	}else if(pGPIOx == GPIOH)
	{
		RCC->AHB1RSTR.bit.gpiohrst = ENABLE;
	}else if(pGPIOx == GPIOI)
	{
		RCC->AHB1RSTR.bit.gpioirst = ENABLE;
	}
}

/*
 * Data read and write
 */
/********************************************************************************************************/
/* @function name 		- GPIO_ReadPin																	*/
/*																										*/
/* @brief				- This function returns the value received by given pin							*/
/*																										*/
/* @parameter[in]		- GPIO port address																*/
/*																										*/
/* @parameter[in]		- Pin number																	*/
/*																										*/
/* @return				- 0 or 1																		*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
uint8_t GPIO_ReadPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	uint8_t temp;
	temp = (uint8_t)((pGPIOx->IDR.reg >> PinNumber) & 0x1);
	return temp;
}

/********************************************************************************************************/
/* @function name 		- GPIO_ReadPort																	*/
/*																										*/
/* @brief				- This function returns the value written in GPIOx port register				*/
/*																										*/
/* @parameter[in]		- GPIO port address																*/
/*																										*/
/* @return				- read port value																*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
uint16_t GPIO_ReadPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t temp;
	temp = (uint16_t)pGPIOx->IDR.reg;
	return temp;
}

/********************************************************************************************************/
/* @function name 		- GPIO_WritePin																	*/
/*																										*/
/* @brief				- This function writes the val value to the given pin number in register		*/
/*																										*/
/* @parameter[in]		- GPIO port address																*/
/*																										*/
/* @parameter[in]		- GPIO pin number																*/
/*																										*/
/* @parameter[in]		- value																			*/
/*																										*/
/* @return				-	none																		*/
/*																										*/
/* @Note				-	none																		*/
/********************************************************************************************************/
void GPIO_WritePin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t val)
{
	if(SET == val)
	{
		pGPIOx->ODR.reg |= (1 << PinNumber);
	}else
	{
		pGPIOx->ODR.reg &= ~(1 << PinNumber);
	}
}

/********************************************************************************************************/
/* @function name 		- GPIO_WritePort																*/
/*																										*/
/* @brief				- This function writes the val value into the given port register				*/
/*																										*/
/* @parameter[in]		- GPIO port address																*/
/*																										*/
/* @parameter[in]		- value																			*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void GPIO_WritePort(GPIO_RegDef_t *pGPIOx, uint8_t val)
{
	pGPIOx->ODR.reg = val;
}

/********************************************************************************************************/
/* @function name 		- GPIO_ToggleOutputPin															*/
/*																										*/
/* @brief				- This function enables or disables given pin according to it's current state	*/
/*																										*/
/* @parameter[in]		- GPIO port address																*/
/*																										*/
/* @parameter[in]		- GPIO pin number																*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	pGPIOx->ODR.reg ^= (1 << PinNumber);
}

/*
 * IRQ configuration and ISR handling
 */
/********************************************************************************************************/
/* @function name 		- GPIO_IRQConfig																*/
/*																										*/
/* @brief				- GPIO interrupt routine														*/
/*																										*/
/* @parameter[in]		- predefined IRQ Number															*/
/*																										*/
/* @parameter[in]		- interrupt priority															*/
/*																										*/
/* @parameter[in]		- on/off state																	*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void GPIO_IRQConfig(uint8_t IRQNumber, uint32_t IRQPriority, uint8_t state)
{
	if(ENABLE == state)
	{
		if(IRQNumber <= 31)
		{
			//ISER0
			NVIC->ISER[0] |= (1 << IRQNumber);

		}else if(IRQNumber > 31 && IRQNumber < 64)
		{
			//ISER1
			NVIC->ISER[1] |= (1 << IRQNumber % 32);

		}else if(IRQNumber >= 64 && IRQNumber < 96)
		{
			//ISER2
			NVIC->ISER[2] |= (1 << IRQNumber % 64);
		}
	}else
	{
		if(IRQNumber <= 31)
				{
					//ICER0
					NVIC->ICER[0] |= (1 << IRQNumber);

				}else if(IRQNumber > 31 && IRQNumber < 64)
				{
					//ICER1
					NVIC->ICER[1] |= (1 << IRQNumber % 32);

				}else if(IRQNumber >= 64 && IRQNumber < 96)
				{
					//ICER2
					NVIC->ICER[2] |= (1 << IRQNumber % 64);
				}
	}
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;
	uint8_t shift_amount = (8 * iprx_section) + 4;
	volatile uint8_t *ipr_reg = (volatile uint8_t *)&(NVIC->IP[0]) + iprx;
	*ipr_reg |= (IRQPriority << shift_amount);

}

/********************************************************************************************************/
/* @function name 		- GPIO_IRQHandling																*/
/*																										*/
/* @brief				- GPIO interrupt handling function												*/
/*																										*/
/* @parameter[in]		- pin number																	*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void GPIO_IRQHandling(uint8_t PinNumber)
{
	if(EXTI->PR.reg & (1 << PinNumber))
	{
		EXTI->PR.reg |= (1 << PinNumber); // clear pending bit
	}
}
/****************************************************** End of file *************************************************************/
