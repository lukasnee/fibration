#pragma once

#include "ioStream.hpp"
#include "stringContainer.hpp"
#include <string_view>
#include <cstdarg>
#include <functional>
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
    static bool setIoStream(IOStream &ioStream);
    static void setColoring(bool state);
    static bool isActive();

    static void setEnable(bool state);
    // USAGE (note: user is responsible for putting '\n' end line)
    static bool log(const Verbosity &verbosity, const Type &type, const std::string_view fmt, ...);
    static bool log(const Verbosity &verbosity, const Type &type, const std::string_view fmt, const va_list &arglist);
    static bool log(const std::string_view fmt, ...);
    static bool logFast(const std::string_view string);
    static bool logFastFromISR(const std::string_view string);

protected:
private:
    static Logger &getInstance();

    Logger(Logger const &) = delete;
    void operator=(Logger const &) = delete;
    Logger();
    ~Logger();

    bool logFormatted(const Verbosity &verbosity, const Type &type, const std::string_view fmt, const va_list &arglist);
    bool printOptimallyInto(StringContainer &stringContainer,
                            std::function<int(StringContainer &logString)> printF,
                            std::function<void(StringContainer &logString)> optimalPrintFailedCallbackF);
    int formatPrefix(const Type &type, char *pOut, const std::size_t &maxSize);
    void logOutOfMem();
    void logStringTooLong();

    bool logColored = true;

    IOStream *pIoStream = nullptr;
    bool isEnabled = true;
};
