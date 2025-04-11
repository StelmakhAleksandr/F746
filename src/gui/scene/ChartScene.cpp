#include "ChartScene.h"
#include "SceneManager.h"
#include "SensorsScene.h"
#include "gui/font/font.h"

ChartScene::ChartScene()
    : Scene()
    , m_screen(lv_obj_create(nullptr))
    , m_backButton(m_screen)
    , m_chart(nullptr)
    , m_series(nullptr)
{
}

ChartScene::~ChartScene()
{
    lv_obj_del(m_screen);
}

void ChartScene::create()
{
    lv_obj_clear_flag(m_screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_scr_load(m_screen);

    m_backButton.setText(LV_SYMBOL_LEFT);
    m_backButton.setSize(35, 35);
    m_backButton.setAlignment(LV_ALIGN_TOP_LEFT, 5, 5);
    m_backButton.setTextAlignment(LV_TEXT_ALIGN_CENTER);
    m_backButton.setFont(&lv_font_montserrat_24);
    m_backButton.setCallback(LV_EVENT_CLICKED, [this](lv_event_t* e) {
        m_sceneManager->setScene<SensorsScene>();
    });

    lv_obj_set_style_bg_color(m_screen, lv_color_white(), LV_PART_MAIN);
}

lv_obj_t* ChartScene::createDropdownList(lv_obj_t* parent, const lv_obj_t* align)
{
    lv_obj_t* dropdown = lv_dropdown_create(parent);
    lv_dropdown_set_options(dropdown, "100%\n200%\n300%\n500%\nRealtime");
    lv_obj_set_size(dropdown, 120, 30);
    lv_obj_align_to(dropdown, align, LV_ALIGN_OUT_LEFT_MID, -10, 0);

    lv_obj_add_event_cb(dropdown, [](lv_event_t* e) {
        ChartScene* chartScene = static_cast<ChartScene*>(lv_event_get_user_data(e));
        uint16_t selectedIndex = lv_dropdown_get_selected((lv_obj_t*)lv_event_get_target(e));

        switch (selectedIndex) {
        case 0: 
            chartScene->setScale(ChartScale::Percent100);
            break;
        case 1: 
            chartScene->setScale(ChartScale::Percent200);
            break;
        case 2: 
            chartScene->setScale(ChartScale::Percent300);
            break;
        case 3: 
            chartScene->setScale(ChartScale::Percent500);
            break;
        case 4: 
            chartScene->setScale(ChartScale::RealTime);
            break;
        default:
            break;
        } }, LV_EVENT_VALUE_CHANGED, this);

    return dropdown;
}

lv_obj_t* ChartScene::createLabel(lv_obj_t* parent, const char* text)
{
    lv_obj_t* label = lv_label_create(parent);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_font(label, &Exo2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(label, LV_ALIGN_TOP_RIGHT, -10, 10);
    return label;
}

void ChartScene::createChart(lv_obj_t* parent)
{
    static const int32_t colDsc[] = { 30, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST };
    static const int32_t rowDsc[] = { LV_GRID_CONTENT, LV_GRID_FR(1), 20, LV_GRID_TEMPLATE_LAST };

    lv_obj_set_grid_dsc_array(parent, colDsc, rowDsc);
    lv_obj_set_style_pad_column(parent, 0, 0);
    lv_obj_set_style_pad_row(parent, 0, 0);

    lv_obj_t* wrapper = lv_obj_create(parent);
    lv_obj_remove_style(wrapper, nullptr, LV_PART_MAIN);
    lv_obj_set_grid_dsc_array(wrapper, nullptr, nullptr);
    lv_obj_set_grid_cell(wrapper, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 1, 2);
    lv_obj_set_scroll_dir(wrapper, LV_DIR_HOR);

    m_chart = lv_chart_create(wrapper);

    lv_group_add_obj(lv_group_get_default(), m_chart);
    lv_obj_add_flag(m_chart, (lv_obj_flag_t)(LV_OBJ_FLAG_SCROLL_ON_FOCUS | LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS));
    lv_obj_set_grid_cell(m_chart, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_obj_set_style_border_width(m_chart, 0, 0);
    lv_obj_set_style_radius(m_chart, 0, 0);

    lv_obj_add_event_cb(m_chart, [](lv_event_t* e) {
        auto* chartScene = static_cast<ChartScene*>(lv_event_get_user_data(e));
        lv_event_code_t code = lv_event_get_code(e);

        if (code == LV_EVENT_PRESSED || code == LV_EVENT_RELEASED) {
            lv_obj_invalidate((lv_obj_t*)lv_event_get_target(e));
        } else if (code == LV_EVENT_DRAW_TASK_ADDED) {
            lv_draw_task_t* drawTask = static_cast<lv_draw_task_t*>(lv_event_get_param(e));
            lv_draw_dsc_base_t* baseDsc = static_cast<lv_draw_dsc_base_t*>(lv_draw_task_get_draw_dsc(drawTask));

            if (baseDsc->part == LV_PART_INDICATOR && lv_chart_get_pressed_point(chartScene->m_chart) == baseDsc->id2) {
                lv_draw_rect_dsc_t outlineDsc;
                lv_draw_rect_dsc_init(&outlineDsc);
                outlineDsc.bg_opa = LV_OPA_TRANSP;
                outlineDsc.outline_color = lv_color_white();
                outlineDsc.outline_width = 2;
                outlineDsc.radius = LV_RADIUS_CIRCLE;

                lv_area_t drawTaskArea;
                lv_draw_task_get_area(drawTask, &drawTaskArea);
                lv_draw_rect(baseDsc->layer, &outlineDsc, &drawTaskArea);

                const int32_t* yArray = lv_chart_get_y_array(chartScene->m_chart, chartScene->m_series);
                std::string result = std::to_string(yArray[baseDsc->id2]);
                lv_point_t textSize;
                lv_text_get_size(&textSize, result.c_str(), &lv_font_montserrat_14, 0, 0, LV_COORD_MAX, LV_TEXT_FLAG_NONE);
                lv_area_t textArea;
                textArea.y2 = drawTaskArea.y1 - LV_DPX(15);
                textArea.y1 = textArea.y2 - textSize.y;
                textArea.x1 = drawTaskArea.x1 + (lv_area_get_width(&drawTaskArea) - textSize.x) / 2;
                textArea.x2 = textArea.x1 + textSize.x;

                lv_area_t bgArea;
                bgArea.x1 = textArea.x1 - LV_DPX(8);
                bgArea.x2 = textArea.x2 + LV_DPX(8);
                bgArea.y1 = textArea.y1 - LV_DPX(8);
                bgArea.y2 = textArea.y2 + LV_DPX(8);


                lv_draw_rect_dsc_t rectDsc;
                lv_draw_rect_dsc_init(&rectDsc);
                rectDsc.bg_color = lv_chart_get_series_color(chartScene->m_chart, chartScene->m_series);
                rectDsc.radius = LV_DPX(5);
                lv_draw_rect(baseDsc->layer, &rectDsc, &bgArea);

                lv_draw_label_dsc_t labelDsc;
                lv_draw_label_dsc_init(&labelDsc);
                labelDsc.color = lv_color_white();
                labelDsc.font = &lv_font_montserrat_14;
                labelDsc.text = result.c_str();
                labelDsc.text_local = true;
                lv_draw_label(baseDsc->layer, &labelDsc, &textArea);
                }
            } }, LV_EVENT_ALL, this);

    m_xScale = createXScale(wrapper, m_chart);
    m_yScale = createYScale(parent, m_chart, 0, 100);
}

lv_obj_t* ChartScene::createXScale(lv_obj_t* parent, const lv_obj_t* chart)
{
    lv_obj_t* horizontalScale = lv_scale_create(parent);
    lv_scale_set_mode(horizontalScale, LV_SCALE_MODE_HORIZONTAL_BOTTOM);
    lv_obj_set_grid_cell(horizontalScale, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_START, 1, 1);
    lv_scale_set_major_tick_every(horizontalScale, 1);
    // lv_scale_set_text_src(horizontalScale, labels);
    lv_scale_set_label_show(horizontalScale, false);
    lv_obj_set_height(horizontalScale, 10);
    lv_obj_set_style_pad_hor(horizontalScale, lv_obj_get_style_pad_left(chart, 0), 0);
    return horizontalScale;
}

lv_obj_t* ChartScene::createYScale(lv_obj_t* parent, const lv_obj_t* chart, int min, int max)
{
    lv_obj_t* verticalScale = lv_scale_create(parent);
    lv_scale_set_mode(verticalScale, LV_SCALE_MODE_VERTICAL_LEFT);
    lv_obj_set_grid_cell(verticalScale, LV_GRID_ALIGN_END, 0, 1, LV_GRID_ALIGN_STRETCH, 1, 1);
    lv_scale_set_total_tick_count(verticalScale, 15);
    lv_scale_set_major_tick_every(verticalScale, 2);
    lv_scale_set_range(verticalScale, min, max);
    lv_obj_set_style_pad_ver(verticalScale, lv_obj_get_style_pad_top(chart, 0), 0);
    return verticalScale;
}

void ChartScene::updateData(std::vector<int32_t>& data)
{
    lv_chart_set_ext_y_array(m_chart, m_series, data.data());
    lv_chart_set_next_value(m_chart, m_series, data.back());
    lv_scale_set_total_tick_count(m_xScale, data.size());
    lv_chart_set_div_line_count(m_chart, 0, data.size());
    lv_chart_set_point_count(m_chart, data.size());
    updateSize();
}

void ChartScene::updateSize()
{
    int32_t percent = 0;
    switch (m_scale) {
    case ChartScale::Percent100:
        percent = 100;
        break;
    case ChartScale::Percent200:
        percent = 200;
        break;
    case ChartScale::Percent300:
        percent = 300;
        break;
    case ChartScale::Percent500:
        percent = 500;
        break;
    case ChartScale::RealTime: {
        int32_t basePointCount = 15;
        percent = 100;
        int32_t scaleFactor = 5;
        int32_t points = lv_chart_get_point_count(m_chart);
        if (points > basePointCount) {
            percent += (points - basePointCount) * scaleFactor;
        }
    } break;
    default:
        percent = 100;
        break;
    }

    lv_obj_set_width(m_chart, LV_PCT(percent));
    lv_obj_set_width(m_xScale, LV_PCT(percent));
}