#pragma once

#include <cmath>
#include <string>
#include <utility>
#include "Common/Concepts.hxx"

namespace Math
{
    template <Common::Concept::Number N>
    class Vector4 {
    public:
        //--- public attributes ---
        union {
            N data[4];
            struct {
                N x, y, z, w;
            };
        };

        //--- public constructors ---
        Vector4() noexcept
        : data{0, 0, 0, 0}
        {
        }

        Vector4(const N val) noexcept
        : data{val, val, val, val}
        {
        }

        Vector4(const N xx, const N yy, const N zz, const N ww) noexcept
        : data{xx, yy, zz, ww}
        {
        }

        Vector4(const Vector4 &rhs) noexcept
        : data{rhs.x, rhs.y, rhs.z, rhs.w}
        {
        }

        Vector4(Vector4 &&rhs) noexcept
        : data{std::move(rhs.x), std::move(rhs.y), std::move(rhs.z), std::move(rhs.w)}
        {
        }

        ~Vector4() noexcept
        {
        }

        //--- public operators ---
        Vector4 &operator=(const Vector4 &rhs) noexcept
        {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            w = rhs.w;

            return *this;
        }

        Vector4 &operator=(Vector4 &&rhs) noexcept
        {
            x = std::move(rhs.x);
            y = std::move(rhs.y);
            z = std::move(rhs.z);
            w = std::move(rhs.w);

            return *this;
        }

        bool operator==(const Vector4 &rhs) const noexcept
        {
            return x == rhs.x &&
                   y == rhs.y &&
                   z == rhs.z &&
                   w == rhs.w;
        }

        bool operator!=(const Vector4 &rhs) const noexcept
        {
            return !(*this == rhs);
        }

        //---
        Vector4 &operator+=(const Vector4 &vec) noexcept
        {
            x += vec.x;
            y += vec.y;
            z += vec.z;
            w += vec.w;

            return *this;
        }

        template <Common::Concept::Number TN>
        Vector4 &operator+=(const TN val) noexcept
        {
            x += val;
            y += val;
            z += val;
            w += val;

            return *this;
        }

        Vector4 &operator-=(const Vector4 &vec) noexcept
        {
            x -= vec.x;
            y -= vec.y;
            z -= vec.z;
            w -= vec.w;

            return *this;
        }

        template <Common::Concept::Number TN>
        Vector4 &operator-=(const TN val) noexcept
        {
            x -= val;
            y -= val;
            z -= val;
            w -= val;

            return *this;
        }

        Vector4 &operator*=(const Vector4 &vec) noexcept
        {
            x *= vec.x;
            y *= vec.y;
            z *= vec.z;
            w *= vec.w;

            return *this;
        }

        template <Common::Concept::Number TN>
        Vector4 &operator*=(const TN val) noexcept
        {
            x *= val;
            y *= val;
            z *= val;
            w *= val;

            return *this;
        }

        Vector4 &operator/=(const Vector4 &vec) noexcept
        {
            x /= vec.x;
            y /= vec.y;
            z /= vec.z;
            w /= vec.w;

            return *this;
        }

        template <Common::Concept::Number TN>
        Vector4 &operator/=(const TN val) noexcept
        {
            x /= val;
            y /= val;
            z /= val;
            w /= val;

            return *this;
        }

        //--- public methods ---
        bool isZero() const noexcept
        {
            return x == 0 &&
                   y == 0 &&
                   z == 0 &&
                   w == 0;
        }

        N length() const noexcept
        {
            return std::sqrt(x * x + y * y + z * z + w * w);
        }

        N lengthSquared() const noexcept
        {
            return x * x + y * y + z * z + w * w;
        }

        Vector4 normalized() const noexcept
        {
            const N length = std::sqrt(x * x + y * y + z * z + w * w);

            return {x / length, y / length, z / length, w / length};
        }

        void normalize() noexcept
        {
            const N length = std::sqrt(x * x + y * y + z * z + w * w);

            x = x / length;
            y = y / length;
            z = z / length;
            w = w / length;
        }

        N dotProduct(const Vector4 &vec) const noexcept
        {
            return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
        }

        std::string str() const
        {
            return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " +
                   std::to_string(w);
        }
    };

    //--- global operators ---
    template <Common::Concept::Number N>
    Vector4<N> operator+(const Vector4<N> &vec1, const Vector4<N> &vec2) noexcept
    {
        return {vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z, vec1.w + vec2.z};
    }

    template <Common::Concept::Number N, Common::Concept::Number TN>
    Vector4<N> operator+(const Vector4<N> &vec, const TN val) noexcept
    {
        return {vec.x + val, vec.y + val, vec.z + val, vec.w + val};
    }

    template <Common::Concept::Number N>
    Vector4<N> operator-(const Vector4<N> &vec1, const Vector4<N> &vec2) noexcept
    {
        return {vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z, vec1.w - vec2.w};
    }

    template <Common::Concept::Number N, Common::Concept::Number TN>
    Vector4<N> operator-(const Vector4<N> &vec, const TN val) noexcept
    {
        return {vec.x - val, vec.y - val, vec.z - val, vec.w - val};
    }

    template <Common::Concept::Number N>
    Vector4<N> operator*(const Vector4<N> &vec1, const Vector4<N> &vec2) noexcept
    {
        return {vec1.x * vec2.x, vec1.y * vec2.y, vec1.z * vec2.z, vec1.w * vec2.w};
    }

    template <Common::Concept::Number N, Common::Concept::Number TN>
    Vector4<N> operator*(const Vector4<N> &vec, const TN val) noexcept
    {
        return {vec.x * val, vec.y * val, vec.z * val, vec.w * val};
    }

    template <Common::Concept::Number N>
    Vector4<N> operator/(const Vector4<N> &vec1, const Vector4<N> &vec2) noexcept
    {
        return {vec1.x / vec2.x, vec1.y / vec2.y, vec1.z / vec2.z, vec1.w / vec2.w};
    }

    template <Common::Concept::Number N, Common::Concept::Number TN>
    Vector4<N> operator/(const Vector4<N> &vec, const TN val) noexcept
    {
        return {vec.x / val, vec.y / val, vec.z / val};
    }

    //--- global functions ---
    template <Common::Concept::Number N>
    N dotProduct(const Vector4<N> &vec1, const Vector4<N> &vec2) noexcept
    {
        return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z + vec1.w * vec2.w;
    }
}
