#pragma once

#include "../Button.h"
#include "Scene.h"

class MainScene : public Scene
{
public:
    MainScene();
    ~MainScene();

    void create() override;

private:
    lv_obj_t* m_screen;
};