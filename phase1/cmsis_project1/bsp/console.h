#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdint.h>
#include <stdbool.h>

void Console_Init(void);
bool Console_ReadByte(uint8_t *byte);
void Console_WriteByte(uint8_t byte);
void Console_WriteString(const char *str);
void Console_WriteStringLocked(const char *str);

#endif