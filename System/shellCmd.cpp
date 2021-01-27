
#include "shell.hpp"

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

int shellCmdEcho(Shell &shell, int argc, char *argv[])
{
    if(argc > 1) 
    {
        for(int i = 1; i < argc; i++)
        {
            shell.printf("%s ", argv[i]);
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

const std::array<Shell::Command, 3> Shell::shellCmds = {{
    {"echo", "echos typed content", shellCmdEcho},
    {"clear", "clear screen", shellCmdClear},
    {"help", "Lists all commands", shellCmdHelp},
}};