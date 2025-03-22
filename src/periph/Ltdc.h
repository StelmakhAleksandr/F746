#pragma once

#include "config/Lcd.h"
#include "periph/Pixel.h"

void setupLtdc(const LcdConfig& config, void* buffer);
void updateLayer(LTDC_Layer_TypeDef* layer, void* buff);
uint32_t bytesPerPixelFromFormat(LayerFormat format);