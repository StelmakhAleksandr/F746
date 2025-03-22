#pragma once

#include "Irq.h"
#include "config/Touch.h"
#include "periph/I2c.h"
#include <deque>

class Touch
{
    friend void ::EXTI15_10_IRQHandler();

public:
    void setup(const TouchConfig& config);
    bool hasTouch();
    TouchState lastTouch();

private:
    void interrupt();
    GpioPin m_button = GpioBuilder::none();
    I2c m_i2c;
    TouchConfig m_config;

    uint8_t countTouch();
    TouchState state(uint8_t touchNumber);

    std::deque<TouchState> m_states;
};
