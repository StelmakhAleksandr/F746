#include "Config.h"

#include "demos/lv_demos.h"
#include "gui/scene/ChartScene.h"
#include "gui/scene/MainScene.h"
#include "gui/scene/SceneManager.h"
#include "gui/scene/SensorsScene.h"
#include "stmcmp/periph/Usart.h"

int main()
{
    auto clocks = ClocksBuilder()
                      .setHse(25_MHz)
                      .setSysclk(216_MHz)
                      .setHclk(216_MHz)
                      .setPclk1(54_MHz)
                      .setPclk2(108_MHz)
                      .build();

    setupSdram(MT48LC4M32B2B56A());
    setupUsart(clocks, 230400);

    LvglPort lvgl;
    lvgl.setup(lcd(), touch(), lvglConfig());

    SceneManager sceneManager;
    sceneManager.setScene<MainScene>();

    // lv_demo_widgets();

    while (true) {
        lv_task_handler();
    }

    return 0;
}