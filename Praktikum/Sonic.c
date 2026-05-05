#include "sonic.h"
#include <stm32g474xx.h>
#include "delay.h"
#include "Event.h"

#define SONIC_PIN 8

static uint16_t lastDistance = 0;

void SonicInit(void)
{
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

    GPIOC->MODER &= ~(3 << (SONIC_PIN * 2));
}

static void SonicSetOutput(void)
{
    GPIOC->MODER &= ~(3 << (SONIC_PIN * 2));
    GPIOC->MODER |=  (1 << (SONIC_PIN * 2));
}

static void SonicSetInput(void)
{
    GPIOC->MODER &= ~(3 << (SONIC_PIN * 2));
}

uint16_t SonicReadMiddle(void)
{
    uint32_t time = 0;

    SonicSetOutput();

    GPIOC->ODR &= ~(1 << SONIC_PIN);
    delayms(1);

    GPIOC->ODR |= (1 << SONIC_PIN);
    delayms(1);

    GPIOC->ODR &= ~(1 << SONIC_PIN);

    SonicSetInput();

    while ((GPIOC->IDR & (1 << SONIC_PIN)) == 0)
    {
    }

    while ((GPIOC->IDR & (1 << SONIC_PIN)) != 0)
    {
        time++;
    }

    return time / 58;
}

void SonicHandler(EVENT_T event)
{
    switch (event.EventID)
    {
        case EVT_INIT_EVT:
            SonicInit();
            break;

        case EVT_SONIC_EVT:
            lastDistance = SonicReadMiddle();
						
            break;

        default:
            break;
    }
}

uint16_t SonicGetDistance(void)
{
    return lastDistance;
}