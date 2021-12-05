#pragma once

#include "stm32f303xe.h"
#include <array>
#include <cstdint>
#include <functional>
#include <limits>
extern "C"
{
#include "arm_math.h"
}

namespace Fib::Dsp
{
using F32 = float;
using QuadF32 = std::array<F32, 4>;
template <typename Type> using SampleBlock = QuadF32;
template <typename Type, std::size_t numOfBlocks> using SampleBlocks = std::array<SampleBlock<Type>, numOfBlocks>;
} // namespace Fib::Dsp