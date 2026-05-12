#include "motor.h"
#include "stm32g4xx.h"

void Motor_Init(void) {
    // 1. Clocks aktivieren: GPIOB und TIM4 (TIM4 hängt an APB1)
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;

    // 2. GPIO Konfiguration
    // PB6 (CH1) und PB7 (CH2) auf Alternate Function (10) setzen
    GPIOB->MODER &= ~(GPIO_MODER_MODE6 | GPIO_MODER_MODE7);
    GPIOB->MODER |= (GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1);
    
    // AF2 ist TIM4 für PB6 und PB7
    GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL6 | GPIO_AFRL_AFSEL7);
    GPIOB->AFR[0] |= (2 << GPIO_AFRL_AFSEL6_Pos) | (2 << GPIO_AFRL_AFSEL7_Pos);

    // Richtungspins PB8 und PB9 als Output (01)
    GPIOB->MODER &= ~(GPIO_MODER_MODE8 | GPIO_MODER_MODE9);
    GPIOB->MODER |= (GPIO_MODER_MODE8_0 | GPIO_MODER_MODE9_0);

    // 3. TIM4 Konfiguration (für 10 kHz PWM bei 170 MHz Systemtakt)
    TIM4->PSC = 170 - 1;          // Timer-Frequenz = 1 MHz
    TIM4->ARR = 100 - 1;          // 100 Schritte -> 10 kHz Frequenz

    // PWM Mode 1 für Channel 1 (Motor Links) und Channel 2 (Motor Rechts)
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
    GPIOB->BSRR = GPIO_BSRR_BS8; // Richtung vorwärts
    GPIOB->BSRR = GPIO_BSRR_BS9;
    Motor_SetSpeed(speed, speed);
}

void Motor_DriveBackward(uint8_t speed) {
    GPIOB->BSRR = GPIO_BSRR_BR8; // Richtung rückwärts
    GPIOB->BSRR = GPIO_BSRR_BR9;
    Motor_SetSpeed(speed, speed);
}

void Motor_TurnCenteredClockwise(uint8_t speed) {
    GPIOB->BSRR = GPIO_BSRR_BS8; // Links vorwärts
    GPIOB->BSRR = GPIO_BSRR_BR9; // Rechts rückwärts
    Motor_SetSpeed(speed, speed);
}

void Motor_TurnCenteredCounterClockwise(uint8_t speed) {
    GPIOB->BSRR = GPIO_BSRR_BR8; // Links rückwärts
    GPIOB->BSRR = GPIO_BSRR_BS9; // Rechts vorwärts
    Motor_SetSpeed(speed, speed);
}

void Motor_Stop(void) {
    Motor_SetSpeed(0, 0);
}