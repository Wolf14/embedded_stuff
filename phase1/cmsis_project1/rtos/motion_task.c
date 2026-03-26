#include "rtos/motion_task.h"
#include "motion/stepper.h"
#include "motion/motion_types.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "bsp/console.h"

static uint32_t g_current_rate_hz = 1000U;
static uint8_t g_current_dir = 0U;

void MotionTask(void *argument){
    motion_cmd_t cmd;
    uint32_t events;

    (void)argument;

    Stepper_Init();
    Stepper_SetRateHz(g_current_rate_hz);
    Stepper_SetDir(g_current_dir);

    for(;;){
        if(xQueueReceive(g_motion_cmd_queue, &cmd, portMAX_DELAY) == pdPASS){
        //if(xQueueReceive(g_motion_cmd_queue, &cmd, pdMS_TO_TICKS(10)) == pdPASS){
            switch(cmd.type){
                case CMD_ENABLE:
                    Stepper_Enable(true);
                    Console_WriteStringLocked("Enabled Stepper\r\n");
                    break;

                case CMD_DISABLE:
                    Stepper_Enable(false);
                    break;

                case CMD_SET_DIR:
                    g_current_dir = (cmd.value != 0U) ? 1U : 0U;
                    Stepper_SetDir(g_current_dir);
                    Console_WriteStringLocked("Set motor direction COMPLETE\r\n");
                    break;

                case CMD_SET_RATE:
                    if(cmd.value > 0U){
                        g_current_rate_hz = cmd.value;
                        Stepper_SetRateHz(g_current_rate_hz);

                    }
                    Console_WriteStringLocked("SET RATE COMMAND Complete\r\n");
                    break;

                case CMD_MOVE:
                    (void)Stepper_StartMove(cmd.value);
                    Console_WriteStringLocked("Executed Stepper Move\r\n");
                    break;

                case CMD_STOP:
                    Stepper_Stop();
                    Console_WriteStringLocked("Stepper Stopped\r\n");
                    break;

                default:
                    break;
            }
        }
        
        /* check ISR notifications */
        if (xTaskNotifyWait(0, 0xFFFFFFFF, &events, 0) == pdPASS)
        {
            if (events & MOTION_EVENT_DONE)
            {
                Console_WriteStringLocked("MOVE COMPLETE\r\n");
            }
        }
    }
}