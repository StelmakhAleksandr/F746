#include "LvglPort.h"

#include "Irq.h"
#include "Task.h"

#include "demos/lv_demos.h"
#include "periph/Sdram.h"
#include <cstring>

extern "C" uint32_t ticksElapsed();

#if LV_USE_STDLIB_MALLOC == LV_STDLIB_CUSTOM
#define START (0xC0000000U + 1024 * 1024 * 6 - 103)
static uint8_t* sdram_ptr = reinterpret_cast<uint8_t*>(START);
static size_t sdram_offset = 0;

extern "C" void lv_mem_init(void)
{
}

extern "C" void* lv_malloc_core(size_t size)
{
    // sendMessage("lv_malloc_core " + std::to_string(size) + "\r\n");
    void* ptr = (void*)((uintptr_t)(sdram_ptr + sdram_offset + 3) & ~3);
    sdram_offset += (size + 3) & ~3;
    return ptr;
}

extern "C" void lv_free_core(void* ptr)
{
    // sendMessage("lv_free_core \r\n");
}

extern "C" void* lv_realloc_core(void* ptr, size_t new_size)
{
    // sendMessage("lv_realloc_core " + std::to_string(new_size) + "\r\n");
    void* new_ptr = lv_malloc_core(new_size);
    if (ptr && new_ptr) {
        memcpy(new_ptr, ptr, new_size); // Копіюємо дані
        lv_free_core(ptr); // Звільняємо стару пам’ять
    }
    return new_ptr;
}

extern "C" void lv_mem_monitor_core(lv_mem_monitor_t* mon_p)
{
    sendMessage("lv_mem_monitor_core \r\n");
    sendMessage("lv_mem_monitor_core FINISH \r\n");
}

extern "C" lv_result_t lv_mem_test_core(void)
{
    // sendMessage("lv_mem_test_core\r\n");
    return LV_RESULT_OK;
}

#endif

lv_color_format_t lvglFormatFromLayerFormat(LayerFormat layerFormat)
{
    switch (layerFormat) {
    case LayerFormat::ARGB8888:
        return LV_COLOR_FORMAT_ARGB8888;
    case LayerFormat::RGB888:
        return LV_COLOR_FORMAT_RGB888;
    case LayerFormat::RGB565:
        return LV_COLOR_FORMAT_RGB565;
    default:
        return LV_COLOR_FORMAT_RGB565;
    }
    return LV_COLOR_FORMAT_RGB565;
}

void test(lv_log_level_t level, const char* buf)
{
    sendMessage(buf);
}

void LvglPort::setup(const LcdConfig& lcdConfig, const TouchConfig& touchConfig)
{
    lv_init();
    setupDisplayDriver(lcdConfig);
    setupTouchDriver(touchConfig);
    lv_tick_set_cb(ticksElapsed);

    lv_log_register_print_cb(test);

    makeTask(&LvglPort::lvglTimerTask, "LvglTimer", TaskStackSize::Large, TaskPriority::High);
}

void LvglPort::setupDisplayDriver(const LcdConfig& lcdConfig)
{
    auto firstBuffer = makePixelContainer(lcdConfig.activeWidth, lcdConfig.activeHeight, bytesPerPixelFromFormat(lcdConfig.format), lcdConfig.allocator);
    auto secondBuffer = makePixelContainer(lcdConfig.activeWidth, lcdConfig.activeHeight, bytesPerPixelFromFormat(lcdConfig.format), lcdConfig.allocator);
    m_buffers = { firstBuffer, secondBuffer };
    m_lcd.setup(lcdConfig, (void*)firstBuffer->baseAddress());
    auto display = lv_display_create(lcdConfig.activeWidth, lcdConfig.activeHeight);
    lv_display_set_user_data(display, lcdConfig.layer);
    addIrqHandler(LTDC_IRQn, [display] {
        if (LTDC->ISR & LTDC_ISR_RRIF) {
            lv_display_flush_ready(display);
            LTDC->ICR |= LTDC_ICR_CRRIF;
        }
    });
    lv_display_set_flush_cb(display, [](lv_display_t* disp, const lv_area_t* area, uint8_t* buff) {
        LTDC_Layer_TypeDef* layer = (LTDC_Layer_TypeDef*)lv_display_get_user_data(disp);
        updateLayer(layer, buff);
    });
    lv_display_set_color_format(display, lvglFormatFromLayerFormat(lcdConfig.format));
    lv_display_set_buffers(display, (void*)firstBuffer->baseAddress(), (void*)secondBuffer->baseAddress(), firstBuffer->size(), LV_DISPLAY_RENDER_MODE_FULL);
}

void LvglPort::setupTouchDriver(const TouchConfig& touchConfig)
{
    m_touch.setup(touchConfig);
    lv_indev_t* indev = lv_indev_create();
    lv_indev_set_user_data(indev, &m_touch);
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, [](lv_indev_t* indev, lv_indev_data_t* data) {
        auto touch = (Touch*)lv_indev_get_user_data(indev);
        if (touch->hasTouch()) {
            auto touchState = touch->lastTouch();
            data->point.x = touchState.x;
            data->point.y = touchState.y;
            data->continue_reading = touch->hasTouch();
            data->state = LV_INDEV_STATE_PRESSED;
        } else {
            data->state = LV_INDEV_STATE_RELEASED;
        }
    });
}

void LvglPort::run()
{
    lv_demo_benchmark();
}

void LvglPort::lvglTimerTask()
{
    while (true) {
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}