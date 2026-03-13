#ifndef EXTI_H
#define EXTI_H

#include <stdint.h>

typedef struct
{
    volatile uint32_t IMR;
    volatile uint32_t EMR;
    volatile uint32_t RTSR;
    volatile uint32_t FTSR;
    volatile uint32_t SWIER;
    volatile uint32_t PR;
} EXTI_TypeDef;

#define EXTI_BASE 0x40013C00UL
#define EXTI ((EXTI_TypeDef *)EXTI_BASE)

/* RCC */
#define RCC_APB2ENR (*(volatile uint32_t *)0x40023844)


void EXTI_EnableInterrupt(uint8_t exti_line);
void EXTI_SetFallingEdge(uint8_t exti_line);
uint8_t EXTI_IsPending(uint8_t exti_line);
void EXTI_ClearPending(uint8_t exti_line);

#endif