// Copyright (c)  2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

#pragma once

#include "dsp.hpp"
#include "ln/ln.h"

#include <limits>

namespace Fib::Dsp::Sample {

// TODO unroll these operations in the conversion functions or cache results
// becuase they are called for every sample which hinders performance.
// Implementation is currently inefficiently nested.

__STATIC_FORCEINLINE constexpr U32 maxValueOfBitDepth(U32 bit_depth) {
    // TODO: assert bit_depth is between 1 and 32
    return (std::numeric_limits<U32>::max() >> (32 - bit_depth));
}

__STATIC_FORCEINLINE constexpr U32 centerValueOfBitDepth(U32 bit_depth) {
    return maxValueOfBitDepth(bit_depth) / 2;
}

__STATIC_FORCEINLINE U32 swap(U32 val) { return __REV(val); };

__STATIC_FORCEINLINE constexpr Q31 rawSampleToQ31(const U32 &sample,
                                                  size_t bit_depth) {
    return static_cast<Q31>(sample) -
           static_cast<Q31>(centerValueOfBitDepth(bit_depth));
}

__STATIC_FORCEINLINE constexpr U32 q31ToRawSample(const Q31 &q31,
                                                  size_t bit_depth) {
    return centerValueOfBitDepth(bit_depth) + static_cast<U32>(q31);
}

template <std::size_t size>
void convert(BufferF32<size> &in, BufferQ31<size> &out) {
    // arm_float_to_q31(in.data(), out.data(), out.size());
    Q31 *pDst = out.data();
    uint32_t blockSize = out.size();
    float32_t *pIn = in.data();
    uint32_t blkCnt;
    blkCnt = blockSize >> 2U;
    while (blkCnt > 0U) {
        *pDst++ = static_cast<Q31>(*pIn++ * 2147483648.0f);
        *pDst++ = static_cast<Q31>(*pIn++ * 2147483648.0f);
        *pDst++ = static_cast<Q31>(*pIn++ * 2147483648.0f);
        *pDst++ = static_cast<Q31>(*pIn++ * 2147483648.0f);
        blkCnt--;
    }
    blkCnt = blockSize % 0x4U;
    while (blkCnt > 0U) {
        *pDst++ = static_cast<Q31>(*pIn++ * 2147483648.0f);
        blkCnt--;
    }
}
template <std::size_t size>
void convert(BufferQ31<size> &in, BufferF32<size> &out) {
    arm_q31_to_float(in.data(), out.data(), out.size());
}

constexpr void convert(const I2sSampleBufferU32 &in, StereoSampleBufferQ31 &out,
                       size_t bit_depth) {
    const size_t inverse_bit_depth =
        std::numeric_limits<U32>::digits - bit_depth;
// TODO: maybe I2S peripheral or DMA can swap for us? Or present data in a
// more convenient format
#pragma GCC unroll 4
    for (std::size_t i = 0; i < out.left.size(); i++) {
        out.left[i] =
            rawSampleToQ31(swap(in[i].left) << inverse_bit_depth, bit_depth);
    }
#pragma GCC unroll 4
    for (std::size_t i = 0; i < out.right.size(); i++) {
        out.right[i] =
            rawSampleToQ31(swap(in[i].right) << inverse_bit_depth, bit_depth);
    }
}

constexpr void convert(const StereoSampleBufferQ31 &in, I2sSampleBufferU32 &out,
                       size_t bit_depth) {
    const size_t inverse_bit_depth =
        std::numeric_limits<U32>::digits - bit_depth;
#pragma GCC unroll 4
    for (size_t i = 0; i < in.left.size(); i++) {
        out[i].left =
            swap(q31ToRawSample(in.left[i], bit_depth) >> inverse_bit_depth);
    }
#pragma GCC unroll 4
    for (size_t i = 0; i < in.right.size(); i++) {
        out[i].right =
            swap(q31ToRawSample(in.right[i], bit_depth) >> inverse_bit_depth);
    }
}

inline void convert(const I2sSampleBufferU32 &in, StereoSampleBufferF32 &out,
                    size_t bit_depth) {
    const size_t inverse_bit_depth =
        std::numeric_limits<U32>::digits - bit_depth;
    const auto center = static_cast<int32_t>(centerValueOfBitDepth(bit_depth));
    constexpr float scale = 1.0f / 2147483648.0f;
#pragma GCC unroll 8
    for (std::size_t i = 0; i < out.left.size(); i++) {
        out.left[i] =
            static_cast<float>(
                (static_cast<int32_t>(swap(in[i].left) << inverse_bit_depth)) -
                center) *
            scale;
        out.right[i] =
            static_cast<float>(
                (static_cast<int32_t>(swap(in[i].right) << inverse_bit_depth)) -
                center) *
            scale;
    }
}

inline void convert(StereoSampleBufferF32 &in, I2sSampleBufferU32 &out,
                    size_t bit_depth) {
    const size_t inverse_bit_depth =
        std::numeric_limits<U32>::digits - bit_depth;
    const auto center = static_cast<uint32_t>(centerValueOfBitDepth(bit_depth));
#pragma GCC unroll 8
    for (std::size_t i = 0; i < in.left.size(); i++) {
        out[i].left = swap((center + static_cast<uint32_t>(static_cast<int32_t>(
                                         in.left[i] * 2147483648.0f))) >>
                           inverse_bit_depth);
        out[i].right =
            swap((center + static_cast<uint32_t>(static_cast<int32_t>(
                               in.right[i] * 2147483648.0f))) >>
                 inverse_bit_depth);
    }
}
} // namespace Fib::Dsp::Sample
