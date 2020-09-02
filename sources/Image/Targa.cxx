#include <fstream>
#include <map>
#include <stdexcept>
#include "Common/Endian.hxx"
#include "Common/Tools.hxx"
#include "Targa.hxx"

namespace Image
{
    //--- internal stuff ---
    namespace C = Color;
    namespace E = Common::Endian;
    namespace Q = C::Quantize;
    namespace T = Common::Tools;
    using IT = Targa::ImageType;
    using IA = Targa::ImageAttribute;

    //--- public constructors ---

    Targa::Targa() noexcept
    : Base(), _colormap_type(0), _image_type(IT::Truecolor), _colormap_offset(0),
      _colormap_length(0), _colormap_entry_size(0), _x_origin(0), _y_origin(0), _depth(24),
      _image_descriptor(T::valueOf(IA::OriginTop)), _image_id(""), _version2(false)
    {
    }

    Targa::Targa(const std::string &filename)
    : Base(), _colormap_type(0), _image_type(IT::Truecolor), _colormap_offset(0),
      _colormap_length(0), _colormap_entry_size(0), _x_origin(0), _y_origin(0), _depth(24),
      _image_descriptor(T::valueOf(IA::OriginTop)), _image_id(""), _version2(false)
    {
        load(filename);
    }

    Targa::Targa(const int64_t width, const int64_t height, const RGBA color)
    : Base(T::inRange(width, MinWidth, MaxWidth) ? width : 0,
           T::inRange(height, MinHeight, MaxHeight) ? height : 0, color),
      _colormap_type(0), _image_type(IT::Truecolor), _colormap_offset(0),
      _colormap_length(0), _colormap_entry_size(0), _x_origin(0), _y_origin(0), _depth(24),
      _image_descriptor(T::valueOf(IA::OriginTop)), _image_id(""), _version2(false)
    {
    }

    Targa::Targa(const Pixels &pixels, const int64_t width, const int64_t height)
    : Base(), _colormap_type(0), _image_type(IT::Truecolor), _colormap_offset(0),
      _colormap_length(0), _colormap_entry_size(0), _x_origin(0), _y_origin(0), _depth(24),
      _image_descriptor(T::valueOf(IA::OriginTop)), _image_id(""), _version2(false)
    {
        if (T::inRange(width, MinWidth, MaxWidth) && T::inRange(height, MinHeight, MaxHeight) &&
          (pixels.size() == static_cast<size_t>(width * height)))
            implReplace(pixels, width, height);
    }

    Targa::Targa(const Targa &rhs)
    : Base(rhs), _colormap_type(rhs._colormap_type), _image_type(rhs._image_type),
      _colormap_offset(rhs._colormap_offset), _colormap_length(rhs._colormap_length),
      _colormap_entry_size(rhs._colormap_entry_size), _x_origin(rhs._x_origin),
      _y_origin(rhs._y_origin), _depth(rhs._depth), _image_descriptor(rhs._image_descriptor),
      _image_id(rhs._image_id), _version2(rhs._version2)
    {
    }

    Targa::Targa(Targa &&rhs)
    : Base(std::move(rhs)), _colormap_type(std::move(rhs._colormap_type)),
      _image_type(std::move(rhs._image_type)), _colormap_offset(std::move(rhs._colormap_offset)),
      _colormap_length(std::move(rhs._colormap_length)),
      _colormap_entry_size(std::move(rhs._colormap_entry_size)),
      _x_origin(std::move(rhs._x_origin)), _y_origin(std::move(rhs._y_origin)),
      _depth(std::move(rhs._depth)), _image_descriptor(std::move(rhs._image_descriptor)),
      _image_id(std::move(rhs._image_id)), _version2(std::move(rhs._version2))
    {
    }

    Targa::~Targa() noexcept
    {
    }

    //--- public operators ---

