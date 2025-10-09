
#include "ln/shell/CLI.hpp"

#include "system.hpp"

#include <type_traits>

namespace ln::shell {

Cmd panic(
    "panic", nullptr, "cause high-level system panic",
    [](Cmd::Ctx ctx) -> Err {
        FIBSYS_PANIC();
        return Err::unexpected;
    },
    []() {
        static Cmd rba(panic, "rba", nullptr, "read bad address (hardfault)", [](Cmd::Ctx ctx) -> Err {
            return *reinterpret_cast<Err *>(0xbadcafe);
            /* should never be reached */
        });
    });
} // namespace ln::shell
