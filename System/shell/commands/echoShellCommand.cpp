#include "shell.hpp"

Shell::Command echo("echo", nullptr, "echos typed content", [] ShellCommandFunctionLambdaSignature {
    Shell::Command::Result result = Shell::Command::Result::fail;

    if (argc > 1)
    {
        ArgVector argVector(argc - 1, argv + 1);
        std::array<char, ArgBuffer::Config::bufferSize> echoBuffer;
        argVector.printTo(echoBuffer.data(), echoBuffer.size(), " ", false);
        shell.printf("%s\n", echoBuffer.data());
        result = Shell::Command::Result::okQuiet;
    }

    return result;
});
