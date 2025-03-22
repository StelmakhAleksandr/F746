#pragma once

#include "periph/Pins.h"
#include <variant>
#include <vector>

enum class LayerBF1 : uint8_t
{
    ConstantAlpha = 0b100,
    CompositeAlpha = 0b110,
};

enum class LayerBF2 : uint8_t
{
    InverseConstantAlpha = 0b101,
    InverseCompositeAlpha = 0b111,
};

enum class LayerFormat : uint8_t
{
    ARGB8888 = 0b000,
    RGB888 = 0b001,
    RGB565 = 0b010,
    ARGB1555 = 0b011,
    ARGB4444 = 0b100,
};

enum class AllocatorType : uint8_t
{
    SDRAM,
    Standard
};

struct LcdConfig
{
    std::vector<GpioBuilder> ltdcPins;
    GpioBuilder displayButton;
    GpioBuilder backLight;
    uint32_t activeWidth;
    uint32_t activeHeight;
    uint32_t horizontalBackPorch;
    uint32_t horizontalFrontPorch;
    uint32_t verticalFrontPorch;
    uint32_t verticalBackPorch;
    uint32_t horizontalSync;
    uint32_t verticalSync;
    bool horizontalSyncPolarity;
    bool verticalSyncPolarity;
    bool noDataEnablePolarity;
    bool pixelClockPolarity;
    LTDC_Layer_TypeDef* layer;
    LayerFormat format;
    AllocatorType allocator;
};
