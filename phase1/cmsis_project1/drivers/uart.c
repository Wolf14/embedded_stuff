#include "uart.h"

/* ================= BIT DEFINITIONS ================= */

/* SR bits (classic STM32-style naming) */
#define UART_SR_PE        (1U << 0)
#define UART_SR_FE        (1U << 1)
#define UART_SR_NE        (1U << 2)
#define UART_SR_ORE       (1U << 3)
#define UART_SR_RXNE      (1U << 5)
#define UART_SR_TC        (1U << 6)
#define UART_SR_TXE       (1U << 7)

#define UART_CR1_RE       (1U << 2)
#define UART_CR1_TE       (1U << 3)
#define UART_CR1_RXNEIE   (1U << 5)
#define UART_CR1_TCIE     (1U << 6)
#define UART_CR1_TXEIE    (1U << 7)
#define UART_CR1_UE       (1U << 13)

#define UART_CR3_DMAT     (1U << 7)



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

static inline uint16_t UART_NextIndex(uint16_t index, uint16_t size)
{
    return (uint16_t)((index + 1U) % size);
}

static bool UART_TxBufferEmpty(const UART_Handle_t *huart)
{
    return (huart->tx_head == huart->tx_tail);
}

static bool UART_TxBufferFull(const UART_Handle_t *huart)
{
    return (UART_NextIndex(huart->tx_head, UART_TX_BUFFER_SIZE) == huart->tx_tail);
}

static bool UART_RxBufferEmpty(const UART_Handle_t *huart)
{
    return (huart->rx_head == huart->rx_tail);
}

static bool UART_RxBufferFull(const UART_Handle_t *huart)
{
    return (UART_NextIndex(huart->rx_head, UART_RX_BUFFER_SIZE) == huart->rx_tail);
}

static int UART_InstanceToIndex(USART_TypeDef *uart)
{
    if (uart == USART1) return 0;
    if (uart == USART2) return 1;
    if (uart == USART3) return 2;
    if (uart == UART4)  return 3;
    if (uart == UART5)  return 4;
    if (uart == USART6) return 5;
    return -1;
}


/*

// * On classic STM32 UART, reading SR then DR clears some error conditions.
// * We do that sequence in the IRQ error path.

static void UART_HandleErrors(UART_Handle_t *huart, uint32_t sr)
{
    if ((sr & (UART_SR_PE | UART_SR_FE | UART_SR_NE | UART_SR_ORE)) != 0U)
    {
        volatile uint32_t dummy;
        dummy = huart->instance->SR;
        dummy = huart->instance->DR;
        (void)dummy;

        huart->error_count++;

        if (huart->callback != NULL)
        {
            huart->callback(huart->callback_context, UART_EVENT_ERROR);
        }
    }
}

*/

static void UART_HandleRx(UART_Handle_t *huart)
{
    if ((huart->instance->SR & UART_SR_RXNE) != 0U)
    {
        uint8_t byte = (uint8_t)(huart->instance->DR & 0xFFU);

        if (!UART_RxBufferFull(huart))
        {
            huart->rx_buffer[huart->rx_head] = byte;
            huart->rx_head = UART_NextIndex(huart->rx_head, UART_RX_BUFFER_SIZE);

            if (huart->callback != NULL)
            {
                huart->callback(huart->callback_context, UART_EVENT_RX_READY);
            }
        }
        else
        {
            /* RX buffer overrun in software: drop newest byte, count as error */
            huart->error_count++;

            if (huart->callback != NULL)
            {
                huart->callback(huart->callback_context, UART_EVENT_ERROR);
            }
        }
    }
}


static void UART_HandleTx(UART_Handle_t *huart)
{
    if ((huart->instance->SR & UART_SR_TXE) != 0U)
    {
        if (!UART_TxBufferEmpty(huart))
        {
            huart->instance->DR = huart->tx_buffer[huart->tx_tail];
            huart->tx_tail = UART_NextIndex(huart->tx_tail, UART_TX_BUFFER_SIZE);
        }
        else
        {
            UART_DisableTXEInterrupt(huart);
            huart->tx_irq_active = 0U;

            if (huart->callback != NULL)
            {
                huart->callback(huart->callback_context, UART_EVENT_TX_COMPLETE);
            }
        }
    }
}






