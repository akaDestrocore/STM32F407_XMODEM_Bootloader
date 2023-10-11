
/* Includes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include <simple_delay.h>
#include "stm32f407xx.h"

void TIM1_Config(void)
{
	RCC->APB2ENR.bit.tim1en = ENABLE;  	//TIM1 EN
	TIM1->PSC = 167; 					// ~1 uS delay
	TIM1->ARR |= 0xFFFF; 				//all bits set to 1
	TIM1->CR1.bit.cen = ENABLE; 		//timer counter EN

	while(SET != TIM1->SR.bit.uif)
	{
		//Update interrupt pending. This bit is set by hardware when the registers are updated
	}
}

void delay_us(int micro)
{
	TIM1->CNT = 0x0000;  				//reset counter register
	while(TIM1->CNT < micro);
}

void delay_ms(int ms)
{
	int i;
	for (i = 0; i < ms; i++)
		{
			delay_us (1000); 			//1 ms delay
		}
}
