#ifndef BSP_H
#define BSP_H

#include "drivers/uart.h"

void BSP_Init(void);
UART_Handle_t *BSP_ConsoleUart(void);

#endif