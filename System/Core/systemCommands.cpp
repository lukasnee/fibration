
#include "shell.hpp"
#include "system.hpp"
#include "logger.hpp"

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "ticks.hpp"

#include "FreeRTOSConfig.h"

Shell::Command resetCommand(
    "reset", nullptr, "soft system reset",
    [](Shell &shell, std::size_t argc, const char *argv[]) -> Shell::Command::Result
    {
        __NVIC_SystemReset();
    });

void hexDump(Shell &shell, const std::uint32_t &address, const std::size_t &size)
{
    unsigned char *buf = reinterpret_cast<unsigned char *>(address);
    int buflen = static_cast<int>(size);
    int i, j;
    for (i = 0; i < buflen; i += 16)
    {
        shell.printf("%08x: ", address + i);
        for (j = 0; j < 16; j++)
            if (i + j < buflen)
                shell.printf("%02x ", buf[i + j]);
            else
                shell.printf("   ");
        shell.printf(" ");
        for (j = 0; j < 16; j++)
            if (i + j < buflen)
                shell.printf("%c", isprint(buf[i + j]) ? buf[i + j] : '.');
        shell.printf("\n");
    }
}

Shell::Command hexdumpCommand(
    "hd", "<address> <size>", "hex dump",
    [](Shell &shell, std::size_t argc, const char *argv[]) -> Shell::Command::Result
    {
        if (argc == 3)
        {
            unsigned int address;
            unsigned int size;
            if (std::sscanf(argv[1], "%x", &address) && std::sscanf(argv[2], "%x", &size))
            {
                hexDump(shell, address, size);
                return Shell::Command::Result::okQuiet;
            }
        }
        return Shell::Command::Result::fail;
    });

Shell::Command echoCommand(
    "echo", nullptr, "echos typed content",
    [](Shell &shell, std::size_t argc, const char *argv[]) -> Shell::Command::Result
    {
        if (argc > 1)
        {
            ArgVector argVector(argc - 1, argv + 1);
            std::array<char, ArgBuffer::Config::bufferSize> echoBuffer;
            argVector.printTo(echoBuffer.data(), echoBuffer.size(), " ", false);
            shell.printf("%s\n", echoBuffer.data());
            return Shell::Command::Result::ok;
        }
        return Shell::Command::Result::fail;
    });

Shell::Command panicCommand(
    "panic", nullptr, "cause system panic",
    [](Shell &shell, std::size_t argc, const char *argv[]) -> Shell::Command::Result
    {
        FibSys::panic();
        // will never be reached
        return Shell::Command::Result::ok;
    });

Shell::Command clearCommand(
    "c", nullptr, "clear screen",
    [](Shell &shell, std::size_t argc, const char *argv[]) -> Shell::Command::Result
    {
        std::size_t i = 0x20;
        while (i--)
        {
            shell.putChar('\n');
        }
        return Shell::Command::Result::okQuiet;
    });

Shell::Command statusCommand(
    "s", nullptr, "show system status",
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

Shell::Command controlCommand(
    "cfg", nullptr, "system configuration",
    [](Shell &shell, std::size_t argc, const char *argv[]) -> Shell::Command::Result
    {
        if (argc)
        {
            shell.printf("unknown command\n");
            return Shell::Command::Result::fail;
        }

        shell.printf("I can configure !\n");
        return Shell::Command::Result::okQuiet;
    },
    []()
    {
        static Shell::Command logCommand(
            controlCommand, "log", "<on|off>", "toggle logging",
            [](Shell &shell, std::size_t argc, const char *argv[]) -> Shell::Command::Result
            {
                Shell::Command::Result result = Shell::Command::Result::fail;

                if (argc == 2)
                {
                    if (!std::strcmp(argv[1], "on"))
                    {
                        Logger::setEnable(true);
                        result = Shell::Command::Result::ok;
                    }
                    else if (!std::strcmp(argv[1], "off"))
                    {

                        Logger::setEnable(false);
                        result = Shell::Command::Result::ok;
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

                return result;
            });
    });
