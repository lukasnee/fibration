
#include "ln/shell/CLI.hpp"

#include "system.hpp"

#include <type_traits>

namespace ln::shell {

Cmd panic_cmd{Cmd::Cfg{.name = "panictest",
                       .short_description = "trigger a system panic for testing purposes",
                       .long_description = R"(Usage:
  panictest
  panictest rba

Commands:
  rba        read bad address to trigger hardfault)",
                       .fn = [](Cmd::Ctx ctx) {
                           if (ctx.args.size() == 0) {
                               FIBSYS_PANIC();
                           }
                           using namespace std::literals::string_view_literals;
                           if (ctx.args.size() == 1 && ctx.args[0] == "rba"sv) {
                               return *reinterpret_cast<Err *>(0xbadcafe);
                           }
                           return Err::badArg;
                       }}};

} // namespace ln::shell
