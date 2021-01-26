#pragma once

#include <cstdint>

#include "thread.hpp"

#include "uart2.hpp"
#include "stream.hpp"

//#include <cstddef>
#include <array>
#include <cstring>
#include <string_view>
using namespace std::string_view_literals;

class Shell : public cpp_freertos::Thread
{
public:
    static void start(Stream &stream, std::uint16_t stackDepth, BaseType_t priority);

    struct Command
    {
        const char *name;
        const char *help;
        int (*handler)(int argc, char *argv[]);
    };

private:
    static constexpr std::string_view prompt = "\e[34mFIB> "sv;

    static constexpr std::size_t rxBufferSize = 256;
    static constexpr std::size_t maxArgs = 16;
    static constexpr std::array<Shell::Command, 16> commands{{0}}; //todo external comnmands

    Shell(Stream &stream, std::uint16_t stackDepth, BaseType_t priority);

    virtual void Run() override;

    void resetRxBuffer(void);
    void receiveChar(char c);
    void sendPrompt(void);
    void echo(char c);
    void echo(const char *string);
    void echoEndLine();
    void echoLine(const char *string);
    void process(void);
    char lastChar(void);
    bool isRxBufferFull(void);
    const Command *findCommand(const char *name);
    int helpHandler(int argc, char *argv[]);
    std::size_t rxSize;
    std::array<char, rxBufferSize> rxBuffer;

    Stream &stream;
};
