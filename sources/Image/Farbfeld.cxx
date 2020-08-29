#include <fstream>
#include <utility>
#include "Common/Endian.hxx"
#include "Common/Tools.hxx"
#include "Farbfeld.hxx"

namespace Image
{
    //--- internal stuff ---

    namespace E = Common::Endian;
    namespace T = Common::Tools;

    //--- public constructors ---

    Farbfeld::Farbfeld()
    : Base()
    {
    }

    Farbfeld::Farbfeld(const std::string &filename)
    : Base()
    {
        load(filename);
    }

    Farbfeld::Farbfeld(const int64_t width, const int64_t height, const RGBA color)
    : Base(width, height, color)
    {
    }

    Farbfeld::Farbfeld(const Pixels &pixels, const int64_t width, const int64_t height)
    : Base(pixels, width, height)
    {
    }

    Farbfeld::Farbfeld(const Farbfeld &rhs)
    : Base(rhs)
    {
    }

    Farbfeld::Farbfeld(Farbfeld &&rhs)
    : Base(std::move(rhs))
    {
    }

    Farbfeld::~Farbfeld()
    {
    }

    //--- public constructors ---

    Farbfeld &Farbfeld::operator=(const Farbfeld &rhs)
    {
        if (this != &rhs)
            Base::operator=(rhs);

        return *this;
    }

    Farbfeld &Farbfeld::operator=(Farbfeld &&rhs)
    {
        if (this != &rhs)
            Base::operator=(std::move(rhs));

        return *this;
    }

    bool Farbfeld::operator==(const Farbfeld &rhs) const
    {
        return Base::operator==(rhs);
    }

    bool Farbfeld::operator!=(const Farbfeld &rhs) const
    {
        return Base::operator!=(rhs);
    }

    //--- public methods ---

    bool Farbfeld::valid() const
    {
        return T::inRange(width(), 0, MaxWidth) && T::inRange(height(), 0, MaxHeight) &&
               (pixels().size() == static_cast<size_t>(width() * height()));
    }

    bool Farbfeld::resize(const int64_t width, const int64_t height, const Scaler scaler)
    {
        if (T::inRange(width, 4, MaxWidth) && T::inRange(height, 4, MaxHeight))
            return implResize(width, height, scaler);

        return false;
    }

    bool Farbfeld::save(const std::string &filename) const
    {
        if (std::ofstream ofile(filename); valid() && ofile.is_open() && ofile.good())
        {
            E::Union32 tmp;

            ofile.write("farbfeld", 8);
            tmp.u = width();
            tmp.u = E::toBE(tmp.u);
            ofile.put(tmp.c1).put(tmp.c2).put(tmp.c3).put(tmp.c4);
            tmp.u = height();
            tmp.u = E::toBE(tmp.u);
            ofile.put(tmp.c1).put(tmp.c2).put(tmp.c3).put(tmp.c4);
            for (auto &pixel : pixels())
                ofile.put(pixel.c1).put(pixel.c2).put(pixel.c3).put(pixel.c4).put(pixel.c5)
                     .put(pixel.c6).put(pixel.c7).put(pixel.c8);
            ofile.close();

            return true;
        }

        return false;
    }

    bool Farbfeld::load(const std::string &filename)
    {
        if (std::ifstream ifile(filename); valid() && ifile.is_open() && ifile.good())
        {
            const size_t size = ifile.seekg(0, std::ios::end).tellg();
            std::string id(8, '\0');
            Pixels pixels;
            E::Union32 width;
            E::Union32 height;

            if ((size < (id.size() + sizeof (width) + sizeof (height))) || (size % 8))
            {
                ifile.close();
                return false;
            }

            ifile.seekg(0, std::ios::beg);
            ifile.read(id.data(), id.size());
            if (id != "farbfeld")
            {
                ifile.close();
                return false;
            }

            ifile.get(width.c1).get(width.c2).get(width.c3).get(width.c4);
            ifile.get(height.c1).get(height.c2).get(height.c3).get(height.c4);
            width.u = E::fromBE(width.u);
            height.u = E::fromBE(height.u);
            if ((size - 16) != (width.u * height.u * sizeof (RGBA)))
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

    bool Farbfeld::identify(const std::string &filename)
    {
        if (std::ifstream ifile(filename); ifile.is_open() && ifile.good())
        {
            const size_t size = ifile.seekg(0, std::ios::end).tellg();
            std::string id(8, '\0');

            if ((size < 16) || (size % 8))
            {
                ifile.close();
                return false;
            }

            ifile.seekg(0, std::ios::beg);
            ifile.read(id.data(), id.size());
            if (id != "farbfeld")
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
