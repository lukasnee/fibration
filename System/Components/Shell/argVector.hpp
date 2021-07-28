
#pragma once

#include <array>
#include <cstdint>

class ArgVector
{
public:
    struct Config
    {
        static constexpr std::size_t maxArgs = 16;
        static constexpr std::size_t maxDelimiterLength = 8;
    };

    ArgVector(std::size_t argc, const char **argv);
    ArgVector() = default;

    /**
     * @brief copy out arguments (contents) by given format into buffer.
     * 
     * @param delimiter Delimiter put printed after every argument, e.g.: " ", "\n"...
     * @param pBufferOut Buffer to print to.
     * @param bufferOutSize Buffer to print to size (max).
     * @param nullSeparated Puts null at the end of every delimiter between arguments. passing empty delimiter "" to copy arguments to some other buffer
     * @return true Printed successfully.
     * @return false Could not format the delimter (delimiter too long) or could not finish printing (no space left in buffer).
     */
    bool printTo(char *pBufferOut, std::size_t bufferOutSize, const char *delimiter = " ", bool nullSeparated = false);

    const std::size_t &getArgc() { return this->count; }
    const char **getArgv() { return this->args.data(); }

protected:
    /**
     * @attention this constructor modifies the input string !
     */
    ArgVector(char *pString, std::size_t stringLength);

    /**
     * @attention this function modifies the input string !
     */
    bool resolveStringToArgs(char *pString, std::size_t stringLength);

    std::size_t count = 0;
    std::array<const char *, Config::maxArgs> args{{nullptr}};
};
