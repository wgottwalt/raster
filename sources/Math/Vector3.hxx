#pragma once

#include <cmath>
#include <string>
#include <utility>
#include "Common/Concepts.hxx"

namespace Math
{
    template <Common::Concept::Number N>
    class Vector3 {
    public:
        //--- public attributes ---
        union {
            N data[3];
            struct {
                N x, y, z;
            };
        };

        //--- public constructors ---
        Vector3() noexcept
        : data{0, 0, 0}
        {
        }

        Vector3(const N val) noexcept
        : data{val, val, val}
        {
        }

        Vector3(const N xx, const N yy, const N zz) noexcept
        : data{xx, yy, zz}
        {
        }

        Vector3(const Vector3 &rhs) noexcept
        : data{rhs.x, rhs.y, rhs.z}
        {
        }

        Vector3(Vector3 &&rhs) noexcept
        : data{std::move(rhs.x), std::move(rhs.y), std::move(rhs.z)}
        {
        }

        ~Vector3()
        {
        }

        //--- public operators ---
        Vector3 &operator=(const Vector3 &rhs) noexcept
        {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;

            return *this;
        }

        Vector3 &operator=(Vector3 &&rhs) noexcept
        {
            x = std::move(rhs.x);
            y = std::move(rhs.y);
            z = std::move(rhs.z);

            return *this;
        }

        bool operator==(const Vector3 &rhs) const noexcept
        {
            return x == rhs.x &&
                   y == rhs.y &&
                   z == rhs.z;
        }

        bool operator!=(const Vector3 &rhs) const noexcept
        {
            return !(*this == rhs);
        }

        //---
        Vector3 &operator+=(const Vector3 &vec) noexcept
        {
            x += vec.x;
            y += vec.y;
            z += vec.z;

            return *this;
        }

        template <Common::Concept::Number TN>
        Vector3 &operator+=(const TN val) noexcept
        {
            x += val;
            y += val;
            z += val;

            return *this;
        }

        Vector3 &operator-=(const Vector3 &vec) noexcept
        {
            x -= vec.x;
            y -= vec.y;
            z -= vec.z;

            return *this;
        }

        template <Common::Concept::Number TN>
        Vector3 &operator-=(const TN val) noexcept
        {
            x -= val;
            y -= val;
            z -= val;
            return *this;
        }

        Vector3 &operator*=(const Vector3 &vec) noexcept
        {
            x *= vec.x;
            y *= vec.y;
            z *= vec.z;
            return *this;
        }

        template <Common::Concept::Number TN>
        Vector3 &operator*=(const TN val) noexcept
        {
            x *= val;
            y *= val;
            z *= val;
            return *this;
        }

        Vector3 &operator/=(const Vector3 &vec) noexcept
        {
            x /= vec.x;
            y /= vec.y;
            z /= vec.z;
            return *this;
        }

        template <Common::Concept::Number TN>
        Vector3 &operator/=(const TN val) noexcept
        {
            x /= val;
            y /= val;
            z /= val;
            return *this;
        }

        //--- public methods ---
        bool isZero() const noexcept
        {
            return x == 0 &&
                   y == 0 &&
                   z == 0;
        }

        N length() const noexcept
        {
            return std::sqrt(x * x + y * y + z * z);
        }

        N lengthSquared() const noexcept
        {
            return x * x + y * y + z * z;
        }

        Vector3 normalized() const noexcept
        {
            const N length = std::sqrt(x * x + y * y + z * z);

            return {x / length, y / length, z / length};
        }

        void normalize() noexcept
        {
            const N length = std::sqrt(x * x + y * y + z * z);

            x = x / length;
            y = y / length;
            z = z / length;
        }

        N distanceToPoint(const Vector3 &vec) const noexcept
        {
            return (*this - vec).length();
        }

        N distanceToPlane(const Vector3 &vec, const Vector3 &normal) const noexcept
        {
            return dotProduct(*this - vec, normal);
        }

