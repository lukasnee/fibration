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
#include <limits>
#include <type_traits>
#include <functional>

using namespace std::string_view_literals;

#define ANSI_COLOR_BLACK "\e[30m"
#define ANSI_COLOR_RED "\e[31m"
#define ANSI_COLOR_GREEN "\e[32m"
#define ANSI_COLOR_YELLOW "\e[33m"
#define ANSI_COLOR_BLUE "\e[34m"
#define ANSI_COLOR_MAGENTA "\e[35m"
#define ANSI_COLOR_CYAN "\e[36m"
#define ANSI_COLOR_WHITE "\e[37m"
#define ANSI_COLOR_DEFAULT "\e[39m"
#define ANSI_COLOR_RESET "\e[0m"

template <typename T>
std::underlying_type_t<T> enumEval(T enumVal)
{
    return static_cast<std::underlying_type_t<T>>(enumVal);
}

class Shell : public cpp_freertos::Thread
{
public:
    struct Config
    {
        static constexpr std::size_t printfBufferSize = 256;
        static constexpr bool regularResponseIsEnabled = true;
        bool coloredOutput = true;
    } config;

    class Command
    {
    public:
        enum Result : std::int8_t
        {
            unknown = std::numeric_limits<std::int8_t>::min(),
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
        };

#define SHELLCMDPARAMS Shell &shell, std::size_t argc, const char *argv[]
#define SHELLCMDARGS shell, argc, argv

        Command(const char *name, const char *usage, const char *description,
                std::function<Result(SHELLCMDPARAMS)> commandF, std::function<void()> ctorCallbackF = nullptr);

        Command(Command &parent, const char *name, const char *usage, const char *description,
                std::function<Result(SHELLCMDPARAMS)> commandF, std::function<void()> ctorCallbackF = nullptr);

        Command(const char *name, std::function<Result(SHELLCMDPARAMS)> commandF);

        const Command *findNeighbourCommand(const char *name) const;
        const Command *findSubcommand(const char *name) const;

        static bool matchToken(const char *strTokens, const char *strToken);

        const char *name = nullptr;
        const char *usage = nullptr;
        const char *description = nullptr;
        const std::function<Result(SHELLCMDPARAMS)> commandF = nullptr;

        // TODO maybe move to other file (namespace CommandHelpers, etc.)
        struct Helper
        {
            struct Literal
            {
                static constexpr const char *onOffUsage = "<on|off>";
                static constexpr const char *badArg = "bad arg\n";
                static constexpr const char *noArg = "no arg\n";
                static constexpr const char *verb = "turned";
                static constexpr const char *on = "on";
                static constexpr const char *off = "off";
            };

            static Result onOffCommand(std::function<bool(bool)> onOffF, const char *strOnOffControlName, SHELLCMDPARAMS);
            static Result onOffCommand(bool &onOffControl, const char *strOnOffControlName, SHELLCMDPARAMS);
        };

    protected:
        void linkTo(Command *&pParent);

        Command *pSubcommands = nullptr;
        Command *pNext = nullptr;

        friend Shell;
    };

    Shell(const char *strPromptLabel, AsciiStream &asciiStream, std::uint16_t stackDepth, BaseType_t priority, Command *pCommandRoot = Shell::pCommandGlobalRoot);

    void print(const char &c, std::size_t timesToRepeat = 1);
    int print(const char *str, std::size_t timesToRepeat = 1);
    void printUnformatted(const char *pData, const std::size_t len, std::size_t timesToRepeat = 1);
    int printf(const char *fmt, ...);

    const Shell::Command *findCommand(std::size_t argcIn, const char *argvIn[], std::size_t &argCmdOffsetOut);

    Command::Result execute(const Command &command, std::size_t argc, const char *argv[], const char *outputColorEscapeSequence = "\e[32m"); // default in green
    Command::Result execute(const Command &command, const char *strArgs = nullptr, const char *outputColorEscapeSequence = "\e[33m");

    static Command::Result help(Shell &shell, const Shell::Command *pCommand, bool recurse = false, const std::size_t maxDepth = 1, std::size_t depth = 0, std::size_t indent = 0);
    static Command helpCommand;

private:
    virtual void Run() override;

    bool receiveChar(const char &c);
    bool lineFeed();

    bool handleEscape(const char &c);
    bool handleAnsiEscape(const char &c);
    bool handleAnsiDelimitedEscape(const char &c);
    bool handleAnsiDelimitedDelEscape(const char &c);
    bool deleteChar();
    bool onHomeKey();
    bool onArrowUpKey();
    bool onArrowDownKey();
    bool onArrowRightKey();
    bool onArrowLeftKey();

    bool backspaceChar();
    bool insertChar(const char &c);

    void promptNew();
    void printPrompt();

    enum class EscapeState : std::int8_t
    {
        failed = -1,
        none = 0,
        escaped,
        delimited,
        intermediate,
        finished,
    } escapeState;

    std::uint32_t escapeTick;

    Input input;
    bool isPrompted = true;

    const char *strPromptLabel;
    AsciiStream &asciiStream;
    Command *pCommandRoot = nullptr;

    static Command *pCommandGlobalRoot;
};
