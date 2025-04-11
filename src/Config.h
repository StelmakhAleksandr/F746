#pragma once

#include "stmcmp/stmcmp.h"

inline TouchConfig touch()
{
    return {
        .button = PI13,
        .address = 0x38,
        .statusReg = 0x02,
        .firstXhReg = 0x03,
        .sizeStateReg = 6,
        .xBitmask = 0x0F,
        .yBitmask = 0x0F,
    };
}

inline std::vector<GpioBuilder> fmcPins()
{
    return {
        PF0, // A0
        PF1, // A1
        PF2, // A2
        PF3, // A3
        PF4, // A4
        PF5, // A5
        PF12, // A6
        PF13, // A7
        PF14, // A8
        PF15, // A9
        PG0, // A10
        PG1, // A11
        PG4, // BA0
        PG5, // BA1
        PD14, // D0
        PD15, // D1
        PD0, // D2
        PD1, // D3
        PE7, // D4
        PE8, // D5
        PE9, // D6
        PE10, // D7
        PE11, // D8
        PE12, // D9
        PE13, // D10
        PE14, // D11
        PE15, // D12
        PD8, // D13
        PD9, // D14
        PD10, // D15
        PE0, // NBL0
        PE1, // NBL1
        PC3, // SDCKEn
        PG8, // SDCLK
        PG15, // SDNCAS
        PH3, // SDNEn
        PH5, // SDNWE
        PF11 // SDNRAS
    };
}

inline SdramConfig MT48LC4M32B2B56A()
{
    return SdramConfig {
        .control = {
            .column = ColumnBits::Bits8,
            .dataWidth = DataWidth::Bits16,
            .bankCount = BankCount::Four,
            .casLatency = CasLatency::Latency2,
            .sdramClock = SdramClock::TwoHCLK,
            .burstRead = BurstRead::Enabled,
            .readPipe = ReadPipe::NoDelay,
        },
        .timing = {
            .modeRegToActive = SdramCycles::Cycles2,
            .exitSelfRefresh = SdramCycles::Cycles7,
            .activeToPrecharge = SdramCycles::Cycles4,
            .rowCycle = SdramCycles::Cycles7,
            .writeRecovery = SdramCycles::Cycles2,
            .rowPrecharge = SdramCycles::Cycles2,
            .rowToColumn = SdramCycles::Cycles2,
        },
        .mrd = {
            .casLatency = CasLatency::Latency2,
            .burstLength = BurstLength::One,
            .burstType = BurstType::Sequential,
            .writeMode = WriteMode::SingleLocationAccess,
        },
        .autoRefresh = SdramCycles::Cycles8,
        .refreshTime = 781,
        .fmcPins = fmcPins(),
    };
}

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

inline LvglConfig lvglConfig()
{
    return {
        .mode = LV_DISPLAY_RENDER_MODE_PARTIAL,
        .linesPerBuffer = 200,
        .allocator = AllocatorType::Standard
    };
}