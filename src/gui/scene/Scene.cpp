#include "Scene.h"

#include "MainScene.h"
#include "SceneManager.h"

void Scene::setupBackButton(Button* button)
{
    button->setText(LV_SYMBOL_HOME);
    button->setSize(70, 70);
    button->setAlignment(LV_ALIGN_TOP_LEFT, 15, 20);
    button->setTextAlignment(LV_TEXT_ALIGN_CENTER);
    button->setFont(&lv_font_montserrat_24);
    button->setCallback(LV_EVENT_CLICKED, [this](lv_event_t* e) {
        m_sceneManager->setScene<MainScene>();
    });
}
