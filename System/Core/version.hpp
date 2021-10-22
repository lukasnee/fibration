#pragma once

#define VERSION(moduleNameValue, majorValue, minorValue, patchValue) \
    namespace Fib                                                    \
    {                                                                \
        namespace Version                                            \
        {                                                            \
            constexpr const char *moduleName = #moduleNameValue;     \
            constexpr std::uint32_t major = majorValue;              \
            constexpr std::uint32_t minor = minorValue;              \
            constexpr std::uint32_t patch = patchValue;              \
        }                                                            \
    }

#include "version.def"

#define GITHASH(gitHashValue)                              \
    namespace Fib                                          \
    {                                                      \
        namespace Version                                  \
        {                                                  \
            constexpr const char *gitHash = #gitHashValue; \
            constexpr const char *compileDate = __DATE__;  \
            constexpr const char *compileTime = __TIME__;  \
        }                                                  \
    }

#include "gitHash.def"