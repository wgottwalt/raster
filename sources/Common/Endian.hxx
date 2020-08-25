#pragma once

#include <type_traits>
#include "Concepts.hxx"

namespace Common::Endian
{
    //--- basic constants ---

    enum class Order : int32_t {
        Little = __ORDER_LITTLE_ENDIAN__,
        Big = __ORDER_BIG_ENDIAN__,
        Native = __BYTE_ORDER__
    };

    //--- converters ---
    template <Common::Concept::Number N>
    constexpr inline N swap(const N val) noexcept
    {
        static_assert (sizeof (N) <= sizeof (uint64_t), "compile error: number to big");

        if constexpr (sizeof (N) == sizeof (uint8_t))
            return val;
        else if constexpr (sizeof (N) == sizeof (uint16_t))
            return ((val & 0x00FF) << 8) | ((val & 0xFF00) >> 8);
        else if constexpr (sizeof (N) == sizeof (uint32_t))
            return ((val & 0x000000FF) << 24) | ((val & 0x0000FF00) << 8) |
                   ((val & 0x00FF0000) >> 8) | ((val & 0xFF000000) >> 24);
        else if constexpr (sizeof (N) == sizeof (uint64_t))
            return ((val & 0x00000000000000FFull) << 56) | ((val & 0x000000000000FF00ull) << 40) |
                   ((val & 0x0000000000FF0000ull) << 24) | ((val & 0x00000000FF000000ull) << 8) |
                   ((val & 0x000000FF00000000ull) >> 8) | ((val & 0x0000FF0000000000ull) >> 24) |
                   ((val & 0x00FF000000000000ull) >> 40) | ((val & 0xFF00000000000000ull) >> 56);
        else // dummy, static_assert will catch this
            return val;
    }

    template <Common::Concept::Number N>
    constexpr inline N toBE(const N val) noexcept
    {
        if constexpr (Order::Native == Order::Little)
            return swap(val);
        else
            return val;
    }

    template <Common::Concept::Number N>
    constexpr inline N fromBE(const N val) noexcept
    {
        if constexpr (Order::Native == Order::Little)
            return swap(val);
        else
            return val;
    }

    template <Common::Concept::Number N>
    constexpr inline N toLE(const N val) noexcept
    {
        if constexpr (Order::Native == Order::Little)
            return val;
        else
            return swap(val);
    }

    template <Common::Concept::Number N>
    constexpr inline N fromLE(const N val) noexcept
    {
        if constexpr (Order::Native == Order::Little)
            return val;
        else
            return swap(val);
    }

    template <Common::Concept::Number N>
    constexpr inline N hostToNetwork(const N val) noexcept
    {
        return toBE(val);
    }

    template <Common::Concept::Number N>
    constexpr inline N networkToHost(const N val) noexcept
    {
        return fromBE(val);
    }

    //--- helper types ---

    union Union8 {
        uint8_t u;
        int8_t s;
        uint8_t u1;
        int8_t s1;
        char c1;
    };

    union Union16 {
        uint16_t u;
        int16_t s;
        struct {
#if defined __ORDER_LITTLE_ENDIAN__
            uint8_t u1, u2;
#elif defined __ORDER_BIG_ENDIAN__
            uint8_t u2, u1;
#else
#error "fucked up endianness"
#endif
        };
        struct {
#if defined __ORDER_LITTLE_ENDIAN__
            int8_t s1, s2;
#elif defined __ORDER_BIG_ENDIAN__
            int8_t s2, s1;
#else
#error "fucked up endianness"
#endif
        };
        struct {
#if defined __ORDER_LITTLE_ENDIAN__
            char c1, c2;
#elif defined __ORDER_BIG_ENDIAN__
            char c2, c1;
#else
#error "fucked up endianness"
#endif
        };
    };

    union Union32 {
        uint32_t u;
        int32_t s;
        struct {
#if defined __ORDER_LITTLE_ENDIAN__
            uint8_t u1, u2, u3, u4;
#elif defined __ORDER_BIG_ENDIAN__
            uint8_t u4, u3, u2, u1;
#else
#error "fucked up endianness"
#endif
        };
        struct {
#if defined __ORDER_LITTLE_ENDIAN__
            int8_t s1, s2, s3, s4;
#elif defined __ORDER_BIG_ENDIAN__
            int8_t s4, s3, s2, s1;
#else
#error "fucked up endianness"
#endif
        };
        struct {
#if defined __ORDER_LITTLE_ENDIAN__
            char c1, c2, c3, c4;
#elif defined __ORDER_BIG_ENDIAN__
            char c4, c3, c2, c1;
#else
#error "fucked up endianness"
#endif
        };
    };

    union Union64 {
        uint64_t u;
        int64_t s;
        struct {
#if defined __ORDER_LITTLE_ENDIAN__
            uint8_t u1, u2, u3, u4, u5, u6, u7, u8;
#elif defined __ORDER_BIG_ENDIAN__
            uint8_t u8, u7, u6, u5, u4, u3, u2, u1;
#else
#error "fucked up endianness"
#endif
        };
        struct {
#if defined __ORDER_LITTLE_ENDIAN__
            int8_t s1, s2, s3, s4, s5, s6, s7, s8;
#elif defined __ORDER_BIG_ENDIAN__
            int8_t s8, s7, s6, s5, s4, s3, s2, s1;
#else
#error "fucked up endianness"
#endif
        };
        struct {
#if defined __ORDER_LITTLE_ENDIAN__
            char c1, c2, c3, c4, c5, c6, c7, c8;
#elif defined __ORDER_BIG_ENDIAN__
            char c8, c7, c6, c5, c4, c3, c2, c1;
#else
#error "fucked up endianness"
#endif
        };
    };
}
