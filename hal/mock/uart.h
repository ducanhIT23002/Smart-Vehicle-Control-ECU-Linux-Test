#ifndef UART_H
#define UART_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void UART0_Init(uint32_t baudrate);
void UART0_SendString(const char *str);

#ifdef __cplusplus
}
#endif

#endif /* UART_H */