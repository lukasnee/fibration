#include "log.hpp"
#include "system.hpp"

#include "dmaQueue.hpp"

#include <string>
#include <cstdio>
#include <cstdarg>

Uart1 * Log::pStream = nullptr;

bool Log::logColored = true;

void Log::init()
{
    Log::pStream = &Periph::getUart1();
}

void Log::trace(const std::string &context, const std::string &fmt, ...)
{
    if(LOG_ENABLED)
    {
        va_list arglist;
        va_start(arglist, fmt);
        log(VERBOSITY_0, LOG_TYPE_TRACE, context, fmt, arglist);
        va_end(arglist);
    }
};

void Log::fatal(const std::string &context, const std::string &fmt, ...)
{
    if(LOG_ENABLED)
    {
        va_list arglist;
        va_start(arglist, fmt);
        log(VERBOSITY_0, LOG_TYPE_FATAL, context, fmt, arglist);
        va_end(arglist);
    }
};

void Log::system(const std::string &context, const std::string &fmt, ...)
{
    if(LOG_ENABLED)
    {
        va_list arglist;
        va_start(arglist, fmt);
        log(VERBOSITY_0, LOG_TYPE_SYSTEM, context, fmt, arglist);
        va_end(arglist);
    }
};

void Log::error(const std::string &context, const std::string &fmt, ... )
{
    if(LOG_ENABLED)
    {
        va_list arglist;
        va_start(arglist, fmt);
        log(VERBOSITY_0, LOG_TYPE_ERROR, context, fmt, arglist);
        va_end(arglist);
    }
};

void Log::warning(const std::string &context, const std::string &fmt, ... )
{
    if(LOG_ENABLED)
    {
        va_list arglist;
        va_start(arglist, fmt);	
        log(VERBOSITY_0, LOG_TYPE_WARNING, context, fmt, arglist);
        va_end(arglist);
    }
};

void Log::info(const std::string &context, const std::string &fmt, ... )
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
    const std::string &context, const std::string &fmt, ... )
{
    if(LOG_ENABLED)
    {
        va_list arglist;
        va_start(arglist, fmt);
        log(verbosity, LOG_TYPE_INFO, context, fmt, arglist);
        va_end(arglist);
    }
}

void Log::logOutOfMem()
{
    const std::string str = "\e[31mstream out of mem\e[0m\r\n";
    pStream->txPush(reinterpret_cast<const std::uint8_t*>(str.c_str()), str.size(), );
}

void Log::log(Verbosity verbosity, Type type, const std::string &context, const std::string &fmt, va_list arglist)
{
    if(LOG_ENABLED)
    {
        if(verbosity <= LOG_VERBOSITY_LEVEL)
        {
            /* todo could be very inefficient using std::string (memory and
            perfomance wise), test this. maybe try https://github.com/fmtlib/fmt
            for printig and formatting */

            const int allocSize = 0x80; // a typical log line should fit...
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
                int len = formatPrefixAndContext(type, context, pStrHead, maxStrLen);
                if (len > maxStrLen)
                {
                    // did not fit - reallocate needed length buffer and try again
                    delete[] pStrBase;
                    pStrBase = new std::uint8_t[len + sizeof('\0')];
                    if(pStrBase == nullptr)
                    {
                        logOutOfMem();
                        return;
                    }
                    else
                    {
                        pStrHead = pStrBase;
                        maxStrLen = len;
                        strLen = 0;
                        formatPrefixAndContext(type, context, pStrHead, maxStrLen);
                    }
                }
                
                strLen += len; // notice printed prefix and context
                pStrHead += len; maxStrLen -= len;

                // try to print log message
                len = vsnprintf(reinterpret_cast<char*>(pStrHead), maxStrLen, fmt.c_str(), arglist);
                if (len > maxStrLen)
                {
                    // did not fit - push previous print, and try to print log message to a new mem alloc
                    pStream->txPush(pStrBase, strLen + sizeof('\0'));
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
                        vsnprintf(reinterpret_cast<char*>(pStrHead), maxStrLen, fmt.c_str(), arglist);
                    }
                }
                strLen += len; // notice printed log message
                pStrHead += len; maxStrLen -= len;

                // push whats left
                pStream->txPush(pStrBase, strLen + sizeof('\0'));
            }
        }
    }
}

// try just char pointer array... maybe more efficient?
const std::string Log::arLogType[LOG_TYPE_MAX + 1] = {
    {"\e[32mTRACE"}, 	// green
    {"\e[35mFATAL"}, 	// magenta
    {"\e[34mSYS  "}, 	// blue
    {"\e[31mERROR"}, 	// red
    {"\e[33mWARN "}, 	// yellow
    {"\e[36mINFO "}, 	// cyan
};

int Log::formatPrefixAndContext(Type type, const std::string &context, std::uint8_t * pStrBase, const std::size_t maxSize)
{
    std::uint32_t tick = FibSys::getTick();
    std::uint32_t minutes = tick / 1000 / 60;
    std::uint32_t seconds = tick / 1000 % 60;
    std::uint32_t milliseconds = tick % 1000;

    // print log prefix
    return snprintf(reinterpret_cast<char*>(pStrBase), 
        maxSize, logColored ? "\n\r%02lu:%02lu.%03lu %s \e[1m%s\e[0m " : "\n\r%02lu:%02lu.%03lu %s %s ",
        minutes, seconds, milliseconds, 
        arLogType[type].c_str() + (logColored ? 0 : 5), context.c_str());
}

