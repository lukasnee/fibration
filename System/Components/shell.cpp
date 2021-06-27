#include "shell.hpp"
#include "logger.hpp"
#include "system.hpp"
#include "resources.hpp"

//TODO: arrow up to echo and execute last request

void Shell::start(TextStream &textStream, std::uint16_t stackDepth, BaseType_t priority)
{
    static Shell shell(textStream, stackDepth, priority);
}

Shell::Shell(TextStream &textStream, std::uint16_t stackDepth, BaseType_t priority)
    : Thread("shell", stackDepth, priority), textStream(textStream)
{
    if (Start() == false)
    {
        FibSys::panic();
    }
};

int Shell::printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    std::array<char, Shell::txBufferSize> txBuffer;

    int len = vsnprintf(txBuffer.data(), txBuffer.size(), fmt, args);
    this->echo(txBuffer.data());

    va_end(args);

    return len;
}

void Shell::printc(const char c)
{
    this->echo(c);
}

void Shell::Run()
{
    if(this->textStream.init())
    {
        this->echoEndLine();
        this->echoLine("Type 'help' to list all commands");
        this->promptNew();

        // Consume and send characters to shell...forever
        while (true)
        {
            // NOTE: escape sequences are time sensitive !
            // TODO: move this to a dedicated uart receiver task and join by char queue
            if(this->receiveChar(this->textStream.getc()))
            { // escape sequence finished (not time sensitive)
                // Logger::trace("shell", "c:%u t:%u %s", this->rxCursorIdx, this->rxCharsTotal, this->rxBuffer.data());
            }
        }
    }
    this->textStream.deinit();
}

void Shell::echo(char c, std::size_t timesToRepeat)
{
    while (timesToRepeat--)
    {
        if (c == '\n')
        {
            this->textStream.putc('\r');
        }
        this->textStream.putc(c);
    }
}

void Shell::echo(const char *string, std::size_t timesToRepeat)
{
    while (timesToRepeat--)
    {
        for (const char *c = string; *c != '\0'; ++c)
        {
            this->echo(*c);
        }
    }
}

void Shell::echoData(const char *pData, const std::size_t len, std::size_t timesToRepeat)
{
    while (timesToRepeat--)
    {
        std::size_t lenIt = len;
        const char * pDataIt = pData;
        
        while (lenIt--)
        {
            this->echo(*(pDataIt++));
        }
    }
}

void Shell::echoLine(const char *string)
{
    for (const char *c = string; *c != '\0'; ++c)
    {
        this->echo(*c);
    }
    this->echoEndLine();
}

void Shell::echoEndLine()
{
    this->echo("\r\n");
}
void Shell::resetRxBuffer(void)
{
    this->rxBuffer.fill('\0');
    this->rxCharsTotal = 1;
    this->rxCursorIdx = 0;
}

void Shell::promptNew(void)
{
    this->resetRxBuffer();
    this->printPrompt();
}

void Shell::printPrompt(void)
{
    this->echo(Shell::prompt.data());
}

bool Shell::rxBufferCursorOnBase()
{
    return (this->rxCursorIdx == 0);
}

bool Shell::rxBufferCursorNotOnBase()
{
    return (false == this->rxBufferCursorOnBase());
}

bool Shell::rxBufferCursorOnTail()
{
    return (this->rxCharsTotal - this->rxCursorIdx == 1);
}

bool Shell::rxBufferCursorNotOnTail()
{
    return (false == this->rxBufferCursorOnTail());
}

bool Shell::rxBufferIsEmpty()
{
    return (this->rxBufferCursorOnBase() and this->rxBufferCursorOnTail());
}

bool Shell::rxBufferIsNotEmpty()
{
    return (false == this->rxBufferIsEmpty());
}

bool Shell::rxBufferIsFull()
{
    return (this->rxCharsTotal >= this->rxBuffer.size());
}

bool Shell::rxBufferIsNotFull()
{
    return (false == this->rxBufferIsFull());
}

char Shell::rxBufferCharOnCursor()
{
    return this->rxBuffer.at(this->rxCursorIdx);
}

char Shell::rxBufferLastChar()
{
    if(this->rxBufferIsNotEmpty())
    {
        return this->rxBuffer.at(this->rxCharsTotal - 1 - sizeof('\0'));
    }
    else
    {
        return '\0';
    }
}

bool Shell::rxBufferCursorStep()
{
    if(this->rxBufferIsNotFull())
    {
        this->rxCursorIdx++;
        return true;
    }
    return false;
}

bool Shell::rxBufferCursorStepBack()
{
    if(this->rxBufferCursorNotOnBase())
    {
        this->rxCursorIdx--;
        return true;
    }
    return false;
}

bool Shell::visualCursorStep()
{            
    // ovewrite char with same value effectively moving cursor right
    if(this->rxBufferIsNotFull())
    {
        this->echo(this->rxBufferCharOnCursor());
        return true;
    }
    return false;
}

bool Shell::visualCursorStepBack()
{
    if(this->rxBufferCursorNotOnBase())
    {
        this->echo('\b');
        return true;
    }
    return false;
}

bool Shell::cursorStep()
{
    return (this->visualCursorStep() && this->rxBufferCursorStep());
}

bool Shell::cursorStepBack()
{
    return (this->visualCursorStepBack() && this->rxBufferCursorStepBack());
}

