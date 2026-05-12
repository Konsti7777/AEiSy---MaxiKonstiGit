#include "sonic.h"
#include <stm32g474xx.h>
#include "delay.h"
#include "Event.h"

/* 3 sensor pins (PC6, PC7, PC8) — adjust if wiring differs */
static uint8_t sonic_pins[3] = {6, 7, 8};
static uint8_t current_sensor = 0;

/* Separate storage for left (0), middle (1), right (2) sensors */
static uint16_t lastDistanceLeft = 0;
static uint16_t lastDistanceMiddle = 0;
static uint16_t lastDistanceRight = 0;

void SonicInit(void)
{
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
    GPIOC->MODER &= ~(3 << (sonic_pins[1] * 2));
}

static void SonicSetOutput(uint8_t pin)
{
    GPIOC->MODER &= ~(3 << (pin * 2));
    GPIOC->MODER |=  (1 << (pin * 2));
}

static void SonicSetInput(uint8_t pin)
{
    GPIOC->MODER &= ~(3 << (pin * 2));
}

/* Read a specific sensor by index (0=left, 1=middle, 2=right) */
static uint16_t SonicReadSensor(uint8_t index)
{
    uint8_t pin = sonic_pins[index];
    uint32_t time = 0;

    SonicSetOutput(pin);

    GPIOC->ODR &= ~(1 << pin);
    delayms(1);

    GPIOC->ODR |= (1 << pin);
    delayms(1);

    GPIOC->ODR &= ~(1 << pin);

    SonicSetInput(pin);

    while ((GPIOC->IDR & (1 << pin)) == 0)
    {
    }

    while ((GPIOC->IDR & (1 << pin)) != 0)
    {
        time++;
    }

    return time / 58;
}

/* Read next sensor in sequence (for backward compatibility) */
uint16_t SonicReadNextSensor(void)
{
    return SonicReadSensor(current_sensor);
}

void SonicHandler(EVENT_T event)
{
    switch (event.EventID)
    {
        case EVT_INIT_EVT:
            SonicInit();
            break;

        case EVT_SONIC_EVT:
            /* Measure current sensor and store in appropriate location */
            switch (current_sensor)
            {
                case 0:
                    lastDistanceLeft = SonicReadSensor(0);
                    break;
                case 1:
                    lastDistanceMiddle = SonicReadSensor(1);
                    break;
                case 2:
                    lastDistanceRight = SonicReadSensor(2);
                    break;
            }
            current_sensor = (current_sensor + 1) % 3;
            break;

        default:
            break;
    }
}

/* Get distance from specific sensor (0=left, 1=middle, 2=right) */
uint16_t SonicGetDistanceByIndex(uint8_t index)
{
    switch (index)
    {
        case 0:
            return lastDistanceLeft;
        case 1:
            return lastDistanceMiddle;
        case 2:
            return lastDistanceRight;
        default:
            return 0;
    }
}

/* Get middle sensor distance (backward compatible) */
uint16_t SonicGetDistanceMiddle(void)
{
    return lastDistanceMiddle;
}

/* Get left sensor distance */
uint16_t SonicGetDistanceLeft(void)
{
    return lastDistanceLeft;
}

/* Get right sensor distance */
uint16_t SonicGetDistanceRight(void)
{
    return lastDistanceRight;
}