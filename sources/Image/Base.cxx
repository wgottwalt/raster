#include <algorithm>
#include <map>
#include <tuple>
#include <utility>
#include "Color/MiddleCutQuantizer.hxx"
#include "Common/Tools.hxx"
#include "Math/Vector2.hxx"
#include "Base.hxx"

namespace Image
{
    //--- internal stuff ---

    namespace T = Common::Tools;
    using V2d = Math::Vector2<double>;

    constexpr inline std::tuple<double,double,double,double>
    boundingBox(const V2d &p1, const V2d &p2, const V2d &p3)
    {
        return {T::min(p1.x, p2.x, p3.x), T::min(p1.y, p2.y, p3.y),
                T::max(p1.x, p2.x, p3.x), T::max(p1.y, p2.y, p3.y)};
    }

    //--- public constructors ---

    Base::Base() noexcept
    : _data(), _width(0), _height(0)
    {
    }

    Base::Base(const int64_t width, const int64_t height, const RGBA color) noexcept(false)
    : _data(width * height, color), _width(width), _height(height)
    {
    }

    Base::Base(const Pixels &pixels, const int64_t width, const int64_t height) noexcept(false)
    : _data(pixels), _width(width), _height(height)
    {
    }

    Base::Base(const Base &rhs) noexcept(false)
    : _data(rhs._data), _width(rhs._width), _height(rhs._height)
    {
    }

    Base::Base(Base &&rhs) noexcept
    : _data(std::move(rhs._data)), _width(std::move(rhs._width)), _height(std::move(rhs._height))
    {
    }

    Base::~Base() noexcept
    {
    }

    //--- public operators ---

    Base &Base::operator=(const Base &rhs) noexcept(false)
    {
        if (this != &rhs)
        {
            _data = rhs._data;
            _width = rhs._width;
            _height = rhs._height;
        }

        return *this;
    }

    Base &Base::operator=(Base &&rhs) noexcept
    {
        if (this != &rhs)
        {
            _data = std::move(rhs._data);
            _width = std::move(rhs._width);
            _height = std::move(rhs._height);
        }

        return *this;
    }

    bool Base::operator==(const Base &rhs) const noexcept
    {
        return _data == rhs._data &&
               _width == rhs._width &&
               _height == rhs._height;
    }

    bool Base::operator!=(const Base &rhs) const noexcept
    {
        return !(*this == rhs);
    }

    //--- public methods ---

    int64_t Base::width() const noexcept
    {
        return _width;
    }

    int64_t Base::height() const noexcept
    {
        return _height;
    }

    const Base::Pixels &Base::pixels() const noexcept
    {
        return _data;
    }

    int64_t Base::usedColors() const noexcept(false)
    {
        std::map<uint64_t, uint64_t> color_map;

        for (auto &pixel : _data)
            ++color_map[pixel.value];

        return color_map.size();
    }

    void Base::flipVertical() noexcept(false)
    {
        for (int64_t row = 0; row < _height; ++row)
            std::reverse(_data.begin() + (row * _width), _data.begin() + (row * _width + _width));
    }

    void Base::flipHorizontal() noexcept(false)
    {
        for (int64_t row = 0; row < _height; ++row)
            std::reverse(_data.begin() + (row * _width), _data.begin() + (row * _width + _width));
        std::reverse(_data.begin(), _data.end());
    }

    bool Base::filter(const Filter filter) noexcept(false)
    {
        if ((_width > 2) && (_height > 2))
        {
            const Pixels pixels = implFilter(_data, _width, _height, filter);

            implReplace(pixels, _width, _height);

            return true;
        }

        return false;
    }

    bool Base::reduceColors(const int64_t colors, const Quantizer quantizer) noexcept(false)
    {
        Pixels out_pixels;
        Pixels palette;
        bool result = false;

        switch (quantizer)
        {
            case Quantizer::MiddleCut:
                result = Color::Quantize::middleCut<RGBA>(_width, _height, colors, _data,
                                                          out_pixels, palette);
        }

        if (result)
            implReplace(out_pixels, _width, _height);

        return result;
    }

    Base::RGBA Base::pixel(const int64_t x, const int64_t y) const noexcept(false)
    {
        if (T::inRange(x, 0, width()) && T::inRange(y, 0, height()))
            return implPixel(x, y);
        else
            throw "POOF for now";
    }

    bool Base::setPixel(const int64_t x, const int64_t y, const RGBA color) noexcept
    {
        if (T::inRange(x, 0, width()) && T::inRange(y, 0, height()))
        {
            implSetPixel(x, y, color);
            return true;
        }

        return false;
    }

