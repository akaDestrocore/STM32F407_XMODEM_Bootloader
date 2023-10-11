/*												@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@@@@@@@
 @file     stm32f407xx_rcc.h									@@@@@@@@@@@@@@@@@  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 @brief    STM32F407xx Device Peripheral 							@@@@@@@@@@@@@@@   @@@@@@@     @   @@@@@@@@@@@@@@
  		   RCC HAL module driver.							@@@@@@@@@@@@@     @@@@@@@@  @@@@@@@@@@@@@@@@@@@@
 @author   destrocore										@@@@@@@@@@@@ @@@  (@@@@@@  @@@@@@@@@@@@@@@@@@@@@
 @version  V1.0											@@@@@@@@@@@@@@@@   @@@@/  @@@@@@@&   &@@.  @@@@@
												@@@@@@@@@@@@@@@@   @@@&  @@@@@     @@@@@@@@ @@@@
This file provides firmware functions to manage 						@@@@@@@@@@@@@@@@@   @   @@@.    &@@@@@@@@@@@@@@@
the following functionalities of the Reset and 							@@@@@@@@@@@@@@@@@             @@@         %   @@
Clock Control (RCC) peripheral:									@@@@@@@@@@@@@@@@@   @@@@@          @@@@@@@@@@@ @
+ Initialization and de-initialization function 						@@@@@@@@@@@@@@@@@@@@@@@.%@  @@@@@  @@@@@@@@@@@@@
+ Peripheral Control functions									@@@@@@@@@@@@@@@@@@              @@@@@@@@@@@@@@@@
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
#ifndef __STM32F407xx_RCC_H_
#define __STM32F407xx_RCC_H_


#include <stm32f407xx.h>


extern volatile uint32_t SystemCoreClock;


/*
 * RCC PLL configuration structure definition
 */
typedef struct
{
  uint32_t State;   /*!< The new state of the PLL. 														*/
  uint32_t Source;  /*!< RCC_PLLSource: PLL entry clock source.											*/
  uint32_t M;       /*!< PLLM: Division factor for PLL VCO input clock.									*/
  uint32_t N;       /*!< PLLN: Multiplication factor for PLL VCO output clock.							*/
  uint32_t P;       /*!< PLLP: Division factor for main system clock (SYSCLK).         					*/
  uint32_t Q;       /*!< PLLQ: Division factor for OTG FS, SDIO and RNG clocks.							*/
}RCC_PLLInit_t;

/*
 * RCC Internal/External Oscillator (HSE, HSI, LSE and LSI) configuration structure definition
 */
typedef struct
{
  uint32_t OscillatorType;       /*!< The oscillators to be configured.This parameter can be a value of @RCC_Oscillator*/
  uint32_t HSEState;             /*!< The new state of the HSE.                        						*/
  uint32_t LSEState;             /*!< The new state of the LSE.												*/
  uint32_t HSIState;             /*!< The new state of the HSI.												*/
  uint32_t HSICalibrationValue;  /*!< The HSI calibration trimming value (default is RCC_HSICALIBRATION_DEFAULT).
                                       This parameter must be a number between Min_Data = 0x00 and Max_Data = 0x1F */
  uint32_t LSIState;             /*!< The new state of the LSI.												*/

  RCC_PLLInit_t PLL;        	 /*!< PLL structure parameters                                              */
}RCC_OscInit_t;

/*
 * RCC System, AHB and APB busses clock configuration structure definition
 */
typedef struct
{
  uint32_t ClockType;             /*!< The clock to be configured.
                                       This parameter can be a value of @RCC_System_Clock      		*/
  uint32_t SYSCLKSource;          /*!< The clock source (SYSCLKS) used as system clock.
                                       This parameter can be a value of @RCC_System_Clock_Source    */
  uint32_t AHBCLKDivider;         /*!< The AHB clock (HCLK) divider. This clock is derived from the system clock (SYSCLK).*/
  uint32_t APB1CLKDivider;        /*!< The APB1 clock (PCLK1) divider. This clock is derived from the AHB clock (HCLK).*/
  uint32_t APB2CLKDivider;        /*!< The APB2 clock (PCLK2) divider. This clock is derived from the AHB clock (HCLK). */

}RCC_ClkInit_t;

/*
 * @RCC_Oscillator
 */
typedef enum
{
	RCC_OSCILLATORTYPE_HSE = 0x1U,
	RCC_OSCILLATORTYPE_HSI = 0x2U,
	RCC_OSCILLATORTYPE_LSE = 0x4U,
	RCC_OSCILLATORTYPE_LSI = 0x8U
}RCC_Oscillator_t;

