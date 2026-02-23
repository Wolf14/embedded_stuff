.syntax unified
.cpu cortex-m4
.thumb

/* ==================================================
 * Vector table
 * ================================================== */
.section .isr_vector, "a", %progbits
.global g_pfnVectors

g_pfnVectors:
    .word _estack          /* Initial stack pointer (loaded by hardware) */
    .word Reset_Handler    /* Reset handler */

/* ==================================================
 * Reset handler
 * ================================================== */
.section .text.Reset_Handler
.global Reset_Handler
.type Reset_Handler, %function

Reset_Handler:
    bl  runtime_init      /* Jump to C runtime initialization */
    b   .                 /* Should never return */

/* ==================================================
 * External symbols
 * ================================================== */
.extern _estack
.extern runtime_init
