#pragma once

#include "dsp.hpp"

namespace Fib::Dsp::Math
{
/** @brief  maps (scales) value from [inMin inMax] relative range to [outMin outMax] relative range */
template <typename T> T mapRelative(const T &in, const T &inMin, const T &inMax, const T &outMin, const T &outMax)
{
    return (in - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

/** @brief scales `zeroBasedIn` value that has upper limit `inMax` to zero based value with upper limit `outMax` */
template <typename T> T mapAbsolute(const T &zeroBasedIn, const T &inMax, const T &outMax)
{
    return mapRelative(zeroBasedIn, static_cast<T>(0), inMax, static_cast<T>(0), outMax);
}

/** @brief returns a normalized value (0 to 1) of `in` that ranges from `inMin` to `inMax` */
template <typename T> T normalize(const T &in, const T &inMin, const T &inMax)
{
    return mapRelative(in, inMin, inMax, static_cast<T>(0), static_cast<T>(1));
}

/** @brief returns a value ranging from `outMin` to `outMax` of `inNormal` (0 to 1) value */
template <typename T> T denormalize(const T &inNormal, const T &outMin, const T &outMax)
{
    return mapRelative(inNormal, static_cast<T>(0), static_cast<T>(1), outMin, outMax);
}

QuadF32 secondPower(const QuadF32 &bases);
QuadF32 fourthPower(QuadF32 input);

} // namespace Fib::Dsp::Math