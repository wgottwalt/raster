#pragma once

#include <utility>
#include "Common/Concepts.hxx"
#include "Math/Math.hxx"

namespace Math
{
    template <Common::Concept::Number T>
    class Shape {
    public:
        //--- public types and constants ---
        using Intersection = std::tuple<bool,bool,T,Vector3<T>,Shape *>;
        static constexpr T MinRange = 0;
        static constexpr T MaxRange = std::numeric_limits<T>::max();

        //--- public constructors ---
        Shape() noexcept
        : _origin()
        {
        }

        Shape(const Vector3<T> &org) noexcept
        : _origin(org)
        {
        }

        Shape(const Shape &rhs) noexcept
        : _origin(rhs._origin)
        {
        }

        Shape(Shape &&rhs) noexcept
        : _origin(std::move(rhs._origin))
        {
        }

        virtual ~Shape() noexcept
        {
        }

        //--- public operators ---
        Shape &operator=(const Shape &rhs) noexcept
        {
            _origin = rhs._origin;

            return *this;
        }

        Shape &operator=(Shape &&rhs) noexcept
        {
            _origin = std::move(rhs._origin);

            return *this;
        }

        bool operator==(const Shape &rhs) const noexcept
        {
            return _origin == rhs._origin;
        }

        bool operator!=(const Shape &rhs) const noexcept
        {
            return !(*this == rhs);
        }

        //--- public methods ---
        Vector3<T> origin() const noexcept
        {
            return _origin;
        }

        void setOrigin(const Vector3<T> &vec) noexcept
        {
            _origin = vec;
        }

        virtual Intersection intersect(const Ray<T,Vector3> &ray) = 0;

    private:
        //--- private properties ---
        Vector3<T> _origin;
    };
}
