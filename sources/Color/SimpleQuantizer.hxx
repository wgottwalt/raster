#pragma once

#include <cmath>
#include <limits>
#include "Common/Concepts.hxx"
#include "ColorDetail.hxx"

namespace Color::Quantize
{
    template <Common::Concept::Class RGBA>
    bool simple(const int64_t width, const int64_t height, const int64_t colors,
                const std::vector<RGBA> &in_pixels, std::vector<RGBA> &out_pixels,
                std::vector<RGBA> &palette) noexcept(false)
    {
        using CType = decltype(RGBA::r);

        if ((width < 1) || (height < 1))
            return false;

        if (in_pixels.size() != static_cast<size_t>(width * height))
            return false;

        if (in_pixels.size() <= static_cast<size_t>(colors))
            return true;

        const CType Max = std::numeric_limits<CType>::max();
        const CType ChanMax = std::cbrt(colors);

        palette.reserve(colors);
        for (CType ir = 0; ir < ChanMax; ++ir)
        {
            const CType r = Max / ChanMax * ir;

            for (CType ig = 0; ig < ChanMax; ++ig)
            {
                const CType g = Max / ChanMax * ig;

                for (CType ib = 0; ib < ChanMax; ++ib)
                {
                    const CType b = Max / ChanMax * ib;

                    palette.push_back({r, g, b, Max});
                }
            }
        }
        palette.resize(colors);

        out_pixels.resize(width * height);
        for (size_t i = 0; i < in_pixels.size(); ++i)
            out_pixels[i] = palette[Color::Detail::closestMatch(in_pixels[i], palette)];

        return true;
    }
}
