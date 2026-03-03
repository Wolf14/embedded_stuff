#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdint.h>
#include <stdbool.h>

#define CONSOLE_LINE_MAX 128U

void     console_init(void);
void     console_on_rx_char(char c);
bool     console_line_available(void);
uint16_t console_read_line(char *dst, uint16_t maxlen);
bool     console_overflowed(void);
void     console_clear_overflow(void);
void     console_reset(void);

#endif /* CONSOLE_H */