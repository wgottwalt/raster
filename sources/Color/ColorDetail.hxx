#pragma once

#include <cmath>
#include <limits>
#include "Common/Concepts.hxx"

namespace Color::Detail
{
    template <Common::Concept::Class RGBA>
    inline RGBA rgbaDistance(const RGBA &val1, const RGBA &val2)
    {
        using VType = decltype(RGBA::value);

        return {static_cast<VType>(std::abs(val1.r - val2.r)) +
                static_cast<VType>(std::abs(val1.g - val2.g)) +
                static_cast<VType>(std::abs(val1.b - val2.b)) +
                static_cast<VType>(std::abs(val1.a - val2.a))};
    }

    template <Common::Concept::Class RGBA>
    size_t closestMatch(const RGBA &pixel, const std::vector<RGBA> &palette)
    {
        using VType = decltype(RGBA::value);

        size_t index = 0;
        VType current_distance = std::numeric_limits<size_t>::max();
        VType distance = 0;

        for (size_t i = 0; i < palette.size(); ++i)
        {
            distance = rgbaDistance(pixel, palette[i]).value;
            if (distance < current_distance)
            {
                current_distance = distance;
                index = i;
            }
        }

        return index;
    }
}
