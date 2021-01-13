#include "log.hpp"
#include "system.hpp"
#include "peripherals.hpp"

#include "dmaQueue.hpp"

#include <string_view>
#include <cstdio>
#include <cstdarg>

Uart1 * Log::pStream = nullptr;

bool Log::logColored = true;

void Log::init()
{
    Log::pStream = &Periph::getUart1();
}

void Log::trace(const std::string_view context, const std::string_view fmt, ...)
{
    if(LOG_ENABLED)
    {
        va_list arglist;
        va_start(arglist, fmt);
        log(VERBOSITY_0, LOG_TYPE_TRACE, context, fmt, arglist);
        va_end(arglist);
    }
};

void Log::fatal(const std::string_view context, const std::string_view fmt, ...)
{
    if(LOG_ENABLED)
    {
        va_list arglist;
        va_start(arglist, fmt);
        log(VERBOSITY_0, LOG_TYPE_FATAL, context, fmt, arglist);
        va_end(arglist);
    }
};

void Log::system(const std::string_view context, const std::string_view fmt, ...)
{
    if(LOG_ENABLED)
    {
        va_list arglist;
        va_start(arglist, fmt);
        log(VERBOSITY_0, LOG_TYPE_SYSTEM, context, fmt, arglist);
        va_end(arglist);
    }
};

void Log::error(const std::string_view context, const std::string_view fmt, ... )
{
    if(LOG_ENABLED)
    {
        va_list arglist;
        va_start(arglist, fmt);
        log(VERBOSITY_0, LOG_TYPE_ERROR, context, fmt, arglist);
        va_end(arglist);
    }
};

void Log::warning(const std::string_view context, const std::string_view fmt, ... )
{
    if(LOG_ENABLED)
    {
        va_list arglist;
        va_start(arglist, fmt);	
        log(VERBOSITY_0, LOG_TYPE_WARNING, context, fmt, arglist);
        va_end(arglist);
    }
};

void Log::info(const std::string_view context, const std::string_view fmt, ... )
{
    if(LOG_ENABLED)
    {
        va_list arglist;
        va_start(arglist, fmt);	
        log(VERBOSITY_0, LOG_TYPE_INFO, context, fmt, arglist);
        va_end(arglist);
    }
};

void Log::info(Verbosity verbosity, 
    const std::string_view context, const std::string_view fmt, ... )
{
    if(LOG_ENABLED)
    {
        va_list arglist;
        va_start(arglist, fmt);
        log(verbosity, LOG_TYPE_INFO, context, fmt, arglist);
        va_end(arglist);
    }
}

void Log::DIRECT(const std::string_view staticStrv)
{
    if(LOG_ENABLED)
    {
        pStream->txPush(reinterpret_cast<const uint8_t*>(staticStrv.data()), staticStrv.length(), false, true);
    }
}

void Log::clear()
{
    static const std::string_view staticStrv = "\r\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    DIRECT(staticStrv);
}

void Log::logOutOfMem()
{
    static const std::string_view staticStrv = "\e[31mstream out of mem\e[0m\r\n";
    DIRECT(staticStrv);
}

void Log::log(Verbosity verbosity, Type type, const std::string_view context, const std::string_view fmt, va_list arglist)
{
    if(LOG_ENABLED)
    {
        if(verbosity <= LOG_VERBOSITY_LEVEL)
        {
            int allocSize = 0x80; // a typical log line should fit...

            std::uint8_t * pStrBase = new std::uint8_t[allocSize];
            if(pStrBase == nullptr)
            {
                logOutOfMem();
                return;
            }
            else
            {
                std::uint8_t * pStrHead = pStrBase;
                int maxStrLen = allocSize - sizeof('\0');
                std::uint32_t strLen = 0;

                // try to print prefix and context
                int len = Log::formatPrefixAndContext(type, context, pStrHead, maxStrLen);
                if (len > maxStrLen)
                {
                    // did not fit - reallocate needed length buffer and try again
                    delete[] pStrBase;
                    if(len < 0x400) // some limitation
                    {
                        allocSize = len + sizeof('\0');
                        pStrBase = new std::uint8_t[allocSize];
                        if(pStrBase == nullptr)
                        {
                            Log::logOutOfMem();
                            return;
                        }
                        else
                        {
                            pStrHead = pStrBase;
                            maxStrLen = allocSize - sizeof('\0');
                            strLen = 0;
                            Log::formatPrefixAndContext(type, context, pStrHead, maxStrLen);
                        }
                    }
                }
                
                strLen += len; // notice printed prefix and context
                pStrHead += len; maxStrLen -= len;

                // try to print log message
                len = vsnprintf(reinterpret_cast<char*>(pStrHead), maxStrLen, fmt.data(), arglist);
                if (len > maxStrLen)
                {
                    // did not fit - push previous print, and try to print log message to a new mem alloc
                    pStream->txPush(pStrBase, strLen, false, false);
                    if(len < 0x400) // some limitation
                    {
                        allocSize = len + sizeof('\0');
                        pStrBase = new std::uint8_t[allocSize]; // should fit
                        if(pStrBase == nullptr)
                        {
                            logOutOfMem();
                            return;
                        }
                        else
                        {
                            pStrHead = pStrBase;
                            maxStrLen = allocSize - sizeof('\0');
                            strLen = 0;
                            vsnprintf(reinterpret_cast<char*>(pStrHead), maxStrLen, fmt.data(), arglist);
                        }
                    }
                }
                
                strLen += len; // notice printed log message
                pStrHead += len; maxStrLen -= len;

                // push whats left
                pStream->txPush(pStrBase, strLen, false, false);
            }
        }
    }
}

// try just char pointer array... maybe more efficient?
constexpr std::string_view Log::arLogType[LOG_TYPE_MAX + 1] = {
    {"\e[32mTRACE"}, // green
    {"\e[35mFATAL"}, // magenta
    {"\e[34mSYS  "}, // blue
    {"\e[31mERROR"}, // red
    {"\e[33mWARN "}, // yellow
    {"\e[36mINFO "}, // cyan
};

int Log::formatPrefixAndContext(Type type, const std::string_view context, std::uint8_t * pStrBase, const std::size_t maxSize)
{
    std::uint32_t tick = FibSys::getSysTick();
    std::uint32_t minutes = tick / 1000 / 60;
    std::uint32_t seconds = tick / 1000 % 60;
    std::uint32_t milliseconds = tick % 1000;

    // print log prefix
    return snprintf(reinterpret_cast<char*>(pStrBase), 
        maxSize, logColored ? "\n\r%02lu:%02lu.%03lu %s \e[1m%s\e[0m " : "\n\r%02lu:%02lu.%03lu %s %s ",
        minutes, seconds, milliseconds, 
        arLogType[type].data() + (logColored ? 0 : 5), context.data());
}
