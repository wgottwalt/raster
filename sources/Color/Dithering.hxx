#pragma once

#include <cstdint>
#include <limits>
#include <vector>
#include "Common/Concepts.hxx"
#include "Common/Tools.hxx"
#include "ColorDetail.hxx"

namespace Color::Dithering
{
    namespace T = Common::Tools;

    enum class Algorithm : uint8_t {
        Burkes,
        FloydSteinberg
    };

    template <Concept::RGBA Color>
    std::vector<Color> apply(const std::vector<Color> &pixels, const std::vector<Color> &palette,
                             const int64_t width, const int64_t height, const Algorithm alg)
    {
        const uint64_t psize = palette.size();
        const int32_t Max = std::numeric_limits<decltype(Color::r)>::max();
        std::vector<Color> tpxls;

        if (psize > 1)
            tpxls = pixels;
        else
            return tpxls;

        switch (alg)
        {
            case Algorithm::Burkes:
            {
                for (int64_t y = 0; y < height; ++y)
                {
                    for (int64_t x = 0; x < width; ++x)
                    {
                        const auto op = tpxls[y * width + x];
                        const auto np = palette[Detail::closestMatch(op, palette)];
                        const int32_t er = op.r - np.r;
                        const int32_t eg = op.g - np.g;
                        const int32_t eb = op.b - np.b;

                        tpxls[y * width + x] = np;
                        if (x < (width - 1))
                        {
                            auto &p = tpxls[y * width + x + 1];
                            p.r = T::clamp(static_cast<int32_t>(p.r) + ((er << 3) >> 5), 0, Max);
                            p.g = T::clamp(static_cast<int32_t>(p.g) + ((eg << 3) >> 5), 0, Max);
                            p.b = T::clamp(static_cast<int32_t>(p.b) + ((eb << 3) >> 5), 0, Max);
                        }
                        if (x < (width - 2))
                        {
                            auto &p = tpxls[y * width + x + 2];
                            p.r = T::clamp(static_cast<int32_t>(p.r) + ((er << 2) >> 5), 0, Max);
                            p.g = T::clamp(static_cast<int32_t>(p.g) + ((eg << 2) >> 5), 0, Max);
                            p.b = T::clamp(static_cast<int32_t>(p.b) + ((eb << 2) >> 5), 0, Max);
                        }
                        if (y < (height - 1))
                        {
                            const size_t pos = (y + 1) * width + x;
                            auto &tp = tpxls[pos];

                            if (x > 2)
                            {
                                auto &p = tpxls[pos - 2];
                                p.r = T::clamp(static_cast<int32_t>(p.r) + ((er << 1) >> 5), 0, Max);
                                p.g = T::clamp(static_cast<int32_t>(p.g) + ((eg << 1) >> 5), 0, Max);
                                p.b = T::clamp(static_cast<int32_t>(p.b) + ((eb << 1) >> 5), 0, Max);
                            }
                            if (x > 1)
                            {
                                auto &p = tpxls[pos - 1];
                                p.r = T::clamp(static_cast<int32_t>(p.r) + ((er << 2) >> 5), 0, Max);
                                p.g = T::clamp(static_cast<int32_t>(p.g) + ((eg << 2) >> 5), 0, Max);
                                p.b = T::clamp(static_cast<int32_t>(p.b) + ((eb << 2) >> 5), 0, Max);
                            }

                            tp.r = T::clamp(static_cast<int32_t>(tp.r) + ((er << 3) >> 5), 0, Max);
                            tp.g = T::clamp(static_cast<int32_t>(tp.g) + ((eg << 3) >> 5), 0, Max);
                            tp.b = T::clamp(static_cast<int32_t>(tp.b) + ((eb << 3) >> 5), 0, Max);

                            if (x < (width - 1))
                            {
                                auto &p = tpxls[pos + 1];
                                p.r = T::clamp(static_cast<int32_t>(p.r) + ((er << 2) >> 5), 0, Max);
                                p.g = T::clamp(static_cast<int32_t>(p.g) + ((eg << 2) >> 5), 0, Max);
                                p.b = T::clamp(static_cast<int32_t>(p.b) + ((eb << 2) >> 5), 0, Max);
                            }
                            if (x < (width - 2))
                            {
                                auto &p = tpxls[pos + 2];
                                p.r = T::clamp(static_cast<int32_t>(p.r) + ((er << 1) >> 5), 0, Max);
                                p.g = T::clamp(static_cast<int32_t>(p.g) + ((eg << 1) >> 5), 0, Max);
                                p.b = T::clamp(static_cast<int32_t>(p.b) + ((eb << 1) >> 5), 0, Max);
                            }
                        }
                    }
                }
                break;
            }

            case Algorithm::FloydSteinberg:
            {
                for (int64_t y = 0; y < height; ++y)
                {
                    for (int64_t x = 0; x < width; ++x)
                    {
                        const auto &op = tpxls[y * width + x];
                        const auto np = palette[Detail::closestMatch(op, palette)];
                        const int32_t er = op.r - np.r;
                        const int32_t eg = op.g - np.g;
                        const int32_t eb = op.b - np.b;

                        tpxls[y * width + x] = np;
                        if (x < (width - 1))
                        {
                            auto &p = tpxls[y * width + x + 1];
                            p.r = T::clamp(static_cast<int32_t>(p.r) + er * 7 / 16, 0, Max);
                            p.g = T::clamp(static_cast<int32_t>(p.g) + er * 7 / 16, 0, Max);
                            p.g = T::clamp(static_cast<int32_t>(p.b) + er * 7 / 16, 0, Max);
                        }
                        if (y < (height - 1))
                        {
                            const size_t pos = (y + 1) * width + x;
                            auto &tp = tpxls[pos];

                            if (x > 0)
                            {
                                auto &p = tpxls[pos - 1];
                                p.r = T::clamp(static_cast<int32_t>(p.r) + er * 3 / 16, 0, Max);
                                p.g = T::clamp(static_cast<int32_t>(p.g) + eg * 3 / 16, 0, Max);
                                p.b = T::clamp(static_cast<int32_t>(p.b) + eb * 3 / 16, 0, Max);
                            }

                            tp.r = T::clamp(static_cast<int32_t>(tp.r) + er * 5 / 16, 0, Max);
                            tp.g = T::clamp(static_cast<int32_t>(tp.g) + eg * 5 / 16, 0, Max);
                            tp.b = T::clamp(static_cast<int32_t>(tp.b) + eb * 5 / 16, 0, Max);

                            if (x < (width - 1))
                            {
                                auto &p = tpxls[pos + 1];
                                p.r = T::clamp(static_cast<int32_t>(p.r) + er / 16, 0, Max);
                                p.g = T::clamp(static_cast<int32_t>(p.g) + eg / 16, 0, Max);
                                p.b = T::clamp(static_cast<int32_t>(p.b) + eb / 16, 0, Max);
                            }
                        }
                    }
                }
            }
        }

        return tpxls;
    }
}
