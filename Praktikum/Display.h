#ifndef _DISPLAY_H
   #define _DISPLAY_H

     // global type definitions
     #include <stdint.h>
		 #include "Event.h"
		 
		 void DisplayHandler(EVENT_T currentEvent);
			void LCD_Write_CMD(uint8_t cmd);
			void LCD_Write_WORD(uint16_t word);
			void Set_Text_Init(void);
			void Set_Graphic_Init(void);
			static uint8_t readStatus(void);
			void LCD_PutString(const char *s);
			void LCD_PutChar(char c);
			void LCD_ClearText(void);
			void LCD_SetCursor(uint8_t x, uint8_t y);
			void LCD_SetPixel(uint8_t x, uint8_t y);
			void LCD_DrawHLine(uint8_t x1, uint8_t x2, uint8_t y);
			void LCD_DrawVLine(uint8_t x, uint8_t y1, uint8_t y2);
			void LCD_DrawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
			void LCD_DrawBitmap(const char *picture);
			void LCD_SetGraphicAddress(uint8_t x, uint8_t y);
			void LCD_Clear(void);
			void LCD_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
#endif  // _DISPLAY_H
