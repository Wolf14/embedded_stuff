#ifndef RTOS_OBJECTS_H
#define RTOS_OBJECTS_H

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "motion/motion_types.h"
#include "semphr.h"

extern QueueHandle_t g_motion_cmd_queue;
extern TaskHandle_t g_motion_task_handle;
extern TaskHandle_t g_command_task_handle;
extern SemaphoreHandle_t g_console_mutex;

#endif