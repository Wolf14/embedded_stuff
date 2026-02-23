.syntax unified
.cpu cortex-m4
.thumb

.global _estack
.global Reset_Handler

_estack = 0x20020000   /* 128 KB SRAM top */

.section .isr_vector
.word _estack
.word Reset_Handler

.section .text
Reset_Handler:
    bl main
    b .
