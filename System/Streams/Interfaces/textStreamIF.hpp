#pragma once

#include "dataStreamIF.hpp"
#include "thread.hpp"
//#include <optional>

class TextStream
{
public:
    TextStream(DataStreamIF &dataStream);

    bool init();
    void deinit();

    void putc(char c, TickType_t timeout = portMAX_DELAY);
    void puts(const char *s, TickType_t timeout = portMAX_DELAY);
    void puts(const char *s, std::uint16_t len, TickType_t timeout = portMAX_DELAY);

    char getc(TickType_t timeout = portMAX_DELAY);

protected:
private:
    DataStreamIF &dataStream;
};