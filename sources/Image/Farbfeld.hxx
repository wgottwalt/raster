#pragma once

#include <limits>
#include "Base.hxx"

namespace Image
{
    //
    // Farbfeld - the famous suckless (https://suckless.org) image format
    //
    class Farbfeld : public Base {
    public:
        //--- public types and constants ---
        static constexpr int64_t MinWidth = 0;
        static constexpr int64_t MinHeight = 0;
        static constexpr int64_t MaxWidth = std::numeric_limits<uint32_t>::max();
        static constexpr int64_t MaxHeight = MaxWidth;

        //--- public constructors ---
        Farbfeld() noexcept;
        Farbfeld(const std::string &filename) noexcept(false);
        Farbfeld(const int64_t width, const int64_t height, const RGBA color = RGBA::Black)
            noexcept(false);
        Farbfeld(const Pixels &pixels, const int64_t width, const int64_t height) noexcept(false);
        Farbfeld(const Farbfeld &rhs) noexcept(false);
        Farbfeld(Farbfeld &&rhs) noexcept;
        virtual ~Farbfeld() noexcept;

        //--- public constructors ---
        Farbfeld &operator=(const Farbfeld &rhs) noexcept(false);
        Farbfeld &operator=(Farbfeld &&rhs) noexcept;
        bool operator==(const Farbfeld &rhs) const noexcept;
        bool operator!=(const Farbfeld &rhs) const noexcept;

        //--- public methods ---
        virtual bool valid() const override final;
        virtual bool resize(const int64_t width, const int64_t height, const Scaler scaler)
            noexcept(false) override final;
        virtual bool save(const std::string &filename) const noexcept(false) override final;
        virtual bool load(const std::string &filename) noexcept(false) override final;

        //--- static public methods ---
        static bool identify(const std::string &filename) noexcept(false);
    };
}
