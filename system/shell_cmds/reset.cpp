#include "ln/shell/CLI.hpp"

#include "stm32f3xx.h"

namespace ln::shell {

Cmd reset_cmd{Cmd::Cfg{.name = "reset", .short_description = "soft system reset", .fn = [](Cmd::Ctx ctx) {
                           __NVIC_SystemReset();
                           return Err::unexpected;
                       }}};

} // namespace ln::shell