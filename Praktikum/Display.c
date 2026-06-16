#include "Display.h"
#include <stm32g4xx.h>
#include "sonic.h"
#include "stdio.h"
#include "stm32g474xx.h"
#include "cmps14.h"
#include "delay.h"
#include <stdlib.h> 
#include "Assets/hs-logo.c"
#include "Assets/dashboard-mockup.c"
#include "drive.h"

#define LCD_DATA_ADDR (*(volatile uint8_t*) 0x60000000)
#define LCD_CMD_ADDR (*(volatile uint8_t*) 0x60000001)

#define LCD_WIDTH 240
#define LCD_HEIGHT 128



void DisplayInit(void){
	//-------------RCC Register-------------
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOFEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
	//---------MODER oder AFR---------------
	GPIOF->MODER &= ~GPIO_MODER_MODE10_Msk;
	GPIOF->MODER |= (2<<20);
	GPIOF->AFR[1] &= ~GPIO_AFRH_AFRH2;
	GPIOF->AFR[1] |= (12<<(2*4));			// 4Bits wide
	
	GPIOD->MODER &= ~GPIO_MODER_MODE14_Msk;
	GPIOD->MODER |= (2<<(14*2));
	GPIOD->AFR[1] &= ~GPIO_AFRH_AFRH6;
	GPIOD->AFR[1] |= (12<<(6*4));	
	
	GPIOD->MODER &= ~GPIO_MODER_MODE15_Msk;
	GPIOD->MODER |= (2U<<(15*2));
	GPIOD->AFR[1] &= ~GPIO_AFRH_AFRH7;
	GPIOD->AFR[1] |= (12<<(7*4));

	GPIOD->MODER &= ~GPIO_MODER_MODE0_Msk;
	GPIOD->MODER |= (2<<(0*2));
	GPIOD->AFR[0] &= ~GPIO_AFRL_AFRL0;
	GPIOD->AFR[0] |= (12<<(0*4));		
	
	GPIOD->MODER &= ~GPIO_MODER_MODE1_Msk;
	GPIOD->MODER |= (2<<(1*2));
	GPIOD->AFR[0] &= ~GPIO_AFRL_AFRL1;
	GPIOD->AFR[0] |= (12<<(1*4));

	GPIOE->MODER &= ~GPIO_MODER_MODE7_Msk;
	GPIOE->MODER |= (2<<(7*2));
	GPIOE->AFR[0] &= ~GPIO_AFRL_AFRL7;
	GPIOE->AFR[0] |= (12<<(7*4));		
	
	GPIOE->MODER &= ~GPIO_MODER_MODE8_Msk;
	GPIOE->MODER |= (2<<(8*2));
	GPIOE->AFR[1] &= ~GPIO_AFRH_AFRH0;
	GPIOE->AFR[1] |= (12<<(0*4));	
	
	GPIOE->MODER &= ~GPIO_MODER_MODE9_Msk;
	GPIOE->MODER |= (2<<(9*2));
	GPIOE->AFR[1] &= ~GPIO_AFRH_AFRH1;
	GPIOE->AFR[1] |= (12<<(1*4));

	GPIOE->MODER &= ~GPIO_MODER_MODE10_Msk;
	GPIOE->MODER |= (2<<(10*2));
	GPIOE->AFR[1] &= ~GPIO_AFRH_AFRH2;
	GPIOE->AFR[1] |= (12<<(2*4));

	GPIOD->MODER &= ~GPIO_MODER_MODE4_Msk;
	GPIOD->MODER |= (2<<(4*2));
	GPIOD->AFR[0] &= ~GPIO_AFRL_AFRL4;
	GPIOD->AFR[0] |= (12<<(4*4));	
	
	GPIOD->MODER &= ~GPIO_MODER_MODE5_Msk;
	GPIOD->MODER |= (2<<(5*2));
	GPIOD->AFR[0] &= ~GPIO_AFRL_AFRL5;
	GPIOD->AFR[0] |= (12<<(5*4));	
	
	GPIOD->MODER &= ~GPIO_MODER_MODE7_Msk;
	GPIOD->MODER |= (2<<(7*2));
	GPIOD->AFR[0] &= ~GPIO_AFRL_AFRL7;
	GPIOD->AFR[0] |= (12<<(7*4));	


	//---------Clock anmachen---------------
	RCC->AHB3ENR |= RCC_AHB3ENR_FMCEN;
	
	//---------Steuerregister----------------
	
	FMC_Bank1_R->BTCR[0] = (1<<0) | (0<<2) | (0<<3) | (0<<4) | (0<<5) | (1<<12) | (1<<21);
	FMC_Bank1_R->BTCR[1] = (3<<30) | (255<<8) | (15<<0);
	
	
	
	Set_Text_Init();
	Set_Graphic_Init();
	
	
	
	LCD_Write_CMD(0x81);	//EXOR Mode
	LCD_Write_CMD(0x9C);	//Display Mode: Text on, graphic on
	//LCD_Write_CMD(0x94);//Display Mode: Text on, graphic off
	//LCD_Write_CMD(0x98);//Display Mode: Text off, graphic on

	LCD_ClearText();
	LCD_Clear();
	LCD_DrawBitmap(HSimage);
	delayms(1500);
	LCD_Clear();

	LCD_ClearText();
	LCD_Clear();
	//delayms(30000);

	
	    // oberer Rahmen
    LCD_DrawRect(5, 1, 234, 25);
    LCD_SetCursor(8, 1);       // Text: 30x16 Koordinaten
    LCD_PutString("Dashboard Test");
		
    // Kreis / Kompass
    LCD_DrawCircle(116, 56, 30);
		
    LCD_SetCursor(14, 4);
    LCD_PutString("N");
    LCD_SetCursor(11, 6);
    LCD_PutString("W");
    LCD_SetCursor(17, 6);
    LCD_PutString("E");
    LCD_SetCursor(14, 8);
    LCD_PutString("S");
}

