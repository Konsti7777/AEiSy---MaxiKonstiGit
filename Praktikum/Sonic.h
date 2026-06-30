#ifndef SONIC_H
#define SONIC_H

#include <stdint.h>
#include "Event.h"

void SonicInit(void);
uint16_t SonicReadNextSensor(void);
void SonicHandler(EVENT_T event);

/* Get distance from specific sensor (0=left, 1=middle, 2=right) */
uint16_t SonicGetDistanceByIndex(uint8_t index);

/* Get distances by sensor position */
uint16_t SonicGetDistanceMiddle(void);       /* middle sensor */
uint16_t SonicGetDistanceLeft(void);   /* left sensor */
uint16_t SonicGetDistanceRight(void);  /* right sensor */

#endif