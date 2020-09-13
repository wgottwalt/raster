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
    : Base(T::inRange(width, MinWidth, MaxWidth) ? width : 1,
           T::inRange(height, MinHeight, MaxHeight) ? height : 1, color)
    {
    }

    Simple00::Simple00(const Pixels &pixels, const int64_t width, const int64_t height)
    : Base()
    {
        if ((static_cast<uint64_t>(width * height)) == static_cast<uint64_t>(pixels.size()) &&
          T::inRange(width, MinWidth, MaxWidth) && T::inRange(height, MinHeight, MaxHeight))
            implReplace(pixels, width, height);
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
        return T::inRange(width(), MinWidth, MaxWidth) &&
               T::inRange(height(), MinHeight, MaxHeight) &&
               (static_cast<uint64_t>(pixels().size()) == static_cast<uint64_t>(width() * height()));
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
            {
                ofile.put(pixel.c1).put(pixel.c2).put(pixel.c3).put(pixel.c4).put(pixel.c5)
                     .put(pixel.c6).put(pixel.c7).put(pixel.c8);
            }
            ofile.close();

            return true;
        }

        return false;
    }

    bool Simple00::load(const std::string &filename)
    {
        if (std::ifstream ifile(filename); ifile.is_open() && ifile.good())
        {
            const uint64_t size = ifile.seekg(0, std::ios::end).tellg();
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
            if ((width.u == 0) || (height.u == 0) || (static_cast<uint64_t>(width.u) *
              static_cast<uint64_t>(height.u) * static_cast<uint64_t>(sizeof (RGBA))) != (size - 16))
            {
                ifile.close();
                return false;
            }

            pixels.resize(width.u * height.u, RGBA::Black);
            for (auto &pixel : pixels)
            {
                ifile.get(pixel.c1).get(pixel.c2).get(pixel.c3).get(pixel.c4).get(pixel.c5)
                     .get(pixel.c6).get(pixel.c7).get(pixel.c8);
            }
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
            const uint64_t size = ifile.seekg(0, std::ios::end).tellg();
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
