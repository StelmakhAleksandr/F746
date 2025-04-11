#include "SensorsScene.h"

#include "AirQualityChartScene.h"
#include "ChartScene.h"
#include "HumidityChartScene.h"
#include "MainScene.h"
#include "SceneManager.h"
#include "TemperatureChartScene.h"
#include "gui/image/image.h"
#include <functional>

SensorsScene::SensorsScene()
    : Scene()
    , m_screen(lv_obj_create(nullptr))
    , m_backButton(m_screen)
{
}

SensorsScene::~SensorsScene()
{
    lv_obj_del(m_screen);
}

void SensorsScene::create()
{
    lv_obj_clear_flag(m_screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_scr_load(m_screen);

    lv_obj_set_style_bg_color(m_screen, lv_color_white(), LV_PART_MAIN);

    setupBackButton(&m_backButton);

    auto addButton = [this](SensorGui& gui, int x, int y, const lv_image_dsc_t* image) {
        lv_obj_t* root = lv_obj_create(m_screen);
        lv_obj_set_size(root, 150, 150);
        lv_obj_align(root, LV_ALIGN_TOP_LEFT, x, y);
        lv_obj_set_scrollbar_mode(root, LV_SCROLLBAR_MODE_OFF);
        lv_obj_clear_flag(root, LV_OBJ_FLAG_SCROLLABLE);

        lv_obj_t* temperatureImg = lv_img_create(root);
        lv_img_set_src(temperatureImg, image);
        lv_obj_align(temperatureImg, LV_ALIGN_TOP_LEFT, -15, 0);

        gui.label = lv_label_create(root);
        lv_obj_set_style_text_font(gui.label, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_align(gui.label, LV_ALIGN_TOP_LEFT, 70, 20);
        lv_obj_set_style_text_color(gui.label, lv_color_black(), LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_t* imgbtn = lv_imgbtn_create(root);
        lv_imgbtn_set_src(imgbtn, LV_IMGBTN_STATE_RELEASED, NULL, &chart, NULL);
        lv_imgbtn_set_src(imgbtn, LV_IMGBTN_STATE_PRESSED, NULL, &chartPressed, NULL);
        lv_obj_align(imgbtn, LV_ALIGN_TOP_LEFT, 70, 60);

        lv_obj_add_event_cb(imgbtn, [](lv_event_t* e) {
            SensorGui* gui = static_cast<SensorGui*>(lv_event_get_user_data(e));
            gui->callback(e); }, LV_EVENT_CLICKED, &gui);
    };

    lv_obj_t* img = lv_img_create(m_screen);
    lv_img_set_src(img, &department);
    lv_img_set_pivot(img, 0, 0);
    lv_img_set_zoom(img, 150);
    lv_obj_align(img, LV_ALIGN_TOP_LEFT, 170, 5);

    lv_obj_t* img2 = lv_img_create(m_screen);
    lv_img_set_src(img2, &kpi_0);
    lv_img_set_pivot(img2, 0, 0);
    lv_img_set_zoom(img2, 110);
    lv_obj_align(img2, LV_ALIGN_TOP_LEFT, 370, 5);

    m_temperature.callback = [this](lv_event_t* event) {
        m_sceneManager->setScene<TemperatureChartScene>();
    };

    m_humidity.callback = [this](lv_event_t* event) {
        m_sceneManager->setScene<HumidityChartScene>();
    };

    m_airQuality.callback = [this](lv_event_t* event) {
        m_sceneManager->setScene<AirQualityChartScene>();
    };

    addButton(m_temperature, 10, 105, &temperature);
    addButton(m_humidity, 165, 105, &humidity);
    addButton(m_airQuality, 320, 105, &airquality);

    lv_label_set_text(m_temperature.label, "25°C");
    lv_label_set_text(m_humidity.label, "50%");
    lv_label_set_text(m_airQuality.label, "100");
}