    bool Base::setLine(const int64_t x1, const int64_t y1, const int64_t x2, const int64_t y2,
                       const RGBA color) noexcept
    {
        if (T::inRange(x1, 0, width()) && T::inRange(y1, 0, height()) &&
          T::inRange(x2, 0, width()) && T::inRange(y2, 0, height()))
        {
            implSetLine(x1, y1, x2, y2, color);
            return true;
        }

        return false;
    }

    bool Base::setTriangle(const int64_t x1, const int64_t y1, const int64_t x2, const int64_t y2,
                           const int64_t x3, const int64_t y3, const RGBA color, const bool fill)
        noexcept
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

    bool Base::setRectangle(const int64_t x1, const int64_t y1, const int64_t x2, const int64_t y2,
                            const RGBA color, const bool fill) noexcept
    {
        if (T::inRange(x1, 0, width()) && T::inRange(y1, 0, height()) &&
          T::inRange(x2, 0, width()) && T::inRange(y2, 0, height()))
        {
            implSetRectangle(x1, y1, x2, y2, color, fill);
            return true;
        }

        return false;
    }

    bool Base::setCircle(const int64_t x, const int64_t y, const int64_t radius, const RGBA color,
                         const bool fill) noexcept
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

    XImage *Base::cloneXImage(Display *display, Visual *visual) const noexcept
    {
        if (!display)
        {
            display = XOpenDisplay(nullptr);
            visual = DefaultVisual(display, 0);
        }

        if (!display || !visual)
            return nullptr;

        // only support truecolor (24bit or 32bit) desktops for now
        if (visual->c_class == TrueColor)
        {
            const size_t size = _width * _height * sizeof (uint32_t);

            if (char *raw = new(std::nothrow) char[size]; raw)
            {
                size_t pos = 0;

                for (auto &pixel : _data)
                {
                    // XImage is bgra, also convert 16bit channel depth to 8bit
                    raw[pos++] = pixel.bh;
                    raw[pos++] = pixel.gh;
                    raw[pos++] = pixel.rh;
                    raw[pos++] = pixel.ah;
                }

                return XCreateImage(display, visual, 24, ZPixmap, 0, raw, _width, _height, 32, 0);
            }
        }

        return nullptr;
    }

    //--- protected methods ---

    Base::RGBA Base::implPixel(const int64_t x, const int64_t y) const noexcept
    {
        return _data[y * _width + x];
    }

    void Base::implSetPixel(const int64_t x, const int64_t y, const RGBA color) noexcept
    {
        _data[y * _width + x] = color;
    }

    void Base::implSetLine(int64_t x1, int64_t y1, int64_t x2, int64_t y2, const RGBA color)
        noexcept
    {
        if ((x1 != x2) || (y1 != y2))
        {
            const int64_t delta_x = std::abs(x2 - x1);
            const int64_t delta_y = -std::abs(y2 - y1);
            const int64_t switch_x = x1 < x2 ? 1 : -1;
            const int64_t switch_y = y1 < y2 ? 1 : -1;
            int64_t error_val = delta_x + delta_y;
            int64_t error2_val = 0;

            while (true)
            {
                _data[y1 * _width + x1] = color;
                if ((x1 == x2) && (y1 == y2))
                    break;

                error2_val = 2 * error_val;
                if (error2_val > delta_y)
                {
                    error_val += delta_y;
                    x1 += switch_x;
                }
                if (error2_val < delta_x)
                {
                    error_val += delta_x;
                    y1 += switch_y;
                }
            }
        }
        else
            _data[y1 * _width + x1] = color;
    }

    void Base::implSetTriangle(const int64_t x1, const int64_t y1, const int64_t x2,
                               const int64_t y2, const int64_t x3, const int64_t y3,
                               const RGBA color, const bool fill) noexcept
    {
        if (fill)
        {
            const V2d p1(x1, y1);
            const V2d p2(x2, y2);
            const V2d p3(x3, y3);
            const V2d p12(p2.x - p1.x, p2.y - p1.y);
            const V2d p13(p3.x - p1.x, p3.y - p1.y);
            const auto [minx, miny, maxx, maxy] = boundingBox(p1, p2, p3);
            const auto cross_tmp = Math::crossProduct(p12, p13);

            for (int64_t y = miny; y <= maxy; ++y)
            {
                for (int64_t x = minx; x <= maxx; ++x)
                {
                    const V2d v(x - p1.x, y - p1.y);
                    const double s = Math::crossProduct(v, p13) / cross_tmp;
                    const double t = Math::crossProduct(p12, v) / cross_tmp;

                    if ((s >= 0) && (t >= 0) && ((s + t) <= 1))
                        _data[y * _width + x] = color;
                }
            }
        }
        else
        {
            implSetLine(x1, y1, x2, y2, color);
            implSetLine(x1, y1, x3, y3, color);
            implSetLine(x2, y2, x3, y3, color);
        }
    }

