#pragma once

#include "ioStream.hpp"
#include "thread.hpp"

#include <string_view>
//#include <optional>

class AsciiStream
{
public:
    struct Config
    {
        static constexpr std::size_t printfBufferSize = 256;
    };

    AsciiStream(IOStream &ioStream);

    bool init();
    void deinit();

    void putChar(const char &c, TickType_t timeout = portMAX_DELAY);
    void putString(const std::string_view string, TickType_t timeout = portMAX_DELAY);
    void putString(const char *s, std::uint16_t len, TickType_t timeout = portMAX_DELAY);
    void putString(const char *s, TickType_t timeout = portMAX_DELAY);
    int printf(const char *fmt, const va_list &argList);
    int printf(const char *fmt, ...);

    char getChar(TickType_t timeout = portMAX_DELAY);

private:
    IOStream &ioStream;
};