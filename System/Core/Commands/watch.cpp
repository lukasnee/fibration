
#include "commands.hpp"

#include "system.hpp"
#include "shell.hpp"

#include "ticks.hpp"
#include "FreeRTOSConfig.h"

#include <array>
#include <stdint.h>

class WatchThread : public cpp_freertos::Thread
{
public:
    WatchThread(Shell &shell, std::size_t periodMs, const Shell::Command &command, std::size_t argc, const char **argv, std::size_t commandArgOffset)
        : Thread("watch", 1000, FibSys::Priority::appLow),
          pShell(&shell),
          periodMs(periodMs),
          pCommand(&command),
          commandArgOffset(commandArgOffset),
          argBuffer(argc, argv)
    {
    }

private:
    void Run() override
    {
        while (true)
        {
            if (this->pShell && this->pCommand)
            {
                std::array<char, 256> array;
                this->argBuffer.printTo(array.data(), array.size(), " ");
                this->pShell->printf("watching \'%s\' every %lu ms\n\n", array.data(), this->periodMs);
                this->pShell->execute(*this->pCommand,
                                      this->argBuffer.getArgc() - this->commandArgOffset,
                                      this->argBuffer.getArgv() + this->commandArgOffset,
                                      "\e[36m");
            }
            this->Delay(cpp_freertos::Ticks::MsToTicks(this->periodMs));
        }
    }

    Shell *pShell = nullptr;
    std::size_t periodMs;
    const Shell::Command *pCommand;
    std::size_t commandArgOffset;
    ArgBuffer argBuffer;
};

namespace Core::Commands
{
    Shell::Command watch(
        "watch,w",
        "<periodMs> <COMMAND...>",
        "watch command at a given period",
        [](Shell &shell, std::size_t argc, const char *argv[]) -> Shell::Command::Result
        {
            static bool isWatchThreadStarted = false;
            static WatchThread *pWatchThread = nullptr;

            if (argc > 2)
            {
                constexpr std::size_t watchCmdArgOffset = 2;
                std::size_t watchedCmdArgOffset;
                const Shell::Command *pCommand = shell.findCommand(
                    argc - watchCmdArgOffset,
                    argv + watchCmdArgOffset,
                    watchedCmdArgOffset);

                if (pCommand)
                {
                    if (!isWatchThreadStarted)
                    {
                        pWatchThread = new WatchThread(shell,
                                                       std::strtoul(argv[1], nullptr, 10),
                                                       *pCommand,
                                                       argc - watchCmdArgOffset,
                                                       argv + watchCmdArgOffset,
                                                       watchedCmdArgOffset);

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
        });
} // namespace Core::Commands