#include "Fmc.h"

namespace {
constexpr uint32_t FMC_PINS_AF = 12;
}

void setMode(SdramCommandMode mode, uint32_t additional = 0);

void setupFmcSdram(const SdramConfig& config)
{
    for (auto pin : config.fmcPins) {
        pin.setModer(PinModer::Output)
            .setType(PinType::PushPull)
            .setSpeed(PinSpeed::VeryHigh)
            .setPull(PinPull::Up)
            .setAlternateFunction(FMC_PINS_AF)
            .build();
    }
    RCC->AHB3ENR |= RCC_AHB3ENR_FMCEN;
    FMC_Bank5_6->SDCR[0] &= ~(FMC_SDCR1_NC | FMC_SDCR1_NR | FMC_SDCR1_MWID | FMC_SDCR1_NB | FMC_SDCR1_CAS | FMC_SDCR1_WP | FMC_SDCR1_SDCLK | FMC_SDCR1_RBURST | FMC_SDCR1_RPIPE);
    FMC_Bank5_6->SDTR[0] &= ~(FMC_SDTR1_TMRD | FMC_SDTR1_TXSR | FMC_SDTR1_TRAS | FMC_SDTR1_TRC | FMC_SDTR1_TWR | FMC_SDTR1_TRP | FMC_SDTR1_TRCD);
    FMC_Bank5_6->SDCR[0] |= ((static_cast<uint32_t>(config.control.column) << FMC_SDCR1_NC_Pos)
        | (1 << FMC_SDCR1_NR_Pos)
        | (static_cast<uint32_t>(config.control.dataWidth) << FMC_SDCR1_MWID_Pos)
        | (static_cast<uint32_t>(config.control.bankCount) << FMC_SDCR1_NB_Pos)
        | (static_cast<uint32_t>(config.control.casLatency) << FMC_SDCR1_CAS_Pos)
        | (static_cast<uint32_t>(config.control.sdramClock) << FMC_SDCR1_SDCLK_Pos)
        | (static_cast<uint32_t>(config.control.burstRead) << FMC_SDCR1_RBURST_Pos)
        | (static_cast<uint32_t>(config.control.readPipe) << FMC_SDCR1_RPIPE_Pos));
    FMC_Bank5_6->SDTR[0] |= ((static_cast<uint32_t>(config.timing.modeRegToActive) << FMC_SDTR1_TMRD_Pos)
        | (static_cast<uint32_t>(config.timing.exitSelfRefresh) << FMC_SDTR1_TXSR_Pos)
        | (static_cast<uint32_t>(config.timing.activeToPrecharge) << FMC_SDTR1_TRAS_Pos)
        | (static_cast<uint32_t>(config.timing.rowCycle) << FMC_SDTR1_TRC_Pos)
        | (static_cast<uint32_t>(config.timing.writeRecovery) << FMC_SDTR1_TWR_Pos)
        | (static_cast<uint32_t>(config.timing.rowPrecharge) << FMC_SDTR1_TRP_Pos)
        | (static_cast<uint32_t>(config.timing.rowToColumn) << FMC_SDTR1_TRCD_Pos));
    FMC_Bank5_6->SDRTR = config.refreshTime;
    setMode(SdramCommandMode::ClockConfigurationEnable);
    setMode(SdramCommandMode::AllBankPrecharge);
    setMode(SdramCommandMode::AutoRefresh, (static_cast<uint32_t>(config.autoRefresh) << FMC_SDCMR_NRFS_Pos));
    setMode(SdramCommandMode::LoadModeRegister, (config.mrd.value() << FMC_SDCMR_MRD_Pos));
    setMode(SdramCommandMode::Normal);
}

void setMode(SdramCommandMode mode, uint32_t additional)
{
    FMC_Bank5_6->SDCMR = ((static_cast<uint32_t>(mode) << FMC_SDCMR_MODE_Pos) | FMC_SDCMR_CTB1 | additional);
    while (FMC_Bank5_6->SDSR & FMC_SDSR_BUSY) {
    }
}