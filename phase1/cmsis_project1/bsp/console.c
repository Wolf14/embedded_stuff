#include "bsp/console.h"
#include "drivers/uart.h"
#include "stm32f4xx.h"
#include "rtos/rtos_objects.h"

//#define USART2_IRQn 38

static UART_Handle_t s_console_uart;

void Console_Init(void)
{
    /*
     * If your UART_Init() does not configure GPIO alternate function pins,
     * do that here before UART_Init().
     */

    UART_Init(&s_console_uart, USART2, 115200);

    NVIC_SetPriority(USART2_IRQn, 14);
    NVIC_EnableIRQ(USART2_IRQn);
}

bool Console_ReadByte(uint8_t *byte)
{
    if (byte == NULL)
    {
        return false;
    }

    return (UART_ReadByte(&s_console_uart, byte) == UART_STATUS_OK);
}

void Console_WriteByte(uint8_t byte)
{
    (void)UART_WriteByte(&s_console_uart, byte);
}

void Console_WriteStringLocked(const char *str)
{
    if (str == NULL)
    {
        return;
    }

    if (g_console_mutex != NULL)
    {
        if (xSemaphoreTake(g_console_mutex, portMAX_DELAY) == pdPASS)
        {
            Console_WriteString(str);
            xSemaphoreGive(g_console_mutex);
        }
    }
    else
    {
        Console_WriteString(str);
    }
}

void Console_WriteString(const char *str)
{
    if (str == NULL)
    {
        return;
    }

    while (*str != '\0')
    {
        (void)UART_WriteByte(&s_console_uart, (uint8_t)(*str));
        str++;
    }
}