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

// TODO: SampleTime as class with arithmetic operands overrides (parallel value-to-value operations). Heres some scrap
// code:
// template <typename Type> SampleBlock<Type> operator+(const SampleBlock<Type> &addend)
// {
//     SampleBlock<Type> sum;
//     for (std::size_t i = 0; i < sum.size(); i++)
//     {
//         sum[i] = (*this)[i] + addend[i];
//     }
//     return sum;
// }

// template <typename Type> SampleBlock<Type> operator+(const SampleBlock<Type> &first) const
// {
//     SampleBlock<Type> sum;
//     for (std::size_t i = 0; i < sum.size(); i++)
//     {
//         sum[i] = (*this)[i] + addend[i];
//     }
//     return point(x + first.x, y + first.y);
// }

} // namespace Fib::Dsp