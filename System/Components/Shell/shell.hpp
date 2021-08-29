#pragma once

#include "input.hpp"

#include "thread.hpp"

#include "asciiStreamIF.hpp"
#include "uart2.hpp"

//#include <cstddef>
#include <array>
#include <cstdint>
#include <cstring>
#include <cstdarg>
#include <string_view>

using namespace std::string_view_literals;

class Shell : public cpp_freertos::Thread
{
public:
    struct Config
    {
        // prompt in blue, command entry in yellow
        static constexpr std::string_view prompt = "\e[34mFIB> \e[33m"sv;
        static constexpr std::size_t printfBufferSize = 256;
        static constexpr bool printEndLineCR = false;
        static constexpr bool printEndLineLF = true;
        static constexpr bool regularResponseIsEnabled = true;

        static_assert(printEndLineLF || printEndLineLF, "no end-line configured!");

        bool coloredOutput = true;
    } config;

    static void start(AsciiStream &asciiStream, std::uint16_t stackDepth, BaseType_t priority);

    void print(const char &c, std::size_t timesToRepeat = 1);
    void printUnformatted(const char *pData, const std::size_t len, std::size_t timesToRepeat = 1);
    void printEOL();

    int print(const char *str, std::size_t timesToRepeat = 1);
    int printf(const char *fmt, ...);
    class Command
    {
    public:
        enum Result : std::int8_t
        {
            fail8 = -8,
            fail7 = -7,
            fail6 = -6,
            fail5 = -5,
            fail4 = -4,
            fail3 = -3,
            badArg = -2,
            fail = -1,
            ok = 0,
            okQuiet,
            unknown,
        };

        using CtorCallbackF = void(void); // method for constructing subcommands
        using CommandF = Result(Shell &shell, std::size_t argc, const char *argv[]);

        Command(const char *name, const char *usage = "", const char *description = "", CommandF commandF = nullptr, CtorCallbackF ctorCallbackF = nullptr);
        Command(Command &parent, const char *name, const char *usage = "", const char *description = "", CommandF commandF = nullptr, CtorCallbackF ctorCallbackF = nullptr);
        const Command *findNeighbourCommand(const char *name) const;
        const Command *findSubcommand(const char *name) const;

        static bool matchToken(const char *strTokens, const char *strToken);

        const char *name = nullptr;
        const char *usage = nullptr;
        const char *description = nullptr;
        const CommandF *commandF = nullptr;

    protected:
        void linkTo(Command *&pParent);

        Command *pSubcommands = nullptr;
        Command *pNext = nullptr;

        friend Shell;
    };

    static const Shell::Command *findCommand(std::size_t argcIn, const char *argvIn[], std::size_t &argCmdOffsetOut);
    static Command::Result help(Shell &shell, const Shell::Command *pCommand, bool recurse = false, const std::size_t maxDepth = 1, std::size_t depth = 0, std::size_t indent = 0);
    static Command helpCommand;

    Command::Result execute(const Command &command, std::size_t argc, const char *argv[], const char *outputColorEscapeSequence = "\e[32m"); // default in green
    Command::Result execute(const Command &command, const char *strArgs = nullptr, const char *outputColorEscapeSequence = "\e[33m");

private:
    Shell(AsciiStream &asciiStream, std::uint16_t stackDepth, BaseType_t priority);

    virtual void Run() override;

    bool escape(const char &c);
    bool backspaceChar();
    bool insertChar(const char &c);
    bool lineFeed();

    bool receiveChar(const char &c);
    void promptNew();
    void printPrompt();

    bool visualCursorStep();
    bool visualCursorStepBack();

    bool cursorStep();
    bool cursorStepBack();

    bool deleteChar();

    bool processAnsiCursorControl(const char &c);
    bool processAnsiEscapeSequences(const char &c);

    Input input;
    bool isPrompted = true;
    static Command *pCommandRoot;

    AsciiStream &asciiStream;
};
