#pragma once

#include "dataStreamIF.hpp"
#include "thread.hpp"
//#include <optional>

class AsciiStream
{
public:
    struct Config
    {
        static constexpr std::size_t printfBufferSize = 256;
    };

    AsciiStream(DataStreamIF &dataStream);

    bool init();
    void deinit();

    void putChar(const char &c, TickType_t timeout = portMAX_DELAY);
    void putString(const char *s, TickType_t timeout = portMAX_DELAY);
    void putString(const char *s, std::uint16_t len, TickType_t timeout = portMAX_DELAY);

    char getChar(TickType_t timeout = portMAX_DELAY);

    int printf(const char *fmt, ...);

private:
    DataStreamIF &dataStream;
};