    Targa &Targa::operator=(const Targa &rhs)
    {
        if (this != &rhs)
        {
            Base::operator=(rhs);
            _colormap_type = rhs._colormap_type;
            _image_type = rhs._image_type;
            _colormap_offset = rhs._colormap_offset;
            _colormap_length = rhs._colormap_length;
            _colormap_entry_size = rhs._colormap_entry_size;
            _x_origin = rhs._x_origin;
            _y_origin = rhs._y_origin;
            _depth = rhs._depth;
            _image_descriptor = rhs._image_descriptor;
            _image_id = rhs._image_id;
            _version2 = rhs._version2;
        }

        return *this;
    }

    Targa &Targa::operator=(Targa &&rhs)
    {
        if (this != &rhs)
        {
            Base::operator=(std::move(rhs));
            _colormap_type = std::move(rhs._colormap_type);
            _image_type = std::move(rhs._image_type);
            _colormap_offset = std::move(rhs._colormap_offset);
            _colormap_length = std::move(rhs._colormap_length);
            _colormap_entry_size = std::move(rhs._colormap_entry_size);
            _x_origin = std::move(rhs._x_origin);
            _y_origin = std::move(rhs._y_origin);
            _depth = std::move(rhs._depth);
            _image_descriptor = std::move(rhs._image_descriptor);
            _image_id = std::move(rhs._image_id);
            _version2 = std::move(rhs._version2);
        }

        return *this;
    }

    bool Targa::operator==(const Targa &rhs) const noexcept
    {
        return Base::operator==(rhs) &&
               _colormap_type == rhs._colormap_type &&
               _image_type == rhs._image_type &&
               _colormap_offset == rhs._colormap_offset &&
               _colormap_length == rhs._colormap_length &&
               _colormap_entry_size == rhs._colormap_entry_size &&
               _x_origin == rhs._x_origin &&
               _y_origin == rhs._y_origin &&
               _depth == rhs._depth &&
               _image_descriptor == rhs._image_descriptor &&
               _image_id == rhs._image_id &&
               _version2 == rhs._version2;
    }

    bool Targa::operator!=(const Targa &rhs) const noexcept
    {
        return !(*this == rhs);
    }

    //--- public methods ---

    std::string Targa::id() const
    {
        return _image_id;
    }

    void Targa::setId(const std::string &str)
    {
        // well, id is limited to 255 chars
        _image_id = str.substr(0, 255);
    }

    Targa::ImageType Targa::imageType() const
    {
        return static_cast<IT>(_image_type);
    }

    void Targa::setImageType(const ImageType type)
    {
        switch (type)
        {
            case IT::NoData:
                // why the complexity here? well, Targa can be used to exchange palettes without
                // actual pixel data ... quite similar to Amiga's IFF ILBM format, so by setting
                // this image type the save method is able to write out a targa file only holding
                // the palette of the current pixel data
                if (_colormap_type >= 1)
                {
                    _colormap_type = 1;
                    _colormap_offset = 0;
                    _colormap_length = 256;
                    if (T::includes(_depth, 15, 16, 24, 32))
                        _colormap_entry_size = _depth;
                    else
                        _colormap_entry_size = 24;
                    _depth = 8;
                    _image_descriptor &= ~0x0F;
                    if (_colormap_entry_size == 16)
                        _image_descriptor |= 0x01;
                    else if (_colormap_entry_size == 32)
                        _image_descriptor |= 0x08;
                }
                else
                {
                    _colormap_type = 0;
                    _colormap_offset = 0;
                    _colormap_length = 0;
                    _colormap_entry_size = 0;
                    _image_descriptor &= ~0x0F;
                    if (_depth == 16)
                        _image_descriptor |= 0x01;
                    else if (_depth == 32)
                        _image_descriptor |= 0x08;
                }
                break;

            case IT::Mapped:
            case IT::MappedRLE:
                _colormap_type = 1;
                _colormap_offset = 0;
                _colormap_length = 256;
                if (T::includes(_depth, 15, 16, 24, 32))
                    _colormap_entry_size = _depth;
                else
                    _colormap_entry_size = 24;
                _depth = 8;
                _image_descriptor &= ~0x0F;
                if (_colormap_entry_size == 16)
                    _image_descriptor |= 0x01;
                else if (_colormap_entry_size == 32)
                    _image_descriptor |= 0x08;
                break;

            case IT::Truecolor:
            case IT::TruecolorRLE:
                _colormap_type = 0;
                _colormap_offset = 0;
                _colormap_length = 0;
                _colormap_entry_size = 0;
                _image_descriptor &= ~0x0F;
                if (_depth <= 8)
                    _depth = 24;
                if (_depth == 16)
                    _image_descriptor |= 0x01;
                if (_depth == 32)
                    _image_descriptor |= 0x08;
                break;

            case IT::Mono:
            case IT::MonoRLE:
                _colormap_type = 0;
                _colormap_offset = 0;
                _colormap_length = 0;
                _colormap_entry_size = 0;
                _depth = 8;
                _image_descriptor &= ~0x0F;
                break;

            case IT::MappedAll:
            case IT::MappedAllQuad:
                throw std::logic_error("propreitary image type not supported (and never will be)");
        }

        _image_type = type;
    }

