#include "uart_log.h"

#define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS   0
#define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS       0
#define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS       0
#define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS      0
#define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS   0

#define NANOPRINTF_IMPLEMENTATION
#include "nanoprintf/nanoprintf.h"

static void putchar(int c, void *ctx) {
    USART2->DR = (uint8_t)c;
    while (!(USART2->SR & USART_SR_TXE)) {
    }
    while (!(USART2->SR & USART_SR_TC)) {
    }
}

void DBG(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    npf_vpprintf(&putchar, NULL, fmt, args);
    va_end(args);
}