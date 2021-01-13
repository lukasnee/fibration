#pragma once

#define LOG_ENABLED 				true
#define LOG_VERBOSITY_LEVEL 		Log::VERBOSITY_2

#include <string_view>
#include <cstdarg>

#include "uart1.hpp"

class Log
{
public:
 
	static void init();

	enum Verbosity
	{
		VERBOSITY_0,
		VERBOSITY_1,
		VERBOSITY_2,
	};
	
	static void trace(const std::string_view context, const std::string_view fmt, ...);
	static void fatal(const std::string_view context, const std::string_view fmt, ...);
	static void system(const std::string_view context, const std::string_view fmt, ...);
	static void error(const std::string_view context, const std::string_view fmt, ...);
	static void warning(const std::string_view context, const std::string_view fmt, ...);
	static void info(const std::string_view context, const std::string_view fmt, ...);
	static void info(Verbosity verbosity, const std::string_view context, const std::string_view fmt, ...);

    static void DIRECT(const std::string_view rawMsg);
    static void clear();

 	// singleton pattern
	static Log& get() { static Log instance; return instance; };
	Log(Log const&) = delete;
	void operator=(Log const&) = delete;

    static void colorEnable() { logColored = true; };
    static void colorDisable() { logColored = false; };

	Log();
	~Log();

protected:
	static void syserr(const std::string_view context, const std::string_view fmt, ...);

private:	
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

	static void log(Verbosity verbosity, Type type, const std::string_view context, const std::string_view fmt, va_list arglist);
	static int formatPrefixAndContext(Type type, const std::string_view context, std::uint8_t * pOut, const std::size_t maxSize);

	static Uart1 * pStream;

    // config
    static bool logColored;

	static void logOutOfMem();
};
