#include <stdint.h>
#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "drivers/gpio.h"

static void LedTask(void *argument);


void main(void){
    GPIO_EnableClock(GPIOA);
    GPIO_SetMode(GPIOA,5,GPIO_MODE_OUTPUT);

    xTaskCreate(
        LedTask,         /* task function */
        "LED",           /* task name */
        128,             /* stack depth in words */
        NULL,            /* parameter */
        1,               /* priority */
        NULL             /* task handle */
    );

    vTaskStartScheduler();

    while (1)
    {
        /* Should never reach here unless scheduler fails */
    }
}

static void LedTask(void *argument)
{
    (void)argument;

    while (1)
    {
        GPIO_ToggleOutputPin(GPIOA, 5);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}