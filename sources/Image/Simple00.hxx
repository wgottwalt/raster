#pragma once

#include <limits>
#include "Base.hxx"

namespace Image
{
    //
    // Simple00 - first version of a very simple picture format
    //
    // - max dimensions are 2^32 bits
    // - a width or height of 0 is invalid
    // - screen orientation is top-left to bottom-right
    // - pixel data format is RGBA16161616
    // - data structure is row-major
    // - version 0 format: identifier "simple00" (8 bytes), width (32bit unsigned),
    //                     height (32bit unsigned), pixel data (width * height * RGBA16161616)
    // - everything is big endian
    //
    class Simple00 : public Base {
    public:
        //--- public types and constants ---
        static constexpr int64_t MinWidth = 1;
        static constexpr int64_t MinHeight = 1;
        static constexpr int64_t MaxWidth = std::numeric_limits<uint32_t>::max();
        static constexpr int64_t MaxHeight = std::numeric_limits<uint32_t>::max();

        //--- public constructors ---
        Simple00() noexcept;
        Simple00(const std::string &filename);
        Simple00(const int64_t width, const int64_t height, const RGBA color = RGBA::Black);
        Simple00(const Pixels &pixels, const int64_t width, const int64_t height);
        Simple00(const Simple00 &rhs);
        Simple00(Simple00 &&rhs);
        virtual ~Simple00() noexcept;

        //--- public constructors ---
        Simple00 &operator=(const Simple00 &rhs);
        Simple00 &operator=(Simple00 &&rhs);
        bool operator==(const Simple00 &rhs) const noexcept;
        bool operator!=(const Simple00 &rhs) const noexcept;

        //--- public methods ---
        virtual bool valid() const override final;
        virtual bool resize(const int64_t width, const int64_t height, const Scaler scaler)
                            override final;
        virtual bool save(const std::string &filename) const override final;
        virtual bool load(const std::string &filename) override final;

        //--- static public methods ---
        static bool identify(const std::string &filename);
    };
}
