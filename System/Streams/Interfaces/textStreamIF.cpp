#include "textStreamIF.hpp"
#include <cstring>

TextStream::TextStream(DataStreamIF &dataStream) : dataStream(dataStream) {}

bool TextStream::init()
{
    return this->dataStream.init();
}

void TextStream::deinit()
{
    this->dataStream.deinit(); // NOTE: result ignored
}

void TextStream::putc(char c, TickType_t timeout)
{
    this->dataStream.push(reinterpret_cast<std::uint8_t *>(&c), 1, timeout);
}

void TextStream::puts(const char *s, uint16_t len, TickType_t timeout)
{
    this->dataStream.push(reinterpret_cast<const std::uint8_t *>(s), len, timeout);
}

void TextStream::puts(const char *s, TickType_t timeout)
{
    puts(s, std::strlen(s), timeout);
}

char TextStream::getc(TickType_t timeout)
{
    char c;
    this->dataStream.pull(reinterpret_cast<std::uint8_t&>(c), timeout);
    return c;
}
