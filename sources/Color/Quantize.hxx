#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <execution>
#include <limits>
#include <map>
#include <vector>
#include "Common/Concepts.hxx"
#include "Common/Tools.hxx"

namespace Color::Quantize
{
    namespace Detail
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

    template <Common::Concept::Class RGBA>
    bool middleCut(const int64_t width, const int64_t height, const int64_t colors,
                   const std::vector<RGBA> &in_pixels, std::vector<RGBA> &out_pixels,
                   std::vector<RGBA> &palette)
    {
        using VType = decltype(RGBA::value);
        using CType = decltype(RGBA::r);

        if ((width < 1) || (height < 1))
            return false;

        if (in_pixels.size() != static_cast<size_t>(width * height))
            return false;

        if (in_pixels.size() < static_cast<size_t>(colors))
            return false;

        std::vector<RGBA> tmp;
        std::map<VType,size_t> tmp2;
        std::vector<std::vector<RGBA>> lists;
        size_t list_size = 0;
        CType lower_red = std::numeric_limits<CType>::max();
        CType lower_green = std::numeric_limits<CType>::max();
        CType lower_blue = std::numeric_limits<CType>::max();
        CType upper_red = 0;
        CType upper_green = 0;
        CType upper_blue = 0;
        CType red = 0;
        CType green = 0;
        CType blue = 0;
        CType max = 0;

        for (auto &pixel : in_pixels)
        {
            lower_red = std::min(lower_red, pixel.r);
            lower_green = std::min(lower_green, pixel.g);
            lower_blue = std::min(lower_blue, pixel.b);
            upper_red = std::max(upper_red, pixel.r);
            upper_green = std::max(upper_green, pixel.g);
            upper_blue = std::max(upper_blue, pixel.b);
            ++tmp2[pixel.value];
        }
        tmp.reserve(tmp2.size());
        for (auto &pair : tmp2)
            tmp.push_back(pair.first);

        red = upper_red - lower_red;
        green = upper_green - lower_green;
        blue = upper_blue - lower_blue;
        max = Common::Tools::max(red, green, blue);

        std::sort(std::execution::par_unseq, tmp.begin(), tmp.end(),
        [&](const RGBA &c1, const RGBA &c2)
        {
            if (max == red)
                return c1.r < c2.r;
            else if (max == green)
                return c1.g < c2.g;
            else
                return c1.b < c2.b;
        });

        list_size = tmp.size() / colors;
        for (size_t i = 0; i < static_cast<size_t>(colors); ++i)
        {
            std::vector<RGBA> list(tmp.begin() + (list_size * i),
                                   tmp.begin() + (list_size * i) + list_size);
            lists.push_back(list);
        }

        palette.reserve(colors);
        for (auto &list : lists)
            palette.push_back(list[list.size() / 2]);

        out_pixels.resize(width * height);
        for (size_t i = 0; i < in_pixels.size(); ++i)
            out_pixels[i] = palette[Detail::closestMatch(in_pixels[i], palette)];

        return true;
    }
}
