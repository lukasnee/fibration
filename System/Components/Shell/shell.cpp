#include "shell.hpp"
#include "logger.hpp"
#include "system.hpp"
#include "resources.hpp"
#include "version.hpp"

//TODO: command name sinonyms. for example: "help" and "h" could call the same command 
//TODO: arrow up to echo and execute last request

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
        this->putEOL();
        this->printf("Fibration v%u.%u.%u\n", Fib::Version::major, Fib::Version::minor, Fib::Version::patch);
        this->putString("Type 'help [COMMAND...]' to list commands\n");
        this->putEOL();
        this->promptNew();

        while (true)
        {
            // NOTE: escape sequences are time sensitive !
            // TODO: move this to a dedicated uart receiver task and join by char queue
            if (this->receiveChar(this->asciiStream.getc()))
            { // escape sequence finished (not time sensitive)
                // Logger::trace("shell", "c:%u t:%u %s", this->rxCursorIdx, this->rxCharsTotal, this->input.data());
            }
        }
    }
    this->asciiStream.deinit();
}

void Shell::putChar(const char &c, std::size_t timesToRepeat)
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

void Shell::putData(const char *pData, const std::size_t len, std::size_t timesToRepeat)
{
    while (timesToRepeat--)
    {
        std::size_t lenIt = len;
        const char *pDataIt = pData;

        while (lenIt--)
        {
            this->putChar(*(pDataIt++));
        }
    }
}

void Shell::putEOL()
{
    if (Config::printEndLineCR)
    {
        this->putChar('\r');
    }

    if (Config::printEndLineLF)
    {
        this->putChar('\n');
    }
}

int Shell::putString(const char *string, std::size_t timesToRepeat)
{
    int charsPrinted = 0;

    while (timesToRepeat--)
    {
        for (const char *c = string; *c != '\0'; ++c)
        {
            this->putChar(*c);
            charsPrinted++;
        }
    }

    return charsPrinted;
}

int Shell::printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    std::array<char, Config::txBufferSize> txBuffer;

    vsnprintf(txBuffer.data(), txBuffer.size(), fmt, args);
    int charsPrinted = this->putString(txBuffer.data());

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

const Shell::Command *Shell::Command::findNeighbourCommand(const char *name) const
{
    const Command *result = nullptr;

    for (const Command *pNext = this; pNext != nullptr; pNext = pNext->pNext)
    {
        if (std::strcmp(pNext->name, name) == 0)
        {
            result = pNext;
        }
    }

    return result;
}

const Shell::Command *Shell::Command::findSubcommand(const char *name) const
{
    const Command *result = nullptr;

    for (const Command *pNext = this->pSubcommands; pNext != nullptr; pNext = pNext->pNext)
    {
        if (std::strcmp(pNext->name, name) == 0)
        {
            result = pNext;
        }
    }

    return result;
}

