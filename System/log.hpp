#pragma once

#define LOG_ENABLED 				true
#define LOG_VERBOSITY_LEVEL 		Log::VERBOSITY_2

#include <string>
#include <cstdarg>

class Log
{
public:
	static void error(const std::string &context, const std::string &fmt, ... );
	static void warning(const std::string &context, const std::string &fmt, ... );
	static void info(const std::string &context, const std::string &fmt, ... );

	enum Verbosity
	{
		VERBOSITY_0,
		VERBOSITY_1,
		VERBOSITY_2,
	};

	static void info(Verbosity verbosity, const std::string &context, const std::string &fmt, ... );

protected:
	static void syserr(const std::string &context, const std::string &fmt, ... );

private:	
	typedef enum Type_ 
	{
		LOG_TYPE_SYSERR,
		LOG_TYPE_ERROR,
		LOG_TYPE_WARNING,
		LOG_TYPE_INFO,
	LOG_TYPES_SIZE} Type;
	
static void log(Verbosity verbosity, 
	Type type, const std::string &context, const std::string &fmt, va_list arglist);

};
