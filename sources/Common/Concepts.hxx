#pragma once

#include <concepts>
#include <type_traits>

namespace Common::Concept
{
    template <typename T>
    concept Integer = std::is_integral_v<T>;

    template <typename T>
    concept SignedInteger = std::is_integral_v<T> && std::is_signed_v<T>;

    template <typename T>
    concept UnsignedInteger = std::is_integral_v<T> && std::is_unsigned_v<T>;

    template <typename T>
    concept Float = std::is_floating_point_v<T>;

    template <typename T>
    concept Number = std::is_integral_v<T> || std::is_floating_point_v<T>;

    template <typename T>
    concept Class = std::is_class_v<T>;
}
