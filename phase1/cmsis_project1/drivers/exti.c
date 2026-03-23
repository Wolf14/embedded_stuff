#include "exti.h"


void EXTI_EnableInterrupt(uint8_t exti_line){
    EXTI->IMR |= ((1 << exti_line));
}
void EXTI_SetFallingEdge(uint8_t exti_line){
    EXTI->FTSR |= ((1 << exti_line));
}

uint8_t EXTI_IsPending(uint8_t exti_line){
    return (EXTI->PR & (1 << exti_line)) != 0;
}

void EXTI_ClearPending(uint8_t exti_line){
    EXTI->PR = (1 << exti_line);        /* Important: Writing 1 ( Not 0 ), clears the pending PR on that line*/
}