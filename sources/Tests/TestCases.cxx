#include "TestCases.hxx"

namespace TestCases
{
    bool pictureTest01(Image::Base &pic)
    {
        if ((pic.width() > 320) && (pic.height() > 240))
        {
            using Color = Image::Base::RGBA;
            const int64_t w = pic.width();
            const int64_t h = pic.height();
            const int64_t hw = w >> 1;
            const int64_t hh = h >> 1;
            const int64_t tw = (w >> 2) * 3;
            const int64_t h8 = h >> 3;

            pic.setRectangle(0, 0, hw, hh, Color::Red, true);
            pic.setRectangle(hw, 0, w - 1, hh, Color::Green, true);
            pic.setRectangle(0, hh, hw, h - 1, Color::Blue, true);
            pic.setRectangle(hw, hh, w - 1, h - 1, Color::White, true);

            for (int64_t i = 0, part = std::min(w / 4 - 2, h / 4 - 2); i < part; i += 2)
            {
                const int64_t comp = 0xffff / part * i;

                pic.setCircle(hw / 2, hh / 2, std::min(w / 4 - i - 2, h / 4 - i - 2),
                              Color::Black, false);
                pic.setCircle(hw / 2 * 3, hh / 2, std::min(w / 4 - i - 2, h / 4 - i - 2),
                              Color(0, 0xffff - comp, 0, 0xffff), true);
                pic.setCircle(hw / 2, hh / 2 * 3, std::min(w / 4 - i - 2, h / 4 - i - 2),
                              Color(comp, comp, comp, 0xffff), true);
            }

            for (int64_t i = 0; i < (std::min(hw / 4, hh) - 2); i += 2)
                pic.setRectangle(hw + i + 2, hh + i + 2, tw - i - 2, h - i - 3, Color::Black, false);

            pic.setRectangle(tw + 2, hh + 2, w - 3, h - 3, Color::Black, false);
            for (int64_t i = 0, part = (hw / 2 - 8); i < part; i += 2)
            {
                const int64_t comp = 0xffff / part * i;

                pic.setLine(tw + i + 4, h8 * 4 + 4, tw + i + 4, h8 * 5 - 2,
                            Color(comp, comp, comp, 0xffff));
                pic.setLine(tw + i + 4, h8 * 5 + 1, tw + i + 4, h8 * 6 - 2,
                            Color(comp, 0, 0, 0xffff));
                pic.setLine(tw + i + 4, h8 * 6 + 1, tw + i + 4, h8 * 7 - 2,
                            Color(0, comp, 0, 0xffff));
                pic.setLine(tw + i + 4, h8 * 7 + 1, tw + i + 4, h - 5, Color(0, 0, comp, 0xffff));
            }

            return true;
        }

        return false;
    }
}
