
#include "shell.hpp"

namespace Core::Commands
{
    extern Shell::Command clear;
    extern Shell::Command config;
    extern Shell::Command echo;

    void hexdumpCb(Shell &shell, const std::uint32_t &address, const std::size_t &size);
    extern Shell::Command hexdump;
    
    extern Shell::Command panic;
    extern Shell::Command reset;
    extern Shell::Command status;
    extern Shell::Command watch;
} // namespace Core::Commands
