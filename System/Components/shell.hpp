#pragma once

#include <cstdint>

#include "thread.hpp"

#include "uart2.hpp"
#include "textStreamInterface.hpp"

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
    static void start(TextStreamInterface &textStream, std::uint16_t stackDepth, BaseType_t priority);

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

    static const std::array<Shell::Command, 6> shellCmds;

    Shell(TextStreamInterface &textStream, std::uint16_t stackDepth, BaseType_t priority);

    virtual void Run() override;

    void echo(char c, std::size_t timesToRepeat = 1);
    void echo(const char *string, std::size_t timesToRepeat = 1);
    void echoData(const char *pData, const std::size_t len, std::size_t timesToRepeat = 1);
    void echoEndLine();
    void resetRxBuffer();
    bool receiveChar(char c);
    void promptNew();
    void printPrompt();

    bool rxBufferCursorOnBase();
    bool rxBufferCursorNotOnBase();
    bool rxBufferCursorOnTail();
    bool rxBufferCursorNotOnTail();
    bool rxBufferIsEmpty();
    bool rxBufferIsNotEmpty();
    bool rxBufferIsFull();
    bool rxBufferIsNotFull();
    char rxBufferCharOnCursor();
    char rxBufferLastChar();
    bool rxBufferCursorStep();
    bool rxBufferCursorStepBack();

    bool visualCursorStep();
    bool visualCursorStepBack();

    bool cursorStep();
    bool cursorStepBack();

    bool insert();
    bool deleteChar();

    void echoLine(const char *string);
    bool processAnsiCursorControl(char c);
    bool processAnsiEscapeSequences(char c);
    void processRxBufferIntoArgs();
    const Command *findCommand(const char *name);
    int helpHandler(Shell &shell, int argc, char *argv[]);
    std::size_t rxCursorIdx;
    std::size_t rxCharsTotal;
    std::array<char, rxBufferSize> rxBuffer;
    struct Config {
        bool coloredOutput = true;
    } config;
    

    TextStreamInterface &textStream;
};
