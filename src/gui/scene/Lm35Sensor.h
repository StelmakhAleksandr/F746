#pragma once
#include "stm32f7xx.h"

class Lm35Sensor
{
public:
    explicit Lm35Sensor(ADC_TypeDef* adc, uint8_t channel)
        : m_adc(adc)
        , m_channel(channel)
    {
    }

    void init() const
    {
        // Enable ADC clock
        RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;

        m_adc->CR2 = 0;
        m_adc->SQR3 = m_channel;

        if (m_channel <= 9)
            m_adc->SMPR2 |= (7U << (m_channel * 3));
        else
            m_adc->SMPR1 |= (7U << ((m_channel - 10) * 3));

        m_adc->CR2 |= ADC_CR2_ADON;
    }

    [[nodiscard]] uint16_t readRaw() const
    {
        m_adc->CR2 |= ADC_CR2_ADON;
        m_adc->CR2 |= ADC_CR2_SWSTART;

        while (!(m_adc->SR & ADC_SR_EOC)) {
        }

        return m_adc->DR;
    }

    [[nodiscard]] float readVoltage(float vref = 3.3f) const
    {
        return ((readRaw() / 4095.0f) * vref); //* 1.47f;
    }

    [[nodiscard]] float readCelsius(float vref = 3.3f) const
    {
        return readVoltage(vref) * 100.0f; // 10 мВ/°C → ×100
    }

private:
    ADC_TypeDef* m_adc;
    uint8_t m_channel;
};
