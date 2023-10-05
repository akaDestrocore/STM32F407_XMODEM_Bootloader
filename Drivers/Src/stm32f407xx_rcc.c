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

#include <stm32f407xx_rcc.h>

#define HSI_VALUE    ((uint32_t)16000000U) // HSI default value
#define HSE_VALUE    8000000U // HSE default value is 8000000U
volatile uint32_t SystemCoreClock = HSI_VALUE;

uint16_t a_AHB_Prescaler[8] = {2,4,8,16,64,128,256,512};
uint16_t a_APB1_Prescaler[4] = {2,4,8,16};
uint16_t a_APB2_Prescaler[4] = {2,4,8,16};

/********************************************************************************************************/
/* @function name 		- RCC_OscConfig															 		*/
/*																									 	*/
/* @brief				- This function configures oscillator for desired value 						*/
/*																										*/
/* @parameter[in]		- Pointer to Oscillator Initialization Strucuture								*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void RCC_OscConfig(RCC_OscInit_t *pRCC_Osc)
{
	if(NULL == pRCC_Osc)
	{
		return;
	}
	//configure HSE
	if(RCC_OSCILLATORTYPE_HSE == pRCC_Osc->OscillatorType)
	{
		switch(pRCC_Osc->HSEState)
		{
			case RCC_HSE_BYPASS:
			{
				RCC->CR.bit.hseon = DISABLE;
				RCC->CR.bit.hsebyp = RCC_CR_HSEBYP;
				RCC->CR.bit.hseon = RCC_CR_HSEON;
				break;
			}
			case RCC_HSE_ON:
			{
				RCC->CR.bit.hseon = RCC_CR_HSEON;
				break;
			}
			case RCC_HSE_OFF:
			{
				RCC->CR.bit.hseon = DISABLE;
			}
			default:
				break;
		}
	}
	//configure HSI
	if(RCC_OSCILLATORTYPE_HSI == pRCC_Osc->OscillatorType)
	{
		switch(pRCC_Osc->HSIState)
		{
			case RCC_HSI_ON:
			{
				RCC->CR.bit.hsion = RCC_CR_HSION;
				break;
			}
			case RCC_HSI_OFF:
			{
				RCC->CR.bit.hsion = DISABLE;
				break;
			}
			default:
				break;
		}
	}
	//configure LSE
	if(RCC_OSCILLATORTYPE_LSE == pRCC_Osc->OscillatorType)
	{
		switch(pRCC_Osc->LSEState)
		{
			case RCC_LSE_ON:
			{
				RCC->BDCR.bit.lseon = ENABLE;
				break;
			}
			case RCC_LSE_OFF:
			{
				RCC->BDCR.bit.lseon = DISABLE;
				break;
			}
			default:
				break;
		}
	}
	//configure LSI
	if(RCC_OSCILLATORTYPE_LSI == pRCC_Osc->OscillatorType)
		{
			switch(pRCC_Osc->LSIState)
			{
				case RCC_LSI_ON:
				{
					RCC->CSR.bit.lsion = ENABLE;
					break;
				}
				case RCC_LSI_OFF:
				{
					RCC->CSR.bit.lsion = DISABLE;
					break;
				}
				default:
					break;
			}
		}
	//configure PLL
	if(RCC_PLL_NONE != pRCC_Osc->PLL.State)
	{
		switch(pRCC_Osc->PLL.State)
		{
			case RCC_PLL_OFF:
			{
				RCC->CR.bit.pllon = DISABLE;
				break;
			}
			case RCC_PLL_ON:
			{
				RCC->PLLCFGR.bit.pllsrc = pRCC_Osc->PLL.Source;
				RCC->PLLCFGR.bit.pllm = pRCC_Osc->PLL.M;
				RCC->PLLCFGR.bit.pllp = pRCC_Osc->PLL.P;
				RCC->PLLCFGR.bit.plln = pRCC_Osc->PLL.N;
				RCC->PLLCFGR.bit.pllq = pRCC_Osc->PLL.Q;
				RCC->CR.bit.pllon = RCC_CR_PLLON;
				break;
			}
			default:
				break;
		}

	}
}

/********************************************************************************************************/
/* @function name 		- RCC_ClkInit_t															 		*/
/*																									 	*/
/* @brief				- This function configures oscillator for desired value 						*/
/*																										*/
/* @parameter[in]		- Pointer to Clock Initialization Structure										*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void RCC_ClockConfig(RCC_ClkInit_t  *pRCC_Clk)
{
	if(NULL == pRCC_Clk)
	{
	    return;
	}
	// Set flash latency based on new system clock frequency
	FLASH->ACR.bit.latency = 0x5U; //5WS for 150 < HCLK ≤ 168
	while(!(FLASH->ACR.bit.latency = 0x5U))
	{

	}

	//HCLK Configuration

	if(RCC_CLOCKTYPE_HCLK == ((pRCC_Clk->ClockType) & RCC_CLOCKTYPE_HCLK))
	{
		RCC->CFGR.bit.ppre1 = 0x7U;
		RCC->CFGR.bit.ppre2 = 0x7U;
		RCC->CFGR.bit.hpre = pRCC_Clk->AHBCLKDivider;
	}

	//SYSCLK Configuration

	if(RCC_CLOCKTYPE_SYSCLK == ((pRCC_Clk->ClockType) & RCC_CLOCKTYPE_SYSCLK))
	{
		//HSE is selected as System Clock Source
		if(RCC_SYSCLKSOURCE_HSE == pRCC_Clk->SYSCLKSource)
		{
			//wait HSE ready flag off
			while(!(RESET == RCC->CR.bit.hserdy))
			{

			}
		}
		//PLL is selected as System Clock Source
		else if((RCC_SYSCLKSOURCE_PLLCLK == pRCC_Clk->SYSCLKSource) ||
				(RCC_SYSCLKSOURCE_PLLRCLK == pRCC_Clk->SYSCLKSource))
		{
			//wait PLL ready flag
			while(!(SET == RCC->CR.bit.pllrdy))
			{

			}
		}
		else
		{
			//wait for HSI ready flag
			while(!(SET == RCC->CR.bit.hsirdy))
			{

			}
		}

		RCC->CFGR.bit.sw = pRCC_Clk->SYSCLKSource;
	}

	//PCLK1 Configuration
	if(RCC_CLOCKTYPE_PCLK1 == ((pRCC_Clk->ClockType) & RCC_CLOCKTYPE_PCLK1))
	{
		RCC->CFGR.bit.ppre1 = pRCC_Clk->APB1CLKDivider;
	}

	//PCLK2 Configuration
	if(RCC_CLOCKTYPE_PCLK2 == ((pRCC_Clk->ClockType) & RCC_CLOCKTYPE_PCLK2))
	{
		RCC->CFGR.bit.ppre2 = pRCC_Clk->APB2CLKDivider;
	}

	//Update the SystemCoreClock global variable
	SystemCoreClock = RCC_GetSysClockFreq();
}

/********************************************************************************************************/
/* @function name 		- RCC_GetSysClockFreq															*/
/*																									 	*/
/* @brief				- This function returns current system clock frequency HCLK freq				*/
/*																										*/
/* @return				- System clock frequency														*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
uint32_t RCC_GetSysClockFreq(void)
{
    uint32_t sys_clk_freq, clk_src, temp, ahb_prescaler;

    // Get the clock source used for the system clock
    clk_src = RCC->CFGR.bit.sws;

    if(RCC_SYSCLKSOURCE_HSI == clk_src)
    {
        sys_clk_freq = 16000000; // HSI
    }
    else if(RCC_SYSCLKSOURCE_HSE == clk_src)
    {
        sys_clk_freq = 8000000; // HSE
    }
    else if(RCC_SYSCLKSOURCE_PLLCLK == clk_src)
    {
        sys_clk_freq = RCC_GetPLLOutputClock(); // PLL
    }

    // Get the AHB prescaler
    temp = RCC->CFGR.bit.hpre;

    if(temp < 8)
    {
        ahb_prescaler = 1;
    }
    else
    {
        ahb_prescaler = a_AHB_Prescaler[temp-8];
    }

    sys_clk_freq /= ahb_prescaler;

    return sys_clk_freq;
}


/********************************************************************************************************/
/* @function name 		- RCC_GetPLLOutputClock															*/
/*																									 	*/
/* @brief				- This function returns PLL output clock frequency								*/
/*																										*/
/* @return				- PLL output frequency															*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
uint32_t RCC_GetPLLOutputClock(void)
{
    uint32_t pll_clk_src;
    uint64_t pll_input_freq, pll_output_freq;

    // Calculate the PLL input frequency
    pll_clk_src = RCC->PLLCFGR.bit.pllsrc;

    /* Determine the input frequency for the PLL */
    if (RCC_PLLCFGR_PLLSRC_HSI == pll_clk_src)
    {	//HSI = 0
        pll_input_freq = HSI_VALUE;
    }
    else if (RCC_PLLCFGR_PLLSRC_HSE == pll_clk_src)
    {
       /* Get the HSE oscillator frequency */
       pll_input_freq = HSE_VALUE;
    }
    else
    {
       /* Invalid PLL source, set frequency to 0 */
       pll_input_freq = 0;
    }

    uint32_t pllp_val;
    switch (RCC->PLLCFGR.bit.pllp)
    {
		case 0:
		{
			pllp_val = 2; break;
		}
		case 1:
		{
			pllp_val = 4; break;
		}
		case 2:
		{
			pllp_val = 6; break;
		}
		case 3:
		{
			pllp_val = 8; break;
		}
		default: pllp_val = 2; // default case, should not happen
    }

    // Calculate the PLL output frequency
    pll_output_freq = (pll_input_freq * (RCC->PLLCFGR.bit.plln)) / ((RCC->PLLCFGR.bit.pllm) * pllp_val);


    return pll_output_freq;
}
//TODO:
/********************************************************************************************************/
/* @function name 		- RCC_MCOConfig															 		*/
/*																									 	*/
/* @brief				- Selects the clock source to output on MCO1 pin(PA8) or on MCO2 pin(PC9)		*/
/*																										*/
/* @parameter[in]		- RCC_MCOx specifies the output direction for the clock source.					*/
/*																										*/
/* @parameter[in]		- RCC_MCOSource specifies the clock source to output.							*/
/*																										*/
/* @parameter[in]		- RCC_MCODiv specifies the MCOx prescaler.										*/
/*																										*/
/* @return				- none																			*/
/*																										*/
/* @Note				- PA8/PC9 should be configured in alternate function mode.						*/
/********************************************************************************************************/
void RCC_MCOConfig(uint32_t RCC_MCOx, uint32_t RCC_MCOSource, uint32_t RCC_MCODiv)
{
	GPIO_Handle_t MCO1;
	GPIO_Handle_t MCO2;

	if(RCC_MCO1 == RCC_MCOx)
	{
		//MCO1 configuration

		MCO1.pGPIOx = GPIOA;
		MCO1.GPIO_Config.PinMode = GPIO_MODE_AF;
		MCO1.GPIO_Config.PinAltFuncMode = GPIO_AF0_MCO;
		MCO1.GPIO_Config.PinOPType = GPIO_OUTPUT_PP;
		MCO1.GPIO_Config.PinPuPdControl = GPIO_PIN_NO_PUPD;
		MCO1.GPIO_Config.PinSpeed = GPIO_SPEED_FAST;
		MCO1.GPIO_Config.PinNumber = GPIO_PIN_8;

		GPIO_Init(&MCO1);

		RCC->CFGR.bit.mco1 = RCC_MCOSource;
		RCC->CFGR.bit.mco1pre = RCC_MCODiv;
	}
	else if(RCC_MCO2 == RCC_MCOx)
	{
		//MCO2 configuration

		MCO2.pGPIOx = GPIOC;
		MCO2.GPIO_Config.PinMode = GPIO_MODE_AF;
		MCO2.GPIO_Config.PinAltFuncMode = GPIO_AF0_MCO;
		MCO2.GPIO_Config.PinOPType = GPIO_OUTPUT_PP;
		MCO2.GPIO_Config.PinPuPdControl = GPIO_PIN_NO_PUPD;
		MCO2.GPIO_Config.PinSpeed = GPIO_SPEED_FAST;
		MCO2.GPIO_Config.PinNumber = GPIO_PIN_9;

		GPIO_Init(&MCO2);

		RCC->CFGR.bit.mco2 = RCC_MCOSource;
		RCC->CFGR.bit.mco2pre = RCC_MCODiv;
	}
}

