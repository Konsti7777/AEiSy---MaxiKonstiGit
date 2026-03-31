#ifndef _LED_H
#define _LED_H

#include "Event.h"

void Led_Init(void);
void Led_HandleEvent(const EVENT_T* event);

#endif  // _LED_H
