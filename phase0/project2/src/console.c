#include <stdint.h>
#include "drivers/uart.h"
#include <stdbool.h>
#include <string.h>
#include "console.h"

/*
 * These two functions should be provided elsewhere in your project.
 * For STM32, they would typically disable/enable interrupts globally
 * or just disable/enable the UART RX interrupt.
 */
//extern void console_lock(void);
//extern void console_unlock(void);

/* Private module state */
static volatile char     s_line_buf[CONSOLE_LINE_MAX];
static volatile uint16_t s_line_len       = 0;
static volatile bool     s_line_ready     = false;
static volatile bool     s_line_overflow  = false;
static volatile bool     s_drop_until_eol = false;

void console_init(void)
{
    //console_lock();

    for (uint16_t i = 0; i < CONSOLE_LINE_MAX; i++) {
        s_line_buf[i] = '\0';
    }

    s_line_len       = 0;
    s_line_ready     = false;
    s_line_overflow  = false;
    s_drop_until_eol = false;

    //console_unlock();
}

void console_on_rx_char(char c)
{
    /*
     * Called from UART RX ISR context.
     *
     * Design choice for v1:
     * - If a full line is waiting and main() has not consumed it yet,
     *   ignore all new incoming characters until main clears it.
     */
    if (s_line_ready) {
        return;
    }

    /*
     * If we previously overflowed, keep discarding input until end-of-line.
     * This prevents partial/corrupted lines from being treated as valid.
     */
    if (s_drop_until_eol) {
        if ((c == '\r') || (c == '\n')) {
            s_drop_until_eol = false;
            s_line_len = 0;
        }
        return;
    }

    /* Treat CR or LF as end-of-line */
    if ((c == '\r') || (c == '\n')) {
        /*
         * Ignore empty lines (user just pressed Enter with no content).
         * If you want empty lines to count, remove this check.
         */
        if (s_line_len == 0U) {
            return;
        }

        /* Null-terminate the line so main() can use it as a C-string */
        if (s_line_len < CONSOLE_LINE_MAX) {
            s_line_buf[s_line_len] = '\0';
        } else {
            s_line_buf[CONSOLE_LINE_MAX - 1U] = '\0';
        }

        s_line_ready = true;
        return;
    }

    /* Normal character: append if there is room */
    if (s_line_len < (CONSOLE_LINE_MAX - 1U)) {
        s_line_buf[s_line_len] = c;
        s_line_len++;
    } else {
        /*
         * Buffer full:
         * - mark overflow
         * - discard everything until newline
         * - reset current partial line
         */
        s_line_overflow  = true;
        s_drop_until_eol = true;
        s_line_len       = 0;
    }
}

bool console_line_available(void)
{
    return s_line_ready;
}

uint16_t console_read_line(char *dst, uint16_t maxlen)
{
    if ((dst == NULL) || (maxlen == 0U)) {
        return 0U;
    }

    if (!s_line_ready) {
        return 0U;
    }

    //console_lock();

    uint16_t copy_len = s_line_len;

    /* Ensure destination has room for null terminator */
    if (copy_len >= maxlen) {
        copy_len = maxlen - 1U;
    }

    for (uint16_t i = 0; i < copy_len; i++) {
        dst[i] = (char)s_line_buf[i];
    }
    dst[copy_len] = '\0';

    /* Clear internal state for next line */
    for (uint16_t i = 0; i < CONSOLE_LINE_MAX; i++) {
        s_line_buf[i] = '\0';
    }

    s_line_len   = 0;
    s_line_ready = false;

    //console_unlock();

    return copy_len;
}

bool console_overflowed(void)
{
    return s_line_overflow;
}

void console_clear_overflow(void)
{
    //console_lock();
    s_line_overflow = false;
    //console_unlock();
}

void console_reset(void)
{
    console_init();
}