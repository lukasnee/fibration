
#include "shell.hpp"

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
            shell.printf("%s\n", argv[i]);
        }
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
    static int i = 0;
    shell.printf("Hello World! %d\n", i++);
    return -2;
}

int shellCmdStatus(Shell &shell, int argc, char *argv[])
{
    char szRunTimeStats[10 * 40];
    vTaskGetRunTimeStats(szRunTimeStats);

    shell.printf(/*"initialFreeHeapSize:%lu\n\r"
                 "       freeHeapSize:%lu\n\r"
                 "    minFreeHeapSize:%lu\n\r"*/
                 "uptime: %lu s\n\r"
                 "RunTimeStatsCntVal:%lu\n\r"
                 "Task\t\ttime,.1ms\ttime,%%\n\r%s",
                 /*initialFreeHeapSize,
                 freeHeapSize,
                 minFreeHeapSize,*/
                 xTaskGetTickCount() / configTICK_RATE_HZ,
                 uint32GetRunTimeCounterValue(),
                 szRunTimeStats);
    return 0;
}

const std::array<Shell::Command, 4> Shell::shellCmds{{
    {"echo", "echos typed content", shellCmdEcho},
    {"clear", "clear screen", shellCmdClear},
    {"help", "Lists all commands", shellCmdHelp},
    {"status", "show system status", shellCmdStatus},
}};