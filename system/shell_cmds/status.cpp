#include "ln/shell/CLI.hpp"

#include "FreeRTOS/Addons/Clock.hpp"

namespace ln::shell {

Cmd status_cmd{Cmd::Cfg{
    .name = "status,s", .short_description = "show system status", .fn = [](Cmd::Ctx ctx) {
        char run_time_stats_buf[20 * 40];
        vTaskGetRunTimeStats(run_time_stats_buf);
        ctx.cli.printf("uptime: %lu s\n",
                       duration_cast<std::chrono::seconds>(FreeRTOS::Addons::Clock::now().time_since_epoch()).count());
        ctx.cli.printf("freeHeapSbize: %lu\n\r", xPortGetFreeHeapSize());
        ctx.cli.printf("RunTimeStatsCntVal: %lu\n", uint32GetRunTimeCounterValue());
        ctx.cli.printf("Task\t\ttime,.1ms\ttime,%%\n%s", run_time_stats_buf);
        return Err::ok;
    }}};

} // namespace ln::shell
