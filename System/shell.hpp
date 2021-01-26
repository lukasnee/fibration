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
    static constexpr std::string_view prompt = "FIB> "sv;

    static constexpr std::size_t rxBufferSize = 256;
    static constexpr std::size_t maxArgs = 16;
    static constexpr std::array<Shell::Command, 16> commands{{0}}; //todo external comnmands

    Shell(Stream &stream, std::uint16_t stackDepth, BaseType_t priority);

    virtual void Run() override;

    void echo(char c);
    void echo(const char *string);
    void resetRxBuffer(void);
    char lastChar(void);
    bool isRxBufferFull(void);
    const Command *findCommand(const char *name);
    void sendPrompt(void);
    void process(void);
    void receiveChar(char c);
    void putLine(const char *string);
    int helpHandler(int argc, char *argv[]);
    std::size_t rxSize;
    std::array<char, rxBufferSize> rxBuffer;

    Stream &stream;
};
