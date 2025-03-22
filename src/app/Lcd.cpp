#include "Lcd.h"
#include "Task.h"

void Lcd::setup(const LcdConfig& config, void* buffer)
{
    m_button = GpioBuilder(config.displayButton).setModer(PinModer::Output).setSpeed(PinSpeed::VeryHigh).setType(PinType::PushPull).build();
    m_backlight = GpioBuilder(config.backLight).setModer(PinModer::Output).setSpeed(PinSpeed::VeryHigh).setType(PinType::PushPull).build();
    on();
    backLightOn();
    setupLtdc(config, buffer);
}

void Lcd::on()
{
    m_button.on();
}

void Lcd::off()
{
    m_button.off();
}

void Lcd::backLightOn()
{
    m_backlight.on();
}

void Lcd::backLightOff()
{
    m_backlight.off();
}
