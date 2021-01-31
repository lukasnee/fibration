#include "shell.hpp"

#include "system.hpp"
#include "peripherals.hpp"

void Shell::start(Stream &stream, std::uint16_t stackDepth, BaseType_t priority)
{
    static Shell shell(stream, stackDepth, priority);
}

Shell::Shell(Stream &stream, std::uint16_t stackDepth, BaseType_t priority)
    : Thread("shell", stackDepth, priority), stream(stream)
{
    this->echoEndLine();
    this->echoLine("Type 'help' to list all commands");
    this->promptNew();

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
    // Consume and send characters to shell...forever
    char c;
    while (true)
    {
        c = this->stream.getc();
        this->receiveChar(c);
    }
}

void Shell::echo(char c)
{
    if (c == '\n')
    {
        this->stream.putc('\r');
    }
    this->stream.putc(c);
}

void Shell::echo(const char *string)
{
    for (const char *c = string; *c != '\0'; ++c)
    {
        this->echo(*c);
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
    this->rxSize = 0;
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

void Shell::receiveChar(char c)
{
    if (c == '\r') // end of line
    {
        if(this->rxBuffer.size() - this->rxSize > 1) 
        {
            //this->rxBuffer[this->rxSize++] = '\r';
            this->rxBuffer[this->rxSize++] = '\n';
        }
        this->echoEndLine();
    }
    else if (c == '\b') // backspace
    {
        if(this->rxSize) 
        {
            this->rxBuffer[--this->rxSize] = '\0';
            this->echo("\b \b");
        }
        return;
    }
    else if(c >= 0x20) // ascii text symbols
    {
        this->rxBuffer[this->rxSize++] = c;
        this->echo(c);
    }
    else{
        return;
    }

    this->process();
}

void Shell::process(void)
{
    if (this->lastChar() == '\n')
    {
        std::array<char *, Shell::maxArgs> argv{{0}};
        std::size_t argc = 0;

        char *nextArg = nullptr;
        for (std::size_t i = 0; i < this->rxSize && argc < argv.size(); ++i)
        {
            char *const c = &this->rxBuffer[i];
            if (*c == ' ' || *c == '\n' || i == this->rxSize - 1)
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
                this->printf("\e[39mfibration: %s: command not found", argv[0]);
                this->echoLine(argv[0]);
            }
            else
            {
                echo("\e[32m"); // response in green
                int result = command->handler(*this, argc, argv.data());
                if(ShellConfig::regularResponseIsEnabled)
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
}

char Shell::lastChar(void)
{
    return this->rxBuffer.at(this->rxSize - 1);
}

bool Shell::isRxBufferFull(void)
{
    return (this->rxSize >= Shell::rxBufferSize);
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