#include "uart.h"

extern struct RCC_Type *RCC;
extern struct GPIO_Type *GPIOA;
extern UART_TypeDef * const USART2;

void Board_UART2_InitPinsAndClock(void)
{
    /* Enable USART2 clock */
    RCC->APB1ENR |= (1U << 17);

    /* Enable GPIOA clock */
    RCC->AHB1ENR |= (1U << 0);

    /* PA2, PA3 -> alternate function */
    GPIOA->MODER &= ~((3U << (2U * 2U)) | (3U << (3U * 2U)));
    GPIOA->MODER |=  ((2U << (2U * 2U)) | (2U << (3U * 2U)));

    /* AF7 for USART2 on PA2/PA3 */
    GPIOA->AFRL &= ~((0xFU << (2U * 4U)) | (0xFU << (3U * 4U)));
    GPIOA->AFRL |=  ((7U << (2U * 4U)) | (7U << (3U * 4U)));

    /* High speed */
    GPIOA->OSPEEDR |= ((3U << (2U * 2U)) | (3U << (3U * 2U)));

    /* NVIC setup belongs here too */
}