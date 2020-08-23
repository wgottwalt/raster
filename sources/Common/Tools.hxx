#pragma once

#include <cmath>
#include <limits>
#include "Concepts.hxx"

namespace Common::Tools
{
    template <Concept::Number T, Concept::Number L, Concept::Number R>
    inline bool inRange(const T val, const L min = std::numeric_limits<T>::min(),
                        const R max = std::numeric_limits<T>::max(), const bool include_min = true,
                        const bool include_max = true) noexcept
    {
        if (min > max)
        {
            if (include_min && include_max)
                return (val >= max) && (val <= min);
            if (include_min && !include_max)
                return (val >= max) && (val < min);
            if (!include_min && include_max)
                return (val > max) && (val <= min);
            if (!include_min && !include_max)
                return (val > max) && (val < min);
        }

        if (include_min && include_max)
            return (val >= min) && (val <= max);
        if (include_min && !include_max)
            return (val >= min) && (val < max);
        if (!include_min && include_max)
            return (val > min) && (val <= max);

        return (val > min) && (val < min);
    }

    template <Concept::Number T, Concept::Number H, Concept::Number ...Tail>
    constexpr inline T min(const T val, const H head, const Tail... tail) noexcept
    {
        if constexpr (sizeof... (tail) > 0)
            return std::min(std::min(val, head), min(val, tail...));

        return std::min(val, head);
    }

    template <Concept::Number T, Concept::Number H, Concept::Number ...Tail>
    constexpr inline T max(const T val, const H head, const Tail... tail) noexcept
    {
        if constexpr (sizeof... (tail) > 0)
            return std::max(std::max(val, head), max(val, tail...));

        return std::max(val, head);
    }

    template <Concept::Number T>
    constexpr inline std::tuple<T,T> minMax(const T val1, const T val2) noexcept
    {
        return val1 > val2 ? std::tuple<T,T>(val2, val1) : std::tuple<T,T>(val1, val2);
    }

    template <Concept::Number T>
    constexpr inline std::tuple<T,T> maxMin(const T val1, const T val2) noexcept
    {
        return val1 < val2 ? std::tuple<T,T>(val2, val1) : std::tuple<T,T>(val1, val2);
    }
}
