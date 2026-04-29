#ifndef SONIC_H
#define SONIC_H

#include <stdint.h>
#include "Event.h"

void SonicInit(void);
uint16_t SonicReadMiddle(void);
void SonicHandler(EVENT_T event);
uint16_t SonicGetDistance(void);

#endif