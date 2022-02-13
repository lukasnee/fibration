#pragma once

#include "dsp/map.hpp"

#include "logger.hpp"
#include <limits>

namespace Fib::Dsp::Sample {
/** @attention 1-32 bitDepth max ! */
template <U32 bitDepth> constexpr U32 maxValueOfBitDepth() { return (std::numeric_limits<U32>::max() >> (32 - bitDepth)); }

template <U32 bitDepth> constexpr U32 centerValueOfBitDepth() { return (maxValueOfBitDepth<bitDepth>() / 2); }

inline U32 swap(U32 val) { return __REV(val); };

template <std::size_t bitDepth> Q31 rawSampleToQ31(const U32 &sample) {
    if (sample < centerValueOfBitDepth<bitDepth>()) {
        return static_cast<Q31>(sample) - centerValueOfBitDepth<bitDepth>();
    }
    else {
        return static_cast<Q31>(sample - centerValueOfBitDepth<bitDepth>());
    }
}

template <std::size_t bitDepth> U32 q31ToRawSample(const Q31 &q31) {
    if (q31 < 0) {
        return (centerValueOfBitDepth<bitDepth>() - static_cast<U32>(-1 * q31));
    }
    else {
        return (centerValueOfBitDepth<bitDepth>() + static_cast<U32>(q31));
    }
}

template <std::size_t size> void convert(BufferF32<size> &in, BufferQ31<size> &out) {
    // arm_float_to_q31(in.data(), out.data(), out.size());
    q31_t *pDst = out.data();
    uint32_t blockSize = out.size();
    float32_t *pIn = in.data();
    uint32_t blkCnt;
    blkCnt = blockSize >> 2U;
    while (blkCnt > 0U) {
        *pDst++ = (q31_t)(*pIn++ * 2147483648.0f);
        *pDst++ = (q31_t)(*pIn++ * 2147483648.0f);
        *pDst++ = (q31_t)(*pIn++ * 2147483648.0f);
        *pDst++ = (q31_t)(*pIn++ * 2147483648.0f);
        blkCnt--;
    }
    blkCnt = blockSize % 0x4U;
    while (blkCnt > 0U) {
        *pDst++ = (q31_t)(*pIn++ * 2147483648.0f);
        blkCnt--;
    }
}
template <std::size_t size> void convert(BufferQ31<size> &in, BufferF32<size> &out) {
    arm_q31_to_float(in.data(), out.data(), out.size());
}

template <std::size_t bitDepth> void convert(const I2sSampleBufferU32 &from, StereoSampleBufferQ31 &to) {
    constexpr std::size_t frameToSampleBitDiff = std::numeric_limits<U32>::digits - bitDepth;
    for (std::size_t i = 0; i < to.left.size(); i++) {
        to.left[i] = rawSampleToQ31<bitDepth>(swap(from[i].left) << frameToSampleBitDiff);
        to.right[i] = rawSampleToQ31<bitDepth>(swap(from[i].right) << frameToSampleBitDiff);
    }
}

template <std::size_t bitDepth> void convert(const StereoSampleBufferQ31 &from, I2sSampleBufferU32 &to) {
    constexpr std::size_t frameToSampleBitDiff = std::numeric_limits<U32>::digits - bitDepth;
    for (std::size_t i = 0; i < from.left.size(); i++) {
        to[i].left = swap(q31ToRawSample<bitDepth>(from.left[i]) >> frameToSampleBitDiff);
        to[i].right = swap(q31ToRawSample<bitDepth>(from.right[i]) >> frameToSampleBitDiff);
    }
}

template <std::size_t bitDepth> void convert(I2sSampleBufferU32 &from, StereoSampleBufferF32 &to) {
    StereoSampleBufferQ31 tmp;
    convert<bitDepth>(from, tmp);
    convert(tmp.left, to.left);
    convert(tmp.right, to.right);
}

template <std::size_t bitDepth> void convert(StereoSampleBufferF32 &from, I2sSampleBufferU32 &to) {
    StereoSampleBufferQ31 tmp;
    convert(from.left, tmp.left);
    convert(from.right, tmp.right);
    convert<bitDepth>(tmp, to);
    // Logger::log("F32: %f, Q31: %ld, U32: %lu\n", from.left[0], tmp.left[0], to[0].left);
}
} // namespace Fib::Dsp::Sample