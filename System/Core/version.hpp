#pragma once

#define VERSION(majorValue, minorValue, patchValue)     \
    namespace Fib                                       \
    {                                                   \
        namespace Version                               \
        {                                               \
            constexpr std::uint32_t major = majorValue; \
            constexpr std::uint32_t minor = minorValue; \
            constexpr std::uint32_t patch = patchValue; \
        }                                               \
    }

#include "version.def"
