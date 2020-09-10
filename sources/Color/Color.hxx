#pragma once

#include "RGBA8888.hxx"
#include "RGBA16161616.hxx"
#include "ColorDetail.hxx"
#include "Quantize.hxx"

namespace Color
{
    union UnionRGBA5551 {
        uint16_t u;
        int16_t s;
        struct {
#if defined __ORDER_LITTLE_ENDIAN__
            uint8_t a:1, b:5, g:5, r:5;
#elif defined __ORDER_BIG_ENDIAN__
            uint8_t r:5, g:5, b:5, a:1;
#else
#error "fucked up endianness"
#endif
        };
        struct {
#if defined __ORDER_LITTLE_ENDIAN__
            char c2, c1;
#elif defined __ORDER_BIG_ENDIAN__
            char c1, c2;
#else
#error "fucked up endianness"
#endif
        };
    };

    union UnionRGB565 {
        uint16_t u;
        int16_t s;
        struct {
#if defined __ORDER_LITTLE_ENDIAN__
            uint8_t b:5, g:6, r:5;
#elif defined __ORDER_BIG_ENDIAN__
            uint8_t r:5, g:6, b:5;
#else
#error "fucked up endianness"
#endif
        };
        struct {
#if defined __ORDER_LITTLE_ENDIAN__
            char c2, c1;
#elif defined __ORDER_BIG_ENDIAN__
            char c1, c2;
#else
#error "fucked up endianness"
#endif
        };
    };
}
