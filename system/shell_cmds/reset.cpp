#include "ln/shell/CLI.hpp"

#include "stm32f3xx.h"

namespace ln::shell {

Cmd reset("reset", nullptr, "soft system reset", [](Cmd::Ctx ctx) -> Err { __NVIC_SystemReset(); });

} // namespace ln::shell