#include "ln/shell/CLI.hpp"
#include "ln/ln.h"

#include "stm32f3xx.h"

namespace ln::shell {

Cmd reset_cmd{Cmd::Cfg{.name = "reset",
                       .short_description = "soft system reset",
                       .fn = []([[maybe_unused]]
                                Cmd::Ctx ctx) {
                           ln::reset();
                           return Err::unexpected;
                       }}};

} // namespace ln::shell