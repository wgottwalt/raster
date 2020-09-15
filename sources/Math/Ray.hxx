#pragma once

#include "Vector3.hxx"

namespace Math
{
    template <Common::Concept::Number N, template <typename> typename V>
    class Ray {
    public:
        //--- public types and constants ---
        static constexpr N MinN = 0;
        static constexpr N MaxN = std::numeric_limits<N>::max();

        //--- public constructors ---
        Ray() noexcept
        : _origin(), _direction(), _distance(MaxN)
        {
        }

        Ray(const V<N> &org, const V<N> &dir, const N dist = MaxN) noexcept
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
        V<N> origin() const noexcept
        {
            return _origin;
        }

        void setOrigin(const V<N> &vec) noexcept
        {
            _origin = vec;
        }

        V<N> direction() const noexcept
        {
            return _direction;
        }

        void setDirection(const V<N> &vec) noexcept
        {
            _direction = vec;
        }

        N distance() const noexcept
        {
            return _distance;
        }

        bool setDistance(const N val) noexcept
        {
            if (Common::Tools::inRange(val, 0))
            {
                _distance = val;

                return true;
            }

            return false;
        }

        void pointAt(const V<N> &vec) noexcept
        {
            _direction = (vec - _origin).normalized();
        }

    private:
        //--- private properties ---
        V<N> _origin;
        V<N> _direction;
        N _distance;
    };
}
