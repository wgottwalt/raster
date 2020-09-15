#pragma once

#include "Shape.hxx"

namespace Math
{
    template <Common::Concept::Number T>
    class Sphere : public Shape<T> {
    public:
        //--- public types and constants ---
        static constexpr T MinR = 0;
        static constexpr T MaxR = std::numeric_limits<T>::max();

        //--- public constructors ---
        Sphere() noexcept
        : Shape<T>(), _radius()
        {
        }

        Sphere(const Vector3<T> &org, const T rad) noexcept
        : Shape<T>(org), _radius(rad)
        {
        }

        Sphere(const Sphere &rhs) noexcept
        : Shape<T>(rhs), _radius(rhs._radius)
        {
        }

        Sphere(Sphere &&rhs) noexcept
        : Shape<T>(std::move(rhs)), _radius(std::move(rhs._radius))
        {
        }

        virtual ~Sphere() noexcept
        {
        }

        //--- public operators ---
        Sphere &operator=(const Sphere &rhs) noexcept
        {
            if (this != &rhs)
            {
                Shape<T>::operator=(rhs);
                _radius = rhs._radius;
            }

            return *this;
        }

        Sphere &operator=(Sphere &&rhs) noexcept
        {
            if (this != &rhs)
            {
                Shape<T>::operator=(std::move(rhs));
                _radius = std::move(rhs._radius);
            }

            return *this;
        }

        bool operator==(const Sphere &rhs) const noexcept
        {
            return Shape<T>::operator==(rhs) &&
                   _radius == rhs._radius;
        }

        bool operator!=(const Sphere &rhs) const noexcept
        {
            return !(*this == rhs);
        }

        //--- public methods ---
        T radius() const noexcept
        {
            return _radius;
        }

        bool setRadius(const T val) noexcept
        {
            if (Common::Tools::inRange(val, MinR, MaxR))
            {
                _radius = val;

                return true;
            }

            return false;
        }

        virtual Sphere::Intersection intersect(const Ray<T,Vector3> &ray) override final
        {
            const Vector3<T> diff = Shape<T>::origin() - ray.origin();

            if (const T a = diff.dotProduct(ray.direction()); a > 0)
            {
                const T r2 = _radius * _radius;

                if (const T b = diff.dotProduct(diff) - (a * a); b <= r2)
                {
                    const T c = std::sqrt(r2 - b);
                    const T t0 = a - c;
                    const T t1 = a + c;

                    if (t0 > 0)
                    {
                        const auto cam_to_hit = ray.direction() * t0 + ray.origin();
                        return {true, false, t0, -(Shape<T>::origin() - cam_to_hit).normalized(), this};
                    }
                }
            }

            return {false, false, 0, {}, nullptr};
        }

    private:
        //--- private properties ---
        T _radius;
    };
}
