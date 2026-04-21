#include "Display.h"
#include <stm32g4xx.h>

#define LCD_DATA_ADDR (*(volatile uint8_t*) 0x60000000)
#define LCD_CMD_ADDR (*(volatile uint8_t*) 0x60000001)
	
	

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
	GPIOD->MODER |= (2<<(15*2));
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
	LCD_SetCursor(0,0);
	LCD_PutString("Hallo Welt!");
	LCD_SetCursor(4,4);
	LCD_PutChar('c');
//	GPIOE->MODER &= ~GPIO_MODER_MODE11_Msk; //TODO
//	GPIOE->MODER |= (1<<22);
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
    uint16_t addr = y * 30 + x;   
    
    LCD_Write_DATA(addr & 0xFF);
    LCD_Write_DATA((addr >> 8) & 0xFF);
    LCD_Write_CMD(0x24);  // Address Pointer setzen 
}

void LCD_ClearText(void){
    LCD_SetCursor(0, 0);

    for(int i = 0; i < 30 * 16; i++){   // 30 Spalten × 16 Zeilen (anpassen!)
        LCD_Write_DATA(0x00);			
				LCD_Write_CMD(0xC0);	 		//Write Data & Increase adp
				
    }
}

void DisplayHandler(EVENT_T currentEvent){
	
			switch(currentEvent.EventID)
			{ 
			case EVT_INIT_EVT:
				DisplayInit();
				break;
			case EVT_NOEVT:
				break;
				default:
						break;		
		}
		
	}
