#pragma once

#include "Scene.h"
#include <memory>
#include <vector>

#include "GasSensor.h"
#include "stmcmp/stmcmp.h"

class SceneManager
{
public:
    SceneManager();
    void subscribe(std::function<void()> func)
    {
        m_function = func;
    }

    template <typename SceneType, typename... Args>
    void setScene(Args&&... args)
    {
        m_function = nullptr;
        m_currentScene = std::make_unique<SceneType>(std::forward<Args>(args)...);
        m_currentScene->setSceneManager(this);
        m_currentScene->create();
    }

    std::vector<int32_t>& temperature()
    {
        return m_temperature;
    }

    std::vector<int32_t>& humidity()
    {
        return m_humidity;
    }

    std::vector<int32_t>& airQuality()
    {
        return m_airQuality;
    }

private:
    std::function<void()> m_function;
    std::vector<int32_t> m_temperature;
    std::vector<int32_t> m_humidity;
    std::vector<int32_t> m_airQuality;
    std::unique_ptr<Scene> m_currentScene;
    lv_timer_t* m_timer;
    GasSensor m_gasSensor;
};