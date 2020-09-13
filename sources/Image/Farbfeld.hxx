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
        Farbfeld();
        Farbfeld(const std::string &filename);
        Farbfeld(const int64_t width, const int64_t height, const RGBA color = RGBA::Black);
        Farbfeld(const Pixels &pixels, const int64_t width, const int64_t height);
        Farbfeld(const Farbfeld &rhs);
        Farbfeld(Farbfeld &&rhs);
        virtual ~Farbfeld();

        //--- public constructors ---
        Farbfeld &operator=(const Farbfeld &rhs);
        Farbfeld &operator=(Farbfeld &&rhs);
        bool operator==(const Farbfeld &rhs) const;
        bool operator!=(const Farbfeld &rhs) const;

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
