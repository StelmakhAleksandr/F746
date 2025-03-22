#pragma once

#include "periph/Ltdc.h"
#include "periph/Pixel.h"
#include "periph/SdRam.h"
#include <memory>

class Lcd
{
public:
    void setup(const LcdConfig& config, void* buffer);
    void on();
    void off();
    void backLightOn();
    void backLightOff();

private:
    GpioPin m_backlight = GpioBuilder::none();
    GpioPin m_button = GpioBuilder::none();
};
