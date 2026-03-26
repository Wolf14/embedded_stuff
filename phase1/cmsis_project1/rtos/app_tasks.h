#ifndef APP_TASKS_H
#define APP_TASKS_H

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "motion/motion_types.h"

extern QueueHandle_t g_motion_cmd_queue;
extern TaskHandle_t g_motion_task_handle;

void AppTasks_Create(void);



#endif