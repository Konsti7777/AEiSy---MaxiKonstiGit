#include "sonic.h"
#include <stm32g474xx.h>
#include "delay.h"
#include "Event.h"

#define SONIC_PIN 8

extern volatile unsigned long systickms;

static uint16_t lastDistance = 0;
static uint32_t echoStartTime = 0;
static uint8_t sonicMeasuring = 0;

void SonicInit(void)
{
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // Pin als Input
    GPIOC->MODER &= ~(3 << (SONIC_PIN * 2));

    // EXTI konfigurieren für steigende Flanke
    SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PC;
    EXTI->RTSR1 |= (1 << SONIC_PIN);   // Rising Edge
    EXTI->IMR1 |= (1 << SONIC_PIN);    // Interrupt enable

    NVIC_EnableIRQ(EXTI9_5_IRQn);
}

static void SonicSetOutput(void)
{
    GPIOC->MODER &= ~(3 << (SONIC_PIN * 2));
    GPIOC->MODER |= (1 << (SONIC_PIN * 2));
}

static void SonicSetInput(void)
{
    GPIOC->MODER &= ~(3 << (SONIC_PIN * 2));
}

static void SonicTrigger(void)
{
    SonicSetOutput();

    GPIOC->ODR &= ~(1 << SONIC_PIN);
    //delayms(1);

    GPIOC->ODR |= (1 << SONIC_PIN);
    
    

    GPIOC->ODR &= ~(1 << SONIC_PIN);

    SonicSetInput();
    sonicMeasuring = 1;
}

// Interrupt Handler
void EXTI9_5_IRQHandler(void)
{
    if (EXTI->PR1 & (1 << SONIC_PIN))
    {
        EXTI->PR1 |= (1 << SONIC_PIN); // Clear pending bit

        // Flanke erkannt
        if (GPIOC->IDR & (1 << SONIC_PIN))
        {
            // Steigende Flanke -> Timer starten
            echoStartTime = systickms;
        }
        else
        {
            // Fallende Flanke -> Timer stoppen und berechnen
            if (sonicMeasuring)
            {
                uint32_t echoTime = systickms - echoStartTime;
                lastDistance = (uint16_t)(echoTime * 10 / 58); // in mm
                sonicMeasuring = 0;

                SetEvent(EVT_SONIC_READY_EVT, lastDistance, 0UL);
            }
        }
    }
}

void SonicHandler(EVENT_T event)
{
    switch (event.EventID)
    {
        case EVT_INIT_EVT:
            SonicInit();
            break;

        case EVT_SONIC_EVT:
            SonicTrigger();
            break;

        default:
            break;
    }
}

uint16_t SonicGetDistance(void)
{
    return lastDistance;
}