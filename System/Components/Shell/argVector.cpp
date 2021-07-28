
#include "argVector.hpp"

#include <cstdio>
#include <limits>

ArgVector::ArgVector(std::size_t argc, const char **argv)
{
    if (argv && argc)
    {
        std::size_t i = 0;
        for (auto &arg : this->args)
        {
            if (i < argc)
            {
                arg = argv[this->count++];
            }
            else
            {
                break;
            }
        }
    }
};

ArgVector::ArgVector(char *pString, std::size_t stringLength)
{
    resolveStringToArgs(pString, stringLength);
}

bool ArgVector::resolveStringToArgs(char *pString, std::size_t stringLength)
{
    bool result = false;

    this->count = 0;
    this->args.fill(nullptr);

    if (pString && stringLength)
    {
        char *nextArg = nullptr;
        for (char *pStringHead = pString; static_cast<std::size_t>(pStringHead - pString) < stringLength; ++pStringHead)
        {
            char *const pChar = pStringHead;
            if (*pChar == ' ' || *pChar == '\0')
            {
                *pChar = '\0';
                if (nextArg)
                {
                    if (this->count < this->args.size())
                    {
                        this->args[this->count++] = nextArg;
                        nextArg = nullptr;
                        result = true;
                    }
                    else
                    {
                        result = false;
                        break;
                    }
                }
            }
            else if (!nextArg)
            {
                nextArg = pChar;
            }
        }
    }

    return result;
}

bool ArgVector::printTo(char *pBufferOut, std::size_t bufferOutSize, const char *delimiter, bool nullSeparated)
{
    bool result = false;

    if (this->count && this->args[0] && pBufferOut && bufferOutSize &&
        bufferOutSize <= std::numeric_limits<int>::max() && delimiter)
    {
        std::array<char, (Config::maxDelimiterLength + sizeof("%s"))> fmt;
        int snprintfResult = snprintf(fmt.data(), fmt.size(), "%%s%s", delimiter);
        if (snprintfResult > 0 && static_cast<std::size_t>(snprintfResult) < fmt.size())
        {
            char *pBufferOutHead = pBufferOut;
            int bufferOutSizeLeft = static_cast<int>(bufferOutSize);

            std::size_t cnt = this->count;

            for (auto &arg : this->args)
            {
                if (!cnt || arg == nullptr || bufferOutSizeLeft <= 0)
                {
                    result = true;
                    break;
                }
                else
                {
                    int snprintfResult = snprintf(pBufferOutHead, bufferOutSizeLeft, arg);
                    if (snprintfResult > 0)
                    {
                        pBufferOutHead += snprintfResult;
                        bufferOutSizeLeft -= snprintfResult;

                        if (nullSeparated && bufferOutSizeLeft)
                        {
                            *pBufferOutHead = '\0';
                            pBufferOutHead += sizeof('\0');
                            bufferOutSizeLeft -= sizeof('\0');
                        }
                    }

                    if (cnt > 1)
                    {
                        int snprintfResult = snprintf(pBufferOutHead, bufferOutSizeLeft, delimiter);
                        if (snprintfResult > 0)
                        {
                            pBufferOutHead += snprintfResult;
                            bufferOutSizeLeft -= snprintfResult;
                        }
                    }

                    cnt--;
                }
            }
        }
    }

    return result;
}