/*
 * @RCC_HSE_State
 */
typedef enum
{
	RCC_HSE_OFF = 0x0U,
	RCC_HSE_ON = 0x1U,
	RCC_HSE_BYPASS = 0x2U
}RCC_HSE_State_t;

/*
 * @RCC_HSI_State
 */
typedef enum
{
	RCC_HSI_OFF = 0x0U,
	RCC_HSI_ON = 0x1U
}RCC_HSI_State_t;

/*
 * @RCC_LSE_State
 */
typedef enum
{
	RCC_LSE_OFF = 0x0U,
	RCC_LSE_ON = 0x1U,
	RCC_LSE_BYPASS = 0x2U
}RCC_LSE_State_t;

/*
 * @RCC_LSI_State
 */
typedef enum
{
	RCC_LSI_OFF = 0x0U,
	RCC_LSI_ON = 0x1U
}RCC_LSI_State_t;

/*
 * @RCC_PLL_State
 */
typedef enum
{
	RCC_PLL_NONE = 0x0U,
	RCC_PLL_OFF = 0x1U,
	RCC_PLL_ON  = 0x2U
}RCC_PLL_State_t;

/*
 * @RCC_System_Clock
 */
typedef enum
{
	RCC_CLOCKTYPE_SYSCLK = 0x1U,
	RCC_CLOCKTYPE_HCLK   = 0x2U,
	RCC_CLOCKTYPE_PCLK1  = 0x4U,
	RCC_CLOCKTYPE_PCLK2  = 0x8U
}RCC_System_Clock_t;

/*
 * @RCC_System_Clock_Source
 */
typedef enum
{
	RCC_SYSCLKSOURCE_HSI = 0x0U,
	RCC_SYSCLKSOURCE_HSE = 0x1U,
	RCC_SYSCLKSOURCE_PLLCLK = 0x2U,
	RCC_SYSCLKSOURCE_PLLRCLK = 0x3U
}RCC_System_Clock_Source_t;

/*
 * RCC_AHB_Clock_Source AHB Clock Source
 */
typedef enum
{
	RCC_SYSCLK_DIV1 = 0x0U,
	RCC_SYSCLK_DIV2 = 0x8U,
	RCC_SYSCLK_DIV4 = 0x9U,
	RCC_SYSCLK_DIV8 = 0xAU,
	RCC_SYSCLK_DIV16 = 0xBU,
	RCC_SYSCLK_DIV64 = 0xCU,
	RCC_SYSCLK_DIV128 = 0xDU,
	RCC_SYSCLK_DIV256 = 0xEU,
	RCC_SYSCLK_DIV512 = 0xFU
}RCC_SYSCLK_DIV_t;

/*
 * @RCC_APB1_APB2_Clock_Source APB1/APB2 Clock Source
 */
typedef enum
{
RCC_HCLK_DIV1 = 0x0U,
RCC_HCLK_DIV2 = 0x4U,
RCC_HCLK_DIV4 = 0x5U,
RCC_HCLK_DIV8 = 0x6U,
RCC_HCLK_DIV16 = 0x7U
}RCC_HCLK_DIV_t;

/*
 * @RCC_MCO_Index MCO Index
 */
typedef enum
{
	RCC_MCO1 = 0x0U,
	RCC_MCO2 = 0x1U
}RCC_MCOx_t;

/*
 * @RCC_MCO_Index MCO Index
 */
typedef enum
{
	RCC_MCO1SOURCE_HSI = 0x0U,
	RCC_MCO1SOURCE_LSE = 0x1U,
	RCC_MCO1SOURCE_HSE = 0x2U,
	RCC_MCO1SOURCE_PLLCLK = 0x3U
}RCC_MCOxSOURCE_t;


/*
 *	Oscillator Initialization function
 */
void RCC_OscConfig(RCC_OscInit_t *pRCC_Osc);
/*
 *	Clock Configuration function
 */
void RCC_ClockConfig(RCC_ClkInit_t  *pRCC_Clk);


/* Peripheral Control functions  ************************************************/
uint32_t RCC_GetPLLOutputClock(void);
uint32_t RCC_GetSysClockFreq(void);
void     RCC_MCOConfig(uint32_t RCC_MCOx, uint32_t RCC_MCOSource, uint32_t RCC_MCODiv);
void     RCC_EnableCSS(void);
void     RCC_DisableCSS(void);
uint32_t RCC_GetHCLKFreq(void);
uint32_t RCC_GetPCLK1Freq(void);
uint32_t RCC_GetPCLK2Freq(void);




#endif /* __STM32F407xx_RCC_H_ */
