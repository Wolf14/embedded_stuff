#include "nvic.h"

void NVIC_EnableIRQ(uint8_t irq)
{
    NVIC->ISER[irq/32] = (1U << (irq % 32));
}

void NVIC_DisableIRQ(uint8_t irq){
    NVIC->ICER[irq/32] = (1U << (irq % 32));
}

void NVIC_SetPendingIRQ(uint8_t irq){
    NVIC->ISPR[irq/32] = (1U << (irq % 32));
}

void NVIC_ClearPendingIRQ(uint8_t irq){
    NVIC->ICPR[irq/32] = (1U << (irq % 32));
}

void NVIC_SetPriority(uint8_t irq, uint8_t priority){
    NVIC->IPR[irq/32] = (priority << 4);
}

/* returns the irq value for the specific interrupt line being used (Assuming pin is a GPIO pin)*/
uint8_t NVIC_GPIOPinToIRQ(uint8_t pin){
    if (pin <= 4)
    {
        return 6 + pin;
    }
    else if (pin <= 9)
    {
        return 23;
    }
    else
    {
        return 40;
    }
}