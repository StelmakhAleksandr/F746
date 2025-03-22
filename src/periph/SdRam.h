#pragma once

#include "Fmc.h"
#include "Usart.h"
#include "config/Sdram.h"
#include <cstdint>

inline std::vector<bool> s_blockMap;

void setupSdram(const SdramConfig& config);
bool testMemory();

template <typename T>
class SdramAllocator : public std::allocator<T>
{
public:
    using size_type = size_t;
    using pointer = T*;
    using const_pointer = const T*;
    using value_type = T;

    template <typename U>
    struct rebind
    {
        using other = SdramAllocator<U>;
    };
    SdramAllocator() = default;
    template <typename U>
    SdramAllocator(const SdramAllocator<U>&)
    {
    }
    pointer allocate(size_type n, const void* hint = 0)
    {
        auto size = sizeof(T) * n;
        size_t blocksNeeded = (size + SDRAM_BLOCK_SIZE - 1) / SDRAM_BLOCK_SIZE;
        size_t sequence = 0;
        for (size_t i = 0; i < SDRAM_NUM_BLOCKS; ++i) {
            if (!s_blockMap[i]) {
                sequence++;
                if (sequence == blocksNeeded) {
                    size_t startBlock = i - blocksNeeded + 1;
                    for (size_t j = startBlock; j <= i; ++j) {
                        s_blockMap[j] = true;
                    }
                    auto addr = SDRAM_START + startBlock * SDRAM_BLOCK_SIZE;
                    return reinterpret_cast<pointer>(addr);
                }
            } else {
                sequence = 0;
            }
        }
        return nullptr;
    }
    void deallocate(pointer ptr, size_type n)
    {
        if (!ptr)
            return;

        auto size = sizeof(T) * n;
        uintptr_t startBlock = (reinterpret_cast<uintptr_t>(ptr) - SDRAM_START) / SDRAM_BLOCK_SIZE;
        size_t blocksToFree = (size + SDRAM_BLOCK_SIZE - 1) / SDRAM_BLOCK_SIZE;

        for (size_t i = startBlock; i < startBlock + blocksToFree; ++i) {
            s_blockMap[i] = false;
        }
    }
};
