#include "Display.h"
#include <stm32g4xx.h>

#define LCD_DATA (*(volatile uint8_t*) 0x60000000)
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
	
	
	
	GPIOD->MODER &= ~GPIO_MODER_MODE1_Msk;
	GPIOD->MODER |= (2<<(1*2));
	GPIOD->AFR[0] &= ~GPIO_AFRH_AFRH1;
	GPIOD->AFR[0] |= (12<<(1*4));

	
	//---------Clock anmachen---------------
	RCC->AHB3ENR |= RCC_AHB3ENR_FMCEN;
	
	//---------Steuerregister----------------
	
	FMC_Bank1_R->BTCR[0] = (1<<0) | (0<<2) | (0<<4) | (1<<12) | (1<<21);
	FMC_Bank1_R->BTCR[1] = (3<<30) | (255<<8) | (15<<0);
	
	LCD_Write_WORD(0x0000);
	LCD_Write_CMD(0x40);

	GPIOE->MODER &= ~GPIO_MODER_MODE11_Msk;
	GPIOE->MODER |= (1<<22);
}

void LCD_Write_CMD(uint8_t cmd){
	LCD_DATA = cmd;
}

void LCD_Write_DATA(uint8_t data){
	LCD_CMD_ADDR = data;
}

void LCD_Write_WORD(uint8_t data){
	LCD_Write_DATA(data & 0xFF);
	LCD_Write_DATA((data >> 8) & 0xFF);
}

void DisplayHandler(EVENT_T currentEvent){
	
			switch(currentEvent.EventID)
			{ 
			case EVT_INIT_EVT:
				DisplayInit();
			case EVT_NOEVT:
				break;
				default:
						break;		
		}
		
	}
