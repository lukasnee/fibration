#include "asciiStreamIF.hpp"
#include <cstring>
#include <cstdarg>
#include <cstdio>
#include <array>

AsciiStream::AsciiStream(IOStream &ioStream) : ioStream(ioStream) {}

bool AsciiStream::init()
{
    return this->ioStream.init();
}

void AsciiStream::deinit()
{
    this->ioStream.deinit(); // NOTE: result ignored
}

void AsciiStream::putChar(const char &c, TickType_t timeout)
{
    this->ioStream.push(reinterpret_cast<const std::uint8_t *>(&c), 1, timeout);
}

void AsciiStream::putString(const char *s, uint16_t len, TickType_t timeout)
{
    this->ioStream.push(reinterpret_cast<const std::uint8_t *>(s), len, timeout);
}

void AsciiStream::putString(const char *s, TickType_t timeout)
{
    this->putString(s, std::strlen(s), timeout);
}

char AsciiStream::getChar(TickType_t timeout)
{
    char c;
    this->ioStream.pull(reinterpret_cast<std::uint8_t &>(c), timeout);
    return c;
}

int AsciiStream::printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    std::array<char, Config::printfBufferSize> buffer;
    int charsPrinted = vsnprintf(buffer.data(), buffer.size(), fmt, args);
    if (charsPrinted)
    {
        this->putString(buffer.data());
    }
    va_end(args);

    return charsPrinted;
}