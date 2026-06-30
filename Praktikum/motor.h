#ifndef MOTOR_H
#define MOTOR_H

#include "stdint.h"
#include "event.h"
// Initialisierung von TIM4 und GPIOs
void Motor_Init(void);

// Fahrfunktionen (speed: 0 bis 100)
void Motor_DriveForward(uint8_t speed);
void Motor_DriveBackward(uint8_t speed);
void Motor_TurnCenteredClockwise(uint8_t speed);
void Motor_TurnCenteredCounterClockwise(uint8_t speed);
void Motor_Stop(void);
void MotorHandler(EVENT_T event);
#endif