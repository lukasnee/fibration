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

namespace Fib::Dsp {
using U32 = std::uint32_t;
using F32 = float;
using Q31 = q31_t;

static constexpr std::size_t sampleBufferSize = 0x20; // TODO experiment on this one

template <std::size_t size> using BufferU32 = std::array<U32, size>;
template <std::size_t size> using BufferF32 = std::array<F32, size>;
template <std::size_t size> using BufferQ31 = std::array<Q31, size>;

using SampleBufferU32 = BufferU32<sampleBufferSize>;
using SampleBufferF32 = BufferF32<sampleBufferSize>;
using SampleBufferQ31 = BufferQ31<sampleBufferSize>;

struct StereoSampleBufferF32 {
    SampleBufferF32 left, right;
};

struct StereoSampleBufferQ31 {
    SampleBufferQ31 left, right;
};

struct I2sSampleU32 {
    U32 left, right; // may be 24-bit value in 32-bit frame
};

using I2sSampleBufferU32 = std::array<I2sSampleU32, sampleBufferSize>;

template <std::size_t size> void power2(BufferF32<size> &in, BufferF32<size> &out) {
    arm_mult_f32(in.data(), in.data(), out.data(), out.size());
}

template <std::size_t size> void power4(BufferF32<size> &in, BufferF32<size> &out) {
    power2(in, out);
    power2(out, out);
}
// TODO: SampleTime as class with arithmetic operands overrides (parallel value-to-value operations).
} // namespace Fib::Dsp