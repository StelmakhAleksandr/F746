#pragma once

#include "Lcd.h"
#include "Touch.h"
#include "lvgl.h"

class LvglPort
{
public:
    void setup(const LcdConfig& lcdConfig, const TouchConfig& touchConfig);
    void run();

private:
    void setupDisplayDriver(const LcdConfig& lcdConfig);
    void setupTouchDriver(const TouchConfig& touchConfig);
    static void lvglTimerTask();

    Lcd m_lcd;
    Touch m_touch;
    std::vector<std::shared_ptr<PixelContainer>> m_buffers;
};