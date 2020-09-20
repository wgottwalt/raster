#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include "Compression/LZW16.hxx"

using Secs = std::chrono::seconds;
using MSecs = std::chrono::milliseconds;

const std::string DefInput = "/dev/urandom";
const std::string DefOutput = "/dev/null";
const Secs DefRuntime(10);
const MSecs DefReporttime(1000);

Compression::LZW16 lzw;
std::ifstream ifile;
std::ofstream ofile;
bool run = true;

void showHelp(const char *name)
{
    std::cout << "usage: " << name << " [options]\n"
              << "options:\n"
              << "  --help               this help screen\n"
              << "  --input=<filename>   name of the input file (default: " << DefInput << ")\n"
              << "  --output=<filename>  name of the output file (default: " << DefOutput << ")\n"
              << "  --runtime=<seconds>  amount of seconds the test should run (default: "
                << DefRuntime.count() << ")\n"
              << "  --reporttime=<ms>    milliseconds after an in/out bytes report is printed "
                << "(default: " << DefReporttime.count() << ")\n"
              << std::endl;
}

void reportThread(const MSecs ms)
{
    int64_t in_s = 0;
    int64_t out_s = 0;
    int64_t in_e = 0;
    int64_t out_e = 0;

    while (run)
    {
        in_s = in_e;
        out_s = out_e;
        std::this_thread::sleep_for(ms);
        in_e = lzw.bytesRead();
        out_e = lzw.bytesWritten();
        std::cout << "in:  " << (in_e - in_s) << "\nout: " << (out_e - out_s) << "\n\033[F\033[F";
    }
    std::cout << "\n" << std::endl;
}

void stopThread(const Secs secs)
{
    std::this_thread::sleep_for(secs);
    run = false;
    ifile.setstate(std::ios::eofbit);
}

int32_t main(int32_t argc, char **argv)
{
    std::string ifilename = DefInput;
    std::string ofilename = DefOutput;
    std::chrono::seconds runtime(DefRuntime);
    std::chrono::milliseconds reporttime(DefReporttime);
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
                return 0;
            }

            if (arg.substr(0, 8) == "--input=")
            {
                if (arg.size() > 8)
                    ifilename = arg.substr(8, std::string::npos);
                else
                {
                    std::cerr << "ERROR: no input filename paramater given, using default '"
                              << DefInput << "'" << std::endl;
                    ifilename = DefInput;
                }
                continue;
            }

            if (arg.substr(0, 9) == "--output=")
            {
                if (arg.size() > 9)
                    ofilename = arg.substr(9, std::string::npos);
                else
                {
                    std::cerr << "ERROR: no output filename paramater given, using default '"
                              << DefOutput << "'" << std::endl;
                    ofilename = DefOutput;
                }
                continue;
            }

            if (arg.substr(0, 10) == "--runtime=")
            {
                if (arg.size() > 10)
                {
                    try
                    {
                        const int32_t val = std::stoi(arg.substr(10, std::string::npos));

                        if (val < 1)
                        {
                            std::cerr << "ERROR: runtime must be at least 1 second, using default '"
                                      << DefRuntime.count() << "'" << std::endl;
                            runtime = DefRuntime;
                        }
                        else
                            runtime = Secs(val);
                    }
                    catch (...)
                    {
                        std::cerr << "ERROR: unable to parse runtime seconds value, using default '"
                                  << DefRuntime.count() << "'" << std::endl;
                        runtime = DefRuntime;
                    }
                }
                else
                {
                    std::cerr << "ERROR: no seconds value given, using default '"
                              << DefRuntime.count() << "'" << std::endl;
                    runtime = DefRuntime;
                }
                continue;
            }

            if (arg.substr(0, 13) == "--reporttime=")
            {
                if (arg.size() > 13)
                {
                    try
                    {
                        const int32_t val = std::stoi(arg.substr(13, std::string::npos));

                        if (val < 50)
                        {
                            std::cerr << "ERROR: reporttime must be at least 50 ms, using default '"
                                      << DefReporttime.count() << "'" << std::endl;
                            reporttime = DefReporttime;
                        }
                        else
                            reporttime = MSecs(val);
                    }
                    catch (...)
                    {
                        std::cerr << "ERROR: unable to parse reporttime ms value, using default '"
                                  << DefReporttime.count() << "'" << std::endl;
                        reporttime = DefReporttime;
                    }
                }
                else
                {
                    std::cerr << "ERROR: no ms value given, using default '"
                              << DefReporttime.count() << "'" << std::endl;
                    reporttime = DefReporttime;
                }
                continue;
            }
        }
    }

    ifile.open(ifilename);
    ofile.open(ofilename);
    if (!help && ifile.is_open() && ifile.good() && ofile.is_open() && ofile.good())
    {
        std::thread thread1;
        std::thread thread2;

        std::cout << "input:     " << ifilename << "\n"
                  << "output:    " << ofilename << "\n"
                  << "runtime:   " << runtime.count() << " secs\n"
                  << "reportime: " << reporttime.count() << " ms\n"
                  << std::endl;

        thread1 = std::thread(reportThread, reporttime);
        thread2 = std::thread(stopThread, runtime);

        lzw.encode(ifile, ofile);

        thread2.join();
        thread1.join();
    }

    return 0;
}
