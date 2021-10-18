
#include "commands.hpp"
#include "system.hpp"

#include <type_traits>
namespace Core::Commands
{
    Shell::Command panic(
        "panic", nullptr, "cause high-level system panic",
        [](SHELLCMDPARAMS)
        {
            FIBSYS_PANIC();
            // will never be reached
            return Shell::Command::Result::ok;
        },
        []()
        {
            static Shell::Command rba(panic, "rba", nullptr, "read bad address (hardfault)",
                                      [](SHELLCMDPARAMS)
                                      {
                                          return *reinterpret_cast<Shell::Command::Result *>(0xbadcafe);
                                          /* should never be reached */
                                      });
        });
} // namespace Core::Commands
