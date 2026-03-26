#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "rtos/app_tasks.h"
#include "drivers/timer.h"
#include "drivers/gpio.h"
#include "drivers/uart.h"
#include "bsp/console.h"

#define LED_PIN 5U
#define BUTTON_PIN 13
#define BUTTON_PORT 2

/* ================= IRQ NUMBERS ================= */

//#define TIM6_DAC_IRQn      54
//#define USART2_IRQn        38
//#define DMA1_Stream6_IRQn  17
//#define EXTI15_10_IRQn     40
#define MOTOR_ENABLE_PIN    11
#define MOTOR_STEP_PIN  12

static UART_Handle_t s_huart2;

static void Board_Init(void);
static void UART2_Blocking_SendString(const char *s);
static void UART2_Blocking_SendChar(char c);
static void Demo_QueueTest(void);

static void BSP_TIM6_Init(void)
{
    TIM_EnableClock(TIM6);

    TIM_Config_t tim6_cfg =
    {
        .prescaler = 16U - 1U,     /* 16 MHz -> 1 kHz */
        .auto_reload = 1000U - 1U,    /* 10 millisecond period */
        .auto_reload_preload = true,
        .one_pulse_mode = false
    };

    TIM_Init(TIM6, &tim6_cfg);
    TIM_EnableUpdateInterrupt(TIM6);
    TIM_GenerateUpdateEvent(TIM6);

    NVIC_SetPriority(TIM6_DAC_IRQn, 15);
    NVIC_EnableIRQ(TIM6_DAC_IRQn);
    //TIM_Start(TIM6);
}

//static void BSP_UART2_Init(void)
//{
//    /*
//     * Assumes your UART driver and/or existing low-level setup handles:
//     * - USART2 clock enable
//     * - GPIO pin mux for PA2/PA3 if not already done elsewhere
//     *
//     * If not, move that board-specific pin setup here.
//     */
//
//    UART_Init(&s_huart2, USART2, 115200);
//    //UART_AttachTxDMA(&s_huart2, &s_uart2_tx_dma);
//
//    NVIC_EnableIRQ(USART2_IRQn);
//    //NVIC_EnableIRQ(DMA1_Stream6_IRQn);
//}


int main(void){
    Board_Init();

    AppTasks_Create();

    Demo_QueueTest();

    vTaskStartScheduler();

    while(1){

    }
}

//static void UART2_Blocking_SendChar(char c)
//{
//    while ((USART2->SR & (1U << 7)) == 0U) { }   // wait for TXE
//    USART2->DR = (uint8_t)c;
//}

//static void UART2_Blocking_SendString(const char *s)
//{
//    while (*s)
//    {
//        UART2_Blocking_SendChar(*s++);
//    }
//
//    while ((USART2->SR & (1U << 6)) == 0U) { }   // wait for TC
//}

static void Board_Init(){
    BSP_TIM6_Init();
    //BSP_UART2_Init();
    Console_Init();

    //UART2_Blocking_SendString("BOOT\r\n");

}

static void Demo_QueueTest(){
    Console_WriteString("D\r\n");
    
    
    motion_cmd_t cmd;

    cmd.type = CMD_ENABLE;
    cmd.value = 0U;
    (void)xQueueSend(g_motion_cmd_queue, &cmd, 0U);

    cmd.type = CMD_SET_DIR;
    cmd.value = 1U;
    (void)xQueueSend(g_motion_cmd_queue, &cmd, 0U);

    cmd.type = CMD_SET_RATE;
    cmd.value = 1000U;
    (void)xQueueSend(g_motion_cmd_queue, &cmd, 0U);

    cmd.type = CMD_MOVE;
    cmd.value = 1000U;
    (void)xQueueSend(g_motion_cmd_queue, &cmd, 0U);
    Console_WriteString("Q\r\n");
}