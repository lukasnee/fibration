#include "argBuffer.hpp"
#include <cstring>

ArgBuffer::ArgBuffer() : ArgVector() {}

ArgBuffer::ArgBuffer(std::size_t argc, const char **argv) : ArgVector(argc, argv)
{
    this->printTo(this->buffer.data(), this->buffer.size(), "", true);
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
