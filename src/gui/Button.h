#pragma once

#include "lvgl.h"
#include <functional>
#include <string>

class Button
{
public:
    Button(lv_obj_t* parent);
    ~Button();

    void setText(const std::string& text);
    void setSize(int width, int height);
    void setAlignment(lv_align_t align, int xOffset = 0, int yOffset = 0);
    void setCallback(lv_event_code_t eventCode, std::function<void(lv_event_t*)> callback);
    void setTextAlignment(lv_text_align_t align);
    void setFont(const lv_font_t* font);

private:
    lv_obj_t* m_button;
    lv_obj_t* m_label;
    std::function<void(lv_event_t*)> m_callback;
};