static void UART_EnableClock(USART_TypeDef *uart)
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

    GPIOA->AFR[0] &= ~(0xF << (pin * 4));
    GPIOA->AFR[0] |= (7 << (pin * 4));         /* As the pin is mapped to AF7 according the reference manual*/

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


/* DMA -> UART adapter */
static void UART_OnDmaEvent(void *context, DMA_Event_t event)
{
    UART_Handle_t *huart = (UART_Handle_t *)context;

    if (huart == NULL)
    {
        return;
    }

    huart->instance->CR3 &= ~UART_CR3_DMAT;
    huart->tx_dma_active = 0U;

    if (event == DMA_EVENT_TC)
    {
        /*
         * Simpler version: signal complete when DMA has fed all bytes to DR.
         * More exact production behavior:
         *   enable TCIE here and signal completion only when UART_SR_TC sets.
         */
        if (huart->callback != NULL)
        {
            huart->callback(huart->callback_context, UART_EVENT_TX_COMPLETE);
        }
    }
    else
    {
        huart->error_count++;

        if (huart->callback != NULL)
        {
            huart->callback(huart->callback_context, UART_EVENT_ERROR);
        }
    }
}



/* ============= GetHandle ===================*/

UART_Handle_t* UART_GetHandle(USART_TypeDef *uart){
    for(int i = 0; i < UART_MAX_INSTANCES; i++){
        if(uart_handle_table[i] && uart_handle_table[i]->instance == uart){
            return uart_handle_table[i];
        }
    }
    return 0;
}


/* ================= INIT ================= */

/* Assumes USAR2 as enabling GPIO pins 2 and 3 only*/
void UART_Init(UART_Handle_t *huart, USART_TypeDef *instance, uint32_t baud)
{
    UART_EnableClock(instance);
    if ((huart == NULL) || (instance == NULL) || (baud == 0U))
        return;

    huart->instance      = instance;
    huart->tx_head       = 0U;
    huart->tx_tail       = 0U;
    huart->rx_head       = 0U;
    huart->rx_tail       = 0U;
    huart->tx_irq_active = 0U;
    huart->tx_dma_active = 0U;
    huart->error_count   = 0U;
    huart->tx_dma        = 0;

    /*
     * Clock and GPIO are intentionally NOT configured here.
     * That belongs in board support code.
     */

    instance->CR1 = 0U;
    instance->BRR = 16000000UL / baud;
    instance->CR1 = UART_CR1_TE | UART_CR1_RE | UART_CR1_UE;

    UART_EnableRXNEInterrupt(huart);
    UART_RegisterHandle(huart);

    if(instance == USART2){
        UART_GPIO_Init_UART(2);
        UART_GPIO_Init_UART(3);
    }

}


/* ================= WRITE ================= */

UART_Status_t UART_WriteByte(UART_Handle_t *huart, uint8_t byte)
{
    if ((huart == NULL) || (huart->instance == NULL))
    {
        return UART_STATUS_INVALID_ARG;
    }

    if (huart->tx_dma_active)
    {
        return UART_STATUS_BUSY;
    }

    if (UART_TxBufferFull(huart))
    {
        return UART_STATUS_FULL;
    }

    huart->tx_buffer[huart->tx_head] = byte;
    huart->tx_head = UART_NextIndex(huart->tx_head, UART_TX_BUFFER_SIZE);

    /*
     * Enable TXE interrupt to let ISR drain the buffer.
     * Safe to set repeatedly.
     */
    UART_EnableTXEInterrupt(huart);
    huart->tx_irq_active = 1U;

    return UART_STATUS_OK;
}


UART_Status_t UART_Write(UART_Handle_t *huart, const uint8_t *data, size_t len)
{
    if ((huart == NULL) || (data == NULL))
    {
        return UART_STATUS_INVALID_ARG;
    }

    for (size_t i = 0; i < len; i++)
    {
        UART_Status_t st = UART_WriteByte(huart, data[i]);
        if (st != UART_STATUS_OK)
        {
            return st;
        }
    }

    return UART_STATUS_OK;
}

