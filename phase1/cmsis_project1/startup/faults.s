.syntax unified
.cpu cortex-m4
.thumb

.section .text.HardFault_Handler
.global HardFault_Handler
.type HardFault_Handler, %function

HardFault_Handler:
    tst lr, #4
    ite eq
    mrseq r0, msp
    mrsne r0, psp

    bl HardFault_Handler_C
    b .

.extern HardFault_Handler_C
