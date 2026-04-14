#include "led.h"
#include "stm32g474xx.h"

void LED_Init(void)
{
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;

    GPIOE->MODER &= ~GPIO_MODER_MODE11_Msk;
    GPIOE->MODER |= (1 << 22);
}

void LED_Toggle(void)
{
    GPIOE->ODR ^= GPIO_ODR_OD11;
}

void LED_Handler(EVENT_T curEvent)
{
    switch (curEvent.EventID)
    {
        case EVT_NOEVENT:
            break;

        case EVT_INIT:
            LED_Init();
            break;

        case EVT_LED:
            LED_Toggle();
            break;

        default:
            break;
    }
}