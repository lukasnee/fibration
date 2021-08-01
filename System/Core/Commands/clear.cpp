
#include "commands.hpp"

namespace Core::Commands
{
    Shell::Command clear(
        "clear,c",
        nullptr,
        "clear screen",
        [](Shell &shell, std::size_t argc, const char *argv[]) -> Shell::Command::Result
        {
            std::size_t i = 0x20;
            while (i--)
            {
                shell.print('\n');
            }

            return Shell::Command::Result::okQuiet;
        });

} // namespace Core::Commands