#pragma once

#include "Vector3.hxx"

namespace Math
{
    template <Common::Concept::Number T>
    class Ray {
    public:
        //--- public types and constants ---
        static constexpr T MinT = 0;
        static constexpr T MaxT = std::numeric_limits<T>::max();

        //--- public constructors ---
        Ray() noexcept
        : _origin(), _direction(), _distance(MaxT)
        {
        }

        Ray(const Vector3<T> &org, const Vector3<T> &dir, const T dist = MaxT) noexcept
        : _origin(org), _direction(dir), _distance(dist)
        {
        }

        Ray(const Ray &rhs) noexcept
        : _origin(rhs._origin), _direction(rhs._direction), _distance(rhs._distance)
        {
        }

        Ray(Ray &&rhs) noexcept
        : _origin(std::move(rhs._origin)), _direction(std::move(rhs._direction)),
          _distance(std::move(rhs._distance))
        {
        }

        ~Ray() noexcept
        {
        }

        //--- public operators ---
        Ray &operator=(const Ray &rhs) noexcept
        {
            if (this != rhs)
            {
                _origin = rhs._origin;
                _direction = rhs._direction;
                _distance = rhs._distance;
            }

            return *this;
        }

        Ray &operator=(Ray &&rhs) noexcept
        {
            if (this != rhs)
            {
                _origin = std::move(rhs._origin);
                _direction = std::move(rhs._direction);
                _distance = std::move(rhs._distance);
            }

            return *this;
        }

        bool operator==(const Ray &rhs) const noexcept
        {
            return _origin == rhs._origin &&
                   _direction == rhs._direction &&
                   _distance == rhs._distance;
        }

        bool operator!=(const Ray &rhs) const noexcept
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

        Vector3<T> direction() const noexcept
        {
            return _direction;
        }

        void setDirection(const Vector3<T> &vec) noexcept
        {
            _direction = vec;
        }

        T distance() const noexcept
        {
            return _distance;
        }

        bool setDistance(const T val) noexcept
        {
            if (Common::Tools::inRange(val, 0))
            {
                _distance = val;

                return true;
            }

            return false;
        }

        void pointAt(const Vector3<T> &vec) noexcept
        {
            _direction = (vec - _origin).normalized();
        }

    private:
        //--- private properties ---
        Vector3<T> _origin;
        Vector3<T> _direction;
        T _distance;
    };
}
