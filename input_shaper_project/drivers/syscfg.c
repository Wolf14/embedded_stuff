#include "syscfg.h"

/* RCC register for SYSCFG clock */
#define RCC_APB2ENR (*(volatile uint32_t*)0x40023844)

#define RCC_SYSCFGEN (1 << 14)

void SYSCFG_EnableClock(void)
{
    RCC_APB2ENR |= RCC_SYSCFGEN;
}


void SYSCFG_SetEXTISource(uint8_t exti_line, uint8_t port)
{
    uint8_t reg_index = exti_line / 4;
    uint8_t shift     = (exti_line % 4) * 4;

    SYSCFG->EXTICR[reg_index] &= ~(0xF << shift);
    SYSCFG->EXTICR[reg_index] |=  (port << shift);
}