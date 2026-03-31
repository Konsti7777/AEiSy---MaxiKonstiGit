#include "Led.h"
#include "stm32g474xx.h"

#define LED_PIN 11U

void Led_Init(void)
{
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
    GPIOE->MODER &= ~(3UL << (LED_PIN * 2U));
    GPIOE->MODER |=  (1UL << (LED_PIN * 2U));
}

void Led_HandleEvent(const EVENT_T* event)
{
    if (event == 0)
    {
        return;
    }

    if (event->EventID == EVT_TMR_500MS)
    {
        GPIOE->ODR ^= (1UL << LED_PIN);
    }
}
