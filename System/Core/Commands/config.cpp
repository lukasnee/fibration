#include "commands.hpp"
#include "logger.hpp"

namespace Core::Commands
{
    Shell::Command config(
        "config,cfg", nullptr, "system configuration", [](SHELLCMDPARAMS)
        {
            shell.print("TODO\n");
            return shell.help(shell, &config, true);
        });
} // namespace Core::Commands