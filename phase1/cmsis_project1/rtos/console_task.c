#include "rtos/console_task.h"
#include "rtos/rtos_objects.h"
#include "drivers/uart.h"
#include "motion/motion_types.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "bsp/console.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define CMD_LINE_MAX 64U

//static void uart_write_str(const char *s);
static void process_command(char *line);
static BaseType_t send_motion_cmd(motion_cmd_type_t type, uint32_t value);
static char *skip_spaces(char *s);



void CommandTask(void *argument)
{
    char line[CMD_LINE_MAX];
    uint32_t idx = 0U;
    uint8_t c;

    (void)argument;

    Console_WriteStringLocked("\r\nCommand interface ready\r\n");

    for (;;)
    {
        if (Console_ReadByte(&c))
        {
            /* Optional local echo */
            //UART_WriteByte(&g_uart2, c);
            Console_WriteByte(c);

            if (c == '\r' || c == '\n')
            {
                //UART_WriteByte(&g_uart2, '\r');
                //UART_WriteByte(&g_uart2, '\n');
                Console_WriteStringLocked("\r\n");


                line[idx] = '\0';

                if (idx > 0U)
                {
                    process_command(line);
                }

                idx = 0U;
            }
            else if ((c == '\b' || c == 127U))
            {
                if (idx > 0U)
                {
                    idx--;
                }
            }
            else
            {
                if (idx < (CMD_LINE_MAX - 1U))
                {
                    line[idx++] = (char)c;
                }
                else
                {
                    idx = 0U;
                    //uart_write_str("ERR line too long\r\n");
                    Console_WriteStringLocked("ERR line too long\r\n");
                }
            }
        }

        vTaskDelay(pdMS_TO_TICKS(5));
    }
}


static void process_command(char *line)
{
    char *arg;
    long value;

    if (strcmp(line, "enable") == 0)
    {
        if (send_motion_cmd(CMD_ENABLE, 0U) == pdPASS)
            Console_WriteStringLocked("OK\r\n");
        else
            Console_WriteStringLocked("ERR queue\r\n");
        return;
    }

    if (strcmp(line, "disable") == 0)
    {
        if (send_motion_cmd(CMD_DISABLE, 0U) == pdPASS)
            Console_WriteStringLocked("OK\r\n");
        else
            Console_WriteStringLocked("ERR queue\r\n");
        return;
    }

    if (strcmp(line, "stop") == 0)
    {
        if (send_motion_cmd(CMD_STOP, 0U) == pdPASS)
            Console_WriteStringLocked("OK\r\n");
        else
            Console_WriteStringLocked("ERR queue\r\n");
        return;
    }

    //if (strcmp(line, "status") == 0)
    //{
    //    extern bool Stepper_IsBusy(void);
    //    extern uint32_t Stepper_GetStepsDone(void);
    //    extern uint32_t Stepper_GetTargetSteps(void);
//
    //    char buf[96];
    //    snprintf(buf, sizeof(buf),
    //             "busy=%lu steps=%lu target=%lu\r\n",
    //             (unsigned long)Stepper_IsBusy(),
    //             (unsigned long)Stepper_GetStepsDone(),
    //             (unsigned long)Stepper_GetTargetSteps());
    //    Console_WriteStringLocked(buf);
    //    return;
    //}

    if (strncmp(line, "dir", 3) == 0)
    {
        arg = skip_spaces(line + 3);
        value = strtol(arg, NULL, 10);

        if ((value == 0L) || (value == 1L))
        {
            if (send_motion_cmd(CMD_SET_DIR, (uint32_t)value) == pdPASS)
                Console_WriteStringLocked("OK\r\n");
            else
                Console_WriteStringLocked("ERR queue\r\n");
        }
        else
        {
            Console_WriteStringLocked("ERR dir must be 0 or 1\r\n");
        }
        return;
    }

    if (strncmp(line, "rate", 4) == 0)
    {
        arg = skip_spaces(line + 4);
        value = strtol(arg, NULL, 10);

        if (value > 0L)
        {
            if (send_motion_cmd(CMD_SET_RATE, (uint32_t)value) == pdPASS)
                Console_WriteStringLocked("OK\r\n");
            else
                Console_WriteStringLocked("ERR queue\r\n");
        }
        else
        {
            Console_WriteStringLocked("ERR invalid rate\r\n");
        }
        return;
    }

    if (strncmp(line, "move", 4) == 0)
    {
        arg = skip_spaces(line + 4);
        value = strtol(arg, NULL, 10);

        if (value > 0L)
        {
            if (send_motion_cmd(CMD_MOVE, (uint32_t)value) == pdPASS)
                Console_WriteStringLocked("OK\r\n");
            else
                Console_WriteStringLocked("ERR queue\r\n");
        }
        else
        {
            Console_WriteStringLocked("ERR invalid move\r\n");
        }
        return;
    }

    Console_WriteStringLocked("ERR unknown command\r\n");
}


static BaseType_t send_motion_cmd(motion_cmd_type_t type, uint32_t value)
{
    motion_cmd_t cmd;
    cmd.type = type;
    cmd.value = value;

    return xQueueSend(g_motion_cmd_queue, &cmd, pdMS_TO_TICKS(10));
}

static char *skip_spaces(char *s)
{
    while ((*s == ' ') || (*s == '\t'))
    {
        s++;
    }
    return s;
}

//static void uart_write_str(const char *s)
//{
//    while (*s != '\0')
//    {
//        UART_WriteByte(&g_uart2, (uint8_t)(*s));
//        s++;
//    }
//}