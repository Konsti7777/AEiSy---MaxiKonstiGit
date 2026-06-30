#ifndef CMPS14_H
#define CMPS14_H

#include "stdint.h"
#include "event.h"

uint16_t CMPS14_GetHeading(void);
void CmpsHandler(EVENT_T event);
#endif

