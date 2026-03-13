#include "timer.h"

/* ===== Bit Definitions ===== */

#define TIM_CR1_CEN        (1U << 0)
#define TIM_CR1_UDIS       (1U << 1)
#define TIM_CR1_URS        (1U << 2)
#define TIM_CR1_OPM        (1U << 3)
#define TIM_CR1_ARPE       (1U << 7)

#define TIM_DIER_UIE       (1U << 0)

#define TIM_SR_UIF         (1U << 0)

#define TIM_EGR_UG         (1U << 0)

/* ===== PRIVATE EVENT FLAG ===== */

static volatile bool tim_event_flag = false;


/* ================= INIT ================= */

void TIM_Init(TIM_TypeDef *tim, const TIM_Config_t *config)
{
    /* Stop timer */
    TIM_Stop(tim);

    /* Configure Prescaler & ARR */
    tim->PSC = config->prescaler;
    tim->ARR = config->auto_reload;

    /* Configure CR1 */

    uint32_t cr1 = 0;

    if (config->auto_reload_preload)
        cr1 |= TIM_CR1_ARPE;

    if (config->one_pulse_mode)
        cr1 |= TIM_CR1_OPM;

    tim->CR1 = cr1;

    /* Force update to load registers */
    TIM_GenerateUpdateEvent(tim);
}


/* ================= BASIC CONTROL ================= */

void TIM_Start(TIM_TypeDef *tim)
{
    tim->CR1 |= TIM_CR1_CEN;
}

void TIM_Stop(TIM_TypeDef *tim)
{
    tim->CR1 &= ~TIM_CR1_CEN;
}


/* ================= CONFIG HELPERS ================= */

void TIM_SetPrescaler(TIM_TypeDef *tim, uint32_t value)
{
    tim->PSC = value;
    TIM_GenerateUpdateEvent(tim);
}

void TIM_SetAutoReload(TIM_TypeDef *tim, uint32_t value)
{
    tim->ARR = value;
    TIM_GenerateUpdateEvent(tim);
}


/* ================= INTERRUPTS ================= */

void TIM_EnableUpdateInterrupt(TIM_TypeDef *tim)
{
    tim->DIER |= TIM_DIER_UIE;
}

void TIM_DisableUpdateInterrupt(TIM_TypeDef *tim)
{
    tim->DIER &= ~TIM_DIER_UIE;
}


bool TIM_IsUpdateEvent(TIM_TypeDef *tim)
{
    return (tim->SR & TIM_SR_UIF);
}

void TIM_ClearUpdateFlag(TIM_TypeDef *tim)
{
    tim->SR = ~TIM_SR_UIF;
}


/* ================= EVENTS ================= */

void TIM_GenerateUpdateEvent(TIM_TypeDef *tim)
{
    tim->EGR |= TIM_EGR_UG;
}


/* ================== CLOCKS =================*/
void TIM_EnableClock(TIM_TypeDef *tim)
{
    uint32_t addr = (uint32_t)tim;

    if (addr == TIM1_BASE)
        RCC->APB2ENR |= (1 << 0);

    else if (addr == TIM2_BASE)
        RCC->APB1ENR |= (1 << 0);

    else if (addr == TIM3_BASE)
        RCC->APB1ENR |= (1 << 1);

    else if (addr == TIM4_BASE)
        RCC->APB1ENR |= (1 << 2);

    else if (addr == TIM5_BASE)
        RCC->APB1ENR |= (1 << 3);

    else if (addr == TIM6_BASE)
    {
        RCC->APB1ENR |= (1 << 4);
        RCC->APB1RSTR |= (1 << 4);
        RCC->APB1RSTR &= ~(1 << 4);
    }
    else if (addr == TIM7_BASE)
        RCC->APB1ENR |= (1 << 5);

    else if (addr == TIM8_BASE)
        RCC->APB2ENR |= (1 << 1);
}


void TIM_DisableClock(TIM_TypeDef *tim)
{
    uint32_t addr = (uint32_t)tim;

    if (addr == TIM1_BASE)
        RCC->APB2ENR &= ~(1 << 0);

    else if (addr == TIM2_BASE)
        RCC->APB1ENR &= ~(1 << 0);

    else if (addr == TIM3_BASE)
        RCC->APB1ENR &= ~(1 << 1);

    else if (addr == TIM4_BASE)
        RCC->APB1ENR &= ~(1 << 2);

    else if (addr == TIM5_BASE)
        RCC->APB1ENR &= ~(1 << 3);

    else if (addr == TIM6_BASE)
        RCC->APB1ENR &= ~(1 << 4);

    else if (addr == TIM7_BASE)
        RCC->APB1ENR &= ~(1 << 5);

    else if (addr == TIM8_BASE)
        RCC->APB2ENR &= ~(1 << 1);
}

/* Called from ISR */
void TIM_EventSet(void)
{
    tim_event_flag = true;
}


/* Called from main */
bool TIM_EventGet(void)
{
    if (tim_event_flag)
    {
        tim_event_flag = false;
        return true;
    }

    return false;
}