UART_Status_t UART_WriteString(UART_Handle_t *huart, const char *str)
{
    if ((huart == NULL) || (str == NULL))
    {
        return UART_STATUS_INVALID_ARG;
    }

    while (*str != '\0')
    {
        UART_Status_t st = UART_WriteByte(huart, (uint8_t)(*str));
        if (st != UART_STATUS_OK)
        {
            return st;
        }
        str++;
    }

    return UART_STATUS_OK;
}

UART_Status_t UART_WriteUint(UART_Handle_t *huart, uint32_t value)
{
    char buf[10];
    uint32_t i = 0U;

    if (huart == NULL)
    {
        return UART_STATUS_INVALID_ARG;
    }

    if (value == 0U)
    {
        return UART_WriteByte(huart, (uint8_t)'0');
    }

    while (value > 0U)
    {
        buf[i++] = (char)('0' + (value % 10U));
        value /= 10U;
    }

    while (i > 0U)
    {
        i--;
        UART_Status_t st = UART_WriteByte(huart, (uint8_t)buf[i]);
        if (st != UART_STATUS_OK)
        {
            return st;
        }
    }

    return UART_STATUS_OK;
}

bool UART_RxAvailable(const UART_Handle_t *huart)
{
    if (huart == NULL)
    {
        return false;
    }

    return !UART_RxBufferEmpty(huart);
}

UART_Status_t UART_ReadByte(UART_Handle_t *huart, uint8_t *out)
{
    if ((huart == NULL) || (out == NULL))
    {
        return UART_STATUS_INVALID_ARG;
    }

    if (UART_RxBufferEmpty(huart))
    {
        return UART_STATUS_EMPTY;
    }

    *out = huart->rx_buffer[huart->rx_tail];
    huart->rx_tail = UART_NextIndex(huart->rx_tail, UART_RX_BUFFER_SIZE);

    return UART_STATUS_OK;
}


/* As of now code assumes that USART2 is being used and hence GPIOA->AF7, need to make this more generalistic*/
void UART_IRQHandler(USART_TypeDef *instance)
{
    UART_Handle_t *huart = UART_GetHandle(instance);
    if ((huart == NULL) || (instance == NULL))
    {
        return;
    }

    /* Handle line/status errors first */
    //UART_HandleErrors(huart, instance->SR);

    /* RX before TX is a common choice for responsiveness */
    if ((instance->SR & UART_SR_RXNE) != 0U)
    {
        UART_HandleRx(huart);
    }

    if ((instance->SR & UART_SR_TXE) != 0U)
    {
        UART_HandleTx(huart);
    }
}

//char UART_ReadChar(USART_TypeDef *uart){
//    while (!(uart->SR & (1 << 5)));
//    return uart->DR;
//}

uint8_t UART_Read_Byte(USART_TypeDef *uart)
{
    /* Reading RDR clears RXNE on most STM32 families */
    return (uint8_t)(uart->DR & 0xFFU);
}


/*-------------------------------------------*/

void UART_AttachTxDMA(UART_Handle_t *huart, DMA_Handle_t *hdma)
{
    if ((huart == NULL) || (hdma == NULL))
    {
        return;
    }

    huart->tx_dma = hdma;

    hdma->peripheral_addr  = (uintptr_t)&huart->instance->DR;
    hdma->callback         = UART_OnDmaEvent;
    hdma->callback_context = huart;

    DMA_InitMemToPeriphTx(hdma);
}


UART_Status_t UART_TransmitDMA(UART_Handle_t *huart, const uint8_t *data, uint16_t len)
{
    if ((huart == NULL) || (data == NULL) || (len == 0U))
    {
        return UART_STATUS_INVALID_ARG;
    }

    if (huart->tx_dma == NULL)
    {
        return UART_STATUS_INVALID_ARG;
    }

    if (huart->tx_dma_active || huart->tx_irq_active || !UART_TxBufferEmpty(huart))
    {
        return UART_STATUS_BUSY;
    }

    UART_DisableTXEInterrupt(huart);
    huart->tx_irq_active = 0U;

    if (DMA_StartTx(huart->tx_dma, data, len) != 0)
    {
        return UART_STATUS_BUSY;
    }

    huart->tx_dma_active = 1U;
    huart->instance->CR3 |= UART_CR3_DMAT;

    return UART_STATUS_OK;
}
