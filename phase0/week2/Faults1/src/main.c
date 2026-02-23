#include "include/device.h"
#include <stdint.h>

/* Function to deliberately trigger a HardFault */
void trigger_fault(void)
{
    volatile uint32_t *bad_address = (uint32_t *)0xFFFFFFFF;

    /* Attempt to write to invalid memory */
    *bad_address = 0x12345678;
}

int main(void)
{
    /* Small delay so debugger can attach */
    for (volatile int i = 0; i < 100000; i++);

    trigger_fault();

    while (1)
    {
        /* Should never reach here */
    }
}