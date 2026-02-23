#include<stdint.h>
#include "include/device.h"        
#include "drivers/uart.h"
#include "drivers/gpio.h"


int main(void){

    UART_Init(USART2, 115200);

    UART_WriteString(USART2, "Hello World\r\n");

    while(1){

    }
}