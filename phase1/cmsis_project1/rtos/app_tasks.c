#include "rtos/app_tasks.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "rtos/motion_task.h"
#include "rtos/console_task.h"
#include "semphr.h"

void MonitorTask(void *argument);

QueueHandle_t g_motion_cmd_queue = NULL;
TaskHandle_t g_motion_task_handle = NULL;
TaskHandle_t g_command_task_handle = NULL;
static TaskHandle_t g_monitor_task_handle = NULL;
SemaphoreHandle_t g_console_mutex = NULL;

void AppTasks_Create(void){
    g_console_mutex = xSemaphoreCreateMutex();
    configASSERT(g_console_mutex != NULL);

    g_motion_cmd_queue = xQueueCreate(8U, sizeof(motion_cmd_t));

    configASSERT(g_motion_cmd_queue != NULL);

    xTaskCreate(
        MotionTask,
        "Motion",
        256U,
        NULL,
        2U,
        &g_motion_task_handle
    );

    xTaskCreate(
        MonitorTask,
        "Monitor",
        256U,
        NULL,
        1U,
        &g_monitor_task_handle
    );

    xTaskCreate(
        CommandTask,
        "Commands",
        256U,
        NULL,
        2U,
        &g_command_task_handle
    );
}