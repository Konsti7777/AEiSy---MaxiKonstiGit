#include "cmps14.h"
#include "stm32g4xx.h"


#define CMD_GET_BEARING_16BIT 0x13


void CMPS14_Init(void){
	
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART3EN;
	
	GPIOB->MODER &= ~(GPIO_MODER_MODE10 | GPIO_MODER_MODE11);
	GPIOB->MODER |= (GPIO_MODER_MODE10_1 | GPIO_MODER_MODE11_1);
	
	GPIOB->AFR[1] &= ~(GPIO_AFRH_AFSEL10_Pos | GPIO_AFRH_AFSEL11_Pos);
	GPIOB->AFR[1] |= (7 << GPIO_AFRH_AFSEL10_Pos) | (7 << GPIO_AFRH_AFSEL11_Pos);
	
	USART3->CR1 &= ~USART_CR1_UE;
	
	USART3->BRR = 48000000/9600;
	
	USART3->CR1 &= ~(USART_CR1_M0 | USART_CR1_M1);
	
	USART3->CR2 &= ~(USART_CR2_STOP);
	
	USART3->CR1 &= ~USART_CR1_PCE;
	
	USART3->CR1 |= (USART_CR1_TE | USART_CR1_RE | USART_CR1_UE);
}


void CmpsHandler(EVENT_T event)
{
    switch (event.EventID)
    {
        case EVT_INIT_EVT:
            CMPS14_Init();
            break;

        default:
            break;
    }
}

uint16_t CMPS14_GetHeading(void){
	
	uint8_t highByte, lowByte;
	
	while (!(USART3->ISR & USART_ISR_TXE_TXFNF));
	USART3->TDR = CMD_GET_BEARING_16BIT;
	
	
	while (!(USART3->ISR & USART_ISR_RXNE_RXFNE));
	highByte = (uint8_t)(USART3->RDR);
	
	
	while (!(USART3->ISR & USART_ISR_RXNE_RXFNE));
	lowByte = (uint8_t)(USART3->RDR);
	
	return (uint16_t)((highByte << 8) | lowByte);
	
}
