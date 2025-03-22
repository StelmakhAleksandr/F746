#include "periph/config/Sdram.h"

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