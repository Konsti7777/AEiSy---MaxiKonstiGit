#ifndef _DISPLAY_H
   #define _DISPLAY_H

     // global type definitions
     #include <stdint.h>
		 #include "Event.h"
		 
		 void DisplayHandler(EVENT_T currentEvent);
			void LCD_Write_CMD(uint8_t cmd);
			void LCD_Write_WORD(uint8_t word);
			void Set_Text_Init(void);
			void Set_Graphic_Init(void);
#endif  // _DISPLAY_H
