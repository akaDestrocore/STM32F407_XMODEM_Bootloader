#ifndef __FANCY_DELAY_H
#define __FANCY_DELAY_H

/* Includes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include <stdint.h>

/* Function prototypes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void TIM1_Config(void);
void delay_us(int us);
void delay_ms(int ms);


#ifdef __cplusplus
}
#endif

#endif /* __FANCY_DELAY_H */
