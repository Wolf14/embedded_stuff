.syntax unified
.cpu cortex-m4
.thumb

.section .isr_vector, "a", %progbits
.global g_pfnVectors

g_pfnVectors:
    .word _estack
    .word Reset_Handler
    .word Default_Handler
    .word HardFault_Handler

.section .text.Reset_Handler
.global Reset_Handler
.type Reset_Handler, %function

Reset_Handler:
    bl  runtime_init
    b   .

.extern _estack
.extern runtime_init
.extern Default_Handler
.extern HardFault_Handler
