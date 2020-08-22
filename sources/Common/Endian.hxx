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
}