    uint8_t Targa::depth() const
    {
        return _depth;
    }

    bool Targa::setDepth(const uint8_t val)
    {
        // first set the ImageType, then set depth (everything else would raise the complexity to
        // nuts levels)
        if (!T::includes(val, 15, 16, 24, 32))
            return false;

        switch (_image_type)
        {
            case IT::NoData:
                if (_colormap_type >= 1)
                {
                    _colormap_entry_size = val;
                    _depth = 8;
                }
                else
                    _depth = val;
                _image_descriptor &= ~0x0F;
                if (_colormap_entry_size == 16)
                    _image_descriptor |= 0x01;
                else if (_colormap_entry_size == 32)
                    _image_descriptor |= 0x08;
                break;

            case IT::Mapped:
            case IT::MappedRLE:
                _colormap_entry_size = val;
                _depth = 8;
                _image_descriptor &= ~0x0F;
                if (_colormap_entry_size == 16)
                    _image_descriptor |= 0x01;
                else if (_colormap_entry_size == 32)
                    _image_descriptor |= 0x08;
                break;

            case IT::Truecolor:
            case IT::TruecolorRLE:
                _depth = val;
                _image_descriptor &= ~0x0F;
                if (_colormap_entry_size == 16)
                    _image_descriptor |= 0x01;
                else if (_colormap_entry_size == 32)
                    _image_descriptor |= 0x08;
                break;

            case IT::Mono:
            case IT::MonoRLE:
            case IT::MappedAll:
            case IT::MappedAllQuad:
                return false;
        }

        return true;
    }

    bool Targa::isVersion2() const
    {
        return _version2;
    }

    void Targa::setVersion2(const bool val)
    {
        // setting version 2 (Targa spec 2.0) actually lowers RLE compression effiency
        _version2 = val;
    }

    bool Targa::valid() const
    {
        // XXX: this is just a minimalistic check, beef it up!
        return T::inRange(width(), MinWidth, MaxWidth) &&
               T::inRange(height(), MinHeight, MaxHeight) &&
               (static_cast<size_t>(width() * height()) == pixels().size());
    }

    bool Targa::resize(const int64_t width, const int64_t height, const Scaler scaler)
    {
        if ((width <= MaxWidth) && (height <= MaxHeight))
            return implResize(width, height, scaler);

        return false;
    }