    void Base::implSetRectangle(const int64_t x1, const int64_t y1, const int64_t x2,
                                const int64_t y2, const RGBA color, const bool fill) noexcept
    {
        const auto [xx1, xx2] = T::minMax(x1, x2);
        const auto [yy1, yy2] = T::minMax(y1, y2);

        if (fill)
        {
            int64_t ypos = 0;

            for (int64_t y = yy1; y < yy2; ++y)
            {
                ypos = y * _width;
                std::fill(_data.begin() + ypos + xx1, _data.begin() + ypos + xx2 + 1, color);
            }
        }
        else
        {
            int64_t ypos = yy1 * _width;

            std::fill(_data.begin() + ypos + xx1, _data.begin() + ypos + xx2 + 1, color);
            for (int64_t y = yy1; y <= yy2; ++y)
            {
                ypos = y * _width;
                _data[ypos + xx1] = color;
                _data[ypos + xx2] = color;
            }
            ypos = yy2 * _width;
            std::fill(_data.begin() + ypos + xx1, _data.begin() + ypos + xx2 + 1, color);
        }
    }

    void Base::implSetCircle(const int64_t x, const int64_t y, const int64_t radius,
                             const RGBA color, const bool fill) noexcept
    {
        const int64_t rad = std::abs(radius);

        if (((x + rad) > _width) || ((y + rad) > _height) || ((x - rad) < 0) || ((y - rad) < 0))
            return;

        if (radius != 0)
        {
            const int64_t ypos = y * _width;
            int64_t delta_ypos = 0;
            int64_t f = 1 - rad;
            int64_t delta_x = 0;
            int64_t delta_y = -2 * rad;
            int64_t xx = 0;
            int64_t yy = rad;

            _data[ypos + x + rad] = color;
            _data[ypos + x - rad] = color;
            if (!fill)
            {
                _data[ypos + (rad * _width) + x] = color;
                _data[ypos - (rad * _width) + x] = color;
            }
            else
                implSetLine(x - rad, y, x + rad, y, color);

            if (fill)
            {
                while (xx < yy)
                {
                    if (f >= 0)
                    {
                        --yy;
                        delta_y += 2;
                        f += delta_y;
                    }
                    ++xx;
                    delta_x += 2;
                    f += delta_x + 1;

                    implSetLine(x - xx, y + yy, x + xx, y + yy, color);
                    implSetLine(x - xx, y - yy, x + xx, y - yy, color);
                    implSetLine(x - yy, y + xx, x + yy, y + xx, color);
                    implSetLine(x - yy, y - xx, x + yy, y - xx, color);
                }
            }
            else
            {
                while (xx < yy)
                {
                    if (f >= 0)
                    {
                        --yy;
                        delta_y += 2;
                        f += delta_y;
                    }
                    ++xx;
                    delta_x += 2;
                    f += delta_x + 1;

                    delta_ypos = yy * _width;
                    _data[ypos + delta_ypos + x + xx] = color;
                    _data[ypos + delta_ypos + x - xx] = color;
                    _data[ypos - delta_ypos + x + xx] = color;
                    _data[ypos - delta_ypos + x - xx] = color;
                    delta_ypos = ypos + (xx * _width);
                    _data[delta_ypos + x + yy] = color;
                    _data[delta_ypos + x - yy] = color;
                    delta_ypos = ypos - (xx * _width);
                    _data[delta_ypos + x + yy] = color;
                    _data[delta_ypos + x - yy] = color;
                }
            }
        }
        else
            _data[y * _width + x] = color;
    }