const Shell::Command *Shell::findCommand(std::size_t argcIn, const char *argvIn[], std::size_t &argcLeft, const char **&argvLeft)
{
    const Command *pCommand = Shell::pCommandRoot;
    std::size_t argIt = 0;

    if (pCommand)
    {
        if (argcIn && argvIn[argIt])
        {
            argcLeft = argcIn;
            argvLeft = argvIn;

            pCommand = pCommand->findNeighbourCommand(argvLeft[argIt]);

            if (pCommand)
            {
                while (argcLeft - 1)
                {
                    const Command *pSubcommand = pCommand->findSubcommand(argvLeft[argIt + 1]);

                    if (pSubcommand)
                    {
                        argIt++;
                        argcLeft--;
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

        argvLeft = &argvLeft[argIt];
    }

    if (!pCommand)
    {
        argcLeft = 0;
        argvLeft = nullptr;
    }

    return pCommand;
}

Shell::Command::Result Shell::execute(const Shell::Command &command, std::size_t argc, const char *argv[], const char *outputColorEscapeSequence)
{
    Shell::Command::Result result = Shell::Command::Result::unknown;

    if (command.commandF == nullptr)
    {
        this->putString("\e[31mcommand has no method\n"); // red
    }
    else
    {
        this->putString(outputColorEscapeSequence); // response in green
        result = command.commandF(*this, argc, argv);

        if (Config::regularResponseIsEnabled)
        {
            if (result == Shell::Command::Result::ok)
            {
                this->putString("\n\e[32mOK\n"); // green
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

Shell::Command::Result Shell::help(Shell &shell, const Shell::Command *pCommand, bool recurse, const std::size_t maxDepth, std::size_t depth, std::size_t indent)
{
    Shell::Command::Result result = Shell::Command::Result::ok;

    if (!pCommand)
    {
        result = Command::Result::badArg;
    }
    else
    {
        for (const Shell::Command *pCmdIt = pCommand; pCmdIt != nullptr; pCmdIt = pCmdIt->pNext)
        {
            shell.putChar(' ', indent);

            if (pCmdIt->usage)
            {
                if (shell.printf("%s %s - %s\n", pCmdIt->name, pCmdIt->usage, pCmdIt->description) > 0)
                {
                    result = Command::Result::ok;
                }
            }
            else
            {
                if (shell.printf("%s - %s\n", pCmdIt->name, pCmdIt->description) > 0)
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
    "help", "[all]", "show command usage",
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
                std::size_t argcLeft;
                const char **argvLeft;
                const Command *pCommandFound = Shell::findCommand(argc - 1, argv + 1, argcLeft, argvLeft);
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
    this->input.reset();
    this->printPrompt();
}

void Shell::printPrompt(void)
{
    this->putString(Config::prompt.data());
}

bool Shell::visualCursorStep()
{
    if (false == this->input.isFull())
    {
        // ovewrite char with same value effectively moving cursor right
        this->putChar(this->input.getCharAtCursor());
        return true;
    }
    return false;
}

bool Shell::visualCursorStepBack()
{
    if (false == this->input.isCursorOnBase())
    {
        this->putChar('\b'); // backspaceChar moves cursor back
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
        const char *stringAtCursor = this->input.getStringAtCursor(stringAtCursorLength);
        this->putData(stringAtCursor, stringAtCursorLength + 1);
        this->putString("  ");
        this->putChar('\b', stringAtCursorLength + 1);

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
        /* commands scrolling unsupported */
        return true;
    }
    else if (c == 'B') // cursor down
    {
        /* commands scrolling unsupported */
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
        const char *stringAtCursor = this->input.getStringAtCursor(stringAtCursorLength);
        this->putData(stringAtCursor, stringAtCursorLength);
        this->putString("\b \b");
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
            this->putChar(c);
        }
        else
        {
            // insertChar within line
            std::size_t length;
            this->putChar(c);                                       // overwrite new char
            this->putString(this->input.getStringAtCursor(length)); // overwrite everything on right
            this->putChar('\b', length - sizeof(c));                // backspaceChar to where the cursor should be
        }
    }

    return result;
}

bool Shell::lineFeed()
{
    bool result = false;

    this->putEOL();

    const Shell::Command *pCommand = nullptr;

    if (this->input.resolveIntoArgs())
    {
        std::size_t argcLeft;
        const char **argvLeft;
        pCommand = this->findCommand(this->input.getArgc(), this->input.getArgv(), argcLeft, argvLeft);
        if (!pCommand)
        {
            this->putString("\e[39mcommand not found\n");
            result = false;
        }
        else
        {
            this->execute(*pCommand, argcLeft, argvLeft);
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
    else if (c == '\b') // backspaceChar
    {
        result = true;
        this->backspaceChar();
    }
    else if (c >= 0x20 && c <= 0x7E) // ascii text symbols
    {
        result = true;
        this->insertChar(c);
    }
    else if (c == '\r') // line feed
    {
        result = true;
        this->lineFeed();
    }
    return result;
}
