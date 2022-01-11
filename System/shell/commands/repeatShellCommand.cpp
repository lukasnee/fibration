#include "shell.hpp"

#include "system.hpp"

#include "FreeRTOSConfig.h"
#include "ticks.hpp"

#include <array>
#include <stdint.h>

class RepeatCommandThread : public cpp_freertos::Thread
{
public:
    RepeatCommandThread(Shell &shell, std::size_t periodMs, const Shell::Command &command, std::size_t argc,
                        const char **argv, std::size_t commandArgOffset)
        : Thread("repeat", 1000, FibSys::Priority::appLow), pShell(&shell), periodMs(periodMs), pCommand(&command),
          commandArgOffset(commandArgOffset), argBuffer(argc, argv)
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
                this->pShell->printf("repeating command \'%s\' every %lu ms\n\n", array.data(), this->periodMs);
                this->pShell->execute(*this->pCommand, this->argBuffer.getArgc() - this->commandArgOffset,
                                      this->argBuffer.getArgv() + this->commandArgOffset, "\e[36m");
            }
            this->DelayUntil(cpp_freertos::Ticks::MsToTicks(this->periodMs));
        }
    }

    Shell *pShell = nullptr;
    std::size_t periodMs;
    const Shell::Command *pCommand;
    std::size_t commandArgOffset;
    ArgBuffer argBuffer;
};

Shell::Command repeat("repeat,r", "<periodMs> <COMMAND...>", "repeat command at a given period",
                      [] ShellCommandFunctionLambdaSignature {
                          static bool isRepeatCommandThreadStarted = false;
                          static RepeatCommandThread *pRepeatCommandThread = nullptr;

                          if (argc > 2)
                          {
                              constexpr std::size_t repeatCommandArgOffset = 2;
                              std::size_t repeatedCommandArgOffset;
                              const Shell::Command *pCommand =
                                  shell.findCommand(argc - repeatCommandArgOffset, argv + repeatCommandArgOffset,
                                                    repeatedCommandArgOffset);

                              if (pCommand)
                              {
                                  if (!isRepeatCommandThreadStarted)
                                  {
                                      pRepeatCommandThread = new RepeatCommandThread(
                                          shell, std::strtoul(argv[1], nullptr, 10), *pCommand,
                                          argc - repeatCommandArgOffset, argv + repeatCommandArgOffset,
                                          repeatCommandArgOffset);

                                      isRepeatCommandThreadStarted = pRepeatCommandThread->Start();
                                      if (isRepeatCommandThreadStarted)
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
                              if (isRepeatCommandThreadStarted)
                              {
                                  delete pRepeatCommandThread;
                                  isRepeatCommandThreadStarted = false;
                                  shell.printf("repeat thread stopped\n");
                                  return Shell::Command::Result::ok;
                              }
                          }

                          return Shell::Command::Result::badArg;
                      });
