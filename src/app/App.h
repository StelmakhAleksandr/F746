#pragma once

#include "Config.h"
#include "LvglPort.h"
#include "Task.h"
#include <string>

class App
{
public:
    void setup();
    void run();

private:
    void blinkTask();
    void helloTask();

    GpioPin m_led = GpioBuilder::none();
    LvglPort m_lvgl;
};