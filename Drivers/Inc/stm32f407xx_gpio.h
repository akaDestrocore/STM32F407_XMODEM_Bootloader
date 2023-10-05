/*												@@@@@@@@@@@@@@@@@@@      @@@@@@@@@@@@@@@@@@@@@@@
 @file     stm32f407xx_gpio.h					@@@@@@@@@@@@@@@@@  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 @brief    MC specific header file for		 	@@@@@@@@@@@@@@@   @@@@@@@     @   @@@@@@@@@@@@@@
  		   STM32F407VGT6 based Discovery board	@@@@@@@@@@@@@     @@@@@@@@  @@@@@@@@@@@@@@@@@@@@
 @author   destrocore							@@@@@@@@@@@@ @@@  (@@@@@@  @@@@@@@@@@@@@@@@@@@@@
 @version  V1.0									@@@@@@@@@@@@@@@@   @@@@/  @@@@@@@&   &@@.  @@@@@
												@@@@@@@@@@@@@@@@   @@@&  @@@@@     @@@@@@@@ @@@@
This file provides firmware functions to manage @@@@@@@@@@@@@@@@@   @   @@@.    &@@@@@@@@@@@@@@@
the following functionalities of the General 	@@@@@@@@@@@@@@@@@             @@@         %   @@
Purpose Input/Output (GPIO) peripheral:			@@@@@@@@@@@@@@@@@   @@@@@          @@@@@@@@@@@ @
+ Initialization and de-initialization function @@@@@@@@@@@@@@@@@@@@@@@.%@  @@@@@  @@@@@@@@@@@@@
+ Peripheral Control functions					@@@@@@@@@@@@@@@@@@              @@@@@@@@@@@@@@@@
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
#ifndef __STM32F407XX_GPIO_H_
#define __STM32F407XX_GPIO_H_

#include <stm32f407xx.h>

#define GPIO_BASEADDR_TO_CODE(pGPIOx)		( (pGPIOx == GPIOA)?0:\
											  (pGPIOx == GPIOB)?1:\
											  (pGPIOx == GPIOC)?2:\
											  (pGPIOx == GPIOD)?3:\
											  (pGPIOx == GPIOE)?4:\
											  (pGPIOx == GPIOH)?7:\
											  (pGPIOx == GPIOI)?8:0)

#define GPIO_AF0_MCO 0x0U

/*
 * This is a pin configuration structure for a GPIO pin
 */
typedef struct
{
	uint32_t PinNumber;				/*!< From 0 to 15 >*/
	uint8_t PinMode;				/*!< GPIO_INPUT_MODE, GPIO_OUTPUT_MODE, GPIO_AF_MODE etc. Check GPIO_Mode_t def. >*/
	uint8_t PinSpeed;				/*!< GPIO_SPEED_LOW, GPIO_SPEED_MEDIUM, GPIO_SPEED_FAST or GPIO_SPEED_HIGH >*/
	uint8_t PinPuPdControl;			/*!< GPIO_NO_PUPD, GPIO_PIN_PULL_UP or GPIO_PIN_PULL_DOWN >*/
	uint8_t PinOPType;				/*!< GPIO_OUTOUT_PP or GPIO_OUTPUT_OD >*/
	uint8_t PinAltFuncMode;
}GPIO_PinConfig_t;


/*
 * GPIO possible modes
 */
typedef enum
{
	GPIO_MODE_INPUT = 0x0U,
	GPIO_MODE_OUTPUT,
	GPIO_MODE_AF,
	GPIO_MODE_ANALOG,
	GPIO_MODE_IT,
	GPIO_MODE_IT_RT,
	GPIO_MODE_IT_FT,
	GPIO_MODE_IT_RFT
}GPIO_Mode_t;

typedef enum
{
	GPIO_OUTPUT_PP = 0,
	GPIO_OUTPUT_OD
}GPIO_Output_t;

typedef enum
{
	GPIO_SPEED_LOW = 0,
	GPIO_SPEED_MEDIUM,
	GPIO_SPEED_FAST,
	GPIO_SPEED_HIGH
}GPIO_OutputSpeed_t;

typedef enum
{
	GPIO_PIN_NO_PUPD = 0,
	GPIO_PIN_PULL_UP,
	GPIO_PIN_PULL_DOWN
}GPIO_PUPD_t;

/*
 * GPIO pin numbers
 */
typedef enum
{
	GPIO_PIN_0 = 0,
	GPIO_PIN_1,
	GPIO_PIN_2,
	GPIO_PIN_3,
	GPIO_PIN_4,
	GPIO_PIN_5,
	GPIO_PIN_6,
	GPIO_PIN_7,
	GPIO_PIN_8,
	GPIO_PIN_9,
	GPIO_PIN_10,
	GPIO_PIN_11,
	GPIO_PIN_12,
	GPIO_PIN_13,
	GPIO_PIN_14,
	GPIO_PIN_15
}GPIO_PIN_t;

/*
 * This is a handle structure for a GPIO pin
 */
typedef struct
{
	GPIO_RegDef_t *pGPIOx; 			/*!< This holds the base address of the GPIO port to which the pin belongs >*/
	GPIO_PinConfig_t GPIO_Config; 	/*!< This holds GPIO pin configuration settings >*/
}GPIO_Handle_t;


/********************************************************************************************/
/*								APIs supported by this driver								*/
/*		 For more information about the APIs check the function definitions					*/
/********************************************************************************************/
/*
 * Peripheral Clock setup
 */
void GPIO_PeriphClockControl(GPIO_RegDef_t *pGPIOx, uint8_t state);
/*
 * Initialization and de-initialization
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);
/*
 * Data read and write
 */
uint8_t GPIO_ReadPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WritePin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t val);
void GPIO_WritePort(GPIO_RegDef_t *pGPIOx, uint8_t val);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
/*
 * IRQ configuration and ISR handling
 */
void GPIO_IRQConfig(uint8_t IRQNumber, uint32_t IRQPriority, uint8_t state);
void GPIO_IRQHandling(uint8_t PinNumber);



#endif /* __STM32F407XX_GPIO_H_ */