    bool Targa::save(const std::string &filename) const
    {
        if (std::ofstream ofile(filename); valid() && ofile.is_open() && ofile.good())
        {
            std::string data;
            Pixels palette;
            E::Union16 tmp16;
            E::Union8 tmp8;

            switch (_image_type)
            {
                case IT::NoData:
                    break;

                case IT::Mapped:
                    data = genMappedData(palette, pixels());
                    break;

                case IT::Truecolor:
                    data = genTruecolorData(pixels());
                    break;

                case IT::Mono:
                    data = genMonoData(pixels());
                    break;

                case IT::MappedRLE:
                    data = genMappedRleData(palette, pixels());
                    break;

                case IT::TruecolorRLE:
                    data = genTruecolorRleData(pixels());
                    break;

                case IT::MonoRLE:
                    data = genMonoRleData(pixels());
                    break;

                case IT::MappedAll:
                case IT::MappedAllQuad:
                    // this one shouldn't happen anyway, it can't be set
                    throw std::logic_error("propreitary image type not supported");
            }

            // header
            tmp8.u = _image_id.size();
            ofile.put(tmp8.c1);
            tmp8.u = _colormap_type;
            ofile.put(tmp8.c1);
            tmp8.u = T::valueOf(_image_type);
            ofile.put(tmp8.c1);
            tmp16.u = _colormap_offset;
            ofile.put(tmp16.c1).put(tmp16.c2);
            tmp16.u = _colormap_length;
            ofile.put(tmp16.c1).put(tmp16.c2);
            tmp8.u = _colormap_entry_size;
            ofile.put(tmp8.c1);
            tmp16.u = _x_origin;
            ofile.put(tmp16.c1).put(tmp16.c2);
            tmp16.u = _y_origin;
            ofile.put(tmp16.c1).put(tmp16.c2);
            tmp16.u = width();
            ofile.put(tmp16.c1).put(tmp16.c2);
            tmp16.u = height();
            ofile.put(tmp16.c1).put(tmp16.c2);
            tmp8.u = _depth;
            ofile.put(tmp8.c1);
            tmp8.u = _image_descriptor;
            ofile.put(tmp8.c1);
            if (!_image_id.empty())
            {
                ofile.write(_image_id.c_str(), _image_id.size());
                ofile.put('\0');
            }

            // image palette
            if (_colormap_type && (_colormap_length == palette.size()))
            {
                std::string palette_data;

                switch (_colormap_entry_size)
                {
                    case 15:
                    case 16:
                    {
                        const bool alpha_bit = _image_descriptor & 0x01;
                        C::UnionRGBA5551 tmp;

                        palette_data.reserve(_colormap_length * 2);
                        for (const auto &pixel : palette)
                        {
                            tmp.u = 0;
                            tmp.u |= (pixel.rh >> 3) << 10;
                            tmp.u |= (pixel.gh >> 3) << 5;
                            tmp.u |= (pixel.bh >> 3);
                            tmp.u |= (alpha_bit && static_cast<bool>(pixel.a)) << 15;
                            //tmp.r = pixel.rh >> 3;
                            //tmp.g = pixel.gh >> 3;
                            //tmp.b = pixel.bh >> 3;
                            //tmp.a = alpha_bit && static_cast<bool>(pixel.a);
                            palette_data += tmp.c2;
                            palette_data += tmp.c1;
                        }

                        break;
                    }

                    case 24:
                    {
                        palette_data.reserve(_colormap_length * 3);
                        for (const auto &pixel : palette)
                        {
                            palette_data += pixel.c5;
                            palette_data += pixel.c3;
                            palette_data += pixel.c1;
                        }

                        break;
                    }

                    case 32:
                    {
                        palette_data.reserve(_colormap_length * 4);
                        for (const auto &pixel : palette)
                        {
                            palette_data += pixel.c5;
                            palette_data += pixel.c3;
                            palette_data += pixel.c1;
                            palette_data += pixel.c7;
                        }

                        break;
                    }
                }
                ofile.write(palette_data.c_str(), palette_data.size());
            }

            // image data
            ofile.write(data.c_str(), data.size());

            // footer
            if (_version2)
                ofile.write("\0\0\0\0\0\0\0\0TRUEVISION-XFILE.\0", 26);

            ofile.close();

            return true;
        }

        return false;
    }

    bool Targa::load(const std::string &filename)
    {
    }

    //--- static public methods ---

    bool Targa::identify(const std::string &filename)
    {
        // XXX: implement a proper check
        if (std::ifstream ifile(filename); ifile.is_open() && ifile.good())
        {
            const size_t size = ifile.seekg(0, std::ios::end).tellg();

            if (size < sizeof (Header))
            {
                ifile.close();
                return false;
            }

            return true;
        }

        return false;
    }

    //--- protected methods ---

