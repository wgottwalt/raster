#pragma once

#include <limits>
#include "Base.hxx"

namespace Image
{
    //
    // Simple02 - third version of the Simple image format, same as Simple01, but uses LZW16
    //            compression instead of RLE
    //
    // - uses LZW16 compression ignoring scanlines
    // - version 2 format: identifier "simple02" (8 bytes), width (32bit unsigned), height (32bit
    //                     unsigned), lzw16 size (64bit unsigned), lzw16 data
    // - everything is big endian
    class Simple02 : public Base {
    public:
        //--- public types and constants ---
        static constexpr int64_t MinWidth = 1;
        static constexpr int64_t MinHeight = 1;
        static constexpr int64_t MaxWidth = std::numeric_limits<uint32_t>::max();
        static constexpr int64_t MaxHeight = std::numeric_limits<uint32_t>::max();

        //--- public constructors ---
        Simple02() noexcept;
        Simple02(const std::string &filename) noexcept(false);
        Simple02(const int64_t width, const int64_t height, const RGBA color = RGBA::Black)
            noexcept(false);
        Simple02(const Pixels &pixels, const int64_t width, const int64_t height) noexcept(false);
        Simple02(const Simple02 &rhs) noexcept(false);
        Simple02(Simple02 &&rhs) noexcept;
        virtual ~Simple02() noexcept;

        //--- public constructors ---
        Simple02 &operator=(const Simple02 &rhs) noexcept(false);
        Simple02 &operator=(Simple02 &&rhs) noexcept;
        bool operator==(const Simple02 &rhs) const noexcept;
        bool operator!=(const Simple02 &rhs) const noexcept;

        //--- public methods ---
        virtual bool valid() const noexcept(false) override final;
        virtual bool resize(const int64_t width, const int64_t height, const Scaler scaler)
            noexcept(false) override final;
        virtual bool save(const std::string &filename) const noexcept(false) override final;
        virtual bool load(const std::string &filename) noexcept(false) override final;

        //--- static public methods ---
        static bool identify(const std::string &filename) noexcept;
    };
}
