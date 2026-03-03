#include "include/device.h"
#include "drivers/uart.h"
#include "drivers/timer.h"
#include "src/console.h"

void USART2_IRQHandler(void){
    if (UART_Rx_Ready(USART2))
    {
        char c = (char)UART_Read_Byte(USART2);
        console_on_rx_char(c);
    }
    UART_IRQHandler(USART2);
}


void TIM6_DAC_IRQHandler(void)
{
    if (TIM_IsUpdateEvent(TIM6))
    {
        TIM_ClearUpdateFlag(TIM6);
        TIM_EventSet();
    }
}