#pragma once

#include <algorithm>
#include <cmath>
#include <limits>
#include <map>
#include <vector>
#include "Common/Concepts.hxx"
#include "Common/Tools.hxx"
#include "ColorDetail.hxx"

namespace Color::Quantize
{
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

        if (in_pixels.size() <= static_cast<size_t>(colors))
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

        std::sort(tmp.begin(), tmp.end(),
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
            out_pixels[i] = palette[Color::Detail::closestMatch(in_pixels[i], palette)];

        return true;
    }
}
