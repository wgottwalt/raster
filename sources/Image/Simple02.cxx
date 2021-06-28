#include <fstream>
#include <sstream>
#include <utility>
#include "Common/Endian.hxx"
#include "Common/Tools.hxx"
#include "Compression/LZW16.hxx"
#include "Simple02.hxx"

namespace Image
{
    //--- internal stuff ---

    namespace E = Common::Endian;
    namespace T = Common::Tools;
    namespace C = Compression;

    //--- public constructors ---
    Simple02::Simple02() noexcept
    : Base()
    {
    }

    Simple02::Simple02(const std::string &filename) noexcept(false)
    : Base()
    {
        load(filename);
    }

    Simple02::Simple02(const int64_t width, const int64_t height, const RGBA color) noexcept(false)
    : Base(T::inRange(width, MinWidth, MaxWidth) ? width : 1,
           T::inRange(height, MinHeight, MaxHeight) ? height : 1, color)
    {
    }

    Simple02::Simple02(const Pixels &pixels, const int64_t width, const int64_t height)
        noexcept(false)
    : Base()
    {
        if ((static_cast<uint64_t>(width * height) == static_cast<uint64_t>(pixels.size())) &&
          T::inRange(width, MinWidth, MaxWidth) && T::inRange(height, MinHeight, MaxHeight))
            implReplace(pixels, width, height);
    }

    Simple02::Simple02(const Simple02 &rhs) noexcept(false)
    : Base(rhs)
    {
    }

    Simple02::Simple02(Simple02 &&rhs) noexcept
    : Base(std::move(rhs))
    {
    }

    Simple02::~Simple02() noexcept
    {
    }

    //--- public constructors ---

    Simple02 &Simple02::operator=(const Simple02 &rhs) noexcept(false)
    {
        if (this != &rhs)
            Base::operator=(rhs);

        return *this;
    }

    Simple02 &Simple02::operator=(Simple02 &&rhs) noexcept
    {
        if (this != &rhs)
            Base::operator=(std::move(rhs));

        return *this;
    }

    bool Simple02::operator==(const Simple02 &rhs) const noexcept
    {
        return Base::operator==(rhs);
    }

    bool Simple02::operator!=(const Simple02 &rhs) const noexcept
    {
        return Base::operator!=(rhs);
    }

    //--- public methods ---

    bool Simple02::valid() const
    {
        return T::inRange(width(), MinWidth, MaxWidth) &&
               T::inRange(height(), MinHeight, MaxHeight) &&
               (static_cast<uint64_t>(pixels().size()) == static_cast<uint64_t>(width() * height()));
    }

    bool Simple02::resize(const int64_t width, const int64_t height, const Scaler scaler)
        noexcept(false)
    {
        if ((width <= MaxWidth) && (height <= MaxHeight))
            return implResize(width, height, scaler);

        return false;
    }

    bool Simple02::save(const std::string &filename) const noexcept(false)
    {
        if (std::ofstream ofile(filename); valid() && ofile.is_open() && ofile.good())
        {
            C::LZW16 lzw;
            std::stringstream in;
            std::stringstream out;
            E::Union64 size;
            E::Union32 tmp;

            for (const auto &pixel : pixels())
            {
                in << pixel.c1 << pixel.c2 << pixel.c3 << pixel.c4 << pixel.c5 << pixel.c6
                   << pixel.c7 << pixel.c8;
            }

            lzw.encode(in, out);
            size.u = lzw.bytesWritten();

            ofile.write("simple02", 8);
            tmp.u = width();
            tmp.u = E::toBE(tmp.u);
            ofile.put(tmp.c1).put(tmp.c2).put(tmp.c3).put(tmp.c4);
            tmp.u = height();
            tmp.u = E::toBE(tmp.u);
            ofile.put(tmp.c1).put(tmp.c2).put(tmp.c3).put(tmp.c4);
            size.u = E::toBE(size.u);
            ofile.put(size.c1).put(size.c2).put(size.c3).put(size.c4).put(size.c5).put(size.c6)
                 .put(size.c7).put(size.c8);
            ofile << out.str();
            ofile.close();

            return true;
        }

        return false;
    }

    bool Simple02::load(const std::string &filename) noexcept(false)
    {
        if (std::ifstream ifile(filename); ifile.is_open() && ifile.good())
        {
            const uint64_t size = ifile.seekg(0, std::ios::end).tellg();
            C::LZW16 lzw;
            std::string id(8, '\0');
            std::stringstream in;
            std::stringstream out;
            Pixels pixels;
            E::Union64 dsize;
            E::Union32 width;
            E::Union32 height;

            if (size < 32)
            {
                ifile.close();
                return false;
            }

            ifile.seekg(0, std::ios::beg);
            ifile.read(id.data(), id.size());
            if (id.compare("simple02") != 0)
            {
                ifile.close();
                return false;
            }

            ifile.get(width.c1).get(width.c2).get(width.c3).get(width.c4);
            ifile.get(height.c1).get(height.c2).get(height.c3).get(height.c4);
            width.u = E::fromBE(width.u);
            height.u = E::fromBE(height.u);
            if ((width.u < MinWidth) || (height.u < MinHeight))
            {
                ifile.close();
                return false;
            }

            ifile.get(dsize.c1).get(dsize.c2).get(dsize.c3).get(dsize.c4).get(dsize.c5)
                 .get(dsize.c6).get(dsize.c7).get(dsize.c8);
            dsize.u = E::fromBE(dsize.u);
            if ((dsize.u + 24) != size)
            {
                ifile.close();
                return false;
            }

            in << ifile.rdbuf();
            ifile.close();
            if (dsize.u != static_cast<uint64_t>(in.tellp()))
                return false;

            lzw.decode(in, out);
            if (static_cast<uint64_t>(out.tellp()) != (width.u * height.u * sizeof (RGBA)))
                return false;

            pixels.resize(width.u * height.u);
            for (auto &pixel : pixels)
            {
                out.get(pixel.c1).get(pixel.c2).get(pixel.c3).get(pixel.c4).get(pixel.c5)
                   .get(pixel.c6).get(pixel.c7).get(pixel.c8);
            }

            implReplace(pixels, width.u, height.u);

            return true;
        }

        return false;
    }

    //--- static public methods ---

    bool Simple02::identify(const std::string &filename) noexcept
    {
        if (std::ifstream ifile(filename); ifile.is_open() && ifile.good())
        {
            const uint64_t size = ifile.seekg(0, std::ios::end).tellg();
            std::string id(8, '\0');
            E::Union64 lzw16_size;
            E::Union32 width;
            E::Union32 height;

            if (size < 24)
            {
                ifile.close();
                return false;
            }

            ifile.seekg(0, std::ios::beg);
            ifile.read(id.data(), id.size());
            if (id.compare("simple02") != 0)
            {
                ifile.close();
                return false;
            }

            ifile.get(width.c1).get(width.c2).get(width.c3).get(width.c4);
            ifile.get(height.c1).get(height.c2).get(height.c3).get(height.c4);
            width.u = E::fromBE(width.u);
            height.u = E::fromBE(height.u);
            if ((width.u < 1) || (height.u < 1))
            {
                ifile.close();
                return false;
            }

            ifile.get(lzw16_size.c1).get(lzw16_size.c2).get(lzw16_size.c3).get(lzw16_size.c4)
                 .get(lzw16_size.c5).get(lzw16_size.c6).get(lzw16_size.c7).get(lzw16_size.c8);
            lzw16_size.u = E::fromBE(lzw16_size.u);
            if (size != (lzw16_size.u + 24))
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
