
#include "system.hpp"
#include "shell.hpp"

#include "ticks.hpp"
#include "FreeRTOSConfig.h"

#include <array>
#include <stdint.h>

class WatchThread : public cpp_freertos::Thread
{
public:
    WatchThread(Shell &shell, std::size_t periodMs, const Shell::Command &command, const std::size_t &argc, const char **&argv)
        : Thread("watch", 1000, FibSys::Priority::appLow), pShell(&shell), periodMs(periodMs), pCommand(&command), argBuffer(argc, argv)
    {
    }

private:
    void Run() override
    {
        while (true)
        {
            if (this->pShell && this->pCommand)
            {
                this->pShell->printf("\n\n\nwatching \'");

                std::array<char, ArgBuffer::Config::bufferSize> commandString;
                if (argBuffer.printTo(reinterpret_cast<char *>(commandString.data()), commandString.size()))
                {
                    // TODO does not print ?wtfg????
                    this->pShell->printf(reinterpret_cast<const char *>(commandString.data()));
                }
                this->pShell->printf("\' every %lu ms\n---\n\n", this->periodMs);

                this->pShell->execute(*this->pCommand, this->argBuffer.getArgc(), this->argBuffer.getArgv(), "\e[36m");
            }
            this->Delay(cpp_freertos::Ticks::MsToTicks(this->periodMs));
        }
    }

    Shell *pShell = nullptr;
    std::size_t periodMs;
    const Shell::Command *pCommand;
    ArgBuffer argBuffer;
};

static Shell::Command::Result watchCommandCallback(Shell &shell, std::size_t argc, const char *argv[])
{
    static bool isWatchThreadStarted = false;
    static WatchThread *pWatchThread = nullptr;

    if (argc > 2)
    {
        std::size_t argcLeft;
        const char **argvLeft;
        const Shell::Command *pCommand = shell.findCommand(argc - 2, &argv[2], argcLeft, argvLeft);

        if (pCommand)
        {
            if (!isWatchThreadStarted)
            {
                pWatchThread = new WatchThread(shell, std::strtoul(argv[1], nullptr, 10), *pCommand, argcLeft, argvLeft);
                isWatchThreadStarted = pWatchThread->Start();
                if (isWatchThreadStarted)
                {
                    return Shell::Command::Result::ok;
                }
                else
                {
                    return Shell::Command::Result::fail4;
                }
            }
            else
            {
                return Shell::Command::Result::fail3;
            }
        }
        else
        {
            return Shell::Command::Result::fail;
        }
    }
    else if (argc == 1)
    {
        if (isWatchThreadStarted)
        {
            delete pWatchThread;
            isWatchThreadStarted = false;
            //TODO maybe clear screen here
            shell.printf("watch stopped\n");
            return Shell::Command::Result::ok;
        }
    }

    return Shell::Command::Result::badArg;
}

Shell::Command watchCommand("w", "<periodMs> <COMMAND...>", "watch command at a given period", watchCommandCallback);