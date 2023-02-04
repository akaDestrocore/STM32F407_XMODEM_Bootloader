/*
 * this library has all the embedded LED required initialization functions
 */
#ifndef LED_INIT_H_
#define LED_INIT_H_

#include "stm32f407xx.h"

/* Private define ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
typedef enum{
	NONE = 0,
	GREEN,
	ORANGE,
	RED,
	BLUE,
	ALL
}LED_color_t;


void LED_GPIO_Config(void);
void LED_LightUp(LED_color_t color);
void LED_TurnOff(LED_color_t color);

#endif /* LED_INIT_H_ */
