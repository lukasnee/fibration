
#include "commands.hpp"

namespace Core::Commands
{
    void hexdumpCb(Shell &shell, const std::uint32_t &address, const std::size_t &size)
    {
        unsigned char *buf = reinterpret_cast<unsigned char *>(address);
        int buflen = static_cast<int>(size);
        int i, j;
        for (i = 0; i < buflen; i += 16)
        {
            shell.printf("%08x: ", address + i);
            for (j = 0; j < 16; j++)
                if (i + j < buflen)
                    shell.printf("%02x ", buf[i + j]);
                else
                    shell.print("   ");
            shell.print(' ');
            for (j = 0; j < 16; j++)
                if (i + j < buflen)
                    shell.print(isprint(buf[i + j]) ? buf[i + j] : '.');
            shell.print('\n');
        }
    }

    Shell::Command hexdump(
        "hexdump,hd",
        "<addressHex> <sizeHex>",
        "hex dump",
        [](Shell &shell, std::size_t argc, const char *argv[]) -> Shell::Command::Result
        {
            Shell::Command::Result result = Shell::Command::Result::fail;

            if (argc == 3)
            {
                unsigned int address;
                unsigned int size;
                if (std::sscanf(argv[1], "%x", &address) && std::sscanf(argv[2], "%x", &size))
                {
                    hexdumpCb(shell, address, size);
                    result = Shell::Command::Result::okQuiet;
                }
            }
            return result;
        });
} // namespace Core::Commands