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

    PPM::PPM(const std::string &filename) noexcept(false)
    : Base(), _comment(""), _wide(false), _binary(false)
    {
        load(filename);
    }

    PPM::PPM(const int64_t width, const int64_t height, const RGBA color) noexcept(false)
    : Base(T::inRange(width, MinWidth, MaxWidth) ? width : 1,
           T::inRange(height, MinHeight, MaxHeight) ? height : 1, color),
      _comment(""), _wide(false), _binary(false)
    {
    }

    PPM::PPM(const Pixels &pixels, const int64_t width, const int64_t height) noexcept(false)
    : Base(), _comment(""), _wide(false), _binary(false)
    {
        if ((static_cast<uint64_t>(width * height) == static_cast<uint64_t>(pixels.size())) &&
          T::inRange(width, MinWidth, MaxWidth) &&
          T::inRange(height, MinHeight, MaxHeight))
            implReplace(pixels, width, height);
    }

    PPM::PPM(const PPM &rhs) noexcept(false)
    : Base(rhs), _comment(rhs._comment), _wide(rhs._wide), _binary(rhs._binary)
    {
    }

    PPM::PPM(PPM &&rhs) noexcept
    : Base(std::move(rhs)), _comment(std::move(rhs._comment)), _wide(std::move(rhs._wide)),
      _binary(std::move(rhs._binary))
    {
    }

    PPM::~PPM() noexcept
    {
    }

    //--- public operators ---

    PPM &PPM::operator=(const PPM &rhs) noexcept(false)
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

    PPM &PPM::operator=(PPM &&rhs) noexcept
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

    std::string PPM::comment() const noexcept
    {
        return _comment;
    }

    void PPM::setComment(const std::string &str) noexcept(false)
    {
        _comment = str;
    }

    bool PPM::wideMode() const noexcept
    {
        return _wide;
    }

    void PPM::setWideMode(const bool wide) noexcept(false)
    {
        _wide = wide;
    }

    bool PPM::binaryMode() const noexcept
    {
        return _binary;
    }

    void PPM::setBinaryMode(const bool bin) noexcept(false)
    {
        _binary = bin;
    }

    bool PPM::valid() const noexcept(false)
    {
        // having no pixels, aka a width/height of 0 is considert invalid, but the PPM image format
        // supports it
        return (width() * height() > 0) &&
               (static_cast<uint64_t>(pixels().size())) == static_cast<uint64_t>(width() * height());
    }

    bool PPM::resize(const int64_t width, const int64_t height, const Scaler scaler) noexcept(false)
    {
        if ((width <= MaxWidth) && (height <= MaxHeight))
            return implResize(width, height, scaler);

        return false;
    }

    bool PPM::save(const std::string &filename) const noexcept(false)
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
            {
                for (auto &pixel : pixels())
                    ofile << static_cast<uint16_t>(pixel.rh) << ' '
                          << static_cast<uint16_t>(pixel.gh) << ' '
                          << static_cast<uint16_t>(pixel.bh) << '\n';
            }
            else if (!_wide && _binary)
            {
                for (auto &pixel : pixels())
                    ofile.put(pixel.c1).put(pixel.c3).put(pixel.c5);
            }
            else if (_wide && !_binary)
            {
                for (auto &pixel : pixels())
                    ofile << pixel.r << ' ' << pixel.g << ' ' << pixel.b << '\n';
            }
            else if (_wide && _binary)
            {
                for (auto &pixel : pixels())
                    ofile.put(pixel.c1).put(pixel.c2).put(pixel.c3).put(pixel.c4).put(pixel.c5)
                         .put(pixel.c6);
            }
            ofile.close();

            return true;
        }

        return false;
    }

    bool PPM::load(const std::string &filename) noexcept(false)
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

            if (!T::inRange(width, MinWidth, MaxWidth) ||
              !T::inRange(height, MinHeight, MaxHeight) ||
              !T::inRange(colors, MinWidth, MaxWidth))
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
            {
                for (auto &pixel : pixels)
                    ifile.get(pixel.c1).get(pixel.c2).get(pixel.c3).get(pixel.c4).get(pixel.c5)
                         .get(pixel.c6);
            }
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

    bool PPM::identify(const std::string &filename) noexcept(false)
    {
        if (std::ifstream ifile(filename); ifile.is_open() && ifile.good())
        {
            std::string comment;
            std::string line;
            int64_t width = 0;
            int64_t height = 0;
            int64_t colors = 0;

            std::getline(ifile, line);

            if ((line.substr(0, 2) != "P3") && (line.substr(0, 2) != "P6"))
            {
                ifile.close();
                return false;
            }

            while ((ifile.peek() == '#') && std::getline(ifile, line))
                comment += T::trim(line.substr(1, std::string::npos)) + '\n';
            ifile >> width;
            while ((ifile.peek() == '#') && std::getline(ifile, line))
                comment += T::trim(line.substr(1, std::string::npos)) + '\n';
            ifile >> height;
            while ((ifile.peek() == '#') && std::getline(ifile, line))
                comment += T::trim(line.substr(1, std::string::npos)) + '\n';
            ifile >> colors;

            if (ifile.peek() == '\n')
                ifile.get();

            if (!T::inRange(width, MinWidth, MaxWidth) ||
              !T::inRange(height, MinHeight, MaxHeight) ||
              !T::inRange(colors, MinWidth, MaxWidth))
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
