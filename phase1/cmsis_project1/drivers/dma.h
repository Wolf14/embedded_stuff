#ifndef DMA_H
#define DMA_H

#include <stdint.h>
#include <stddef.h>

/* -------- Generic register qualifiers -------- */
#ifndef __IO
#define __IO volatile
#endif

/* -------- Minimal classic STM32 DMA register layout -------- */
typedef struct
{
    __IO uint32_t CR;
    __IO uint32_t NDTR;
    __IO uint32_t PAR;
    __IO uint32_t M0AR;
    __IO uint32_t M1AR;
    __IO uint32_t FCR;
} DMA_Stream_TypeDef;

typedef struct
{
    __IO uint32_t LISR;
    __IO uint32_t HISR;
    __IO uint32_t LIFCR;
    __IO uint32_t HIFCR;
} DMA_TypeDef;

/* -------- Callback status -------- */
typedef enum
{
    DMA_EVENT_TC,   /* Transfer complete */
    DMA_EVENT_TE,   /* Transfer error */
    DMA_EVENT_FE,   /* FIFO error */
    DMA_EVENT_DME   /* Direct mode error */
} DMA_Event_t;

typedef void (*DMA_Callback_t)(void *context, DMA_Event_t event);

/* -------- DMA configuration -------- */
typedef struct
{
    DMA_TypeDef         *controller;
    DMA_Stream_TypeDef  *stream;

    uint8_t channel;          /* CHSEL value */
    uint8_t is_high_stream;   /* 0 => stream in LISR/LIFCR group, 1 => HISR/HIFCR */
    uint8_t flag_shift;       /* bit-group offset for this stream inside ISR/IFCR */

    uintptr_t peripheral_addr;

    DMA_Callback_t callback;
    void *callback_context;
} DMA_Handle_t;




#define DMA1_BASE            0x40026000UL
#define DMA2_BASE            0x40026400UL

#define DMA1 ((DMA_TypeDef *)DMA1_BASE)
#define DMA2 ((DMA_TypeDef *)DMA2_BASE)


#define DMA1_Stream0 ((DMA_Stream_TypeDef *)(DMA1_BASE + 0x10U))
#define DMA1_Stream1 ((DMA_Stream_TypeDef *)(DMA1_BASE + 0x28U))
#define DMA1_Stream2 ((DMA_Stream_TypeDef *)(DMA1_BASE + 0x40U))
#define DMA1_Stream3 ((DMA_Stream_TypeDef *)(DMA1_BASE + 0x58U))
#define DMA1_Stream4 ((DMA_Stream_TypeDef *)(DMA1_BASE + 0x70U))
#define DMA1_Stream5 ((DMA_Stream_TypeDef *)(DMA1_BASE + 0x88U))
#define DMA1_Stream6 ((DMA_Stream_TypeDef *)(DMA1_BASE + 0xA0U))
#define DMA1_Stream7 ((DMA_Stream_TypeDef *)(DMA1_BASE + 0xB8U))

/* -------- API -------- */
void DMA_InitMemToPeriphTx(DMA_Handle_t *hdma);
int  DMA_StartTx(DMA_Handle_t *hdma, const void *src, size_t len);
void DMA_Abort(DMA_Handle_t *hdma);
void DMA_IRQHandler(DMA_Handle_t *hdma);

#endif