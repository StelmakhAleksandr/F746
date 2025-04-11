#include "MainScene.h"
#include "../font/font.h"
#include "SceneManager.h"
#include "SensorsScene.h"
#include "gui/image/image.h"
#include "lvgl.h"

MainScene::MainScene()
    : Scene()
    , m_screen(lv_obj_create(nullptr))
{
}

MainScene::~MainScene()
{
    lv_obj_del(m_screen);
}

void MainScene::create()
{
    lv_obj_clear_flag(m_screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_scr_load(m_screen);

    lv_obj_set_style_bg_color(m_screen, lv_color_white(), LV_PART_MAIN);

    lv_obj_t* button = lv_btn_create(m_screen);
    lv_obj_set_size(button, 130, 40);
    lv_obj_align(button, LV_ALIGN_BOTTOM_MID, 0, -10);

    lv_obj_t* symbolLabel = lv_label_create(button);
    lv_label_set_text(symbolLabel, LV_SYMBOL_POWER);
    lv_obj_align(symbolLabel, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_text_font(symbolLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t* textLabel = lv_label_create(button);
    lv_label_set_text(textLabel, "Старт");
    lv_obj_set_style_text_font(textLabel, &Exo2, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_align(textLabel, LV_ALIGN_LEFT_MID, 30, 0);

    lv_obj_add_event_cb(button, [](lv_event_t* e) {
        auto* scene = static_cast<MainScene*>(lv_event_get_user_data(e));
        scene->m_sceneManager->setScene<SensorsScene>(); }, LV_EVENT_CLICKED, this);

    lv_obj_t* img = lv_img_create(m_screen);
    lv_img_set_src(img, &department);
    lv_obj_align(img, LV_ALIGN_TOP_MID, 0, 15);
}
