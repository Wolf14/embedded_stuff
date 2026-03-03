#include "uart.h"

/* ================= BIT DEFINITIONS ================= */

#define UART_SR_RXNE      (1U << 5)
#define UART_SR_TXE       (1U << 7)

#define UART_CR1_RE       (1U << 2)
#define UART_CR1_TE       (1U << 3)
#define UART_CR1_RXNEIE   (1U << 5)
#define UART_CR1_TXEIE    (1U << 7)
#define UART_CR1_UE       (1U << 13)



/* ================ LOOKUP TABLE ====================== */

#define UART_MAX_INSTANCES 5
static UART_Handle_t *uart_handle_table[UART_MAX_INSTANCES] = {0};

/* ================= INTERNAL HELPERS ================= */
static void UART_EnableTXEInterrupt(UART_Handle_t *huart){
    huart->instance->CR1 |= UART_CR1_TXEIE;
}

static void UART_DisableTXEInterrupt(UART_Handle_t *huart){
    huart->instance->CR1 &= ~UART_CR1_TXEIE;
}

static void UART_EnableRXNEInterrupt(UART_Handle_t *huart){
    huart->instance->CR1 |= UART_CR1_RXNEIE;
}

static void UART_DisableRXNEInterrupt(UART_Handle_t *huart){
    huart->instance->CR1 &= ~UART_CR1_RXNEIE;
}

static bool buffer_empty(UART_Handle_t *huart){
    return (huart->tx_head == huart->tx_tail);
}

static bool buffer_full(UART_Handle_t *huart){
    return ((huart->tx_head + 1)%UART_TX_BUFFER_SIZE) == huart->tx_tail;
}


static void UART_EnableClock(UART_TypeDef *uart)
{
    if (uart == USART2){
        RCC->APB1ENR |= (1 << 17);
    }
    else if (uart == USART1){
        RCC->APB2ENR |= (1 << 4);
    }
}

/* As of now code assumes that USART being used is mapped to GPIOA(And also assumes mapped to AFRL and not AFRH), need to make this more generalistic*/
static void UART_GPIO_Init_UART(uint8_t pin){
    if ((RCC-> AHB1ENR & (1 << 0)) == 0){
        RCC->AHB1ENR    |= (1 << 0);
    }

    GPIOA->MODER &= ~(3 << (pin * 2));
    GPIOA->MODER |= (2 << (pin * 2));

    GPIOA->AFRL &= ~(0xF << (pin * 4));
    GPIOA->AFRL |= (7 << (pin * 4));         /* As the pin is mapped to AF7 according the reference manual*/

    GPIOA->OSPEEDR |= (3 << (pin * 2));
}

static void UART_RegisterHandle(UART_Handle_t *huart){
    if (huart->instance == USART1)
        uart_handle_table[0] = huart;

    else if (huart->instance == USART2)
        uart_handle_table[1] = huart;

    else if (huart->instance == USART3)
        uart_handle_table[2] = huart;

    else if (huart->instance == UART4)
        uart_handle_table[3] = huart;

    else if (huart->instance == UART5)
        uart_handle_table[4] = huart;
}

/* ============= GetHandle ===================*/

UART_Handle_t* UART_GetHandle(UART_TypeDef *uart){
    for(int i = 0; i < UART_MAX_INSTANCES; i++){
        if(uart_handle_table[i] && uart_handle_table[i]->instance == uart){
            return uart_handle_table[i];
        }
    }
    return 0;
}


/* ================= INIT ================= */

/* Assumes USAR2 as enabling GPIO pins 2 and 3 only*/
void UART_Init(UART_Handle_t *huart, UART_TypeDef *instance, uint32_t baud)
{
    huart->instance = instance;
    huart->tx_head = 0;
    huart->tx_tail = 0;

    UART_RegisterHandle(huart);

    UART_EnableClock(instance);
    if(instance == USART2){
        UART_GPIO_Init_UART(2);
        UART_GPIO_Init_UART(3);
    }

    /* Clock + GPIO config assumed already done */

    uint32_t periph_clk = 16000000UL;
    instance->BRR = periph_clk / baud;

    instance->CR1 = 0;
    instance->CR1 |= UART_CR1_TE | UART_CR1_RE;
    instance->CR1 |= UART_CR1_UE;

    UART_EnableRXNEInterrupt(huart);
}


/* ================= WRITE ================= */

void UART_WriteChar(UART_Handle_t *huart, char c)
{
    uint16_t next = (huart->tx_head + 1) % UART_TX_BUFFER_SIZE;

    /* Wait if buffer full (optional strategy) */
    while (next == huart->tx_tail);

    huart->tx_buffer[huart->tx_head] = c;
    huart->tx_head = next;

    /* Enable interrupt to start transmission */
    UART_EnableTXEInterrupt(huart);

}


void UART_WriteString(UART_Handle_t *huart, const char *str)
{
    while (*str)
    {
        UART_WriteChar(huart, *str++);
    }
}

void UART_WriteUint(UART_Handle_t *huart, uint32_t value){
    char buf[10];
    int i =0;

    if (value == 0){
        UART_WriteChar(huart, '0');
    }

    while(value > 0){
        buf[i++]= '0' + (value%10);
        value = value/10;
    }
    while(i--){
        UART_WriteChar(huart,buf[i]);
    }
}


/* As of now code assumes that USART2 is being used and hence GPIOA->AF7, need to make this more generalistic*/
void UART_IRQHandler(UART_TypeDef *uart)
{
    UART_Handle_t *huart = UART_GetHandle(uart);

    if (uart->SR & UART_SR_TXE)
    {
        if (!buffer_empty(huart))
        {
            uart->DR = huart->tx_buffer[huart->tx_tail];
            huart->tx_tail = (huart->tx_tail + 1) % UART_TX_BUFFER_SIZE;
        }
        else
        {
            /* Nothing left → disable interrupt */
            UART_DisableTXEInterrupt(huart);
        }
    }
}

//char UART_ReadChar(UART_TypeDef *uart){
//    while (!(uart->SR & (1 << 5)));
//    return uart->DR;
//}

uint8_t UART_Read_Byte(UART_TypeDef *uart)
{
    /* Reading RDR clears RXNE on most STM32 families */
    return (uint8_t)(uart->DR & 0xFFU);
}


bool UART_Rx_Ready(UART_TypeDef *uart)
{
    /* Example for newer STM32 families */
    return ((uart->SR & UART_SR_RXNE) != 0U);
}