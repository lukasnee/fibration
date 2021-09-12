#include "logger.hpp"
#include "system.hpp"
#include "resources.hpp"

#include <string_view>
#include <cstdio>
#include <cstdarg>

bool Logger::setAsciiStream(AsciiStream &asciiStream)
{
    bool result = true;

    if (Logger::getInstance().pAsciiStream)
    {
        Logger::getInstance().pAsciiStream->deinit();
    }

    Logger::getInstance().pAsciiStream = &asciiStream;
    result = Logger::getInstance().pAsciiStream->init();

    return result;
}

void Logger::setColoring(bool state)
{
    Logger::getInstance().logColored = state;
}

bool Logger::isActive()
{
    return (Logger::isEnabledCompileTime && Logger::getInstance().isEnabled && Logger::getInstance().pAsciiStream);
}

void Logger::setEnable(bool state)
{
    Logger::getInstance().isEnabled = state;
}

void Logger::log(const std::string_view fmt, ...)
{
    if (Logger::isActive())
    {
        va_list argList;
        va_start(argList, fmt);
        Logger::getInstance().logf(Logger::Verbosity::high, Type::none, fmt, argList);
        va_end(argList);
    }
}

void Logger::log(const Logger::Verbosity &verbosity, const Type &type, const std::string_view fmt, ...)
{
    if (Logger::isActive())
    {
        va_list argList;
        va_start(argList, fmt);
        Logger::getInstance().logf(verbosity, type, fmt, argList);
        va_end(argList);
    }
}

void Logger::log(const Logger::Verbosity &verbosity, const Type &type, const std::string_view fmt, const va_list &argList)
{
    if (Logger::isActive())
    {
        Logger::getInstance().logf(verbosity, type, fmt, argList);
    }
}

Logger &Logger::getInstance()
{
    static Logger instance;
    return instance;
};

Logger::Logger() {}

Logger::~Logger() {}

int Logger::logf(const Logger::Verbosity &verbosity,
                 const Logger::Type &type,
                 const std::string_view fmt,
                 const va_list &argList)
{
    int charsPrinted = 0;

    if (Logger::isActive() && verbosity >= Logger::verbosityFloor)
    {
        charsPrinted = this->printPrefix(type);
        if (0 < charsPrinted)
        {
            int printfResult = this->pAsciiStream->printf(fmt.data(), argList);
            charsPrinted = printfResult > 0 ? charsPrinted + printfResult : printfResult;
        }
    }

    return charsPrinted;
}

int Logger::printPrefix(const Logger::Type &type)
{
    constexpr std::string_view types[static_cast<std::size_t>(Type::_enumTypeSize)] = {
        nullptr,   // none
        {"2mTRA"}, // green TRACE
        {"5mFAT"}, // magenta FATAL
        {"4mSYS"}, // blue SYSTEM
        {"1mERR"}, // red ERROR
        {"3mWAR"}, // yellow WARNING
        {"6mINF"}, // cyan INFO
    };

    std::uint32_t hours, minutes, seconds, milliseconds;
    FibSys::getUptime(hours, minutes, seconds, milliseconds);
    const char *pFormat = logColored ? "%02lu:%02lu:%02lu.%03lu \e[3%s\e[0m " : "%02lu:%02lu:%02lu.%03lu %s ";
    const char *pType = types[static_cast<std::size_t>(type)].data() + (logColored ? 0 : 5);

    return this->pAsciiStream->printf(pFormat, hours, minutes, seconds, milliseconds, pType);
}

extern "C" void logger_log(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    Logger::log(Logger::Verbosity::high, Logger::Type::fatal, fmt, args);
    va_end(args);
}
