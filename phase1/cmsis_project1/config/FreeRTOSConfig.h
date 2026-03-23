#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <stdint.h>

/* CPU / clock */
#define configCPU_CLOCK_HZ                     ( ( unsigned long ) 16000000 )
#define configTICK_RATE_HZ                     ( ( TickType_t ) 1000 )
#define configUSE_PREEMPTION                   1
#define configUSE_TIME_SLICING                 1
#define configUSE_16_BIT_TICKS                 0
#define configUSE_IDLE_HOOK                    0
#define configUSE_TICK_HOOK                    0
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0

/* Tasking */
#define configMAX_PRIORITIES                   5
#define configMINIMAL_STACK_SIZE               128
#define configTOTAL_HEAP_SIZE                  ( ( size_t ) ( 16 * 1024 ) )
#define configMAX_TASK_NAME_LEN                16
#define configUSE_TRACE_FACILITY               0
#define configUSE_MUTEXES                      1
#define configUSE_RECURSIVE_MUTEXES            0
#define configUSE_COUNTING_SEMAPHORES          0
#define configQUEUE_REGISTRY_SIZE              0
#define configUSE_QUEUE_SETS                   0
#define configCHECK_FOR_STACK_OVERFLOW         0
#define configUSE_MALLOC_FAILED_HOOK           0

/* Software timers */
#define configUSE_TIMERS                       0
#define configTIMER_TASK_PRIORITY              2
#define configTIMER_QUEUE_LENGTH               4
#define configTIMER_TASK_STACK_DEPTH           128

/* API inclusion */
#define INCLUDE_vTaskDelay                     1
#define INCLUDE_vTaskDelete                    1
#define INCLUDE_vTaskSuspend                   1
#define INCLUDE_xTaskDelayUntil                1
#define INCLUDE_xTaskGetSchedulerState         1

/* Cortex-M interrupt priority configuration */
#define configPRIO_BITS                        4
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY 15
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5

#define configKERNEL_INTERRUPT_PRIORITY \
    ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

#define configMAX_SYSCALL_INTERRUPT_PRIORITY \
    ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/* Optional assert */
#define configASSERT(x) if ((x) == 0) { taskDISABLE_INTERRUPTS(); for( ;; ); }

/* Map FreeRTOS port interrupt handlers to CMSIS names */
#define vPortSVCHandler    SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

#endif