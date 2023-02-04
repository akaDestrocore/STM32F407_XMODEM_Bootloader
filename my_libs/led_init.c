#include "led_init.h"

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> GPIO CONFIGURATION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void LED_GPIO_Config(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; 			//GPIOD clock enabled
	GPIOD->MODER |= GPIO_MODER_MODER15_0 | GPIO_MODER_MODER14_0 | GPIO_MODER_MODER13_0 | GPIO_MODER_MODER12_0; 	//D15-D12 GPO mode
	GPIOD->OTYPER = 0;
	GPIOD->OSPEEDR = 0;
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */





/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> LIGHT UP LED FUNCTION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void LED_LightUp(LED_color_t color)
{
	switch(color)
	{
		case NONE:
			GPIOD->ODR = 0x0000;
			break;
		case GREEN:
			GPIOD->ODR |= GPIO_ODR_OD12;
			break;
		case ORANGE:
			GPIOD->ODR |= GPIO_ODR_OD13;
			break;
		case RED:
			GPIOD->ODR |= GPIO_ODR_OD14;
			break;
		case BLUE:
			GPIOD->ODR |= GPIO_ODR_OD15;
			break;
		case ALL:
			GPIOD->ODR |= GPIO_ODR_OD12 | GPIO_ODR_OD13| GPIO_ODR_OD14| GPIO_ODR_OD15;
			break;
		default: GPIOD->ODR = 0x0000;
	}
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */





/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> LIGHT UP LED FUNCTION <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void LED_TurnOff(LED_color_t color)
{
	switch(color)
	{
		case GREEN:
			GPIOD->ODR &= ~GPIO_ODR_OD12;
			break;
		case ORANGE:
			GPIOD->ODR &= ~GPIO_ODR_OD13;
			break;
		case RED:
			GPIOD->ODR &= ~GPIO_ODR_OD14;
			break;
		case BLUE:
			GPIOD->ODR &= ~GPIO_ODR_OD15;
			break;
		case ALL:
			GPIOD->ODR = 0x0000;
			break;
		default: return;
	}
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> END OF FILE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
