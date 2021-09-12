#pragma once

#include "asciiStreamIF.hpp"

#include <string_view>

#include <cstdarg>
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

    // CONFIGURATION
    /* log messages will be fairly fast up to this length */
    static constexpr std::size_t optimalLogStringLength = 128;
    /* if log message turns out to be longer than this (@run-time), error will be printed out */
    static constexpr std::size_t maxLogStringLength = 1024;
    /* logger can be disabled here at @compile-time (hopefully optimized out @compile-time) */
    static constexpr bool isEnabledCompileTime = true;
    /* lower verbosity level log entries will be optimized out @compile-time */
    static constexpr Verbosity verbosityFloor = Verbosity::low;

    // SETUP
    static bool setAsciiStream(AsciiStream &asciiStream);
    static void setColoring(bool state);
    static bool isActive();

    static void setEnable(bool state);
    // USAGE (note: user is responsible for putting '\n' end line)
    static void log(const std::string_view fmt, ...);
    static void log(const Verbosity &verbosity, const Type &type, const std::string_view fmt, ...);
    static void log(const Verbosity &verbosity, const Type &type, const std::string_view fmt, const va_list &argList);
protected:
private:
    static Logger &getInstance();

    Logger(Logger const &) = delete;
    void operator=(Logger const &) = delete;
    Logger();
    ~Logger();

    int logf(const Logger::Verbosity &verbosity, const Logger::Type &type, const std::string_view fmt, const va_list &argList);

    int printPrefix(const Type &type);

    bool logColored = true;

    AsciiStream *pAsciiStream = nullptr;
    bool isEnabled = true;
};
