#ifndef DRIVE_H
#define DRIVE_H

#include "stdint.h"
#include "event.h"
#include "motor.h"
void DriveHandler(EVENT_T event);
void driveInit(void);
void move(uint16_t speed);
uint16_t fixedOrientaion(uint16_t desiredOrientation);
uint16_t getInitialheading(void);
#endif