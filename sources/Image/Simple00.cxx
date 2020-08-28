#include <fstream>
#include <utility>
#include "Common/Endian.hxx"
#include "Common/Tools.hxx"
#include "Simple00.hxx"

namespace Image
{
    //--- internal stuff ---

    namespace E = Common::Endian;
    namespace T = Common::Tools;

    //--- public constructors ---

    Simple00::Simple00() noexcept
    : Base()
    {
    }

    Simple00::Simple00(const std::string &filename)
    : Base()
    {
        load(filename);
    }

    Simple00::Simple00(const int64_t width, const int64_t height, const RGBA color)
    : Base(width, height, color)
    {
    }

    Simple00::Simple00(const Pixels &pixels, const int64_t width, const int64_t height)
    : Base(pixels, width, height)
    {
    }

    Simple00::Simple00(const Simple00 &rhs)
    : Base(rhs)
    {
    }

    Simple00::Simple00(Simple00 &&rhs)
    : Base(std::move(rhs))
    {
    }

    Simple00::~Simple00() noexcept
    {
    }

    //--- public constructors ---

    Simple00 &Simple00::operator=(const Simple00 &rhs)
    {
        if (this != &rhs)
            Base::operator=(rhs);

        return *this;
    }

    Simple00 &Simple00::operator=(Simple00 &&rhs)
    {
        if (this != &rhs)
            Base::operator=(std::move(rhs));

        return *this;
    }

    bool Simple00::operator==(const Simple00 &rhs) const noexcept
    {
        return Base::operator==(rhs);
    }

    bool Simple00::operator!=(const Simple00 &rhs) const noexcept
    {
        return Base::operator!=(rhs);
    }

    //--- public methods ---

    bool Simple00::valid() const
    {
        return T::inRange(width(), 1, MaxWidth) && T::inRange(height(), 1, MaxHeight) &&
               (pixels().size() == static_cast<size_t>(width() * height()));
    }

    Simple00::RGBA Simple00::pixel(const int64_t x, const int64_t y) const
    {
        if (T::inRange(x, 0, width()) && T::inRange(y, 0, height()))
            return implPixel(x, y);
        else
            throw "POOF for now";
    }

    bool Simple00::setPixel(const int64_t x, const int64_t y, const RGBA color)
    {
        if (T::inRange(x, 0, width()) && T::inRange(y, 0, height()))
        {
            implSetPixel(x, y, color);
            return true;
        }

        return false;
    }

    bool Simple00::setLine(const int64_t x1, const int64_t y1, const int64_t x2, const int64_t y2,
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

    bool Simple00::setTriangle(const int64_t x1, const int64_t y1, const int64_t x2,
                               const int64_t y2, const int64_t x3, const int64_t y3,
                               const RGBA color, const bool fill)
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

    bool Simple00::setRectangle(const int64_t x1, const int64_t y1, const int64_t x2,
                                const int64_t y2, const RGBA color, const bool fill)
    {
        if (T::inRange(x1, 0, width()) && T::inRange(y1, 0, height()) &&
          T::inRange(x2, 0, width()) && T::inRange(y2, 0, height()))
        {
            implSetRectangle(x1, y1, x2, y2, color, fill);
            return true;
        }

        return false;
    }

    bool Simple00::setCircle(const int64_t x, const int64_t y, const int64_t radius,
                             const RGBA color, const bool fill)
    {
        const int64_t rad = std::abs(radius);

        if (T::inRange(x, 0, width()) && T::inRange(y, 0, height()) &&
          T::inRange(x + rad, 0, width()) && T::inRange(x - rad, 0, width()) &&
          T::inRange(y + rad, 0, height()) && T::inRange(y - rad, 0, height()))
        {
            implSetCircle(x, y, radius, color, fill);
            return true;
        }

        return false;
    }

    bool Simple00::resize(const int64_t width, const int64_t height, const Scaler scaler)
    {
        if ((width <= MaxWidth) && (height <= MaxHeight))
            return implResize(width, height, scaler);

        return false;
    }

    bool Simple00::save(const std::string &filename) const
    {
        if (std::ofstream ofile(filename); valid() && ofile.is_open() && ofile.good())
        {
            E::Union32 tmp;

            ofile.write("simple00", 8);
            tmp.u = width();
            tmp.u = E::toBE(tmp.u);
            ofile.put(tmp.c1).put(tmp.c2).put(tmp.c3).put(tmp.c4);
            tmp.u = height();
            tmp.u = E::toBE(tmp.u);
            ofile.put(tmp.c1).put(tmp.c2).put(tmp.c3).put(tmp.c4);
            for (const auto &pixel : pixels())
                ofile.put(pixel.c1).put(pixel.c2).put(pixel.c3).put(pixel.c4).put(pixel.c5)
                     .put(pixel.c6).put(pixel.c7).put(pixel.c8);
            ofile.close();

            return true;
        }

        return false;
    }

    bool Simple00::load(const std::string &filename)
    {
        if (std::ifstream ifile(filename); ifile.is_open() && ifile.good())
        {
            const size_t size = ifile.seekg(0, std::ios::end).tellg();
            std::string id(8, '\0');
            Pixels pixels;
            E::Union32 width;
            E::Union32 height;

            // header + at least one pixel = 8 + 4 + 4 + 8
            if ((size < 24) || (size % 8))
            {
                ifile.close();
                return false;
            }

            ifile.seekg(0, std::ios::beg);
            ifile.read(id.data(), id.size());
            if (id.compare("simple00") != 0)
            {
                ifile.close();
                return false;
            }

            ifile.get(width.c1).get(width.c2).get(width.c3).get(width.c4);
            ifile.get(height.c1).get(height.c2).get(height.c3).get(height.c4);
            width.u = E::fromBE(width.u);
            height.u = E::fromBE(height.u);
            if ((width.u == 0) || (height.u == 0) || (static_cast<size_t>(width.u) *
              static_cast<size_t>(height.u) * sizeof (RGBA)) != (size - 16))
            {
                ifile.close();
                return false;
            }

            pixels.resize(width.u * height.u, RGBA::Black);
            for (auto &pixel : pixels)
                ifile.get(pixel.c1).get(pixel.c2).get(pixel.c3).get(pixel.c4).get(pixel.c5)
                     .get(pixel.c6).get(pixel.c7).get(pixel.c8);
            ifile.close();

            implReplace(pixels, width.u, height.u);

            return true;
        }

        return false;
    }

    //--- static public methods ---

    bool Simple00::identify(const std::string &filename)
    {
        if (std::ifstream ifile(filename); ifile.is_open() && ifile.good())
        {
            const size_t size = ifile.seekg(0, std::ios::end).tellg();
            std::string id(8, '\0');

            if ((size < 24) || (size % 8))
            {
                ifile.close();
                return false;
            }

            ifile.seekg(0, std::ios::beg);
            ifile.read(id.data(), id.size());
            if (id.compare("simple00") != 0)
            {
                ifile.close();
                return false;
            }

            ifile.close();

            return true;
        }

        return false;
    }
}
