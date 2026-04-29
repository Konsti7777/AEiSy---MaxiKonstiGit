
 //////////////////////////////
 // HEADER FILE  event.h     //
 //////////////////////////////


/**************************************************************************
 *  Autonom Eingebettete Systeme    (AEiSy)                               *                                                                                                  *
 **************************************************************************
 *  PROJECT                                                               *
 *  MODULE        event.h                                                  *
 *  REVISION      1.0                                                     *
 **************************************************************************
 *  PURPOSE:                                                              *
 *    
 *                                                                        *
 **************************************************************************
 *  REMARKS:                                                              *
 *                                                                        *
 *                                                                        *
 **************************************************************************
 *  CHANGE HISTORY:                                                       *
 *   Revision  Date         Author      Description                       *
 *     1.0      04.03.2020   P. Raab     creation                         *
 *                                                                        *
 *                                                                        *
 *************************************************************************/


#ifndef _EVENT_H
   #define _EVENT_H

     // global type definitions
     #include <stdint.h>



/*************************************************************************/
/* -------------- E X P O R T E D   D E F I N I T I O N S -------------- */
/*************************************************************************/



/*  - C o n s t a n t s                                                  */

#define EVT_BUFFER_SIZE   100u


// enum als Option
// Define Event IDs
#define  EVT_NOEVT 0u
#define  EVT_500MS_EVT 1u
#define	 EVT_INIT_EVT 2u
#define	 EVT_SONIC_EVT 3u



/*  - T y p e s                                                          */

typedef struct {
	      uint16_t  EventID;
	      uint16_t  EventParameter;
	      uint32_t  EventParameter2;
      } EVENT_T; 

			
typedef  struct {
      EVENT_T  Buffer[EVT_BUFFER_SIZE];
      uint8_t  read; // zeigt auf das Feld mit dem �ltesten Inhalt
      uint8_t  write; // zeigt immer auf leeres Feld
   } EVENT_QUEUE_T;




			

/*  - M a c r o s                                                        */



/*  - P u p b l i c   V a r i a b l e s                                  */



/*  - P u b l i c  F u n c t i o n  P r o t o t y p e s                 */
void    InitEventHandler(void);
void    SetEvent(uint16_t event, uint16_t parameter, uint32_t parameter2);
EVENT_T GetEvent(void);

	
#endif  // _EVENT_