static void Wait_Ready(void){
	while((readStatus() & 0x03) != 0x03){}
	}


	
void LCD_Write_CMD(uint8_t cmd){
	Wait_Ready();
	LCD_CMD_ADDR = cmd;
}

static uint8_t readStatus(void){
	return LCD_CMD_ADDR;
}

void LCD_Write_DATA(uint8_t data){
	Wait_Ready();
	LCD_DATA_ADDR = data;
}

void LCD_Write_WORD(uint16_t data){
	LCD_Write_DATA(data & 0xFF);
	LCD_Write_DATA((data >> 8) & 0xFF);
}

void Set_Text_Init(void){
	LCD_Write_WORD(0x0000);
	LCD_Write_CMD(0x40);
	
	LCD_Write_WORD(30);
	LCD_Write_CMD(0x41);
}

void Set_Graphic_Init(void){
	LCD_Write_WORD(0x0200);
	LCD_Write_CMD(0x42);
	
	LCD_Write_WORD(30);
	LCD_Write_CMD(0x43);
}

void LCD_PutChar(char c){
    LCD_Write_DATA((uint8_t)c - 0x20);
		LCD_Write_CMD(0xC0); 		//Write Data & Increase adp
}

void LCD_PutString(const char *s){
    while(*s){
        LCD_PutChar(*s++);
    }
}

void LCD_SetCursor(uint8_t x, uint8_t y){
    uint16_t addr =0x0000 + ( y * 30 + x);   
    
    LCD_Write_DATA(addr & 0xFF);
    LCD_Write_DATA((addr >> 8) & 0xFF);
    LCD_Write_CMD(0x24);  // Address Pointer setzen 
}

void LCD_SetGraphicAddress(uint8_t x, uint8_t y){
	uint16_t graphAddr = 0x200 + (y * 30 + x/8);
	LCD_Write_WORD(graphAddr);
	LCD_Write_CMD(0x24);
}

void LCD_ClearText(void){
    LCD_SetCursor(0, 0);

    for(int i = 0; i < 30 * 16; i++){   // 30 Spalten � 16 Zeilen (anpassen!)
        LCD_Write_DATA(0x00);			
				LCD_Write_CMD(0xC0);	 		//Write Data & Increase adp
				
    }
}

void LCD_SetPixel(uint8_t x, uint8_t y){
		LCD_SetGraphicAddress(x,y);
		LCD_Write_CMD(0xF8 | (7 - (x % 8)));
}

void LCD_DrawCircle(uint8_t cx, uint8_t cy, uint8_t radius)
{
	int16_t x = 0;
	int16_t y = radius;
	int16_t d = 1 - (int16_t)radius;

	LCD_DrawCirclePoints((int16_t)cx, (int16_t)cy, x, y);

	while (x < y)
	{
		x++;
		if (d < 0)
		{
			d += (2 * x) + 1;
		}
		else
		{
			y--;
			d += (2 * (x - y)) + 1;
		}

		LCD_DrawCirclePoints((int16_t)cx, (int16_t)cy, x, y);
	}
}

static void LCD_SetPixelSafe(int16_t x, int16_t y)
{
	if ((x >= 0) && (x < LCD_WIDTH) && (y >= 0) && (y < LCD_HEIGHT))
	{
		LCD_SetPixel((uint8_t)x, (uint8_t)y);
	}
}

