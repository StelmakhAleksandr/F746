#pragma once

#include "../Button.h"
#include "Scene.h"
#include <lvgl.h>

class ChartScene : public Scene
{
public:
    ChartScene();
    ~ChartScene() override;

    void create() override;

protected:
    enum class ChartScale : uint8_t
    {
        Percent100,
        Percent200,
        Percent300,
        Percent500,
        RealTime,
    };
    void createChart(lv_obj_t* parent);
    lv_obj_t* createDropdownList(lv_obj_t* parent, const lv_obj_t* align);
    lv_obj_t* createXScale(lv_obj_t* parent, const lv_obj_t* chart);
    lv_obj_t* createYScale(lv_obj_t* parent, const lv_obj_t* chart, int min, int max);
    lv_obj_t* createLabel(lv_obj_t* parent, const char* text);
    void updateData(std::vector<int32_t>& data);

    void updateSize();
    void setScale(ChartScale scale)
    {
        if (m_scale == scale) {
            return;
        }
        lv_obj_t* wrapper = lv_obj_get_parent(m_chart);
        lv_obj_scroll_to_x(wrapper, 0, LV_ANIM_OFF);
        m_scale = scale;
        updateSize();
    }

    lv_obj_t* m_screen;
    Button m_backButton;
    lv_obj_t* m_chart;
    lv_chart_series_t* m_series;
    lv_obj_t *m_xScale, *m_yScale;
    ChartScale m_scale = ChartScale::Percent100;
};