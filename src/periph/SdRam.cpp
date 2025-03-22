#include "SdRam.h"

void setupSdram(const SdramConfig& config)
{
    setupFmcSdram(config);
    s_blockMap.resize(SDRAM_NUM_BLOCKS, false);
}

bool testMemory()
{
    int* sdram = reinterpret_cast<int*>(SDRAM_START);
    size_t elements = SDRAM_SIZE / sizeof(int);
    for (size_t i = 0; i < elements; i++) {
        sdram[i] = static_cast<int>(i);
    }
    __DSB();
    for (size_t i = 0; i < elements; i++) {
        if (sdram[i] != static_cast<int>(i)) {
            return false;
        }
    }
    return true;
}
