#include "argBuffer.hpp"

ArgBuffer::ArgBuffer() : ArgVector() {}

ArgBuffer::ArgBuffer(std::size_t argc, const char **argv) : ArgVector(argc, argv) 
{
    this->printTo(this->buffer.data(), this->buffer.size(), "", true);
}

bool ArgBuffer::resolveIntoArgs()
{
    return this->resolveStringToArgs(reinterpret_cast<char *>(this->buffer.data()), this->buffer.size());
}
