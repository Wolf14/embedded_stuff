#include <stdint.h>
#include <stdbool.h>

#include "bsp/bsp.h"
#include "console.h"
#include "drivers/uart.h"



#define TIM6_DAC_IRQn 54
#define LED_PIN 5
#define USART2_IRQn 38
#define DMA1_Stream6_IRQn 17


/* ============ GLOBALS =============*/

int main(void)
{
    BSP_Init();

    UART_Handle_t *uart = BSP_ConsoleUart();

    while (1)
    {
        /* Feed console from UART RX ring buffer */
        uint8_t c;
        while (UART_ReadByte(uart, &c) == UART_STATUS_OK)
        {
            console_on_rx_char((char)c);
        }

        if (console_line_available())
        {
            char line[CONSOLE_LINE_MAX+1];
            uint16_t len = console_read_line(line, sizeof(line));
            line[len] = '\n';

            if (len > 0U)
            {
                /* Example DMA TX response */
                static const uint8_t msg[] = "DMA path\r\n";

                UART_Status_t st = UART_TransmitDMA(uart, line, len+1);
                if (st != UART_STATUS_OK)
                {
                    /* Optional fallback / debug handling */
                }
                
            }
        }

        if (console_overflowed())
        {
            console_clear_overflow();
            (void)UART_WriteString(uart, "Error: line too long\r\n");
        }
    }
}