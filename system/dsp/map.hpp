#pragma once

#include "dsp.hpp"
#include "math.hpp"

namespace Fib::Dsp::Map
{
/** @brief  maps (scales) value from [inMin inMax] relative range to [outMin outMax] relative range */
template <typename T> T relative(const T &in, const T &inMin, const T &inMax, const T &outMin, const T &outMax)
{
    return (in - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

/** @brief scales `zeroBasedIn` value that has upper limit `inMax` to zero based value with upper limit `outMax` */
template <typename T> T absolute(const T &zeroBasedIn, const T &inMax, const T &outMax)
{
    return relative(zeroBasedIn, static_cast<T>(0), inMax, static_cast<T>(0), outMax);
}

/** @brief returns a normalized value (0 to 1) of `in` that ranges from `inMin` to `inMax` */
template <typename T> T normalize(const T &in, const T &inMin, const T &inMax)
{
    return relative(in, inMin, inMax, static_cast<T>(0), static_cast<T>(1));
}

/** @brief returns a value ranging from `outMin` to `outMax` of `inNormal` (0 to 1) value */
template <typename T> T denormalize(const T &inNormal, const T &outMin, const T &outMax)
{
    return relative(inNormal, static_cast<T>(0), static_cast<T>(1), outMin, outMax);
}

} // namespace Fib::Dsp::Map