    std::string Targa::genMappedData(Pixels &palette, const Pixels &pixels) const
    {
        // max palette size is 8192 bytes, so it could be 2048 32bit colors or 4096 15/16bit colors
        // but I never came across a Targa with more the 256 mapped colors
        const size_t MaxColor = 256;
        Pixels out;
        std::string data;

        if (Q::middleCut(width(), height(), MaxColor, pixels, out, palette))
        {
            std::map<uint64_t, size_t> mapping;

            if (pixels.size() != out.size())
                return data;
            if (palette.size() != MaxColor)
                return data;

            for (size_t i = 0; i < palette.size(); ++i)
                mapping[palette[i].value] = i;

            data.reserve(out.size());
            for (const auto &pixel : out)
                data += static_cast<char>(mapping[pixel.value]);
        }

        return data;
    }

    std::string Targa::genTruecolorData(const Pixels &pixels) const
    {
        std::string data;

        switch (_depth)
        {
            case 15:
            {
                C::UnionRGBA5551 tmp;

                data.reserve(pixels.size() * 2);
                for (const auto &pixel : pixels)
                {
                    tmp.r = pixel.rh >> 3;
                    tmp.g = pixel.gh >> 3;
                    tmp.b = pixel.bh >> 3;
                    tmp.a = 0;
                    data += tmp.c2;
                    data += tmp.c1;
                }

                break;
            }

            case 16:
            {
                C::UnionRGBA5551 tmp;

                data.reserve(pixels.size() * 2);
                for (const auto &pixel : pixels)
                {
                    tmp.r = pixel.rh >> 3;
                    tmp.g = pixel.gh >> 3;
                    tmp.b = pixel.bh >> 3;
                    tmp.a = pixel.ah ? 1 : 0;
                    data += tmp.c2;
                    data += tmp.c1;
                }

                break;
            }

            case 24:
            {
                data.reserve(pixels.size() * 3);
                for (const auto &pixel : pixels)
                {
                    data += pixel.c5;
                    data += pixel.c3;
                    data += pixel.c1;
                }

                break;
            }

            case 32:
            {
                // Targa actually is BGRA/BGR
                data.reserve(pixels.size() * 4);
                for (const auto &pixel : pixels)
                {
                    data += pixel.c5;
                    data += pixel.c3;
                    data += pixel.c1;
                    data += pixel.c7;
                }

                break;
            }
        }

        return data;
    }

    std::string Targa::genMonoData(const Pixels &pixels) const
    {
        std::string data;

        // Targa actually uses bytes for mono data, which is a huge waste of space, but it makes
        // generating the pixel data quite easy
        for (const auto &pixel : pixels)
            data += (pixel.averageRGB() > 0x7FFF) ? 0xFF : 0x0;

        return data;
    }

    std::string Targa::genMappedRleData(Pixels &palette, const Pixels &pixels) const
    {
        const size_t MaxColors = 256;
        Pixels out;
        std::string data;

        if (Color::Quantize::middleCut(width(), height(), MaxColors, pixels, out, palette))
        {
            const size_t size = out.size();
            std::map<uint64_t, size_t> mapping;
            std::string tmp(size, '\0');
            std::vector<uint8_t> buffer;
            size_t count = 0;

            if (pixels.size() != out.size())
                return data;
            if (palette.size() != MaxColors)
                return data;

            for (size_t i = 0; i < palette.size(); ++i)
                mapping[palette[i].value] = i;
            for (size_t i = 0; i < out.size(); ++i)
                tmp[i] = static_cast<char>(mapping[out[i].value]);

            if (_version2)
            {
                for (int64_t l = 0; l < height(); ++l)
                {
                    const size_t lsize = width();
                    const size_t ppos = l * lsize;
                    const std::string pline(tmp.begin() + ppos, tmp.begin() + ppos + lsize);

                    for (size_t i = 0; i < lsize; ++i)
                    {
                        count = 1;
                        while ((i < (lsize - 1)) && (count < 128) && (pline[i] == pline[i + 1]))
                        {
                            ++count;
                            ++i;
                        }

                        if ((count > 1) || (buffer.size() >= 128))
                        {
                            if (!buffer.empty())
                            {
                                data.push_back(static_cast<uint8_t>(buffer.size() - 1));
                                data.insert(data.end(), buffer.begin(), buffer.end());
                                buffer.clear();
                            }
                            data.push_back(static_cast<uint8_t>((count - 1) | 128));
                            data += pline[i];
                        }
                        else
                            buffer.push_back(pline[i]);
                    }

                    if (!buffer.empty())
                    {
                        data.push_back(static_cast<uint8_t>(buffer.size() - 1));
                        data.insert(data.end(), buffer.begin(), buffer.end());
                        buffer.clear();
                    }
                }
            }
            else
            {
                for (size_t i = 0; i < size; ++i)
                {
                    count = 1;
                    while ((i < (size - 1)) && (count < 128) && (tmp[i] == tmp[i + 1]))
                    {
                        ++count;
                        ++i;
                    }

                    if ((count > 1) || (buffer.size() >= 128))
                    {
                        if (!buffer.empty())
                        {
                            data.push_back(static_cast<uint8_t>(buffer.size() - 1));
                            data.insert(data.end(), buffer.begin(), buffer.end());
                            buffer.clear();
                        }
                        data.push_back(static_cast<uint8_t>((count - 1) | 128));
                        data += tmp[i];
                    }
                    else
                        buffer.push_back(tmp[i]);
                }

                if (!buffer.empty())
                {
                    data.push_back(static_cast<uint8_t>(buffer.size() - 1));
                    data.insert(data.end(), buffer.begin(), buffer.end());
                }
            }
        }

        return data;
    }

