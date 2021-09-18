#pragma once

#include "asciiStreamIF.hpp"

#include <string_view>
#include <cstdarg>

#define FIB_SHELL_ENABLED 1

#if FIB_SHELL_ENABLED
#include "shell.hpp"
#endif // #if FIB_SHELL_ENABLED
class Logger
{
public:
    enum struct Verbosity
    {
        low,
        mid,
        high
    };

    enum struct Type : std::size_t
    {
        none = 0,
        trace,
        fatal,
        system,
        error,
        warning,
        info,

        _enumTypeSize
    };

    struct Config
    {
        /* log messages will be fairly fast up to this length */
        static constexpr std::size_t optimalLogStringLength = 128;
        /* if log message turns out to be longer than this (@run-time), error will be printed out */
        static constexpr std::size_t maxLogStringLength = 1024;
        /* logger can be disabled here at @compile-time (hopefully optimized out @compile-time) */
        static constexpr bool isEnabledCompileTime = true;
        /* lower verbosity level log entries will be optimized out @compile-time */
        static constexpr Verbosity verbosityFloor = Verbosity::low;

        bool logging = true;
        bool color = true;
        bool prefix = true;
    };

    /**
     * @brief @attention user is responsible for putting '\n' end line
     */

    static void log(const std::string_view fmt, ...);
    static void log(const Verbosity &verbosity, const Type &type, const std::string_view fmt, ...);
    static void log(const Verbosity &verbosity, const Type &type, const std::string_view fmt, const va_list &argList);

    static Logger &getInstance();

    bool isActive();
    static bool setAsciiStream(AsciiStream &asciiStream);
    const Config &checkConfig();
    Config &modifyConfig();

protected:
    Config config;

private:
    Logger(Logger const &) = delete;
    void operator=(Logger const &) = delete;
    Logger() = default;
    ~Logger() = default;

    int logf(const Logger::Verbosity &verbosity, const Logger::Type &type, const std::string_view fmt, const va_list &argList);

    int printPrefix(const Type &type);

    AsciiStream *pAsciiStream = nullptr;
};
