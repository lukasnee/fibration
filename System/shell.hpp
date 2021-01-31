#pragma once

#include <cstdint>

#include "thread.hpp"

#include "uart2.hpp"
#include "stream.hpp"

//#include <cstddef>
#include <array>
#include <cstring>
#include <cstdarg>
#include <string_view>
using namespace std::string_view_literals;

namespace ShellConfig {
    constexpr bool regularResponseIsEnabled = true;
} //namescape Shell::Config

class Shell : public cpp_freertos::Thread
{
public:
    static void start(Stream &stream, std::uint16_t stackDepth, BaseType_t priority);

    int printf(const char *fmt, ...);
    void printc(const char c);

    struct Command
    {
        const char *name;
        const char *help;
        int (*handler)(Shell &shell, int argc, char *argv[]);
    };

private:
    // prompt in blue, command entry in yellow
    static constexpr std::string_view prompt = "\e[34mFIB> \e[33m"sv;

    static constexpr std::size_t txBufferSize = 256;
    static constexpr std::size_t rxBufferSize = 256;
    static constexpr std::size_t maxArgs = 16;

    static const std::array<Shell::Command, 4> shellCmds;

    Shell(Stream &stream, std::uint16_t stackDepth, BaseType_t priority);

    virtual void Run() override;

    void echo(char c);
    void echo(const char *string);
    void echoEndLine();
    void resetRxBuffer(void);
    void receiveChar(char c);
    void promptNew(void);
    void printPrompt(void);
    void echoLine(const char *string);
    void process(void);
    char lastChar(void);
    bool isRxBufferFull(void);
    const Command *findCommand(const char *name);
    int helpHandler(Shell &shell, int argc, char *argv[]);
    std::size_t rxSize;
    std::array<char, rxBufferSize> rxBuffer;

    Stream &stream;
};