    std::string Targa::genTruecolorRleData(const Pixels &pixels) const
    {
        const size_t size = pixels.size();
        const size_t psize = (_depth + 1) / 8;
        std::vector<uint8_t> buffer;
        std::string data;
        size_t count = 0;

        switch (_depth)
        {
            case 15:
            case 16:
            {
                const bool alpha_bit = _image_descriptor & 0x01;
                auto pushBytes = [&](auto &container, const auto &pixel)
                {
                    E::Union16 tmp;

                    tmp.u = 0;
                    tmp.u |= (pixel.rh >> 3) << 10;
                    tmp.u |= (pixel.gh >> 3) << 5;
                    tmp.u |= (pixel.bh >> 3);
                    tmp.u |= (alpha_bit && static_cast<const bool>(pixel.a)) << 15;

                    container.push_back(tmp.c1);
                    container.push_back(tmp.c2);
                };

                if (_version2)
                {
                    for (int64_t l = 0; l < height(); ++l)
                    {
                        const size_t lsize = width();
                        const size_t ppos = l * lsize;
                        const Pixels pline(pixels.begin() + ppos, pixels.begin() + ppos + lsize);

                        for (size_t i = 0; i < lsize; ++i)
                        {
                            count = 1;
                            while ((i < (lsize - 1)) && (count < 128) && (pline[i] == pline[i + 1]))
                            {
                                ++count;
                                ++i;
                            }

                            if ((count > 1) || (buffer.size() >= (128 * psize)))
                            {
                                if (!buffer.empty())
                                {
                                    data.push_back(static_cast<uint8_t>(buffer.size() / psize - 1));
                                    data.insert(data.end(), buffer.begin(), buffer.end());
                                    buffer.clear();
                                }
                                data.push_back(static_cast<uint8_t>((count - 1) | 128));
                                pushBytes(data, pline[i]);
                            }
                            else
                                pushBytes(buffer, pline[i]);
                        }

                        if (!buffer.empty())
                        {
                            data.push_back(static_cast<uint8_t>(buffer.size() / psize - 1));
                            data.insert(data.end(), buffer.begin(), buffer.end());
                            buffer.clear();
                        }
                    }
                }
                else
                {
                    for (size_t i = 0; i < size; ++i)
                    {
                        count = 1;
                        while ((i < (size - 1)) && (count < 128) && (pixels[i] == pixels[i + 1]))
                        {
                            ++count;
                            ++i;
                        }

                        if ((count > 1) || (buffer.size() >= (128 * psize)))
                        {
                            if (!buffer.empty())
                            {
                                data.push_back(static_cast<uint8_t>(buffer.size() / psize - 1));
                                data.insert(data.end(), buffer.begin(), buffer.end());
                                buffer.clear();
                            }
                            data.push_back(static_cast<uint8_t>((count - 1) | 128));
                            pushBytes(data, pixels[i]);
                        }
                        else
                            pushBytes(buffer, pixels[i]);
                    }

                    if (!buffer.empty())
                    {
                        data.push_back(static_cast<uint8_t>(buffer.size() / psize - 1));
                        data.insert(data.end(), buffer.begin(), buffer.end());
                        buffer.clear();
                    }
                }

                break;
            }

            case 24:
            {
                auto pushBytes = [&](auto &container, const auto &pixel)
                {
                    container.push_back(pixel.c5);
                    container.push_back(pixel.c3);
                    container.push_back(pixel.c1);
                };

                if (_version2)
                {
                    for (int64_t l = 0; l < height(); ++l)
                    {
                        const size_t lsize = width();
                        const size_t ppos = l * lsize;
                        const Pixels pline(pixels.begin() + ppos, pixels.begin() + ppos + lsize);

                        for (size_t i = 0; i < lsize; ++i)
                        {
                            count = 1;
                            while ((i < (lsize - 1)) && (count < 128) && (pline[i] == pline[i + 1]))
                            {
                                ++count;
                                ++i;
                            }

                            if ((count > 1) || (buffer.size() >= (128 * psize)))
                            {
                                if (!buffer.empty())
                                {
                                    data.push_back(static_cast<uint8_t>(buffer.size() / psize - 1));
                                    data.insert(data.end(), buffer.begin(), buffer.end());
                                    buffer.clear();
                                }
                                data.push_back(static_cast<uint8_t>((count - 1) | 128));
                                pushBytes(data, pline[i]);
                            }
                            else
                                pushBytes(buffer, pline[i]);
                        }

                        if (!buffer.empty())
                        {
                            data.push_back(static_cast<uint8_t>(buffer.size() / psize - 1));
                            data.insert(data.end(), buffer.begin(), buffer.end());
                            buffer.clear();
                        }
                    }
                }
                else
                {
                    for (size_t i = 0; i < size; ++i)
                    {
                        count = 1;
                        while ((i < (size - 1)) && (count < 128) && (pixels[i] == pixels[i + 1]))
                        {
                            ++count;
                            ++i;
                        }

                        if ((count > 1) || (buffer.size() >= (128 * psize)))
                        {
                            if (!buffer.empty())
                            {
                                data.push_back(static_cast<uint8_t>(buffer.size() / psize - 1));
                                data.insert(data.end(), buffer.begin(), buffer.end());
                                buffer.clear();
                            }
                            data.push_back(static_cast<uint8_t>((count - 1) | 128));
                            pushBytes(data, pixels[i]);
                        }
                        else
                            pushBytes(buffer, pixels[i]);
                    }

                    if (!buffer.empty())
                    {
                        data.push_back(static_cast<uint8_t>(buffer.size() / psize - 1));
                        data.insert(data.end(), buffer.begin(), buffer.end());
                        buffer.clear();
                    }
                }

                break;
            }

            case 32:
            {
                auto pushBytes = [&](auto &container, const auto &pixel)
                {
                    container.push_back(pixel.c5);
                    container.push_back(pixel.c3);
                    container.push_back(pixel.c1);
                    container.push_back(pixel.c7);
                };

                if (_version2)
                {
                    for (int64_t l = 0; l < height(); ++l)
                    {
                        const size_t lsize = width();
                        const size_t ppos = l * lsize;
                        const Pixels pline(pixels.begin() + ppos, pixels.begin() + ppos + lsize);

                        for (size_t i = 0; i < lsize; ++i)
                        {
                            count = 1;
                            while ((i < (lsize - 1)) && (count < 128) && (pline[i] == pline[i + 1]))
                            {
                                ++count;
                                ++i;
                            }

                            if ((count > 1) || (buffer.size() >= (128 * psize)))
                            {
                                if (!buffer.empty())
                                {
                                    data.push_back(static_cast<uint8_t>(buffer.size() / psize - 1));
                                    data.insert(data.end(), buffer.begin(), buffer.end());
                                    buffer.clear();
                                }
                                data.push_back(static_cast<uint8_t>((count - 1) | 128));
                                pushBytes(data, pline[i]);
                            }
                            else
                                pushBytes(buffer, pline[i]);
                        }

                        if (!buffer.empty())
                        {
                            data.push_back(static_cast<uint8_t>(buffer.size() / psize - 1));
                            data.insert(data.end(), buffer.begin(), buffer.end());
                            buffer.clear();
                        }
                    }
                }
                else
                {
                    for (size_t i = 0; i < size; ++i)
                    {
                        count = 1;
                        while ((i < (size - 1)) && (count < 128) && (pixels[i] == pixels[i + 1]))
                        {
                            ++count;
                            ++i;
                        }

                        if ((count > 1) || (buffer.size() >= (128 * psize)))
                        {
                            if (!buffer.empty())
                            {
                                data.push_back(static_cast<uint8_t>(buffer.size() / psize - 1));
                                data.insert(data.end(), buffer.begin(), buffer.end());
                                buffer.clear();
                            }
                            data.push_back(static_cast<uint8_t>((count - 1) | 128));
                            pushBytes(data, pixels[i]);
                        }
                        else
                            pushBytes(buffer, pixels[i]);
                    }

                    if (!buffer.empty())
                    {
                        data.push_back(static_cast<uint8_t>(buffer.size() / psize - 1));
                        data.insert(data.end(), buffer.begin(), buffer.end());
                        buffer.clear();
                    }
                }

                break;
            }
        }

        return data;
    }

