#ifndef SERIALCOM_VARS_H
#define SERIALCOM_VARS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "ring_buffer.h"

extern UART_HandleTypeDef huart __attribute__((weak));
extern uint8_t TxInProgress __attribute__((weak));
extern Ring_Buffer_t rxBuff __attribute__((weak));
extern Ring_Buffer_t txBuff __attribute__((weak));

#ifdef __cplusplus 
}
#endif

#endif  // SERIALCOM_VARS_H