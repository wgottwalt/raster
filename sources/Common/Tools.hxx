#pragma once

#include <algorithm>
#include <cmath>
#include <limits>
#include <string>
#include "Concepts.hxx"

namespace Common::Tools
{
    //--- value/type extractors ---

    template <typename T>
    constexpr inline typename std::underlying_type<T>::type valueOf(const T &val) noexcept
    {
        return static_cast<typename std::underlying_type<T>::type>(val);
    }

    //--- compare functions ---

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

    //--- list checking ---

    template <typename T, typename H, typename ...Tail>
    constexpr inline bool includes(const T val, const H head, const Tail... tail) noexcept
    {
        if constexpr (sizeof... (tail) > 0)
            return (val == head) || includes(val, tail...);
        return val == head;
    }

    //--- string manipulation ---

    template <typename T1, typename T2>
    size_t countSubSequences(T1 from_it, T2 to_it, T1 search_from_it, T2 search_to_it)
    {
        const ssize_t source_seq_size = std::distance(from_it, to_it);
        const ssize_t search_seq_size = std::distance(search_from_it, search_to_it);
        size_t result = 0;

        if (search_seq_size > source_seq_size)
            return 0;

        const T1 max_it = from_it + (source_seq_size - search_seq_size);
        for (T1 it = from_it; it <= max_it; ++it)
        {
            if (std::equal(it, it + search_seq_size, search_from_it))
            {
                ++result;
                it += search_seq_size;
            }
        }

        return result;
    }

    template <typename T>
    size_t countSubSequences(const T &val1, const T &val2)
    {
        return countSubSequences(val1.begin(), val1.end(), val2.begin(), val2.end());
    }

    std::string trim(const std::string &data, const char delim, const bool at_start = true,
                     const bool at_end = true);
    std::string trim(const std::string &data, const bool at_start = true, const bool at_end = true);
}
