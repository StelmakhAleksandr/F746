#pragma once
#include "stm32f7xx.h"
#include <cmath>

class GasSensor
{
public:
    explicit GasSensor(ADC_TypeDef* adc, uint8_t channel, GPIO_TypeDef* doutPort = nullptr, uint8_t doutPin = 0)
        : m_adc(adc)
        , m_channel(channel)
        , m_doutPort(doutPort)
        , m_doutPin(doutPin)
    {
    }

    void init() const
    {
        RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;
        GPIOA->MODER |= (3U << (0 * 2)); // PA0 = Analog (11b)
        GPIOA->PUPDR &= ~(3U << (0 * 2));

        m_adc->CR2 = 0;
        m_adc->SQR3 = m_channel;

        if (m_channel <= 9)
            m_adc->SMPR2 |= (7U << (m_channel * 3));
        else
            m_adc->SMPR1 |= (7U << ((m_channel - 10) * 3));

        m_adc->CR2 |= ADC_CR2_ADON;

        if (m_doutPort) {
            m_doutPort->MODER &= ~(3U << (m_doutPin * 2));
        }
    }

    [[nodiscard]] uint16_t readRaw() const
    {
        m_adc->CR2 |= ADC_CR2_ADON;
        m_adc->CR2 |= ADC_CR2_SWSTART;

        while (!(m_adc->SR & ADC_SR_EOC)) {
            // Wait
        }

        return m_adc->DR;
    }

    [[nodiscard]] float readVoltage(float vref = 3.3f) const
    {
        return (readRaw() / 4095.0f) * vref;
    }

    [[nodiscard]] int readAirQualityIndex(float vref = 3.3f) const
    {
        float volts = readVoltage(vref);

        float norm = (volts - 0.3f) / (3.5f - 0.3f);
        int value = static_cast<int>(norm * 100.0f);

        if (value < 0)
            value = 0;
        if (value > 100)
            value = 100;

        return value;
    }

    void setRo(float ro)
    {
        m_ro = ro;
    }
    [[nodiscard]] float getRo() const
    {
        return m_ro;
    }

    [[nodiscard]] float readPpm(float vcc = 3.3f, float rl = 10.0f, float m = -0.45f, float b = 1.7f) const
    {
        float vout = readVoltage(vcc);
        if (vout <= 0.0f || m_ro <= 0.0f)
            return 0.0f;

        float rs = rl * (vcc - vout) / vout;
        float ratio = rs / m_ro;

        float logPpm = m * std::log10(ratio) + b;
        return std::pow(10.0f, logPpm);
    }

    void calib()
    {
        // Калібрування
        // float rl = 10.0f; // номінальний опір навантаження (на платі MQ-2)
        // float volts = readVoltage();
        // float rs = rl * (3.3f - volts) / volts;
        // float ro = rs / 9.8f;
        // printf("Ro = %d\n", int(ro * 1000));
        // setRo(ro);
    }

    [[nodiscard]] bool isTriggered() const
    {
        if (!m_doutPort)
            return false;
        return !(m_doutPort->IDR & (1U << m_doutPin)); // active LOW
    }

private:
    ADC_TypeDef* m_adc;
    uint8_t m_channel;
    GPIO_TypeDef* m_doutPort;
    uint8_t m_doutPin;
    float m_ro = 8.476f;
};