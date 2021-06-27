#pragma once

#include <limits>
#include <string>
#include "Base.hxx"

namespace Image
{
    class PPM : public Base {
    public:
        //--- public types and constants ---
        static constexpr int64_t MinWidth = 1;
        static constexpr int64_t MinHeight = 1;
        static constexpr int64_t MaxWidth = std::numeric_limits<uint16_t>::max();
        static constexpr int64_t MaxHeight = MaxWidth;

        //--- public constructors ---
        PPM() noexcept;
        PPM(const std::string &filename) noexcept(false);
        PPM(const int64_t width, const int64_t height, const RGBA color = RGBA::Black)
            noexcept(false);
        PPM(const Pixels &pixels, const int64_t width, const int64_t height) noexcept(false);
        PPM(const PPM &rhs) noexcept(false);
        PPM(PPM &&rhs) noexcept;
        virtual ~PPM() noexcept;

        //--- public operators ---
        PPM &operator=(const PPM &rhs) noexcept(false);
        PPM &operator=(PPM &&rhs) noexcept;
        bool operator==(const PPM &rhs) const noexcept;
        bool operator!=(const PPM &rhs) const noexcept;

        //--- public methods ---
        std::string comment() const noexcept;
        void setComment(const std::string &str = "") noexcept(false);
        bool wideMode() const noexcept;
        void setWideMode(const bool wide = true) noexcept(false);
        bool binaryMode() const noexcept;
        void setBinaryMode(const bool bin = true) noexcept(false);

        virtual bool valid() const override final;
        virtual bool resize(const int64_t width, const int64_t height, const Scaler scaler)
            noexcept(false) override final;
        virtual bool save(const std::string &filename) const noexcept(false) override final;
        virtual bool load(const std::string &filename) noexcept(false) override final;

        //--- static public methods ---
        static bool identify(const std::string &filename) noexcept(false);

    private:
        //--- private properties ---
        std::string _comment;
        bool _wide;
        bool _binary;
    };
}
