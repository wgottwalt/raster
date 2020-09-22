#pragma once

#include <limits>
#include <utility>
#include "Common/Concepts.hxx"
#include "ColorDetail.hxx"
#include "RGBA8888.hxx"
#include "RGBA16161616.hxx"

namespace Color
{
    template <Common::Concept::Number N = int32_t>
    class QRGBA {
    public:
        //--- public properties ---
        N r, g, b, a;

        //--- public constructors ---
        QRGBA() noexcept
        : r(0), g(0), b(0), a(0)
        {
        }

        QRGBA(const N val) noexcept
        : r(val), g(val), b(val), a(val)
        {
        }

        QRGBA(const N rr, const N gg, const N bb, const N aa) noexcept
        : r(rr), g(gg), b(bb), a(aa)
        {
        }

        QRGBA(const QRGBA &rhs) noexcept
        : r(rhs.r), g(rhs.g), b(rhs.b), a(rhs.a)
        {
        }

        QRGBA(QRGBA &&rhs) noexcept
        : r(std::move(rhs.r)), g(std::move(rhs.g)), b(std::move(rhs.b)), a(std::move(rhs.a))
        {
        }

        template <Color::Concept::RGBA Color>
        QRGBA(const Color &rhs) noexcept
        : r(rhs.r), g(rhs.g), b(rhs.b), a(rhs.a)
        {
        }

        template <Color::Concept::RGBA Color>
        QRGBA(Color &&rhs) noexcept
        : r(std::move(rhs.r)), g(std::move(rhs.g)), b(std::move(rhs.b)), a(std::move(rhs.a))
        {
        }

        ~QRGBA() noexcept
        {
        }

        //--- public constructors ---
        QRGBA &operator=(const QRGBA &rhs) noexcept
        {
            if (this != &rhs)
            {
                r = rhs.r;
                g = rhs.g;
                b = rhs.b;
                a = rhs.a;
            }

            return *this;
        }

        QRGBA &operator=(QRGBA &&rhs) noexcept
        {
            if (this != &rhs)
            {
                r = std::move(rhs.r);
                g = std::move(rhs.g);
                b = std::move(rhs.b);
                a = std::move(rhs.a);
            }

            return *this;
        }

        template <Color::Concept::RGBA Color>
        QRGBA &operator=(const Color &rhs) noexcept
        {
            if (this != &rhs)
            {
                r = rhs.r;
                g = rhs.g;
                b = rhs.b;
                a = rhs.a;
            }

            return *this;
        }

        template <Color::Concept::RGBA Color>
        QRGBA &operator=(Color &&rhs) noexcept
        {
            if (this != &rhs)
            {
                r = std::move(rhs.r);
                g = std::move(rhs.g);
                b = std::move(rhs.b);
                a = std::move(rhs.a);
            }

            return *this;
        }

        bool operator==(const QRGBA &rhs) const noexcept
        {
            return r == rhs.r &&
                   g == rhs.g &&
                   b == rhs.b &&
                   a == rhs.a;
        }

        bool operator!=(const QRGBA &rhs) const noexcept
        {
            return !(*this == rhs);
        }

        template <Color::Concept::RGBA Color>
        bool operator==(const Color &rhs) const noexcept
        {
            return r == static_cast<N>(rhs.r) &&
                   g == static_cast<N>(rhs.g) &&
                   b == static_cast<N>(rhs.b) &&
                   a == static_cast<N>(rhs.a);
        }

        template <Color::Concept::RGBA Color>
        bool operator!=(const Color &rhs) const noexcept
        {
            return !(*this == rhs);
        }

        //---
        QRGBA &operator+=(const QRGBA &rhs) noexcept
        {
            r += rhs.r;
            g += rhs.g;
            b += rhs.b;
            a += rhs.a;

            return *this;
        }

        template <Color::Concept::RGBA Color>
        QRGBA &operator+=(const Color &rhs) noexcept
        {
            r += rhs.r;
            g += rhs.g;
            b += rhs.b;
            a += rhs.a;

            return *this;
        }

        template <Common::Concept::Number U>
        QRGBA &operator+=(const U val) noexcept
        {
            r += val;
            g += val;
            b += val;
            a += val;

            return *this;
        }

        QRGBA &operator-=(const QRGBA &rhs) noexcept
        {
            r -= rhs.r;
            g -= rhs.g;
            b -= rhs.b;
            a -= rhs.a;

            return *this;
        }

        template <Color::Concept::RGBA Color>
        QRGBA &operator-=(const Color &rhs) noexcept
        {
            r -= rhs.r;
            g -= rhs.g;
            b -= rhs.b;
            a -= rhs.a;

            return *this;
        }

        template <Common::Concept::Number U>
        QRGBA &operator-=(const U val) noexcept
        {
            r -= val;
            g -= val;
            b -= val;
            a -= val;

            return *this;
        }

