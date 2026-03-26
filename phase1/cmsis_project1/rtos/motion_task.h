#ifndef MOTION_TASK_H
#define MOTION_TASK_H

#include "rtos/rtos_objects.h"

#define MOTION_EVENT_DONE   (1UL << 0)

void MotionTask(void *argument);

#endif