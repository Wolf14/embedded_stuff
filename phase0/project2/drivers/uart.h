#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "include/device.h"
#include "drivers/dma.h"


/* ================= CONFIG ================= */

#ifndef UART_TX_BUFFER_SIZE
#define UART_TX_BUFFER_SIZE 128U
#endif

#ifndef UART_RX_BUFFER_SIZE
#define UART_RX_BUFFER_SIZE 128U
#endif

/* ================= REGISTER STRUCT ================= */

typedef struct{
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
} UART_TypeDef;

#define USART1_BASE     0x40011000UL
#define USART2_BASE     0x40004400UL
#define USART3_BASE     0x40004800UL
#define UART4_BASE      0x40004C00UL
#define UART5_BASE      0x40005000UL
#define USART6_BASE     0x40011400UL

#define USART1 ((UART_TypeDef *)USART1_BASE)
#define USART2 ((UART_TypeDef *)USART2_BASE)
#define USART3 ((UART_TypeDef *)USART3_BASE)
#define UART4 ((UART_TypeDef *)UART4_BASE)
#define UART5 ((UART_TypeDef *)UART5_BASE)
#define USART6 ((UART_TypeDef *)USART6_BASE)

/* ================= Status / Events ================= */

typedef enum
{
    UART_STATUS_OK = 0,
    UART_STATUS_BUSY,
    UART_STATUS_FULL,
    UART_STATUS_EMPTY,
    UART_STATUS_INVALID_ARG
} UART_Status_t;

typedef enum
{
    UART_EVENT_RX_READY,
    UART_EVENT_TX_COMPLETE,
    UART_EVENT_ERROR
} UART_Event_t;


typedef void (*UART_EventCallback_t)(void *context, UART_Event_t event);

/* ================= HANDLE ================= */

typedef struct
{
    UART_TypeDef *instance;

    volatile uint16_t tx_head;
    volatile uint16_t tx_tail;

    volatile uint16_t rx_head;
    volatile uint16_t rx_tail;

    uint8_t tx_buffer[UART_TX_BUFFER_SIZE];
    uint8_t rx_buffer[UART_RX_BUFFER_SIZE];

    volatile uint8_t tx_irq_active;
    volatile uint8_t tx_dma_active;
    volatile uint32_t error_count;

    DMA_Handle_t *tx_dma;

    UART_EventCallback_t callback;
    void *callback_context;
} UART_Handle_t;








/* ================= Public API ================= */

/* Core init: assumes board-level clock/GPIO setup is done separately */
void UART_Init(UART_Handle_t *huart, UART_TypeDef *instance, uint32_t baud);

/* Optional board hook registration in internal lookup table */
UART_Handle_t *UART_GetHandle(UART_TypeDef *instance);

/* TX */
UART_Status_t UART_WriteByte(UART_Handle_t *huart, uint8_t byte);
UART_Status_t UART_Write(UART_Handle_t *huart, const uint8_t *data, size_t len);
UART_Status_t UART_WriteString(UART_Handle_t *huart, const char *str);
UART_Status_t UART_WriteUint(UART_Handle_t *huart, uint32_t value);

/* RX */
bool UART_RxAvailable(const UART_Handle_t *huart);
UART_Status_t UART_ReadByte(UART_Handle_t *huart, uint8_t *out);

//Old read functions:
uint8_t UART_Read_Byte(UART_TypeDef *uart);

/* ISR entry */
void UART_IRQHandler(UART_TypeDef *instance);


/* DMA TX API */
UART_Status_t UART_TransmitDMA(UART_Handle_t *huart, const uint8_t *data, uint16_t len);
void UART_AttachTxDMA(UART_Handle_t *huart, DMA_Handle_t *hdma);

#endif