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

    void putChar(const char &c);
    void putString(const std::string_view string);
    void putString(const char *s, std::uint16_t len);
    void putString(const char *s);
    int printf(const char *fmt, const va_list &argList);
    int printf(const char *fmt, ...);

    char getChar(TickType_t timeout = portMAX_DELAY, OsResource::Context context = OsResource::Context::undefined);

private:
    IOStream &ioStream;
};