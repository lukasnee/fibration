#include "shell.hpp"

#include "system.hpp"
#include "peripherals.hpp"

void Shell::start(Stream &stream, std::uint16_t stackDepth, BaseType_t priority)
{
    static Shell shell(stream, stackDepth, priority);
}

Shell::Shell(Stream &stream, std::uint16_t stackDepth, BaseType_t priority) : Thread("shell", stackDepth, priority), stream(stream)
{
    this->resetRxBuffer();

    this->echoEndLine();
    this->sendPrompt();

    if (Start() == false)
    {
        FibSys::panic();
    }
};

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

void Shell::resetRxBuffer(void)
{
    this->rxBuffer.fill(0);
    this->rxSize = 0;
}

void Shell::sendPrompt(void)
{
    this->echo(Shell::prompt.data());
}

void Shell::receiveChar(char c)
{
    if (this->isRxBufferFull() == false)
    {
        this->echo(c);

        if (c == '\b')
        {
            this->rxBuffer[--this->rxSize] = '\0';
            return;
        }

        this->rxBuffer[this->rxSize++] = c;

        this->process();
    }
}

void Shell::echo(char c)
{
    if (c == '\r') // end of line
    {
        this->stream.putc('\r');
        this->stream.putc('\n'); 
    }
    else if (c == '\b') // backspace
    {
        this->stream.putc('\b');
        this->stream.putc(' ');
        this->stream.putc('\b');
    }
    else
    {
        this->stream.putc(c);
    }
}

void Shell::echo(const char *string)
{
    for (const char *c = string; *c != '\0'; ++c)
    {
        this->echo(*c);
    }
}

void Shell::echoEndLine()
{
    this->echo("\r");
}

void Shell::echoLine(const char *string)
{
    for (const char *c = string; *c != '\0'; ++c)
    {
        this->echo(*c);
    }
    this->echoEndLine();
}

void Shell::process(void)
{
    if (this->lastChar() == '\r' || this->isRxBufferFull())
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

        if (this->rxSize == Shell::rxBufferSize)
        {
            this->echo('\n');
        }

        if (argc >= 1)
        {
            const Shell::Command *command = findCommand(argv[0]);
            if (!command)
            {
                this->echo("Unknown command: ");
                this->echoLine(argv[0]);
                this->echoLine("Type 'help' to list all commands");
            }
            else
            {
                command->handler(argc, argv.data());
            }
        }
        this->resetRxBuffer();
        this->sendPrompt();
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

const Shell::Command *Shell::findCommand(const char * name)
{
    for (const auto &command : Shell::commands)
    {
        if (std::strcmp(command.name, name) == 0)
        {
            return &command;
        }
    }
    return nullptr;
}

int Shell::helpHandler(int argc, char *argv[])
{
    for (const auto &command : Shell::commands)
    {
        this->echo(command.name);
        this->echo(": ");
        this->echoLine(command.help);
    }
    return 0;
}