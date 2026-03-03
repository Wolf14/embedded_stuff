#include "gpio.h"

void GPIO_EnableClock(GPIO_TypeDef *port){
    uint8_t index = ((uint32_t)port - GPIOA_BASE)/ 0x400;
    RCC->AHB1ENR |= (1 << index);
}

void GPIO_SetMode(GPIO_TypeDef *port,uint8_t pin, gpio_mode_t mode){
    port->MODER &=  ~(3 << (pin * 2));
    if (mode == GPIO_MODE_OUTPUT){
        port->MODER |= (1 << (pin * 2));
    }
    else if(mode == GPIO_MODE_AF){
        port->MODER |= (2 << (pin * 2));
    }
}

uint8_t GPIO_ReadPin(GPIO_TypeDef *port, uint8_t pin){
    return (port->IDR >> pin) & 1;
}

void GPIO_WritePin(GPIO_TypeDef *port, uint8_t pin, uint8_t value){
    if(value){
        port->BSRR = (1 << (pin));
    }
    else{
        port->BSRR = (1 << (pin + 16)); /* reset pin*/
    }
}

void GPIO_SetPull(GPIO_TypeDef *port, uint8_t pin, gpio_pull_t pull_status){
    port->PUPDR &= ~(3 << (pin * 2));
    if (pull_status == GPIO_PULLUP){
        port->PUPDR |= (1 << (pin * 2));
    }
    else if (pull_status == GPIO_PULLDOWN){
        port->PUPDR |= (2 << (pin * 2));
    }
}

void GPIO_ToggleOutputPin(GPIO_TypeDef *port, uint8_t pin){
   port->BSRR = (port->ODR & (1 << pin)) ? (1 << (pin + 16)) : (1 << pin);
}