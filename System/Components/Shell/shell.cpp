#include "shell.hpp"
#include "logger.hpp"
#include "system.hpp"
#include "resources.hpp"
// TODO: make arrow up repeat buffer
#include <cstring>

Shell::Command *Shell::pCommandRoot = nullptr;

void Shell::start(AsciiStream &asciiStream, std::uint16_t stackDepth, BaseType_t priority)
{
    static Shell shell(asciiStream, stackDepth, priority);
}

Shell::Shell(AsciiStream &asciiStream, std::uint16_t stackDepth, BaseType_t priority)
    : Thread("shell", stackDepth, priority), asciiStream(asciiStream)
{
    if (Start() == false)
    {
        FibSys::panic();
    }
};

void Shell::Run()
{
    if (this->asciiStream.init())
    {
        this->printEOL();
        this->printf("Type '%s %s' to view available commands\n", this->helpCommand.name, this->helpCommand.usage);
        this->printEOL();
        this->promptNew();

        while (true)
        {
            // NOTE: escape sequences are time sensitive !
            // TODO: move this to a dedicated uart receiver task and join by char queue
            if (this->receiveChar(this->asciiStream.getChar()))
            { // escape sequence finished (not time sensitive)
                // Logger::trace("shell", "c:%u t:%u %s", this->rxCursorIdx, this->rxCharsTotal, this->input.data());
            }
        }
    }
    this->asciiStream.deinit();
}

void Shell::print(const char &c, std::size_t timesToRepeat)
{
    while (timesToRepeat--)
    {
        if (Config::printEndLineCR && c == '\n')
        {
            this->asciiStream.putChar('\r');
        }

        this->asciiStream.putChar(c);
    }
}

void Shell::printUnformatted(const char *pData, const std::size_t len, std::size_t timesToRepeat)
{
    while (timesToRepeat--)
    {
        std::size_t lenIt = len;
        const char *pDataIt = pData;

        while (lenIt--)
        {
            this->print(*(pDataIt++));
        }
    }
}

void Shell::printEOL()
{
    if (Config::printEndLineCR)
    {
        this->print('\r');
    }

    if (Config::printEndLineLF)
    {
        this->print('\n');
    }
}

int Shell::print(const char *string, std::size_t timesToRepeat)
{
    int charsPrinted = 0;

    while (timesToRepeat--)
    {
        for (const char *c = string; *c != '\0'; ++c)
        {
            this->print(*c); // TODO print whole sentence not char by char !
            charsPrinted++;
        }
    }

    return charsPrinted;
}

int Shell::printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    std::array<char, Config::printfBufferSize> txBuffer;
    vsnprintf(txBuffer.data(), txBuffer.size(), fmt, args);
    int charsPrinted = this->print(txBuffer.data());

    va_end(args);

    return charsPrinted;
}

void Shell::Command::linkTo(Command *&pParent)
{
    if (!pParent)
    {
        pParent = this;
    }
    else
    {
        Command *pNext = pParent;
        while (pNext->pNext)
        {
            pNext = pNext->pNext;
        }
        pNext->pNext = this;
    }
}

Shell::Command::Command(const char *name, const char *usage, const char *description, CommandF commandF, CtorCallbackF ctorCallbackF)
    : name(name), usage(usage), description(description), commandF(commandF)
{
    this->linkTo(Shell::pCommandRoot);
    if (ctorCallbackF)
    {
        ctorCallbackF();
    }
}

Shell::Command::Command(Command &parent, const char *name, const char *usage, const char *description, CommandF commandF, CtorCallbackF ctorCallbackF)
    : name(name), usage(usage), description(description), commandF(commandF)
{
    this->linkTo(parent.pSubcommands);
    if (ctorCallbackF)
    {
        ctorCallbackF();
    }
}

