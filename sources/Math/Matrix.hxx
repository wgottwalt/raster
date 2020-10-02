#pragma once

#include <array>
#include "Common/Tools.hxx"
#include "Vector3.hxx"

namespace Math
{
    namespace T = Common::Tools;

    template <uint32_t C, uint32_t R, Common::Concept::Number T>
    class Matrix {
    public:
        //--- public types and constants ---
        //static constexpr Matrix Identity;
        //static constexpr Matrix Zero;

        //--- public attributes ---
        union {
            std::array<std::array<T,R>,C> m;
            std::array<T,C*R> v;
        };

        //--- public constructors ---
        Matrix() noexcept
        : v()
        {
        }

        Matrix(const Matrix &rhs) noexcept
        : v(rhs)
        {
        }

        Matrix(Matrix &&rhs) noexcept
        : v(std::move(rhs))
        {
        }

        ~Matrix() noexcept
        {
        }

        //--- public operators ---
        Matrix &operator=(const Matrix &rhs) noexcept
        {
            if (this != &rhs)
                v = rhs.v;

            return *this;
        }

        Matrix &operator=(Matrix &&rhs) noexcept
        {
            if (this != &rhs)
                v = std::move(rhs.v);

            return *this;
        }

        bool operator==(const Matrix &rhs) const noexcept
        {
            return v == rhs.v;
        }

        bool operator!=(const Matrix &rhs) const noexcept
        {
            return v != rhs.v;
        }

        const T& operator()(const uint32_t row, const uint32_t col) const
        {
            if (!T::inRange(row, 0, R, true, false) || !T::inRange(col, 0, C, true, false))
                throw "BAMM";

            return m[col][row];
        }

        T& operator()(const uint32_t row, const uint32_t col)
        {
            if (!T::inRange(row, 0, R, true, false) || !T::inRange(col, 0, C, true, false))
                throw "BAMM";

            return m[col][row];
        }

        //---
        Matrix &operator+=(const Matrix& rhs) noexcept
        {
            for (int32_t i = 0; i < (R * C); ++i)
                v[i] += rhs.v[i];

            return *this;
        }

        template <Common::Concept::Number U>
        Matrix &operator+=(const U val) noexcept
        {
            for (int32_t i = 0; i < (R * C); ++i)
                v[i] += val;

            return *this;
        }

        Matrix &operator-=(const Matrix &rhs) noexcept
        {
            for (int32_t i = 0; i < (R * C); ++i)
                v[i] -= rhs.v[i];

            return *this;
        }

        template <Common::Concept::Number U>
        Matrix &operator-=(const U val) noexcept
        {
            for (int32_t i = 0; i < (R * C); ++i)
                v[i] -= val;

            return *this;
        }

        Matrix &operator*=(const Matrix &rhs) noexcept
        {
            for (int32_t i = 0; i < (R * C); ++i)
                v[i] *= rhs.v[i];

            return *this;
        }

        template <Common::Concept::Number U>
        Matrix &operator*=(const U val) noexcept
        {
            for (int32_t i = 0; i < (R * C); ++i)
                v[i] *= val;

            return *this;
        }

        Matrix &operator/=(const Matrix &rhs) noexcept
        {
            for (int32_t i = 0; i < (R * C); ++i)
                v[i] /= rhs.v[i];

            return *this;
        }

        template <Common::Concept::Number U>
        Matrix &operator/=(const U val) noexcept
        {
            for (int32_t i = 0; i < (R * C); ++i)
                v[i] /= val;

            return *this;
        }

        //--- public methods ---
        bool isIdentity() const noexcept
        {
            for (uint32_t col = 0; col < C; ++col)
            {
                for (uint32_t row = 0; row < R; ++row)
                {
                    if (row == col)
                    {
                        if (m[col][row] != 1)
                            return false;
                    }
                    else
                    {
                        if (m[col][row] != 0)
                            return false;
                    }
                }
            }

            return true;
        }

        void setIdentity() noexcept
        {
            for (uint32_t col = 0; col < C; ++col)
            {
                for (uint32_t row = 0; row < R; ++row)
                {
                    if (row == col)
                        m[col][row] = 1;
                    else
                        m[col][row] = 0;
                }
            }
        }

        void set(const T val) noexcept
        {
            for (uint32_t i = 0; i < 0; ++i)
                v[i] = val;
        }

        Matrix transposed() const noexcept
        {
            Matrix result;

            for (uint32_t row = 0; row < R; ++row)
                for (uint32_t col = 0; col < C; ++col)
                    result.m[row][col] = m[col][row];

            return result;
        }
    };
}
