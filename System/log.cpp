#include "log.hpp"
#include "system.hpp"

#include "dmaQueue.hpp"

#include <string>
#include <cstdio>
#include <cstdarg>

Uart1 * Log::pStream = nullptr;

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
	pStream->queuePush(reinterpret_cast<const std::uint8_t*>(str.c_str()), str.size());
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

			std::uint8_t * pStrBase = nullptr;
			std::size_t allocSize = 0;

			if(pStream->alloc(0, &pStrBase, &allocSize) == false)
			{
				logOutOfMem();
				return;
			}
			else
			{
				std::uint8_t * pStrHead = pStrBase;
				std::size_t maxStrLen = allocSize - sizeof('\0');
				std::uint32_t strLen = 0;

				int len;
				// try
				len = formatPrefix(type, context, pStrHead, maxStrLen);
				if (static_cast<std::size_t>(len) >= maxStrLen) 
				{
					// did not fit - reallocate needed length buffer
					pStream->free(pStrBase);
					pStream->alloc(allocSize, &pStrBase, &allocSize);
					pStrHead = pStrBase;
					maxStrLen = allocSize - sizeof('\0');
					strLen = 0;

					// try again
					len = formatPrefix(type, context, pStrHead, maxStrLen);
					if(static_cast<std::size_t>(len) >= maxStrLen)
					{
						logOutOfMem();
						return;
					}
				}

				pStrHead += len;
				maxStrLen -= len;
				strLen += len;

				// print user message
				len = vsnprintf(reinterpret_cast<char*>(pStrHead), 
					allocSize, fmt.c_str(), arglist);
				if (static_cast<std::size_t>(len) >= maxStrLen)
				{
					// push only prefix, then push user message separately...
					pStream->queuePush(pStrBase, strLen + sizeof('\0'));
					if(pStream->alloc(0, &pStrBase, &allocSize) == false)
					{
						logOutOfMem();
						return;
					}
					pStrHead = pStrBase;
					maxStrLen = allocSize - sizeof('\0');
					strLen = 0;
				}

				pStrHead += len;
				maxStrLen -= len;
				strLen += len;

				// push whats left
				pStream->queuePush(pStrBase, strLen + sizeof('\0'));
			}
		}
	}
}

// try just char pointer array... maybe more efficient?
const std::string Log::arLogType[LOG_TYPE_MAX + 1] = {
	{"\e[95m TRACE"}, 	// light magenta
	{"\e[91m FATAL"}, 	// light red
	{"\e[34mSYSTEM"}, 	// blue
	{"\e[31m ERROR"}, 	// red
	{"\e[33mWRNING"}, 	// yellow
	{"\e[36m  INFO"}, 	// cyan
};

int Log::formatPrefix(Type type, const std::string &context, std::uint8_t * pStrBase, const std::size_t maxSize)
{
	std::uint32_t tick = FibSys::getTick();
	std::uint32_t minutes = tick / 1000 / 60;
	std::uint32_t seconds = tick / 1000 % 60;
	std::uint32_t milliseconds = tick % 1000;

	// print log prefix
	return snprintf(reinterpret_cast<char*>(pStrBase), 
		maxSize, "\n\r%02lu:%02lu.%03lu %s \e[1m%s\e[0m ",
		minutes, seconds, milliseconds, 
		arLogType[type].c_str(), context.c_str());
}

