#pragma once

#include <limits>
#include "Base.hxx"

namespace Image
{
    //
    // Simple01 - second version of the Simple image format, same as Simple00, but uses RLE
    //            compression
    //
    // - uses RLE compression with 1-byte headers, most significant bit set means repeat, other
    //   seven bits hold the amount (max 128), rle data is per pixel (RGBA16161616) and ignores
    //   scanlines
    // - version 1 format: identifier "simple01" (8 byte), width (32bit unsigned), height (32bit
    //                     unsigned), size (64bit unsigned), rle data (size bytes)
    // - everything is big endian
    //
    class Simple01 : public Base {
    public:
        //--- public types and constants ---
        static constexpr int64_t MinWidth = 1;
        static constexpr int64_t MinHeight = 1;
        static constexpr int64_t MaxWidth = std::numeric_limits<uint32_t>::max();
        static constexpr int64_t MaxHeight = std::numeric_limits<uint32_t>::max();

        //--- public constructors ---
        Simple01() noexcept;
        Simple01(const std::string &filename) noexcept(false);
        Simple01(const int64_t width, const int64_t height, const RGBA color = RGBA::Black)
            noexcept(false);
        Simple01(const Pixels &pixels, const int64_t width, const int64_t height) noexcept(false);
        Simple01(const Simple01 &rhs) noexcept(false);
        Simple01(Simple01 &&rhs) noexcept;
        virtual ~Simple01() noexcept;

        //--- public constructors ---
        Simple01 &operator=(const Simple01 &rhs) noexcept(false);
        Simple01 &operator=(Simple01 &&rhs) noexcept;
        bool operator==(const Simple01 &rhs) const noexcept;
        bool operator!=(const Simple01 &rhs) const noexcept;

        //--- public methods ---
        virtual bool valid() const override final;
        virtual bool resize(const int64_t width, const int64_t height, const Scaler scaler)
            noexcept(false) override final;
        virtual bool save(const std::string &filename) const noexcept(false) override final;
        virtual bool load(const std::string &filename) noexcept(false) override final;

        //--- static public methods ---
        static bool identify(const std::string &filename) noexcept;

    protected:
        //--- proctected methods ---
        std::string encodeRLE(const Pixels &pixels) const noexcept(false);
        Pixels decodeRLE(const std::string &data) const noexcept(false);
    };
}
