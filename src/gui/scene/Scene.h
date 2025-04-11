#pragma once

#include "../Button.h"
#include "lvgl.h"

class SceneManager;

class Scene
{
public:
    virtual ~Scene() = default;
    virtual void create() = 0;

    void setSceneManager(SceneManager* manager)
    {
        m_sceneManager = manager;
    }

    void setupBackButton(Button* button);

protected:
    SceneManager* m_sceneManager = nullptr;
};