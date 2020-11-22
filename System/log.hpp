#pragma once

#define LOG_ENABLED true

#include <string>
#include <cstdarg>

class Log
{
public:
	static void error(const std::string &context, const char *fmt, ... );
	static void warning(const std::string &context, const char *fmt, ... );
	static void message(const std::string &context, const char *fmt, ... );

protected:
	static void syserr(const std::string &context, const char *fmt, ... );

private:	
	typedef enum Type_ 
	{
		LOG_TYPE_SYSERR,
		LOG_TYPE_ERROR,
		LOG_TYPE_WARNING,
		LOG_TYPE_MESSAGE,
	LOG_TYPES_SIZE} Type;
	
	static void log(Type type, 
		const std::string &context, const char *fmt, va_list arglist);

};
