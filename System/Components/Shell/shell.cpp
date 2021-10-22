#include "shell.hpp"
#include "logger.hpp"
#include "system.hpp"
#include "resources.hpp"
// TODO: make arrow up repeat buffer
// TODO: some kind of esacpe signal mechanism to inform running command to exit.
#include <cstring>

Shell::Shell(const char *strPromptLabel, AsciiStream &asciiStream, std::uint16_t stackDepth, BaseType_t priority, Shell::Command *pCommandRoot)
    : Thread("shell", stackDepth, priority), strPromptLabel(strPromptLabel), asciiStream(asciiStream), pCommandRoot(pCommandRoot)
{
    if (Start() == false)
    {
        FIBSYS_PANIC();
    }
};

void Shell::Run()
{
    if (this->asciiStream.init())
    {
        this->print('\n');
        this->printf("Type '%s %s' to view available commands\n", this->helpCommand.name, this->helpCommand.usage);
        this->print('\n');
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

const Shell::Command *Shell::findCommand(std::size_t argcIn, const char *argvIn[], std::size_t &argCmdOffsetOut)
{
    const Command *pCommand = this->pCommandRoot;

    argCmdOffsetOut = 0;

    if (pCommand)
    {
        if (argcIn && argvIn[0])
        {
            pCommand = pCommand->findNeighbourCommand(argvIn[argCmdOffsetOut]);
            if (pCommand)
            {
                while (argcIn - argCmdOffsetOut - 1)
                {
                    const Command *pSubcommand = pCommand->findSubcommand(argvIn[argCmdOffsetOut + 1]);
                    if (pSubcommand)
                    {
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
                this->print("\n" ANSI_COLOR_GREEN "OK");
            }
            else if (static_cast<std::int8_t>(result) < 0)
            {
                this->printf("\n" ANSI_COLOR_RED "FAIL: %d", static_cast<std::int8_t>(result));
            }
            else if (result == Shell::Command::Result::okQuiet)
            {
                /* nothin */
            }
            this->print(ANSI_COLOR_RESET "\n");
        }
    }

    return result;
}

Shell::Command::Result Shell::execute(const Command &command, const char *argString, const char *outputColorEscapeSequence)
{
    ArgBuffer argBuffer(argString);
    return this->execute(command, argBuffer.getArgc(), argBuffer.getArgv(), outputColorEscapeSequence);
}

/** @return true if sequence finished */
bool Shell::receiveChar(const char &c)
{
    bool result = false;

    if (this->handleEscape(c))
    {
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

bool Shell::lineFeed()
{
    bool result = false;

    this->print("\n");

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

/** @result false - nothing to handle */
bool Shell::handleEscape(const char &c)
{
    bool result = false;

    if (c == '\e')
    {
        this->escapeTick = FibSys::getUptimeInMs();
        this->escapeState = EscapeState::escaped;
        result = true;
    }
    else if (enumEval(this->escapeState) >= enumEval(EscapeState::escaped))
    {
        if (FibSys::getUptimeInMs() - this->escapeTick > 2)
        {
            /* timed out */
            this->escapeState = EscapeState::none;
        }
        else if (c == 0x7F) // DELete
        {
            deleteChar();
            result = true;
            this->escapeState = EscapeState::finished;
        }
        else
        {
            result = this->handleAnsiEscape(c);
        }
    }
    else
    {
        this->escapeState = EscapeState::failed;
    }

    if (this->escapeState == EscapeState::failed || this->escapeState == EscapeState::finished)
    {
        this->escapeState = EscapeState::none;
    }

    return result;
}

/** @result false - nothing to handle */
bool Shell::handleAnsiEscape(const char &c)
{
    bool result = false;

    if (c == '[') /* open delimiter */
    {
        this->escapeState = EscapeState::delimited;
        result = true;
    }
    else if (enumEval(this->escapeState) >= enumEval(EscapeState::delimited))
    {
        result = this->handleAnsiDelimitedEscape(c);
    }
    else
    {
        this->escapeState = EscapeState::failed;
    }

    return result;
}

bool Shell::handleAnsiDelimitedEscape(const char &c)
{
    bool result = false;

    if (this->handleAnsiDelimitedDelEscape(c))
    {
        result = true;
    }
    else if (c == 'H')
    {
        this->onHomeKey();
        result = true;
        this->escapeState = EscapeState::finished;
    }
    else if (c == 'A')
    {
        this->onArrowUpKey();
        result = true;
        this->escapeState = EscapeState::finished;
    }
    else if (c == 'B')
    {
        this->onArrowDownKey();
        result = true;
        this->escapeState = EscapeState::finished;
    }
    else if (c == 'C')
    {
        this->onArrowRightKey();
        result = true;
        this->escapeState = EscapeState::finished;
    }
    else if (c == 'D')
    {
        this->onArrowLeftKey();
        result = true;
        this->escapeState = EscapeState::finished;
    }
    else
    {
        this->escapeState = EscapeState::failed;
    }

    return result;
}

bool Shell::handleAnsiDelimitedDelEscape(const char &c)
{
    bool result = false;

    if (enumEval(this->escapeState) >= enumEval(EscapeState::delimited) && c == '3')
    {
        this->escapeState = EscapeState::intermediate;
        result = true;
    }
    else if (enumEval(this->escapeState) >= enumEval(EscapeState::intermediate) && c == '~')
    {
        this->deleteChar();
        this->escapeState = EscapeState::finished;
        result = true;
    }
    else
    {
        this->escapeState = EscapeState::failed;
    }

    return result;
}

bool Shell::deleteChar()
{
    bool result = false;

    if (this->input.deleteCharAtCursor())
    {
        std::size_t stringAtCursorLength;
        const char *stringAtCursor = this->input.getBufferAtCursor(stringAtCursorLength);
        this->printUnformatted(stringAtCursor, stringAtCursorLength + 1);
        this->print("  ");
        this->print('\b', stringAtCursorLength + 1);
        result = true;
    }

    return result;
}

bool Shell::onHomeKey()
{
    while (this->onArrowLeftKey())
    {
    }
    return true;
}

bool Shell::onArrowUpKey()
{
    bool result = false;

    if (this->input.restoreIntoString())
    {
        int charsPrinted = this->printf(this->input.getBufferAtBase());
        if (charsPrinted > 0)
        {
            if (this->input.setCursor(charsPrinted))
            {
                result = true;
            }
        }
    }

    return result;
}

bool Shell::onArrowDownKey()
{
    this->input.reset();
    this->isPrompted = true;
    return true;
}

bool Shell::onArrowLeftKey()
{
    bool result = false;

    if (this->input.cursorStepLeft())
    {
        this->print('\b');
        result = true;
    }

    return result;
}

bool Shell::onArrowRightKey()
{
    bool result = false;

    if (this->input.cursorStepRight())
    {
        std::size_t length;
        this->print(*(this->input.getBufferAtCursor(length) - 1));
        result = true;
    }

    return result;
}

void Shell::promptNew(void)
{
    this->isPrompted = true;
    this->printPrompt();
}

void Shell::printPrompt(void)
{
    this->print(this->strPromptLabel);
}

/** @return true if actually backspaced */
bool Shell::backspaceChar()
{
    bool result = false;

    if (this->input.backspaceCharAtCursor())
    {
        std::size_t stringAtCursorLength;
        const char *stringAtCursor = this->input.getBufferAtCursor(stringAtCursorLength);
        this->print('\b');
        this->printUnformatted(stringAtCursor, stringAtCursorLength);
        this->print(' ');
        for (std::size_t i = stringAtCursorLength; i > 0; i--)
        {
            this->print('\b');
        }
        result = true;
    }

    return result;
}

/** @return true if actually inserted */
bool Shell::insertChar(const char &c)
{
    bool result = false;

    if (this->input.insertChar(c))
    {
        result = true;
        if (this->input.isCursorOnEnd())
        {
            /* append */
            this->print(c);
        }
        else
        {
            /* insert in middle */
            std::size_t length;
            this->print(c);
            this->print(this->input.getBufferAtCursor(length));
            this->print('\b', length - 1);
        }
    }

    return result;
}
