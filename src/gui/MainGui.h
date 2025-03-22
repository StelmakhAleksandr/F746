#pragma once

#include "Button.h"
#include "lvgl.h"
#include <memory>

class MainGui
{
public:
    void run();

private:
    std::unique_ptr<Button> m_button;
    std::unique_ptr<Button> m_button2;
};