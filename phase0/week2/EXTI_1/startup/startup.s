.syntax unified
.cpu cortex-m4
.thumb

.section .isr_vector, "a", %progbits
.global g_pfnVectors

g_pfnVectors:
    .word _estack
    .word Reset_Handler
    .word NMI_Handler       /* NMI Handler*/
    .word HardFault_Handler
    .word Default_Handler /* MemManage */
    .word Default_Handler /* BusFault */
    .word Default_Handler /* UsageFault */
    .word 0
    .word 0
    .word 0
    .word 0
    .word Default_Handler /* SVC */
    .word Default_Handler /* DebugMon */
    .word 0
    .word Default_Handler /* PendSV */
    .word Default_Handler /* SysTick */

    /* External interrupts */
    .word Default_Handler /* WWDG */
    .word Default_Handler /* PVD */
    .word Default_Handler /* TAMP_STAMP */
    .word Default_Handler /* RTC_WKUP */
    .word Default_Handler /* FLASH */
    .word Default_Handler /* RCC */
    .word Default_Handler /* EXTI0 */
    .word Default_Handler /* EXTI1 */
    .word Default_Handler /* EXTI2 */
    .word Default_Handler /* EXTI3 */
    .word Default_Handler /* EXTI4 */
    .word Default_Handler /* DMA1_Stream0 */
    .word Default_Handler /* DMA1_Stream1 */
    .word Default_Handler /* DMA1_Stream2 */
    .word Default_Handler /* DMA1_Stream3 */
    .word Default_Handler /* DMA1_Stream4 */
    .word Default_Handler /* DMA1_Stream5 */
    .word Default_Handler /* DMA1_Stream6 */
    .word Default_Handler /* ADC */
    .word Default_Handler /* CAN1_TX */
    .word Default_Handler /* CAN1_RX0 */
    .word Default_Handler /* CAN1_RX1 */
    .word Default_Handler /* CAN1_SCE */
    .word Default_Handler /* EXTI9_5 */
    .word Default_Handler /* TIM1_BRK_TIM9 */
    .word Default_Handler /* TIM1_UP_TIM10 */
    .word Default_Handler /* TIM1_TRG_COM_TIM11 */
    .word Default_Handler /* TIM1_CC */
    .word Default_Handler /* TIM2 */
    .word Default_Handler /* TIM3 */
    .word Default_Handler /* TIM4 */
    .word Default_Handler /* I2C1_EV */
    .word Default_Handler /* I2C1_ER */
    .word Default_Handler /* I2C2_EV */
    .word Default_Handler /* I2C2_ER */
    .word Default_Handler /* SPI1 */
    .word Default_Handler /* SPI2 */
    .word Default_Handler /* USART1 */
    .word Default_Handler /* USART2 */
    .word Default_Handler /* USART3 */
    .word EXTI15_10_IRQHandler /* EXTI15_10 */   /* IRQ 40 */

.section .text.Reset_Handler
.global Reset_Handler
.type Reset_Handler, %function

Reset_Handler:
    bl runtime_init
    b   .

.extern runtime_init
.extern _estack
.extern Default_Handler
.extern HardFault_Handler


/* ================= WEAK ALIASES ================= */

.weak NMI_Handler
.thumb_set NMI_Handler, Default_Handler

.weak SVC_Handler
.thumb_set SVC_Handler, Default_Handler

.weak PendSV_Handler
.thumb_set PendSV_Handler, Default_Handler

.weak SysTick_Handler
.thumb_set SysTick_Handler, Default_Handler

.weak EXTI0_IRQHandler
.thumb_set EXTI0_IRQHandler, Default_Handler

.weak EXTI1_IRQHandler
.thumb_set EXTI1_IRQHandler, Default_Handler

.weak EXTI2_IRQHandler
.thumb_set EXTI2_IRQHandler, Default_Handler

.weak EXTI3_IRQHandler
.thumb_set EXTI3_IRQHandler, Default_Handler

.weak EXTI4_IRQHandler
.thumb_set EXTI4_IRQHandler, Default_Handler

.weak EXTI9_5_IRQHandler
.thumb_set EXTI9_5_IRQHandler, Default_Handler

.weak EXTI15_10_IRQHandler
.type EXTI15_10_IRQHandler, %function
EXTI15_10_IRQHandler:
    bl Default_Handler
    b .
    