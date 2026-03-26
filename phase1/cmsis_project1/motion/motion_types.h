#ifndef MOTION_TYPES_H
#define MOTION_TYPES_H

#include <stdint.h>

typedef enum{
    CMD_MOVE = 0,
    CMD_STOP,
    CMD_ENABLE,
    CMD_DISABLE,
    CMD_SET_RATE,
    CMD_SET_DIR
} motion_cmd_type_t;

typedef struct{
    motion_cmd_type_t type;
    uint32_t value;
} motion_cmd_t;

#endif