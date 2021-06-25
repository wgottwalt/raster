#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <X11/Xlib.h>
#include "Color/Color.hxx"
#include "Common/Concepts.hxx"

namespace Image
{
    //--- base types and constants ---
    enum class Filter : int16_t {
        Smooth,
        Sharpen,
        Edge,
        Blur,
        Raised
    };

    enum class Scaler : int16_t {
        Clear,
        Keep,
        Nearest,
        FastBillinear
    };

    enum class Quantizer : int16_t {
        MiddleCut // actually a "fucked up" median cut algorithm, results are okay-ish, but slow
    };

    //
    // Base Image class - provides all the pixel manipulation methods, an interface every derived
    //                    class has to follow and holds the actual image data
    //
    class Base {
    public:
        //--- public types and constants ---
        using RGBA = Color::RGBA16161616;
        using Pixels = std::vector<RGBA>;

        //--- public constructors ---
        Base() noexcept;
        Base(const int64_t width, const int64_t height, const RGBA color) noexcept(false);
        Base(const Pixels &pixels, const int64_t width, const int64_t height) noexcept(false);
        Base(const Base &rhs) noexcept(false);
        Base(Base &&rhs) noexcept;
        virtual ~Base() noexcept;

        //--- public operators ---
        Base &operator=(const Base &rhs) noexcept(false);
        Base &operator=(Base &&rhs) noexcept;
        bool operator==(const Base &rhs) const noexcept;
        bool operator!=(const Base &rhs) const noexcept;

        //--- public methods ---
        int64_t width() const noexcept;
        int64_t height() const noexcept;
        const Pixels &pixels() const noexcept;
        int64_t usedColors() const noexcept(false);
        void flipVertical() noexcept(false);
        void flipHorizontal() noexcept(false);
        bool filter(const Filter filter) noexcept(false);
        bool reduceColors(const int64_t colors, const Quantizer quantizer) noexcept(false);
        RGBA pixel(const int64_t x, const int64_t y) const noexcept(false);
        bool setPixel(const int64_t x, const int64_t y, const RGBA color) noexcept;
        bool setLine(const int64_t x1, const int64_t y1, const int64_t x2, const int64_t y2,
                     const RGBA color) noexcept;
        bool setTriangle(const int64_t x1, const int64_t y1, const int64_t x2, const int64_t y2,
                         const int64_t x3, const int64_t y3, const RGBA color, const bool fill)
            noexcept;
        bool setRectangle(const int64_t x1, const int64_t y1, const int64_t x2, const int64_t y2,
                          const RGBA color, const bool fill) noexcept;
        bool setCircle(const int64_t x, const int64_t y, const int64_t radius, const RGBA color,
                       const bool fill) noexcept;
        XImage *cloneXImage(Display *display, Visual *visual) const noexcept;

        virtual bool valid() const = 0;
        virtual bool resize(const int64_t width, const int64_t height, const Scaler scaler)
            noexcept(false) = 0;
        virtual bool save(const std::string &filename) const noexcept(false) = 0;
        virtual bool load(const std::string &filename) noexcept(false) = 0;

    protected:
        //--- protected methods ---
        RGBA implPixel(const int64_t x, const int64_t y) const noexcept;
        void implSetPixel(const int64_t x, const int64_t y, const RGBA color) noexcept;
        void implSetLine(int64_t x1, int64_t y1, int64_t x2, int64_t y2, const RGBA color) noexcept;
        void implSetTriangle(const int64_t x1, const int64_t y1, const int64_t x2, const int64_t y2,
                             const int64_t x3, const int64_t y3, const RGBA color, const bool fill)
            noexcept;
        void implSetRectangle(const int64_t x1, const int64_t y1, const int64_t x2,
                              const int64_t y2, const RGBA color, const bool fill) noexcept;
        void implSetCircle(const int64_t x, const int64_t y, const int64_t radius, const RGBA color,
                           const bool fill) noexcept;
        Pixels implFilter(const Pixels &pixels, const int64_t width, const int64_t height,
                          const Filter filter) const noexcept(false);
        bool implResize(const int64_t width, const int64_t height, const Scaler scaler)
            noexcept(false);
        void implReplace(const Pixels &pixels, const int64_t width, const int64_t height) noexcept;
        void implReplace(Pixels &&pixels, const int64_t width, const int64_t height) noexcept;

    private:
        //--- private properties ---
        Pixels _data;
        int64_t _width;
        int64_t _height;
    };
}
