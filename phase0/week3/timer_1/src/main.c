#include <stdint.h>
#include <stdbool.h>

#include "include/device.h"
#include "drivers/gpio.h"
#include "core/nvic.h"
#include "drivers/timer.h"
#include "drivers/uart.h"


#define TIM6_DAC_IRQn 54
#define LED_PIN 5
#define USART2_IRQn 38
/* ============ GLOBALS =============*/

UART_Handle_t huart2;

/* =============== MAIN =================*/

int main(void)
{
    UART_Init(&huart2, USART2, 115200);

    // Enable clock
    TIM_EnableClock(TIM6);

    GPIO_EnableClock(GPIOA);    /* For LED */

    /* LED PA5 */
    GPIO_SetMode(GPIOA, LED_PIN, GPIO_MODE_OUTPUT);


    /* ---------- TIMER CONFIG ---------- */

    TIM_Config_t tim6_cfg =
    {
        .prescaler = 16000 - 1,      // Assuming 16 MHz timer clock → 1 kHz
        .auto_reload = 1000 - 1,     // 2 second
        .auto_reload_preload = true,
        .one_pulse_mode = false
    };

    TIM_Init(TIM6, &tim6_cfg);

    /* ---------- ENABLE INTERRUPT ---------- */
    TIM_EnableUpdateInterrupt(TIM6);

    TIM_GenerateUpdateEvent(TIM6);

    /* NVIC enable (device specific IRQ number) */
    NVIC_EnableIRQ(TIM6_DAC_IRQn);

    NVIC_EnableIRQ(USART2_IRQn);

    /* ---------- START TIMER ---------- */
    TIM_Start(TIM6);

    uint32_t cnt = 0;
    /* ---------- MAIN LOOP ---------- */
    while (1)
    {
        if (cnt == 1000){
            cnt = 0;
        }
        //
        //if (cnt > 100)   // pick any threshold you want
        //{
        //    GPIO_WritePin(GPIOA, 5, 1);   // force LED ON
        //}

        if (TIM_EventGet()){
            GPIO_ToggleOutputPin(GPIOA, 5);
            UART_WriteString(&huart2, "Count");
            UART_WriteUint(&huart2, cnt);
            UART_WriteString(&huart2, "\r\n");
            cnt++;
        }
        
    }
}