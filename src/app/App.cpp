#include "App.h"

#include "periph/Clocks.h"
#include "periph/SdRam.h"
#include "periph/Usart.h"

void App::setup()
{
    ClocksBuilder()
        .setHse(25_MHz)
        .setSysclk(216_MHz)
        .setHclk(216_MHz)
        .setPclk1(54_MHz)
        .setPclk2(108_MHz)
        .build();
    setupUsart();
    m_led = PI1.setModer(PinModer::Output).build();
    SysTick->VAL = 0;
    SysTick->LOAD = 216000 - 1;
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

    setupSdram(MT48LC4M32B2B56A());
    m_lvgl.setup(lcd(), touch());

    makeTask(std::bind(&App::blinkTask, this), "Blink", TaskStackSize::Small, TaskPriority::Low);
    makeTask(std::bind(&App::helloTask, this), "Hello", TaskStackSize::Small, TaskPriority::Low);

    m_lvgl.run();
}

void App::run()
{
    vTaskStartScheduler();
}

void App::blinkTask()
{
    while (true) {
        m_led.toggle();
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void App::helloTask()
{
    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
