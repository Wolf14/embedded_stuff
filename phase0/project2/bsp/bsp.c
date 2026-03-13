#include <stdint.h>
#include <stdbool.h>

#include "bsp.h"
#include "include/device.h"
#include "drivers/gpio.h"
#include "core/nvic.h"
#include "drivers/timer.h"
#include "drivers/uart.h"
#include "drivers/dma.h"

/* ================= IRQ NUMBERS ================= */

#define TIM6_DAC_IRQn      54
#define USART2_IRQn        38
#define DMA1_Stream6_IRQn  17

/* ================= BOARD CONFIG ================= */

#define LED_PIN 5U

/* ================= STATIC BOARD-OWNED INSTANCES ================= */

static UART_Handle_t s_huart2;

static DMA_Handle_t s_uart2_tx_dma = {
    .controller      = DMA1,
    .stream          = DMA1_Stream6,
    .channel         = 4U,
    .is_high_stream  = 1U,
    .flag_shift      = 16U
};

static DMA_Handle_t s_uart2_rx_dma = {
    .controller      = DMA1,
    .stream          = DMA1_Stream5,
    .channel         = 4U,
    .is_high_stream  = 0U,   // Stream5 uses LISR/LIFCR on STM32F4
    .flag_shift      = 22U   // stream 5 FEIF bit position base in LISR group
};

/* ================= INTERNAL HELPERS ================= */

static void BSP_LED_Init(void)
{
    GPIO_EnableClock(GPIOA);
    GPIO_SetMode(GPIOA, LED_PIN, GPIO_MODE_OUTPUT);
}

static void BSP_TIM6_Init(void)
{
    TIM_EnableClock(TIM6);

    TIM_Config_t tim6_cfg =
    {
        .prescaler = 16000U - 1U,     /* 16 MHz -> 1 kHz */
        .auto_reload = 1000U - 1U,    /* 1 second period */
        .auto_reload_preload = true,
        .one_pulse_mode = false
    };

    TIM_Init(TIM6, &tim6_cfg);
    TIM_EnableUpdateInterrupt(TIM6);
    TIM_GenerateUpdateEvent(TIM6);
    NVIC_EnableIRQ(TIM6_DAC_IRQn);
    TIM_Start(TIM6);
}

static void BSP_UART2_Init(void)
{
    /*
     * Assumes your UART driver and/or existing low-level setup handles:
     * - USART2 clock enable
     * - GPIO pin mux for PA2/PA3 if not already done elsewhere
     *
     * If not, move that board-specific pin setup here.
     */

    UART_Init(&s_huart2, USART2, 115200);
    UART_AttachTxDMA(&s_huart2, &s_uart2_tx_dma);

    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}

/* ================= PUBLIC API ================= */

void BSP_Init(void)
{
    BSP_LED_Init();
    BSP_TIM6_Init();
    BSP_UART2_Init();
}

UART_Handle_t *BSP_ConsoleUart(void)
{
    return &s_huart2;
}

/* ================= IRQ WRAPPERS ================= */

void USART2_IRQHandler(void)
{
    UART_IRQHandler(USART2);
}

void DMA1_Stream6_IRQHandler(void)
{
    DMA_IRQHandler(&s_uart2_tx_dma);
}