#pragma once

#include <cmath>
#include "Common/Tools.hxx"
#include "Shape.hxx"

namespace Math
{
    template <Common::Concept::Number T>
    class Camera : public Shape<T> {
    public:
        //--- public types and constants ---
        static const int64_t MinInt64 = 0;
        static const int64_t MaxInt64 = std::numeric_limits<int64_t>::max();

        //--- public constructors ---
        Camera() noexcept
        : Shape<T>({0, 0, -10}), _up(0, 1, 0), _lookat(0, 0, 0), _w(0, 0, 0), _u(0, 0, 0),
          _v(0, 0, 0), _width(640), _height(480), _wunits(_width), _hunits(_height)
        {
            setupWUV();
        }

        Camera(const Vector3<T> &org, const Vector3<T> &at, const int64_t width,
               const int64_t height) noexcept
        : Shape<T>(org), _up(0, 1, 0), _lookat(at), _w(0, 0, 0), _u(0, 0, 0), _v(0, 0, 0),
          _width(width), _height(height), _wunits(width), _hunits(height)
        {
            setupWUV();
        }

        Camera(const Camera &rhs) noexcept
        : Shape<T>(rhs), _up(rhs._up), _lookat(rhs._lookat), _w(rhs._w), _u(rhs._u), _v(rhs._v),
          _width(rhs._width), _height(rhs._height), _wunits(rhs._wunits), _hunits(rhs._hunits)
        {
        }

        Camera(Camera &&rhs) noexcept
        : Shape<T>(std::move(rhs)), _up(std::move(rhs._up)), _lookat(std::move(rhs._lookat)),
          _w(std::move(rhs._w)), _u(std::move(rhs._u)), _v(std::move(rhs._v)),
          _width(std::move(rhs._width)), _height(std::move(rhs._height)),
          _wunits(std::move(rhs._wunits)), _hunits(std::move(rhs._hunits))
        {
        }

        virtual ~Camera() noexcept
        {
        }

        //--- public operators ---
        Camera &operator=(const Camera &rhs) noexcept
        {
            if (this != &rhs)
            {
                Shape<T>::operator=(rhs);
                _up = rhs._up;
                _lookat = rhs._lookat;
                _w = rhs._w;
                _u = rhs._u;
                _v = rhs._v;
                _width = rhs._width;
                _height = rhs._height;
                _wunits = rhs._wunits;
                _hunits = rhs._hunits;
            }

            return *this;
        }

        Camera &operator=(Camera &&rhs) noexcept
        {
            if (this != &rhs)
            {
                Shape<T>::operator=(std::move(rhs));
                _up = std::move(rhs._up);
                _lookat = std::move(rhs._lookat);
                _w = std::move(rhs._w);
                _u = std::move(rhs._u);
                _v = std::move(rhs._v);
                _width = std::move(rhs._width);
                _height = std::move(rhs._height);
                _wunits = std::move(rhs._wunits);
                _hunits = std::move(rhs._hunits);
            }

            return *this;
        }

        bool operator==(const Camera &rhs) const noexcept
        {
            return Shape<T>::operator==(rhs) &&
                   _up == rhs._up &&
                   _lookat == rhs._lookat &&
                   _w == rhs._w &&
                   _u == rhs._u &&
                   _v == rhs._v &&
                   _width == rhs._width &&
                   _height == rhs._height &&
                   _wunits == rhs._wunits &&
                   _hunits == rhs._hunits;
        }

        bool operator!=(const Camera &rhs) const noexcept
        {
            return !(*this == rhs);
        }

        //--- public methods ---
        Vector3<T> up() const noexcept
        {
            return _up;
        }

        void setUp(const Vector3<T> &vec) noexcept
        {
            _up = vec.normalize();
            setupWUV();
        }

        Vector3<T> lookAt() const noexcept
        {
            return _lookat;
        }

        void setLookAt(const Vector3<T> &vec) noexcept
        {
            _lookat = vec.normalized();
            setupWUV();
        }

        int64_t width() const noexcept
        {
            return _width;
        }

        bool setWidth(const int64_t val, const bool adjust_units = true) noexcept
        {
            if (Common::Tools::inRange(val, MinInt64, MaxInt64))
            {
                _width = val;
                _wunits = adjust_units ? (static_cast<T>(val) / static_cast<T>(10)) : _wunits;

                return true;
            }

            return false;
        }

        int64_t height() const noexcept
        {
            return _height;
        }

        bool setHeight(const int64_t val, const bool adjust_units = true) noexcept
        {
            if (Common::Tools::inRange(val, MinInt64, MaxInt64))
            {
                _height = val;
                _hunits = adjust_units ? (static_cast<T>(val) / static_cast<T>(10)) : _hunits;

                return 0;
            }

            return false;
        }

        T wUnits() const noexcept
        {
            return _wunits;
        }

        bool setWUnits(const T val) noexcept
        {
            if (Common::Tools::inRange(val, 0))
            {
                _wunits = val;

                return true;
            }

            return false;
        }

        T hUnits() const noexcept
        {
            return _hunits;
        }

        bool setHUnits(const T val) noexcept
        {
            if (Common::Tools::inRange(val, 0))
            {
                _hunits = val;

                return true;
            }

            return false;
        }

        Ray<T> ray(const int64_t x, const int64_t y) const noexcept
        {
            if (Common::Tools::inRange(x, MinInt64, MaxInt64) &&
              Common::Tools::inRange(y, MinInt64, MaxInt64))
            {
                const T u = (static_cast<T>(2 * x + 1) / static_cast<T>(2 * _width) - .5) * _wunits;
                const T v = (static_cast<T>(2 * y + 1) / static_cast<T>(2 * _height) - .5) * _hunits;
                const Vector3<T> p = _lookat + (_u * u) + (_v * v);

                return {Shape<T>::origin(), (p - Shape<T>::origin()).normalized()};
            }

            return {{0, 0, 0}, {0, 0, 0}};
        }

        virtual Camera::Intersection intersect(const Ray<T> &) const override final
        {
            return {false, 0, 0};
        }

    protected:
        //--- protected methods ---
        void setupWUV()
        {
            const Vector3<T> diff = Shape<T>::origin() - _lookat;

            _w = diff.normalized();
            _u = _up.crossProduct(diff).normalized();
            _v = _w.crossProduct(_u);
        }

    private:
        //--- private properties ---
        Vector3<T> _up;
        Vector3<T> _lookat;
        Vector3<T> _w;
        Vector3<T> _u;
        Vector3<T> _v;
        int64_t _width;
        int64_t _height;
        T _wunits;
        T _hunits;
    };
}
