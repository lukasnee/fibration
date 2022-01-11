#pragma once

#include "argVector.hpp"

#include <cstdint>
#include <array>

class ArgBuffer : public ArgVector
{
public:
    struct Config
    {
        static constexpr std::size_t bufferSize = 256;
    };

    using Buffer = std::array<char, Config::bufferSize>;

    ArgBuffer();
    ArgBuffer(std::size_t argc, const char *argv[]);
    ArgBuffer(const char *argString);

    bool copyFrom(std::size_t argc, const char *argv[]);
    bool resolveIntoArgs();
    bool restoreIntoString();

protected:
    Buffer buffer{'\0'};
};