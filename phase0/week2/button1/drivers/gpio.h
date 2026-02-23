#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include "HAL/device.h"

typedef enum{
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT
} gpio_mode_t;

typedef enum{
    GPIO_NOPULL,
    GPIO_PULLUP,
    GPIO_PULLDOWN,
} gpio_pull_t;

void GPIO_EnableClock(GPIO_TypeDef *port);
void GPIO_SetMode(GPIO_TypeDef *port, uint8_t pin, gpio_mode_t mode);
uint8_t GPIO_ReadPin(GPIO_TypeDef *port, uint8_t pin);
void GPIO_WritePin(GPIO_TypeDef *port, uint8_t pin, uint8_t value);
void GPIO_SetPull(GPIO_TypeDef *port, uint8_t pin, gpio_pull_t value);

#endif