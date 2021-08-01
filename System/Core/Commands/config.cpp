#include "commands.hpp"
#include "logger.hpp"

namespace Core::Commands
{
    Shell::Command config(
        "config,cfg",
        nullptr,
        "system configuration",
        [](Shell &shell, std::size_t argc, const char *argv[]) -> Shell::Command::Result
        {
            return shell.help(shell, &config, true);
        },
        []()
        {
            static Shell::Command log(
                config,
                "log",
                "<on|off>",
                "toggle logging",
                [](Shell &shell, std::size_t argc, const char *argv[]) -> Shell::Command::Result
                {
                    Shell::Command::Result result = Shell::Command::Result::fail;

                    if (argc == 2)
                    {
                        if (!std::strcmp(argv[1], "on"))
                        {
                            Logger::setEnable(true);
                            result = Shell::Command::Result::ok;
                        }
                        else if (!std::strcmp(argv[1], "off"))
                        {

                            Logger::setEnable(false);
                            result = Shell::Command::Result::ok;
                        }
                        else
                        {
                            shell.print("bad arg\n");
                        }
                    }
                    else
                    {
                        shell.print("no arg\n");
                    }

                    return result;
                });
        });
} // namespace Core::Commands