#include "include/device.h"
#include "drivers/uart.h"
#include "drivers/timer.h"

void USART2_IRQHandler(void){
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