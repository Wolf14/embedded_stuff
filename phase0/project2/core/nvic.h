#ifndef NVIC_H
#define NVIC_H

#include <stdint.h>

#define NVIC_BASE 0xE000E100UL

typedef struct{
    volatile uint32_t ISER[8];
    volatile uint32_t ICER[8];
    volatile uint32_t ISPR[8];
    volatile uint32_t ICPR[8];
    volatile uint32_t IABR[8];
    volatile uint8_t IPR[240];
} NVIC_TypeDef;

#define NVIC ((NVIC_TypeDef *)NVIC_BASE)

void NVIC_EnableIRQ(uint8_t irq);
void NVIC_DisableIRQ(uint8_t irq);
void NVIC_SetPendingIRQ(uint8_t irq);
void NVIC_ClearPendingIRQ(uint8_t irq);
void NVIC_SetPriority(uint8_t irq, uint8_t priority);


void NVIC_PinToIRQ(uint8_t pin);


#endif