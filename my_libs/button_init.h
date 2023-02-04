#ifndef BUTTON_INIT_H_
#define BUTTON_INIT_H_

#include "stm32f407xx.h"


void Button_GPIO_Config(void);
uint8_t ReadButton(void);

#endif /* BUTTON_INIT_H_ */
