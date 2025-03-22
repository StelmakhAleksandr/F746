#include "MainGui.h"

void MainGui::run()
{
    m_button = std::make_unique<Button>(lv_screen_active());
    m_button->setAlign(LV_ALIGN_TOP_MID, 0, 10);
    m_button->setText("Button");
    m_button->setOnClicked([this] {
        m_button->setText("Clicked");
    });

    m_button2 = std::make_unique<Button>(lv_screen_active());
    m_button2->setAlign(LV_ALIGN_TOP_MID, 0, 50);
    m_button2->setText("Button2");
    m_button2->setOnClicked([this] {
        m_button2->setText("Clicked2");
    });
}