/********************************************************************************************************/
/* @function name 		- RCC_EnableCSS																	*/
/*																									 	*/
/* @brief				- This function enables the Clock Security System.								*/
/*																										*/
/* @return				- PLL output frequency															*/
/*																										*/
/* @Note				- If a failure is detected on the HSE oscillator clock, this oscillator			*/
/*         				  is automatically disabled and an interrupt is generated to inform the			*/
/*         				  software about the failure (Clock Security System Interrupt, CSSI),			*/
/*		         		  allowing the MCU to perform rescue operations. The CSSI is linked to			*/
/*         			      the Cortex-M4 NMI (Non-Maskable Interrupt) exception vector					*/
/********************************************************************************************************/
void RCC_EnableCSS(void)
{
	RCC->CR.bit.csson = ENABLE;
}

/********************************************************************************************************/
/* @function name 		- RCC_EnableCSS																	*/
/*																									 	*/
/* @brief				- This function disables the Clock Security System.								*/
/*																										*/
/* @return				- PLL output frequency															*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
void RCC_DisableCSS(void)
{
	RCC->CR.bit.csson = DISABLE;
}

/********************************************************************************************************/
/* @function name 		- RCC_GetHCLKFreq															*/
/*																									 	*/
/* @brief				- Returns the HCLK frequency													*/
/*																										*/
/* @return				- HCLK frequency																*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
uint32_t RCC_GetHCLKFreq(void)
{
  return SystemCoreClock;
}

/********************************************************************************************************/
/* @function name 		- RCC_GetPCLK1Freq																*/
/*																									 	*/
/* @brief				- Returns the PCLK1 frequency													*/
/*																										*/
/* @return				- PCLK1 frequency																*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
uint32_t RCC_GetPCLK1Freq(void)
{
	uint32_t pclk1;
	uint8_t clk_src, temp, ahb_prescaler, apb1_prescaler = 0;

	// Get the clock source used for the system clock
	clk_src = RCC->CFGR.bit.sws;

	if(RCC_SYSCLKSOURCE_HSI == clk_src)
	{
		SystemCoreClock = 16000000; // HSI
	}
	else if(RCC_SYSCLKSOURCE_HSE == clk_src)
	{
		SystemCoreClock = 8000000; // HSE
	}
	else if(RCC_SYSCLKSOURCE_PLLCLK == clk_src)
	{
		SystemCoreClock = RCC_GetPLLOutputClock(); // PLL
	}

	// For AHB
	temp = RCC->CFGR.bit.hpre;

	if(temp < 8)
	{
		ahb_prescaler = 1;
	}
	else
	{
		ahb_prescaler = a_AHB_Prescaler[temp-8];
	}

	// For APB1
	temp = RCC->CFGR.bit.ppre1;

	if(temp < 4)
	{
		apb1_prescaler = 1;
	} else
	{
		apb1_prescaler = a_APB1_Prescaler[temp-4];
	}

	pclk1 = (SystemCoreClock / ahb_prescaler) / apb1_prescaler;

	return pclk1;
}

/********************************************************************************************************/
/* @function name 		- RCC_GetPCLK2Freq																*/
/*																									 	*/
/* @brief				- Returns the PCLK2 frequency													*/
/*																										*/
/* @return				- PCLK2 frequency																*/
/*																										*/
/* @Note				- none																			*/
/********************************************************************************************************/
uint32_t RCC_GetPCLK2Freq(void)
{
	uint32_t pclk2;
	uint8_t clk_src, temp, ahb_prescaler, apb2_prescaler = 0;

	// Get the clock source used for the system clock
	clk_src = RCC->CFGR.bit.sws;

	if(RCC_SYSCLKSOURCE_HSI == clk_src)
	{
		SystemCoreClock = 16000000; //HSI
	}else if(RCC_SYSCLKSOURCE_HSE == clk_src)
	{
		SystemCoreClock = 8000000;    //HSE
	}else if(RCC_SYSCLKSOURCE_PLLCLK == clk_src)
	{
		SystemCoreClock = RCC_GetPLLOutputClock();    //PLL
	}

	//for AHB
	temp = RCC->CFGR.bit.hpre;

	if(temp < 8)
	{
		ahb_prescaler = 1;
	}else
	{
		ahb_prescaler = a_AHB_Prescaler[temp-8];
	}

	//for APB2
	temp = RCC->CFGR.bit.ppre2;

	if(temp < 4)
	{
		apb2_prescaler = 1;
	}else
	{
		apb2_prescaler = a_APB2_Prescaler[temp-4];
	}

	pclk2 = SystemCoreClock / (ahb_prescaler * apb2_prescaler);

	return pclk2;
}
/****************************************************** End of file *************************************************************/
