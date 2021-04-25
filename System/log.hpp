#pragma once

#define LOG_ENABLED true
#define LOG_VERBOSITY_LEVEL Log::VERBOSITY_2

#include "Streams/uartTextService.hpp"
#include <string_view>
#include <cstdarg>

class Log
{
public:
    static void setUartService(UartService *pUartService);

    static void trace(const std::string_view context, const std::string_view fmt, ...);
    static void fatal(const std::string_view context, const std::string_view fmt, ...);
    static void system(const std::string_view context, const std::string_view fmt, ...);
    static void error(const std::string_view context, const std::string_view fmt, ...);
    static void warning(const std::string_view context, const std::string_view fmt, ...);
    static void info(const std::string_view context, const std::string_view fmt, ...);

    enum Verbosity
    {
        VERBOSITY_0,
        VERBOSITY_1,
        VERBOSITY_2,
    };
    static void info(Verbosity verbosity, const std::string_view context, const std::string_view fmt, ...);

    static void direct(const std::string_view rawMsg);
    static void directFromISR(const std::string_view staticStrv);
    static void clear();

    void colorEnable() { this->logColored = true; };
    void colorDisable() { this->logColored = false; };

protected:
    void syserr(const std::string_view context, const std::string_view fmt, ...);

private:
    Log(){}
    ~Log(){}
    Log(Log const &) = delete;
    void operator=(Log const &) = delete;

    static Log &getInstance();

    typedef enum Type_
    {
        LOG_TYPE_TRACE,
        LOG_TYPE_FATAL,
        LOG_TYPE_SYSTEM,
        LOG_TYPE_ERROR,
        LOG_TYPE_WARNING,
        LOG_TYPE_INFO,

        LOG_TYPE_MAX = LOG_TYPE_INFO
    } Type;

    static const std::string_view arLogType[LOG_TYPE_MAX + 1];

    void log(Verbosity verbosity, Type type, const std::string_view context, const std::string_view fmt, va_list arglist);
    int formatPrefixAndContext(Type type, const std::string_view context, char *pOut, const std::size_t maxSize);

    // config
    bool logColored = true;

    void logOutOfMem();

    UartService *pUartService;
};
