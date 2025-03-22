#include "Ltdc.h"
#include "Pixel.h"
#include "periph/SdRam.h"

extern "C" void LTDC_ER_IRQHandler()
{
    if (LTDC->ISR & LTDC_ISR_TERRIF) {
        LTDC->ICR |= LTDC_ICR_CTERRIF;
        sendMessage("LTDC Transfer Error!\r\n");
    }
    if (LTDC->ISR & LTDC_ISR_FUIF) {
        LTDC->ICR |= LTDC_ICR_CFUIF;
        sendMessage("LTDC FIFO UNDERRUN DETECTED\n");
        LTDC->ICR |= LTDC_ICR_CFUIF;
    }
}

uint32_t bytesPerPixelFromFormat(LayerFormat format)
{
    switch (format) {
    case LayerFormat::ARGB8888:
        return 4U;
    case LayerFormat::RGB888:
        return 3U;
    case LayerFormat::RGB565:
    case LayerFormat::ARGB1555:
    case LayerFormat::ARGB4444:
        return 2U;
    default:
        return 4U;
    }
    return 4U;
}

void setupLtdcLayer(uint32_t horizontalTotalPorch, uint32_t verticalTotalPorch, uint32_t activeWidth, uint32_t activeHeight, LTDC_Layer_TypeDef* layer, LayerFormat format, void* buffer);

void setupPllSai()
{
    RCC->PLLSAICFGR = (192 << RCC_PLLSAICFGR_PLLSAIN_Pos) | (0b010 << RCC_PLLSAICFGR_PLLSAIQ_Pos) | (0b010 << RCC_PLLSAICFGR_PLLSAIR_Pos);
    RCC->DCKCFGR1 &= ~RCC_DCKCFGR1_PLLSAIDIVR;
    RCC->DCKCFGR1 |= (0b01 << RCC_DCKCFGR1_PLLSAIDIVR_Pos);
    RCC->CR |= RCC_CR_PLLSAION;
    while (!(RCC->CR & RCC_CR_PLLSAIRDY)) {
    }
}

void ltdcReload()
{
    LTDC->SRCR |= LTDC_SRCR_IMR;
}

void updateLayer(LTDC_Layer_TypeDef* layer, void* buff)
{
    layer->CFBAR = (uint32_t)buff;
    LTDC->SRCR |= LTDC_SRCR_VBR;
}

void setupLtdc(const LcdConfig& config, void* buffer)
{
    for (GpioBuilder pin : config.ltdcPins) {
        pin.build();
    }
    setupPllSai();
    RCC->APB2ENR |= RCC_APB2ENR_LTDCEN;
    NVIC_EnableIRQ(LTDC_IRQn);
    NVIC_EnableIRQ(LTDC_ER_IRQn);
    LTDC->IER |= LTDC_IER_RRIE;
    LTDC->IER |= LTDC_IER_TERRIE;
    LTDC->IER |= LTDC_IER_FUIE;
    uint32_t horizontalTotalPorch = config.horizontalSync + config.horizontalBackPorch;
    uint32_t verticalTotalPorch = config.verticalSync + config.verticalBackPorch;
    uint32_t totalWidth = horizontalTotalPorch + config.activeWidth + config.horizontalFrontPorch;
    uint32_t totalHeight = verticalTotalPorch + config.activeHeight + config.verticalFrontPorch;
    LTDC->GCR &= ~(LTDC_GCR_HSPOL | LTDC_GCR_VSPOL | LTDC_GCR_DEPOL | LTDC_GCR_PCPOL);
    LTDC->GCR |= (config.horizontalSyncPolarity | config.verticalSyncPolarity | config.noDataEnablePolarity | config.pixelClockPolarity);
    LTDC->SSCR &= ~(LTDC_SSCR_VSH | LTDC_SSCR_HSW);
    LTDC->SSCR |= ((config.horizontalSync - 1) << LTDC_SSCR_HSW_Pos) | ((config.verticalSync - 1) << LTDC_SSCR_VSH_Pos);
    LTDC->BPCR &= ~(LTDC_BPCR_AVBP | LTDC_BPCR_AHBP);
    LTDC->BPCR |= (((config.horizontalBackPorch + config.horizontalSync) << LTDC_BPCR_AHBP_Pos) | ((config.verticalBackPorch + config.verticalSync) << LTDC_BPCR_AVBP_Pos));
    LTDC->AWCR &= ~(LTDC_AWCR_AAH | LTDC_AWCR_AAW);
    LTDC->AWCR |= (((config.horizontalBackPorch + config.horizontalSync + config.activeWidth - 1) << LTDC_AWCR_AAW_Pos) | ((config.verticalBackPorch + config.verticalSync + config.activeHeight - 1) << LTDC_AWCR_AAH_Pos));
    LTDC->TWCR &= ~(LTDC_TWCR_TOTALH | LTDC_TWCR_TOTALW);
    LTDC->TWCR |= (((totalWidth - 1) << LTDC_TWCR_TOTALW_Pos) | ((totalHeight - 1) << LTDC_TWCR_TOTALH_Pos));
    LTDC->BCCR = 0xFFFF0000;
    ltdcReload();
    LTDC->GCR |= LTDC_GCR_LTDCEN | LTDC_GCR_DEN;
    ltdcReload();
    setupLtdcLayer(horizontalTotalPorch, verticalTotalPorch, config.activeWidth, config.activeHeight, config.layer, config.format, buffer);
    ltdcReload();
}

void setupLtdcLayer(uint32_t horizontalTotalPorch, uint32_t verticalTotalPorch, uint32_t activeWidth, uint32_t activeHeight, LTDC_Layer_TypeDef* layer, LayerFormat format, void* buffer)
{
    uint32_t bytesPerPixel = bytesPerPixelFromFormat(format);
    uint32_t horizontalStart = horizontalTotalPorch + 1;
    uint32_t horizontalStop = horizontalTotalPorch + activeWidth;
    layer->WHPCR = (horizontalStart << LTDC_LxWHPCR_WHSTPOS_Pos) | (horizontalStop << LTDC_LxWHPCR_WHSPPOS_Pos);
    uint32_t verticalStart = verticalTotalPorch + 1;
    uint32_t verticalStop = verticalTotalPorch + activeHeight;
    layer->WVPCR = (verticalStart << LTDC_LxWVPCR_WVSTPOS_Pos) | (verticalStop << LTDC_LxWVPCR_WVSPPOS_Pos);
    layer->PFCR = (uint32_t)format;
    layer->CACR = 0xFF;
    layer->DCCR = 0xFF0000FF;
    layer->CFBAR = (uint32_t)buffer;
    uint32_t lineLength = (activeWidth * bytesPerPixel + 3U);
    uint32_t pitch = (activeWidth * bytesPerPixel);
    layer->CFBLR = ((lineLength << LTDC_LxCFBLR_CFBLL_Pos) & LTDC_LxCFBLR_CFBLL_Msk) | ((pitch << LTDC_LxCFBLR_CFBP_Pos) & LTDC_LxCFBLR_CFBP_Msk);
    layer->CFBLNR = activeHeight;
    layer->BFCR = ((uint32_t)LayerBF1::CompositeAlpha << LTDC_LxBFCR_BF1_Pos) | ((uint32_t)LayerBF2::InverseCompositeAlpha << LTDC_LxBFCR_BF2_Pos);
    layer->CR &= ~LTDC_LxCR_CLUTEN;
    layer->CR |= LTDC_LxCR_LEN;
}
