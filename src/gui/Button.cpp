#include "Button.h"

Button::Button(lv_obj_t* parent)
{
    m_button = lv_btn_create(parent);
    m_label = lv_label_create(m_button);
}

Button::~Button()
{
    lv_obj_del(m_button);
}

void Button::setText(const std::string& text)
{
    lv_label_set_text(m_label, text.c_str());
}

void Button::setSize(int width, int height)
{
    lv_obj_set_size(m_button, width, height);
}

void Button::setAlignment(lv_align_t align, int xOffset, int yOffset)
{
    lv_obj_align(m_button, align, xOffset, yOffset);
}

void Button::setCallback(lv_event_code_t eventCode, std::function<void(lv_event_t*)> callback)
{
    m_callback = std::move(callback);
    lv_obj_add_event_cb(m_button, [](lv_event_t* e) {
        auto* btn = static_cast<Button*>(lv_event_get_user_data(e));
        if (btn->m_callback) {
            btn->m_callback(e); 
        } }, eventCode, this);
}

void Button::setTextAlignment(lv_text_align_t align)
{
    lv_label_set_long_mode(m_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_align(m_label, align, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_center(m_label);
}

void Button::setFont(const lv_font_t* font)
{
    lv_obj_set_style_text_font(m_label, font, LV_PART_MAIN | LV_STATE_DEFAULT);
}
