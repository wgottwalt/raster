#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include "Common/Tools.hxx"
#include "Image/Image.hxx"

namespace T = Common::Tools;

void usage(const char *appname)
{
    std::cout << "usage: " << appname << " --input=<image file> --output=<image file> [options]\n"
              << std::endl;
}

int32_t main(int32_t argc, char **argv)
{
    if (argc > 1)
    {
        std::string ifilename;
        std::string ofilename;
        std::shared_ptr<Image::Base> in_image;
        std::shared_ptr<Image::Base> out_image;

        for (int32_t i = 1; i < argc; ++i)
        {
            const std::string arg(argv[i]);

            if (arg.substr(0, 8) == "--input=")
                ifilename = arg.substr(8, std::string::npos);

            if (arg.substr(0, 9) == "--output=")
                ofilename = arg.substr(9, std::string::npos);
        }

        if (std::ifstream(ifilename) && !ofilename.empty())
        {
            const std::string ff1(".ff");
            const std::string ff2(".farbfeld");
            const std::string ppm(".ppm");
            const std::string simple0(".sp0");
            const std::string simple1(".sp1");
            const std::string simple2(".simple00");
            const std::string simple3(".simple01");

            if (Image::Farbfeld::identify(ifilename))
                in_image = std::make_shared<Image::Farbfeld>(ifilename);
            else if (Image::PPM::identify(ifilename))
                in_image = std::make_shared<Image::PPM>(ifilename);
            else if (Image::Simple00::identify(ifilename))
                in_image = std::make_shared<Image::Simple00>(ifilename);
            else if (Image::Simple01::identify(ifilename))
                in_image = std::make_shared<Image::Simple01>(ifilename);

            if (in_image)
            {
                if (T::countSubSequences(ofilename, ff1) || T::countSubSequences(ofilename, ff2))
                    out_image = std::make_shared<Image::Farbfeld>(in_image->pixels(),
                                in_image->width(), in_image->height());
                else if (T::countSubSequences(ofilename, ppm))
                    out_image = std::make_shared<Image::PPM>(in_image->pixels(), in_image->width(),
                                in_image->height());
                else if (T::countSubSequences(ofilename, simple0) ||
                  T::countSubSequences(ofilename, simple2))
                    out_image = std::make_shared<Image::Simple00>(in_image->pixels(),
                                in_image->width(), in_image->height());
                else if (T::countSubSequences(ofilename, simple1) ||
                  T::countSubSequences(ofilename, simple3))
                    out_image = std::make_shared<Image::Simple01>(in_image->pixels(),
                                in_image->width(), in_image->height());
            }
        }

        if (out_image)
            out_image->save(ofilename);
    }

    return 0;
}
