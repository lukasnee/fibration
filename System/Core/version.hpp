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
