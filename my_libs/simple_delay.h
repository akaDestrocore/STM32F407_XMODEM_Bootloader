#ifndef __SIMPLE_DELAY_H
#define __SIMPLE_DELAY_H

/* Includes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include <stdint.h>
#include "stm32f407xx.h"

/* Function prototypes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void timer_Config(void);
void Delay_SysTick_Handler(void);
void delay_ms(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif /* __SIMPLE_DELAY_H */
