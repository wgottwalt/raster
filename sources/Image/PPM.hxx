#pragma once

#include <limits>
#include <string>
#include "Base.hxx"

namespace Image
{
    class PPM : public Base {
    public:
        //--- public types and constants ---
        static const int64_t MaxWidth = std::numeric_limits<uint16_t>::max();
        static const int64_t MaxHeight = MaxWidth;

        //--- public constructors ---
        PPM() noexcept;
        PPM(const std::string &filename);
        PPM(const int64_t width, const int64_t height, const RGBA color = RGBA::Black);
        PPM(const Pixels &pixels, const int64_t width, const int64_t height);
        PPM(const PPM &rhs);
        PPM(PPM &&rhs);
        virtual ~PPM() noexcept;

        //--- public operators ---
        PPM &operator=(const PPM &rhs);
        PPM &operator=(PPM &&rhs);
        bool operator==(const PPM &rhs) const noexcept;
        bool operator!=(const PPM &rhs) const noexcept;

        //--- public methods ---
        std::string comment() const;
        void setComment(const std::string &str = "");
        bool wideMode() const;
        void setWideMode(const bool wide = true);
        bool binaryMode() const;
        void setBinaryMode(const bool bin = true);

        virtual bool valid() const override final;
        virtual bool resize(const int64_t width, const int64_t height, const Scaler scaler)
                            override final;
        virtual bool save(const std::string &filename) const override final;
        virtual bool load(const std::string &filename) override final;

        //--- static public methods ---
        static bool identify(const std::string &filename);

    private:
        //--- private properties ---
        std::string _comment;
        bool _wide;
        bool _binary;
    };
}
