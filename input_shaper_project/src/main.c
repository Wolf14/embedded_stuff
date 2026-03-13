#include<stdint.h>

#include "include/device.h"
#include "drivers/gpio.h"
#include "core/nvic.h"
#include "drivers/timer.h"
#include "drivers/uart.h"
#include "drivers/syscfg.h"
#include "drivers/exti.h"

#define LED_PIN 5U
#define BUTTON_PIN 13
#define BUTTON_PORT 2

/* ================= IRQ NUMBERS ================= */

#define TIM6_DAC_IRQn      54
#define USART2_IRQn        38
#define DMA1_Stream6_IRQn  17
#define EXTI15_10_IRQn     40
#define MOTOR_ENABLE_PIN    11
#define MOTOR_STEP_PIN  12

static UART_Handle_t s_huart2;

static void BSP_TIM6_Init(void)
{
    TIM_EnableClock(TIM6);

    TIM_Config_t tim6_cfg =
    {
        .prescaler = 16U - 1U,     /* 16 MHz -> 1 kHz */
        .auto_reload = 1000U - 1U,    /* 10 millisecond period */
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
    //UART_AttachTxDMA(&s_huart2, &s_uart2_tx_dma);

    NVIC_EnableIRQ(USART2_IRQn);
    //NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}

int main(void){
    GPIO_EnableClock(GPIOA);
    GPIO_EnableClock(GPIOC);

    SYSCFG_EnableClock();
    GPIO_SetMode(GPIOA,LED_PIN, GPIO_MODE_OUTPUT);
    GPIO_SetMode(GPIOA,12, GPIO_MODE_OUTPUT);
    GPIO_SetMode(GPIOA,11, GPIO_MODE_OUTPUT);
    GPIO_SetMode(GPIOC,BUTTON_PIN, GPIO_MODE_INPUT);

    SYSCFG_SetEXTISource(BUTTON_PIN, BUTTON_PORT);

    EXTI_EnableInterrupt(BUTTON_PIN);
    EXTI_SetFallingEdge(BUTTON_PIN);

    EXTI_ClearPending(BUTTON_PIN);

    BSP_TIM6_Init();
    BSP_UART2_Init();


    NVIC_EnableIRQ(EXTI15_10_IRQn); // EXTI15_10 IRQ number

    GPIO_WritePin(GPIOA,MOTOR_ENABLE_PIN,1);    // disable 

    while(1){
        if(TIM_EventGet()){
            GPIO_ToggleOutputPin(GPIOA, LED_PIN);
            GPIO_ToggleOutputPin(GPIOA, MOTOR_STEP_PIN);
            UART_WriteString(&s_huart2,"Hello\n");
        }
    }
}