static void LCD_DrawCirclePoints(int16_t cx, int16_t cy, int16_t x, int16_t y)
{
	LCD_SetPixelSafe(cx + x, cy + y);
	LCD_SetPixelSafe(cx - x, cy + y);
	LCD_SetPixelSafe(cx + x, cy - y);
	LCD_SetPixelSafe(cx - x, cy - y);
	LCD_SetPixelSafe(cx + y, cy + x);
	LCD_SetPixelSafe(cx - y, cy + x);
	LCD_SetPixelSafe(cx + y, cy - x);
	LCD_SetPixelSafe(cx - y, cy - x);
}
	

void LCD_DrawHLine(uint8_t x1, uint8_t x2, uint8_t y){
    for(uint8_t x = x1; x <= x2; x++){
        LCD_SetPixel(x, y);
    }
}

void LCD_DrawVLine(uint8_t x, uint8_t y1, uint8_t y2){
    for(uint8_t y = y1; y <= y2; y++){
        LCD_SetPixel(x, y);
    }
}


void LCD_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
    int dx = abs(x1 - x0);
    int sx = (x0 < x1) ? 1 : -1;

    int dy = -abs(y1 - y0);
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx + dy;
    int e2;

    while (1)
    {
        LCD_SetPixel(x0, y0);

        if (x0 == x1 && y0 == y1)
            break;

        e2 = 2 * err;

        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }

        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void LCD_DrawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2){
    LCD_DrawHLine(x1, x2, y1); // oben
    LCD_DrawHLine(x1, x2, y2); // unten
    LCD_DrawVLine(x1, y1, y2); // links
    LCD_DrawVLine(x2, y1, y2); // rechts
}

void LCD_DrawBitmap(const char *picture){
	uint16_t addr = 0x0200;
	LCD_Write_WORD(addr);
	LCD_Write_CMD(0x24);
	
	for (int row = 0; row < 128; row++){
		for(int col = 0; col < 30; col++){
			LCD_Write_DATA(picture[row * 30 + col]);
			LCD_Write_CMD(0xC0);
		}
	}
}

void LCD_Clear(void){
	uint16_t addr = 0x0200;
	LCD_Write_WORD(addr);
	LCD_Write_CMD(0x24);
	
	for (int row = 0; row < 128; row++){
		for(int col = 0; col < 30; col++){
			LCD_Write_DATA(0x00);
			LCD_Write_CMD(0xc0);
		}
	}
}

void bufferedPutString(uint16_t valueToPrint, int x, int y){
	LCD_SetCursor(x,y);
	char buffer[10];
	snprintf(buffer, sizeof buffer, "%u  ", valueToPrint);
	LCD_PutString(buffer);
}

