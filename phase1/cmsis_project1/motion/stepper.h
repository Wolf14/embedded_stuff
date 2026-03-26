#ifndef STEPPER_H
#define STEPPER_H

#include <stdint.h>
#include <stdbool.h>
#include "FreeRTOS.h"

void Stepper_Init(void);
void Stepper_Enable(bool en);
void Stepper_SetDir(bool dir);
void Stepper_SetRateHz(uint32_t rate_hz);
BaseType_t Stepper_StartMove(uint32_t steps);
void Stepper_Stop(void);
bool Stepper_IsBusy();
uint32_t Stepper_GetStepsDone(void);
uint32_t Stepper_GetTargetSteps(void);

bool Stepper_TimerISR(void);


#endif
