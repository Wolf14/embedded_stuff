#include<stdint.h>

#define RCC_AHB1ENR     (*(volatile uint32_t*)0x40023830)

#define GPIOA_MODER     (*(volatile uint32_t*)0x40020000)
#define GPIOA_ODR        (*(volatile uint32_t*)0x40020014)
#define GPIOA_BSRR      (*(volatile uint32_t*)0x40020018)

int main(void)
{
    RCC_AHB1ENR |= (1 << 0);

    GPIOA_MODER &= ~(3 << (5*2));     // set 11th bit to 0
    GPIOA_MODER |= (1 << (5*2));    // set 10th bit to 1

    while(1)
    {
        //GPIOA_ODR ^= (1 << 5);      // flip bit status of 5th bit in ODR register

        // Try using BSRR:
        GPIOA_BSRR = (GPIOA_ODR & (1 << 5)) ? (1 << (16 + 5)) : (1 << 5);

        // while(1)
        // {

        // }
        volatile int dummy = 0;
        for (volatile int i = 0; i < 500000; i++)
        {
            dummy++;
        }
        dummy = 0;
    }
}