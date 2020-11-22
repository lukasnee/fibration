#pragma once

#define LOG_ENABLED true

#include <string>
#include <cstdio>
#include <cstdarg>

class Log
{
public:

	typedef enum Type_ 
	{
		LOG_TYPE_ERROR,
		LOG_TYPE_WARNING,
		LOG_TYPE_MESSAGE,
	LOG_TYPES_SIZE} Type;

	static void error(const std::string &context, const char *fmt, ... )
	{
		va_list arglist;
		va_start(arglist, fmt);
		log(LOG_TYPE_ERROR, context, fmt, arglist);
		va_end(arglist);
	};

	static void warning(const std::string &context, const char *fmt, ... )
	{
		va_list arglist;
		va_start(arglist, fmt);	
		log(LOG_TYPE_WARNING, context, fmt, arglist);
		va_end(arglist);
	};

	static void message(const std::string &context, const char *fmt, ... )
	{
		va_list arglist;
		va_start(arglist, fmt);	
		log(LOG_TYPE_MESSAGE, context, fmt, arglist);
		va_end(arglist);
	};

private:
	static void log(Type type, 
		const std::string &context, const char *fmt, va_list arglist)
	{
		if(LOG_ENABLED)
		{
			// todo real timeStamp
			const std::string timeStamp = "0.123"; // ms	

			// try just char pointer array... maybe more efficient?
			const std::string arStrType[LOG_TYPES_SIZE] = { 
				"ERR", "WRN", "MSG" };

			if(type >= LOG_TYPES_SIZE)
			{
				printf("SYSERR: log type undefined.");
			}
			else
			{
				/* todo could be very inefficient using std::string, test this
				 todo try https://github.com/fmtlib/fmt for printig and 
				 formatting */
				printf("%s, %s, %s, ", timeStamp.c_str(), 
					arStrType[type].c_str(), context.c_str());
					
				vprintf(fmt, arglist);
				putchar('\n');
				putchar('\r');
			}
		}
	};
};