        QRGBA &operator*=(const QRGBA &rhs) noexcept
        {
            r *= rhs.r;
            g *= rhs.g;
            b *= rhs.b;
            a *= rhs.a;

            return *this;
        }

        template <Color::Concept::RGBA Color>
        QRGBA &operator*=(const Color &rhs) noexcept
        {
            r *= rhs.r;
            g *= rhs.g;
            b *= rhs.b;
            a *= rhs.a;

            return *this;
        }

        template <Common::Concept::Number U>
        QRGBA &operator*=(const U val) noexcept
        {
            r *= val;
            g *= val;
            b *= val;
            a *= val;

            return *this;
        }

        QRGBA &operator/=(const QRGBA &rhs) noexcept
        {
            r /= rhs.r;
            g /= rhs.g;
            b /= rhs.b;
            a /= rhs.a;

            return *this;
        }

        template <Color::Concept::RGBA Color>
        QRGBA &operator/=(const Color &rhs) noexcept
        {
            r /= rhs.r;
            g /= rhs.g;
            b /= rhs.b;
            a /= rhs.a;

            return *this;
        }

        template <Common::Concept::Number U>
        QRGBA &operator/=(const U val) noexcept
        {
            r /= val;
            g /= val;
            b /= val;
            a /= val;

            return *this;
        }

        //--- public methods ---
        N averageRGB() const noexcept
        {
            return (r + g + b) / 3;
        }

        N averageRGBA() const noexcept
        {
            return (r + g + b + a) / 4;
        }

        N grey() const noexcept
        {
            return 0.299083 * r + 0.585841 * g + 0.114076 * b;
        }

        template <Color::Concept::RGBA Color>
        Color rgba() const noexcept
        {
            return *this >> ((sizeof (Color::r) - 1) * 8);
        }
    };

    //--- global public operators ---
    template <Common::Concept::Number N>
    QRGBA<N> operator+(const QRGBA<N> &c0, const QRGBA<N> &c1) noexcept
    {
        return {c0.r + c1.r, c0.g + c1.g, c0.b + c1.b, c0.a + c1.a};
    }

    template <Common::Concept::Number N, Color::Concept::RGBA Color>
    QRGBA<N> operator+(const QRGBA<N> &c0, const Color &c1) noexcept
    {
        return {c0.r + c1.r, c0.g + c1.g, c0.b + c1.b, c0.a + c1.a};
    }

    template <Common::Concept::Number N, Common::Concept::Number U>
    QRGBA<N> operator+(const QRGBA<N> &c, const U val) noexcept
    {
        return {c.r + val, c.g + val, c.b + val, c.a + val};
    }

    template <Common::Concept::Number N>
    QRGBA<N> operator-(const QRGBA<N> &c0, const QRGBA<N> &c1) noexcept
    {
        return {c0.r - c1.r, c0.g - c1.g, c0.b - c1.b, c0.a - c1.a};
    }

    template <Common::Concept::Number N, Color::Concept::RGBA Color>
    QRGBA<N> operator-(const QRGBA<N> &c0, const Color &c1) noexcept
    {
        return {c0.r - c1.r, c0.g - c1.g, c0.b - c1.b, c0.a - c1.a};
    }

    template <Common::Concept::Number N, Common::Concept::Number U>
    QRGBA<N> operator-(const QRGBA<N> &c, const U val) noexcept
    {
        return {c.r - val, c.g - val, c.b - val, c.a - val};
    }

    template <Common::Concept::Number N>
    QRGBA<N> operator*(const QRGBA<N> &c0, const QRGBA<N> &c1) noexcept
    {
        return {c0.r * c1.r, c0.g * c1.g, c0.b * c1.b, c0.a * c1.a};
    }

    template <Common::Concept::Number N, Color::Concept::RGBA Color>
    QRGBA<N> operator*(const QRGBA<N> &c0, const Color &c1) noexcept
    {
        return {c0.r * c1.r, c0.g * c1.g, c0.b * c1.b, c0.a * c1.a};
    }

    template <Common::Concept::Number N, Common::Concept::Number U>
    QRGBA<N> operator*(const QRGBA<N> &c, const U val) noexcept
    {
        return {c.r * val, c.g * val, c.b * val, c.a * val};
    }

    template <Common::Concept::Number N>
    QRGBA<N> operator/(const QRGBA<N> &c0, const QRGBA<N> &c1) noexcept
    {
        return {c0.r / c1.r, c0.g / c1.g, c0.b / c1.b, c0.a / c1.a};
    }

    template <Common::Concept::Number N, Color::Concept::RGBA Color>
    QRGBA<N> operator/(const QRGBA<N> &c0, const Color &c1) noexcept
    {
        return {c0.r / c1.r, c0.g / c1.g, c0.b / c1.b, c0.a / c1.a};
    }

    template <Common::Concept::Number N, Common::Concept::Number U>
    QRGBA<N> operator/(const QRGBA<N> &c, const U val) noexcept
    {
        return {c.r / val, c.g / val, c.b / val, c.a / val};
    }
}
