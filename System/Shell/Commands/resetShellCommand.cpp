#include "shell.hpp"

Shell::Command reset("reset", nullptr, "soft system reset",
                     [] ShellCommandFunctionLambdaSignature { __NVIC_SystemReset(); });
