#include "argBuffer.hpp"
#include <cstring>

ArgBuffer::ArgBuffer() : ArgVector() {}

ArgBuffer::ArgBuffer(std::size_t argc, const char *argv[])
{
    this->copyFrom(argc, argv);
}

bool ArgBuffer::copyFrom(std::size_t argc, const char *argv[])
{
    bool result = true;

    this->count = 0;
    this->args.fill(nullptr);
    std::size_t bufferLeft = this->buffer.size();
    for (const char **argvIt = argv; (*argvIt && argvIt < (argv + argc) && count < this->args.size()); argvIt++)
    {
        const auto strLength = std::strlen(*argvIt);
        if (strLength < bufferLeft)
        {
            char *pArg = &this->buffer[this->buffer.size() - bufferLeft];
            std::strncpy(pArg, *argvIt, bufferLeft);
            bufferLeft -= strLength + sizeof('\0');
            this->args[this->count++] = pArg;
        }
        else
        {
            result = false;
            break;
        }
    }

    return result;
}

ArgBuffer::ArgBuffer(const char *argString)
{
    std::strncpy(this->buffer.data(), argString, this->buffer.size());
    this->resolveIntoArgs();
}

bool ArgBuffer::resolveIntoArgs()
{
    return this->resolveStringToArgs(reinterpret_cast<char *>(this->buffer.data()), this->buffer.size());
}

bool ArgBuffer::restoreIntoString()
{
    bool result = false;

    for (std::size_t argIt = 1; argIt < this->count; argIt++)
    {
        std::size_t charBufferOffset = this->args[argIt] - this->buffer.data();
        if (charBufferOffset < this->buffer.size())
        {
            if (this->buffer[charBufferOffset] == '\0')
            {
                this->buffer[charBufferOffset] = ' ';
            }
            result = true;
        }
        else
        {
            break;
        }
    }

    return result;
}
