#include "shell.hpp"
#include "logger.hpp"
#include "system.hpp"
#include "resources.hpp"
// TODO: make arrow up repeat buffer
#include <cstring>

Shell::Command::Result Shell::help(Shell &shell, const Shell::Command *pCommand, bool recurse, const std::size_t maxDepth, std::size_t depth, std::size_t indent)
{
    Shell::Command::Result result = Shell::Command::Result::ok;

    if (!pCommand)
    {
        result = Command::Result::badArg;
    }
    else
    {
        constexpr int commandColumnWidth = 40;

        for (const Shell::Command *pCmdIt = pCommand; pCmdIt != nullptr; pCmdIt = pCmdIt->pNext)
        {
            if (indent >= 3)
            {
                shell.print(' ', indent - 3);
                // shell.print("|\n");
                // shell.print(' ', indent - 3);
                shell.print("`- ");
            }

            int charsPrinted = 0;

            if (pCmdIt->name)
            {
                if (pCmdIt->usage)
                {
                    charsPrinted = shell.printf("%s %s ", pCmdIt->name, pCmdIt->usage);
                }
                else
                {
                    charsPrinted = shell.printf("%s  ", pCmdIt->name);
                }
            }

            if (charsPrinted > 0)
            {
                if (charsPrinted < commandColumnWidth)
                {
                    shell.print(' ', commandColumnWidth - charsPrinted - indent);
                }

                if (pCmdIt->description)
                {
                    charsPrinted = shell.print(pCmdIt->description);
                }

                shell.print('\n');

                if (charsPrinted >= 0)
                {
                    result = Command::Result::ok;
                }
            }

            if (result == Command::Result::ok && recurse && depth < maxDepth && pCmdIt->pSubcommands)
            {
                result = help(shell, pCmdIt->pSubcommands, recurse, maxDepth, depth + 1, indent + strlen(pCmdIt->name) + sizeof(' '));
            }

            if (depth == 0)
            {
                break;
            }
        }
    }
    return result;
}

Shell::Command Shell::helpCommand = Shell::Command(
    "help", "[all|[COMMAND...]]", "show command usage",
    [](Shell &shell, std::size_t argc, const char *argv[]) -> Shell::Command::Result
    {
        Shell::Command::Result result = Shell::Command::Result::unknown;

        if (argc)
        {
            if (argc == 1)
            {
                for (const Shell::Command *pCmdIt = Shell::pCommandGlobalRoot; pCmdIt != nullptr; pCmdIt = pCmdIt->pNext)
                {
                    result = Shell::help(shell, pCmdIt, 0, false);
                }
            }
            else if (argc == 2 && !std::strcmp(argv[1], "all"))
            {
                for (const Shell::Command *pCmdIt = Shell::pCommandGlobalRoot; pCmdIt != nullptr; pCmdIt = pCmdIt->pNext)
                {
                    result = Shell::help(shell, pCmdIt, true, 7);
                }
            }
            else if (argc > 1)
            {
                constexpr std::size_t helpCommandOffset = 1;
                std::size_t argOffset;
                const Command *pCommandFound = shell.findCommand(argc - helpCommandOffset, argv + helpCommandOffset, argOffset);
                if (pCommandFound)
                {
                    result = Shell::help(shell, pCommandFound, 1, true);
                }
            }
        }
        return result;
    });

Shell::Command::Result Shell::Command::Helper::onOffCommand(std::function<bool(bool)> onOffF, const char *strOnOffControlName, SHELLCMDPARAMS)
{
    Shell::Command::Result result = Shell::Command::Result::fail;

    if (argc != 2)
    {
        shell.print(Literal::noArg);
    }
    else if (!std::strcmp(argv[1], Literal::on))
    {
        if (onOffF(true))
        {
            shell.printf("%s %s %s\n", strOnOffControlName, Literal::verb, Literal::on);
            result = Shell::Command::Result::ok;
        }
    }
    else if (!std::strcmp(argv[1], Literal::off))
    {
        if (onOffF(false))
        {
            shell.printf("%s %s %s\n", strOnOffControlName, Literal::verb, Literal::off);
            result = Shell::Command::Result::ok;
        }
    }
    else
    {
        shell.print(Literal::badArg);
    }

    return result;
};

Shell::Command::Result Shell::Command::Helper::onOffCommand(bool &onOffControl, const char *strOnOffControlName, SHELLCMDPARAMS)
{
    return onOffCommand([&](bool state)
                        {
                            onOffControl = state;
                            return true;
                        },
                        strOnOffControlName, SHELLCMDARGS);
};

void Shell::Command::linkTo(Command *&pParent)
{
    if (!pParent)
    {
        pParent = this;
    }
    else
    {
        Command *pNext = pParent;
        while (pNext->pNext)
        {
            pNext = pNext->pNext;
        }
        pNext->pNext = this;
    }
}

Shell::Command::Command(const char *name, const char *usage, const char *description,
                        std::function<Result(SHELLCMDPARAMS)> commandF, std::function<void()> ctorCallbackF)
    : name(name), usage(usage), description(description), commandF(commandF)
{
    this->linkTo(Shell::pCommandGlobalRoot);
    if (ctorCallbackF)
    {
        ctorCallbackF();
    }
}

Shell::Command::Command(Command &parent, const char *name, const char *usage, const char *description,
                        std::function<Result(SHELLCMDPARAMS)> commandF, std::function<void()> ctorCallbackF)
    : name(name), usage(usage), description(description), commandF(commandF)
{
    this->linkTo(parent.pSubcommands);
    if (ctorCallbackF)
    {
        ctorCallbackF();
    }
}

Shell::Command::Command(const char *name, std::function<Result(SHELLCMDPARAMS)> commandF)
    : name(name), usage(nullptr), description(nullptr), commandF(commandF)
{
    this->linkTo(Shell::pCommandGlobalRoot);
}

bool Shell::Command::matchToken(const char *strTokens, const char *strToken)
{
    bool result = false;

    const std::size_t strTokenLength = std::strlen(strToken);
    const char *strThisToken = strTokens;

    for (const char *strCharIt = strThisToken; *strCharIt != '\0'; strCharIt++)
    {
        const bool itAtLastChar = (*(strCharIt + 1) == '\0');
        if (*strCharIt == ',' || itAtLastChar)
        {
            const std::size_t thisTokenLength = strCharIt + (itAtLastChar ? 1 : 0) - strThisToken;
            if (strTokenLength == thisTokenLength && 0 == std::strncmp(strToken, strThisToken, thisTokenLength))
            {
                result = true;
                break;
            }
            else if (*strCharIt == ',')
            {
                strThisToken = strCharIt + 1;
            }
        }
    }

    return result;
}

const Shell::Command *Shell::Command::findNeighbourCommand(const char *name) const
{
    const Command *result = nullptr;

    for (const Command *pNext = this; pNext != nullptr; pNext = pNext->pNext)
    {
        if (Command::matchToken(pNext->name, name))
        {
            result = pNext;
            break;
        }
    }

    return result;
}

const Shell::Command *Shell::Command::findSubcommand(const char *name) const
{
    const Command *result = nullptr;

    for (const Command *pNext = this->pSubcommands; pNext != nullptr; pNext = pNext->pNext)
    {
        if (Command::matchToken(pNext->name, name))
        {
            result = pNext;
            break;
        }
    }

    return result;
}

Shell::Command *Shell::pCommandGlobalRoot = nullptr;
