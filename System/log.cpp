#include "log.hpp"
#include "system.hpp"

#include <string>
#include <cstdio>
#include <cstdarg>

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

void Log::syserr(const std::string &context, const std::string &fmt, ... )
{
	if(LOG_ENABLED)
	{
		va_list arglist;
		va_start(arglist, fmt);	
		log(VERBOSITY_0, LOG_TYPE_SYSERR, context, fmt, arglist);
		va_end(arglist);
	}
};

void Log::log(Verbosity verbosity, 
	Type type, const std::string &context, const std::string &fmt, va_list arglist)
{
	if(LOG_ENABLED)
	{

		if(verbosity <= LOG_VERBOSITY_LEVEL)
		{
			// try just char pointer array... maybe more efficient?
			const std::string arLogType[LOG_TYPES_SIZE] = {
				{"\e[39mSYSERR"}, 	// white
				{"\e[31mERROR"}, 	// red
				{"\e[33mWARNING"}, 	// yellow
				{"\e[36mINFO"}, 	// cyan
			};

			uint32_t logTick = System::getTick();

			/* todo could be very inefficient using std::string (memory and 
			perfomance wise), test this. maybe try https://github.com/fmtlib/fmt 
			for printig and formatting */
			printf("%02lu:%02lu.%03lu\t%s\e[0m\t%s\t",
				logTick/1000/60, // minutes 
				logTick/1000%60, // seconds
				System::getTick()%1000, // milliseconds
				arLogType[type].c_str(),
				context.c_str());
			vprintf(fmt.c_str(), arglist);
			printf("\n\r");
		}
	}
};
