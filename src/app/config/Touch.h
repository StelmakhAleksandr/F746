#pragma once

#include "periph/Pins.h"

struct TouchConfig
{
    GpioBuilder button = GpioBuilder(0, 0);
    uint8_t address;
    uint8_t statusReg;
    uint8_t firstXhReg;
    uint8_t sizeStateReg;
    uint8_t xBitmask;
    uint8_t yBitmask;
};

struct TouchState
{
    uint16_t x;
    uint16_t y;
    uint8_t weight;
    uint8_t misc;
};
