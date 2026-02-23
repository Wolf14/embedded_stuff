#include "HAL/device.h"
#include "drivers/led.h"
#include "drivers/gpio.h"

#define LED_PIN 5


int main(void)
{
    LED_Init();
    GPIO_EnableClock(GPIOC);
    GPIO_SetMode(GPIOC,13,GPIO_MODE_INPUT);
    //GPIO_SetPull(GPIOC, 13, GPIO_PULLUP); //Not required as external pull up resistor is used

    while(1){
        //LED_Toggle();
        //LED_Blink(100000);
        if (GPIO_ReadPin(GPIOC,13))
            LED_On();
        else{
            LED_Off();
        }
    }
}