#include "TemperatureChartScene.h"

#include "SceneManager.h"

TemperatureChartScene::TemperatureChartScene()
    : ChartScene()
{
}

TemperatureChartScene::~TemperatureChartScene()
{
}

void TemperatureChartScene::create()
{
    ChartScene::create();

    lv_obj_t* wrapper = lv_obj_create(m_screen);
    lv_obj_remove_style_all(wrapper);
    lv_obj_set_size(wrapper, lv_obj_get_width(m_screen), lv_obj_get_height(m_screen) - 45);
    lv_obj_align(wrapper, LV_ALIGN_TOP_LEFT, 0, 45);
    lv_obj_set_flex_flow(wrapper, LV_FLEX_FLOW_ROW);

    lv_obj_t* chartContainer = lv_obj_create(wrapper);
    lv_obj_remove_style_all(chartContainer);
    lv_obj_set_height(chartContainer, lv_pct(100));
    lv_obj_set_flex_grow(chartContainer, 1);

    auto* label = createLabel(m_screen, "Температура");
    createDropdownList(m_screen, label);

    createChart(chartContainer);

    lv_color_t c = lv_palette_main(LV_PALETTE_RED);
    m_series = lv_chart_add_series(m_chart, c, LV_CHART_AXIS_PRIMARY_Y);
    m_sceneManager->subscribe([this]() {
        // printf("Temperature: %d\n", m_sceneManager->temperature().size());
        updateData(m_sceneManager->temperature());
    });
    updateData(m_sceneManager->temperature());
}