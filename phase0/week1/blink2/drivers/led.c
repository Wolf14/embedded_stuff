#include "drivers/led.h"
#include "HAL/device.h"

#define LED_PIN 5

void LED_Init(void)
{
    RCC->AHB1ENR |= (1<<0);

    GPIOA->MODER &= ~(3 << (LED_PIN * 2));
    GPIOA->MODER |= (1 << (LED_PIN * 2));
}

void LED_On(void)
{
    GPIOA->BSRR |= (1 << (LED_PIN));
}

void LED_Off(void)
{
    GPIOA->BSRR |= (1 << (LED_PIN + 16));
}

void LED_Toggle(void)
{
    GPIOA->BSRR = GPIOA->ODR & (1 << (LED_PIN)) ? (1 << (LED_PIN + 16)) : (1 << (LED_PIN));
}

void LED_Blink(uint32_t delay_ms)
{
    LED_On();
    volatile int dummy = 0;
    for (volatile uint32_t i = 0; i < delay_ms; i++)
    {
        dummy++;
    }
    LED_Off();
    for (volatile uint32_t i = 0; i < delay_ms; i++){
        dummy--;
    }
}