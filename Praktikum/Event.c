
 //////////////////////////////
 // SOURCE FILE  event.c     //
 //////////////////////////////


/**************************************************************************
 *  Autonom Eingebettete Systeme    (AEiSy)                               *  
 *                                                                        *
 **************************************************************************
 *  PROJECT         *
 *  MODULE        event.c                                                 *
 *  REVISION      1.0                                                     *
 *  AUTHOR                                                                *
 **************************************************************************
 *  PURPOSE:                                                              *
 *   <The header file of the operating system contains all exported       *
 *   constructs.>                                                         *
 *                                                                        *
 *                                                                        *
 **************************************************************************
 *  CHANGE HISTORY:                                                       *
 *   Revision  Date         Author      Description                       *
 *     1.0      04.03.2020   P. Raab     creation                         *
 *                                                                        *
 *************************************************************************/


/* ---------------- G L O B A L   D E F I N I T I O N S ---------------- */

/*  - I n c l u d e s                                                    */

#include "event.h"            // home header file



/* ----------------- G L O B A L    V A R I A B L E S ------------------ */

EVENT_QUEUE_T   EventQueue;





/* ------------  F U N C T I O N   D E F I N I T I O N ----------------- */

 

 /*  FUNCTION <init(..)>

 ******************************************************************************
 *  PURPOSE:                                                                  *
 *    Initialization of EventHandler                                          *
 *                                                                            *
 ******************************************************************************
 *  PARAMETER:                                                                *
 *    none                                                                    *
 *                                                                            *
 ******************************************************************************
 *  RETURN VALUE                                                              *
 *      none                                                                  *
 *                                                                            *
 ******************************************************************************
 *  CHANGE HISTORY:                                                           *
 *   Revision   Date          Author      Description                         *
 *      0        03.03.2020    P. Raab     creation                           *
 *                                                                            *
 ******************************************************************************/
void InitEventHandler(void)
{
	  EventQueue.read  = 0;
	  EventQueue.write = 0;
}


void SetEvent(uint16_t event, uint16_t parameter, uint32_t parameter2)
{
	
	  /* check, if queue is not full */
	  if (    !( (EventQueue.write + 1) == EventQueue.read )   // 1 entry left
			   && !( (EventQueue.read == 0) && (EventQueue.write + 1 == EVT_BUFFER_SIZE) ) ) // buffer overflow
		{
			
				/* insert new event in event queue */
				EventQueue.Buffer[EventQueue.write].EventID = event;  
				EventQueue.Buffer[EventQueue.write].EventParameter = parameter;
			    EventQueue.Buffer[EventQueue.write].EventParameter2 = parameter2;
				EventQueue.write++;
				
				/* end of buffer */
				if (EventQueue.write >= EVT_BUFFER_SIZE)
				{
					 EventQueue.write = 0;
				}
	  }
	
}



EVENT_T GetEvent(void)
{
	   EVENT_T retEvent;
	
	  if (EventQueue.read != EventQueue.write)
	  {
				retEvent.EventID = EventQueue.Buffer[EventQueue.read].EventID;
				retEvent.EventParameter = EventQueue.Buffer[EventQueue.read].EventParameter;
			  retEvent.EventParameter2 = EventQueue.Buffer[EventQueue.read].EventParameter2;
				EventQueue.read++;
				
				if (EventQueue.read >= EVT_BUFFER_SIZE)
				{
					 EventQueue.read = 0;
				}
		}
		else
    {
			 	retEvent.EventID = EVT_NOEVT;
				retEvent.EventParameter = 0;
		}
	  
		return retEvent;
}

