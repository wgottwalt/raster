#include <limits>
#include <utility>
#include "RGBA16161616.hxx"
#include "RGBA8888.hxx"

namespace Color
{
    //--- internal stuff ---

    static const uint16_t Max = std::numeric_limits<uint8_t>::max();

    //--- public constructors ---

    RGBA8888::RGBA8888() noexcept
#if defined __ORDER_LITTLE_ENDIAN__
    : a(Max), b(0), g(0), r(0)
#elif defined __ORDER_BIG_ENDIAN__
    : r(0), g(0), b(0), a(Max)
#else
#error "fucked up endianness"
#endif
    {
    }

    RGBA8888::RGBA8888(const uint32_t val) noexcept
    : value(val)
    {
    }

    RGBA8888::RGBA8888(const uint8_t rr, const uint8_t gg, const uint8_t bb,
                       const uint8_t aa) noexcept
#if defined __ORDER_LITTLE_ENDIAN__
    : a(aa), b(bb), g(gg), r(rr)
#elif defined __ORDER_BIG_ENDIAN__
    : r(rr), g(gg), b(bb), a(aa)
#else
#error "fucked up endianness"
#endif
    {
    }

    RGBA8888::RGBA8888(const RGBA8888 &rhs) noexcept
    : value(rhs.value)
    {
    }

    RGBA8888::RGBA8888(RGBA8888 &&rhs) noexcept
    : value(std::move(rhs.value))
    {
    }

    RGBA8888::~RGBA8888() noexcept
    {
    }

    //--- public operators ---

    RGBA8888 &RGBA8888::operator=(const RGBA8888 &rhs) noexcept
    {
        value = rhs.value;

        return *this;
    }

    RGBA8888 &RGBA8888::operator=(RGBA8888 &&rhs) noexcept
    {
        value = std::move(rhs.value);

        return *this;
    }

    bool RGBA8888::operator==(const RGBA8888 &rhs) const noexcept
    {
        return value == rhs.value;
    }

    bool RGBA8888::operator!=(const RGBA8888 &rhs) const noexcept
    {
        return value != rhs.value;
    }

    RGBA8888 RGBA8888::operator>>(const uint8_t val) const noexcept
    {
        return {static_cast<uint8_t>(r >> val), static_cast<uint8_t>(g >> val),
                static_cast<uint8_t>(b >> val), static_cast<uint8_t>(a >> val)};
    }

    RGBA8888 RGBA8888::operator<<(const uint8_t val) const noexcept
    {
        return {static_cast<uint8_t>(r << val), static_cast<uint8_t>(g << val),
                static_cast<uint8_t>(b << val), static_cast<uint8_t>(a << val)};
    }

    //--- public methods ---

    RGBA8888 &RGBA8888::set(const uint32_t val) noexcept
    {
        value = val;

        return *this;
    }

    RGBA8888 &RGBA8888::set(const uint8_t rr, const uint8_t gg, const uint8_t bb,
                            const uint8_t aa) noexcept
    {
        r = rr;
        g = gg;
        b = bb;
        a = aa;

        return *this;
    }

    uint8_t RGBA8888::averageRGB() const noexcept
    {
        return (static_cast<uint32_t>(r) + static_cast<uint32_t>(g) + static_cast<uint32_t>(b)) / 3;
    }

    uint8_t RGBA8888::averageRGBA() const noexcept
    {
        return (static_cast<uint32_t>(r) + static_cast<uint32_t>(g) + static_cast<uint32_t>(b) +
                static_cast<uint32_t>(a)) / 4;
    }

    uint8_t RGBA8888::grey() const noexcept
    {
        return 0.299083 * r + 0.585841 * g + 0.114076 * b;
    }

    RGBA16161616 RGBA8888::toRGBA16161616() const noexcept
    {
        return {static_cast<uint16_t>(r << 8), static_cast<uint16_t>(g << 8),
                static_cast<uint16_t>(b << 8), static_cast<uint16_t>(a << 8)};
    }
}
