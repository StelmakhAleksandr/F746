#pragma once

#include "app/config/Touch.h"

inline TouchConfig touch()
{
    return {
        .button = PI13,
        .address = 0x38,
        .statusReg = 0x02,
        .firstXhReg = 0x03,
        .sizeStateReg = 6,
        .xBitmask = 0x0F,
        .yBitmask = 0x0F,
    };
}