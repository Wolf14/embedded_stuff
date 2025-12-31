#include<stdint.h>

#define RCC_AHB1ENR     (*(volatile uint32_t*)0x40023830) 
#define GPIOA_MODER     (*(volatile uint32_t*)0x40020000)
#define GPIO_ODR        (*(volatile uint32_t*)0x40020014)

int main(void)
{
    RCC_AHB1ENR |= (1 << 0);
    GPIOA_MODER |= (1 << 10);

    while(1)
    {
        GPIO_ODR ^= (1 << 5);
        for (volatile int i = 0; i < 100000; i++);
    }
}