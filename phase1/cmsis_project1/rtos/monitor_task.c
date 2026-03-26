#include "FreeRTOS.h"
#include "task.h"
#include "drivers/gpio.h"
#include "stm32f4xx.h"
#include "bsp/console.h"

void MonitorTask(void *argument){
    (void)argument;

    GPIO_EnableClock(GPIOA);
    GPIO_SetMode(GPIOA, 5U, GPIO_MODE_OUTPUT);

    for(;;){
        GPIO_ToggleOutputPin(GPIOA, 5U);
        Console_WriteString("MON\r\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}