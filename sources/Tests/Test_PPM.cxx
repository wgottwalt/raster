#include <iostream>
#include "Image/PPM.hxx"
#include "TestCases.hxx"

void showHelp(const char *name)
{
    std::cout << "usage: " << name << " [options]\n"
              << "options:\n"
              << "  --help               this help screen\n"
              << "  --output=<filename>  uses filename for created image\n"
              << "  --width=<width>      uses this width instead of the default '"
                << TestCase::DefaultWidth << "'\n"
              << "  --height=<height>    uses this height instead of the default '"
                << TestCase::DefaultHeight << "'\n"
              << "  --compression=<c>    uses the compression c (none/binary)\n"
              << "  --wide=<yes/no>      uses wide mode (aka 48bit depth)\n"
              << std::endl;
}

int32_t main(int32_t argc, char **argv)
{
    std::string filename = "ppm_test.ppm";
    int64_t width = TestCase::DefaultWidth;
    int64_t height = TestCase::DefaultHeight;
    bool binary = true;
    bool wide = false;
    bool help = false;

    if (argc > 1)
    {
        std::string arg;

        for (int32_t i = 1; i < argc; ++i)
        {
            arg = argv[i];

            if (arg == "--help")
            {
                showHelp(argv[0]);
                help = true;
                continue;
            }

            if (arg.substr(0, 9) == "--output=")
            {
                if (arg.size() > 9)
                    filename = arg.substr(9, std::string::npos);
                else
                {
                    std::cerr << "ERROR: no filename parameter given, using default '" << filename
                              << "'" << std::endl;
                }
                continue;
            }

            if (arg.substr(0, 8) == "--width=")
            {
                if (arg.size() > 8)
                {
                    try
                    {
                        width = std::stoll(arg.substr(8, std::string::npos));
                        if (width > Image::PPM::MaxWidth)
                        {
                            std::cerr << "ERROR: width " << width << " above maximum PPM width, "
                                      << "using default width '" << TestCase::DefaultWidth << "'"
                                      << std::endl;
                            width = TestCase::DefaultWidth;
                        }
                    }
                    catch (...)
                    {
                        std::cerr << "ERROR: unable to parse width '"
                                  << arg.substr(8, std::string::npos)
                                  << "', using default width " << TestCase::DefaultWidth
                                  << std::endl;
                        width = TestCase::DefaultWidth;
                    }
                }
                else
                {
                    std::cout << "ERROR: no width parameter given, using default width '"
                              << TestCase::DefaultWidth << "'" << std::endl;
                }
                continue;
            }

            if (arg.substr(0, 9) == "--height=")
            {
                if (arg.size() > 9)
                {
                    try
                    {
                        height = std::stoll(arg.substr(9, std::string::npos));
                        if (height > Image::PPM::MaxHeight)
                        {
                            std::cerr << "ERROR: height " << height << " above maximum PPM height, "
                                      << "using default height '" << TestCase::DefaultHeight << "'"
                                      << std::endl;
                            height = TestCase::DefaultHeight;
                        }
                    }
                    catch (...)
                    {
                        std::cerr << "ERROR: unable to parse height '"
                                  << arg.substr(9, std::string::npos)
                                  << "', using default height " << TestCase::DefaultHeight
                                  << std::endl;
                        height = TestCase::DefaultHeight;
                    }
                }
                else
                {
                    std::cout << "ERROR: no height parameter given, using default height '"
                              << TestCase::DefaultHeight << "'" << std::endl;
                }
                continue;
            }

            if (arg.substr(0, 14) == "--compression=")
            {
                if (arg.size() > 14)
                {
                    arg = arg.substr(14, std::string::npos);
                    if (arg == "binary")
                        binary = true;
                    else if (arg == "none")
                        binary = false;
                    else
                    {
                        std::cerr << "ERROR: unknown compression parameter, using default '"
                                  << (binary ? "binary" : "none") << "'" << std::endl;
                        binary = true;
                    }
                }
                else
                {
                    std::cerr << "ERROR: no compression parameter given, using default '"
                              << (binary ? "binary" : "none") << "'" << std::endl;
                }
                continue;
            }

            if (arg.substr(0, 7) == "--wide=")
            {
                if (arg.size() > 7)
                {
                    arg = arg.substr(7, std::string::npos);
                    if (arg == "yes")
                        wide = true;
                    else if (arg == "no")
                        wide = false;
                    else
                    {
                        std::cerr << "ERROR: unknown wide paramater given, using default '"
                                  << (wide ? "yes" : "no") << "'" << std::endl;
                    }
                }
                else
                {
                    std::cerr << "ERROR: no wide parameter given, using default '"
                              << (wide ? "yes" : "no") << "'" << std::endl;
                }
                continue;
            }
        }
    }

    if (!help)
    {
        Image::PPM ppm(width, height);

        if (TestCase::applyToImageCase00(ppm))
        {
            ppm.setWideMode(wide);
            ppm.setBinaryMode(binary);
            ppm.save(filename);
        }
        else
        {
            if (width < TestCase::DefaultMinWidth)
            {
                std::cerr << "ERROR: '" << width << "' below minimum required width '"
                          << TestCase::DefaultMinWidth << "'" << std::endl;
            }

            if (height < TestCase::DefaultMinHeight)
            {
                std::cerr << "ERROR: '" << height << "' below minimum required height '"
                          << TestCase::DefaultMinHeight << "'" << std::endl;
            }
        }
    }

    return 0;
}
