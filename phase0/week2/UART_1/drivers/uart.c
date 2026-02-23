#include "uart.h"


static void UART_EnableClock(UART_TypeDef *uart){
    if (uart == USART2){
        RCC->APB1ENR |= (1 << 17);
    }
    else if (uart == USART1){
        RCC->APB2ENR |= (1 << 4);
    }
}

static void  UART_GPIO_Init_UART(uint8_t pin){
    RCC->AHB1ENR |= (1 << 0);

    GPIOA->MODER &= ~(3 << (pin * 2));
    GPIOA->MODER |= (2 << (pin * 2));

    GPIOA->AFRL &= ~(0xF << (pin * 4));
    GPIOA->AFRL |= (7 << (pin * 4));

    /*optional HIGH SPEED SETTING*/
    GPIOA->OSPEEDR |= (3 << (pin * 2));
}

/*----------------------------------------------------------------*/

void UART_Init(UART_TypeDef *uart, uint32_t baudrate){
    UART_EnableClock(uart);

    if (uart == USART2){
        UART_GPIO_Init_UART(2);     /*  For Tx   */
        UART_GPIO_Init_UART(3);     /*  For Rx   */
    }

    uart->CR1 = 0;

    uint32_t periph_clk = 16000000UL;
    uart->BRR = periph_clk / baudrate;

    uart->CR1 |= (1 << 3);  /*  Transmitter Enable  */
    uart->CR1 |= (1 << 2);  /*  Receiver Enable */
    uart->CR1 |= (1 << 13);  /*  USART Enable */
}

void UART_WriteChar(UART_TypeDef *uart, char c){
    while(!(uart->SR & (1 << 7)));

    uart->DR = c;
}

void UART_WriteString(UART_TypeDef *uart, const char* str){
    while(*str)
    {
        UART_WriteChar(uart, *str++);
    }
}

char UART_ReadChar(UART_TypeDef* uart){
    while(!(uart->SR & (1 << 5)));

    return (char)(uart->DR);
}