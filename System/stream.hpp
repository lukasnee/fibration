#pragma once

#include <array>
#include <optional>
#include "thread.hpp"
#include "queue.hpp"

class Stream
{
public:
    Stream(){};

    void setOwner(cpp_freertos::Thread *pOwner) { this->pOwner = pOwner; };

    virtual void putc(char c) = 0;
    virtual void puts(const char *s) = 0;
    virtual void puts(const char *s, std::uint16_t len) = 0;

    virtual char getc(void) = 0;
    // TODO
    // virtual std::optional<char *> gets(void) = 0;
    // virtual std::optional<char *> gets(std::uint16_t len) = 0;

protected:
    cpp_freertos::Thread *pOwner;
};