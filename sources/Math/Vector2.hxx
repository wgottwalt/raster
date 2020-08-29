#pragma once

#include <cmath>
#include <string>
#include <utility>
#include "Common/Concepts.hxx"

namespace Math
{
    template <Common::Concept::Number N>
    class Vector2 {
    public:
        //--- attributes ---
        union {
            N data[2];
            struct {
                N x, y;
            };
        };

        //--- public constructors ---
        Vector2() noexcept
        : data{0, 0}
        {
        }

        Vector2(const N val) noexcept
        : data{val, val}
        {
        }

        Vector2(const N xx, const N yy) noexcept
        : data{xx, yy}
        {
        }

        Vector2(const Vector2 &rhs) noexcept
        : data{rhs.x, rhs.y}
        {
        }

        Vector2(Vector2 &&rhs) noexcept
        : x(std::move(rhs.x)), y(std::move(rhs.y))
        {
        }

        ~Vector2() noexcept
        {
        }

        //--- public operators ---
        Vector2 &operator=(const Vector2 &rhs) noexcept
        {
            x = rhs.x;
            y = rhs.y;

            return *this;
        }

        Vector2 &operator=(Vector2 &&rhs) noexcept
        {
            x = std::move(rhs.x);
            y = std::move(rhs.y);

            return *this;
        }

        bool operator==(const Vector2 &rhs) const noexcept
        {
            return x == rhs.x &&
                   y == rhs.y;
        }

        bool operator!=(const Vector2 &rhs) const noexcept
        {
            return !(*this == rhs);
        }

        //---
        Vector2 &operator-() noexcept
        {
            x = -x;
            y = -y;

            return *this;
        }

        Vector2 operator-() const noexcept
        {
            return {-x, -y};
        }

        Vector2 &operator+=(const Vector2 &rhs) noexcept
        {
            x += rhs.x;
            y += rhs.y;

            return *this;
        }

        template <Common::Concept::Number TN>
        Vector2 &operator+=(const TN val) noexcept
        {
            x += val;
            y += val;

            return *this;
        }

        Vector2 &operator-=(const Vector2 &rhs) noexcept
        {
            x -= rhs.x;
            y -= rhs.y;

            return *this;
        }

        template <Common::Concept::Number TN>
        Vector2 &operator-=(const TN val) noexcept
        {
            x -= val;
            y -= val;

            return *this;
        }

        Vector2 &operator*=(const Vector2 &rhs) noexcept
        {
            x *= rhs.x;
            y *= rhs.y;

            return *this;
        }

        template <Common::Concept::Number TN>
        Vector2 &operator*=(const TN val) noexcept
        {
            x *= val;
            y *= val;

            return *this;
        }

        Vector2 &operator/=(const Vector2 &rhs) noexcept
        {
            x /= rhs.x;
            y /= rhs.y;

            return *this;
        }

        template <Common::Concept::Number TN>
        Vector2 &operator/=(const TN val) noexcept
        {
            x /= val;
            y /= val;

            return *this;
        }

        //--- public methods ---
        bool isZero() const noexcept
        {
            return x == 0 &&
                   y == 0;
        }

        N length() const noexcept
        {
            return std::sqrt(x * x + y * y);
        }

        N lengthSquared() const noexcept
        {
            return x * x + y * y;
        }

        Vector2 normalized() const noexcept
        {
            const N length = std::sqrt(x * x + y * y);

            return {x / length, y / length};
        }

        void normalize() noexcept
        {
            const N length = std::sqrt(x * x + y * y);

            x = x / length;
            y = y / length;
        }

        N distanceToPoint(const Vector2 &vec) const noexcept
        {
            return (*this - vec).length();
        }

        N distanceToLine(const Vector2 &origin, const Vector2 &direction) const noexcept
        {
            if (direction.isZero())
                return (*this - origin).length();
            else
            {
                const Vector2 tmp = origin + dotProduct(*this - origin, direction) * direction;

                return (*this - tmp).length();
            }
        }

        N dotProduct(const Vector2 &vec) const noexcept
        {
            return x * vec.x + y * vec.y;
        }

        N crossProduct(const Vector2 &vec) const
        {
            return (x * vec.y) - (y * vec.x);
        }

        std::string str() const
        {
            return std::to_string(x) + ", " + std::to_string(y);
        }
    };

    //--- global operators ---
    template <Common::Concept::Number N>
    inline Vector2<N> operator+(const Vector2<N> &vec1, const Vector2<N> &vec2) noexcept
    {
        return {vec1.x + vec2.x, vec1.y + vec2.y};
    }

    template <Common::Concept::Number N, Common::Concept::Number TN>
    inline Vector2<N> operator+(const Vector2<N> &vec, const TN val) noexcept
    {
        return {vec.x + val, vec.y + val};
    }

    template <Common::Concept::Number N>
    inline Vector2<N> operator-(const Vector2<N> &vec1, const Vector2<N> &vec2) noexcept
    {
        return {vec1.x - vec2.x, vec1.y - vec2.y};
    }

    template <Common::Concept::Number N, Common::Concept::Number TN>
    inline Vector2<N> operator-(const Vector2<N> &vec, const TN val) noexcept
    {
        return {vec.x - val, vec.y - val};
    }

    template <Common::Concept::Number N>
    inline Vector2<N> operator*(const Vector2<N> &vec1, const Vector2<N> &vec2) noexcept
    {
        return {vec1.x * vec2.x, vec1.y * vec2.y};
    }

    template <Common::Concept::Number N, Common::Concept::Number TN>
    inline Vector2<N> operator*(const Vector2<N> &vec, const TN val) noexcept
    {
        return {vec.x * val, vec.y * val};
    }

    template <Common::Concept::Number N>
    inline Vector2<N> operator/(const Vector2<N> &vec1, const Vector2<N> &vec2) noexcept
    {
        return {vec1.x / vec2.x, vec1.y / vec2.y};
    }

    template <Common::Concept::Number N, Common::Concept::Number TN>
    inline Vector2<N> operator/(const Vector2<N> &vec, const TN val) noexcept
    {
        return {vec.x / val, vec.y / val};
    }

    //--- global functions ---
    template <Common::Concept::Number N>
    inline N dotProduct(const Vector2<N> &vec1, const Vector2<N> &vec2) noexcept
    {
        return vec1.x * vec2.x + vec1.y * vec2.y;
    }

    template <Common::Concept::Number N>
    inline N crossProduct(const Vector2<N> &vec1, const Vector2<N> &vec2) noexcept
    {
        return (vec1.x * vec2.y) - (vec1.y * vec2.x);
    }
}
