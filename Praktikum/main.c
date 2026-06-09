/**************************************************************************
 *  COPYRIGHT (C) Peter Raab  2022                                        *
 *								Markus Hager 2026																				*
 *                                                                        *
 *                                                                        *
 **************************************************************************
 *  PROJECT       AEiSy: Px - Basisproject                                *
 *  MODULE        main.c                                                  *
 *  REVISION      1.1                                                     *
 **************************************************************************
 *  PURPOSE:                                                              *
 *   The main routine is the start of every C-program. The purpose of     *
 *   the main is to initialize the hardware and start the operation       *
 *   system.                                                              *
 *                                                                        *
 **************************************************************************
 *  REMARKS:                                                              *
 *                                                                        *
 *                                                                        *
 **************************************************************************
 *  CHANGE HISTORY:                                                       *
 *   Revision  Date         Author      Description                       *
 *     1.1      30.03.2026   Hager         Update                         *
 *                                                                        *
 *                                                                        *
 *************************************************************************/


/* ---------------- G L O B A L   D E F I N I T I O N S ---------------- */

/*  - I n c l u d e s                                                    */

#include <stdint.h>       // uint8_t, uint16_t, uint32_t, int8_t, ...
#include <string.h>       // strlen, memset, memcpy, strcpy, strcmp, ...
#include <stdio.h>        // snprintf, printf, sprintf, ...

//#include <stdbool.h>      // bool, true, false
//#include <stdlib.h>       // malloc, free, atoi, abs, rand, ...
//#include <math.h>         // sin, cos, sqrt, pow, fabs, ...
//#include <stddef.h>       // NULL, size_t, offsetof
//#include <limits.h>       // INT_MAX, UINT8_MAX, ...
//#include <ctype.h>        // isdigit, isalpha, toupper, tolower, ...
//#include <float.h>        // FLT_MAX, DBL_MIN, ...

#include "stm32g474xx.h"
#include "delay.h"
#include "Event.h"
#include "LED.h"
#include "Display.h"
#include "sonic.h"
#include "cmps14.h"
#include "motor.h"
#include "drive.h"

/* ----------- V A R I A B L E S   &  C O N S T A N T S  --------------- */

extern unsigned long SystemCoreClock;
volatile unsigned long systickms;



/* ------------- F u n c t i o n  P r o t o t y p e s  ----------------- */



/* ----------------------- F U N C T I O N S  -------------------------- */

void SysTick_Handler(){
	
	systickms++;
	if((systickms % 500) == 0){
		SetEvent(EVT_500MS_EVT, 0U, 0UL);
		SetEvent(EVT_DISPLAY_SONIC_EVT, 0U, 0UL);
		
	}	
	if ((systickms % 100) == 0)
    {
        SetEvent(EVT_SONIC_EVT, 0U, 0UL);
				SetEvent(EVT_FAHREN_EVT, 0U, 0UL);
    }

	}
/* --------------  S t a r t    o f    p r o g r a m  -----------------  */




int main(void) {
	InitEventHandler();
  EVENT_T currentEvent;
	SysTick_Config(SystemCoreClock / 1000);
	SetEvent(EVT_INIT_EVT,0,0);
	/* Endlosschleife */
	while(1)
	{
		currentEvent = GetEvent();
		//LedHandler(currentEvent);
		DisplayHandler(currentEvent);
		SonicHandler(currentEvent);
		CmpsHandler(currentEvent);
		MotorHandler(currentEvent);
		//DriveHandler(currentEvent);
	}
		//delayms(500);

}

