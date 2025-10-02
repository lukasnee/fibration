#include "shell.hpp"

#include "system.hpp"

#include "FreeRTOS/Task.hpp"
#include "FreeRTOS/Addons/Clock.hpp"

#include <array>
#include <stdint.h>

class RepeatCommandThread : public FreeRTOS::Task {
public:
    RepeatCommandThread(Shell &shell, FreeRTOS::Addons::Clock::duration period, const Shell::Command &command,
                        std::size_t argc, const char **argv, std::size_t commandArgOffset)
        : Task(FibSys::Priority::appLow, 1000, "repeat"), pShell(&shell), period(period), pCommand(&command),
          commandArgOffset(commandArgOffset), argBuffer(argc, argv) {}

private:
    void taskFunction() override {
        while (true) {
            if (this->pShell && this->pCommand) {
                std::array<char, 256> array;
                this->argBuffer.printTo(array.data(), array.size(), " ");
                this->pShell->printf("repeating command \'%s\' every %lu ms\n\n", array.data(),
                                     std::chrono::duration_cast<std::chrono::milliseconds>(this->period).count());
                this->pShell->execute(*this->pCommand, this->argBuffer.getArgc() - this->commandArgOffset,
                                      this->argBuffer.getArgv() + this->commandArgOffset, "\e[36m");
            }
            this->delayUntil(this->period);
        }
    }

    Shell *pShell = nullptr;
    FreeRTOS::Addons::Clock::duration period;
    const Shell::Command *pCommand;
    std::size_t commandArgOffset;
    ArgBuffer argBuffer;
};

Shell::Command repeat("repeat,r", "<period_ms> <COMMAND...>", "repeat command at a given period",
                      [] ShellCommandFunctionLambdaSignature {
                          static bool isRepeatCommandThreadStarted = false;
                          static RepeatCommandThread *pRepeatCommandThread = nullptr;

                          if (argc > 2) {
                              constexpr std::size_t repeatCommandArgOffset = 2;
                              std::size_t repeatedCommandArgOffset;
                              const Shell::Command *pCommand =
                                  shell.findCommand(argc - repeatCommandArgOffset, argv + repeatCommandArgOffset,
                                                    repeatedCommandArgOffset);

                              if (pCommand) {
                                  if (!isRepeatCommandThreadStarted) {
                                      void *threadMemory = pvPortMalloc(sizeof(RepeatCommandThread));
                                      if (threadMemory != nullptr) {
                                          pRepeatCommandThread = new (threadMemory) RepeatCommandThread(
                                              shell, std::chrono::milliseconds(std::strtoul(argv[1], nullptr, 10)),
                                              *pCommand, argc - repeatCommandArgOffset, argv + repeatCommandArgOffset,
                                              repeatCommandArgOffset);
                                      }
                                      else {
                                          return Shell::Command::Result::fail5;
                                      }
                                      isRepeatCommandThreadStarted = true;
                                      if (isRepeatCommandThreadStarted) {
                                          return Shell::Command::Result::ok;
                                      }
                                      else {
                                          return Shell::Command::Result::fail4;
                                      }
                                  }
                                  else {
                                      return Shell::Command::Result::fail3;
                                  }
                              }
                              else {
                                  return Shell::Command::Result::fail;
                              }
                          }
                          else if (argc == 1) {
                              if (isRepeatCommandThreadStarted) {
                                  pRepeatCommandThread->~RepeatCommandThread();
                                  vPortFree(pRepeatCommandThread);
                                  isRepeatCommandThreadStarted = false;
                                  shell.printf("repeat thread stopped\n");
                                  return Shell::Command::Result::ok;
                              }
                          }

                          return Shell::Command::Result::badArg;
                      });
