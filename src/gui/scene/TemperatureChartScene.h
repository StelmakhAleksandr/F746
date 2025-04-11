#pragma once

#include "ChartScene.h"

class TemperatureChartScene : public ChartScene
{
public:
    TemperatureChartScene();
    ~TemperatureChartScene();

    void create() override;
};