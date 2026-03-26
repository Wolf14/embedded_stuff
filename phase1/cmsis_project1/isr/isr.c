#include "include/device.h"
//#include "drivers/exti.h"
#include "drivers/gpio.h"
#include "drivers/uart.h"
#include "drivers/timer.h"

#include "motion/stepper.h"

#include "rtos/rtos_objects.h"
#include "rtos/motion_task.h"


//void EXTI15_10_IRQHandler(void){
//    /* code basically resets pending bit and performs work done by specific interrupt */
//    if (EXTI_IsPending(13)){
//        EXTI_ClearPending(13);
//        GPIO_ToggleOutputPin(GPIOA, 5);
//        GPIO_ToggleOutputPin(GPIOA, 11);
//    }
//
//}

//void USART2_IRQHandler(void){
//    if (UART_Rx_Ready(USART2))
//    {
//        char c = (char)UART_Read_Byte(USART2);
//        console_on_rx_char(c);
//    }
//    UART_IRQHandler(USART2);
//}


void USART2_IRQHandler(void)
{
    UART_IRQHandler(USART2);
}

//void TIM6_DAC_IRQHandler(void)
//{
//    Stepper_TimerISR();
//}

void TIM6_DAC_IRQHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if(Stepper_TimerISR()){
        xTaskNotifyFromISR(
            g_motion_task_handle,
            MOTION_EVENT_DONE,
            eSetBits,
            &xHigherPriorityTaskWoken
        );
    }
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

//void TIM6_DAC_IRQHandler(void)
//{
//    if (TIM_IsUpdateEvent(TIM6))
//    {
//        TIM_ClearUpdateFlag(TIM6);
//        TIM_EventSet();
//    }
//}