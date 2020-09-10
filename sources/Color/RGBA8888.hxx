#pragma once

#include <cstdint>

namespace Color
{
    class RGBA8888 {
    public:
        //--- public types and constants ---
        static constexpr uint64_t Black = 0x000000FF;
        static constexpr uint64_t White = 0xFFFFFFFF;
        static constexpr uint64_t Red = 0xFF0000FF;
        static constexpr uint64_t Green = 0x00FF00FF;
        static constexpr uint64_t Blue = 0x0000FFFF;

        //--- public properties ---
        union {
            uint32_t value;
#if defined __ORDER_LITTLE_ENDIAN__
            struct {
                uint8_t a, b, g, r;
            };
            struct {
                char c4, c3, c2, c1;
            };
#elif defined __ORDER_BIG_ENDIAN__
            struct {
                uint8_t r, g, b, a;
            };
            struct {
                char c1, c2, c3, c4;
            };
#else
#error "fucked up endianness"
#endif
        };

        //--- public constructors ---
        RGBA8888() noexcept;
        RGBA8888(const uint32_t val) noexcept;
        RGBA8888(const uint8_t rr, const uint8_t gg, const uint8_t bb, const uint8_t aa) noexcept;
        RGBA8888(const RGBA8888 &rhs) noexcept;
        RGBA8888(RGBA8888 &&rhs) noexcept;
        ~RGBA8888() noexcept;

        //--- public operators ---
        RGBA8888 &operator=(const RGBA8888 &rhs) noexcept;
        RGBA8888 &operator=(RGBA8888 &&rhs) noexcept;
        bool operator==(const RGBA8888 &rhs) const noexcept;
        bool operator!=(const RGBA8888 &rhs) const noexcept;
        RGBA8888 operator>>(const uint8_t val) const noexcept;
        RGBA8888 operator<<(const uint8_t val) const noexcept;

        //--- public methods ---
        RGBA8888 &set(const uint32_t val) noexcept;
        RGBA8888 &set(const uint8_t rr, const uint8_t gg, const uint8_t bb,
                      const uint8_t aa) noexcept;
        uint8_t averageRGB() const noexcept;
        uint8_t averageRGBA() const noexcept;
        uint8_t grey() const noexcept;
    };
}
