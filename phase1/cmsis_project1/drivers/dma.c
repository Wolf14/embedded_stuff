#include "dma.h"
#include "stm32f4xx.h"

/* -------- CR bit definitions (classic STM32 stream DMA style) -------- */
//#define DMA_SxCR_EN            (1U << 0)
//#define DMA_SxCR_DMEIE         (1U << 1)
//#define DMA_SxCR_TEIE          (1U << 2)
//#define DMA_SxCR_TCIE          (1U << 4)

//#define DMA_SxCR_DIR_Pos       6U
#define DMA_SxCR_DIR_M2P       (1U << DMA_SxCR_DIR_Pos)

//#define DMA_SxCR_CIRC          (1U << 8)
//#define DMA_SxCR_PINC          (1U << 9)
//#define DMA_SxCR_MINC          (1U << 10)

//#define DMA_SxCR_PSIZE_Pos     11U
//#define DMA_SxCR_MSIZE_Pos     13U

//#define DMA_SxCR_PL_Pos        16U
//#define DMA_SxCR_CHSEL_Pos     25U

/* FCR bits */
//#define DMA_SxFCR_DMDIS        (1U << 2)  /* 0 => direct mode enabled */
//#define DMA_SxFCR_FEIE         (1U << 7)

/* Common 8-bit width values */
#define DMA_PSIZE_8BIT         (0U << DMA_SxCR_PSIZE_Pos)
#define DMA_MSIZE_8BIT         (0U << DMA_SxCR_MSIZE_Pos)

/* Priority example */
#define DMA_PRIORITY_HIGH      (2U << DMA_SxCR_PL_Pos)

/*
 * Flag layout per stream uses a 6-bit group in ISR/IFCR:
 * FEIFx, DMEIFx, TEIFx, HTIFx, TCIFx
 *
 * We only care about FE/DME/TE/TC here.
 * flag_shift should point to FEIF bit of the stream group.
 */
#define DMA_FLAG_FEIF(shift)   (1U << ((shift) + 0U))
#define DMA_FLAG_DMEIF(shift)  (1U << ((shift) + 2U))
#define DMA_FLAG_TEIF(shift)   (1U << ((shift) + 3U))
#define DMA_FLAG_TCIF(shift)   (1U << ((shift) + 5U))


static void BSP_DMA1_Init(void)
{
    RCC->AHB1ENR |= (1U << 21);   // DMA1EN
}



static uint32_t DMA_GetIsr(const DMA_Handle_t *hdma)
{
    return hdma->is_high_stream ? hdma->controller->HISR : hdma->controller->LISR;
}

static void DMA_ClearFlags(const DMA_Handle_t *hdma, uint32_t mask)
{
    if (hdma->is_high_stream)
    {
        hdma->controller->HIFCR = mask;
    }
    else
    {
        hdma->controller->LIFCR = mask;
    }
}

static void DMA_DisableStream(DMA_Stream_TypeDef *s)
{
    s->CR &= ~DMA_SxCR_EN;

    /* In production code, add timeout protection here. */
    while (s->CR & DMA_SxCR_EN)
    {
        /* wait for hardware to actually disable stream */
    }
}

void DMA_InitMemToPeriphTx(DMA_Handle_t *hdma)
{
    BSP_DMA1_Init();


    DMA_Stream_TypeDef *s = hdma->stream;

    DMA_DisableStream(s);

    /* Clear any stale flags for this stream */
    uint32_t clear_mask =
        DMA_FLAG_FEIF(hdma->flag_shift)  |
        DMA_FLAG_DMEIF(hdma->flag_shift) |
        DMA_FLAG_TEIF(hdma->flag_shift)  |
        DMA_FLAG_TCIF(hdma->flag_shift);

    DMA_ClearFlags(hdma, clear_mask);

    /*
     * Configure:
     * - memory-to-peripheral
     * - memory increment
     * - peripheral fixed
     * - 8-bit peripheral, 8-bit memory
     * - transfer complete + error interrupts
     * - selected channel
     * - direct mode (FIFO disabled for simple UART TX)
     */
    s->CR =
          DMA_SxCR_DIR_M2P
        | DMA_SxCR_MINC
        | DMA_PSIZE_8BIT
        | DMA_MSIZE_8BIT
        | DMA_PRIORITY_HIGH
        | DMA_SxCR_TCIE
        | DMA_SxCR_TEIE
        | DMA_SxCR_DMEIE
        | ((uint32_t)hdma->channel << DMA_SxCR_CHSEL_Pos);

    /* Direct mode enabled: DMDIS = 0 */
    s->FCR = 0U;

    s->PAR = (uint32_t)hdma->peripheral_addr;
    s->NDTR = 0U;
    s->M0AR = 0U;
}

int DMA_StartTx(DMA_Handle_t *hdma, const void *src, size_t len)
{
    DMA_Stream_TypeDef *s = hdma->stream;

    if ((src == NULL) || (len == 0U) || (len > 0xFFFFU))
    {
        return -1;
    }

    if (s->CR & DMA_SxCR_EN)
    {
        return -2;
    }

    /* Clear stale flags before a new start */
    uint32_t clear_mask =
        DMA_FLAG_FEIF(hdma->flag_shift)  |
        DMA_FLAG_DMEIF(hdma->flag_shift) |
        DMA_FLAG_TEIF(hdma->flag_shift)  |
        DMA_FLAG_TCIF(hdma->flag_shift);

    DMA_ClearFlags(hdma, clear_mask);

    s->M0AR = (uint32_t)(uintptr_t)src;
    s->NDTR = (uint32_t)len;

    s->CR |= DMA_SxCR_EN;

    return 0;
}

void DMA_Abort(DMA_Handle_t *hdma)
{
    DMA_DisableStream(hdma->stream);

    uint32_t clear_mask =
        DMA_FLAG_FEIF(hdma->flag_shift)  |
        DMA_FLAG_DMEIF(hdma->flag_shift) |
        DMA_FLAG_TEIF(hdma->flag_shift)  |
        DMA_FLAG_TCIF(hdma->flag_shift);

    DMA_ClearFlags(hdma, clear_mask);
}

void DMA_IRQHandler(DMA_Handle_t *hdma)
{
    uint32_t isr = DMA_GetIsr(hdma);

    uint32_t feif  = DMA_FLAG_FEIF(hdma->flag_shift);
    uint32_t dmeif = DMA_FLAG_DMEIF(hdma->flag_shift);
    uint32_t teif  = DMA_FLAG_TEIF(hdma->flag_shift);
    uint32_t tcif  = DMA_FLAG_TCIF(hdma->flag_shift);

    uint32_t pending = isr & (feif | dmeif | teif | tcif);

    if (pending == 0U)
    {
        return;
    }

    /* Clear flags first */
    DMA_ClearFlags(hdma, pending);

    /* On terminal conditions, stop the stream */
    if (pending & (teif | feif | dmeif | tcif))
    {
        DMA_DisableStream(hdma->stream);
    }

    if (hdma->callback != NULL)
    {
        if (pending & tcif)  { hdma->callback(hdma->callback_context, DMA_EVENT_TC); }
        if (pending & teif)  { hdma->callback(hdma->callback_context, DMA_EVENT_TE); }
        if (pending & feif)  { hdma->callback(hdma->callback_context, DMA_EVENT_FE); }
        if (pending & dmeif) { hdma->callback(hdma->callback_context, DMA_EVENT_DME); }
    }
}  