#include <stdint.h>

typedef struct
{
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t pc;
    uint32_t psr;
} HardFaultFrame;

void HardFault_Handler_C(HardFaultFrame *frame)
{
    while (1);
}

void Default_Handler(void){
    while(1){
        
    }
}