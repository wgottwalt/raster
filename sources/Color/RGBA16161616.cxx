#include <limits>
#include <utility>
#include "RGBA8888.hxx"
#include "RGBA16161616.hxx"

namespace Color
{
    //--- internal stuff ---

    static const uint16_t Max = std::numeric_limits<uint16_t>::max();

    //--- public constructors ---

    RGBA16161616::RGBA16161616() noexcept
#if defined __ORDER_LITTLE_ENDIAN__
    : a(Max), b(0), g(0), r(0)
#elif defined __ORDER_BIG_ENDIAN__
    : r(0), g(0), b(0), a(Max)
#else
#error "fucked up endianness"
#endif
    {
    }

    RGBA16161616::RGBA16161616(const uint64_t val) noexcept
    : value(val)
    {
    }

    RGBA16161616::RGBA16161616(const uint16_t rr, const uint16_t gg, const uint16_t bb,
                               const uint16_t aa) noexcept
#if defined __ORDER_LITTLE_ENDIAN__
    : a(aa), b(bb), g(gg), r(rr)
#elif defined __ORDER_BIG_ENDIAN__
    : r(rr), g(gg), b(bb), a(aa)
#else
#error "fucked up endianness"
#endif
    {
    }

    RGBA16161616::RGBA16161616(const uint8_t rrh, const uint8_t rrl, const uint8_t ggh,
                               const uint8_t ggl, const uint8_t bbh, const uint8_t bbl,
                               const uint8_t aah, const uint8_t aal) noexcept
#if defined __ORDER_LITTLE_ENDIAN__
    : al(aal), ah(aah), bl(bbl), bh(bbh), gl(ggl), gh(ggh), rl(rrl), rh(rrh)
#elif defined __ORDER_BIG_ENDIAN__
    : rh(rrh), rl(rrl), gh(ggh), gl(ggl), bh(bbh), bl(bbl), ah(aah), al(aal)
#else
#error "fucked up endianness"
#endif
    {
    }

    RGBA16161616::RGBA16161616(const RGBA16161616 &rhs) noexcept
    : value(rhs.value)
    {
    }

    RGBA16161616::RGBA16161616(RGBA16161616 &&rhs) noexcept
    : value(std::move(rhs.value))
    {
    }

    RGBA16161616::~RGBA16161616() noexcept
    {
    }

    //--- public operators ---

    RGBA16161616 &RGBA16161616::operator=(const RGBA16161616 &rhs) noexcept
    {
        value = rhs.value;

        return *this;
    }

    RGBA16161616 &RGBA16161616::operator=(RGBA16161616 &&rhs) noexcept
    {
        value = std::move(rhs.value);

        return *this;
    }

    bool RGBA16161616::operator==(const RGBA16161616 &rhs) const noexcept
    {
        return value == rhs.value;
    }

    bool RGBA16161616::operator!=(const RGBA16161616 &rhs) const noexcept
    {
        return value != rhs.value;
    }

    RGBA16161616 RGBA16161616::operator>>(const uint8_t val) const noexcept
    {
        return {static_cast<uint16_t>(r >> val), static_cast<uint16_t>(g >> val),
                static_cast<uint16_t>(b >> val), static_cast<uint16_t>(a >> val)};
    }

    RGBA16161616 RGBA16161616::operator<<(const uint8_t val) const noexcept
    {
        return {static_cast<uint16_t>(r << val), static_cast<uint16_t>(g << val),
                static_cast<uint16_t>(b << val), static_cast<uint16_t>(a << val)};
    }

    //--- public methods ---

    RGBA16161616 &RGBA16161616::set(const uint64_t val) noexcept
    {
        value = val;

        return *this;
    }

    RGBA16161616 &RGBA16161616::set(const uint16_t rr, const uint16_t gg, const uint16_t bb,
                                    const uint16_t aa) noexcept
    {
        r = rr;
        g = gg;
        b = bb;
        a = aa;

        return *this;
    }

    RGBA16161616 &RGBA16161616::set(const uint8_t rrh, const uint8_t rrl, const uint8_t ggh,
                                    const uint8_t ggl, const uint8_t bbh, const uint8_t bbl,
                                    const uint8_t aah, const uint8_t aal) noexcept
    {
        rh = rrh;
        rl = rrl;
        gh = ggh;
        gl = ggl;
        bh = bbh;
        bl = bbl;
        ah = aah;
        al = aal;

        return *this;
    }

    uint16_t RGBA16161616::averageRGB() const noexcept
    {
        return (static_cast<uint32_t>(r) + static_cast<uint32_t>(g) + static_cast<uint32_t>(b)) / 3;
    }

    uint16_t RGBA16161616::averageRGBA() const noexcept
    {
        return (static_cast<uint32_t>(r) + static_cast<uint32_t>(g) + static_cast<uint32_t>(b) +
                static_cast<uint32_t>(a)) / 4;
    }

    uint16_t RGBA16161616::grey() const noexcept
    {
        return 0.299083 * r + 0.585841 * g + 0.114076 * b;
    }

    RGBA8888 RGBA16161616::toRGBA8888() const noexcept
    {
        return {static_cast<uint8_t>(r >> 8), static_cast<uint8_t>(g >> 8),
                static_cast<uint8_t>(b >> 8), static_cast<uint8_t>(a >> 8)};
    }

    std::string RGBA16161616::str() const noexcept
    {
        return std::to_string(r) + ", " + std::to_string(g) + ", " +
               std::to_string(b) + ", " + std::to_string(a);
    }
}
