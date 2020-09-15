#pragma once

#include "Shape.hxx"

namespace Math
{
    template <Common::Concept::Number T>
    class Plane : public Shape<T> {
    public:
        //--- public constructors ---
        Plane() noexcept
        : Shape<T>(), _normal(0, 1, 0)
        {
        }

        Plane(const Vector3<T> &org, const Vector3<T> &norm) noexcept
        : Shape<T>(org), _normal(norm)
        {
        }

        Plane(const Plane &rhs) noexcept
        : Shape<T>(rhs), _normal(rhs._normal)
        {
        }

        Plane(Plane &&rhs) noexcept
        : Shape<T>(std::move(rhs)), _normal(std::move(rhs._normal))
        {
        }

        virtual ~Plane()
        {
        }

        //--- public operators ---
        Plane &operator=(const Plane &rhs) noexcept
        {
            if (this != &rhs)
            {
                Shape<T>::operator=(rhs);
                _normal = rhs._normal;
            }

            return *this;
        }

        Plane &operator=(Plane &&rhs) noexcept
        {
            if (this != &rhs)
            {
                Shape<T>::operator=(std::move(rhs));
                _normal = std::move(rhs._normal);
            }

            return *this;
        }

        bool operator==(const Plane &rhs) const noexcept
        {
            return operator==(rhs) &&
                   _normal == rhs._normal;
        }

        bool operator!=(const Plane &rhs) const noexcept
        {
            return !(*this == rhs);
        }

        //--- public methods ---
        Vector3<T> normal() const noexcept
        {
            return _normal;
        }

        void setNormal(const Vector3<T> &vec) noexcept
        {
            _normal = vec.normalized();
        }

        virtual Shape<T>::Intersection intersect(const Ray<T,Vector3> &ray) override final
        {
            const auto denom = Math::dotProduct(_normal, ray.direction());

            if (std::abs(denom) > 0)
            {
                const auto t = (Shape<T>::origin() - ray.origin()).dotProduct(_normal) / denom;

                if (t >= 0)
                    return {true, false, t, _normal, this};
            }

            return {false, false, 0, {}, nullptr};
        }

    private:
        //--- private properties ---
        Vector3<T> _normal;
    };
}
