#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <X11/Xlib.h>
#include "Color/RGBA16161616.hxx"
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
        Base();
        Base(const int64_t width, const int64_t height, const RGBA color);
        Base(const Pixels &pixels, const int64_t width, const int64_t height);
        Base(const Base &rhs);
        Base(Base &&rhs);
        virtual ~Base();

        //--- public operators ---
        Base &operator=(const Base &rhs);
        Base &operator=(Base &&rhs);
        bool operator==(const Base &rhs) const noexcept;
        bool operator!=(const Base &rhs) const noexcept;

        //--- public methods ---
        int64_t width() const noexcept;
        int64_t height() const noexcept;
        const Pixels &pixels() const noexcept;
        int64_t usedColors() const;
        void flipVertical();
        void flipHorizontal();
        bool filter(const Filter filter);
        bool reduceColors(const int64_t colors, const Quantizer quantizer);
        XImage *cloneXImage(Display *display, Visual *visual) const;

        virtual bool valid() const = 0;
        virtual RGBA pixel(const int64_t x, const int64_t y) const = 0;
        virtual bool setPixel(const int64_t x, const int64_t y, const RGBA color) = 0;
        virtual bool setLine(const int64_t x1, const int64_t y1, const int64_t x2, const int64_t y2,
                             const RGBA color) = 0;
        virtual bool setTriangle(const int64_t x1, const int64_t y1, const int64_t x2,
                                 const int64_t y2, const int64_t x3, const int64_t y3,
                                 const RGBA color, const bool fill) = 0;
        virtual bool setRectangle(const int64_t x1, const int64_t y1, const int64_t x2,
                                  const int64_t y2, const RGBA color, const bool fill) = 0;
        virtual bool setCircle(const int64_t x, const int64_t y, const int64_t radius,
                               const RGBA color, const bool fill) = 0;
        virtual bool resize(const int64_t width, const int64_t height, const Scaler scaler) = 0;
        virtual bool save(const std::string &filename) const = 0;
        virtual bool load(const std::string &filename) = 0;

    protected:
        //--- protected methods ---
        RGBA implPixel(const int64_t x, const int64_t y) const;
        void implSetPixel(const int64_t x, const int64_t y, const RGBA color);
        void implSetLine(int64_t x1, int64_t y1, int64_t x2, int64_t y2, const RGBA color);
        void implSetTriangle(const int64_t x1, const int64_t y1, const int64_t x2, const int64_t y2,
                             const int64_t x3, const int64_t y3, const RGBA color, const bool fill);
        void implSetRectangle(const int64_t x1, const int64_t y1, const int64_t x2,
                              const int64_t y2, const RGBA color, const bool fill);
        void implSetCircle(const int64_t x, const int64_t y, const int64_t radius, const RGBA color,
                           const bool fill);
        Pixels implFilter(const Pixels &pixels, const int64_t width, const int64_t height,
                          const Filter filter) const;
        bool implResize(const int64_t width, const int64_t height, const Scaler scaler);
        void implReplace(const Pixels &pixels, const int64_t width, const int64_t height);
        void implReplace(Pixels &&pixels, const int64_t width, const int64_t height);

    private:
        //--- private properties ---
        Pixels _data;
        int64_t _width;
        int64_t _height;
    };
}
