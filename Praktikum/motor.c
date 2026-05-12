#include "motor.h"
#include "stm32g4xx.h"

void Motor_Init(void) {
    // 1. Clocks aktivieren: GPIOD und TIM4
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIODEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;

    // 2. GPIO Konfiguration für PWM
    // PD12 (CH1) und PD13 (CH2) auf Alternate Function (10) setzen
    GPIOD->MODER &= ~(GPIO_MODER_MODE12 | GPIO_MODER_MODE13);
    GPIOD->MODER |= (GPIO_MODER_MODE12_1 | GPIO_MODER_MODE13_1);
    
    // AF2 ist TIM4 für PD12 und PD13 (AFRH = Pins 8-15)
    GPIOD->AFR[1] &= ~(GPIO_AFRH_AFSEL12 | GPIO_AFRH_AFSEL13);
    GPIOD->AFR[1] |= (2 << GPIO_AFRH_AFSEL12_Pos) | (2 << GPIO_AFRH_AFSEL13_Pos);

    // Richtungspins PD14 und PD15 als Output (01)
    GPIOD->MODER &= ~(GPIO_MODER_MODE14 | GPIO_MODER_MODE15);
    GPIOD->MODER |= (GPIO_MODER_MODE14_0 | GPIO_MODER_MODE15_0);

    // 3. TIM4 Konfiguration (48 MHz Systemtakt)
    TIM4->PSC = 48 - 1;          // Timer-Frequenz = 1 MHz
    TIM4->ARR = 100 - 1;         // 100 Schritte -> 10 kHz PWM-Frequenz

    // PWM Mode 1 für Channel 1 und Channel 2
    TIM4->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos) | (6 << TIM_CCMR1_OC2M_Pos);
    TIM4->CCMR1 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE; // Preload aktivieren

    // Capture/Compare Enable: Ausgänge einschalten
    TIM4->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;

    // Counter einschalten
    TIM4->CR1 |= TIM_CR1_CEN;
}

// Interne Hilfsfunktion
void Motor_SetSpeed(uint8_t left, uint8_t right) {
    TIM4->CCR1 = left;
    TIM4->CCR2 = right;
}

void Motor_DriveForward(uint8_t speed) {
    // Beide Motoren Richtung "Vorwärts"
    GPIOD->BSRR = GPIO_BSRR_BS14 | GPIO_BSRR_BS15;
    Motor_SetSpeed(speed, speed);
}

void Motor_DriveBackward(uint8_t speed) {
    // Beide Motoren Richtung "Rückwärts" (Reset Bits 14 & 15)
    GPIOD->BSRR = GPIO_BSRR_BR14 | GPIO_BSRR_BR15;
    Motor_SetSpeed(speed, speed);
}

void Motor_TurnCenteredClockwise(uint8_t speed) {
    GPIOD->BSRR = GPIO_BSRR_BS14; // Links vorwärts
    GPIOD->BSRR = GPIO_BSRR_BR15; // Rechts rückwärts
    Motor_SetSpeed(speed, speed);
}

void Motor_TurnCenteredCounterClockwise(uint8_t speed) {
    GPIOD->BSRR = GPIO_BSRR_BR14; // Links rückwärts
    GPIOD->BSRR = GPIO_BSRR_BS15; // Rechts vorwärts
    Motor_SetSpeed(speed, speed);
}

void Motor_Stop(void) {
    Motor_SetSpeed(0, 0);
}

void MotorHandler(EVENT_T event)
{
    switch (event.EventID)
    {
        case EVT_INIT_EVT:
            Motor_Init();
            break;

        case EVT_FAHREN_EVT:
            Motor_DriveForward(50);
            break; // Wichtig: Break fehlte hier!

        default:
            break;
    }
}