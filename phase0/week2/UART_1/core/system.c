#include <stdint.h>

#define SCB_AIRCR (*(volatile uint32_t*)0xE000ED0C)

#define AIRCR_VECTKEY   (0x5FA << 16)
#define AIRCR_SYSRERSETREQ  (1 << 2)

void NVIC_SystemReset(void){
    SCB_AIRCR = AIRCR_VECTKEY | (SCB_AIRCR & (7 << 8)) | AIRCR_SYSRERSETREQ;
}