bool Shell::Command::matchToken(const char *strTokens, const char *strToken)
{
    bool result = false;

    const std::size_t strTokenLength = std::strlen(strToken);
    const char *strThisToken = strTokens;

    for (const char *strCharIt = strThisToken; *strCharIt != '\0'; strCharIt++)
    {
        const bool itAtLastChar = (*(strCharIt + 1) == '\0');
        if (*strCharIt == ',' || itAtLastChar)
        {
            const std::size_t thisTokenLength = strCharIt + (itAtLastChar ? 1 : 0) - strThisToken;
            if (strTokenLength == thisTokenLength && 0 == std::strncmp(strToken, strThisToken, thisTokenLength))
            {
                result = true;
                break;
            }
            else if (*strCharIt == ',')
            {
                strThisToken = strCharIt + 1;
            }
        }
    }

    return result;
}

const Shell::Command *Shell::Command::findNeighbourCommand(const char *name) const
{
    const Command *result = nullptr;

    for (const Command *pNext = this; pNext != nullptr; pNext = pNext->pNext)
    {
        if (Command::matchToken(pNext->name, name))
        {
            result = pNext;
            break;
        }
    }

    return result;
}

const Shell::Command *Shell::Command::findSubcommand(const char *name) const
{
    const Command *result = nullptr;

    for (const Command *pNext = this->pSubcommands; pNext != nullptr; pNext = pNext->pNext)
    {
        if (Command::matchToken(pNext->name, name))
        {
            result = pNext;
            break;
        }
    }

    return result;
}

