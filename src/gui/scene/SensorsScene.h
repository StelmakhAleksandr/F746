#pragma once

#include "Scene.h"

class SensorsScene : public Scene
{
public:
    SensorsScene();
    ~SensorsScene();

    void create() override;

private:
    struct SensorGui
    {
        lv_obj_t* label;
        lv_obj_t* button;
        std::function<void(lv_event_t* event)> callback;
    };
    SensorGui m_temperature;
    SensorGui m_humidity;
    SensorGui m_airQuality;

    lv_obj_t* m_screen;
    Button m_backButton;
};