#pragma once

#include "periph/SdRam.h"
#include "periph/config/Lcd.h"
#include <cstring>
#include <memory>
#include <vector>

class Pixel
{
public:
    Pixel() = default;
    Pixel(const uint8_t* data, size_t bytesPerPixel)
        : m_bytesPerPixel(bytesPerPixel)
    {
        std::memcpy(m_data, data, bytesPerPixel);
    }
    Pixel(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue, size_t bytesPerPixel = 4);
    uint8_t alpha() const;
    uint8_t red() const;
    uint8_t green() const;
    uint8_t blue() const;
    void setColor(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue);

    const uint8_t* rawData() const
    {
        return m_data;
    }

    size_t bytesPerPixel() const
    {
        return m_bytesPerPixel;
    }

private:
    uint8_t m_data[4] {};
    size_t m_bytesPerPixel = 4;
};

class PixelContainer
{
public:
    virtual ~PixelContainer() = default;
    virtual size_t size() const = 0;
    virtual Pixel at(size_t x, size_t y) const = 0;
    virtual void set(size_t x, size_t y, const Pixel& pixel) = 0;
    virtual uint32_t baseAddress() const = 0;
};

template <typename Allocator = std::allocator<uint8_t>>
class PixelContainerImpl : public PixelContainer
{
public:
    using VectorType = std::vector<uint8_t, Allocator>;

    PixelContainerImpl(size_t width, size_t height, size_t bytesPerPixel)
        : m_width(width)
        , m_height(height)
        , m_bytesPerPixel(bytesPerPixel)
    {
        pixels.resize(m_width * m_height * bytesPerPixel);
    }

    size_t size() const override
    {
        return pixels.size();
    }

    Pixel at(size_t x, size_t y) const override
    {
        size_t index = (y * m_width * m_bytesPerPixel) + x * m_bytesPerPixel;
        return Pixel(&pixels[index], m_bytesPerPixel);
    }

    void set(size_t x, size_t y, const Pixel& pixel) override
    {
        size_t index = (y * m_width * m_bytesPerPixel) + x * m_bytesPerPixel;
        std::memcpy(&pixels[index], pixel.rawData(), m_bytesPerPixel);
    }

    uint32_t baseAddress() const override
    {
        return reinterpret_cast<uint32_t>(pixels.data());
    }

private:
    VectorType pixels;
    size_t m_width;
    size_t m_height;
    size_t m_bytesPerPixel;
};

std::shared_ptr<PixelContainer> makePixelContainer(size_t width, size_t height, uint32_t bytesPerPixel, AllocatorType allocator);