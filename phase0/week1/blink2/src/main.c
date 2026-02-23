#include "HAL/device.h"
#include "drivers/led.h"

#define LED_PIN 5

int main(void)
{
    LED_Init();
    while(1){
        //LED_Toggle();
        LED_Blink(100000);
    }
}