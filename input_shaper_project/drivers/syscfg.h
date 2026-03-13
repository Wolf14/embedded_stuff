#ifndef SYSCFG_H
#define SYSCFG_H

#include <stdint.h>

/* Base address */
#define SYSCFG_BASE 0x40013800UL
typedef struct
{
    volatile uint32_t MEMRMP;      /* Memory remap */
    volatile uint32_t PMC;         /* Peripheral mode */
    volatile uint32_t EXTICR[4];   /* EXTI config */
    volatile uint32_t CMPCR;       /* Compensation cell */
} SYSCFG_TypeDef;

#define SYSCFG ((SYSCFG_TypeDef *)SYSCFG_BASE)

#define SYSCFG_PORT_A   0
#define SYSCFG_PORT_B   1
#define SYSCFG_PORT_C   2
#define SYSCFG_PORT_D   3
#define SYSCFG_PORT_E   4
#define SYSCFG_PORT_F   5
#define SYSCFG_PORT_G   6
#define SYSCFG_PORT_H   7


void SYSCFG_EnableClock(void);
void SYSCFG_SetEXTISource(uint8_t exti_line, uint8_t port);


#endif