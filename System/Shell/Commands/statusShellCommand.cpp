#include "shell.hpp"

char szRunTimeStats[20 * 40];

Shell::Command status("status,s", nullptr, "show system status", [] ShellCommandFunctionLambdaSignature {
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
                 xTaskGetTickCount() / configTICK_RATE_HZ, uint32GetRunTimeCounterValue(), szRunTimeStats);
    return Shell::Command::Result::ok;
});
