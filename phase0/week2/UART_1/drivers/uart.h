#ifndef USART_H
#define USART_H

#include <stdint.h>
#include "include/device.h"

typedef struct{
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
} UART_TypeDef;

#define USART1_BASE     0x40011000UL
#define USART2_BASE     0x40004400UL
#define USART3_BASE     0x40004800UL
#define UART4_BASE      0x40004C00UL
#define UART5_BASE      0x40005000UL
#define USART6_BASE     0x40011400UL

#define USART1 ((UART_TypeDef *)USART1_BASE)
#define USART2 ((UART_TypeDef *)USART2_BASE)
#define USART3 ((UART_TypeDef *)USART3_BASE)
#define UART4 ((UART_TypeDef *)UART4_BASE)
#define UART5 ((UART_TypeDef *)UART5_BASE)
#define USART6 ((UART_TypeDef *)USART6_BASE)

void UART_Init(UART_TypeDef *uart, uint32_t baudrate);
void UART_WriteChar(UART_TypeDef *uart, char c);
void UART_WriteString(UART_TypeDef *uart, const char *str);
char UART_ReadChar(UART_TypeDef *uart);

#endif