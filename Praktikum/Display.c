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
void LCD_ClearLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    int dx = abs(x1 - x0);
    int sx = (x0 < x1) ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx + dy;
    int e2;

    while (1) {
        // Hier nutzen wir den Clear-Pixel-Befehl statt Set-Pixel
        LCD_SetGraphicAddress(x0, y0);
        LCD_Write_CMD(0xF0 | (7 - (x0 % 8))); // 0xF0 löscht das Pixel

        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}
void LCD_DrawDashboard(void)
{
    uint16_t distanceRight = SonicGetDistanceRight();
    bufferedPutString(distanceRight, 1, 9);

    uint16_t distanceMiddle = SonicGetDistanceMiddle();
    bufferedPutString(distanceMiddle, 26, 9);

    uint16_t distanceLeft = SonicGetDistanceLeft();
    bufferedPutString(distanceLeft, 13, 15);

    // Statische Variablen behalten ihren Wert zwischen den Funktionsaufrufen
    static int16_t last_x0 = -1, last_y0 = -1, last_x1 = -1, last_y1 = -1;

    // 1. Bestimme die Koordinaten für die NEUE linke Linie basierend auf der Distanz
    int16_t next_x0 = 0, next_y0 = 0, next_x1 = 0, next_y1 = 0;
    uint8_t draw_line = 1; // Flag, ob überhaupt gezeichnet werden soll

    if (distanceLeft >= 400) {
        next_x0 = 5;  next_y0 = 36; next_x1 = 66; next_y1 = 106;
    } else if (distanceLeft >= 300) {
        next_x0 = 14; next_y0 = 36; next_x1 = 75; next_y1 = 106;
    } else if (distanceLeft >= 200) {
        next_x0 = 32; next_y0 = 36; next_x1 = 93; next_y1 = 106;
    } else if (distanceLeft >= 100) {
        next_x0 = 41; next_y0 = 36; next_x1 = 102; next_y1 = 106;
    } else {
        draw_line = 0; // Keine Linie bei unter 100
    }

    // 2. LÖSCHE die alte Linie (falls im vorherigen Schritt eine gezeichnet wurde)
    if (last_x0 != -1) {
        LCD_ClearLine(last_x0, last_y0, last_x1, last_y1);
    }

    // 3. ZEICHNE die neue Linie und merke dir die Koordinaten für das nächste Mal
    if (draw_line) {
        LCD_DrawLine(next_x0, next_y0, next_x1, next_y1);
        last_x0 = next_x0;
        last_y0 = next_y0;
        last_x1 = next_x1;
        last_y1 = next_y1;
    } else {
        // Zurücksetzen, wenn keine Linie aktiv ist
        last_x0 = -1;
    }

    // --- RECHTE LINIEN & UNTERE PLATTFORM ---
    // (Wenn diese auch dynamisch werden sollen, wende dasselbe Prinzip an. 
    // Momentan werden sie bei jedem Aufruf einfach starr stumpf übereinander gezeichnet)
    LCD_DrawLine(235, 36, 174, 106);
    LCD_DrawLine(226, 36, 165, 106);
    LCD_DrawLine(217, 36, 156, 106);
    LCD_DrawLine(208, 36, 147, 106);
    LCD_DrawLine(199, 36, 138, 106);

    LCD_DrawHLine(82, 158, 90);
    LCD_DrawHLine(78, 162, 96);
    LCD_DrawHLine(73, 167, 102);
    LCD_DrawHLine(68, 172, 108);
    LCD_DrawHLine(63, 177, 114);
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

	
