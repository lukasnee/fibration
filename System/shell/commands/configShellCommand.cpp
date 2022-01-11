#include "shell.hpp"

#include "logger.hpp"

Shell::Command config("config,cfg", nullptr, "system configuration", [] ShellCommandFunctionLambdaSignature {
    shell.print("TODO\n");
    return shell.help(shell, &config, true);
});
