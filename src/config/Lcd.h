#include "periph/config/Lcd.h"

#include "lvgl.h"

inline std::vector<GpioBuilder> ltdcPins()
{
    return {
        PI15.setAlternateFunction(14).setSpeed(PinSpeed::VeryHigh), // LCD_R0
        PJ0.setAlternateFunction(14).setSpeed(PinSpeed::VeryHigh), // LCD_R1
        PJ1.setAlternateFunction(14).setSpeed(PinSpeed::VeryHigh), // LCD_R2
        PJ2.setAlternateFunction(14).setSpeed(PinSpeed::VeryHigh), // LCD_R3
        PJ3.setAlternateFunction(14).setSpeed(PinSpeed::VeryHigh), // LCD_R4
        PJ4.setAlternateFunction(14).setSpeed(PinSpeed::VeryHigh), // LCD_R5
        PJ5.setAlternateFunction(14).setSpeed(PinSpeed::VeryHigh), // LCD_R6
        PJ6.setAlternateFunction(14).setSpeed(PinSpeed::VeryHigh), // LCD_R7
        PJ7.setAlternateFunction(14).setSpeed(PinSpeed::VeryHigh), // LCD_G0
        PJ8.setAlternateFunction(14).setSpeed(PinSpeed::VeryHigh), // LCD_G1
        PJ9.setAlternateFunction(14).setSpeed(PinSpeed::VeryHigh), // LCD_G2
        PJ10.setAlternateFunction(14).setSpeed(PinSpeed::VeryHigh), // LCD_G3
        PJ11.setAlternateFunction(14).setSpeed(PinSpeed::VeryHigh), // LCD_G4
        PK0.setAlternateFunction(14).setSpeed(PinSpeed::VeryHigh), // LCD_G5
        PK1.setAlternateFunction(14).setSpeed(PinSpeed::VeryHigh), // LCD_G6
        PK2.setAlternateFunction(14).setSpeed(PinSpeed::VeryHigh), // LCD_G7
        PE4.setAlternateFunction(14).setSpeed(PinSpeed::VeryHigh), // LCD_B0
        PJ13.setAlternateFunction(14).setSpeed(PinSpeed::VeryHigh), // LCD_B1
        PJ14.setAlternateFunction(14).setSpeed(PinSpeed::VeryHigh), // LCD_B2
        PJ15.setAlternateFunction(14).setSpeed(PinSpeed::VeryHigh), // LCD_B3
        PG12.setAlternateFunction(9).setSpeed(PinSpeed::VeryHigh), // LCD_B4
        PK4.setAlternateFunction(14).setSpeed(PinSpeed::VeryHigh), // LCD_B5
        PK5.setAlternateFunction(14).setSpeed(PinSpeed::VeryHigh), // LCD_B6
        PK6.setAlternateFunction(14).setSpeed(PinSpeed::VeryHigh), // LCD_B7
        PI9.setAlternateFunction(14).setSpeed(PinSpeed::VeryHigh), // LCD_VSYNC
        PI10.setAlternateFunction(14).setSpeed(PinSpeed::VeryHigh), // LCD_HSYNC
        PI14.setAlternateFunction(14).setSpeed(PinSpeed::VeryHigh), // LCD_CLK
        PK7.setAlternateFunction(14).setSpeed(PinSpeed::VeryHigh), // LCD_DE
        PH1.setModer(PinModer::Input)
    };
}

inline LcdConfig lcd()
{
    return {
        .ltdcPins = ltdcPins(),
        .displayButton = PI12,
        .backLight = PK3,
        .activeWidth = 480,
        .activeHeight = 272,
        .horizontalBackPorch = 13, // HBP = 13 тактів
        .horizontalFrontPorch = 32, // HFP = 32 такти
        .verticalFrontPorch = 2, // VFP = 2 такти
        .verticalBackPorch = 2, // VBP = 2 такти
        .horizontalSync = 41, // HSYNC = 41 такт
        .verticalSync = 10, // VSYNC = 10 тактів
        .horizontalSyncPolarity = false,
        .verticalSyncPolarity = false,
        .noDataEnablePolarity = false,
        .pixelClockPolarity = false,
        .layer = LTDC_Layer2,
        .format = LayerFormat::RGB565,
        .allocator = AllocatorType::SDRAM
    };
}
