#include <fstream>
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

    PPM::PPM(const std::string &filename)
    : Base(), _comment(""), _wide(false), _binary(false)
    {
        load(filename);
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

    std::string PPM::comment() const
    {
        return _comment;
    }

    void PPM::setComment(const std::string &str)
    {
        _comment = str;
    }

    bool PPM::wideMode() const
    {
        return _wide;
    }

    void PPM::setWideMode(const bool wide)
    {
        _wide = wide;
    }

    bool PPM::binaryMode() const
    {
        return _binary;
    }

    void PPM::setBinaryMode(const bool bin)
    {
        _binary = bin;
    }

    bool PPM::valid() const
    {
        // having no pixels, aka a width/height of 0 is considert invalid, but the PPM image format
        // supports it
        return width() && height() && (pixels().size() == static_cast<size_t>(width() * height()));
    }

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

    bool PPM::setCircle(const int64_t x, const int64_t y, const int64_t radius, const RGBA color,
                        const bool fill)
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

    bool PPM::resize(const int64_t width, const int64_t height, const Scaler scaler)
    {
        if (T::inRange(width, 4, MaxWidth) && T::inRange(height, 4, MaxHeight))
            return implResize(width, height, scaler);

        return false;
    }

    bool PPM::save(const std::string &filename) const
    {
        if (!valid())
            return false;

        if (std::ofstream ofile(filename); ofile.is_open() && ofile.good())
        {
            if (_binary)
                ofile << "P6\n";
            else
                ofile << "P3\n";

            // multiline comments can be tricky, so need to catch newlines
            if (!_comment.empty())
            {
                size_t i = 0;

                ofile << "# ";
                for (i = 0; i < _comment.size(); ++i)
                {
                    if (_comment[i] == '\n')
                        ofile << "# ";
                    ofile << _comment[i];
                }
                if (_comment[i] != '\n')
                    ofile << '\n';
            }
            ofile << width() << ' ' << height() << '\n' << (_wide ? MaxWidth : 255) << '\n';

            if (!_wide && !_binary)
                for (auto &pixel : pixels())
                    ofile << static_cast<uint16_t>(pixel.rh) << ' '
                          << static_cast<uint16_t>(pixel.gh) << ' '
                          << static_cast<uint16_t>(pixel.bh) << '\n';
            else if (!_wide && _binary)
                for (auto &pixel : pixels())
                    ofile.put(pixel.c1).put(pixel.c3).put(pixel.c5);
            else if (_wide && !_binary)
                for (auto &pixel : pixels())
                    ofile << pixel.r << ' ' << pixel.g << ' ' << pixel.b << '\n';
            else if (_wide && _binary)
                for (auto &pixel : pixels())
                    ofile.put(pixel.c1).put(pixel.c2).put(pixel.c3).put(pixel.c4).put(pixel.c5)
                         .put(pixel.c6);
            ofile.close();

            return true;
        }

        return false;
    }

    bool PPM::load(const std::string &filename)
    {
        if (std::ifstream ifile(filename); ifile.is_open() && ifile.good())
        {
            std::string comment;
            std::string line;
            Pixels pixels;
            int64_t width = 0;
            int64_t height = 0;
            int64_t colors = 0;
            int64_t binary = false;

            std::getline(ifile, line);
            if (!line.compare("P6"))
                binary = true;
            else if (!line.compare("P3"))
                binary = false;
            else
            {
                ifile.close();
                return false;
            }

            // comments can be all over the header, which can be really anoying
            while ((ifile.peek() == '#') && std::getline(ifile, line))
                comment += T::trim(line.substr(1, std::string::npos)) + '\n';
            ifile >> width;
            while ((ifile.peek() == '#') && std::getline(ifile, line))
                comment += T::trim(line.substr(1, std::string::npos)) + '\n';
            ifile >> height;
            while ((ifile.peek() == '#') && std::getline(ifile, line))
                comment += T::trim(line.substr(1, std::string::npos)) + '\n';
            ifile >> colors;

            // there is ONE newline at the end of the header .. then pixel data starts
            if (ifile.peek() == '\n')
                ifile.get();

            if (!T::inRange(width, 0, MaxWidth) || !T::inRange(height, 0, MaxHeight) ||
              !T::inRange(colors, 1, MaxWidth))
            {
                ifile.close();
                return false;
            }

            pixels.resize(width * height, RGBA::Black);
            if (!binary)
            {
                uint32_t tmp = 0;

                for (auto &pixel : pixels)
                {
                    ifile >> tmp;
                    pixel.r = tmp * MaxWidth / colors;
                    ifile >> tmp;
                    pixel.g = tmp * MaxWidth / colors;
                    ifile >> tmp;
                    pixel.b = tmp * MaxWidth / colors;
                }
            }
            else if (binary && (colors < 256))
            {
                for (auto &pixel : pixels)
                {
                    ifile.get(pixel.c2).get(pixel.c4).get(pixel.c6);
                    pixel.r = pixel.r * MaxWidth / colors;
                    pixel.g = pixel.g * MaxWidth / colors;
                    pixel.b = pixel.b * MaxWidth / colors;
                }
            }
            else if (binary && (colors > 255))
                for (auto &pixel : pixels)
                    ifile.get(pixel.c1).get(pixel.c2).get(pixel.c3).get(pixel.c4).get(pixel.c5)
                         .get(pixel.c6);
            ifile.close();

            implReplace(pixels, width, height);
            _comment = comment;
            _wide = (colors > 255) ? true : false;
            _binary = binary;

            return true;
        }

        return false;
    }

    //--- static public methods ---

    bool PPM::identify(const std::string &filename)
    {
        if (std::ifstream ifile(filename); ifile.is_open() && ifile.good())
        {
            std::string line;

            std::getline(ifile, line);
            ifile.close();

            if ((line.substr(0, 2) == "P3") || (line.substr(0, 2) == "P6"))
                return true;
        }

        return false;
    }
}