    Base::Pixels Base::implFilter(const Pixels &pixels, const int64_t width, const int64_t height,
                                  const Filter filter) const noexcept(false)
    {
        Pixels result(width * height, RGBA::Black);
        std::vector<int64_t> indices = {-(width + 1), -width, -(width - 1),
                                        -1 , 0, 1,
                                        width - 1, width, width + 1};
        std::vector<float> kernel;
        RGBA pixel;
        int64_t pos = 0;
        int64_t int_r = 0;
        int64_t int_g = 0;
        int64_t int_b = 0;
        float factor = .0;
        float red = .0;
        float green = .0;
        float blue = .0;

        switch (filter)
        {
            case Filter::Smooth:
                kernel = {1, 1, 1,
                          1, 2, 1,
                          1, 1, 1};
                break;

            case Filter::Sharpen:
                kernel = {-1, -1, -1,
                          -1, 9, -1,
                          -1, -1, -1};
                break;

            case Filter::Edge:
                kernel = {-1, -1, -1,
                          -1, 8, -1,
                          -1, -1, -1};
                break;

            case Filter::Blur:
                kernel = {0, 0, 1,
                          0, 0, 0,
                          1, 0, 0};
                break;

            case Filter::Raised:
                kernel = {0, 0, -2,
                          0, 2, 0,
                          1, 0, 0};
                break;
        }

        for (auto k : kernel)
            factor += k;

        if (factor == .0)
            factor = 1.0;

        for (int64_t y = 1; y < (height - 1); ++y)
        {
            for (int64_t x = 1; x < (width - 1); ++x)
            {
                red = .0;
                green = .0;
                blue = .0;
                pos = y * width + x;

                for (size_t k = 0; k < kernel.size(); ++k)
                {
                    pixel = pixels[pos + indices[k]];
                    red += pixel.r * kernel[k];
                    green += pixel.g * kernel[k];
                    blue += pixel.b * kernel[k];
                }

                int_r = red / factor;
                int_g = green / factor;
                int_b = blue / factor;

                // clamping min and max values (internal rgba data is 16bit per channel)
                if (int_r > 0xFFFF)
                    int_r = 0xFFFF;
                else if (int_r < 0)
                    int_r = 0;

                if (int_g > 0xFFFF)
                    int_g = 0xFFFF;
                else if (int_g < 0)
                    int_g = 0;

                if (int_b > 0xFFFF)
                    int_b = 0xFFFF;
                else if (int_b < 0)
                    int_b = 0;

                result[pos].r = int_r;
                result[pos].g = int_g;
                result[pos].b = int_b;
                result[pos].a = pixels[pos].a;
            }
        }

        return result;
    }

    bool Base::implResize(const int64_t width, const int64_t height, const Scaler scaler)
        noexcept(false)
    {
        if ((width < 3) || (height < 3)) // scaling that low is useless
            return false;

        const int64_t min_width = std::min(_width, width);
        const int64_t min_height = std::min(_height, height);
        // to stay with integer math I need a little trick here, hope noone ever tries to
        // scale beyond 2^47 pixels :D
        const int64_t column_ratio = ((_width - 1) << 16) / width;
        const int64_t row_ratio = ((_height - 1) << 16) / height;
        Pixels pixels(width * height, RGBA::Black);

        switch (scaler)
        {
            case Scaler::FastBillinear:
            {
                for (int64_t row = 0; row < height; ++row)
                {
                    const int64_t p_row = (row * row_ratio) >> 16;
                    const int64_t pos = p_row * _width;

                    for (int64_t column = 0; column < width; ++column)
                    {
                        const int64_t p_column = (column * column_ratio) >> 16;
                        const RGBA p1 = _data[pos + p_column];
                        const RGBA p2 = _data[pos + p_column + 1];
                        const RGBA p3 = _data[pos + p_column + _width];
                        const RGBA p4 = _data[pos + p_column + _width + 1];

                        pixels[row * width + column].set((p1.r + p2.r + p3.r + p4.r) >> 2,
                                                         (p1.g + p2.g + p3.g + p4.g) >> 2,
                                                         (p1.b + p2.b + p3.b + p4.b) >> 2,
                                                         (p1.a + p2.a + p3.a + p4.a) >> 2);
                    }
                }

                break;
            }

            case Scaler::Nearest:
            {
                for (int64_t row = 0; row < height; ++row)
                {
                    const int64_t p_row = (row * row_ratio) >> 16;
                    const int64_t pos1 = row * width;
                    const int64_t pos2 = p_row * _width;

                    for (int64_t column = 0; column < width; ++column)
                    {
                        const int64_t p_column = (column * column_ratio) >> 16;
                        pixels[pos1 + column] = _data[pos2 + p_column];
                    }
                }

                break;
            }

            case Scaler::Keep:
                for (int64_t row = 0; row < min_height; ++row)
                    for (int64_t column = 0; column < min_width; ++column)
                        pixels[row * width + column] = _data[row * _width + column];
                break;

            case Scaler::Clear:
            default:
                break;
        }

        implReplace(pixels, width, height);

        return true;
    }

    void Base::implReplace(const Pixels &pixels, const int64_t width, const int64_t height) noexcept
    {
        _data = pixels;
        _width = width;
        _height = height;
    }

    void Base::implReplace(Pixels &&pixels, const int64_t width, const int64_t height) noexcept
    {
        _data = std::move(pixels);
        _width = width;
        _height = height;
    }
}
