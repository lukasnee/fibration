
#include "shell.hpp"
#include "system.hpp"

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

int shellCmdEcho(Shell &shell, int argc, char *argv[])
{
    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            shell.printf("%s ", argv[i]);
        }
        shell.printc('\n');
        return 0;
    }
    return -1;
}

int shellCmdClear(Shell &shell, int argc, char *argv[])
{
    std::size_t i = 0x20;
    while (i--)
    {
        shell.printc('\n');
    }
    return 1;
}

int shellCmdHelp(Shell &shell, int argc, char *argv[])
{
        for (auto &cmd : Shell::shellCmds)
        {
            shell.printf("%s - %s\n", cmd.name, cmd.help);
        }
        return 0;
}

int shellCmdStatus(Shell &shell, int argc, char *argv[])
{
    char szRunTimeStats[10 * 40];
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
    return 0;
}

int shellPanic(Shell &shell, int argc, char *argv[])
{
    FibSys::panic();
    // will never be reached
    return 0;
}

int shellCmdLogToggle(Shell &shell, int argc, char *argv[])
{
    int result = -1;

    if (argc == 2)
    {
        if (!std::strcmp(argv[1], "on"))
        {
            Logger::setEnable(true);
            result = 0;
}
        else if (!std::strcmp(argv[1], "off"))
        {

            Logger::setEnable(false);
            result = 0;
        }
        else
{
            shell.printf("bad arg\n");
        }
    }
    else
    {
        shell.printf("no arg\n");
    }
}

const std::array<Shell::Command, 6> Shell::shellCmds{{
    {"help", "Lists all commands", shellCmdHelp},
    {"echo", "echos typed content", shellCmdEcho},
    {"panic", "cause system panic", shellCmdPanic},
    {"c", "clear screen", shellCmdClear},
    {"s", "show system status", shellCmdStatus},
    {"p", "cause system panic", shellCmdPanic},
    {"l", "<on|off>", shellCmdLogToggle},
}};
