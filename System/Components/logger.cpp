#include "logger.hpp"
#include "system.hpp"
#include "resources.hpp"

#include <string_view>
#include <cstdio>
#include <cstdarg>

bool Logger::setIoStream(IOStream &ioStream)
{
    bool result = true;

    if (Logger::getInstance().pIoStream)
    {
        Logger::getInstance().pIoStream->deinit();
    }

    Logger::getInstance().pIoStream = &ioStream;
    result = Logger::getInstance().pIoStream->init();

    return result;
}

void Logger::setColoring(bool state)
{
    Logger::getInstance().logColored = state;
}

bool Logger::isActive()
{
    return (Logger::isEnabledCompileTime && Logger::getInstance().isEnabled && Logger::getInstance().pIoStream);
}

void Logger::setEnable(bool state)
{
    Logger::getInstance().isEnabled = state;
}

bool Logger::log(const std::string_view fmt, ...)
{
    bool result = false;

    if (Logger::isActive())
    {
        va_list arglist;
        va_start(arglist, fmt);
        result = Logger::getInstance().logFormatted(Logger::Verbosity::high, Type::none, fmt, arglist);
        va_end(arglist);
    }

    return result;
}

bool Logger::log(const Logger::Verbosity &verbosity, const Type &type, const std::string_view fmt, ...)
{
    bool result = false;

    if (Logger::isActive())
    {
        va_list arglist;
        va_start(arglist, fmt);
        result = Logger::getInstance().logFormatted(verbosity, type, fmt, arglist);
        va_end(arglist);
    }

    return result;
}

bool Logger::log(const Logger::Verbosity &verbosity, const Type &type, const std::string_view fmt, const va_list &arglist)
{
    return Logger::getInstance().logFormatted(verbosity, type, fmt, arglist);
}

bool Logger::logFast(const std::string_view string)
{
    bool result = false;

    if (Logger::isActive())
    {
        result = Logger::getInstance().pIoStream->push(reinterpret_cast<const uint8_t *>(string.data()), string.length(), 1);
    }

    return result;
}

bool Logger::logFastFromISR(const std::string_view string)
{
    bool result = false;

    if (Logger::isActive())
    {
        result = Logger::getInstance().pIoStream->pushFromIsr(reinterpret_cast<const uint8_t *>(string.data()), string.length());
    }

    return result;
}

Logger &Logger::getInstance()
{
    static Logger instance;
    return instance;
};

Logger::Logger() {}

Logger::~Logger() {}

bool Logger::logFormatted(const Logger::Verbosity &verbosity,
                          const Logger::Type &type,
                          const std::string_view fmt,
                          const va_list &arglist)
{
    bool result = true;

    if (Logger::isActive() && verbosity >= Logger::verbosityFloor)
    {
        // prefix stage callbacks
        const auto usingPrefixPrintF = [&](StringContainer &logString)
        {
            return this->formatPrefix(type, logString.getHead(), logString.getCharsLeft());
        };
        const auto itShouldAlwaysFitButIfNot_FreeItAndReprint = [&](StringContainer &logString)
        {
            logString.free();
        };

        // message stage callbacks
        const auto usingMessagePrintF = [&](StringContainer &logString)
        {
            return vsnprintf(logString.getHead(), logString.getCharsLeft(), fmt.data(), arglist);
        };
        const auto ifItDoesNotFitPushPrefixPartAndReprintMessage = [&](StringContainer &logString)
        {
            this->pIoStream->push(reinterpret_cast<const uint8_t *>(logString.getBase()), logString.getCharsUsed(), 1);
            logString.free(); // remove dynamic allocation
        };

        StringContainer logString;
        if (type != Type::none)
        {
            if (this->printOptimallyInto(logString,
                                         usingPrefixPrintF,
                                         itShouldAlwaysFitButIfNot_FreeItAndReprint))
            {
                result = false;
            }
        }
        result &= this->printOptimallyInto(logString,
                                           usingMessagePrintF,
                                           ifItDoesNotFitPushPrefixPartAndReprintMessage);

        // push what's left
        result &= this->pIoStream->push(reinterpret_cast<const uint8_t *>(logString.getBase()), logString.getCharsUsed(), 1);
        logString.free(); // TODO remove dynamic allocation
    }

    return result;
}

// try to print fast within optimal string lenght. If string did not fit the container - reallocate and reprint
bool Logger::printOptimallyInto(StringContainer &stringContainer,
                                std::function<int(StringContainer &logString)> printF,
                                std::function<void(StringContainer &logString)> optimalPrintFailedCallbackF)
{
    bool result = false;

    if (stringContainer.getCharsLeft() == 0 && false == stringContainer.allocate(Logger::optimalLogStringLength))
    {
        this->logOutOfMem();
        return false;
    }

    int stepInNumOfChars = printF(stringContainer);

    if (stepInNumOfChars > stringContainer.getCharsLeft()) // string did not fit into optimally sized container
    {
        optimalPrintFailedCallbackF(stringContainer);
        if (stepInNumOfChars >= static_cast<int>(Logger::maxLogStringLength)) // string is impossible to reprint
        {
            this->logStringTooLong();
            stringContainer.free();
            stepInNumOfChars = 0;
        }
        else // it is possible to reallocate and reprint
        {
            if (false == stringContainer.allocate(stepInNumOfChars)) // allocation failed
            {
                this->logOutOfMem();
                stepInNumOfChars = 0;
            }
            else if (printF(stringContainer)) // success - printed (unfortunately not optimally)
            {
                result = true;
            }
        }
    }
    else // success - printed optimally
    {
        result = true;
    }
    stringContainer.step(stepInNumOfChars);

    return result;
}

int Logger::formatPrefix(const Logger::Type &type,
                         char *pStringBase,
                         const std::size_t &maxSize)
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

    return snprintf(pStringBase, maxSize, pFormat, hours, minutes, seconds, milliseconds, pType);
}

void Logger::logOutOfMem()
{
    static constexpr std::string_view string = "\e[31mlogger out of mem\e[0m\n";
    Logger::logFast(string);
}

void Logger::logStringTooLong()
{
    static constexpr std::string_view string = "\e[31mlog entry too long\e[0m\n";
    Logger::logFast(string);
}
