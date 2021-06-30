#include <fstream>
#include <utility>
#include "Common/Endian.hxx"
#include "Common/Tools.hxx"
#include "Simple01.hxx"

namespace Image
{
    //--- internal stuff ---

    namespace E = Common::Endian;
    namespace T = Common::Tools;

    //--- public constructors ---
    Simple01::Simple01() noexcept
    : Base()
    {
    }

    Simple01::Simple01(const std::string &filename) noexcept(false)
    : Base()
    {
        load(filename);
    }

    Simple01::Simple01(const int64_t width, const int64_t height, const RGBA color) noexcept(false)
    : Base(T::inRange(width, MinWidth, MaxWidth) ? width : 1,
           T::inRange(height, MinHeight, MaxHeight) ? height : 1, color)
    {
    }

    Simple01::Simple01(const Pixels &pixels, const int64_t width, const int64_t height)
        noexcept(false)
    : Base()
    {
        if ((static_cast<uint64_t>(width * height) == static_cast<uint64_t>(pixels.size())) &&
          T::inRange(width, MinWidth, MaxWidth) && T::inRange(height, MinHeight, MaxHeight))
            implReplace(pixels, width, height);
    }

    Simple01::Simple01(const Simple01 &rhs) noexcept(false)
    : Base(rhs)
    {
    }

    Simple01::Simple01(Simple01 &&rhs) noexcept
    : Base(std::move(rhs))
    {
    }

    Simple01::~Simple01() noexcept
    {
    }

    //--- public constructors ---

    Simple01 &Simple01::operator=(const Simple01 &rhs) noexcept(false)
    {
        if (this != &rhs)
            Base::operator=(rhs);

        return *this;
    }

    Simple01 &Simple01::operator=(Simple01 &&rhs) noexcept
    {
        if (this != &rhs)
            Base::operator=(std::move(rhs));

        return *this;
    }

    bool Simple01::operator==(const Simple01 &rhs) const noexcept
    {
        return Base::operator==(rhs);
    }

    bool Simple01::operator!=(const Simple01 &rhs) const noexcept
    {
        return Base::operator!=(rhs);
    }

    //--- public methods ---

    bool Simple01::valid() const noexcept(false)
    {
        return T::inRange(width(), MinWidth, MaxWidth) &&
               T::inRange(height(), MinHeight, MaxHeight) &&
               (static_cast<uint64_t>(pixels().size()) == static_cast<uint64_t>(width() * height()));
    }

    bool Simple01::resize(const int64_t width, const int64_t height, const Scaler scaler)
        noexcept(false)
    {
        if ((width <= MaxWidth) && (height <= MaxHeight))
            return implResize(width, height, scaler);

        return false;
    }

    bool Simple01::save(const std::string &filename) const noexcept(false)
    {
        if (std::ofstream ofile(filename); valid() && ofile.is_open() && ofile.good())
        {
            const std::string data(encodeRLE(pixels()));
            E::Union64 size = {.u = data.size()};
            E::Union32 tmp;

            ofile.write("simple01", 8);
            tmp.u = width();
            tmp.u = E::toBE(tmp.u);
            ofile.put(tmp.c1).put(tmp.c2).put(tmp.c3).put(tmp.c4);
            tmp.u = height();
            tmp.u = E::toBE(tmp.u);
            ofile.put(tmp.c1).put(tmp.c2).put(tmp.c3).put(tmp.c4);
            size.u = E::toBE(size.u);
            ofile.put(size.c1).put(size.c2).put(size.c3).put(size.c4).put(size.c5).put(size.c6)
                 .put(size.c7).put(size.c8);
            ofile.write(data.c_str(), data.size());
            ofile.close();

            return true;
        }

        return false;
    }

    bool Simple01::load(const std::string &filename) noexcept(false)
    {
        if (std::ifstream ifile(filename); ifile.is_open() && ifile.good())
        {
            const uint64_t size = ifile.seekg(0, std::ios::end).tellg();
            std::string id(8, '\0');
            std::string buffer;
            Pixels pixels;
            E::Union64 dsize;
            E::Union32 width;
            E::Union32 height;

            if (size < 33) // id(8) + width(4) + height(4) + dsize(8) + RLE header(1) + one pixel(8)
            {
                ifile.close();
                return false;
            }

            ifile.seekg(0, std::ios::beg);
            ifile.read(id.data(), id.size());
            if (id.compare("simple01") != 0)
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

            buffer.resize(dsize.u, '\0');
            ifile.read(buffer.data(), buffer.size());
            ifile.close();

            pixels = decodeRLE(buffer);
            if (pixels.size() != (static_cast<size_t>(width.u) * static_cast<size_t>(height.u)))
                return false;

            implReplace(pixels, width.u, height.u);

            return true;
        }

        return false;
    }

