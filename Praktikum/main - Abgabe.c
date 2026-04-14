#include "Event.h"

/* ----------- V A R I A B L E S   &  C O N S T A N T S  --------------- */

extern unsigned long SystemCoreClock;
volatile unsigned long systickms;

/* ------------- F u n c t i o n  P r o t o t y p e s  ----------------- */

/* ----------------------- F U N C T I O N S  -------------------------- */

void SysTick_Handler()
{
	
systickms++;
	if ((systickms % 500) == 0)
	{
		SetEvent(EVT_LED_EVT, 0U, 0UL);
		
	}
	

}

/* --------------  S t a r t    o f    p r o g r a m  -----------------  */

int main(void)
{
	EVENT_T currentEvent;
	
 


RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
	
GPIOE->MODER &= ~GPIO_MODER_MODE11_Msk;
	GPIOE->MODER |= (1 << 22);
	InitEventHandler();
	SysTick_Config(SystemCoreClock / 500);
	
/* Endlosschleife */
while (1)

	{
		currentEvent = GetEvent();
		switch (currentEvent.EventID)

		{
		case EVT_LED_EVT:
			GPIOE->ODR ^= GPIO_ODR_OD11;
		case EVT_NOEVT:
			default :
break;
			

		}
		
 //delayms(500);


	}
}