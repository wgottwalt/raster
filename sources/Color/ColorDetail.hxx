#pragma once

#include <cmath>
#include <limits>
#include "Common/Concepts.hxx"

namespace Color
{
    class RGBA8888;
    class RGBA16161616;

    namespace Concept
    {
        template <typename T>
        concept RGBA = std::is_same<T,RGBA8888>::value || std::is_same<T,RGBA16161616>::value;
    }
}

namespace Color::Detail
{
    template <Concept::RGBA Color>
    inline Color rgbaDistance(const Color &val1, const Color &val2)
    {
        using VType = decltype(Color::value);

        return {static_cast<VType>(std::abs(val1.r - val2.r)) +
                static_cast<VType>(std::abs(val1.g - val2.g)) +
                static_cast<VType>(std::abs(val1.b - val2.b)) +
                static_cast<VType>(std::abs(val1.a - val2.a))};
    }

    template <Concept::RGBA Color>
    size_t closestMatch(const Color &pixel, const std::vector<Color> &palette)
    {
        using VType = decltype(Color::value);

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
