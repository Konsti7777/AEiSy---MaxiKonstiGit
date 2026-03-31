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
#include "Led.h"


/* ----------- V A R I A B L E S   &  C O N S T A N T S  --------------- */

extern unsigned long SystemCoreClock;

volatile unsigned long g_systickMs = 0U;



/* ------------- F u n c t i o n  P r o t o t y p e s  ----------------- */

void SysTick_Handler(void);



/* ----------------------- F U N C T I O N S  -------------------------- */

void SysTick_Handler(void)
{
	g_systickMs++;

	if ((g_systickMs % 500U) == 0U)
	{
		SetEvent(EVT_TMR_500MS, 0U, 0UL);
	}
}



/* --------------  S t a r t    o f    p r o g r a m  -----------------  */

int main(void){
	EVENT_T currentEvent;

	/* Initialize event queue and system clock value */
	InitEventHandler();
	Led_Init();
	SystemCoreClockUpdate();

	/* 1 ms SysTick interrupt */
	SysTick_Config(SystemCoreClock / 1000U);

	/* Endlosschleife */
	while(1)
	{
		/* Consumer: fetch next event from queue */
		currentEvent = GetEvent();

		/* Dispatch events to software modules */
		Led_HandleEvent(&currentEvent);
	}
}
