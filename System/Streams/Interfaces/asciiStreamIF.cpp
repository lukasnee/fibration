#include "asciiStreamIF.hpp"
#include <cstring>

AsciiStream::AsciiStream(DataStreamIF &dataStream) : dataStream(dataStream) {}

bool AsciiStream::init()
{
    return this->dataStream.init();
}

void AsciiStream::deinit()
{
    this->dataStream.deinit(); // NOTE: result ignored
}

void AsciiStream::putChar(const char &c, TickType_t timeout)
{
    this->dataStream.push(reinterpret_cast<const std::uint8_t *>(&c), 1, timeout);
}

void AsciiStream::puts(const char *s, uint16_t len, TickType_t timeout)
{
    this->dataStream.push(reinterpret_cast<const std::uint8_t *>(s), len, timeout);
}

void AsciiStream::puts(const char *s, TickType_t timeout)
{
    puts(s, std::strlen(s), timeout);
}

char AsciiStream::getc(TickType_t timeout)
{
    char c;
    this->dataStream.pull(reinterpret_cast<std::uint8_t&>(c), timeout);
    return c;
}