const Shell::Command *Shell::findCommand(std::size_t argcIn, const char *argvIn[], std::size_t &argCmdOffsetOut)
{
    const Command *pCommand = Shell::pCommandRoot;
    std::size_t argIt = 0;

    argCmdOffsetOut = 0;

    if (pCommand)
    {
        if (argcIn && argvIn[argIt])
        {
            pCommand = pCommand->findNeighbourCommand(argvIn[argCmdOffsetOut]);

            if (pCommand)
            {
                while (argcIn - argCmdOffsetOut - 1)
                {
                    const Command *pSubcommand = pCommand->findSubcommand(argvIn[argCmdOffsetOut + 1 + argIt]);

                    if (pSubcommand)
                    {
                        argIt++;
                        argCmdOffsetOut++;
                        pCommand = pSubcommand;
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }

    return pCommand;
}

Shell::Command::Result Shell::execute(const Shell::Command &command, std::size_t argc, const char *argv[], const char *outputColorEscapeSequence)
{
    Shell::Command::Result result = Shell::Command::Result::unknown;

    if (command.commandF == nullptr)
    {
        this->print("\e[31mcommand has no method\n"); // red
    }
    else
    {
        this->print(outputColorEscapeSequence); // response in green
        result = command.commandF(*this, argc, argv);

        if (Config::regularResponseIsEnabled)
        {
            if (result == Shell::Command::Result::ok)
            {
                this->print("\n\e[32mOK\n"); // green
            }
            else if (static_cast<std::int8_t>(result) < 0)
            {
                this->printf("\n\e[31mFAIL: %d\n", static_cast<std::int8_t>(result)); // red
            }
            else if (result == Shell::Command::Result::okQuiet)
            {
                /* nothin */
            }
        }
    }

    return result;
}

Shell::Command::Result Shell::execute(const Command &command, const char *argString, const char *outputColorEscapeSequence)
{
    ArgBuffer argBuffer(argString);
    return this->execute(command, argBuffer.getArgc(), argBuffer.getArgv(), outputColorEscapeSequence);
}

Shell::Command::Result Shell::help(Shell &shell, const Shell::Command *pCommand, bool recurse, const std::size_t maxDepth, std::size_t depth, std::size_t indent)
{
    Shell::Command::Result result = Shell::Command::Result::ok;

    if (!pCommand)
    {
        result = Command::Result::badArg;
    }
    else
    {
        constexpr int commandColumnWidth = 40;

        for (const Shell::Command *pCmdIt = pCommand; pCmdIt != nullptr; pCmdIt = pCmdIt->pNext)
        {
            shell.print(' ', indent);

            int charsPrinted = 0;
            if (pCmdIt->usage)
            {
                charsPrinted = shell.printf("%s %s ", pCmdIt->name, pCmdIt->usage);
            }
            else
            {
                charsPrinted = shell.printf("%s  ", pCmdIt->name);
            }

            if (charsPrinted > 0)
            {
                if (charsPrinted < commandColumnWidth)
                {
                    for (std::size_t i = commandColumnWidth - charsPrinted; i; i--)
                    {
                        shell.print(' ');
                    }
                }
                charsPrinted = shell.printf("%s\n", pCmdIt->description);
                if (charsPrinted > 0)
                {
                    result = Command::Result::ok;
                }
            }

            if (result == Command::Result::ok && recurse && depth < maxDepth && pCmdIt->pSubcommands)
            {
                result = help(shell, pCmdIt->pSubcommands, recurse, maxDepth, depth + 1, indent + strlen(pCmdIt->name) + sizeof(' '));
            }

            if (depth == 0)
            {
                break;
            }
        }
    }
    return result;
}

Shell::Command Shell::helpCommand = Shell::Command(
    "help", "[all|[COMMAND...]]", "show command usage",
    [](Shell &shell, std::size_t argc, const char *argv[]) -> Shell::Command::Result
    {
        Shell::Command::Result result = Shell::Command::Result::unknown;

        if (argc)
        {
            if (argc == 1)
            {
                for (const Shell::Command *pCmdIt = Shell::pCommandRoot; pCmdIt != nullptr; pCmdIt = pCmdIt->pNext)
                {
                    result = Shell::help(shell, pCmdIt, 0, false);
                }
            }
            else if (argc == 2 && !std::strcmp(argv[1], "all"))
            {
                for (const Shell::Command *pCmdIt = Shell::pCommandRoot; pCmdIt != nullptr; pCmdIt = pCmdIt->pNext)
                {
                    result = Shell::help(shell, pCmdIt, 7, true);
                }
            }
            else if (argc > 1)
            {
                constexpr std::size_t helpCommandOffset = 1;
                std::size_t argOffset;
                const Command *pCommandFound = Shell::findCommand(argc - helpCommandOffset, argv + helpCommandOffset, argOffset);
                if (pCommandFound)
                {
                    result = Shell::help(shell, pCommandFound, 1, true);
                }
            }
        }
        return result;
    });

void Shell::promptNew(void)
{
    this->isPrompted = true;
    this->printPrompt();
}

void Shell::printPrompt(void)
{
    this->print(Config::prompt.data());
}

bool Shell::visualCursorStep()
{
    if (false == this->input.isFull())
    {
        // ovewrite char with same value effectively moving cursor right
        this->print(this->input.getCharAtCursor());
        return true;
    }
    return false;
}

bool Shell::visualCursorStepBack()
{
    if (false == this->input.isCursorOnBase())
    {
        this->print('\b'); // backspaceChar moves cursor back
        return true;
    }
    return false;
}

bool Shell::cursorStep()
{
    return (this->visualCursorStep() && this->input.cursorStep());
}

bool Shell::cursorStepBack()
{
    return (this->visualCursorStepBack() && this->input.cursorStepBack());
}

bool Shell::deleteChar()
{
    if (this->input.deleteCharAtCursor())
    {
        std::size_t stringAtCursorLength;
        const char *stringAtCursor = this->input.getBufferAtCursor(stringAtCursorLength);
        this->printUnformatted(stringAtCursor, stringAtCursorLength + 1);
        this->print("  ");
        this->print('\b', stringAtCursorLength + 1);

        return true;
    }
    return false;
}

// ESC[H	moves cursor to home position (0, 0)
// ESC[{line};{column}H
// ESC[{line};{column}f	moves cursor to line #, column #
// ESC[#A	moves cursor up # lines
// ESC[#B	moves cursor down # lines
// ESC[#C	moves cursor right # columns
// ESC[#D	moves cursor left # columns
// ESC[#E	moves cursor to beginning of next line, # lines down
// ESC[#F	moves cursor to beginning of previous line, # lines down
// ESC[#G	moves cursor to column #
// ESC[#;#R	reports current cursor line and column
// ESC[s	saves the current cursor position
// ESC[u	restores the cursor to the last saved position

// returns true if sequence finished
bool Shell::processAnsiCursorControl(const char &c)
{
    static std::size_t delSequence = 0;

    if (c == '3') // DEL first symbol
    {
        delSequence++;
        return false;
    }
    else if (c == '~' && delSequence == 1)
    {
        this->deleteChar();
        delSequence = 0;
        return true;
    }
    else if (c == 'H') // home position
    {
        while (this->cursorStepBack())
        {
        }
        return true;
    }
    else if (c == 'A') // cursor up
    {
        this->input.restoreIntoString();
        this->printf(this->input.getBufferAtBase());
        return true;
    }
    else if (c == 'B') // cursor down
    {
        this->input.reset();
        this->isPrompted = true;
        return true;
    }
    else if (c == 'C') // cursor right
    {
        return this->cursorStep();
    }
    else if (c == 'D') // cursor left
    {
        return this->cursorStepBack();
    }

    return true;
}

// returns true if sequence finished
bool Shell::processAnsiEscapeSequences(const char &c)
{
    static bool delimited = false;

    if (c == '[') // open delimiter
    {
        delimited = true;
    }
    else if (delimited) // proccess command
    {
        if (this->processAnsiCursorControl(c))
        {
            delimited = false; // close delimiter
            return true;
        }
    }

    return false;
}

// returns false if escape sequence in progress (time sensitive)
bool Shell::escape(const char &c)
{
    bool result = true;

    if (c == 0x7F) // DEL(ete)
    {
        this->deleteChar();
        result = false;
    }
    else if (this->processAnsiEscapeSequences(c))
    {
        result = false;
    }

    return result;
}

// returns true if actually backspaced
bool Shell::backspaceChar()
{
    bool result = false;

    if (this->input.backspaceCharAtCursor())
    {
        std::size_t stringAtCursorLength;
        const char *stringAtCursor = this->input.getBufferAtCursor(stringAtCursorLength);
        this->printUnformatted(stringAtCursor, stringAtCursorLength);
        this->print("\b \b");
        result = true;
    }
    return result;
}

// returns true if actually inserted
bool Shell::insertChar(const char &c)
{
    bool result = false;

    if (this->input.insertChar(c))
    {
        result = true;
        if (this->input.isCursorOnEnd())
        {
            // append char
            this->print(c);
        }
        else
        {
            // insertChar within line
            std::size_t length;
            this->print(c);                                     // overwrite new char
            this->print(this->input.getBufferAtCursor(length)); // overwrite everything on right
            this->print('\b', length - sizeof(c));              // backspaceChar to where the cursor should be
        }
    }

    return result;
}

bool Shell::lineFeed()
{
    bool result = false;

    this->printEOL();

    const Shell::Command *pCommand = nullptr;

    if (this->input.resolveIntoArgs())
    {
        std::size_t argOffset;
        pCommand = this->findCommand(this->input.getArgc(), this->input.getArgv(), argOffset);
        if (!pCommand)
        {
            this->print("\e[39mcommand not found\n");
            result = false;
        }
        else
        {
            this->execute(*pCommand, this->input.getArgc() - argOffset, this->input.getArgv() + argOffset);
            result = true;
        }
    }

    this->promptNew();

    return result;
}

// returns false if escape sequence in progress (time sensitive)
bool Shell::receiveChar(const char &c)
{
    bool result = false;

    static bool escaped = false;
    if (c == '\e') // escaped
    {
        escaped = true;
    }
    else if (escaped)
    {
        escaped = this->escape(c);
    }
    else if (c == '\b')
    {
        result = true;
        this->backspaceChar();
    }
    else if (' ' <= c && c <= '~')
    {
        result = true;

        if (this->isPrompted)
        {
            this->input.reset();
            this->isPrompted = false;
        }
        this->insertChar(c);
    }
    else if (c == '\r')
    {
        result = true;

        if (this->isPrompted)
        {
            this->input.reset();
        }
        this->lineFeed();
    }
    return result;
}
