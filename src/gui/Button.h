#pragma once

#include "lvgl.h"
#include <functional>
#include <string>

class Button
{
public:
    Button(lv_obj_t* parent);
    void setAlign(lv_align_t align, int32_t x_ofs, int32_t y_ofs);
    void setText(const std::string& text);
    void setOnClicked(std::function<void()> func);

private:
    static void buttonEventHandler(lv_event_t* e);

private:
    lv_obj_t* m_obj = nullptr;
    lv_obj_t* m_label = nullptr;
    std::function<void()> m_onClicked;
};