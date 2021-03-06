#pragma once

#include "dataStreamIF.hpp"
#include "thread.hpp"
//#include <optional>

class AsciiStream
{
public:
    AsciiStream(DataStreamIF &dataStream);

    bool init();
    void deinit();

    void putChar(const char &c, TickType_t timeout = portMAX_DELAY);
    void puts(const char *s, TickType_t timeout = portMAX_DELAY);
    void puts(const char *s, std::uint16_t len, TickType_t timeout = portMAX_DELAY);

    char getc(TickType_t timeout = portMAX_DELAY);

private:
    DataStreamIF &dataStream;
};