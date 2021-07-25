
#include "commands.hpp"

namespace Core::Commands
{
    Shell::Command reset(
        "reset", nullptr, "soft system reset",
        [](Shell &shell, std::size_t argc, const char *argv[]) -> Shell::Command::Result
        {
            __NVIC_SystemReset();
        });
} // namespace Core::Commands