void DisplaySonicDistance(void){
	LCD_SetCursor(5,0);
	LCD_PutString("Sonic-Sensor-Distance:");
	LCD_DrawRect(1,9,239,30);
	
	uint16_t distanceRight = SonicGetDistanceRight();
	bufferedPutString(distanceRight,2,2);
	
	uint16_t distanceMiddle = SonicGetDistanceMiddle();
	bufferedPutString(distanceMiddle,13,2);
	
	uint16_t distanceLeft = SonicGetDistanceLeft();
	bufferedPutString(distanceLeft,25,2);
	
	LCD_SetCursor(7,7);
	LCD_PutString("Kompass-Rotation:");
	uint16_t rotation = CMPS14_GetHeading();
	bufferedPutString(rotation/10,13,8);
	
}
// Löscht eine freie Linie (Pendant zu LCD_DrawLine)
void LCD_ClearLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    int dx = abs(x1 - x0);
    int sx = (x0 < x1) ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx + dy;
    int e2;

    while (1) {
        LCD_SetGraphicAddress(x0, y0);
        LCD_Write_CMD(0xF0 | (7 - (x0 % 8))); // 0xF0 löscht das Pixel

        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

// Löscht eine horizontale Linie (Pendant zu LCD_DrawHLine)
void LCD_ClearHLine(uint8_t x1, uint8_t x2, uint8_t y) {
    for(uint8_t x = x1; x <= x2; x++){
        LCD_SetGraphicAddress(x, y);
        LCD_Write_CMD(0xF0 | (7 - (x % 8)));
    }
}
// Hilfsstruktur um Koordinaten sauber zu speichern
typedef struct {
    int16_t x0, y0, x1, y1;
} LineCoords_t;

void LCD_DrawDashboard(void)
{
		uint16_t rotation = CMPS14_GetHeading();
		bufferedPutString(rotation/10,12,6);
		
		
		bufferedPutString(getInitialheading(),11,2);
		
    // --- 1. SENSORWERTE ABFRAGEN & TEXT AUSGEBEN ---
    uint16_t distanceRight  = SonicGetDistanceRight();
    uint16_t distanceMiddle = SonicGetDistanceMiddle();
    uint16_t distanceLeft   = SonicGetDistanceLeft();

    bufferedPutString(distanceRight, 1, 9);
    bufferedPutString(distanceLeft, 26, 9);
    bufferedPutString(distanceMiddle, 13, 15);

    // Statische Speicher für die jeweils ZULETZT gezeichneten Linien
    static LineCoords_t last_left  = {-1, 0, 0, 0};
    static LineCoords_t last_right = {-1, 0, 0, 0};
    static LineCoords_t last_plat  = {-1, 0, 0, 0}; // x0=links, x1=rechts, y0=höhe

    // Neue temporäre Koordinaten
    LineCoords_t next_left  = {0, 0, 0, 0};
    LineCoords_t next_right = {0, 0, 0, 0};
    LineCoords_t next_plat  = {0, 0, 0, 0};

    uint8_t draw_left = 1, draw_right = 1, draw_plat = 1;

    // --- 2. LOGIK: LINKE SEITE (Abstand Links) ---
    if (distanceRight >= 400)       { next_left = (LineCoords_t){5, 36, 66, 106}; }
    else if (distanceRight >= 300)  { next_left = (LineCoords_t){14, 36, 75, 106}; }
    else if (distanceRight >= 200)  { next_left = (LineCoords_t){32, 36, 93, 106}; }
    else if (distanceRight >= 100)  { next_left = (LineCoords_t){41, 36, 102, 106}; }
    else                           { draw_left = 0; }

    // --- 3. LOGIK: RECHTE SEITE (Abstand Rechts) ---
    if (distanceLeft >= 400)      { next_right = (LineCoords_t){235, 36, 174, 106}; }
    else if (distanceLeft >= 300) { next_right = (LineCoords_t){226, 36, 165, 106}; }
    else if (distanceLeft >= 200) { next_right = (LineCoords_t){217, 36, 156, 106}; }
    else if (distanceLeft >= 100) { next_right = (LineCoords_t){208, 36, 147, 106}; }
    else                           { draw_right = 0; }

    // --- 4. LOGIK: UNTERE PLATTFORM (Abstand Mitte) ---
    // Hier wandert die Linie je nach Nähe weiter nach unten (näher an den Betrachter)
    if (distanceMiddle >= 500)      { next_plat = (LineCoords_t){63, 0, 177, 114}; }
    else if (distanceMiddle >= 400) { next_plat = (LineCoords_t){68, 0, 172, 108}; }
    else if (distanceMiddle >= 300) { next_plat = (LineCoords_t){73, 0, 167, 102}; }
    else if (distanceMiddle >= 200) { next_plat = (LineCoords_t){78, 0, 162, 96}; }
    else if (distanceMiddle >= 100) { next_plat = (LineCoords_t){82, 0, 158, 90}; }
    else                            { draw_plat = 0; }


    // --- 5. LÖSCHEN & NEU ZEICHNEN ---

    // Links verarbeiten
    if (last_left.x0 != -1) {
        LCD_ClearLine(last_left.x0, last_left.y0, last_left.x1, last_left.y1);
    }
    if (draw_left) {
        LCD_DrawLine(next_left.x0, next_left.y0, next_left.x1, next_left.y1);
        last_left = next_left;
    } else { last_left.x0 = -1; }

    // Rechts verarbeiten
    if (last_right.x0 != -1) {
        LCD_ClearLine(last_right.x0, last_right.y0, last_right.x1, last_right.y1);
    }
    if (draw_right) {
        LCD_DrawLine(next_right.x0, next_right.y0, next_right.x1, next_right.y1);
        last_right = next_right;
    } else { last_right.x0 = -1; }

    // Plattform (Mitte) verarbeiten
    if (last_plat.x0 != -1) {
        // Nutzen die Struktur als (x1, frei, x2, y) für die HLine
        LCD_ClearHLine(last_plat.x0, last_plat.x1, last_plat.y1);
    }
    if (draw_plat) {
        LCD_DrawHLine(next_plat.x0, next_plat.x1, next_plat.y1);
        last_plat = next_plat;
    } else { last_plat.x0 = -1; }
}
	
void DisplayHandler(EVENT_T currentEvent){
	
			switch(currentEvent.EventID)
			{ 
			case EVT_INIT_EVT:
				DisplayInit();
				break;
			case EVT_NOEVT:
				break;
			case EVT_DISPLAY_SONIC_EVT:
				//DisplaySonicDistance();
				LCD_DrawDashboard();
				break;
				default:
						break;		
		}
		
	}

	
