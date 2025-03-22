#include "Usart.h"

void setupUsart()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODER9_Msk)) | GPIO_MODER_MODER9_1;
    GPIOA->AFR[1] = (GPIOA->AFR[1] & ~GPIO_AFRH_AFRH1_Msk) | 0b0111 << GPIO_AFRH_AFRH1_Pos;
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT9;
    GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR9;
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR9;

    USART1->BRR = 108000000 / 115200;
    USART1->CR1 |= USART_CR1_UE | USART_CR1_TE;
}

void sendMessage(const std::string &msg)
{
    for (const auto &c : msg)
    {
        while (!(USART1->ISR & USART_ISR_TXE))
        {
        }
        USART1->TDR = c;
    }
}