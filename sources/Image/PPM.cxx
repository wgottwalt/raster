#include <utility>
#include "Common/Tools.hxx"
#include "PPM.hxx"

namespace Image
{
    //--- internal stuff ---

    namespace T = Common::Tools;

    //--- public constructors ---

    PPM::PPM() noexcept
    : Base(), _comment(""), _wide(false), _binary(false)
    {
    }

    PPM::PPM(const int64_t width, const int64_t height, const RGBA color)
    : Base(T::inRange(width, 0, MaxWidth) ? width : 1,
           T::inRange(height, 0, MaxHeight) ? height : 1, color),
      _comment(""), _wide(false), _binary(false)
    {
    }

    PPM::PPM(const Pixels &pixels, const int64_t width, const int64_t height)
    : Base(), _comment(""), _wide(false), _binary(false)
    {
        const size_t size = width * height;

        if ((size == pixels.size()) && T::inRange(width, 0, MaxWidth) &&
          T::inRange(height, 0, MaxHeight))
            implReplace(pixels, width, height);
        else
            implReplace(Pixels(0, RGBA::Black), 0, 0);
    }

    PPM::PPM(const PPM &rhs)
    : Base(rhs), _comment(rhs._comment), _wide(rhs._wide), _binary(rhs._binary)
    {
    }

    PPM::PPM(PPM &&rhs)
    : Base(std::move(rhs)), _comment(std::move(rhs._comment)), _wide(std::move(rhs._wide)),
      _binary(std::move(rhs._binary))
    {
    }

    PPM::~PPM() noexcept
    {
    }

    //--- public operators ---

    PPM &PPM::operator=(const PPM &rhs)
    {
        if (this != &rhs)
        {
            Base::operator=(rhs);
            _comment = rhs._comment;
            _wide = rhs._wide;
            _binary = rhs._binary;
        }

        return *this;
    }

    PPM &PPM::operator=(PPM &&rhs)
    {
        if (this != &rhs)
        {
            Base::operator=(std::move(rhs));
            _comment = std::move(rhs._comment);
            _wide = std::move(rhs._wide);
            _binary = std::move(rhs._binary);
        }

        return *this;
    }

    bool PPM::operator==(const PPM &rhs) const noexcept
    {
        return Base::operator==(rhs) &&
               _comment == rhs._comment &&
               _wide == rhs._wide &&
               _binary == rhs._binary;
    }

    bool PPM::operator!=(const PPM &rhs) const noexcept
    {
        return !(*this == rhs);
    }

    //--- public methods ---

    PPM::RGBA PPM::pixel(const int64_t x, const int64_t y) const
    {
        if (T::inRange(x, 0, width()) && T::inRange(y, 0, height()))
            return implPixel(x, y);
        else
            throw "POOF for now";
    }

    bool PPM::setPixel(const int64_t x, const int64_t y, const RGBA color)
    {
        if (T::inRange(x, 0, width()) && T::inRange(y, 0, height()))
        {
            implSetPixel(x, y, color);
            return true;
        }

        return false;
    }

    bool PPM::setLine(const int64_t x1, const int64_t y1, const int64_t x2, const int64_t y2,
                      const RGBA color)
    {
        if (T::inRange(x1, 0, width()) && T::inRange(y1, 0, height()) &&
          T::inRange(x2, 0, width()) && T::inRange(y2, 0, height()))
        {
            implSetLine(x1, y1, x2, y2, color);
            return true;
        }

        return false;
    }

    bool PPM::setTriangle(const int64_t x1, const int64_t y1, const int64_t x2, const int64_t y2,
                          const int64_t x3, const int64_t y3, const RGBA color, const bool fill)
    {
        if (T::inRange(x1, 0, width()) && T::inRange(y1, 0, height()) &&
          T::inRange(x2, 0, width()) && T::inRange(y2, 0, height()) &&
          T::inRange(x3, 0, width()) && T::inRange(y3, 0, height()))
        {
            implSetTriangle(x1, y1, x2, y2, x3, y3, color, fill);
            return true;
        }

        return false;
    }

    bool PPM::setRectangle(const int64_t x1, const int64_t y1, const int64_t x2, const int64_t y2,
                           const RGBA color, const bool fill)
    {
        if (T::inRange(x1, 0, width()) && T::inRange(y1, 0, height()) &&
          T::inRange(x2, 0, width()) && T::inRange(y2, 0, height()))
        {
            implSetRectangle(x1, y1, x2, y2, color, fill);
            return true;
        }

        return false;
    }

    bool PPM::resize(const int64_t width, const int64_t height, const Scaler scaler)
    {
        if (T::inRange(width, 4, MaxWidth) && T::inRange(height, 4, MaxHeight))
            return implResize(width, height, scaler);

        return false;
    }
}