    std::string Targa::genMonoRleData(const Pixels &pixels) const
    {
        const size_t size = pixels.size();
        std::vector<uint8_t> buffer;
        std::string tmp(size, '\0');
        std::string data;
        size_t count = 0;

        for (size_t i = 0; i < size; ++i)
            tmp[i] = (pixels[i].averageRGB() > 0x7FFF) ? 0xFF : 0x0;

        if (_version2)
        {
            for (int64_t l = 0; l < height(); ++l)
            {
                const size_t lsize = width();
                const size_t ppos = l * lsize;
                const std::string pline(tmp.begin() + ppos, tmp.begin() + ppos + lsize);

                for (size_t i = 0; i < lsize; ++i)
                {
                    count = 1;
                    while ((i < (lsize - 1)) && (count < 128) && (pline[i] == pline[i + 1]))
                    {
                        ++count;
                        ++i;
                    }

                    if ((count > 1) || (buffer.size() >= 128))
                    {
                        if (!buffer.empty())
                        {
                            data.push_back(static_cast<uint8_t>(buffer.size()) - 1);
                            data.insert(data.end(), buffer.begin(), buffer.end());
                            buffer.clear();
                        }
                        data.push_back(static_cast<uint8_t>((count - 1) | 128));
                        data += pline[i];
                    }
                    else
                        buffer.push_back(pline[i]);
                }

                if (!buffer.empty())
                {
                    data.push_back(static_cast<uint8_t>(buffer.size()) - 1);
                    data.insert(data.end(), buffer.begin(), buffer.end());
                    buffer.clear();
                }
            }
        }
        else
        {
            for (size_t i = 0; i < size; ++i)
            {
                count = 1;
                while ((i < (size - 1)) && (count < 128) && (tmp[i] == tmp[i + 1]))
                {
                    ++count;
                    ++i;
                }

                if ((count > 1) || (buffer.size() >= 128))
                {
                    if (!buffer.empty())
                    {
                        data.push_back(static_cast<uint8_t>(buffer.size()) - 1);
                        data.insert(data.end(), buffer.begin(), buffer.end());
                        buffer.clear();
                    }
                    data.push_back(static_cast<uint8_t>((count - 1) | 128));
                    data += tmp[i];
                }
                else
                    buffer.push_back(tmp[i]);
            }

            if (!buffer.empty())
            {
                data.push_back(static_cast<uint8_t>(buffer.size()) - 1);
                data.insert(data.end(), buffer.begin(), buffer.end());
                buffer.clear();
            }
        }

        return data;
    }
}
