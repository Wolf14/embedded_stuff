#include <stdint.h>

#include "include/device.h"
#include "drivers/gpio.h"
#include "drivers/exti.h"
#include "drivers/syscfg.h"
#include "drivers/nvic.h"

#define LED_PIN    5
#define BUTTON_PIN 13
#define BUTTON_PORT 2 

/* ================= MAIN ================= */

int main(void)
{
    /* Enable clocks */
    GPIO_EnableClock(GPIOA);    /* For LED */
    GPIO_EnableClock(GPIOC);    /* For Button */
    SYSCFG_EnableClock();

    /* LED PA5 */
    GPIO_SetMode(GPIOA, LED_PIN, GPIO_MODE_OUTPUT);

    /* Button PC13 */
    GPIO_SetMode(GPIOC, BUTTON_PIN, GPIO_MODE_INPUT);

    /* Map EXTI13 → Port C */
    SYSCFG_SetEXTISource(BUTTON_PIN, BUTTON_PORT);

    /* Configure EXTI */
    EXTI_EnableInterrupt(BUTTON_PIN);
    EXTI_SetFallingEdge(BUTTON_PIN);     // button press
    
    EXTI_ClearPending(BUTTON_PIN);      /* To clear any garbage value that might have been set due to noise during startup(spurios trigger condition)*/

    /* NVIC enable IRQ */
    NVIC_EnableIRQ(40);   // EXTI15_10 IRQ number

    while (1)
    {
        /* CPU sleeps waiting for interrupt */
        __asm__("wfi");   // optional low power
    }
}