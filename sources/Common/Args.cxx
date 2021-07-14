#include <cctype>
#include "Args.hxx"

//--- public constructors ---

Args::Args(int32_t &argc, char **argv, const HelpFunc func) noexcept(false)
: _argc{argc}, _argv{argv}, _func{func}, _help_called{false}
{
    _help_called = parseHelp();
}

Args::~Args() noexcept
{
}

//--- public methods ---

bool Args::help(const bool force) const noexcept(false)
{
    if ((_help_called || force) && _func)
    {
        _func(_argv[0]);
        return true;
    }

    return false;
}

Args::Result Args::longOpt(const std::string &param, const bool has_value) const noexcept(false)
{
    if (!param.empty())
    {
        const size_t psize{param.size()};

        for (int32_t i{1}; i < _argc; ++i)
        {
            const std::string arg{_argv[i]};
            const size_t asize{arg.size()};

            if ((asize <= 2) || (arg.substr(0, 2) != "--"))
                continue;

            if (arg.substr(2, psize) == param)
            {
                if (has_value)
                {
                    if ((asize > (psize + 3)) && (arg[psize + 2] == '='))
                        return {true, arg.substr(psize + 3, std::string::npos)};

                    if ((asize == (psize + 2)) && ((i + 1) < _argc) && (_argv[i + 1][0] != '-'))
                        return {true, _argv[i + 1]};
                }
                else
                {
                    if (asize == (psize + 2))
                        return {true, ""};
                }
            }
        }
    }

    return {false, ""};
}

Args::Result Args::shortOpt(const char param, const bool has_value) const noexcept(false)
{
    if (std::isalnum(param))
    {
        for (int32_t i = 1; i < _argc; ++i)
        {
            const std::string arg{_argv[i]};
            const size_t asize{arg.size()};

            if ((asize <= 1) || (arg[0] != '-'))
                continue;

            if ((asize > 1) && (arg[1] == param))
            {
                if (has_value)
                {
                    if ((asize > 3) && (arg[2] == '='))
                        return {true, arg.substr(3, std::string::npos)};

                    if ((asize == 2) && ((i + 1) < _argc) && (_argv[i + 1][0] != '-'))
                        return {true, _argv[i + 1]};
                }
                else
                {
                    if (asize == 2)
                        return {true, ""};
                }
            }
        }
    }

    return {false, ""};
}

//--- protected methods ---

bool Args::parseHelp() const noexcept(false)
{
    for (int32_t i = 1; i < _argc; ++i)
    {
        const std::string_view arg{_argv[i]};

        if ((arg == "--help") || (arg == "-h"))
            return true;
    }

    return false;
}
