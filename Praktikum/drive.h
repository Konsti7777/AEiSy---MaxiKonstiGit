#ifndef DRIVE_H
#define DRIVE_H

#include "stdint.h"
#include "event.h"
#include "motor.h"

void driveInit(void);
void move(uint16_t speed);
#endif