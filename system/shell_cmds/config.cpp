#include "ln/shell/CLI.hpp"

namespace ln::shell {

Cmd config("config,cfg", nullptr, "system configuration", [](Cmd::Ctx ctx) -> Err {
    ctx.cli.print("TODO\n");
    return Err::fail;
});

} // namespace ln::shell
