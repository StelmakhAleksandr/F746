#include "Pixel.h"

Pixel::Pixel(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue, size_t bytesPerPixel)
    : m_bytesPerPixel(bytesPerPixel)
{
    setColor(alpha, red, green, blue);
}

uint8_t Pixel::alpha() const
{
    if (m_bytesPerPixel == 4) {
        return m_data[3];
    }
    return 255;
}

uint8_t Pixel::red() const
{
    if (m_bytesPerPixel == 4 || m_bytesPerPixel == 3)
        return m_data[2];

    if (m_bytesPerPixel == 2) {
        uint16_t rgb565;
        std::memcpy(&rgb565, m_data, 2);
        return (rgb565 >> 11) & 0x1F;
    }
    return 0;
}

uint8_t Pixel::green() const
{
    if (m_bytesPerPixel == 4 || m_bytesPerPixel == 3)
        return m_data[1];

    if (m_bytesPerPixel == 2) {
        uint16_t rgb565;
        std::memcpy(&rgb565, m_data, 2);
        return (rgb565 >> 5) & 0x3F;
    }
    return 0;
}

uint8_t Pixel::blue() const
{
    if (m_bytesPerPixel == 4 || m_bytesPerPixel == 3)
        return m_data[0];

    if (m_bytesPerPixel == 2) {
        uint16_t rgb565;
        std::memcpy(&rgb565, m_data, 2);
        return rgb565 & 0x1F;
    }
    return 0;
}

void Pixel::setColor(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue)
{
    if (m_bytesPerPixel == 4) {
        uint32_t argb = (alpha << 24) | (red << 16) | (green << 8) | blue;
        std::memcpy(m_data, &argb, 4);
    } else if (m_bytesPerPixel == 3) {
        m_data[0] = red;
        m_data[1] = green;
        m_data[2] = blue;
    } else if (m_bytesPerPixel == 2) {
        uint16_t rgb565 = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
        std::memcpy(m_data, &rgb565, 2);
    }
}

std::shared_ptr<PixelContainer> makePixelContainer(size_t width, size_t height, uint32_t bytesPerPixel, AllocatorType allocator)
{
    if (allocator == AllocatorType::SDRAM) {
        return std::make_shared<PixelContainerImpl<SdramAllocator<uint8_t>>>(width, height, bytesPerPixel);
    } else {
        return std::make_shared<PixelContainerImpl<std::allocator<uint8_t>>>(width, height, bytesPerPixel);
    }
    return nullptr;
}