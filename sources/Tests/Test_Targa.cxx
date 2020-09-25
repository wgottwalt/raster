#include <iostream>
#include "Common/Tools.hxx"
#include "Image/Targa.hxx"
#include "TestCases.hxx"

namespace T = Common::Tools;

static const std::string DefFilename = "targa_test.tga";
static const int32_t DefDepth = 24;

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
              << "  --version=<0,1>      picks the Targa format version\n"
              << "  --depth=<num>        sets one of the supported depths of 1, 8, 16, 24 or 32 bits\n"
              << std::endl;
}

int32_t main(int32_t argc, char **argv)
{
    std::string filename = DefFilename;
    int64_t width = TestCase::DefaultWidth;
    int64_t height = TestCase::DefaultHeight;
    int32_t version = 1;
    int32_t depth = DefDepth;
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
                        if (width > Image::Targa::MaxWidth)
                        {
                            std::cerr << "ERROR: width " << width << " above maximum Targa width "
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
                        if (height > Image::Targa::MaxHeight)
                        {
                            std::cerr << "ERROR: height " << height << " above maximum Targa height"
                                      << ", using default height '" << TestCase::DefaultHeight
                                      << "'" << std::endl;
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
                        if ((version < 1) && (version > 2))
                        {
                            std::cerr << "ERROR: unsupported version '" << version << "', using "
                                      << "default version '1'" << std::endl;
                            version = 1;
                        }
                    }
                    catch (...)
                    {
                        std::cerr << "ERROR: unable to parse version '"
                                  << arg.substr(10, std::string::npos) << "', using default '1'"
                                  << std::endl;
                        version = 1;
                    }
                }
                else
                {
                    std::cerr << "ERROR: no version parameter gives, using default version '1'"
                              << std::endl;
                    version = 1;
                }
                continue;
            }

            if (arg.substr(0, 8) == "--depth=")
            {
                if (arg.size() > 8)
                {
                    try
                    {
                        depth = std::stoi(arg.substr(8, std::string::npos));
                        if (!T::includes(depth, 1, 8, 16, 24, 32))
                        {
                            std::cerr << "ERROR: unsupported depth '" << depth << "', using "
                                      << "default depth '" << DefDepth << "'" << std::endl;
                            depth = DefDepth;
                        }
                    }
                    catch (...)
                    {
                        std::cerr << "ERROR: unable to parse depth '"
                                  << arg.substr(8, std::string::npos) << "', using default '"
                                  << DefDepth << "'" << std::endl;
                        depth = DefDepth;
                    }
                }
                else
                {
                    std::cerr << "ERROR: no depth parameter gives, using default depth '" << DefDepth
                              << "'" << std::endl;
                    depth = DefDepth;
                }

                continue;
            }
        }
    }

    if (!help)
    {
        Image::Targa targa(width, height);

        if (TestCase::applyToImageCase00(targa))
        {
            switch (depth)
            {
                case 1:
                    targa.setImageType(Image::Targa::ImageType::Mono);
                    targa.setGreyscaleMonochromeMode(false);
                    break;

                case 8:
                    targa.setImageType(Image::Targa::ImageType::Mapped);
                    break;

                case 16:
                    targa.setDepth(16);
                    break;

                case 32:
                    targa.setDepth(32);
                    break;

                case 24:
                default:
                    targa.setDepth(24);
            }
            targa.setVersion2((version > 1) ? true : false);
            targa.save(filename);
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
