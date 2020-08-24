#include <fstream>
#include <iostream>
#include <string>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "Common/Tools.hxx"
#include "Image/PPM.hxx"

namespace T = Common::Tools;
namespace I = Image;

void view(I::Base *base_image, std::string &title)
{
    Display *display = XOpenDisplay(nullptr);
    Visual *visual = DefaultVisual(display, 0);
    XImage *ximage = base_image->cloneXImage(display, visual);
    Window window = XCreateSimpleWindow(display, RootWindow(display, 0), 0, 0, base_image->width(),
                                        base_image->height(), 1, 0, 0);
    bool loop = true;

    title = "file: " + title;

    XSelectInput(display, window, ButtonPressMask | ExposureMask);
    XStoreName(display, window, title.c_str());
    XMapWindow(display, window);
    while (loop)
    {
        XEvent event;

        XNextEvent(display, &event);
        switch (event.type)
        {
            case Expose:
                XPutImage(display, window, DefaultGC(display, 0), ximage, 0, 0, 0, 0,
                          base_image->width(), base_image->height());
                break;

            case ButtonPress:
                loop = false;
                break;

            default:
                break;
        }
    }

    XDestroyWindow(display, window);
    XDestroyImage(ximage);
    XCloseDisplay(display);
}

void usage(const std::string &appname)
{
    std::cout << "usage: " << appname << " <image filename>" << std::endl;
}

int32_t main(int32_t argc, char **argv)
{
    std::string filename;
    I::Base *image = nullptr;

    if ((argc > 1) && std::ifstream(argv[1]))
    {
        filename = argv[1];
        if (I::PPM::identify(filename))
            image = new I::PPM(filename);
    }
    else
    {
        usage(argv[0]);
        return 1;
    }

    if (image && (argc > 2))
    {
        for (int32_t i = 2; i < argc; ++i)
        {
            std::string arg(argv[i]);

            if (arg.substr(0, 7) == "--fliph")
                image->flipHorizontal();

            if (arg.substr(0, 7) == "--flipv")
                image->flipVertical();

            if (arg.substr(0, 9) == "--scalew=")
            {
                const int64_t scalew = std::stoi(arg.substr(9, std::string::npos));

                image->resize(scalew, image->height(), Image::Scaler::FastBillinear);
            }

            if (arg.substr(0, 9) == "--scaleh=")
            {
                const int64_t scaleh = std::stoi(arg.substr(9, std::string::npos));

                image->resize(image->width(), scaleh, Image::Scaler::FastBillinear);
            }

            if (arg.substr(0, 9) == "--filter=")
            {
                arg = arg.substr(9, std::string::npos);
                std::transform(arg.begin(), arg.end(), arg.begin(), ::tolower);

                if (arg == "smooth")
                    image->filter(I::Filter::Smooth);
                if (arg == "sharpen")
                    image->filter(I::Filter::Sharpen);
                if (arg == "edge")
                    image->filter(I::Filter::Edge);
                if (arg == "blur")
                    image->filter(I::Filter::Blur);
                if (arg == "raised")
                    image->filter(I::Filter::Raised);
            }
        }
    }

    if (image)
    {
        std::cout << "used colors: " << image->usedColors() << std::endl;
        view(image, filename);
        delete image;
    }

    return 0;
}