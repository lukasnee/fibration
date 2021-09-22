
#include "commands.hpp"
#include "system.hpp"

namespace Core::Commands
{
    Shell::Command panic(
        "panic", nullptr, "cause system panic",
        [](Shell &shell, std::size_t argc, const char *argv[]) -> Shell::Command::Result
        {
            FIBSYS_PANIC();
            // will never be reached
            return Shell::Command::Result::ok;
        });
} // namespace Core::Commands
