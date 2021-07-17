#pragma once

#include <functional>
#include <string>
#include <tuple>

namespace Common
{
    class Args {
    public:
        //--- public types and constants ---
        using HelpFunc = std::function<void (const std::string &)>;
        using Result = std::tuple<bool,const std::string>;

        //--- public constructors ---
        Args(int32_t &argc, char **argv, const HelpFunc func) noexcept(false);
        Args(const Args &rhs) noexcept(false) = delete;
        Args(Args &&rhs) noexcept = delete;
        ~Args() noexcept;

        //--- public operators ---
        Args &operator=(const Args &rhs) noexcept(false) = delete;
        Args &operator=(Args &&rhs) noexcept = delete;

        //--- public methods ---
        bool help(const bool force = false) const noexcept(false);
        Result longOpt(const std::string &param, const bool has_value = false) const noexcept(false);
        Result shortOpt(const char param, const bool has_value = false) const noexcept(false);

    protected:
        //--- protected methods ---
        bool parseHelp() const noexcept(false);

    private:
        //--- private properties ---
        int32_t &_argc;
        char **_argv;
        HelpFunc _func;
        bool _help_called;
    };
}
