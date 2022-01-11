#pragma once

#include "dsp/map.hpp"

#include <limits>

namespace Fib::Dsp::Sample
{

/** @brief get the max value for the given bit depth
 *  @note 1-32 bitDepth max ! */
template <std::uint32_t bitDepth> constexpr std::uint32_t getMaxValueOfBitDepth()
{
    return (std::numeric_limits<std::uint32_t>::max() >> (32 - bitDepth));
}

/** @brief get the center value for the given bit depth */
template <std::uint64_t bitDepth> constexpr std::uint64_t bitDepthToCenterValue()
{
    return (getMaxValueOfBitDepth<bitDepth>() / 2);
}

constexpr q31_t floatToQ31(float in)
{
    return (in > 0.99999997f
                ? std::numeric_limits<q31_t>::max()
                : in < -1.0f ? std::numeric_limits<q31_t>::min()
                             : static_cast<q31_t>(in * (static_cast<float>(std::numeric_limits<q31_t>::max()))));
}

/** @brief convert u32 to fixed-point value q31 */
q31_t u32ToQ31(const std::uint32_t &sample);

/** @brief convert fixed-point value q31 to u32 */
template <std::uint32_t sampleBitDepth> q31_t sampleToQ31(const std::uint32_t &sample)
{
    {
        q31_t result;

        result = u32ToQ31(
            Map::absolute<std::uint32_t>(sample, getMaxValueOfBitDepth<sampleBitDepth>(), getMaxValueOfBitDepth<32>()));

        return result;
    }
}

/** @brief convert fixed point q31 to u32 */
std::uint32_t q31ToU32(const q31_t &q31);

/** @brief convert fixed point q31 to some bit-depth raw sample value */
template <std::uint32_t sampleBitDepth> std::uint32_t q31ToSample(const q31_t &q31)
{
    std::uint32_t result;

    result = Map::absolute<std::uint32_t>(q31ToU32(q31), getMaxValueOfBitDepth<32>(),
                                          getMaxValueOfBitDepth<sampleBitDepth>());

    return result;
}

/**  @brief swap uint32 endianness */
std::uint32_t swap(std::uint32_t val);
} // namespace Fib::Dsp::Sample