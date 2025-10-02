#include "ln/shell/CLI.hpp"

#include "FreeRTOS.h"
#include "task.h"
namespace ln::shell {

char szRunTimeStats[20 * 40];

Cmd status("status,s", nullptr, "show system status", [](Cmd::Ctx ctx) {
    vTaskGetRunTimeStats(szRunTimeStats);
    ctx.cli.printf("uptime: %lu s\n",
                  duration_cast<std::chrono::seconds>(FreeRTOS::Addons::Clock::now().time_since_epoch()).count());
    ctx.cli.printf("freeHeapSbize: %lu\n\r", xPortGetFreeHeapSize());
    ctx.cli.printf("RunTimeStatsCntVal: %lu\n", uint32GetRunTimeCounterValue());
    ctx.cli.printf("Task\t\ttime,.1ms\ttime,%%\n%s", szRunTimeStats);
    return Err::ok;
});

} // namespace ln::shell
