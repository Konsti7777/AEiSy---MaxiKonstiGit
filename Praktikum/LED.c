#include "LED.h"
#include <stm32g4xx.h>

void LedInit(void){
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
	GPIOE->MODER &= ~GPIO_MODER_MODE11_Msk;
	GPIOE->MODER |= (1<<(11*2));				//Cause MODER always uses 2bits do encode Mode			
}

void LedHandler(EVENT_T currentEvent){
	
			switch(currentEvent.EventID)
			{ 
			case EVT_500MS_EVT:
				GPIOE->ODR ^=GPIO_ODR_OD11;
			case EVT_INIT_EVT:
				LedInit();
			case EVT_NOEVT:
				break;
				default:
						break;		
		}
		
	}