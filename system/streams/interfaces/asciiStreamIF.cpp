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

void AsciiStream::putChar(const char &c)
{
    this->ioStream.push(reinterpret_cast<const std::uint8_t *>(&c), 1);
}

void AsciiStream::putString(const std::string_view string)
{
    this->putString(string.data(), string.length());
}

void AsciiStream::putString(const char *s)
{
    this->putString(s, std::strlen(s));
}

void AsciiStream::putString(const char *s, uint16_t len)
{
    this->ioStream.push(reinterpret_cast<const std::uint8_t *>(s), len);
}

int AsciiStream::printf(const char *fmt, const va_list& argList)
{
    std::array<char, Config::printfBufferSize> buffer;
    int charsPrinted = vsnprintf(buffer.data(), buffer.size(), fmt, argList);
    if (charsPrinted > 0)
    {
        this->putString(buffer.data(), static_cast<std::uint16_t>(charsPrinted));
    }

    return charsPrinted;
}

int AsciiStream::printf(const char *fmt, ...)
{
    va_list argList;
    va_start(argList, fmt);
    int charsPrinted = this->printf(fmt, argList);
    va_end(argList);

    return charsPrinted;
}

char AsciiStream::getChar(TickType_t timeout, OsResource::Context context)
{
    char c = '\0';
    this->ioStream.pull(reinterpret_cast<std::uint8_t &>(c), timeout, context);
    return c;
}