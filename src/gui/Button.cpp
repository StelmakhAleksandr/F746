#include "Button.h"

void Button::buttonEventHandler(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    void* data = lv_event_get_user_data(e);
    Button* button = ((Button*)data);
    if (code == LV_EVENT_CLICKED) {
        if (button->m_onClicked) {
            button->m_onClicked();
        }
    }
}

Button::Button(lv_obj_t* parent)
{
    m_obj = lv_button_create(parent);
    m_label = lv_label_create(m_obj);
    lv_obj_add_event_cb(m_obj, buttonEventHandler, LV_EVENT_ALL, this);
}

void Button::setAlign(lv_align_t align, int32_t x_ofs, int32_t y_ofs)
{
    lv_obj_align(m_obj, align, x_ofs, y_ofs);
}

void Button::setText(const std::string& text)
{
    lv_label_set_text(m_label, text.c_str());
    lv_obj_center(m_label);
}

void Button::setOnClicked(std::function<void()> func)
{
    m_onClicked = func;
}
