
#include "commands.hpp"

namespace Core::Commands
{
    Shell::Command status(
        "status,s",
        nullptr,
        "show system status",
        [](Shell &shell, std::size_t argc, const char *argv[]) -> Shell::Command::Result
        {
            char szRunTimeStats[20 * 40];
            vTaskGetRunTimeStats(szRunTimeStats);

            shell.printf(/*"initialFreeHeapSize:%lu\n\r"
                 "       freeHeapSize:%lu\n\r"
                 "    minFreeHeapSize:%lu\n\r"*/
                         "uptime: %lu s\n"
                         "RunTimeStatsCntVal:%lu\n"
                         "Task\t\ttime,.1ms\ttime,%%\n%s",
                         /*initialFreeHeapSize,
                    freeHeapSize,
                    minFreeHeapSize,*/
                         xTaskGetTickCount() / configTICK_RATE_HZ,
                         uint32GetRunTimeCounterValue(),
                         szRunTimeStats);
            return Shell::Command::Result::ok;
        });
} // namespace Core::Commands