    //--- static public methods ---

    bool Simple01::identify(const std::string &filename) noexcept
    {
        if (std::ifstream ifile(filename); ifile.is_open() && ifile.good())
        {
            const uint64_t size = ifile.seekg(0, std::ios::end).tellg();
            std::string id(8, '\0');
            E::Union64 rle_size;
            E::Union32 width;
            E::Union32 height;

            if (size < 33)
            {
                ifile.close();
                return false;
            }

            ifile.seekg(0, std::ios::beg);
            ifile.read(id.data(), id.size());
            if (id.compare("simple01") != 0)
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

            ifile.get(rle_size.c1).get(rle_size.c2).get(rle_size.c3).get(rle_size.c4)
                 .get(rle_size.c5).get(rle_size.c6).get(rle_size.c7).get(rle_size.c8);
            rle_size.u = E::fromBE(rle_size.u);
            if (size != (rle_size.u + 24))
            {
                ifile.close();
                return false;
            }

            ifile.close();

            return true;
        }

        return false;
    }

    //--- proctected methods ---

    std::string Simple01::encodeRLE(const Pixels &pixels) const noexcept(false)
    {
        const size_t psize = sizeof (RGBA);
        const size_t size = pixels.size();
        std::vector<uint8_t> buffer;
        std::string result;
        auto putPixelBytes = [&](auto &container, const size_t i)
        {
            container.push_back(pixels[i].rh);
            container.push_back(pixels[i].rl);
            container.push_back(pixels[i].gh);
            container.push_back(pixels[i].gl);
            container.push_back(pixels[i].bh);
            container.push_back(pixels[i].bl);
            container.push_back(pixels[i].ah);
            container.push_back(pixels[i].al);
        };

        for (size_t i = 0; i < size; ++i)
        {
            size_t count = 1;

            while ((i < (size - 1)) && (count < 128) && (pixels[i] == pixels[i + 1]))
            {
                ++count;
                ++i;
            }

            if ((count > 1) || (buffer.size() >= (128 * psize)))
            {
                if (!buffer.empty())
                {
                    result.push_back(static_cast<uint8_t>(buffer.size() / psize - 1));
                    result.insert(result.end(), buffer.begin(), buffer.end());
                    buffer.clear();
                }
                result.push_back(static_cast<uint8_t>((count - 1) | 128));
                putPixelBytes(result, i);
            }
            else
                putPixelBytes(buffer, i);
        }

        if (!buffer.empty())
        {
            result.push_back(static_cast<uint8_t>(buffer.size() / psize - 1));
            result.insert(result.end(), buffer.begin(), buffer.end());
        }

        return result;
    }

    Simple01::Pixels Simple01::decodeRLE(const std::string &data) const noexcept(false)
    {
        const size_t size = data.size();
        Pixels result;
        size_t pos = 0;

        while (pos < (size - 1))
        {
            const uint8_t rle = data[pos];
            const size_t count = (rle & 128) ? ((rle & ~128) + 1) : (rle + 1);
            RGBA pixel;

            ++pos;
            if (rle & 128)
            {
                pixel.rh = data[pos++];
                pixel.rl = data[pos++];
                pixel.gh = data[pos++];
                pixel.gl = data[pos++];
                pixel.bh = data[pos++];
                pixel.bl = data[pos++];
                pixel.ah = data[pos++];
                pixel.al = data[pos++];
                for (size_t i = 0; i < count; ++i)
                    result.push_back(pixel);
            }
            else
            {
                for (size_t i = 0; i < count; ++i)
                {
                    pixel.rh = data[pos++];
                    pixel.rl = data[pos++];
                    pixel.gh = data[pos++];
                    pixel.gl = data[pos++];
                    pixel.bh = data[pos++];
                    pixel.bl = data[pos++];
                    pixel.ah = data[pos++];
                    pixel.al = data[pos++];
                    result.push_back(pixel);
                }
            }
        }

        return result;
    }
}
