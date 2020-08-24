#include "Tools.hxx"

namespace Common::Tools
{
    std::string trim(const std::string &data, const char delim, const bool at_start,
                     const bool at_end)
    {
        std::string tmp(data);

        if (at_start)
            while (!tmp.empty() && (*(tmp.begin())) == delim)
                tmp.erase(tmp.begin(), tmp.begin() + 1);
        if (at_end)
            while (!tmp.empty() && (*(tmp.end() - 1)) == delim)
                tmp.erase(tmp.end() - 1, tmp.end());

        return tmp;
    }

    std::string trim(const std::string &data, const bool at_start, const bool at_end)
    {
        std::string tmp(data);

        if (at_start)
            while (!tmp.empty() && std::isspace(*(tmp.begin())))
                tmp.erase(tmp.begin(), tmp.begin() + 1);
        if (at_end)
            while (!tmp.empty() && std::isspace(*(tmp.end() - 1)))
                tmp.erase(tmp.end() - 1, tmp.end());

        return tmp;
    }
}
