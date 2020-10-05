#pragma once

#include <cstdint>
#include <string>

namespace Color
{
    class RGBA8888;

    class RGBA16161616 {
    public:
        //--- public types and constants ---
        static constexpr uint64_t Black = 0x000000000000FFFFull;
        static constexpr uint64_t White = 0xFFFFFFFFFFFFFFFFull;
        static constexpr uint64_t Red = 0xFFFF00000000FFFFull;
        static constexpr uint64_t Green = 0x0000FFFF0000FFFFull;
        static constexpr uint64_t Blue = 0x00000000FFFFFFFFull;

        //--- public properties ---
        union {
            uint64_t value;
#if defined __ORDER_LITTLE_ENDIAN__
            struct {
                uint16_t a, b, g, r;
            };
            struct {
                uint8_t al, ah, bl, bh, gl, gh, rl, rh;
            };
            struct {
                char c8, c7, c6, c5, c4, c3, c2, c1;
            };
#elif defined __ORDER_BIG_ENDIAN__
            struct {
                uint16_t r, g, b, a;
            };
            struct {
                uint8_t rh, rl, gh, gl, bh, bl, ah, al;
            };
            struct {
                char c1, c2, c3, c4, c5, c6, c7, c8;
            };
#else
#error "fucked up endianness"
#endif
        };

        //--- public constructors ---
        RGBA16161616() noexcept;
        RGBA16161616(const uint64_t val) noexcept;
        RGBA16161616(const uint16_t rr, const uint16_t gg, const uint16_t bb,
                     const uint16_t aa) noexcept;
        RGBA16161616(const uint8_t rrh, const uint8_t rrl, const uint8_t ggh, const uint8_t ggl,
                     const uint8_t bbh, const uint8_t bbl, const uint8_t aah,
                     const uint8_t aal) noexcept;
        RGBA16161616(const RGBA16161616 &rhs) noexcept;
        RGBA16161616(RGBA16161616 &&rhs) noexcept;
        ~RGBA16161616() noexcept;

        //--- public operators ---
        RGBA16161616 &operator=(const RGBA16161616 &rhs) noexcept;
        RGBA16161616 &operator=(RGBA16161616 &&rhs) noexcept;
        bool operator==(const RGBA16161616 &rhs) const noexcept;
        bool operator!=(const RGBA16161616 &rhs) const noexcept;
        RGBA16161616 operator>>(const uint8_t val) const noexcept;
        RGBA16161616 operator<<(const uint8_t val) const noexcept;

        //--- public methods ---
        RGBA16161616 &set(const uint64_t val) noexcept;
        RGBA16161616 &set(const uint16_t rr, const uint16_t gg, const uint16_t bb,
                          const uint16_t aa) noexcept;
        RGBA16161616 &set(const uint8_t rrh, const uint8_t rrl, const uint8_t ggh,
                          const uint8_t ggl, const uint8_t bbh, const uint8_t bbl,
                          const uint8_t aah, const uint8_t aal) noexcept;
        uint16_t averageRGB() const noexcept;
        uint16_t averageRGBA() const noexcept;
        uint16_t grey() const noexcept;
        RGBA8888 toRGBA8888() const noexcept;
        std::string str() const noexcept;
    };
}
