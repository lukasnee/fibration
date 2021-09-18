#include "logger.hpp"
#include "system.hpp"
#include "resources.hpp"

#include <string_view>
#include <cstdio>
#include <cstdarg>

void Logger::log(const std::string_view fmt, ...)
{
    if (Logger::getInstance().Logger::isActive())
    {
        va_list argList;
        va_start(argList, fmt);
        Logger::getInstance().logf(Logger::Verbosity::high, Type::none, fmt, argList);
        va_end(argList);
    }
}

void Logger::log(const Logger::Verbosity &verbosity, const Type &type, const std::string_view fmt, ...)
{
    if (Logger::getInstance().Logger::isActive())
    {
        va_list argList;
        va_start(argList, fmt);
        Logger::getInstance().logf(verbosity, type, fmt, argList);
        va_end(argList);
    }
}

void Logger::log(const Logger::Verbosity &verbosity, const Type &type, const std::string_view fmt, const va_list &argList)
{
    if (Logger::getInstance().Logger::isActive())
    {
        Logger::getInstance().logf(verbosity, type, fmt, argList);
    }
}

Logger &Logger::getInstance()
{
    static Logger instance;
    return instance;
};

bool Logger::isActive()
{
    return (Logger::Config::isEnabledCompileTime && this->checkConfig().logging && this->pAsciiStream);
}

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

const Logger::Config &Logger::checkConfig() { return this->config; }

Logger::Config &Logger::modifyConfig() { return this->config; }

int Logger::logf(const Logger::Verbosity &verbosity,
                 const Logger::Type &type,
                 const std::string_view fmt,
                 const va_list &argList)
{
    int charsPrinted = 0;

    if (Logger::isActive() && verbosity >= Logger::Config::verbosityFloor)
    {
        if (this->config.prefix)
        {
            charsPrinted = this->printPrefix(type);
        }

        if (0 <= charsPrinted)
        {
            int printfResult = this->pAsciiStream->printf(fmt.data(), argList);
            charsPrinted = printfResult > 0 ? charsPrinted + printfResult : printfResult;
        }
    }

    return charsPrinted;
}

int Logger::printPrefix(const Logger::Type &type)
{

#define ANSI_COLOR_BLACK "\e[30m"
#define ANSI_COLOR_RED "\e[31m"
#define ANSI_COLOR_GREEN "\e[32m"
#define ANSI_COLOR_YELLOW "\e[33m"
#define ANSI_COLOR_BLUE "\e[34m"
#define ANSI_COLOR_MAGENTA "\e[35m"
#define ANSI_COLOR_CYAN "\e[36m"
#define ANSI_COLOR_WHITE "\e[37m"
#define ANSI_COLOR_DEFAULT "\e[39m"
#define ANSI_COLOR_RESET "\e[0m"

#define UPTIME_FMT "%luT%02lu:%02lu:%02lu.%03lu"

    constexpr std::string_view types[static_cast<std::size_t>(Type::_enumTypeSize)] = {
        nullptr,
        {ANSI_COLOR_GREEN "TRA"},
        {ANSI_COLOR_MAGENTA "FAT"},
        {ANSI_COLOR_BLUE "SYS"},
        {ANSI_COLOR_RED "ERR"},
        {ANSI_COLOR_YELLOW "WAR"},
        {ANSI_COLOR_CYAN "INF"},
    };

    std::uint32_t days, hours, minutes, seconds, milliseconds;
    FibSys::getUptime(days, hours, minutes, seconds, milliseconds);
    const char *pFormat = this->config.color ? UPTIME_FMT " %s" ANSI_COLOR_RESET " " : UPTIME_FMT " %s ";
    const char *pType = types[static_cast<std::size_t>(type)].data() + (this->config.color ? 0 : 5);

    return this->pAsciiStream->printf(pFormat, days, hours, minutes, seconds, milliseconds, pType);
}

extern "C" void logger_log(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    Logger::log(Logger::Verbosity::high, Logger::Type::fatal, fmt, args);
    va_end(args);
}

#if FIB_SHELL_ENABLED
namespace ShellCommands
{
    static Shell::Command log(
        "log", Shell::Command::Helper::Literal::onOffUsage, nullptr, [](SHELLCMDPARAMS)
        {
            const char *strControlName = "logging";
            return Shell::Command::Helper::onOffCommand(Logger::getInstance().modifyConfig().logging, strControlName, SHELLCMDARGS);
        },
        []()
        {
            static Shell::Command color(
                log, "color", Shell::Command::Helper::Literal::onOffUsage, nullptr, [](SHELLCMDPARAMS)
                { return Shell::Command::Helper::onOffCommand(Logger::getInstance().modifyConfig().color, "color", SHELLCMDARGS); });
            static Shell::Command prefix(
                log, "prefix", Shell::Command::Helper::Literal::onOffUsage, nullptr, [](SHELLCMDPARAMS)
                { return Shell::Command::Helper::onOffCommand(Logger::getInstance().modifyConfig().prefix, "prefix", SHELLCMDARGS); });
        });
} // namespace ShellCommands
#endif // #if FIB_SHELL_ENABLED