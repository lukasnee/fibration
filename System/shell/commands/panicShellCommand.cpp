
#include "shell.hpp"

#include "system.hpp"

#include <type_traits>

Shell::Command panic(
    "panic", nullptr, "cause high-level system panic",
    [] ShellCommandFunctionLambdaSignature {
        FIBSYS_PANIC();
        // will never be reached
        return Shell::Command::Result::ok;
    },
    []() {
        static Shell::Command rba(panic, "rba", nullptr, "read bad address (hardfault)",
                                  [] ShellCommandFunctionLambdaSignature {
                                      return *reinterpret_cast<Shell::Command::Result *>(0xbadcafe);
                                      /* should never be reached */
                                  });
    });
