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


#include <stm32f407xx.h>
#include <circularBuffer.h>
#include <simple_delay.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

const uint32_t version __attribute__((section(".version"))) = 0x2;

USART_Handle_t USART = {0};

CircularBuffer_t rxCB, txCB;

extern uint8_t TxInProgress;

void SystemClock_Config(void);
void USART_Config(void);
void LED_Config(void);

char message[1024] = "Welcome to orange blinky application!\n\r\n\r";

int main(void)
{
	SystemClock_Config();
	TIM1_Config();
	USART_Config();
	LED_Config();

	USART_SendData(&USART,(uint8_t*)message,strlen(message));

	while(1)
	{
		GPIOD->ODR.bit.odr_13 = ENABLE;
		delay_ms(1000);
		GPIOD->ODR.bit.odr_13 = DISABLE;
		delay_ms(1000);
	}
}

/*
 * local green and orange LEDs configuration function
 */
void LED_Config(void)
{
	GPIO_Handle_t LED_GPIO_h = {0};

	LED_GPIO_h.pGPIOx = GPIOD;
	LED_GPIO_h.GPIO_Config.PinMode = GPIO_MODE_OUTPUT;
	LED_GPIO_h.GPIO_Config.PinOPType = GPIO_OUTPUT_PP;
	LED_GPIO_h.GPIO_Config.PinPuPdControl = GPIO_PIN_PULL_DOWN;
	LED_GPIO_h.GPIO_Config.PinSpeed = GPIO_SPEED_HIGH;

	//Green
	LED_GPIO_h.GPIO_Config.PinNumber = GPIO_PIN_12;
	GPIO_Init(&LED_GPIO_h);

	//Orange
	LED_GPIO_h.GPIO_Config.PinNumber = GPIO_PIN_13;
	GPIO_Init(&LED_GPIO_h);

	//Red
	LED_GPIO_h.GPIO_Config.PinNumber = GPIO_PIN_14;
	GPIO_Init(&LED_GPIO_h);

	//Red
	LED_GPIO_h.GPIO_Config.PinNumber = GPIO_PIN_15;
	GPIO_Init(&LED_GPIO_h);
}

/*
 * local USART configuration function
 */
void USART_Config(void)
{
	GPIO_Handle_t USART_GPIO_h = {0};

	USART_GPIO_h.pGPIOx = GPIOB;
	USART_GPIO_h.GPIO_Config.PinMode = GPIO_MODE_AF;
	USART_GPIO_h.GPIO_Config.PinAltFuncMode = 7;
	USART_GPIO_h.GPIO_Config.PinOPType = GPIO_OUTPUT_PP;
	USART_GPIO_h.GPIO_Config.PinPuPdControl = GPIO_PIN_NO_PUPD;
	USART_GPIO_h.GPIO_Config.PinSpeed = GPIO_SPEED_HIGH;

	//TX
	USART_GPIO_h.GPIO_Config.PinNumber = GPIO_PIN_6;
	GPIO_Init(&USART_GPIO_h);

	//RX
	USART_GPIO_h.GPIO_Config.PinNumber = GPIO_PIN_7;
	USART_GPIO_h.GPIO_Config.PinOPType = GPIO_MODE_INPUT;
	GPIO_Init(&USART_GPIO_h);

	//Peripheral Configuration
	USART.pUSARTx = USART1;
	USART.USART_Config.USART_Baud = USART_BAUD_115200;
	USART.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
	USART.USART_Config.USART_Mode = USART_MODE_TXRX;
	USART.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
	USART.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
	USART.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;

	USART_Init(&USART);

	USART.pUSARTx->CR1.bit.rxneie = ENABLE;
	USART.pUSARTx->CR3.bit.eie = ENABLE;

	USART_IRQInterruptConfig(USART1_IRQn, 14, ENABLE);
}

void SystemClock_Config(void)
{
	RCC_OscInit_t Osc = {0};
	RCC_ClkInit_t Clk = {0};


	//osc init
	Osc.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	Osc.HSEState = RCC_HSE_ON;
	Osc.PLL.State = RCC_PLL_ON;
	Osc.PLL.Source = RCC_PLLCFGR_PLLSRC_HSE;
	Osc.PLL.M = 4;
	Osc.PLL.N = 168;
	Osc.PLL.P = 0;
	Osc.PLL.Q = 4;
	RCC_OscConfig(&Osc);

	//clk init
	Clk.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
            |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	Clk.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	Clk.AHBCLKDivider = RCC_SYSCLK_DIV1;
	Clk.APB1CLKDivider = RCC_HCLK_DIV4;
	Clk.APB2CLKDivider = RCC_HCLK_DIV2;

	RCC_ClockConfig(&Clk);
}
