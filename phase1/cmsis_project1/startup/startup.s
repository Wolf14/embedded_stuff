    .syntax unified
    .cpu cortex-m4
    .thumb

    .section .isr_vector, "a", %progbits
    .global g_pfnVectors

    g_pfnVectors:
        .word _estack
        .word Reset_Handler
        .word NMI_Handler
        .word HardFault_Handler
        .word Default_Handler /* MemManage */
        .word Default_Handler /* BusFault */
        .word Default_Handler /* UsageFault */
        .word 0
        .word 0
        .word 0
        .word 0
        .word SVC_Handler     /* SVC */
        .word Default_Handler /* DebugMon */
        .word 0
        .word PendSV_Handler  /* PendSV */
        .word SysTick_Handler /* SysTick */

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
        .word DMA1_Stream6_IRQHandler /* DMA1_Stream6 */
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
        .word USART2_IRQHandler /* USART2 */
        .word Default_Handler /* USART3 */
        .word EXTI15_10_IRQHandler /* EXTI15_10 */   /* IRQ 40 */

        .word Default_Handler      /* RTC_Alarm */
        .word Default_Handler      /* OTG_FS_WKUP */
        .word Default_Handler      /* TIM8_BRK_TIM12 */
        .word Default_Handler      /* TIM8_UP_TIM13 */
        .word Default_Handler      /* TIM8_TRG_COM_TIM14 */
        .word Default_Handler      /* TIM8_CC */
        .word Default_Handler      /* DMA1_Stream7 */
        .word Default_Handler      /* FMC */
        .word Default_Handler      /* SDIO */
        .word Default_Handler      /* TIM5 */
        .word Default_Handler      /* SPI3 */
        .word Default_Handler      /* UART4 */
        .word Default_Handler      /* UART5 */
        .word TIM6_DAC_IRQHandler  /* TIM6_DAC */      /* IRQ 54 */
        .word Default_Handler      /* TIM7 */
        .word Default_Handler      /* DMA2_Stream0 */
        .word Default_Handler      /* DMA2_Stream1 */
        .word Default_Handler      /* DMA2_Stream2 */
        .word Default_Handler      /* DMA2_Stream3 */
        .word Default_Handler      /* DMA2_Stream4 */
        .word Default_Handler      /* ETH */
        .word Default_Handler      /* ETH_WKUP */
        .word Default_Handler      /* CAN2_TX */
        .word Default_Handler      /* CAN2_RX0 */
        .word Default_Handler      /* CAN2_RX1 */
        .word Default_Handler      /* CAN2_SCE */
        .word Default_Handler      /* OTG_FS */
        .word Default_Handler      /* DMA2_Stream5 */
        .word Default_Handler      /* DMA2_Stream6 */
        .word Default_Handler      /* DMA2_Stream7 */
        .word Default_Handler      /* USART6 */
        .word Default_Handler      /* I2C3_EV */
        .word Default_Handler      /* I2C3_ER */
        .word Default_Handler      /* OTG_HS_EP1_OUT */
        .word Default_Handler      /* OTG_HS_EP1_IN */
        .word Default_Handler      /* OTG_HS_WKUP */
        .word Default_Handler      /* OTG_HS */
        .word Default_Handler      /* DCMI */
        .word Default_Handler      /* CRYP */
        .word Default_Handler      /* HASH_RNG */
        .word Default_Handler      /* FPU */

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
    .type SVC_Handler, %function
    SVC_Handler:
        bl Default_Handler
        b .

    .weak PendSV_Handler
    .type PendSV_Handler, %function
    PendSV_Handler:
        bl Default_Handler
        b .

    .weak SysTick_Handler
    .type SysTick_Handler, %function
    SysTick_Handler:
        bl Default_Handler
        b .

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
        
    .weak DMA1_Stream6_IRQHandler
    .type DMA1_Stream6_IRQHandler, %function
    DMA1_Stream6_IRQHandler:
        bl Default_Handler
        b .


    .weak USART2_IRQHandler
    .type USART2_IRQHandler, %function
    USART2_IRQHandler:
        bl Default_Handler
        b .

    .weak TIM6_DAC_IRQHandler
    .type TIM6_DAC_IRQHandler, %function
    TIM6_DAC_IRQHandler:
        bl Default_Handler
        b .