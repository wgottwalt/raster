#include <iostream>
#include <X11/Xutil.h>
#include "Image/PPM.hxx"

const int64_t width = 800;
const int64_t height = 600;

int32_t main(int32_t argc, char **argv)
{
    Image::PPM ppm(width, height, Image::PPM::RGBA::Black);
    Display *display = XOpenDisplay(nullptr);
    Visual *visual = DefaultVisual(display, 0);
    Window window = XCreateSimpleWindow(display, RootWindow(display, 0), 0, 0, ppm.width(),
                                        ppm.height(), 1, 0, 0);
    int32_t loop = 0;
    const uint16_t part = std::numeric_limits<uint16_t>::max() / height / 2;

    for (int64_t i = 0; i < (height / 2); ++i)
        ppm.setLine(0, i * 2, (width - 1), i * 2, Image::PPM::RGBA(i * part, i * part, i * part, 0xffff));
    for (int64_t i = 0; i < 30; ++++++i)
        ppm.setTriangle(width / 2, i, i, height - 1 - i, width - 1 - i, height - 1 - i, Image::PPM::RGBA::Red, false);
    ppm.setTriangle(width / 2, 35, 35, height - 36, width - 36, height - 36, Image::PPM::RGBA::Blue, true);
    ppm.setRectangle(200, 200, width - 200, height - 200, Image::PPM::RGBA::Green, true);
    ppm.setRectangle(210, 210, width - 210, height - 210, Image::PPM::RGBA::Black, false);
    ppm.setCircle(width / 2, height / 2, (std::min(width, height) / 2) - (std::min(width, height) / 3), Image::PPM::RGBA::White, false);
    ppm.setCircle(width / 2, height / 2, (std::min(width, height) / 2) - (std::min(width, height) / 3) - 5, Image::PPM::RGBA::White, true);

    if (argc == 2)
    {
        ppm.setComment("created by " + std::string(argv[0]));
        ppm.setWideMode();
        ppm.setBinaryMode();
        ppm.save(argv[1]);
    }

    XImage *ximage = ppm.cloneXImage(display, visual);

    XSelectInput(display, window, ButtonPressMask | ExposureMask);
    XMapWindow(display, window);

    while (loop < 10)
    {
        XEvent event;

        XNextEvent(display, &event);
        switch (event.type)
        {
            case Expose:
                XPutImage(display, window, DefaultGC(display, 0), ximage, 0, 0, 0, 0, ppm.width(),
                          ppm.height());
                break;

            case ButtonPress:
                XDestroyImage(ximage);
                ppm.filter(Image::Filter::Smooth);
                ppm.flipHorizontal();
                ppm.resize(width - (loop + 1) * 50, height - (loop + 1) * 50, Image::Scaler::FastBillinear);
                ximage = ppm.cloneXImage(display, visual);
                XPutImage(display, window, DefaultGC(display, 0), ximage, 0, 0, 0, 0, ppm.width(),
                          ppm.height());
                ++loop;
                break;

            default:
                break;
        }
    }

    XDestroyWindow(display, window);
    XDestroyImage(ximage);
    XCloseDisplay(display);

    return 0;
}