        N distanceToPlane(const Vector3 &vec1, const Vector3 &vec2, const Vector3 &vec3) const
            noexcept
        {
            const Vector3 normal = normal(vec2 - vec1, vec3 - vec1);

            return dotProduct(*this - vec1, normal);
        }

        N distanceToLine(const Vector3 &origin, const Vector3 &direction) const noexcept
        {
            if (direction.isZero())
                return (*this - origin).length();

            const Vector3 normal = origin + dotProduct(*this - origin, direction) * direction;

            return (*this - origin).length();
        }

        N dotProduct(const Vector3 &vec) const noexcept
        {
            return x * vec.x + y * vec.y + z * vec.z;
        }

        Vector3 crossProduct(const Vector3 &vec) const noexcept
        {
            return {y * vec.z - z * vec.y,
                    z * vec.x - x * vec.z,
                    x * vec.y - y * vec.x};
        }

        Vector3 normal(const Vector3 &vec) const noexcept
        {
            return crossProduct(vec).normalized();
        }

        Vector3 normal(const Vector3 &vec1, const Vector3 &vec2) const noexcept
        {
            return crossProduct((vec1 - *this), (vec2 - *this)).normalized();
        }

        std::string str() const
        {
            return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
        }
    };

    //--- global operators ---
    template <Common::Concept::Number N>
    inline Vector3<N> operator+(const Vector3<N> &vec1, const Vector3<N> &vec2) noexcept
    {
        return {vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z};
    }

    template <Common::Concept::Number N, Common::Concept::Number TN>
    inline Vector3<N> operator+(const Vector3<N> &vec, const TN val) noexcept
    {
        return {vec.x + val, vec.y + val, vec.z + val};
    }

    template <Common::Concept::Number N>
    inline Vector3<N> operator-(const Vector3<N> &vec1, const Vector3<N> &vec2) noexcept
    {
        return {vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z};
    }

    template <Common::Concept::Number N, Common::Concept::Number TN>
    inline Vector3<N> operator-(const Vector3<N> &vec, const TN val) noexcept
    {
        return {vec.x - val, vec.y - val, vec.z - val};
    }

    template <Common::Concept::Number N>
    inline Vector3<N> operator*(const Vector3<N> &vec1, const Vector3<N> &vec2) noexcept
    {
        return {vec1.x * vec2.x, vec1.y * vec2.y, vec1.z * vec2.z};
    }

    template <Common::Concept::Number N, Common::Concept::Number TN>
    inline Vector3<N> operator*(const Vector3<N> &vec, const TN val) noexcept
    {
        return {vec.x * val, vec.y * val, vec.z * val};
    }

    template <Common::Concept::Number N>
    inline Vector3<N> operator/(const Vector3<N> &vec1, const Vector3<N> &vec2) noexcept
    {
        return {vec1.x / vec2.x, vec1.y / vec2.y, vec1.z / vec2.z};
    }

    template <Common::Concept::Number N, Common::Concept::Number TN>
    inline Vector3<N> operator/(const Vector3<N> &vec, const TN val) noexcept
    {
        return {vec.x / val, vec.y / val, vec.z / val};
    }

    //--- global functions ---
    template <Common::Concept::Number N>
    inline N dotProduct(const Vector3<N> &vec1, const Vector3<N> &vec2) noexcept
    {
        return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
    }

    template <Common::Concept::Number N>
    inline Vector3<N> crossProduct(const Vector3<N> &vec1, const Vector3<N> &vec2) noexcept
    {
        return {vec1.y * vec2.z - vec1.z * vec2.y,
                vec1.z * vec2.x - vec1.x * vec2.z,
                vec1.x * vec2.y - vec1.y * vec2.x};
    }

    template <Common::Concept::Number N>
    inline Vector3<N> normal(const Vector3<N> &vec1, const Vector3<N> &vec2) noexcept
    {
        return crossProduct(vec1, vec2).normalized();
    }

    template <Common::Concept::Number N>
    inline Vector3<N> normal(const Vector3<N> &vec1, const Vector3<N> &vec2, const Vector3<N> &vec3)
        noexcept
    {
        return crossProduct((vec2 - vec1), (vec3 - vec1)).normalized();
    }
}
