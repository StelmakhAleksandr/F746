#pragma once

#include "periph/Pins.h"
#include <functional>
#include <map>

class Touch;
inline Touch* G_TOUCH = nullptr;

void addIrqHandler(IRQn_Type irq, std::function<void()> func);

struct IrqConfig
{
    IRQn_Type type;
};

extern "C" void LTDC_IRQHandler();
extern "C" void EXTI15_10_IRQHandler();
