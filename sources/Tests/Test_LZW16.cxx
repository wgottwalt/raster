#include <fstream>
#include <iostream>
#include <string>
#include "Compression/LZW16.hxx"

void usage(const char *str)
{
    std::cout << "usage: " << str << " [--encode/--decode] <input file> <output file>" << std::endl;
}

int32_t main(int32_t argc, char **argv)
{
    if (argc == 4)
    {
        Compression::LZW16 lzw;
        std::ifstream ifile(argv[2], std::ios::binary);
        std::ofstream ofile(argv[3], std::ios::binary);

        if (std::string(argv[1]) == "--encode")
            lzw.encode(ifile, ofile);
        else if (std::string(argv[1]) == "--decode")
            lzw.decode(ifile, ofile);
        else
            usage(argv[0]);

        std::cout << "in:  " << lzw.bytesRead() << " bytes\nout: " << lzw.bytesWritten()
                  << " bytes" << std::endl;
    }
    else
        usage(argv[0]);

    return 0;
}
