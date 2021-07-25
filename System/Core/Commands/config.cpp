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
            if (argc)
            {
                shell.printf("unknown command\n");
                return Shell::Command::Result::fail;
            }

            shell.printf("I can configure !\n");
            return Shell::Command::Result::okQuiet;
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
                            shell.printf("bad arg\n");
                        }
                    }
                    else
                    {
                        shell.printf("no arg\n");
                    }

                    return result;
                });
        });
} // namespace Core::Commands