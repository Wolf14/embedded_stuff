.syntax unified
.cpu cortex-m4
.thumb

.global HardFault_Handler
.type HardFault_Handler, %function

.extern HardFault_Handler_C
.extern NVIC_SystemReset

HardFault_Handler:
    /* Check which stack pointer was active */
    tst lr, #4
    ite eq
    mrseq r0, msp     /* r0 = MSP if bit == 0 */
    mrsne r0, psp     /* r0 = PSP if bit == 1 */
    
    bl HardFault_Handler_C
    bl NVIC_SystemReset
    b .