bool Shell::deleteChar()
{
    if (this->rxBufferIsNotEmpty() && this->rxBufferCursorNotOnTail())
    {
        std::memmove(&this->rxBuffer[this->rxCursorIdx], &this->rxBuffer[this->rxCursorIdx + 1], this->rxCharsTotal - (this->rxCursorIdx + 1));
        this->rxBuffer[--this->rxCharsTotal] = '\0';

        this->echoData(&this->rxBuffer[this->rxCursorIdx], this->rxCharsTotal - (this->rxCursorIdx + 1));
        this->echo("  ");
        this->echo('\b', this->rxCharsTotal - this->rxCursorIdx + 1);

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
bool Shell::processAnsiCursorControl(char c)
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
        while (this->rxBufferCursorStepBack())
        {
            this->visualCursorStepBack();
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
        if (this->rxBufferCursorNotOnTail())
        {
            this->cursorStep();
        }
        return true;
    }
    else if (c == 'D') // cursor left
    {
        if (this->rxBufferCursorNotOnBase())
        {
            this->cursorStepBack();
        }
        return true;
    }

    return true;
}

// returns true if sequence finished
bool Shell::processAnsiEscapeSequences(char c)
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
bool Shell::receiveChar(char c)
{
    static bool escaped = false;

    if (c == '\e') // escaped
    {
        escaped = true;
        return false;
    }
    else if (escaped)
    {
        if (c == 0x7F) // DEL(ete)
        {
            this->deleteChar();
            escaped = false;
        }
        else if (this->processAnsiEscapeSequences(c))
        {
            escaped = false;
        }
        
        return false;
    }
    else if (c == '\b') // backspace
    {
        if (this->rxBufferCursorNotOnBase())
        {
            std::memmove(&this->rxBuffer[this->rxCursorIdx - 1], &this->rxBuffer.at(this->rxCursorIdx), this->rxCharsTotal - this->rxCursorIdx);
            if(this->cursorStepBack())
            {
                this->rxBuffer[--this->rxCharsTotal] = '\0';
                this->echoData(&this->rxBuffer[this->rxCursorIdx], this->rxCharsTotal - this->rxCursorIdx);
                this->echo("  ");
                this->echo('\b', this->rxCharsTotal - this->rxCursorIdx + 1);
            }
        }
        
        return true;
    }
    else if (c >= 0x20 && c <= 0x7E) // ascii text symbols
    {
        if (this->rxBufferIsNotFull())
        {
            if (this->rxBufferCursorNotOnTail()) 
            {
                std::memmove(&this->rxBuffer[this->rxCursorIdx + 1], 
                &this->rxBuffer.at(this->rxCursorIdx), 
                this->rxCharsTotal - this->rxCursorIdx);
            }

            this->rxBuffer[this->rxCursorIdx] = c;
            this->rxBufferCursorStep();
            this->rxBuffer[this->rxCharsTotal++] = '\0';

            if (this->rxBufferCursorOnTail()) // then append char
            {
                this->echo(c);
            }
            else // insert within line
            {
                this->echo(&this->rxBuffer[this->rxCursorIdx - 1]);
                this->echo('\b', this->rxCharsTotal - this->rxCursorIdx - 1);
            }
        }
        return true;
    }
    else if (c == '\r') // line feed (enter!)
    {
        this->echoEndLine();
        this->processRxBufferIntoArgs();
        return true;
    }
    return true;
}

void Shell::processRxBufferIntoArgs()
{

    std::array<char *, Shell::maxArgs> argv{{0}};
    std::size_t argc = 0;

    char *nextArg = nullptr;
    for (std::size_t i = 0; i < this->rxCharsTotal && argc < argv.size(); ++i)
    {
        char *const c = &this->rxBuffer[i];
        if (*c == ' ' || *c == '\0')
        {
            *c = '\0';
            if (nextArg)
            {
                argv[argc++] = nextArg;
                nextArg = nullptr;
            }
        }
        else if (!nextArg)
        {
            nextArg = c;
        }
    }

    if (argc >= 1)
    {
        const Shell::Command *command = findCommand(argv[0]);
        if (!command)
        {
            this->printf("\e[39mfibration: %s: command not found ", argv[0]);
            this->echoLine(argv[0]);
        }
        else
        {
            echo("\e[32m"); // response in green
            int result = command->handler(*this, argc, argv.data());
            if (ShellConfig::regularResponseIsEnabled)
            {
                if (result == 0)
                {
                    this->echoLine("\e[32mOK"); // green
                }
                else if (result < 0)
                {
                    this->printf("\e[31mFAIL: %d\n", result); // red
                }
                else if (result == 1) // no echo
                {
                }
            }
        }
    }

    this->resetRxBuffer();
    this->printPrompt();
}

const Shell::Command *Shell::findCommand(const char *name)
{
    for (const auto &command : Shell::shellCmds)
    {
        if (std::strcmp(command.name, name) == 0)
        {
            return &command;
        }
    }
    return nullptr;
}

int Shell::helpHandler(Shell &shell, int argc, char *argv[])
{
    for (const auto &command : Shell::shellCmds)
    {
        shell.echo(command.name);
        shell.echo(": ");
        shell.echoLine(command.help);
    }
    return 0;
}