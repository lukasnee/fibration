#include "shell.hpp"

Shell::Command clear("clear,c", nullptr, "clear screen", [] ShellCommandFunctionLambdaSignature {
    std::size_t i = 0x30;
    while (i--)
    {
        shell.print('\n');
    }

    return Shell::Command::Result::okQuiet;
});
