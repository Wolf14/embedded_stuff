#include "include/device.h"
#include "drivers/timer.h"

void TIM6_DAC_IRQHandler(void)
{
    if (TIM_IsUpdateEvent(TIM6))
    {
        TIM_ClearUpdateFlag(TIM6);
        TIM_EventSet();
    }
}