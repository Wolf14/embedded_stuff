#include "include/device.h"
#include "drivers/exti.h"
#include "drivers/gpio.h"

void EXTI15_10_IRQHandler(void){
    /* code basically resets pending bit and performs work done by specific interrupt */
    if (EXTI_IsPending(13)){
        EXTI_ClearPending(13);
        GPIO_ToggleOutputPin(GPIOA, 5);
    }

}