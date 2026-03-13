#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdbool.h>
#include "include/device.h"

/* ================= REGISTER STRUCT ================= */

typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t RESERVED1;         // 0x18
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t RESERVED2;         // 0x18
    volatile uint32_t RESERVED3;         // 0x1C
    volatile uint32_t RESERVED4;         // 0x18
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
} TIM_TypeDef;


/* ================= TIMER INSTANCES ================= */
/* Timer base addresses (example STM32F4 style) */

#define TIM1_BASE   0x40010000UL
#define TIM2_BASE   0x40000000UL
#define TIM3_BASE   0x40000400UL
#define TIM4_BASE   0x40000800UL
#define TIM5_BASE   0x40000C00UL
#define TIM6_BASE   0x40001000UL
#define TIM7_BASE   0x40001400UL
#define TIM8_BASE   0x40010400UL

#define TIM6    ((TIM_TypeDef*)TIM6_BASE)
#define TIM7    ((TIM_TypeDef*)TIM7_BASE)


/* ================= CONFIG STRUCT ================= */

typedef struct
{
    uint32_t prescaler;
    uint32_t auto_reload;
    bool     auto_reload_preload;
    bool     one_pulse_mode;
} TIM_Config_t;


/* ================= API ================= */

void TIM_EnableClock(TIM_TypeDef *tim);
void TIM_DisableClock(TIM_TypeDef *tim);
void TIM_Init(TIM_TypeDef *tim, const TIM_Config_t *config);
void TIM_Start(TIM_TypeDef *tim);
void TIM_Stop(TIM_TypeDef *tim);

void TIM_SetPrescaler(TIM_TypeDef *tim, uint32_t value);
void TIM_SetAutoReload(TIM_TypeDef *tim, uint32_t value);

void TIM_EnableUpdateInterrupt(TIM_TypeDef *tim);
void TIM_DisableUpdateInterrupt(TIM_TypeDef *tim);

bool TIM_IsUpdateEvent(TIM_TypeDef *tim);
void TIM_ClearUpdateFlag(TIM_TypeDef *tim);

void TIM_GenerateUpdateEvent(TIM_TypeDef *tim);


/* ===== NEW EVENT API ===== */

void TIM_EventSet(void);
bool TIM_EventGet(void);

#endif