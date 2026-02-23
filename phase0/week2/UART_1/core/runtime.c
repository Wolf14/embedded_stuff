#include <stdint.h>

extern uint32_t _sdata;
extern uint32_t _sidata;
extern uint32_t _edata;

extern uint32_t _sbss;
extern uint32_t _ebss;

extern int main(void);

void runtime_init(void)
{
    uint32_t* src;
    uint32_t* dst;

    src = &_sidata;
    dst = &_sdata;

    while(dst < &_edata){
        *dst++ = *src++;
    }

    dst = &_sbss;
    while(dst < &_ebss){
        *dst++ = 0U;
    }

    (void)main();

    while(1){
        /* trap */
    }
}