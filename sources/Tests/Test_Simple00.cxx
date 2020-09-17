#include <iostream>
#include "Image/Simple00.hxx"
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
              << std::endl;
}

int32_t main(int32_t argc, char **argv)
{
    std::string filename = "simple00_test.spl";
    int64_t width = TestCase::DefaultWidth;
    int64_t height = TestCase::DefaultHeight;
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
                            std::cerr << "ERROR: width " << width << " above maximum Simple00 "
                                      << "width, using default width '" << TestCase::DefaultWidth
                                      << "'" << std::endl;
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
                            std::cerr << "ERROR: height " << height << " above maximum Simple00 "
                                      << "height, using default height '"
                                      << TestCase::DefaultHeight << "'" << std::endl;
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
        }
    }

    if (!help)
    {
        Image::Simple00 simple(width, height);

        if (TestCase::applyToImageCase00(simple))
            simple.save(filename);
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
