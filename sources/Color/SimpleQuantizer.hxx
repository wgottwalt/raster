#pragma once

#include <limits>
#include "Common/Concepts.hxx"
#include "ColorDetail.hxx"

namespace Color::Quantize
{
    template <Common::Concept::Class RGBA>
    bool simple(const int64_t width, const int64_t height, const int64_t colors,
                const std::vector<RGBA> &in_pixels, std::vector<RGBA> &out_pixels,
                std::vector<RGBA> &palette)
    {
        using CType = decltype(RGBA::r);

        if ((width < 1) || (height < 1))
            return false;

        if (in_pixels.size() != static_cast<size_t>(width * height))
            return false;

        if (in_pixels.size() <= static_cast<size_t>(colors))
            return true;

        const CType Max = std::numeric_limits<CType>::max();

        palette.reserve(colors);
        for (int64_t i = 0; i < (colors >> 2); ++i)
        {
            const auto val = Max / (colors >> 2) * i;

            palette.push_back({val, val, val, Max});
            palette.push_back({val, 0, 0, Max});
            palette.push_back({0, val, 0, Max});
            palette.push_back({0, 0, val, Max});
        }

        out_pixels.resize(width * height);
        for (size_t i = 0; i < in_pixels.size(); ++i)
            out_pixels[i] = palette[Color::Detail::closestMatch(in_pixels[i], palette)];

        return true;
    }
}
