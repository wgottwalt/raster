#include <iostream>
#include "Image/Simple00.hxx"
#include "Image/Simple01.hxx"
#include "Image/Simple02.hxx"
#include "TestCases.hxx"

static const std::string DefFilename = "simple_test.spl";

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
              << "  --version=<0..2>     picks the Simple image version\n"
              << std::endl;
}

int32_t main(int32_t argc, char **argv)
{
    std::string filename = DefFilename;
    int64_t width = TestCase::DefaultWidth;
    int64_t height = TestCase::DefaultHeight;
    int32_t version = 0;
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
                    std::cerr << "ERROR: no filename parameter given, using default '"
                              << DefFilename << "'" << std::endl;
                    filename = DefFilename;
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
                            std::cerr << "ERROR: width " << width << " above maximum Simple width "
                                      << ", using default width '" << TestCase::DefaultWidth << "'"
                                      << std::endl;
                            width = TestCase::DefaultWidth;
                        }
                    }
                    catch (...)
                    {
                        std::cerr << "ERROR: unable to parse width '"
                                  << arg.substr(8, std::string::npos) << "', using default width '"
                                  << TestCase::DefaultWidth << "'" << std::endl;
                        width = TestCase::DefaultWidth;
                    }
                }
                else
                {
                    std::cerr << "ERROR: no width parameter given, using default width '"
                              << TestCase::DefaultWidth << "'" << std::endl;
                    width = TestCase::DefaultWidth;
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
                            std::cerr << "ERROR: height " << height << " above maximum Simple "
                                      << "height, using default height '"
                                      << TestCase::DefaultHeight << "'" << std::endl;
                            height = TestCase::DefaultHeight;
                        }
                    }
                    catch (...)
                    {
                        std::cerr << "ERROR: unable to parse height '"
                                  << arg.substr(9, std::string::npos) << "', using default height '"
                                  << TestCase::DefaultHeight << "'" << std::endl;
                        height = TestCase::DefaultHeight;
                    }
                }
                else
                {
                    std::cerr << "ERROR: no height parameter given, using default height '"
                              << TestCase::DefaultHeight << "'" << std::endl;
                    height = TestCase::DefaultHeight;
                }
                continue;
            }

            if (arg.substr(0, 10) == "--version=")
            {
                if (arg.size() > 10)
                {
                    try
                    {
                        version = std::stoi(arg.substr(10, std::string::npos));
                        if (version > 2)
                        {
                            std::cerr << "ERROR: unsupported version '" << version << "', using "
                                      << "default version '0'" << std::endl;
                            version = 0;
                        }
                    }
                    catch (...)
                    {
                        std::cerr << "ERROR: unable to parse version '"
                                  << arg.substr(10, std::string::npos) << "', using default '0'"
                                  << std::endl;
                        version = 0;
                    }
                }
                else
                {
                    std::cerr << "ERROR: no version parameter gives, using default version '0'"
                              << std::endl;
                    version = 0;
                }
                continue;
            }
        }
    }

    if (!help)
    {
        Image::Base *simple = nullptr;

        switch (version)
        {
            case 2:
                simple = new Image::Simple02(width, height);
                break;

            case 1:
                simple = new Image::Simple01(width, height);
                break;

            case 0:
            default:
                simple = new Image::Simple00(width, height);
        }

        if (simple && TestCase::applyToImageCase00(*simple))
        {
            simple->save(filename);
            delete simple;
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
